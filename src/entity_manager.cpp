#include "entity_manager.hpp"
#include "proxy_manager.hpp"
#include "debug_log.hpp"

#include <RC/Unreal/UObjectGlobals.hpp>
#include <RC/Unreal/UObject.hpp>
#include <RC/Unreal/AActor.hpp>
#include <RC/Unreal/UWorld.hpp>
#include <RC/Unreal/UClass.hpp>
#include <RC/DynamicOutput/Output.hpp>

#include <cmath>
#include <unordered_map>
#include <windows.h>

using namespace RC;
using namespace RC::Unreal;

namespace sdb {

// Local copy of mod.cpp's seh_invoke — small and self-contained enough to
// duplicate per-file rather than restructure headers to share, matching
// this codebase's existing pattern for small POD/helper duplication (e.g.
// NativeFTransform). __try/__except can't share a stack frame with C++
// objects that need unwinding (MSVC C2712), hence the free-function
// trampoline shape rather than a lambda.
static bool seh_invoke(void (*fn)(void*), void* ctx)
{
    __try {
        fn(ctx);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

void EntityManager::init()
{
    initialized_ = true;
}

void EntityManager::on_entity_descriptor(const EntityDescriptorData& desc)
{
    // Zombie proxy rendering is unconditionally disabled right now (see
    // spawn_zombie_actor's doc comment) — don't even track these. Tracking-
    // but-never-spawning was found live 2026-08-14 to grow g_state().entities
    // unboundedly over a session (the server's zombie simulation broadcasts
    // continuously; nothing ever removes a "disabled" entry the way a real
    // pickup/despawn would), adding cost to every unrelated scan of this map
    // (get_nearest_entity, tick()'s own iteration) on top of the wasted
    // retry attempts tick() already guards against separately. Remove this
    // guard together with re-enabling spawn_zombie_actor, not before.
    if (desc.kind == EntityKind::Zombie) return;

    std::lock_guard<std::mutex> lk(g_state().entityMtx);

    auto it = g_state().entities.find(desc.entityId);
    if (it != g_state().entities.end()) {
        // Update descriptor fields; preserve position/actor if already set.
        auto& e        = it->second;
        e.kind          = desc.kind;
        e.revision      = desc.revision;
        e.quantity      = desc.quantity;
        e.ownerPlayerId = desc.ownerPlayerId;
        e.classPath     = desc.classPath;
        e.itemId        = desc.itemId;
    } else {
        WorldEntity e;
        e.entityId      = desc.entityId;
        e.kind          = desc.kind;
        e.revision      = desc.revision;
        e.quantity      = desc.quantity;
        e.ownerPlayerId = desc.ownerPlayerId;
        e.classPath     = desc.classPath;
        e.itemId        = desc.itemId;
        // hasPosition stays false; actor spawned after first EntityState.
        g_state().entities[desc.entityId] = std::move(e);
    }

    Output::send<LogLevel::Normal>(
        STR("SDB: entity desc  eid={:d}  kind={:d}  qty={:d}\n"),
        desc.entityId,
        static_cast<int>(desc.kind),
        desc.quantity);
}

void EntityManager::on_entity_state(uint64_t entityId, const EntityStateData& st)
{
    // Same guard as on_entity_descriptor above, for the same reason — state
    // can arrive before the descriptor and would otherwise create a
    // tracked-forever entry via the "not found, create minimal" branch below.
    if (st.kind == EntityKind::Zombie) return;

    std::lock_guard<std::mutex> lk(g_state().entityMtx);

    auto it = g_state().entities.find(entityId);
    if (it == g_state().entities.end()) {
        // State arrived before descriptor — create a minimal entry.
        WorldEntity e;
        e.entityId    = entityId;
        e.kind        = st.kind;
        e.revision    = st.revision;
        e.hasPosition = true;
        e.x = st.x; e.y = st.y; e.z = st.z; e.yaw = st.yaw;
        e.health = st.health; e.state = st.state;
        g_state().entities[entityId] = std::move(e);
        return;
    }

    auto& e    = it->second;
    e.revision = st.revision;
    e.x        = st.x;
    e.y        = st.y;
    e.z        = st.z;
    e.yaw      = st.yaw;
    e.health   = st.health;
    e.state    = st.state;
    e.hasPosition = true;
    // Actor spawning happens in tick() once hasPosition is true.

    // GroundItem/PlacedStructure are placed once and never move again, so
    // this was never needed before — but Zombie entities keep sending
    // EntityState as the server-side simulation moves them
    // (server/src/world/zombie-simulation.js's roam tick), and nothing else
    // in this file re-applies position to an already-spawned actor. Direct
    // teleport, not smoothed — same one-shot K2_SetActorLocationAndRotation
    // call proxy_manager.cpp's teleport_proxy() uses, just without that
    // function's exponential-smoothing layer (real-time player movement
    // needs smoothing to not look teleport-y at ~20Hz; a zombie updating
    // every couple of seconds doesn't hide a direct snap the same way — a
    // reasonable V1 simplification, not an oversight, see the approved plan).
    if (e.kind == EntityKind::Zombie && e.actor) {
        struct TeleportCtx { AActor* actor; double x, y, z, yaw; };
        TeleportCtx ctx{ static_cast<AActor*>(e.actor), e.x, e.y, e.z, e.yaw };
        auto doTeleport = [](void* ctxRaw) {
            auto* c = static_cast<TeleportCtx*>(ctxRaw);
            FVector  loc{ c->x, c->y, c->z };
            FRotator rot{ 0.0, c->yaw, 0.0 };
            FHitResult hit{};
            c->actor->K2_SetActorLocationAndRotation(loc, rot, false, hit, true);
        };
        // SEH-wrapped: e.actor is a raw pointer this mod doesn't fully
        // control the lifetime of once a real AI Controller possesses it
        // (see entity_manager.cpp's disabled spawn_zombie_actor for the
        // full crash history) — if it ever goes dangling between spawn and
        // a later position update, this turns a hard crash into a dropped
        // update instead.
        if (!seh_invoke(doTeleport, &ctx)) {
            debug_log("on_entity_state: SEH caught an exception teleporting zombie eid=" +
                      std::to_string(entityId) + " — actor pointer likely stale, dropping this update");
        }
    }
}

void EntityManager::on_entity_despawn(uint64_t entityId)
{
    std::lock_guard<std::mutex> lk(g_state().entityMtx);

    auto it = g_state().entities.find(entityId);
    if (it == g_state().entities.end()) return;

    destroy_entity_actor(it->second);
    g_state().entities.erase(it);

    Output::send<LogLevel::Normal>(
        STR("SDB: entity despawn  eid={:d}\n"), entityId);
}

void EntityManager::tick(UWorld* world, AActor* /*local_pawn*/)
{
    if (!initialized_) return;

    std::lock_guard<std::mutex> lk(g_state().entityMtx);

    // Zombie proxy rendering is unconditionally disabled right now
    // (spawn_zombie_actor always returns nullptr — see its own doc comment
    // for why). Without this skip, every zombie entity a client ever
    // receives from the server's ongoing simulation stays in this map
    // forever (a "disabled" attempt never succeeds and never gets removed
    // the way a picked-up/despawned entity would) and gets retried every
    // 2s indefinitely — an unbounded, ever-growing per-tick cost over a
    // long session that this project has already been burned by once
    // before for a *transient* failure case (see lastActorAttemptUs's own
    // doc comment in state.hpp, "caused 1-2 FPS"). Found live 2026-08-14:
    // ~7900 retry attempts accumulated over roughly one play session,
    // plausibly contributing to a render-thread freeze that needed a force
    // -close (game logic/this tick loop kept running per debug.log — the
    // freeze was specifically on the render side, consistent with UE5's
    // separate game/render threads and a growing per-tick workload here).
    const uint64_t now = now_micros();
    for (auto& [id, entity] : g_state().entities) {
        if (entity.kind == EntityKind::Zombie) continue;
        if (!entity.actor && entity.hasPosition && world &&
            now - entity.lastActorAttemptUs >= 2'000'000ULL) {
            entity.lastActorAttemptUs = now;
            entity.actor = spawn_entity_actor(world, entity);
        }
    }
}

std::optional<uint64_t> EntityManager::get_nearest_entity(
    float x, float y, float z, float maxDist) const
{
    std::lock_guard<std::mutex> lk(g_state().entityMtx);

    uint64_t bestId   = 0;
    float    bestDist = maxDist * maxDist;

    for (const auto& [id, entity] : g_state().entities) {
        if (!entity.hasPosition) continue;
        const float dx = entity.x - x;
        const float dy = entity.y - y;
        const float dz = entity.z - z;
        const float d2 = dx*dx + dy*dy + dz*dz;
        if (d2 < bestDist) { bestDist = d2; bestId = id; }
    }

    if (bestId == 0) return std::nullopt;
    return bestId;
}

// ── Actor management ──────────────────────────────────────────────────────────
//
// find_and_claim_native_pickup: for an entity the local player owns, the
// game's own single-player drop logic has already spawned a real pickup
// actor near entity.x/y/z — we must not spawn a second one (2026-08-12 live
// test: two items visible on the dropping client, one on everyone else), but
// we still need *some* actor pointer to hand K2_DestroyActor() when the item
// is later picked up by someone else and EntityDespawn arrives, otherwise
// the native actor just sits there forever on the owner's own client
// (2026-08-12: "still see it in p1" after a remote pickup). So instead of
// skipping the entity, find the native actor by exact class + nearest
// position and adopt it as entity.actor without touching its state.
//
// Matches by class *name* via FindAllOf rather than by UClass pointer via
// GetClassPrivate()/ForEachUObject: the vendor UE4SS stub declares those two
// under the wrong mangled names (LoopAction in the real lib lives in
// RC::LoopAction not RC::Unreal::LoopAction, and GetClassPrivate is really
// on UObjectBase not UObject), so they fail to link. GetFullName() is
// confirmed working (used by the ProcessEvent trace), so derive the class's
// short name from it instead — format is "<MetaClass> <Path>.<ClassName>".
static AActor* find_and_claim_native_pickup(UClass* pickupClass, const WorldEntity& entity)
{
    const std::wstring fullName = static_cast<UObject*>(pickupClass)->GetFullName();
    const auto dot = fullName.find_last_of(L'.');
    const std::wstring className = (dot == std::wstring::npos) ? fullName : fullName.substr(dot + 1);
    if (className.empty()) return nullptr;

    std::vector<UObject*> candidates;
    UObjectGlobals::FindAllOf(className.c_str(), candidates);

    AActor* best      = nullptr;
    float   bestDist2 = 500.0f * 500.0f;

    for (UObject* obj : candidates) {
        auto* actor = static_cast<AActor*>(obj);

        // Skip actors another owned entity already claimed, so two drops of
        // the same item type near each other don't both grab the closer one.
        bool claimed = false;
        for (const auto& [otherId, other] : g_state().entities) {
            if (other.entityId != entity.entityId && other.actor == actor) { claimed = true; break; }
        }
        if (claimed) continue;

        const FVector loc = actor->K2_GetActorLocation();
        const float dx = static_cast<float>(loc.X) - entity.x;
        const float dy = static_cast<float>(loc.Y) - entity.y;
        const float dz = static_cast<float>(loc.Z) - entity.z;
        const float d2 = dx*dx + dy*dy + dz*dz;
        if (d2 < bestDist2) { bestDist2 = d2; best = actor; }
    }

    return best;
}

// find_native_vehicle_near: vehicles are static world objects at fixed spawn
// points — every client already has its own native instance of all ~56 of
// them from level load (research/04_ida_investigation_log.md Session 58's
// world-data extraction), so there's nothing to spawn here at all. This
// just locates the existing actor nearest to the server's known spawn-point
// position and adopts it, the exact same technique (and the exact same
// safety profile — no BeginDeferredActorSpawnFromClass, no class
// resolution, no AI-controller concerns) as find_and_claim_native_pickup
// above, deliberately avoiding every risky new technique that caused the
// zombie-proxy crash saga this session. Unlike GroundItem (one exact class
// per item), a vehicle spawn point can hold any of ~17 confirmed vehicle
// Blueprint classes (research pak_all_files.txt, cross-referenced against
// Session 12's "17 vehicle types" finding) — checks all of them since the
// server doesn't currently tell the client which one to expect.
// FIXED 2026-08-14 (live freeze on both test machines, reproducible) — this
// used to call FindAllOf (a full world UObject scan, already documented
// elsewhere in this codebase as expensive enough on its own to cause 1-2
// FPS — see state.hpp's lastActorAttemptUs comment) once per class (17)
// for EVERY vehicle entity, every 2s, until each of the ~56 vehicles
// individually resolved — up to ~950 world scans every 2 seconds. That's
// the same cost class as the documented 1-2 FPS case, just ~56x larger,
// and lines up exactly with a freeze appearing quickly after join (as soon
// as all 56 vehicle entities arrive via replay and start retrying
// concurrently) rather than needing a long session to build up, unlike the
// separate zombie-entity-growth issue found earlier the same night. Fixed
// by scanning each class exactly ONCE per session (cached, matching
// resolve_zombie_archetype_class's own caching pattern) instead of once
// per entity — vehicles are static from level load, so the candidate list
// never needs to change during a session.
static AActor* find_native_vehicle_near(const WorldEntity& entity)
{
    static const wchar_t* kVehicleClasses[] = {
        L"Vehicle_4x4Jeep_C", L"Vehicle_Ambulance_C", L"Vehicle_BigRig_C", L"Vehicle_Buggy_C",
        L"Vehicle_Charger_C", L"Vehicle_DamagedSedan_C", L"Vehicle_FlatbedTruck_C",
        L"Vehicle_GolfCart_C", L"Vehicle_Humvee_C", L"Vehicle_ModifiedPickupUtilityTruck_C",
        L"Vehicle_PickupTruck_C", L"Vehicle_PickupUtilityTruck_C", L"Vehicle_RV_C",
        L"Vehicle_SUV_C", L"Vehicle_SwatVan_C", L"Vehicle_Truck_C", L"Vehicle_Van_C",
    };

    static std::vector<AActor*> s_candidates;
    static bool s_scanned = false;
    if (!s_scanned) {
        s_scanned = true;
        for (const wchar_t* className : kVehicleClasses) {
            std::vector<UObject*> found;
            UObjectGlobals::FindAllOf(className, found);
            for (UObject* obj : found) s_candidates.push_back(static_cast<AActor*>(obj));
        }
        debug_log("find_native_vehicle_near: one-time scan found " +
                  std::to_string(s_candidates.size()) + " native vehicle instances");
    }

    AActor* best      = nullptr;
    float   bestDist2 = 2000.0f * 2000.0f; // spawn points are exact, but allow slack for a vehicle that's drifted/been driven slightly

    for (AActor* actor : s_candidates) {
        bool claimed = false;
        for (const auto& [otherId, other] : g_state().entities) {
            if (other.entityId != entity.entityId && other.actor == actor) { claimed = true; break; }
        }
        if (claimed) continue;

        const FVector loc = actor->K2_GetActorLocation();
        const float dx = static_cast<float>(loc.X) - entity.x;
        const float dy = static_cast<float>(loc.Y) - entity.y;
        const float dz = static_cast<float>(loc.Z) - entity.z;
        const float d2 = dx*dx + dy*dy + dz*dz;
        if (d2 < bestDist2) { bestDist2 = d2; best = actor; }
    }

    return best;
}

// spawn_placed_structure_actor: PlacedStructure's itemId names the same
// UJigsawItem_DataAsset_C GroundItem already resolves (research/
// 04_ida_investigation_log.md Session 58) — but reads BuildActorClass
// (TSubclassOf<AActor> @0x4E8) instead of PickupClass (@0x128), since a
// built piece and a dropped pickup of the same item are different actor
// classes. No BP_JigPickupComponent/PickupBuildFromGround setup afterward —
// those are pickup-specific, not meaningful for a placed structure.
static AActor* spawn_placed_structure_actor(UWorld* world, void* itemAsset, const WorldEntity& entity)
{
    auto* buildClass = *reinterpret_cast<UClass**>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x4E8);
    if (!buildClass) {
        debug_log("spawn_placed_structure_actor: BuildActorClass is null for itemId=" + entity.itemId);
        return nullptr;
    }

    AActor* actor = spawn_actor_at(world, buildClass, entity.x, entity.y, entity.z, entity.yaw);
    if (!actor) {
        debug_log("spawn_placed_structure_actor: spawn_actor_at failed for itemId=" + entity.itemId);
        return nullptr;
    }

    actor->SetActorHiddenInGame(false);
    debug_log("spawn_placed_structure_actor: spawned itemId=" + entity.itemId +
              " eid=" + std::to_string(entity.entityId));
    return actor;
}

// Zombie entities repurpose entity.itemId to carry a plain archetype name
// (e.g. "BP_Zombie_Roamer" — see host-agent.js's zombie-tick broadcast),
// not a real JigsawItem itemId — there's no DataAsset behind it at all, so
// this resolves the class directly via resolve_class_by_name() instead of
// going through resolve_item_asset(). Full package paths from
// research/pak_all_files.txt; only the three archetypes the server-side
// simulation actually spawns (server/src/world/zombie-simulation.js's
// ZONE_KIND_TO_ARCHETYPE) are mapped — anything else falls through to
// nullptr, same "log and bail" shape as every other resolve failure here.
static UClass* resolve_zombie_archetype_class(const std::string& archetype)
{
    static const std::unordered_map<std::string, std::wstring> kArchetypePaths = {
        { "BP_Zombie_Roamer", L"/Game/AI/Zombies/Roamer/BP_Zombie_Roamer.BP_Zombie_Roamer_C" },
        { "BP_Zombie_Dog",    L"/Game/AI/Zombies/Dog/BP_Zombie_Dog.BP_Zombie_Dog_C" },
        { "BP_ZombieBoss",    L"/Game/AI/Zombies/Boss/BP_ZombieBoss.BP_ZombieBoss_C" },
    };
    const auto it = kArchetypePaths.find(archetype);
    if (it == kArchetypePaths.end()) return nullptr;

    // Cache per-archetype — resolve_class_by_name does a live object search,
    // no need to repeat it for every zombie of the same archetype.
    static std::unordered_map<std::string, UClass*> s_cache;
    auto cached = s_cache.find(archetype);
    if (cached != s_cache.end()) return cached->second;

    UClass* cls = resolve_class_by_name(it->second);
    s_cache[archetype] = cls; // cache the miss too — avoids retrying every tick if it's genuinely unresolvable
    return cls;
}

// spawn_zombie_actor — history (2026-08-14, all same live session):
//
// 1. First version included a defensive GetController/UnPossess check
//    (raw single-UObject*-field reflection call) — crashed both clients
//    after 4 successful spawns: EXCEPTION_ACCESS_VIOLATION reading
//    0xFFFFFFFFFFFFFFFF, the exact bit pattern of this file's own
//    ANY_PACKAGE sentinel. Confirmed real auto-possession happens (the
//    thing the check was trying to detect) before crashing.
// 2. Removed that block entirely — crashed AGAIN on the very next live
//    test: PC1 hit the identical 0xFFFFFFFFFFFFFFFF address (proving
//    GetController/UnPossess was never the real root cause), PC2 hit a
//    different, real heap address (0x00000001e31a0b88).
// 3. Re-examined what actually runs on EVERY tick for an already-spawned
//    zombie (not just once at spawn): on_entity_state's teleport call
//    (K2_SetActorLocationAndRotation on entity.actor). Working theory: with
//    the AI Controller left possessing the pawn (since step 2 stopped
//    trying to unpossess it), the zombie's own native AI/behavior tree is
//    genuinely running and could destroy/replace the actor through normal
//    gameplay logic with no involvement from this mod's code at all —
//    leaving entity.actor dangling, and the next position update from the
//    server's roam-movement broadcast calls K2_SetActorLocationAndRotation
//    on freed memory. A real heap address (PC2) and a reused/poisoned
//    region that happened to still contain the -1 sentinel pattern from an
//    earlier resolve_class_by_name call (PC1) are both consistent with a
//    dangling-pointer read, unlike two unrelated bugs.
// 4. This version: SetActorTickEnabled(false) instead of touching the
//    controller at all — a real, already-linked C++ export (same safety
//    category as SetActorHiddenInGame/K2_SetActorLocationAndRotation,
//    already proven safe elsewhere in this file), not a raw-reflection
//    guess. Stops the actor's own tick-driven logic without needing to
//    read/call anything on the controller. NOT YET LIVE-VERIFIED — this is
//    the next thing to test, not a confirmed fix.
// Step 5 (2026-08-14, continued): step 4's SetActorTickEnabled(false) on
// the *pawn* didn't change PC2's crash at all — same exact heap address,
// same failure, even on a fresh process launch. That's the key clue: in UE,
// an AController ticks independently of whatever Pawn it possesses —
// disabling the pawn's own tick does nothing to stop the controller's AI
// logic from continuing to run and (per the working theory in step 3)
// destroy/replace the actor out from under this mod's stored pointer.
//
// Rather than guess at GetController()'s UFUNCTION ABI again (that's what
// crashed the very first attempt), read the Pawn's own `Controller`
// UPROPERTY directly via GetValuePtrByPropertyNameInChain — the same
// reflection-based property-read pattern already proven safe and used
// throughout this codebase (e.g. mod.cpp's find_local_pawn() reading a
// PlayerController's "Pawn"/"AcknowledgedPawn" property the same way, just
// in the opposite direction). This sidesteps every raw-UFunction-params-
// struct guess entirely — the reflection system resolves the real field
// offset itself, it isn't hand-guessed. SetActorTickEnabled is then called
// on the controller through the same real, already-linked, already-proven
// path used on the pawn itself.
// DISABLED 2026-08-14 (step 7, final for tonight) — removing the Controller
// property code entirely (step 6) changed NOTHING: both clients crashed
// again at their own exact same addresses as every prior attempt (PC1:
// 0xFFFFFFFFFFFFFFFF, PC2: 0x00000001e31a0b88). That conclusively rules out
// every piece of controller-handling code across all three fix attempts —
// the real bug is in something present in every version tested tonight,
// most likely resolve_class_by_name's primary FindObject call (Class=nullptr
// means no type filtering at all — it matches ANY object by name, not
// specifically a UClass, and the result is blindly cast; if it ever matches
// a same-named non-class object, using it for spawning would produce
// exactly this kind of delayed, varying-spawn-count corruption) or the
// on_entity_state position-teleport call added this session. Five live
// crashes across three guessed fixes, including one that froze the game and
// required force-killing both the game and a hung IDA session, is well past
// the point of guessing further live. Disabled outright. The proven-stable
// pieces (suppress_zombie_spawners — 857/857 across every single attempt
// tonight, zero issues; the server-side simulation itself, which never
// crashed anything) are untouched and stay enabled. Re-enabling this needs
// a real dedicated debugging session — ideally: (a) type-check
// resolve_class_by_name's FindObject result before casting, and/or (b) SEH-
// wrap the position-teleport call in on_entity_state, and/or (c) a careful
// live IDA attach done BEFORE the crash point (not scrambling to attach
// mid-session) to catch the actual fault with full stack context.
static AActor* spawn_zombie_actor(UWorld* world, const WorldEntity& entity)
{
    (void)world;
    debug_log("spawn_zombie_actor: disabled pending dedicated debugging, archetype=" + entity.itemId);
    return nullptr;
#if 0
    UClass* cls = resolve_zombie_archetype_class(entity.itemId);
    if (!cls) {
        debug_log("spawn_zombie_actor: could not resolve class for archetype=" + entity.itemId);
        return nullptr;
    }

    AActor* actor = spawn_actor_at(world, cls, entity.x, entity.y, entity.z, entity.yaw);
    if (!actor) {
        debug_log("spawn_zombie_actor: spawn_actor_at failed for archetype=" + entity.itemId);
        return nullptr;
    }
    actor->SetActorHiddenInGame(false);
    actor->SetActorTickEnabled(false);

    debug_log("spawn_zombie_actor: spawned archetype=" + entity.itemId +
              " eid=" + std::to_string(entity.entityId));
    return actor;
#endif
}

// spawn_entity_actor: resolve entity.itemId back to its live
// UJigsawItem_DataAsset_C* (same item_asset_cache used for equipment sync,
// see proxy_manager.cpp) and read its PickupClass field (TSubclassOf<AActor>
// @0x0128, research/CXXHeaderDump/JigsawItem_DataAsset.hpp) — the server
// doesn't carry a classPath for loot items (only itemId, see host-agent.js),
// and the client already has the real DataAsset loaded locally, so resolving
// the pickup Blueprint client-side avoids needing an itemId->asset-path
// table on the server at all. PlacedStructure entities share this same
// itemId->DataAsset resolution (see spawn_placed_structure_actor above) —
// only which class field gets read off the result differs.
AActor* EntityManager::spawn_entity_actor(UWorld* world, const WorldEntity& entity)
{
    if (!world) return nullptr;

    // Vehicle has no itemId at all (it isn't a JigsawItem, an archetype
    // name, or anything else this mod resolves a class from) — must be
    // checked before the itemId-empty gate below, which would otherwise
    // reject every vehicle entity outright. See find_native_vehicle_near's
    // doc comment for the full rationale (no spawning at all — every
    // client already has its own native instance of every vehicle from
    // level load, this just finds and adopts the nearest one).
    if (entity.kind == EntityKind::Vehicle)
        return find_native_vehicle_near(entity);

    if (entity.itemId.empty()) return nullptr;

    // Zombie doesn't go through resolve_item_asset at all — entity.itemId
    // is a plain archetype name here, not a real JigsawItem itemId (see
    // spawn_zombie_actor's own doc comment).
    if (entity.kind == EntityKind::Zombie)
        return spawn_zombie_actor(world, entity);

    void* itemAsset = resolve_item_asset(entity.itemId);
    if (!itemAsset) {
        debug_log("spawn_entity_actor: no DataAsset for itemId=" + entity.itemId);
        return nullptr;
    }

    if (entity.kind == EntityKind::PlacedStructure)
        return spawn_placed_structure_actor(world, itemAsset, entity);

    bool isOwnDrop = false;
    {
        uint64_t localPlayerId = 0;
        std::lock_guard<std::mutex> lk(g_state().sessionMtx);
        localPlayerId = g_state().session.playerId;
        isOwnDrop = (entity.ownerPlayerId != 0 && entity.ownerPlayerId == localPlayerId);
    }

    auto* pickupClass = *reinterpret_cast<UClass**>(
        reinterpret_cast<uintptr_t>(itemAsset) + 0x128);
    if (!pickupClass) {
        debug_log("spawn_entity_actor: PickupClass is null for itemId=" + entity.itemId);
        return nullptr;
    }

    if (isOwnDrop) {
        AActor* native = find_and_claim_native_pickup(pickupClass, entity);
        if (native)
            debug_log("spawn_entity_actor: claimed native pickup itemId=" + entity.itemId +
                      " eid=" + std::to_string(entity.entityId));
        return native;
    }

    AActor* actor = spawn_actor_at(world, pickupClass, entity.x, entity.y, entity.z, entity.yaw);
    if (!actor) {
        debug_log("spawn_entity_actor: spawn_actor_at failed for itemId=" + entity.itemId);
        return nullptr;
    }

    actor->SetActorHiddenInGame(false);

    // Same theory as spawn_and_attach_weapon_visual's use of this function:
    // a Blueprint pickup's mesh/collision may need this to configure itself
    // from item data when it wasn't spawned through the game's own "drop to
    // world" flow. Best-effort — call if present, ignore if not.
    UFunction* buildFn = actor->GetFunctionByNameInChain(L"PickupBuildFromGround");
    if (buildFn) actor->ProcessEvent(buildFn, nullptr);

    // Live-tested 2026-08-12: a dropped stack (e.g. 5 cigarettes) only ever
    // showed as a single item on the receiving client — this function never
    // populated the pickup's own BP_JigPickupComponent.ItemDataAsset or
    // called SetCount at all, so it silently kept whatever the CDO's default
    // count is (1). Same reflection-based property lookup
    // proxy_manager.cpp's set_pickup_item_data() already uses successfully
    // (layout-agnostic — works whether this pickup class extends
    // ABP_SkeletalMeshPickup_C or ABP_StaticMeshPickup_C, which sit
    // BP_JigPickupComponent at different fixed offsets), but with the real
    // entity.quantity instead of a hardcoded 1.
    {
        auto** pickupCompSlot = static_cast<UObject**>(
            actor->GetValuePtrByPropertyNameInChain(L"BP_JigPickupComponent"));
        UObject* pickupComp = (pickupCompSlot && *pickupCompSlot) ? *pickupCompSlot : nullptr;
        if (pickupComp) {
            *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(pickupComp) + 0x0A8) = itemAsset;

            UFunction* setCountFn = pickupComp->GetFunctionByNameInChain(L"SetCount");
            if (setCountFn) {
                struct Params { int32_t OverrideCount = 1; } params;
                params.OverrideCount = entity.quantity > 0 ? static_cast<int32_t>(entity.quantity) : 1;
                pickupComp->ProcessEvent(setCountFn, &params);
            }
            debug_log("spawn_entity_actor: set ItemDataAsset + count=" + std::to_string(entity.quantity) +
                      " itemId=" + entity.itemId);
        } else {
            debug_log("spawn_entity_actor: BP_JigPickupComponent not found, quantity not applied itemId=" + entity.itemId);
        }
    }

    debug_log("spawn_entity_actor: spawned itemId=" + entity.itemId +
              " eid=" + std::to_string(entity.entityId));
    return actor;
}

void EntityManager::destroy_entity_actor(WorldEntity& entity)
{
    if (!entity.actor) return;
    static_cast<AActor*>(entity.actor)->K2_DestroyActor();
    entity.actor = nullptr;
}

} // namespace sdb
