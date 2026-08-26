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
    WeaponAttachments     = 43,
    PawnAppearance        = 44,
    PlayMontage           = 45,
    PlayerLights          = 46,
    FirstJoin             = 47,
    WeaponFired           = 48,
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

    // Extended PlayerController stats (gap 4/7, offsets from research
    // Session 32, live-confirmed Session 37 — forename/surname resolved
    // correctly via a plain in-place FString read, no crash). respawnLoc/
    // passive skills intentionally NOT included yet: RespawnLoc is a full
    // FTransform whose internal FQuat/FVector byte layout hasn't been
    // live-verified, and the 10 passive skills are a much larger separate
    // chunk of data — both deferred.
    std::string forename;
    std::string surname;
    int32_t zombieKills           = 0;
    int32_t daysSurvived          = 0;
    int32_t bossZombieKills       = 0;
    int32_t animalKills           = 0;
    int32_t humanKills            = 0;
    float   distanceTravelled     = 0.0f;
    int32_t infestationsDestroyed = 0;
};

// ── ProfileRevision payload (client→server) ───────────────────────────────────
// Per-item wire format: [slotIndex:u8][itemIdLen:u16BE][itemId...][qty:u16BE]
struct InventorySlot {
    uint8_t     slotIndex = 0;   // index within its container, not global
    std::string itemId;          // FName string, e.g. "DA_AK74"
    uint16_t    quantity  = 0;
};

// A single real container from BP_JigMultiplayer_C.MainJigContainers
// (backpack, secure container, etc.) — Columns/Rows are runtime-resizable
// (ExpandContainer), never a fixed constant (research/04_ida_investigation_log.md
// Session 29), so they're carried on the wire instead of assumed. Per-container
// wire format: [columns:u16BE][rows:u16BE][itemCount:u16BE][items...]
struct InventoryContainer {
    uint16_t columns = 0;
    uint16_t rows    = 0;
    std::vector<InventorySlot> items;
};

struct PlayerProgress {
    uint32_t revision = 0;
    float health=1.f, hunger=1.f, thirst=1.f;
    float stamina=1.f, radiation=0.f;
    int32_t level = 0;
    float xp = 0.f;
    float posX=0, posY=0, posZ=0, yaw=0;
    std::vector<InventoryContainer> containers;

    // Extended stats trailer (gap 4/7) — appended after slots on the wire so
    // the original 51-byte header + slot list is untouched; a payload that
    // ends right after the slots (already-persisted pre-gap-4/7 saves) still
    // decodes fine, just with these left at their defaults.
    std::string forename;
    std::string surname;
    int32_t zombieKills           = 0;
    int32_t daysSurvived          = 0;
    int32_t bossZombieKills       = 0;
    int32_t animalKills           = 0;
    int32_t humanKills            = 0;
    float   distanceTravelled     = 0.0f;
    int32_t infestationsDestroyed = 0;
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

// ── WeaponAttachments payload (from BP_JigPickupComponent_C.RepAttachments) ───
// One entry per attachment currently installed on one of the local player's
// equipped weapon slots (11 Primary / 12 Secondary / 13 Sidearm / 14 Melee).
// Flat list (not grouped per weapon) — same style as EquipmentSlot, simpler
// to encode/decode and there are rarely more than a handful of entries.
// Wire format: [tag=1][entryCount:u16BE], per entry:
//   [weaponSlotIndex:u8][containerIndex:u8][itemIdLen:u16BE][itemId...]
struct WeaponAttachmentEntry {
    uint8_t     weaponSlotIndex = 0; // 11-14, which equipped weapon this attachment is on
    uint8_t     containerIndex  = 0; // FS_RepAttachmentInfo.AttachmentContainerIndex
    std::string itemId;              // attachment's own DA_ ItemID, e.g. "HolographicSight"
    // 2026-08-17: only meaningful for toggleable attachments (tactical
    // lights/lasers — ABP_AMainLocalAttachment_C::ActivateState, +0x2C8,
    // shared by every attachment subclass); false/ignored for anything else
    // (mags, scopes, suppressors have no on/off state). Appended after the
    // original fields so old encodings without it still decode (defaults to
    // false) — same forward-compat approach as PlayerProgress's trailer.
    bool        active = false;
};

struct WeaponAttachments {
    std::vector<WeaponAttachmentEntry> entries;
};

// ── PawnAppearance payload (from BP_PlayerCharacter_C's own customization
// fields — IsPlayerMale?/HairMesh/Hair Color/BeardMesh/Beard Color) ───────────
// Asset references are carried as their short object name (e.g.
// "Chr_MaleHair3"), resolved on the receiving end via
// UObjectGlobals::FindObject against already-loaded assets — the small,
// fixed set of character-creation options is always resident in memory once
// any character exists, no on-demand asset loading needed. Empty string =
// not set (e.g. no beard).
// Wire format: [tag=1][isMale:u8][hairMeshLen:u16BE][hairMesh...]
//   [hairColorLen:u16BE][hairColor...][beardMeshLen:u16BE][beardMesh...]
//   [beardColorLen:u16BE][beardColor...]
// bodyPartMeshNames order matches proxy_manager.cpp's kBodyPartOffsets /
// mod.cpp's kBodyPartOffsets exactly: Torso, Biceps, LowerThighs, head, Arms,
// Feet, LowerLegs, Legs, Hands. Needed because just syncing isMale doesn't
// retroactively change which body-shape mesh a proxy (spawned once, at a
// fixed default gender) is using — the actual per-part SkeletalMesh has to
// be synced too, same as hair/beard. Read/matched from the real assigned
// mesh rather than computed from a naming convention: the male variants
// aren't uniformly named (e.g. Biceps is "SK_Chr_Underwear_Male_01_Biceps",
// not "SK_Chr_Male_Biceps").
static constexpr int BODY_PART_COUNT = 9;

struct PawnAppearance {
    bool        isMale = true;
    std::string hairMeshName;
    std::string hairColorName;
    std::string beardMeshName;
    std::string beardColorName;
    std::string skinColorName;
    std::array<std::string, BODY_PART_COUNT> bodyPartMeshNames;
    std::string mouthMeshName;     // BP_PlayerCharacter.hpp Mouth @0x0740, no dedicated color property
    std::string eyebrowsMeshName;  // BP_PlayerCharacter.hpp EyebrowsMesh @0x0790, no dedicated color property
    // Accessory1/2/3 (BP_PlayerCharacter.hpp @0x0758/@0x0750/@0x0748) — three
    // separate face-prop slots (piercings/etc.), confirmed via the real
    // CharacterCreatorMenu Blueprint's AccessoryType1/2/3 functions, same
    // preset-mesh-dropdown mechanism as Hair/Beard/Mouth/Eyebrows.
    std::string accessory1MeshName;
    std::string accessory2MeshName;
    std::string accessory3MeshName;
};

// One-shot montage playback, relayed client-authoritative same as Equipment/
// WeaponAttachments/PawnAppearance (gateway.js just forwards the raw frame,
// no server-side decode). montageName is the asset's short object name
// (e.g. "AM_Melee_Knife_1"), resolved on the receiving end the same way
// itemId strings resolve to a live UObject* via a name-keyed FindAllOf scan
// (see proxy_manager.cpp's item_asset_cache/resolve_item_asset — this reuses
// that exact pattern for the "AnimMontage" class instead).
struct PlayMontageData {
    std::string montageName;
    float       playRate = 1.0f;
};

// ── PlayerLights payload (character-level toggles, not per-item equip
// state — see Equipment's own slot 16 for whether a flashlight item is
// equipped at all) — BP_PlayerCharacter.hpp: FlashlightOn? @0x13E5,
// PlayerUsingNightVision? @0x1401, both plain bools read directly off the
// pawn. Relayed client-authoritative same as Equipment/WeaponAttachments/
// PawnAppearance. Wire format: [tag=1][flashlightOn:u8][nightVisionOn:u8]
// [flashlightIntensity:f32BE]
//
// flashlightIntensity (2026-08-17, added after live-testing SetVisibility
// alone did nothing): ground-truth bytecode decode of FlashlightToggle
// (research/04_ida_investigation_log.md) found the real toggle mechanism is
// ULightComponentBase::SetIntensity(float) — not SetVisibility at all, a
// common UE pattern (visibility stays true, intensity zeroes instead, to
// avoid recreating the render proxy). The ON-path intensity is a *computed*
// value (per-equipped-item, not a bytecode constant), so it's read live off
// the sender's own Flashlight component and carried across rather than
// guessed/hardcoded — only meaningful when flashlightOn is true.
struct PlayerLights {
    bool  flashlightOn        = false;
    bool  nightVisionOn       = false;
    float flashlightIntensity = 0.0f;
};

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

// ItemDropRequest: itemId-based (see server/src/lib/protocol.js decodeItemDropRequest
// for the full rationale — matches by itemId server-side, not a container slot
// index the client can't cleanly reproduce from the RequestDropAsPickup hook).
// Format: [version=1][quantity:u16BE][posX/Y/Z:f32BE][itemIdLen:u16BE][itemId utf8]
std::vector<uint8_t> encode_item_drop_request(const std::string& itemId, uint16_t quantity,
                                               float x, float y, float z);

// InteractionRequest/BUILD payload — itemId-based (see protocol.cpp for
// full rationale). f.entityId is left 0 on this frame (there's no entity
// yet — the server assigns one and replies via EntitySpawn/InteractionResult).
std::vector<uint8_t> encode_interaction_request_build(const std::string& itemId,
                                                        float x, float y, float z, float yaw);

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

std::vector<uint8_t>              encode_weapon_attachments(const WeaponAttachments& a);
std::optional<WeaponAttachments>  decode_weapon_attachments(const uint8_t* p, size_t n);

std::vector<uint8_t>              encode_pawn_appearance(const PawnAppearance& a);
std::optional<PawnAppearance>     decode_pawn_appearance(const uint8_t* p, size_t n);

std::vector<uint8_t>              encode_play_montage(const PlayMontageData& m);
std::optional<PlayMontageData>    decode_play_montage(const uint8_t* p, size_t n);

std::vector<uint8_t>              encode_player_lights(const PlayerLights& l);
std::optional<PlayerLights>       decode_player_lights(const uint8_t* p, size_t n);

uint64_t now_micros();

} // namespace sdb
