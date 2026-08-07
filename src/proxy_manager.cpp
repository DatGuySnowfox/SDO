#include "proxy_manager.hpp"

#include <RC/Unreal/UObjectGlobals.hpp>
#include <RC/Unreal/UObject.hpp>
#include <RC/Unreal/AActor.hpp>
#include <RC/Unreal/UWorld.hpp>
#include <RC/Unreal/UClass.hpp>
#include <RC/DynamicOutput/Output.hpp>

#include <windows.h>

using namespace RC;
using namespace RC::Unreal;

namespace sdb {

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

    const FVector  loc{ static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
    const FRotator rot{ 0.0, static_cast<double>(yaw), 0.0 };
    AActor* actor = world->SpawnActor(s_proxy_class, &loc, &rot);
    if (!actor) {
        Output::send<LogLevel::Warning>(STR("SDB: proxy spawn failed\n"));
        return nullptr;
    }

    Output::send<LogLevel::Normal>(STR("SDB: proxy spawned\n"));
    return actor;
}

void ProxyManager::destroy_proxy(AActor* actor)
{
    if (!actor) return;
    actor->K2_DestroyActor();
}

} // namespace sdb
