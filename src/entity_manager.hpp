#pragma once
#include "state.hpp"
#include <optional>
#include <cstdint>

namespace RC::Unreal {
    class AActor;
    class UWorld;
}

namespace sdb {

// EntityManager spawns and tracks world entities (ground items, zombies, vehicles, …)
// in the UE5 world.  It mirrors the server's authoritative entity list.
//
// Two-phase lifecycle:
//   1. on_entity_descriptor() — stores classPath/itemId/kind; no actor yet (no position).
//   2. on_entity_state()      — stores position/health; triggers actor spawn via tick().
class EntityManager {
public:
    EntityManager() = default;

    void init();

    // Called when the server sends EntitySpawn (descriptor frame).
    void on_entity_descriptor(const EntityDescriptorData& desc);

    // Called when the server sends EntityState (position/health frame).
    void on_entity_state(uint64_t entityId, const EntityStateData& st);

    // Called when the server sends EntityDespawn.
    void on_entity_despawn(uint64_t entityId);

    // Per-frame update; world and local_pawn may be null (entity spawning skipped).
    void tick(RC::Unreal::UWorld* world, RC::Unreal::AActor* local_pawn);

    // Returns the entityId of the nearest entity with a position within maxDist UE units,
    // or nullopt if none found.
    std::optional<uint64_t> get_nearest_entity(float x, float y, float z,
                                                float maxDist = 200.f) const;

private:
    RC::Unreal::AActor* spawn_entity_actor(RC::Unreal::UWorld* world,
                                           const WorldEntity&  entity);
    void destroy_entity_actor(WorldEntity& entity);

    bool initialized_ = false;
};

inline EntityManager& g_entity_manager() {
    static EntityManager em;
    return em;
}

} // namespace sdb
