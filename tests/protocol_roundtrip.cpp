// protocol_roundtrip.cpp — standalone, dependency-free round-trip test for
// src/protocol.hpp / src/protocol.cpp.
//
// protocol.cpp has no UE4SS/UE5 dependency (only <array>/<cstdint>/<optional>/
// <string>/<vector> plus <Windows.h> for now_micros()), so it — and this test
// — build and run completely standalone, no game or mod DLL involved.
//
// What this covers, per MsgType (see src/protocol.hpp MsgType enum, ~30
// values):
//
//  - Types with a real, symmetric encode+decode pair in protocol.cpp are
//    round-tripped through those exact functions:
//      Movement, ProfileRevision/PlayerProgressRestore (PlayerProgress),
//      Equipment, WeaponAttachments, PawnAppearance, and the generic
//      world-action JSON envelope (used by CharacterCreate, InteractionRequest,
//      InteractionResult, ItemPickupResult, ItemDropResult).
//
//  - Types where protocol.cpp only implements ONE side (the other side is
//    implemented in JS, server/src/lib/protocol.js) are round-tripped against
//    a small mirror function written in *this test file* from the documented
//    wire format in protocol.hpp's comments, so the C++ function under test
//    is still fully exercised end-to-end:
//      WorldState       (decode_world_state only  -> test-local encoder)
//      PlayerDamage     (decode_player_damage only -> test-local encoder)
//      EntitySpawn      (decode_entity_descriptor only -> test-local encoder)
//      EntityState      (decode_entity_state only -> test-local encoder)
//      ItemDropRequest  (encode_item_drop_request only -> test-local decoder)
//
//  - Types that carry no dedicated payload struct at all (pure Frame-header
//    messages: auth/heartbeat/join/connect-disconnect/death-respawn/etc.) are
//    round-tripped through encode_frame/decode_frame directly, since the frame
//    header *is* the entire message for these.
//
// Every field is checked with a value chosen to catch endianness, sign, and
// truncation bugs (negative floats, values with high bits set, non-ASCII-safe
// but valid UTF-8 strings, near-uint16 boundary lengths, etc.) — not all-zero
// data.

#include "../src/protocol.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>

using namespace sdb;

// ---------------------------------------------------------------------------
// Tiny assertion engine (mirrors tests/integration.js's ok() style)
// ---------------------------------------------------------------------------

static int g_total = 0;
static int g_failures = 0;

static void ok(bool cond, const std::string& label) {
    g_total++;
    if (cond) {
        std::printf("  PASS  %s\n", label.c_str());
    } else {
        std::printf("  FAIL  %s\n", label.c_str());
        g_failures++;
    }
}

static bool feq(float a, float b) {
    // Exact equality is fine here: all values round-trip through the exact
    // same IEEE-754 bit pattern (memcpy bit-cast), no arithmetic involved.
    return std::memcmp(&a, &b, sizeof(float)) == 0;
}

// ---------------------------------------------------------------------------
// Local big-endian helpers for building/parsing mirror (test-only) payloads
// for the asymmetric message types. Deliberately re-implemented here (not
// reusing protocol.cpp's static w16/w32/w64/r16/r32/r64) so this test does
// not silently pass by sharing a buggy helper with the code under test.
// ---------------------------------------------------------------------------

static void put_u16(std::vector<uint8_t>& b, uint16_t v) {
    b.push_back(static_cast<uint8_t>((v >> 8) & 0xff));
    b.push_back(static_cast<uint8_t>(v & 0xff));
}
static void put_u32(std::vector<uint8_t>& b, uint32_t v) {
    b.push_back(static_cast<uint8_t>((v >> 24) & 0xff));
    b.push_back(static_cast<uint8_t>((v >> 16) & 0xff));
    b.push_back(static_cast<uint8_t>((v >> 8) & 0xff));
    b.push_back(static_cast<uint8_t>(v & 0xff));
}
static void put_u64(std::vector<uint8_t>& b, uint64_t v) {
    put_u32(b, static_cast<uint32_t>(v >> 32));
    put_u32(b, static_cast<uint32_t>(v));
}
static void put_f32(std::vector<uint8_t>& b, float f) {
    uint32_t u;
    std::memcpy(&u, &f, 4);
    put_u32(b, u);
}
static void put_str16(std::vector<uint8_t>& b, const std::string& s) {
    put_u16(b, static_cast<uint16_t>(s.size()));
    b.insert(b.end(), s.begin(), s.end());
}

static uint16_t get_u16(const uint8_t* p) {
    return static_cast<uint16_t>((p[0] << 8) | p[1]);
}
static uint32_t get_u32(const uint8_t* p) {
    return (static_cast<uint32_t>(p[0]) << 24) | (static_cast<uint32_t>(p[1]) << 16)
         | (static_cast<uint32_t>(p[2]) << 8)  |  static_cast<uint32_t>(p[3]);
}
static float get_f32(const uint8_t* p) {
    uint32_t u = get_u32(p);
    float f;
    std::memcpy(&f, &u, 4);
    return f;
}

// ---------------------------------------------------------------------------
// 1. Frame envelope — encode_frame / decode_frame
//    Exercises every header field plus arbitrary payload bytes, and is also
//    the sole round-trip coverage for the pure header-only MsgTypes (no
//    dedicated payload struct exists for these in protocol.cpp):
//    HostAuthenticate, ClientAuthenticate, AuthenticationOk,
//    AuthenticationFail, HostHeartbeat, ClientHeartbeat, JoinRequest,
//    JoinAccepted, JoinRejected, PlayerConnected, PlayerDisconnected, Death,
//    Respawn, SaveAck, InteractionRequest, DeathRequest, RespawnRequest,
//    EntityDespawn, ItemPickupRequest, ZombieAttackRequest,
//    ZombieDamageResult, Error.
// ---------------------------------------------------------------------------

static void test_frame_roundtrip(MsgType type, const std::string& label,
                                  const std::vector<uint8_t>& payload) {
    Frame f;
    f.type = type;
    f.flags = 0xBEEF;
    f.connectionId = 0x0102030405060708ULL;
    for (int i = 0; i < 16; ++i) f.sessionId[i] = static_cast<uint8_t>(0xA0 + i);
    for (int i = 0; i < 16; ++i) f.worldId[i]   = static_cast<uint8_t>(0x50 + i);
    f.playerId    = 0xFFEEDDCCBBAA9988ULL;   // high bit set — catches sign bugs
    f.entityId    = 0x1122334455667799ULL;
    f.timestampUs = 0x0011223344556677ULL;
    f.payload     = payload;

    uint8_t buf[FRAME_MAX_TOTAL];
    uint32_t seq = 41, tick = 999;
    const int n = encode_frame(buf, sizeof(buf), f, seq, tick);
    ok(n == static_cast<int>(FRAME_HEADER_SIZE + payload.size()),
       label + ": encode_frame returned expected length");
    ok(seq == 42, label + ": encode_frame increments seq (41 -> 42)");
    ok(tick == 1000, label + ": encode_frame increments tick (999 -> 1000)");

    auto decoded = decode_frame(buf, n);
    ok(decoded.has_value(), label + ": decode_frame succeeds");
    if (!decoded) return;

    ok(decoded->type == type, label + ": type round-trips");
    ok(decoded->flags == f.flags, label + ": flags round-trips");
    ok(decoded->connectionId == f.connectionId, label + ": connectionId round-trips");
    ok(decoded->sessionId == f.sessionId, label + ": sessionId round-trips");
    ok(decoded->worldId == f.worldId, label + ": worldId round-trips");
    ok(decoded->playerId == f.playerId, label + ": playerId (high-bit set) round-trips");
    ok(decoded->entityId == f.entityId, label + ": entityId round-trips");
    ok(decoded->sequence == 42, label + ": sequence round-trips as written by encode_frame");
    ok(decoded->tick == 1000, label + ": tick round-trips as written by encode_frame");
    ok(decoded->timestampUs == f.timestampUs, label + ": timestampUs round-trips (non-zero, so not overwritten by now_micros())");
    ok(decoded->payload == payload, label + ": payload bytes round-trip exactly");
}

static void test_frame_envelope_all_header_only_types() {
    std::printf("\n-- Frame envelope (encode_frame/decode_frame) --\n");

    // A representative arbitrary payload for exercising payload passthrough
    // where a type happens to carry ad-hoc bytes not covered by a struct.
    const std::vector<uint8_t> rawPickupPayload = { 1, 0xFF }; // ItemPickupRequest's actual wire shape
    std::vector<uint8_t> joinPayload(64, 0);
    joinPayload[0] = 'A'; joinPayload[1] = 'x'; // non-trivial display-name bytes

    test_frame_roundtrip(MsgType::HostAuthenticate,   "HostAuthenticate",   {});
    test_frame_roundtrip(MsgType::ClientAuthenticate,  "ClientAuthenticate", {0x00, 0x05, 't','i','c','k','e'});
    test_frame_roundtrip(MsgType::AuthenticationOk,    "AuthenticationOk",   {});
    test_frame_roundtrip(MsgType::AuthenticationFail,  "AuthenticationFail", {});
    test_frame_roundtrip(MsgType::HostHeartbeat,       "HostHeartbeat",      {});
    test_frame_roundtrip(MsgType::ClientHeartbeat,     "ClientHeartbeat",    {});
    test_frame_roundtrip(MsgType::JoinRequest,         "JoinRequest",        joinPayload);
    test_frame_roundtrip(MsgType::JoinAccepted,        "JoinAccepted",       {});
    test_frame_roundtrip(MsgType::JoinRejected,        "JoinRejected",       {});
    test_frame_roundtrip(MsgType::PlayerConnected,     "PlayerConnected",    {});
    test_frame_roundtrip(MsgType::PlayerDisconnected,  "PlayerDisconnected", {});
    test_frame_roundtrip(MsgType::Death,               "Death",              {});
    test_frame_roundtrip(MsgType::Respawn,              "Respawn",            {});
    test_frame_roundtrip(MsgType::SaveAck,              "SaveAck",            {});
    test_frame_roundtrip(MsgType::InteractionRequest,   "InteractionRequest (raw payload passthrough)", {'{','}'});
    test_frame_roundtrip(MsgType::DeathRequest,         "DeathRequest",       {});
    test_frame_roundtrip(MsgType::RespawnRequest,       "RespawnRequest",     {});
    test_frame_roundtrip(MsgType::EntityDespawn,        "EntityDespawn",      {});
    test_frame_roundtrip(MsgType::ItemPickupRequest,    "ItemPickupRequest",  rawPickupPayload);
    test_frame_roundtrip(MsgType::ZombieAttackRequest,  "ZombieAttackRequest", {});
    test_frame_roundtrip(MsgType::ZombieDamageResult,   "ZombieDamageResult", {});
    test_frame_roundtrip(MsgType::Error,                "Error",              {'e','r','r'});

    // Empty-payload frame (payload_len == 0 path).
    {
        Frame f;
        f.type = MsgType::ClientHeartbeat;
        uint8_t buf[FRAME_HEADER_SIZE];
        uint32_t seq = 0, tick = 0;
        const int n = encode_frame(buf, sizeof(buf), f, seq, tick);
        ok(n == static_cast<int>(FRAME_HEADER_SIZE), "zero-payload frame: encode_frame length == header size");
        auto d = decode_frame(buf, n);
        ok(d.has_value() && d->payload.empty(), "zero-payload frame: decode_frame yields empty payload");
    }

    // Oversized-payload rejection (encode_frame must refuse, not truncate).
    {
        Frame f;
        f.type = MsgType::Movement;
        f.payload.assign(FRAME_MAX_PAYLOAD + 1, 0x41);
        uint8_t buf[FRAME_MAX_TOTAL + 16];
        uint32_t seq = 0, tick = 0;
        const int n = encode_frame(buf, sizeof(buf), f, seq, tick);
        ok(n == 0, "encode_frame rejects payload > FRAME_MAX_PAYLOAD");
    }

    // decode_frame must reject a corrupted magic / version / header-size field.
    {
        Frame f;
        f.type = MsgType::ClientHeartbeat;
        uint8_t buf[FRAME_HEADER_SIZE];
        uint32_t seq = 0, tick = 0;
        encode_frame(buf, sizeof(buf), f, seq, tick);

        uint8_t corruptMagic[FRAME_HEADER_SIZE];
        std::memcpy(corruptMagic, buf, sizeof(buf));
        corruptMagic[0] ^= 0xFF;
        ok(!decode_frame(corruptMagic, FRAME_HEADER_SIZE).has_value(),
           "decode_frame rejects corrupted magic");

        ok(!decode_frame(buf, FRAME_HEADER_SIZE - 1).has_value(),
           "decode_frame rejects a too-short buffer");
    }
}

// ---------------------------------------------------------------------------
// 2. Movement — encode_movement / decode_movement
// ---------------------------------------------------------------------------

static void test_movement() {
    std::printf("\n-- Movement (encode_movement/decode_movement) --\n");

    Movement m;
    m.x = -1234.5f; m.y = 6789.25f; m.z = -0.125f;
    m.yaw = -179.99f; m.aimYaw = 359.5f;
    m.velocityX = -500.0f; m.velocityY = 0.001f; m.velocityZ = 999999.9f;
    m.movementDirection = -90.0f;
    m.movementState = 250;   // near uint8 max, catches truncation
    m.aimState = 3;
    m.animationState = 128;  // high bit set on a uint8

    uint8_t buf[MOVEMENT_PAYLOAD_SIZE];
    encode_movement(m, buf);

    auto d = decode_movement(buf, MOVEMENT_PAYLOAD_SIZE);
    ok(d.has_value(), "decode_movement succeeds on well-formed payload");
    if (!d) return;

    ok(feq(d->x, m.x), "Movement.x round-trips");
    ok(feq(d->y, m.y), "Movement.y round-trips");
    ok(feq(d->z, m.z), "Movement.z round-trips");
    ok(feq(d->yaw, m.yaw), "Movement.yaw round-trips");
    ok(feq(d->aimYaw, m.aimYaw), "Movement.aimYaw round-trips");
    ok(feq(d->velocityX, m.velocityX), "Movement.velocityX round-trips");
    ok(feq(d->velocityY, m.velocityY), "Movement.velocityY round-trips");
    ok(feq(d->velocityZ, m.velocityZ), "Movement.velocityZ round-trips");
    ok(feq(d->movementDirection, m.movementDirection), "Movement.movementDirection round-trips");
    ok(d->movementState == m.movementState, "Movement.movementState (250) round-trips without truncation");
    ok(d->aimState == m.aimState, "Movement.aimState round-trips");
    ok(d->animationState == m.animationState, "Movement.animationState (high bit set) round-trips");

    // Wrong length must be rejected.
    ok(!decode_movement(buf, MOVEMENT_PAYLOAD_SIZE - 1).has_value(),
       "decode_movement rejects wrong-length payload");

    // Non-finite float must be rejected (NaN in x). Movement's wire format is
    // big-endian, so the NaN bit pattern must be written big-endian too (not
    // via a native-endian memcpy) or this just corrupts x into some other,
    // possibly-finite value on a little-endian host.
    {
        uint8_t bad[MOVEMENT_PAYLOAD_SIZE];
        std::memcpy(bad, buf, sizeof(bad));
        std::vector<uint8_t> nanBytes;
        put_f32(nanBytes, std::nanf(""));
        std::memcpy(bad, nanBytes.data(), 4); // overwrite x, big-endian
        ok(!decode_movement(bad, MOVEMENT_PAYLOAD_SIZE).has_value(),
           "decode_movement rejects a NaN field");
    }
}

// ---------------------------------------------------------------------------
// 3. WorldState — decode_world_state only in protocol.cpp; encode here
//    mirrors the documented format: [tag=1][revision:u32BE][authorityMs:u64BE]
//    [timeOfDay/rain/snow/fog/cloudCover/wind/thunder: 7xf32BE] = 41 bytes.
// ---------------------------------------------------------------------------

static std::vector<uint8_t> mirror_encode_world_state(const WorldState& ws) {
    std::vector<uint8_t> b;
    b.push_back(1);
    put_u32(b, ws.revision);
    put_u64(b, ws.authorityMs);
    put_f32(b, ws.timeOfDay);
    put_f32(b, ws.rain);
    put_f32(b, ws.snow);
    put_f32(b, ws.fog);
    put_f32(b, ws.cloudCover);
    put_f32(b, ws.wind);
    put_f32(b, ws.thunder);
    return b;
}

static void test_world_state() {
    std::printf("\n-- WorldState (decode_world_state, mirror encoder) --\n");

    WorldState ws;
    ws.revision = 0xABCD1234u;
    ws.authorityMs = 0x0123456789ABCDEFULL;
    ws.timeOfDay = 1337.5f;   // within [0, 2400] validity range
    ws.rain = 0.75f;
    ws.snow = 0.0f;
    ws.fog = 1.0f;
    ws.cloudCover = 0.5f;
    ws.wind = -3.5f;          // decode has no sign restriction on wind
    ws.thunder = 0.25f;

    auto buf = mirror_encode_world_state(ws);
    ok(buf.size() == 41, "mirror-encoded WorldState payload is 41 bytes");

    auto d = decode_world_state(buf.data(), static_cast<int>(buf.size()));
    ok(d.has_value(), "decode_world_state succeeds on well-formed payload");
    if (!d) return;

    ok(d->revision == ws.revision, "WorldState.revision round-trips");
    ok(d->authorityMs == ws.authorityMs, "WorldState.authorityMs round-trips (64-bit)");
    ok(feq(d->timeOfDay, ws.timeOfDay), "WorldState.timeOfDay round-trips");
    ok(feq(d->rain, ws.rain), "WorldState.rain round-trips");
    ok(feq(d->snow, ws.snow), "WorldState.snow round-trips");
    ok(feq(d->fog, ws.fog), "WorldState.fog round-trips");
    ok(feq(d->cloudCover, ws.cloudCover), "WorldState.cloudCover round-trips");
    ok(feq(d->wind, ws.wind), "WorldState.wind (negative) round-trips");
    ok(feq(d->thunder, ws.thunder), "WorldState.thunder round-trips");

    // timeOfDay out of [0, 2400] must be rejected.
    {
        WorldState bad = ws;
        bad.timeOfDay = 2400.01f;
        auto b2 = mirror_encode_world_state(bad);
        ok(!decode_world_state(b2.data(), static_cast<int>(b2.size())).has_value(),
           "decode_world_state rejects timeOfDay > 2400");
    }
    // Wrong tag byte must be rejected.
    {
        auto b2 = buf;
        b2[0] = 2;
        ok(!decode_world_state(b2.data(), static_cast<int>(b2.size())).has_value(),
           "decode_world_state rejects wrong tag byte");
    }
}

// ---------------------------------------------------------------------------
// 4. PlayerDamage — decode_player_damage only; mirror encoder per doc:
//    [tag=1][current:f32BE][maximum:f32BE][revision:u32BE] = 13 bytes.
// ---------------------------------------------------------------------------

static std::vector<uint8_t> mirror_encode_player_damage(const PlayerDamage& d) {
    std::vector<uint8_t> b;
    b.push_back(1);
    put_f32(b, d.current);
    put_f32(b, d.maximum);
    put_u32(b, d.revision);
    return b;
}

static void test_player_damage() {
    std::printf("\n-- PlayerDamage (decode_player_damage, mirror encoder) --\n");

    PlayerDamage dmg;
    dmg.current = 42.5f;
    dmg.maximum = 100.0f;
    dmg.revision = 0xFFFF0001u; // exercises high bits of u32

    auto buf = mirror_encode_player_damage(dmg);
    ok(buf.size() == 13, "mirror-encoded PlayerDamage payload is 13 bytes");

    auto d = decode_player_damage(buf.data(), static_cast<int>(buf.size()));
    ok(d.has_value(), "decode_player_damage succeeds on well-formed payload");
    if (!d) return;

    ok(feq(d->current, dmg.current), "PlayerDamage.current round-trips");
    ok(feq(d->maximum, dmg.maximum), "PlayerDamage.maximum round-trips");
    ok(d->revision == dmg.revision, "PlayerDamage.revision (high bits set) round-trips");

    // current > maximum is allowed by decode (only current<0 / maximum<=0 rejected) —
    // confirm the boundary rules instead.
    {
        PlayerDamage bad = dmg;
        bad.current = -1.0f;
        auto b2 = mirror_encode_player_damage(bad);
        ok(!decode_player_damage(b2.data(), static_cast<int>(b2.size())).has_value(),
           "decode_player_damage rejects negative current");
    }
    {
        PlayerDamage bad = dmg;
        bad.maximum = 0.0f;
        auto b2 = mirror_encode_player_damage(bad);
        ok(!decode_player_damage(b2.data(), static_cast<int>(b2.size())).has_value(),
           "decode_player_damage rejects maximum <= 0");
    }
}

// ---------------------------------------------------------------------------
// 5. EntitySpawn (EntityDescriptorData) — decode_entity_descriptor only;
//    mirror encoder per doc: [tag=1][kind:u8][revision:u32BE][qty:u16BE]
//    [ownerPlayerId:u64BE][classPathLen:u16BE][classPath...][itemIdLen:u16BE][itemId...]
// ---------------------------------------------------------------------------

static std::vector<uint8_t> mirror_encode_entity_descriptor(const EntityDescriptorData& d) {
    std::vector<uint8_t> b;
    b.push_back(1);
    b.push_back(static_cast<uint8_t>(d.kind));
    put_u32(b, d.revision);
    put_u16(b, d.quantity);
    put_u64(b, d.ownerPlayerId);
    put_str16(b, d.classPath);
    put_str16(b, d.itemId);
    return b;
}

static void test_entity_descriptor() {
    std::printf("\n-- EntitySpawn (decode_entity_descriptor, mirror encoder) --\n");

    EntityDescriptorData d;
    d.kind = EntityKind::Vehicle;
    d.revision = 7;
    d.quantity = 0xFFFF;              // uint16 max
    d.ownerPlayerId = 0x8000000000000001ULL; // high bit set
    d.classPath = "/Game/Blueprints/Vehicles/BP_PickupTruck.BP_PickupTruck_C";
    d.itemId = "DA_PickupTruck";

    auto buf = mirror_encode_entity_descriptor(d);
    auto dec = decode_entity_descriptor(buf.data(), buf.size());
    ok(dec.has_value(), "decode_entity_descriptor succeeds on well-formed payload");
    if (!dec) return;

    ok(dec->kind == d.kind, "EntityDescriptor.kind round-trips");
    ok(dec->revision == d.revision, "EntityDescriptor.revision round-trips");
    ok(dec->quantity == d.quantity, "EntityDescriptor.quantity (uint16 max) round-trips");
    ok(dec->ownerPlayerId == d.ownerPlayerId, "EntityDescriptor.ownerPlayerId (high bit set) round-trips");
    ok(dec->classPath == d.classPath, "EntityDescriptor.classPath round-trips");
    ok(dec->itemId == d.itemId, "EntityDescriptor.itemId round-trips");

    // Empty strings (min-size payload, 20 bytes) must still decode.
    {
        EntityDescriptorData e;
        e.kind = EntityKind::Unknown;
        auto b2 = mirror_encode_entity_descriptor(e);
        ok(b2.size() == 20, "min-size EntityDescriptor payload (empty strings) is 20 bytes");
        auto dec2 = decode_entity_descriptor(b2.data(), b2.size());
        ok(dec2.has_value() && dec2->classPath.empty() && dec2->itemId.empty(),
           "decode_entity_descriptor handles empty classPath/itemId");
    }

    // Truncated buffer must be rejected.
    ok(!decode_entity_descriptor(buf.data(), buf.size() - 1).has_value(),
       "decode_entity_descriptor rejects a truncated buffer");
}

// ---------------------------------------------------------------------------
// 6. EntityState (EntityStateData) — decode_entity_state only; mirror encoder
//    per doc: [tag=1][kind:u8][revision:u32BE][x/y/z/yaw/health:5xf32BE][state:u8]
//    = exactly 27 bytes.
// ---------------------------------------------------------------------------

static std::vector<uint8_t> mirror_encode_entity_state(const EntityStateData& d) {
    std::vector<uint8_t> b;
    b.push_back(1);
    b.push_back(static_cast<uint8_t>(d.kind));
    put_u32(b, d.revision);
    put_f32(b, d.x);
    put_f32(b, d.y);
    put_f32(b, d.z);
    put_f32(b, d.yaw);
    put_f32(b, d.health);
    b.push_back(d.state);
    return b;
}

static void test_entity_state() {
    std::printf("\n-- EntityState (decode_entity_state, mirror encoder) --\n");

    EntityStateData d;
    d.kind = EntityKind::Zombie;
    d.revision = 0x00010203u;
    d.x = -100.25f; d.y = 200.5f; d.z = -0.5f;
    d.yaw = 269.75f;
    d.health = 0.0f; // dead
    d.state = 255;

    auto buf = mirror_encode_entity_state(d);
    ok(buf.size() == static_cast<size_t>(ENTITY_STATE_PAYLOAD_SIZE),
       "mirror-encoded EntityState payload is exactly 27 bytes");

    auto dec = decode_entity_state(buf.data(), buf.size());
    ok(dec.has_value(), "decode_entity_state succeeds on well-formed payload");
    if (!dec) return;

    ok(dec->kind == d.kind, "EntityState.kind round-trips");
    ok(dec->revision == d.revision, "EntityState.revision round-trips");
    ok(feq(dec->x, d.x), "EntityState.x (negative) round-trips");
    ok(feq(dec->y, d.y), "EntityState.y round-trips");
    ok(feq(dec->z, d.z), "EntityState.z (negative) round-trips");
    ok(feq(dec->yaw, d.yaw), "EntityState.yaw round-trips");
    ok(feq(dec->health, d.health), "EntityState.health round-trips");
    ok(dec->state == d.state, "EntityState.state (255) round-trips");

    // Wrong length must be rejected (fixed 27-byte format).
    ok(!decode_entity_state(buf.data(), buf.size() - 1).has_value(),
       "decode_entity_state rejects wrong-length payload");
}

// ---------------------------------------------------------------------------
// 7. ItemDropRequest — encode_item_drop_request only; mirror decoder per doc:
//    [version=1][quantity:u16BE][posX/Y/Z:f32BE][itemIdLen:u16BE][itemId utf8]
// ---------------------------------------------------------------------------

struct MirrorItemDropRequest {
    uint8_t version;
    uint16_t quantity;
    float x, y, z;
    std::string itemId;
};

static std::optional<MirrorItemDropRequest> mirror_decode_item_drop_request(const std::vector<uint8_t>& b) {
    if (b.size() < 17) return std::nullopt;
    MirrorItemDropRequest r;
    r.version = b[0];
    r.quantity = get_u16(&b[1]);
    r.x = get_f32(&b[3]);
    r.y = get_f32(&b[7]);
    r.z = get_f32(&b[11]);
    const uint16_t idLen = get_u16(&b[15]);
    if (b.size() < static_cast<size_t>(17 + idLen)) return std::nullopt;
    r.itemId = std::string(reinterpret_cast<const char*>(&b[17]), idLen);
    return r;
}

static void test_item_drop_request() {
    std::printf("\n-- ItemDropRequest (encode_item_drop_request, mirror decoder) --\n");

    const std::string itemId = "DA_AK74";
    const uint16_t quantity = 0x8001; // high bit set
    const float x = -12.5f, y = 0.0f, z = 999.125f;

    auto buf = encode_item_drop_request(itemId, quantity, x, y, z);
    ok(buf.size() == 17 + itemId.size(), "encode_item_drop_request produces expected length");

    auto dec = mirror_decode_item_drop_request(buf);
    ok(dec.has_value(), "mirror decoder parses encode_item_drop_request output");
    if (!dec) return;

    ok(dec->version == 1, "ItemDropRequest.version == 1");
    ok(dec->quantity == quantity, "ItemDropRequest.quantity (high bit set) round-trips");
    ok(feq(dec->x, x), "ItemDropRequest.x round-trips");
    ok(feq(dec->y, y), "ItemDropRequest.y round-trips");
    ok(feq(dec->z, z), "ItemDropRequest.z round-trips");
    ok(dec->itemId == itemId, "ItemDropRequest.itemId round-trips");

    // Empty itemId edge case.
    {
        auto buf2 = encode_item_drop_request("", 1, 0, 0, 0);
        ok(buf2.size() == 17, "encode_item_drop_request with empty itemId is exactly 17 bytes");
        auto dec2 = mirror_decode_item_drop_request(buf2);
        ok(dec2.has_value() && dec2->itemId.empty(), "empty itemId round-trips as empty string");
    }
}

// ---------------------------------------------------------------------------
// 8. PlayerProgress — encode_player_progress / decode_player_progress
//    (shared wire format for ProfileRevision client->server AND
//    PlayerProgressRestore server->client)
// ---------------------------------------------------------------------------

static void test_player_progress() {
    std::printf("\n-- PlayerProgress / ProfileRevision & PlayerProgressRestore (encode_player_progress/decode_player_progress) --\n");

    PlayerProgress p;
    p.revision = 0xCAFEBABEu;
    p.health = 0.42f; p.hunger = 0.1f; p.thirst = 0.99f;
    p.stamina = 0.0f; p.radiation = 1.0f;
    p.level = -1; // int32_t: encode casts to uint32_t and decode casts back — verify sign survives
    p.xp = 123456.75f;
    p.posX = -9999.5f; p.posY = 0.0f; p.posZ = 42.25f; p.yaw = -180.0f;

    InventoryContainer backpack;
    backpack.columns = 6; backpack.rows = 4;
    InventorySlot s1; s1.slotIndex = 0; s1.itemId = "DA_AK74"; s1.quantity = 1;
    InventorySlot s2; s2.slotIndex = 255; s2.itemId = "DA_Bandage"; s2.quantity = 0xFFFF;
    backpack.items = { s1, s2 };

    InventoryContainer secureContainer; // empty container (no items) must still round-trip
    secureContainer.columns = 3; secureContainer.rows = 3;

    p.containers = { backpack, secureContainer };

    p.forename = "Jean-Luc";
    p.surname = "O'Neil";  // apostrophe — catches naive-escaping-style bugs
    p.zombieKills = 12345;
    p.daysSurvived = -7;   // shouldn't happen in practice, but exercises sign round-trip
    p.bossZombieKills = 3;
    p.animalKills = 0;
    p.humanKills = 9999;
    p.distanceTravelled = 100000.5f;
    p.infestationsDestroyed = 2147483647; // INT32_MAX

    auto buf = encode_player_progress(p);
    auto d = decode_player_progress(buf.data(), buf.size());
    ok(d.has_value(), "decode_player_progress succeeds on encode_player_progress output");
    if (!d) return;

    ok(d->revision == p.revision, "PlayerProgress.revision round-trips");
    ok(feq(d->health, p.health), "PlayerProgress.health round-trips");
    ok(feq(d->hunger, p.hunger), "PlayerProgress.hunger round-trips");
    ok(feq(d->thirst, p.thirst), "PlayerProgress.thirst round-trips");
    ok(feq(d->stamina, p.stamina), "PlayerProgress.stamina round-trips");
    ok(feq(d->radiation, p.radiation), "PlayerProgress.radiation round-trips");
    ok(d->level == p.level, "PlayerProgress.level (-1) round-trips through uint32 cast with sign intact");
    ok(feq(d->xp, p.xp), "PlayerProgress.xp round-trips");
    ok(feq(d->posX, p.posX), "PlayerProgress.posX round-trips");
    ok(feq(d->posY, p.posY), "PlayerProgress.posY round-trips");
    ok(feq(d->posZ, p.posZ), "PlayerProgress.posZ round-trips");
    ok(feq(d->yaw, p.yaw), "PlayerProgress.yaw round-trips");

    ok(d->containers.size() == 2, "PlayerProgress.containers count round-trips");
    if (d->containers.size() == 2) {
        ok(d->containers[0].columns == backpack.columns, "container[0].columns round-trips");
        ok(d->containers[0].rows == backpack.rows, "container[0].rows round-trips");
        ok(d->containers[0].items.size() == 2, "container[0].items count round-trips");
        if (d->containers[0].items.size() == 2) {
            ok(d->containers[0].items[0].slotIndex == s1.slotIndex, "item[0].slotIndex round-trips");
            ok(d->containers[0].items[0].itemId == s1.itemId, "item[0].itemId round-trips");
            ok(d->containers[0].items[0].quantity == s1.quantity, "item[0].quantity round-trips");
            ok(d->containers[0].items[1].slotIndex == s2.slotIndex, "item[1].slotIndex (255) round-trips");
            ok(d->containers[0].items[1].itemId == s2.itemId, "item[1].itemId round-trips");
            ok(d->containers[0].items[1].quantity == s2.quantity, "item[1].quantity (uint16 max) round-trips");
        }
        ok(d->containers[1].items.empty(), "container[1] (empty container) round-trips with zero items");
    }

    ok(d->forename == p.forename, "PlayerProgress.forename round-trips");
    ok(d->surname == p.surname, "PlayerProgress.surname (apostrophe) round-trips");
    ok(d->zombieKills == p.zombieKills, "PlayerProgress.zombieKills round-trips");
    ok(d->daysSurvived == p.daysSurvived, "PlayerProgress.daysSurvived (negative) round-trips");
    ok(d->bossZombieKills == p.bossZombieKills, "PlayerProgress.bossZombieKills round-trips");
    ok(d->animalKills == p.animalKills, "PlayerProgress.animalKills round-trips");
    ok(d->humanKills == p.humanKills, "PlayerProgress.humanKills round-trips");
    ok(feq(d->distanceTravelled, p.distanceTravelled), "PlayerProgress.distanceTravelled round-trips");
    ok(d->infestationsDestroyed == p.infestationsDestroyed, "PlayerProgress.infestationsDestroyed (INT32_MAX) round-trips");

    // Backward-compat: a payload truncated right after the containers (no
    // extended-stats trailer, as documented for pre-gap-4/7 saves) must still
    // decode, leaving the trailer fields at their PlayerProgress defaults.
    {
        PlayerProgress minimal;
        minimal.revision = 5;
        minimal.containers = {}; // no containers -> buffer ends right after the 51-byte header
        auto bufMin = encode_player_progress(minimal);
        // Truncate off the trailer entirely (everything after byte 51) to
        // simulate a pre-gap-4/7 payload that never had a trailer at all.
        std::vector<uint8_t> truncated(bufMin.begin(), bufMin.begin() + 51);
        auto dmin = decode_player_progress(truncated.data(), truncated.size());
        ok(dmin.has_value(), "decode_player_progress accepts a payload truncated right after the header (no trailer)");
        if (dmin) {
            ok(dmin->forename.empty(), "trailer-less payload leaves forename at default (empty)");
            ok(dmin->zombieKills == 0, "trailer-less payload leaves zombieKills at default (0)");
        }
    }

    // Malformed input: containerCount claims more than the buffer holds.
    {
        auto bufBad = encode_player_progress(p);
        // Bump the container count field (offset 49, u16BE) so it lies.
        bufBad[49] = 0xFF; bufBad[50] = 0xFF;
        ok(!decode_player_progress(bufBad.data(), bufBad.size()).has_value(),
           "decode_player_progress rejects a container count that overruns the buffer");
    }
}

// ---------------------------------------------------------------------------
// 9. Equipment — encode_equipment / decode_equipment
// ---------------------------------------------------------------------------

static void test_equipment() {
    std::printf("\n-- Equipment (encode_equipment/decode_equipment) --\n");

    Equipment e;
    EquipmentSlot slot0; slot0.slotIndex = 0; slot0.itemId = "DA_Balaclava";
    EquipmentSlot slot11; slot11.slotIndex = 11; slot11.itemId = "DA_AK74"; // Primary
    EquipmentSlot slot20; slot20.slotIndex = 20; slot20.itemId = ""; // unequipped, empty itemId
    e.slots = { slot0, slot11, slot20 };

    auto buf = encode_equipment(e);
    auto d = decode_equipment(buf.data(), buf.size());
    ok(d.has_value(), "decode_equipment succeeds on encode_equipment output");
    if (!d) return;

    ok(d->slots.size() == 3, "Equipment.slots count round-trips");
    if (d->slots.size() == 3) {
        ok(d->slots[0].slotIndex == 0 && d->slots[0].itemId == "DA_Balaclava", "slot[0] round-trips");
        ok(d->slots[1].slotIndex == 11 && d->slots[1].itemId == "DA_AK74", "slot[1] round-trips");
        ok(d->slots[2].slotIndex == 20 && d->slots[2].itemId.empty(), "slot[2] (empty itemId, unequipped) round-trips");
    }

    // Empty equipment set.
    {
        Equipment empty;
        auto b2 = encode_equipment(empty);
        ok(b2.size() == 3, "encode_equipment with zero slots is exactly 3 bytes (tag+count)");
        auto d2 = decode_equipment(b2.data(), b2.size());
        ok(d2.has_value() && d2->slots.empty(), "decode_equipment handles zero slots");
    }

    // Truncated buffer.
    ok(!decode_equipment(buf.data(), buf.size() - 1).has_value(),
       "decode_equipment rejects a truncated buffer");
}

// ---------------------------------------------------------------------------
// 10. WeaponAttachments — encode_weapon_attachments / decode_weapon_attachments
// ---------------------------------------------------------------------------

static void test_weapon_attachments() {
    std::printf("\n-- WeaponAttachments (encode_weapon_attachments/decode_weapon_attachments) --\n");

    WeaponAttachments a;
    WeaponAttachmentEntry e1; e1.weaponSlotIndex = 11; e1.containerIndex = 0; e1.itemId = "HolographicSight";
    WeaponAttachmentEntry e2; e2.weaponSlotIndex = 14; e2.containerIndex = 255; e2.itemId = "Suppressor_9mm";
    a.entries = { e1, e2 };

    auto buf = encode_weapon_attachments(a);
    auto d = decode_weapon_attachments(buf.data(), buf.size());
    ok(d.has_value(), "decode_weapon_attachments succeeds on encode_weapon_attachments output");
    if (!d) return;

    ok(d->entries.size() == 2, "WeaponAttachments.entries count round-trips");
    if (d->entries.size() == 2) {
        ok(d->entries[0].weaponSlotIndex == 11 && d->entries[0].containerIndex == 0
           && d->entries[0].itemId == "HolographicSight", "entries[0] round-trips");
        ok(d->entries[1].weaponSlotIndex == 14 && d->entries[1].containerIndex == 255
           && d->entries[1].itemId == "Suppressor_9mm", "entries[1] (containerIndex=255) round-trips");
    }

    // Empty entries.
    {
        WeaponAttachments empty;
        auto b2 = encode_weapon_attachments(empty);
        auto d2 = decode_weapon_attachments(b2.data(), b2.size());
        ok(d2.has_value() && d2->entries.empty(), "decode_weapon_attachments handles zero entries");
    }
}

// ---------------------------------------------------------------------------
// 11. PawnAppearance — encode_pawn_appearance / decode_pawn_appearance
// ---------------------------------------------------------------------------

static void test_pawn_appearance() {
    std::printf("\n-- PawnAppearance (encode_pawn_appearance/decode_pawn_appearance) --\n");

    PawnAppearance a;
    a.isMale = false;
    a.hairMeshName = "Chr_FemaleHair3";
    a.hairColorName = "HairColor_Auburn";
    a.beardMeshName = ""; // females: no beard, tests empty-string field mid-struct
    a.beardColorName = "";
    a.skinColorName = "SkinTone_04";
    a.bodyPartMeshNames = {
        "SK_Chr_Underwear_Female_01_Torso", "SK_Chr_Underwear_Female_01_Biceps",
        "SK_Chr_Underwear_Female_01_LowerThighs", "SK_Chr_Female_Head",
        "SK_Chr_Underwear_Female_01_Arms", "SK_Chr_Female_Feet",
        "SK_Chr_Underwear_Female_01_LowerLegs", "SK_Chr_Underwear_Female_01_Legs",
        "SK_Chr_Female_Hands"
    };
    a.mouthMeshName = "Mouth_02";
    a.eyebrowsMeshName = "Eyebrows_05";
    a.accessory1MeshName = "Piercing_Nose_01";
    a.accessory2MeshName = "";
    a.accessory3MeshName = "";

    auto buf = encode_pawn_appearance(a);
    auto d = decode_pawn_appearance(buf.data(), buf.size());
    ok(d.has_value(), "decode_pawn_appearance succeeds on encode_pawn_appearance output");
    if (!d) return;

    ok(d->isMale == a.isMale, "PawnAppearance.isMale (false) round-trips");
    ok(d->hairMeshName == a.hairMeshName, "PawnAppearance.hairMeshName round-trips");
    ok(d->hairColorName == a.hairColorName, "PawnAppearance.hairColorName round-trips");
    ok(d->beardMeshName.empty(), "PawnAppearance.beardMeshName (empty) round-trips");
    ok(d->beardColorName.empty(), "PawnAppearance.beardColorName (empty) round-trips");
    ok(d->skinColorName == a.skinColorName, "PawnAppearance.skinColorName round-trips");

    bool bodyOk = true;
    for (int i = 0; i < BODY_PART_COUNT; ++i)
        if (d->bodyPartMeshNames[i] != a.bodyPartMeshNames[i]) bodyOk = false;
    ok(bodyOk, "PawnAppearance.bodyPartMeshNames[0..8] all round-trip in order");

    ok(d->mouthMeshName == a.mouthMeshName, "PawnAppearance.mouthMeshName round-trips");
    ok(d->eyebrowsMeshName == a.eyebrowsMeshName, "PawnAppearance.eyebrowsMeshName round-trips");
    ok(d->accessory1MeshName == a.accessory1MeshName, "PawnAppearance.accessory1MeshName round-trips");
    ok(d->accessory2MeshName.empty(), "PawnAppearance.accessory2MeshName (empty) round-trips");
    ok(d->accessory3MeshName.empty(), "PawnAppearance.accessory3MeshName (empty) round-trips");

    // isMale = true, all fields populated (opposite boolean branch).
    {
        PawnAppearance male;
        male.isMale = true;
        male.hairMeshName = "Chr_MaleHair3";
        male.beardMeshName = "Chr_MaleBeard1";
        male.beardColorName = "HairColor_Black";
        for (auto& s : male.bodyPartMeshNames) s = "SK_Chr_Underwear_Male_01_Part";
        auto b2 = encode_pawn_appearance(male);
        auto d2 = decode_pawn_appearance(b2.data(), b2.size());
        ok(d2.has_value() && d2->isMale == true && d2->beardMeshName == "Chr_MaleBeard1",
           "PawnAppearance.isMale (true) + populated beard fields round-trip");
    }

    // Truncated buffer.
    ok(!decode_pawn_appearance(buf.data(), buf.size() - 1).has_value(),
       "decode_pawn_appearance rejects a truncated buffer");
}

// ---------------------------------------------------------------------------
// 12. World-action JSON envelope — encode_world_action / decode_world_action
//     Used for: CharacterCreate, InteractionRequest, InteractionResult,
//     ItemPickupResult, ItemDropResult (all flat single-level JSON objects).
//     Also exercises json_str/json_bool/json_double helpers.
// ---------------------------------------------------------------------------

static void test_world_action_json() {
    std::printf("\n-- World-action JSON (encode_world_action/decode_world_action + json_* helpers) --\n");

    const std::string json =
        "{\"success\":true,\"reason\":\"ok\",\"amount\":12.5,\"forename\":\"Jean-Luc\"}";

    auto buf = encode_world_action(json);
    ok(buf.size() == 2 + json.size(), "encode_world_action length is len-prefix + payload");

    auto dec = decode_world_action(buf.data(), buf.size());
    ok(dec.has_value() && *dec == json, "decode_world_action round-trips the exact JSON string");

    if (dec) {
        ok(json_bool(*dec, "success") == true, "json_bool parses true");
        ok(json_str(*dec, "reason") == "ok", "json_str extracts a string field");
        ok(json_str(*dec, "forename") == "Jean-Luc", "json_str extracts a hyphenated string field");
        ok(std::fabs(json_double(*dec, "amount") - 12.5) < 1e-9, "json_double parses a fractional number");
        ok(json_bool(*dec, "missingKey") == false, "json_bool defaults to false for a missing key");
        ok(json_str(*dec, "missingKey").empty(), "json_str defaults to empty for a missing key");
    }

    // Empty JSON body (valid per format: zero-length string is legal).
    {
        auto b2 = encode_world_action("");
        ok(b2.size() == 2, "encode_world_action(\"\") is exactly the 2-byte length prefix");
        auto d2 = decode_world_action(b2.data(), b2.size());
        ok(d2.has_value() && d2->empty(), "decode_world_action handles an empty JSON body");
    }

    // Truncated buffer (length prefix lies about available data).
    {
        auto b2 = buf;
        b2.resize(b2.size() - 1); // chop the last byte off but keep the length prefix as-is
        ok(!decode_world_action(b2.data(), b2.size()).has_value(),
           "decode_world_action rejects a buffer shorter than its length prefix claims");
    }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main() {
    std::printf("SurrounDeadBridge protocol round-trip test\n");
    std::printf("(standalone — no UE4SS/UE5 dependency, see src/protocol.cpp)\n");

    test_frame_envelope_all_header_only_types();
    test_movement();
    test_world_state();
    test_player_damage();
    test_entity_descriptor();
    test_entity_state();
    test_item_drop_request();
    test_player_progress();
    test_equipment();
    test_weapon_attachments();
    test_pawn_appearance();
    test_world_action_json();

    std::printf("\n%d/%d assertions passed\n", g_total - g_failures, g_total);
    if (g_failures > 0) {
        std::printf("FAILED (%d failing assertion%s)\n", g_failures, g_failures == 1 ? "" : "s");
        return 1;
    }
    std::printf("ALL PASSED\n");
    return 0;
}
