#pragma once
#include "protocol.hpp"
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace sdb {

// Per-remote-player state.
struct RemotePlayer {
    uint64_t playerId   = 0;
    float    x = 0, y = 0, z = 0;
    float    yaw        = 0.0f;
    float    aimYaw     = 0.0f;
    float    health     = 100.0f;
    bool     dead       = false;
    uint8_t  movState   = 0;
    uint8_t  animState  = 0;
    void*    proxyActor = nullptr;
    uint64_t updatedUs  = 0;
    uint64_t lastSpawnAttemptUs = 0; // throttles retry after a failed spawn_proxy()
    std::vector<EquipmentSlot> equipment; // last Equipment frame received, for proxy appearance sync
};

// Bridge session context.
struct Session {
    bool     ready       = false;
    Uuid16   sessionId   {};
    Uuid16   worldId     {};
    uint64_t connectionId = 0;
    uint64_t playerId     = 0;
    uint64_t entityId     = 0;
};

// A world entity (ground item, zombie, vehicle, building piece, …).
// Descriptor and state arrive in separate frames; actor spawning is deferred
// until hasPosition is set by the first EntityState frame.
struct WorldEntity {
    uint64_t    entityId      = 0;
    EntityKind  kind          = EntityKind::Unknown;
    uint32_t    revision      = 0;
    uint16_t    quantity      = 0;
    uint64_t    ownerPlayerId = 0;
    std::string classPath;
    std::string itemId;          // FName string, e.g. "DA_AK74"
    bool        hasPosition   = false;
    float       x = 0, y = 0, z = 0, yaw = 0;
    float       health        = 0.0f;
    uint8_t     state         = 0;
    void*       actor         = nullptr;
};

static constexpr int MAX_INV_SLOTS = 40;

// One slot in the local player's inventory.
struct LocalSlot {
    std::string itemId;          // FName string, e.g. "DA_AK74"
    uint16_t    quantity = 0;
    bool        occupied = false;
};

struct BridgeState {
    // Frame counters – incremented atomically by senders.
    std::atomic<uint32_t> seq  {0};
    std::atomic<uint32_t> tick {0};

    // Session context – written once on JoinAccepted, read by all senders.
    std::mutex  sessionMtx;
    Session     session;

    // Latest world-state pushed by the host.
    std::mutex  worldMtx;
    WorldState  worldState;
    bool        worldStateValid = false;

    // Local player lifecycle.
    std::atomic<bool>     hasPawn         {false};
    std::atomic<bool>     sentDeath       {false};
    std::atomic<uint64_t> noPlayerSinceUs {0};

    // Remote player map (keyed by playerId).
    std::mutex                               playersMtx;
    std::unordered_map<uint64_t, RemotePlayer> players;

    // World entity map (keyed by entityId).
    std::mutex                               entityMtx;
    std::unordered_map<uint64_t, WorldEntity> entities;

    // Local player inventory (string itemIds from game FNames).
    std::mutex inventoryMtx;
    LocalSlot  inventory[MAX_INV_SLOTS];
    uint32_t   progressRevision = 0;

    // Local player vitals read from game components (not from server).
    LocalVitals localVitals{};

    // Pending teleport from PlayerProgressRestore; applied on next game tick.
    std::atomic<bool> pendingTeleport{false};
    float teleportX = 0, teleportY = 0, teleportZ = 0, teleportYaw = 0;

    // New-player detection: time when session was latched.
    std::atomic<uint64_t> sessionLatchUs{0};
    // Set when the server sends PlayerProgressRestore (i.e. returning player).
    std::atomic<bool> receivedProgressRestore{false};
    // Set when char creation request has been written to the flag file.
    std::atomic<bool> ccRequestWritten{false};
    // Set when char creation is complete (done JSON was found and consumed).
    std::atomic<bool> ccDone{false};
    // Character data from creation (protected by sessionMtx).
    std::string ccForename;
    std::string ccSurname;
    std::string ccSex;
    std::string ccAge;
    int         ccOccupation{0};
};

inline BridgeState& g_state() {
    static BridgeState s;
    return s;
}

} // namespace sdb
