#include "entity_manager.hpp"

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

    for (auto& [id, entity] : g_state().entities) {
        if (!entity.actor && entity.hasPosition && world)
            entity.actor = spawn_entity_actor(world, entity);
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
// spawn_entity_actor: use entity.classPath to find the blueprint class.
// classPath from the server is the full asset path, e.g.:
//   /Game/Inventory/DataAssets/DA_AK74.DA_AK74_C
// Extract the class name (last component after '.') and call FindFirstOf.
//
// Replace the stub when confirmed working in-game.

AActor* EntityManager::spawn_entity_actor(UWorld* world, const WorldEntity& entity)
{
    (void)world; (void)entity;
    // TODO: extract class name from entity.classPath, FindFirstOf, SpawnActor
    return nullptr;
}

void EntityManager::destroy_entity_actor(WorldEntity& entity)
{
    if (!entity.actor) return;
    static_cast<AActor*>(entity.actor)->K2_DestroyActor();
    entity.actor = nullptr;
}

} // namespace sdb
