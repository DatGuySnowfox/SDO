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
//         [posX:f32][posY:f32][posZ:f32][yaw:f32][slotCount:u16]  = 51 bytes
// Per slot: [slotIndex:u8][itemIdLen:u16BE][itemId...][qty:u16BE]
// ---------------------------------------------------------------------------

static constexpr size_t PLAYER_PROGRESS_HEADER_SIZE = 51;

std::vector<uint8_t> encode_player_progress(const PlayerProgress& prog)
{
    size_t total = PLAYER_PROGRESS_HEADER_SIZE;
    for (const auto& slot : prog.slots)
        total += 1 + 2 + slot.itemId.size() + 2;

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
    w16(p + 49, static_cast<uint16_t>(prog.slots.size()));

    size_t off = PLAYER_PROGRESS_HEADER_SIZE;
    for (const auto& slot : prog.slots) {
        buf[off++] = slot.slotIndex;
        const uint16_t idLen = static_cast<uint16_t>(slot.itemId.size());
        w16(buf.data() + off, idLen); off += 2;
        if (idLen) {
            std::memcpy(buf.data() + off, slot.itemId.data(), idLen);
            off += idLen;
        }
        w16(buf.data() + off, slot.quantity); off += 2;
    }

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
    const uint16_t slotCount = r16(p + 49);

    const float vals[] = { prog.health, prog.hunger, prog.thirst, prog.stamina,
                            prog.radiation, prog.xp,
                            prog.posX, prog.posY, prog.posZ, prog.yaw };
    for (float v : vals)
        if (!std::isfinite(v)) return std::nullopt;

    size_t off = PLAYER_PROGRESS_HEADER_SIZE;
    prog.slots.reserve(slotCount);
    for (uint16_t i = 0; i < slotCount; ++i) {
        if (off + 3 > n) return std::nullopt;
        InventorySlot slot;
        slot.slotIndex = p[off++];
        const uint16_t idLen = r16(p + off); off += 2;
        if (off + idLen + 2 > n) return std::nullopt;
        slot.itemId = std::string(reinterpret_cast<const char*>(p + off), idLen);
        off += idLen;
        slot.quantity = r16(p + off); off += 2;
        prog.slots.push_back(std::move(slot));
    }

    return prog;
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
