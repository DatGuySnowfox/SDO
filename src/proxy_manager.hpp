#pragma once
#include "state.hpp"

namespace RC::Unreal {
    class AActor;
    class UWorld;
}

namespace sdb {

// ProxyManager spawns and drives remote-player proxy actors in the UE5 world.
//
// Phase 1: position tracking + actor teleport via K2_SetActorLocationAndRotation.
// Phase 2: actor spawning via UWorld::SpawnActor (needs proxy blueprint class path).
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

    // Returns new AActor* or nullptr; Phase 2 – currently a stub.
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
