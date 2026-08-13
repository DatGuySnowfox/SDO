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

using namespace RC;
using namespace RC::Unreal;

namespace sdb {

void EntityManager::init()
{
    initialized_ = true;
}

void EntityManager::on_entity_descriptor(const EntityDescriptorData& desc)
{
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

    const uint64_t now = now_micros();
    for (auto& [id, entity] : g_state().entities) {
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

// spawn_entity_actor: resolve entity.itemId back to its live
// UJigsawItem_DataAsset_C* (same item_asset_cache used for equipment sync,
// see proxy_manager.cpp) and read its PickupClass field (TSubclassOf<AActor>
// @0x0128, research/CXXHeaderDump/JigsawItem_DataAsset.hpp) — the server
// doesn't carry a classPath for loot items (only itemId, see host-agent.js),
// and the client already has the real DataAsset loaded locally, so resolving
// the pickup Blueprint client-side avoids needing an itemId->asset-path
// table on the server at all.
AActor* EntityManager::spawn_entity_actor(UWorld* world, const WorldEntity& entity)
{
    if (!world || entity.itemId.empty()) return nullptr;

    bool isOwnDrop = false;
    {
        uint64_t localPlayerId = 0;
        std::lock_guard<std::mutex> lk(g_state().sessionMtx);
        localPlayerId = g_state().session.playerId;
        isOwnDrop = (entity.ownerPlayerId != 0 && entity.ownerPlayerId == localPlayerId);
    }

    void* itemAsset = resolve_item_asset(entity.itemId);
    if (!itemAsset) {
        debug_log("spawn_entity_actor: no DataAsset for itemId=" + entity.itemId);
        return nullptr;
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
