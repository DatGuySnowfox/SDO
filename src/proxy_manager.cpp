#include "proxy_manager.hpp"

#include <RC/Unreal/UObjectGlobals.hpp>
#include <RC/Unreal/UObject.hpp>
#include <RC/Unreal/AActor.hpp>
#include <RC/Unreal/UWorld.hpp>
#include <RC/Unreal/UClass.hpp>
#include <RC/DynamicOutput/Output.hpp>

#include <windows.h>
#include <cmath>

using namespace RC;
using namespace RC::Unreal;

namespace sdb {

// UE5 LWC FTransform's real memory layout: three SIMD-aligned 32-byte blocks
// (Rotation quat XYZW, Translation XYZ+pad, Scale3D XYZ+pad), each stored as
// doubles — confirmed via IDA decompile of the engine's own Kismet exec thunk
// for BeginDeferredActorSpawnFromClass, which builds an identical 96-byte
// default-identity transform from three constant xmmwords before Blueprint
// pins override individual fields. Not the vendored SDK's FVector/FRotator —
// those are just X/Y/Z doubles with no matching FTransform type at all.
struct NativeFTransform {
    double rotX = 0.0, rotY = 0.0, rotZ = 0.0, rotW = 1.0;
    double locX = 0.0, locY = 0.0, locZ = 0.0, locPad = 0.0;
    double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0, scalePad = 0.0;
};

// UE4SS.dll's own UWorld::SpawnActor wrapper unconditionally returns nullptr
// on this build — live IDA tracing (research/04_ida_investigation_log.md
// Session 40) followed its real call chain (UWorld::SpawnActor ->
// RC::Unreal::UGameplayStatics::BeginDeferredActorSpawnFromClass) down into
// an internal UE4SS reflection/type-cache lookup (FNV-1a hash + hashtable
// probe) that never resolves, leaving the result permanently null — confirmed
// live for every real spawn_proxy() attempt this session, independent of
// class or world validity. Bypasses that broken wrapper entirely by calling
// the game's own native engine function directly (resolved by RVA, same
// pattern as get_class_private() above). This native
// BeginDeferredActorSpawnFromClass was separately verified via full
// Hex-Rays decompile: it resolves WorldContextObject -> UWorld via
// UEngine::GetWorldFromContextObject, then calls the real, stock
// UWorld::SpawnActor (confirmed 100% vanilla UE5 with no SurrounDead-specific
// gate) with bDeferConstruction=true.
static void* call_begin_deferred_spawn(void* world_context, void* actor_class,
                                        const NativeFTransform* xform)
{
    // (WorldContextObject, ActorClass, SpawnTransform, CollisionHandlingOverride,
    //  Owner, <unidentified trailing byte, always 0 for our use>)
    using Fn = void*(__fastcall*)(void*, void*, const NativeFTransform*, char, void*, char);
    static Fn fn = reinterpret_cast<Fn>(
        reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr)) + 0x2E80E80);
    return fn(world_context, actor_class, xform, 0, nullptr, 0);
}

// AActor::FinishSpawning()'s real native implementation — identified via its
// signature match to UE5 source (a one-time bHasFinishedSpawning-style guard
// flag at offset+92, then a full parent-relative transform composition before
// applying it), found as the callee of the Kismet exec thunk for the
// "Finish Spawning Actor" node. Real UGameplayStatics::FinishSpawningActor
// is just `Actor->FinishSpawning(...); return Actor;` — FinishSpawning
// itself is void in UE5 source, so its return value here is not used.
static void call_finish_spawning(void* actor, const NativeFTransform* xform)
{
    // (this=Actor, SpawnTransform, bIsDefaultTransform, InstanceDataCache, TransformScaleMethod)
    using Fn = void*(__fastcall*)(void*, const NativeFTransform*, char, void*, char);
    static Fn fn = reinterpret_cast<Fn>(
        reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr)) + 0x2AAAB90);
    fn(actor, xform, 0, nullptr, 0);
}

// UObject::GetClassPrivate() in the vendored UE4SS stub header is declared
// on the wrong class for its own mangled name (the header's comment shows
// the true export as `UObjectBase::GetClassPrivate`, but the C++ declaration
// puts it on `UObject`, so the import lib entry the linker generates never
// matches the DLL's actual export and this fails LNK2019). Resolved by
// address instead, same pattern already used in mod.cpp for the two
// RegisterXPreCallback exports that aren't in the stub at all. MSVC returns
// a reference (UClass*&) as a pointer to the referenced storage in RAX, so
// the resolved signature returns UClass** here, not UClass*.
static UClass* get_class_private(UObject* obj)
{
    using Fn = UClass**(__fastcall*)(void*);
    static Fn fn = [] {
        auto* ue4ss = GetModuleHandleW(L"UE4SS.dll");
        return ue4ss ? reinterpret_cast<Fn>(GetProcAddress(ue4ss,
            "?GetClassPrivate@UObjectBase@Unreal@RC@@QEAAAEAPEAVUClass@23@XZ")) : nullptr;
    }();
    if (!fn || !obj) return nullptr;
    UClass** ref = fn(obj);
    return ref ? *ref : nullptr;
}

void ProxyManager::init()
{
    initialized_ = true;
}

void ProxyManager::on_player_connected(uint64_t playerId)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto& p   = g_state().players[playerId];
    p.playerId = playerId;
    Output::send<LogLevel::Normal>(STR("SDB: remote player connected {:d}\n"), playerId);
}

void ProxyManager::on_player_disconnected(uint64_t playerId)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    if (it->second.proxyActor)
        destroy_proxy(static_cast<AActor*>(it->second.proxyActor));

    g_state().players.erase(it);
    Output::send<LogLevel::Normal>(STR("SDB: remote player disconnected {:d}\n"), playerId);
}

void ProxyManager::on_movement(uint64_t playerId, const Movement& m)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    auto& p     = it->second;
    p.x         = m.x;
    p.y         = m.y;
    p.z         = m.z;
    p.yaw       = m.yaw;
    p.aimYaw    = m.aimYaw;
    p.movState  = m.movementState;
    p.animState = m.animationState;
    p.updatedUs = now_micros();
}

void ProxyManager::on_equipment(uint64_t playerId, const Equipment& e)
{
    std::lock_guard<std::mutex> lock(g_state().playersMtx);
    auto it = g_state().players.find(playerId);
    if (it == g_state().players.end()) return;

    // Appearance sync (mesh/anim per slot) is Phase 2 pending proxy actor
    // spawning — for now just cache the latest loadout on the remote player.
    it->second.equipment = e.slots;
}

void ProxyManager::tick(UWorld* world, AActor* /*local_pawn*/)
{
    if (!initialized_) return;

    std::lock_guard<std::mutex> lock(g_state().playersMtx);

    for (auto& [id, player] : g_state().players) {
        if (!player.proxyActor) {
            // Retry at most every 5s on failure — do_game_tick() calls
            // tick() many times per second, and a failing spawn_proxy() with
            // no cooldown here was a real bug (Session 36): every failed
            // SpawnActor call on every tick tanked the frame rate even
            // though each individual call failed cleanly rather than crashing.
            const uint64_t now = now_micros();
            if (world && now - player.lastSpawnAttemptUs >= 5'000'000ULL) {
                player.lastSpawnAttemptUs = now;
                player.proxyActor = spawn_proxy(world,
                                                player.x, player.y, player.z,
                                                player.yaw);
            }
            continue;
        }

        if (!player.dead)
            teleport_proxy(static_cast<AActor*>(player.proxyActor),
                           player.x, player.y, player.z, player.yaw);
    }
}

void ProxyManager::teleport_proxy(AActor* actor, float x, float y, float z, float yaw)
{
    if (!actor) return;
    FVector  loc{ static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
    FRotator rot{ 0.0, static_cast<double>(yaw), 0.0 };
    FHitResult hit{};
    actor->K2_SetActorLocationAndRotation(loc, rot, false, hit, true);
}

AActor* ProxyManager::spawn_proxy(UWorld* world, float x, float y, float z, float yaw)
{
    if (!world) return nullptr;

    // NOT WORKING YET (Session 36): both BP_PlayerCharacter_C and BP_Zombie_C
    // were rejected by SpawnActor 100% of the time in live testing (no crash,
    // clean nullptr every call). Zombies are definitely not CLASS_NotPlaceable
    // — the game's own spawner places them constantly — so that rules out the
    // leading hypothesis from Session 5's SpawnActor validation checklist and
    // points at something more fundamental in how this UE4SS SpawnActor
    // wrapper is being called, not which class is targeted. Needs IDA-level
    // investigation of the actual export before another live attempt is
    // worth making — see research/04_ida_investigation_log.md Session 36.
    // Left targeting BP_PlayerCharacter_C (the real desired proxy visual)
    // since the class choice isn't the blocker.
    static UClass* s_proxy_class = nullptr;
    if (!s_proxy_class) {
        UObject* obj = UObjectGlobals::FindFirstOf(STR("BP_PlayerCharacter_C"));
        if (obj) s_proxy_class = get_class_private(obj);
    }
    if (!s_proxy_class) return nullptr;

    // Pitch/roll are 0, so the FRotator->FQuat conversion collapses to a pure
    // Z-axis rotation: (0, 0, sin(yaw/2), cos(yaw/2)).
    const double yawRad = static_cast<double>(yaw) * (3.14159265358979323846 / 180.0);
    NativeFTransform xform;
    xform.rotZ = std::sin(yawRad * 0.5);
    xform.rotW = std::cos(yawRad * 0.5);
    xform.locX = static_cast<double>(x);
    xform.locY = static_cast<double>(y);
    xform.locZ = static_cast<double>(z);

    // world doubles as its own WorldContextObject: UWorld::GetWorld() (the
    // virtual call BeginDeferredActorSpawnFromClass resolves it through)
    // trivially returns `this` — confirmed live during the same trace.
    void* pending = call_begin_deferred_spawn(world, s_proxy_class, &xform);
    if (!pending) {
        Output::send<LogLevel::Warning>(STR("SDB: proxy spawn failed\n"));
        return nullptr;
    }

    call_finish_spawning(pending, &xform);

    // BP_PlayerCharacter_C's own death handling (loot-crate spawn, death-location
    // UI, input lock) turns out to be keyed off any instance's health reaching
    // zero, not off which instance is actually possessed by the local
    // PlayerController — the Blueprint was never designed to have a second,
    // locally-spawned instance coexisting. Live-confirmed 2026-08-10: killing a
    // proxy actor triggered the real player's own death sequence (their loot
    // crate, their death-location marker) while their own pawn remained
    // standing, frozen. The proxy is purely cosmetic and never needs to take
    // damage, so disabling its collision prevents any damage trace from ever
    // landing on it in the first place, sidestepping the whole class of bug
    // rather than trying to patch the Blueprint's death logic itself.
    static_cast<AActor*>(pending)->SetActorEnableCollision(false);

    Output::send<LogLevel::Normal>(STR("SDB: proxy spawned\n"));
    return static_cast<AActor*>(pending);
}

void ProxyManager::destroy_proxy(AActor* actor)
{
    if (!actor) return;
    actor->K2_DestroyActor();
}

} // namespace sdb
