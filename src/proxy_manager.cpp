#include "proxy_manager.hpp"

#include <RC/Unreal/UObjectGlobals.hpp>
#include <RC/Unreal/UObject.hpp>
#include <RC/Unreal/AActor.hpp>
#include <RC/Unreal/UWorld.hpp>
#include <RC/Unreal/UClass.hpp>
#include <RC/DynamicOutput/Output.hpp>

using namespace RC;
using namespace RC::Unreal;

namespace sdb {

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

void ProxyManager::tick(UWorld* world, AActor* /*local_pawn*/)
{
    if (!initialized_) return;

    std::lock_guard<std::mutex> lock(g_state().playersMtx);

    for (auto& [id, player] : g_state().players) {
        if (!player.proxyActor) {
            if (world)
                player.proxyActor = spawn_proxy(world,
                                                player.x, player.y, player.z,
                                                player.yaw);
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

AActor* ProxyManager::spawn_proxy(UWorld* world,
                                  float /*x*/, float /*y*/, float /*z*/,
                                  float /*yaw*/)
{
    // Phase 2: find the proxy blueprint class once, then call SpawnActor.
    //
    // When the proxy class path is known (from UE4SS object browser), replace
    // this stub with:
    //   static UClass* proxy_class = nullptr;
    //   if (!proxy_class) {
    //       UObject* obj = UObjectGlobals::FindFirstOf(STR("BP_RemoteProxy_C"));
    //       if (obj) proxy_class = obj->GetClassPrivate();
    //   }
    //   if (!proxy_class) return nullptr;
    //   FVector  loc{x, y, z};
    //   FRotator rot{0.0, yaw, 0.0};
    //   return world->SpawnActor(proxy_class, &loc, &rot);
    (void)world;
    return nullptr;
}

void ProxyManager::destroy_proxy(AActor* actor)
{
    if (!actor) return;
    actor->K2_DestroyActor();
}

} // namespace sdb
