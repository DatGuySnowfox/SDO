#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sdb {

static constexpr uint32_t FRAME_MAGIC       = 0x53444F33u; // "SDO3"
static constexpr uint16_t FRAME_VERSION     = 3;
static constexpr uint16_t FRAME_HEADER_SIZE = 88;
static constexpr uint32_t FRAME_MAX_PAYLOAD = 64u * 1024u;
static constexpr uint32_t FRAME_MAX_TOTAL   = FRAME_HEADER_SIZE + FRAME_MAX_PAYLOAD;

enum class MsgType : uint16_t {
    HostAuthenticate      = 1,
    ClientAuthenticate    = 2,
    AuthenticationOk      = 3,
    AuthenticationFail    = 4,
    HostHeartbeat         = 5,
    ClientHeartbeat       = 6,
    JoinRequest           = 10,
    JoinAccepted          = 11,
    JoinRejected          = 12,
    PlayerConnected       = 13,
    PlayerDisconnected    = 14,
    Movement              = 20,
    Equipment             = 21,
    Death                 = 22,
    Respawn               = 23,
    ProfileRevision       = 24,
    SaveAck               = 25,
    InteractionRequest    = 26,
    InteractionResult     = 27,
    DeathRequest          = 28,
    RespawnRequest        = 29,
    WorldState            = 30,
    EntitySpawn           = 31,
    EntityState           = 32,
    EntityDespawn         = 33,
    ItemDropRequest       = 34,
    ItemPickupRequest     = 35,
    ItemPickupResult      = 36,
    ZombieAttackRequest   = 37,
    ZombieDamageResult    = 38,
    ItemDropResult        = 39,
    PlayerDamage          = 40,
    CharacterCreate       = 41,
    PlayerProgressRestore = 42,
    Error                 = 255,
};

// Aligned to JS WorldEntityKind values.
enum class EntityKind : uint8_t {
    Unknown         = 0,
    Zombie          = 1,
    GroundItem      = 2,
    Vehicle         = 3,
    PlacedStructure = 4,
};

enum class InteractionType : uint8_t {
    BUILD = 1,
    LOOT  = 2,
    USE   = 3,
};

using Uuid16 = std::array<uint8_t, 16>;

struct Frame {
    MsgType  type{};
    uint16_t flags        = 0;
    uint64_t connectionId = 0;
    Uuid16   sessionId{};
    Uuid16   worldId{};
    uint64_t playerId     = 0;
    uint64_t entityId     = 0;
    uint32_t sequence     = 0;
    uint32_t tick         = 0;
    uint64_t timestampUs  = 0;
    std::vector<uint8_t> payload;
};

// ── Fixed-format payloads (wire formats confirmed correct) ────────────────────

struct Movement {
    float x, y, z;
    float yaw, aimYaw;
    float velocityX, velocityY, velocityZ;
    float movementDirection;
    uint8_t movementState  = 0;
    uint8_t aimState       = 0;
    uint8_t animationState = 0;
};
static constexpr int MOVEMENT_PAYLOAD_SIZE = 39;

struct WorldState {
    uint32_t revision    = 0;
    uint64_t authorityMs = 0;
    float timeOfDay=0, rain=0, snow=0, fog=0, cloudCover=0, wind=0, thunder=0;
};

struct PlayerDamage {
    float    current  = 0.0f;
    float    maximum  = 0.0f;
    uint32_t revision = 0;
};

// ── Entity descriptor (from EntitySpawn frame payload) ────────────────────────
// Wire format: [tag=1][kind:u8][revision:u32BE][qty:u16BE][ownerPlayerId:u64BE]
//              [classPathLen:u16BE][classPath...][itemIdLen:u16BE][itemId...]
struct EntityDescriptorData {
    uint64_t    entityId      = 0;  // filled from frame header
    EntityKind  kind          = EntityKind::Unknown;
    uint32_t    revision      = 0;
    uint16_t    quantity      = 0;
    uint64_t    ownerPlayerId = 0;
    std::string classPath;
    std::string itemId;             // FName string, e.g. "DA_AK74"
};

// ── Entity state (from EntityState frame payload, exactly 27 bytes) ───────────
// Wire format: [tag=1][kind:u8][revision:u32BE][x/y/z/yaw/health:5×f32BE][state:u8]
struct EntityStateData {
    uint64_t   entityId = 0;    // filled from frame header
    EntityKind kind     = EntityKind::Unknown;
    uint32_t   revision = 0;
    float      x = 0, y = 0, z = 0, yaw = 0;
    float      health   = 0.0f;
    uint8_t    state    = 0;
};
static constexpr int ENTITY_STATE_PAYLOAD_SIZE = 27;

// ── Local player vitals (read from game components, not from server) ──────────
struct LocalVitals {
    double  health    = 0.0;
    double  hunger    = 0.0;
    double  thirst    = 0.0;
    double  stamina   = 0.0;
    double  radiation = 0.0;
    int32_t level     = 0;
    double  xp        = 0.0;
};

// ── ProfileRevision payload (client→server) ───────────────────────────────────
// Per-slot wire format: [slotIndex:u8][itemIdLen:u16BE][itemId...][qty:u16BE]
struct InventorySlot {
    uint8_t     slotIndex = 0;
    std::string itemId;          // FName string, e.g. "DA_AK74"
    uint16_t    quantity  = 0;
};

struct PlayerProgress {
    uint32_t revision = 0;
    float health=1.f, hunger=1.f, thirst=1.f;
    float stamina=1.f, radiation=0.f;
    int32_t level = 0;
    float xp = 0.f;
    float posX=0, posY=0, posZ=0, yaw=0;
    std::vector<InventorySlot> slots;
};

// ── Equipment payload (from BP_JigHelperComp_C.ServerEquippedItems) ───────────
// slotIndex is a fixed 0..20 index over the 21 equipment slots in declaration
// order (Facewear, Headwear, Eyewear, Accessory, Torso, Gloves, Legs, Feet,
// Container, BodyArmor, Backpack, Primary, Secondary, Sidearm, Melee,
// Throwable, Flashlight, Binoculars, GPS, Compass, FishingRod).
// Wire format: [tag=1][slotCount:u16BE], per slot [slotIndex:u8][itemIdLen:u16BE][itemId...]
struct EquipmentSlot {
    uint8_t     slotIndex = 0;   // 0..20
    std::string itemId;          // DA_ name, e.g. "DA_AK74"; empty = unequipped
};

struct Equipment {
    std::vector<EquipmentSlot> slots;
};

static constexpr int EQUIPMENT_SLOT_COUNT = 21;

// ── Encode / decode ───────────────────────────────────────────────────────────

int  encode_frame(uint8_t* buf, int cap, const Frame& f, uint32_t& seq, uint32_t& tck);
void encode_movement(const Movement& m, uint8_t out[MOVEMENT_PAYLOAD_SIZE]);

std::optional<Frame>        decode_frame(const uint8_t* data, int len);
std::optional<Movement>     decode_movement(const uint8_t* payload, int len);
std::optional<WorldState>   decode_world_state(const uint8_t* payload, int len);
std::optional<PlayerDamage> decode_player_damage(const uint8_t* payload, int len);

// Entity frames (new formats from JS shared-protocol)
std::optional<EntityDescriptorData> decode_entity_descriptor(const uint8_t* p, size_t n);
std::optional<EntityStateData>      decode_entity_state(const uint8_t* p, size_t n);

// World-action JSON codec: uint16BE length + UTF-8 JSON (no tag byte)
std::vector<uint8_t>       encode_world_action(const std::string& json);
std::optional<std::string> decode_world_action(const uint8_t* p, size_t n);

// Flat JSON field extraction for result payloads
std::string json_str(const std::string& json, const std::string& key);
bool        json_bool(const std::string& json, const std::string& key);
double      json_double(const std::string& json, const std::string& key);

// Unique request ID matching JS validation /^[a-zA-Z0-9._:-]{1,80}$/
std::string next_request_id();

// ProfileRevision encode/decode (inventory slot itemId encoded as length-prefixed
// string). Shared format for both ProfileRevision (client→server) and
// PlayerProgressRestore (server→client, replays the last-saved ProfileRevision
// payload verbatim) — decode_player_progress must be used for both, not
// decode_movement.
std::vector<uint8_t>            encode_player_progress(const PlayerProgress& p);
std::optional<PlayerProgress>   decode_player_progress(const uint8_t* p, size_t n);

std::vector<uint8_t>            encode_equipment(const Equipment& e);
std::optional<Equipment>        decode_equipment(const uint8_t* p, size_t n);

uint64_t now_micros();

} // namespace sdb
