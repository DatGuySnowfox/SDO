#pragma once
#include "state.hpp"

namespace RC::Unreal {
    class AActor;
    class UWorld;
}

namespace sdb {

// ProxyManager spawns and drives remote-player proxy actors in the UE5 world.
//
// Position tracking + actor teleport via K2_SetActorLocationAndRotation, and
// actor spawning via UWorld::SpawnActor. No dedicated proxy Blueprint exists
// in the game (research/04_ida_investigation_log.md Session 12) — spawns
// BP_PlayerCharacter_C itself, which gets the right mesh/animations for
// free; since nothing possesses it with a PlayerController, it doesn't
// process input and stays wherever teleport_proxy() puts it.
class ProxyManager {
public:
    ProxyManager() = default;
    ~ProxyManager() = default;

    void init();

    void on_player_connected(uint64_t playerId);
    void on_player_disconnected(uint64_t playerId);
    void on_movement(uint64_t playerId, const Movement& m);
    void on_equipment(uint64_t playerId, const Equipment& e);

    // Per-frame update – world and local_pawn may be null (proxies are skipped).
    void tick(RC::Unreal::UWorld* world, RC::Unreal::AActor* local_pawn);

private:
    void teleport_proxy(RC::Unreal::AActor* actor,
                        float x, float y, float z, float yaw);

    // Returns new AActor* or nullptr on failure.
    RC::Unreal::AActor* spawn_proxy(RC::Unreal::UWorld* world,
                                    float x, float y, float z, float yaw);

    void destroy_proxy(RC::Unreal::AActor* actor);

    bool initialized_ = false;
};

inline ProxyManager& g_proxy_manager() {
    static ProxyManager pm;
    return pm;
}

} // namespace sdb
