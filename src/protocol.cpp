#include "protocol.hpp"
#include <atomic>
#include <cstring>
#include <cmath>
#include <cstdio>

#ifdef _WIN32
#  include <Windows.h>
#else
#  include <time.h>
#endif

namespace sdb {

// ---------------------------------------------------------------------------
// Byte-order helpers (big-endian, no UB)
// ---------------------------------------------------------------------------

static void w16(uint8_t* p, uint16_t v) {
    p[0] = (v >> 8) & 0xff;
    p[1] =  v       & 0xff;
}
static void w32(uint8_t* p, uint32_t v) {
    p[0] = (v >> 24) & 0xff; p[1] = (v >> 16) & 0xff;
    p[2] = (v >>  8) & 0xff; p[3] =  v        & 0xff;
}
static void w64(uint8_t* p, uint64_t v) {
    w32(p,     static_cast<uint32_t>(v >> 32));
    w32(p + 4, static_cast<uint32_t>(v));
}
static uint16_t r16(const uint8_t* p) {
    return (static_cast<uint16_t>(p[0]) << 8) | p[1];
}
static uint32_t r32(const uint8_t* p) {
    return  (static_cast<uint32_t>(p[0]) << 24)
          | (static_cast<uint32_t>(p[1]) << 16)
          | (static_cast<uint32_t>(p[2]) <<  8)
          |  static_cast<uint32_t>(p[3]);
}
static uint64_t r64(const uint8_t* p) {
    return (static_cast<uint64_t>(r32(p)) << 32) | r32(p + 4);
}

// float ↔ uint32 bit-cast (avoids strict-alias UB)
static uint32_t f2u(float f) { uint32_t u; std::memcpy(&u, &f, 4); return u; }
static float    u2f(uint32_t u) { float f; std::memcpy(&f, &u, 4); return f; }

// ---------------------------------------------------------------------------
// encode_frame
// ---------------------------------------------------------------------------

int encode_frame(uint8_t* buf, int cap, const Frame& f,
                 uint32_t& seq, uint32_t& tck)
{
    const int payload_len = static_cast<int>(f.payload.size());
    const int total       = FRAME_HEADER_SIZE + payload_len;
    if (total > cap || payload_len > static_cast<int>(FRAME_MAX_PAYLOAD))
        return 0;

    std::memset(buf, 0, total);

    w32(buf +  0, FRAME_MAGIC);
    w16(buf +  4, FRAME_VERSION);
    w16(buf +  6, static_cast<uint16_t>(f.type));
    w16(buf +  8, f.flags);
    w16(buf + 10, FRAME_HEADER_SIZE);
    w32(buf + 12, static_cast<uint32_t>(payload_len));
    w64(buf + 16, f.connectionId);
    std::memcpy(buf + 24, f.sessionId.data(), 16);
    std::memcpy(buf + 40, f.worldId.data(),   16);
    w64(buf + 56, f.playerId);
    w64(buf + 64, f.entityId);
    w32(buf + 72, ++seq);
    w32(buf + 76, ++tck);
    w64(buf + 80, f.timestampUs ? f.timestampUs : now_micros());

    if (payload_len > 0)
        std::memcpy(buf + FRAME_HEADER_SIZE, f.payload.data(), payload_len);

    return total;
}

// ---------------------------------------------------------------------------
// decode_frame
// ---------------------------------------------------------------------------

std::optional<Frame> decode_frame(const uint8_t* data, int len)
{
    if (len < FRAME_HEADER_SIZE)                                       return std::nullopt;
    if (r32(data)      != FRAME_MAGIC)                                 return std::nullopt;
    if (r16(data + 4)  != FRAME_VERSION)                               return std::nullopt;
    if (r16(data + 10) != FRAME_HEADER_SIZE)                          return std::nullopt;

    const uint32_t payload_len = r32(data + 12);
    if (payload_len > FRAME_MAX_PAYLOAD)                               return std::nullopt;
    if (len != FRAME_HEADER_SIZE + static_cast<int>(payload_len))     return std::nullopt;

    Frame f;
    f.type         = static_cast<MsgType>(r16(data + 6));
    f.flags        = r16(data + 8);
    f.connectionId = r64(data + 16);
    std::memcpy(f.sessionId.data(), data + 24, 16);
    std::memcpy(f.worldId.data(),   data + 40, 16);
    f.playerId     = r64(data + 56);
    f.entityId     = r64(data + 64);
    f.sequence     = r32(data + 72);
    f.tick         = r32(data + 76);
    f.timestampUs  = r64(data + 80);

    if (payload_len > 0) {
        f.payload.assign(data + FRAME_HEADER_SIZE,
                         data + FRAME_HEADER_SIZE + payload_len);
    }
    return f;
}

// ---------------------------------------------------------------------------
// Movement  (39-byte payload, no tag byte — matches encodeMovement in JS)
// ---------------------------------------------------------------------------

void encode_movement(const Movement& m, uint8_t out[MOVEMENT_PAYLOAD_SIZE])
{
    const float floats[9] = {
        m.x, m.y, m.z,
        m.yaw, m.aimYaw,
        m.velocityX, m.velocityY, m.velocityZ,
        m.movementDirection
    };
    for (int i = 0; i < 9; ++i)
        w32(out + i * 4, f2u(floats[i]));
    out[36] = m.movementState;
    out[37] = m.aimState;
    out[38] = m.animationState;
}

std::optional<Movement> decode_movement(const uint8_t* p, int len)
{
    if (len != MOVEMENT_PAYLOAD_SIZE) return std::nullopt;

    Movement m;
    m.x                  = u2f(r32(p +  0));
    m.y                  = u2f(r32(p +  4));
    m.z                  = u2f(r32(p +  8));
    m.yaw                = u2f(r32(p + 12));
    m.aimYaw             = u2f(r32(p + 16));
    m.velocityX          = u2f(r32(p + 20));
    m.velocityY          = u2f(r32(p + 24));
    m.velocityZ          = u2f(r32(p + 28));
    m.movementDirection  = u2f(r32(p + 32));
    m.movementState      = p[36];
    m.aimState           = p[37];
    m.animationState     = p[38];

    const float vals[9] = {
        m.x, m.y, m.z, m.yaw, m.aimYaw,
        m.velocityX, m.velocityY, m.velocityZ, m.movementDirection
    };
    for (float v : vals)
        if (!std::isfinite(v)) return std::nullopt;

    return m;
}

// ---------------------------------------------------------------------------
// WorldState  (41-byte payload)
// ---------------------------------------------------------------------------

std::optional<WorldState> decode_world_state(const uint8_t* p, int len)
{
    if (len != 41 || p[0] != 1) return std::nullopt;

    WorldState ws;
    ws.revision    = r32(p + 1);
    ws.authorityMs = r64(p + 5);
    ws.timeOfDay   = u2f(r32(p + 13));
    ws.rain        = u2f(r32(p + 17));
    ws.snow        = u2f(r32(p + 21));
    ws.fog         = u2f(r32(p + 25));
    ws.cloudCover  = u2f(r32(p + 29));
    ws.wind        = u2f(r32(p + 33));
    ws.thunder     = u2f(r32(p + 37));

    const float vals[7] = {
        ws.timeOfDay, ws.rain, ws.snow, ws.fog,
        ws.cloudCover, ws.wind, ws.thunder
    };
    for (float v : vals)
        if (!std::isfinite(v)) return std::nullopt;

    if (ws.timeOfDay < 0.0f || ws.timeOfDay > 2400.0f) return std::nullopt;
    return ws;
}

// ---------------------------------------------------------------------------
// PlayerDamage  (13-byte payload: tag=1, f32 current, f32 max, u32 rev)
// ---------------------------------------------------------------------------

std::optional<PlayerDamage> decode_player_damage(const uint8_t* p, int len)
{
    if (len != 13 || p[0] != 1) return std::nullopt;

    PlayerDamage d;
    d.current  = u2f(r32(p + 1));
    d.maximum  = u2f(r32(p + 5));
    d.revision = r32(p + 9);

    if (!std::isfinite(d.current) || !std::isfinite(d.maximum)) return std::nullopt;
    if (d.current < 0.0f || d.maximum <= 0.0f) return std::nullopt;
    return d;
}

// ---------------------------------------------------------------------------
// EntityDescriptor  (variable-length, from encodeWorldEntityDescriptor in JS)
// Format: [tag=1][kind:u8][revision:u32BE][qty:u16BE][ownerPlayerId:u64BE]
//         [classPathLen:u16BE][classPath...][itemIdLen:u16BE][itemId...]
// ---------------------------------------------------------------------------

std::optional<EntityDescriptorData> decode_entity_descriptor(const uint8_t* p, size_t n)
{
    // Minimum: 1+1+4+2+8+2+2 = 20 bytes (both strings empty)
    if (n < 20 || p[0] != 1) return std::nullopt;

    EntityDescriptorData d;
    d.kind          = static_cast<EntityKind>(p[1]);
    d.revision      = r32(p + 2);
    d.quantity      = r16(p + 6);
    d.ownerPlayerId = r64(p + 8);

    // classPath
    const uint16_t cpLen = r16(p + 16);
    if (n < static_cast<size_t>(18 + cpLen + 2)) return std::nullopt;
    d.classPath = std::string(reinterpret_cast<const char*>(p + 18), cpLen);

    // itemId
    const size_t iidOff = 18 + cpLen;
    const uint16_t iidLen = r16(p + iidOff);
    if (n < iidOff + 2 + iidLen) return std::nullopt;
    d.itemId = std::string(reinterpret_cast<const char*>(p + iidOff + 2), iidLen);

    return d;
}

// ---------------------------------------------------------------------------
// EntityState  (exactly 27 bytes, from encodeWorldEntityState in JS)
// Format: [tag=1][kind:u8][revision:u32BE][x/y/z/yaw/health:5×f32BE][state:u8]
// ---------------------------------------------------------------------------

std::optional<EntityStateData> decode_entity_state(const uint8_t* p, size_t n)
{
    if (n != static_cast<size_t>(ENTITY_STATE_PAYLOAD_SIZE) || p[0] != 1)
        return std::nullopt;

    EntityStateData d;
    d.kind     = static_cast<EntityKind>(p[1]);
    d.revision = r32(p + 2);
    d.x        = u2f(r32(p +  6));
    d.y        = u2f(r32(p + 10));
    d.z        = u2f(r32(p + 14));
    d.yaw      = u2f(r32(p + 18));
    d.health   = u2f(r32(p + 22));
    d.state    = p[26];

    if (!std::isfinite(d.x) || !std::isfinite(d.y) || !std::isfinite(d.z))
        return std::nullopt;

    return d;
}

// ---------------------------------------------------------------------------
// World-action JSON codec  (encodeWorldAction / decodeWorldAction in JS)
// Wire format: uint16BE length + UTF-8 JSON  — no tag byte.
// ---------------------------------------------------------------------------

std::vector<uint8_t> encode_world_action(const std::string& json)
{
    const uint16_t len = static_cast<uint16_t>(json.size());
    std::vector<uint8_t> out(2 + json.size());
    out[0] = (len >> 8) & 0xFF;
    out[1] = len & 0xFF;
    std::memcpy(out.data() + 2, json.data(), json.size());
    return out;
}

std::vector<uint8_t> encode_item_drop_request(const std::string& itemId, uint16_t quantity,
                                               float x, float y, float z)
{
    const auto idLen = static_cast<uint16_t>(itemId.size());
    std::vector<uint8_t> out(17 + idLen);
    out[0] = 1; // version
    w16(out.data() + 1, quantity);
    w32(out.data() + 3, f2u(x));
    w32(out.data() + 7, f2u(y));
    w32(out.data() + 11, f2u(z));
    w16(out.data() + 15, idLen);
    std::memcpy(out.data() + 17, itemId.data(), itemId.size());
    return out;
}

// InteractionRequest/BUILD: itemId-based, matching encode_item_drop_request's
// shape (see server/src/lib/protocol.js's decodeInteractionRequest for the
// full rationale — a numeric pieceTypeId had no DataAsset/class equivalent
// and was a dead end for client-side rendering; itemId resolves through the
// same UJigsawItem_DataAsset_C machinery GroundItem already uses, just
// reading BuildActorClass instead of PickupClass off the result).
// Format: [version=1][interactionType=BUILD][posX/Y/Z/yaw:f32BE][itemIdLen:u16BE][itemId utf8]
std::vector<uint8_t> encode_interaction_request_build(const std::string& itemId,
                                                        float x, float y, float z, float yaw)
{
    const auto idLen = static_cast<uint16_t>(itemId.size());
    std::vector<uint8_t> out(20 + idLen);
    out[0] = 1; // version
    out[1] = static_cast<uint8_t>(InteractionType::BUILD);
    w32(out.data() + 2,  f2u(x));
    w32(out.data() + 6,  f2u(y));
    w32(out.data() + 10, f2u(z));
    w32(out.data() + 14, f2u(yaw));
    w16(out.data() + 18, idLen);
    std::memcpy(out.data() + 20, itemId.data(), itemId.size());
    return out;
}

std::optional<std::string> decode_world_action(const uint8_t* p, size_t n)
{
    if (n < 2) return std::nullopt;
    const uint16_t len = (static_cast<uint16_t>(p[0]) << 8) | p[1];
    if (n < static_cast<size_t>(2 + len)) return std::nullopt;
    return std::string(reinterpret_cast<const char*>(p + 2), len);
}

// ---------------------------------------------------------------------------
// Flat JSON field extraction  (our result payloads are flat single-level objects)
// ---------------------------------------------------------------------------

std::string json_str(const std::string& json, const std::string& key)
{
    const std::string search = "\"" + key + "\":\"";
    const auto pos = json.find(search);
    if (pos == std::string::npos) return {};
    const auto start = pos + search.size();
    const auto end   = json.find('"', start);
    if (end == std::string::npos) return {};
    return json.substr(start, end - start);
}

bool json_bool(const std::string& json, const std::string& key)
{
    const std::string search = "\"" + key + "\":";
    const auto pos = json.find(search);
    if (pos == std::string::npos) return false;
    auto i = pos + search.size();
    while (i < json.size() && json[i] == ' ') ++i;
    return json.size() >= i + 4 && json.substr(i, 4) == "true";
}

double json_double(const std::string& json, const std::string& key)
{
    const std::string search = "\"" + key + "\":";
    const auto pos = json.find(search);
    if (pos == std::string::npos) return 0.0;
    const auto start = pos + search.size();
    try { return std::stod(json.substr(start)); }
    catch (...) { return 0.0; }
}

// ---------------------------------------------------------------------------
// Request ID  (8-char lowercase hex counter, matches /^[a-zA-Z0-9._:-]{1,80}$/)
// ---------------------------------------------------------------------------

static std::atomic<uint32_t> s_req_counter{0};

std::string next_request_id()
{
    char buf[9];
    std::snprintf(buf, sizeof(buf), "%08x",
                  s_req_counter.fetch_add(1, std::memory_order_relaxed));
    return std::string(buf);
}

// ---------------------------------------------------------------------------
// ProfileRevision payload  (client→server, and replayed verbatim server→client
// as PlayerProgressRestore — see decode_player_progress)
// Header: [tag=1][revision:u32][health:f32][hunger:f32][thirst:f32]
//         [stamina:f32][radiation:f32][level:u32][xp:f32]
//         [posX:f32][posY:f32][posZ:f32][yaw:f32][containerCount:u16]  = 51 bytes
// Per container: [columns:u16BE][rows:u16BE][itemCount:u16BE][items...]
// Per item: [slotIndex:u8][itemIdLen:u16BE][itemId...][qty:u16BE]
//
// Gap 11 (2026-08-10): replaces the earlier flat, globally-indexed slot list
// capped at MAX_INV_SLOTS=40. The game's own inventory (BP_JigMultiplayer_C.
// MainJigContainers) has no fixed slot count — each container carries its own
// runtime-resizable Columns/Rows (research/04_ida_investigation_log.md Session
// 29) — so encoding a flat cap was solving the wrong problem. This is a
// breaking wire-format change: old payloads persisted in the server DB under
// the previous flat format will not decode correctly against this container
// list and should be treated as stale (dev-stage mod, no migration provided).
//
// Extended stats trailer (gap 4/7), appended after the containers:
// [forenameLen:u16BE][forename utf8][surnameLen:u16BE][surname utf8]
// [zombieKills:u32][daysSurvived:u32][bossZombieKills:u32][animalKills:u32]
// [humanKills:u32][distanceTravelled:f32][infestationsDestroyed:u32]
// Optional on decode: a payload that ends right after the containers (i.e. one
// persisted by the server before this trailer existed) still decodes fine,
// just with these fields left at their PlayerProgress defaults.
// ---------------------------------------------------------------------------

static constexpr size_t PLAYER_PROGRESS_HEADER_SIZE = 51;
static constexpr size_t PLAYER_PROGRESS_TRAILER_FIXED_SIZE = 2 + 2 + 4 * 6 + 4; // sans the two strings

std::vector<uint8_t> encode_player_progress(const PlayerProgress& prog)
{
    size_t total = PLAYER_PROGRESS_HEADER_SIZE;
    for (const auto& container : prog.containers) {
        total += 2 + 2 + 2; // columns, rows, itemCount
        for (const auto& item : container.items)
            total += 1 + 2 + item.itemId.size() + 2;
    }
    total += PLAYER_PROGRESS_TRAILER_FIXED_SIZE + prog.forename.size() + prog.surname.size();

    std::vector<uint8_t> buf(total);
    uint8_t* p = buf.data();

    p[0] = 1;
    w32(p +  1, prog.revision);
    w32(p +  5, f2u(prog.health));
    w32(p +  9, f2u(prog.hunger));
    w32(p + 13, f2u(prog.thirst));
    w32(p + 17, f2u(prog.stamina));
    w32(p + 21, f2u(prog.radiation));
    w32(p + 25, static_cast<uint32_t>(prog.level));
    w32(p + 29, f2u(prog.xp));
    w32(p + 33, f2u(prog.posX));
    w32(p + 37, f2u(prog.posY));
    w32(p + 41, f2u(prog.posZ));
    w32(p + 45, f2u(prog.yaw));
    w16(p + 49, static_cast<uint16_t>(prog.containers.size()));

    size_t off = PLAYER_PROGRESS_HEADER_SIZE;
    for (const auto& container : prog.containers) {
        w16(buf.data() + off, container.columns); off += 2;
        w16(buf.data() + off, container.rows);    off += 2;
        w16(buf.data() + off, static_cast<uint16_t>(container.items.size())); off += 2;
        for (const auto& item : container.items) {
            buf[off++] = item.slotIndex;
            const uint16_t idLen = static_cast<uint16_t>(item.itemId.size());
            w16(buf.data() + off, idLen); off += 2;
            if (idLen) {
                std::memcpy(buf.data() + off, item.itemId.data(), idLen);
                off += idLen;
            }
            w16(buf.data() + off, item.quantity); off += 2;
        }
    }

    auto write_str = [&](const std::string& s) {
        const uint16_t len = static_cast<uint16_t>(s.size());
        w16(buf.data() + off, len); off += 2;
        if (len) { std::memcpy(buf.data() + off, s.data(), len); off += len; }
    };
    write_str(prog.forename);
    write_str(prog.surname);
    w32(buf.data() + off, static_cast<uint32_t>(prog.zombieKills));           off += 4;
    w32(buf.data() + off, static_cast<uint32_t>(prog.daysSurvived));          off += 4;
    w32(buf.data() + off, static_cast<uint32_t>(prog.bossZombieKills));       off += 4;
    w32(buf.data() + off, static_cast<uint32_t>(prog.animalKills));           off += 4;
    w32(buf.data() + off, static_cast<uint32_t>(prog.humanKills));            off += 4;
    w32(buf.data() + off, f2u(prog.distanceTravelled));                      off += 4;
    w32(buf.data() + off, static_cast<uint32_t>(prog.infestationsDestroyed)); off += 4;

    return buf;
}

std::optional<PlayerProgress> decode_player_progress(const uint8_t* p, size_t n)
{
    if (n < PLAYER_PROGRESS_HEADER_SIZE || p[0] != 1) return std::nullopt;

    PlayerProgress prog;
    prog.revision  = r32(p +  1);
    prog.health    = u2f(r32(p +  5));
    prog.hunger    = u2f(r32(p +  9));
    prog.thirst    = u2f(r32(p + 13));
    prog.stamina   = u2f(r32(p + 17));
    prog.radiation = u2f(r32(p + 21));
    prog.level     = static_cast<int32_t>(r32(p + 25));
    prog.xp        = u2f(r32(p + 29));
    prog.posX      = u2f(r32(p + 33));
    prog.posY      = u2f(r32(p + 37));
    prog.posZ      = u2f(r32(p + 41));
    prog.yaw       = u2f(r32(p + 45));
    const uint16_t containerCount = r16(p + 49);

    const float vals[] = { prog.health, prog.hunger, prog.thirst, prog.stamina,
                            prog.radiation, prog.xp,
                            prog.posX, prog.posY, prog.posZ, prog.yaw };
    for (float v : vals)
        if (!std::isfinite(v)) return std::nullopt;

    size_t off = PLAYER_PROGRESS_HEADER_SIZE;
    prog.containers.reserve(containerCount);
    for (uint16_t c = 0; c < containerCount; ++c) {
        if (off + 6 > n) return std::nullopt;
        InventoryContainer container;
        container.columns = r16(p + off); off += 2;
        container.rows    = r16(p + off); off += 2;
        const uint16_t itemCount = r16(p + off); off += 2;

        container.items.reserve(itemCount);
        for (uint16_t i = 0; i < itemCount; ++i) {
            if (off + 3 > n) return std::nullopt;
            InventorySlot item;
            item.slotIndex = p[off++];
            const uint16_t idLen = r16(p + off); off += 2;
            if (off + idLen + 2 > n) return std::nullopt;
            item.itemId = std::string(reinterpret_cast<const char*>(p + off), idLen);
            off += idLen;
            item.quantity = r16(p + off); off += 2;
            container.items.push_back(std::move(item));
        }
        prog.containers.push_back(std::move(container));
    }

    // Extended stats trailer — optional; absent entirely on payloads persisted
    // before gap 4/7 landed, so a short remainder just leaves the defaults.
    auto read_str = [&](std::string& out) -> bool {
        if (off + 2 > n) return false;
        const uint16_t len = r16(p + off); off += 2;
        if (off + len > n) return false;
        out = std::string(reinterpret_cast<const char*>(p + off), len);
        off += len;
        return true;
    };
    if (read_str(prog.forename) && read_str(prog.surname) && off + 28 <= n) {
        prog.zombieKills           = static_cast<int32_t>(r32(p + off)); off += 4;
        prog.daysSurvived          = static_cast<int32_t>(r32(p + off)); off += 4;
        prog.bossZombieKills       = static_cast<int32_t>(r32(p + off)); off += 4;
        prog.animalKills           = static_cast<int32_t>(r32(p + off)); off += 4;
        prog.humanKills            = static_cast<int32_t>(r32(p + off)); off += 4;
        prog.distanceTravelled     = u2f(r32(p + off));                  off += 4;
        prog.infestationsDestroyed = static_cast<int32_t>(r32(p + off)); off += 4;
    }

    return prog;
}

// ---------------------------------------------------------------------------
// Equipment  (from BP_JigHelperComp_C.ServerEquippedItems)
// Header: [tag=1][slotCount:u16BE]
// Per slot: [slotIndex:u8][itemIdLen:u16BE][itemId...]
// ---------------------------------------------------------------------------

std::vector<uint8_t> encode_equipment(const Equipment& e)
{
    size_t total = 3; // tag + slotCount
    for (const auto& slot : e.slots)
        total += 1 + 2 + slot.itemId.size();

    std::vector<uint8_t> buf(total);
    uint8_t* p = buf.data();

    p[0] = 1;
    w16(p + 1, static_cast<uint16_t>(e.slots.size()));

    size_t off = 3;
    for (const auto& slot : e.slots) {
        buf[off++] = slot.slotIndex;
        const uint16_t idLen = static_cast<uint16_t>(slot.itemId.size());
        w16(buf.data() + off, idLen); off += 2;
        if (idLen) {
            std::memcpy(buf.data() + off, slot.itemId.data(), idLen);
            off += idLen;
        }
    }

    return buf;
}

std::optional<Equipment> decode_equipment(const uint8_t* p, size_t n)
{
    if (n < 3 || p[0] != 1) return std::nullopt;

    Equipment e;
    const uint16_t slotCount = r16(p + 1);
    size_t off = 3;
    e.slots.reserve(slotCount);
    for (uint16_t i = 0; i < slotCount; ++i) {
        if (off + 3 > n) return std::nullopt;
        EquipmentSlot slot;
        slot.slotIndex = p[off++];
        const uint16_t idLen = r16(p + off); off += 2;
        if (off + idLen > n) return std::nullopt;
        slot.itemId = std::string(reinterpret_cast<const char*>(p + off), idLen);
        off += idLen;
        e.slots.push_back(std::move(slot));
    }

    return e;
}

std::vector<uint8_t> encode_weapon_attachments(const WeaponAttachments& a)
{
    size_t total = 3; // tag + entryCount
    for (const auto& e : a.entries)
        total += 1 + 1 + 2 + e.itemId.size();

    std::vector<uint8_t> buf(total);
    uint8_t* p = buf.data();

    p[0] = 1;
    w16(p + 1, static_cast<uint16_t>(a.entries.size()));

    size_t off = 3;
    for (const auto& e : a.entries) {
        buf[off++] = e.weaponSlotIndex;
        buf[off++] = e.containerIndex;
        const uint16_t idLen = static_cast<uint16_t>(e.itemId.size());
        w16(buf.data() + off, idLen); off += 2;
        if (idLen) {
            std::memcpy(buf.data() + off, e.itemId.data(), idLen);
            off += idLen;
        }
    }

    return buf;
}

std::optional<WeaponAttachments> decode_weapon_attachments(const uint8_t* p, size_t n)
{
    if (n < 3 || p[0] != 1) return std::nullopt;

    WeaponAttachments a;
    const uint16_t entryCount = r16(p + 1);
    size_t off = 3;
    a.entries.reserve(entryCount);
    for (uint16_t i = 0; i < entryCount; ++i) {
        if (off + 4 > n) return std::nullopt;
        WeaponAttachmentEntry e;
        e.weaponSlotIndex = p[off++];
        e.containerIndex  = p[off++];
        const uint16_t idLen = r16(p + off); off += 2;
        if (off + idLen > n) return std::nullopt;
        e.itemId = std::string(reinterpret_cast<const char*>(p + off), idLen);
        off += idLen;
        a.entries.push_back(std::move(e));
    }

    return a;
}

std::vector<uint8_t> encode_pawn_appearance(const PawnAppearance& a)
{
    size_t total = 1 + 1
                 + 2 + a.hairMeshName.size()
                 + 2 + a.hairColorName.size()
                 + 2 + a.beardMeshName.size()
                 + 2 + a.beardColorName.size()
                 + 2 + a.skinColorName.size()
                 + 2 + a.mouthMeshName.size()
                 + 2 + a.eyebrowsMeshName.size()
                 + 2 + a.accessory1MeshName.size()
                 + 2 + a.accessory2MeshName.size()
                 + 2 + a.accessory3MeshName.size();
    for (const auto& s : a.bodyPartMeshNames) total += 2 + s.size();

    std::vector<uint8_t> buf(total);
    uint8_t* p = buf.data();

    p[0] = 1;
    p[1] = a.isMale ? 1 : 0;

    size_t off = 2;
    auto writeStr = [&](const std::string& s) {
        w16(buf.data() + off, static_cast<uint16_t>(s.size())); off += 2;
        if (!s.empty()) { std::memcpy(buf.data() + off, s.data(), s.size()); off += s.size(); }
    };
    writeStr(a.hairMeshName);
    writeStr(a.hairColorName);
    writeStr(a.beardMeshName);
    writeStr(a.beardColorName);
    writeStr(a.skinColorName);
    for (const auto& s : a.bodyPartMeshNames) writeStr(s);
    writeStr(a.mouthMeshName);
    writeStr(a.eyebrowsMeshName);
    writeStr(a.accessory1MeshName);
    writeStr(a.accessory2MeshName);
    writeStr(a.accessory3MeshName);

    return buf;
}

std::optional<PawnAppearance> decode_pawn_appearance(const uint8_t* p, size_t n)
{
    if (n < 2 || p[0] != 1) return std::nullopt;

    PawnAppearance a;
    a.isMale = p[1] != 0;

    size_t off = 2;
    auto readStr = [&](std::string& out) -> bool {
        if (off + 2 > n) return false;
        const uint16_t len = r16(p + off); off += 2;
        if (off + len > n) return false;
        out = std::string(reinterpret_cast<const char*>(p + off), len);
        off += len;
        return true;
    };
    if (!readStr(a.hairMeshName))  return std::nullopt;
    if (!readStr(a.hairColorName)) return std::nullopt;
    if (!readStr(a.beardMeshName)) return std::nullopt;
    if (!readStr(a.beardColorName)) return std::nullopt;
    if (!readStr(a.skinColorName)) return std::nullopt;
    for (auto& s : a.bodyPartMeshNames) {
        if (!readStr(s)) return std::nullopt;
    }
    if (!readStr(a.mouthMeshName))    return std::nullopt;
    if (!readStr(a.eyebrowsMeshName)) return std::nullopt;
    if (!readStr(a.accessory1MeshName)) return std::nullopt;
    if (!readStr(a.accessory2MeshName)) return std::nullopt;
    if (!readStr(a.accessory3MeshName)) return std::nullopt;

    return a;
}

// ---------------------------------------------------------------------------
// PlayMontage: [nameLen:u16BE][name utf8][playRate:f32BE]
// ---------------------------------------------------------------------------

std::vector<uint8_t> encode_play_montage(const PlayMontageData& m)
{
    std::vector<uint8_t> buf(2 + m.montageName.size() + 4);
    w16(buf.data(), static_cast<uint16_t>(m.montageName.size()));
    std::memcpy(buf.data() + 2, m.montageName.data(), m.montageName.size());
    w32(buf.data() + 2 + m.montageName.size(), f2u(m.playRate));
    return buf;
}

std::optional<PlayMontageData> decode_play_montage(const uint8_t* p, size_t n)
{
    if (n < 2) return std::nullopt;
    const uint16_t len = r16(p);
    if (n < static_cast<size_t>(2 + len + 4)) return std::nullopt;

    PlayMontageData m;
    m.montageName = std::string(reinterpret_cast<const char*>(p + 2), len);
    m.playRate    = u2f(r32(p + 2 + len));
    return m;
}

// ---------------------------------------------------------------------------
// now_micros
// ---------------------------------------------------------------------------

uint64_t now_micros()
{
#ifdef _WIN32
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    const uint64_t ft64 = (static_cast<uint64_t>(ft.dwHighDateTime) << 32)
                        |  static_cast<uint64_t>(ft.dwLowDateTime);
    return (ft64 - 116444736000000000ULL) / 10;
#else
    struct timespec ts{};
    clock_gettime(CLOCK_REALTIME, &ts);
    return static_cast<uint64_t>(ts.tv_sec) * 1'000'000ULL
         + static_cast<uint64_t>(ts.tv_nsec) / 1'000ULL;
#endif
}

} // namespace sdb
