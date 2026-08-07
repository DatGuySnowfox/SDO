'use strict';

// Wire protocol v3 — matches the binary format used by UE4SS.dll ↔ runtime ↔ gateway.
// All multi-byte integers are big-endian.
//
// Frame layout:
//   [0]  4  UInt32BE  MAGIC = 0x53444F33
//   [4]  2  UInt16BE  PROTOCOL_VERSION = 3
//   [6]  2  UInt16BE  type  (MsgType)
//   [8]  2  UInt16BE  flags
//  [10]  2  UInt16BE  headerLength = 88  (constant)
//  [12]  4  UInt32BE  payloadLength  (0..65536)
//  [16]  8  UInt64BE  connectionId
//  [24] 16  bytes     sessionId  (UUID raw bytes)
//  [40] 16  bytes     worldId    (UUID raw bytes)
//  [56]  8  UInt64BE  playerId
//  [64]  8  UInt64BE  entityId
//  [72]  4  UInt32BE  sequence
//  [76]  4  UInt32BE  tick
//  [80]  8  UInt64BE  timestampMicros
//  [88]  N  bytes     payload (type-specific)

const FRAME_MAGIC       = 0x53444F33;
const PROTOCOL_VERSION  = 3;
const HEADER_SIZE       = 88;
const MAX_PAYLOAD       = 65536;

const MsgType = Object.freeze({
    HostAuthenticate:       1,
    ClientAuthenticate:     2,
    AuthenticationAccepted: 3,
    AuthenticationRejected: 4,
    HostHeartbeat:          5,
    ClientHeartbeat:        6,
    JoinRequest:           10,
    JoinAccepted:          11,
    JoinRejected:          12,
    PlayerConnected:       13,
    PlayerDisconnected:    14,
    Movement:              20,
    Equipment:             21,
    Death:                 22,
    Respawn:               23,
    ProfileRevision:       24,
    SaveAcknowledgement:   25,
    InteractionRequest:    26,
    InteractionResult:     27,
    DeathRequest:          28,
    RespawnRequest:        29,
    WorldState:            30,
    EntitySpawn:           31,
    EntityState:           32,
    EntityDespawn:         33,
    ItemDropRequest:       34,
    ItemPickupRequest:     35,
    ItemPickupResult:      36,
    ZombieAttackRequest:   37,
    ZombieDamageResult:    38,
    ItemDropResult:        39,
    PlayerDamage:          40,
    PlayerProgressRestore: 42,
});

// Returns a parsed frame object, or null if buf is too short.
// Throws on bad magic or oversized payload.
function decodeFrame(buf) {
    if (buf.length < HEADER_SIZE) return null;

    const magic = buf.readUInt32BE(0);
    if (magic !== FRAME_MAGIC)
        throw new Error(`bad_magic: 0x${magic.toString(16)}`);

    const payloadLength = buf.readUInt32BE(12);
    if (payloadLength > MAX_PAYLOAD)
        throw new Error(`payload_too_large: ${payloadLength}`);
    if (buf.length < HEADER_SIZE + payloadLength) return null;

    return {
        type:            buf.readUInt16BE(6),
        flags:           buf.readUInt16BE(8),
        connectionId:    buf.readBigUInt64BE(16),
        sessionId:       Buffer.from(buf.subarray(24, 40)),
        worldId:         Buffer.from(buf.subarray(40, 56)),
        playerId:        buf.readBigUInt64BE(56),
        entityId:        buf.readBigUInt64BE(64),
        sequence:        buf.readUInt32BE(72),
        tick:            buf.readUInt32BE(76),
        timestampMicros: buf.readBigUInt64BE(80),
        payload:         Buffer.from(buf.subarray(HEADER_SIZE, HEADER_SIZE + payloadLength)),
    };
}

// Encodes a frame object to a Buffer ready for transmission.
function encodeFrame(f) {
    const payload = f.payload || Buffer.alloc(0);
    const out = Buffer.allocUnsafe(HEADER_SIZE + payload.length);

    out.writeUInt32BE(FRAME_MAGIC, 0);
    out.writeUInt16BE(PROTOCOL_VERSION, 4);
    out.writeUInt16BE(f.type, 6);
    out.writeUInt16BE(f.flags || 0, 8);
    out.writeUInt16BE(HEADER_SIZE, 10);
    out.writeUInt32BE(payload.length, 12);
    out.writeBigUInt64BE(f.connectionId || 0n, 16);
    if (f.sessionId) f.sessionId.copy(out, 24);
    else out.fill(0, 24, 40);
    if (f.worldId) f.worldId.copy(out, 40);
    else out.fill(0, 40, 56);
    out.writeBigUInt64BE(f.playerId || 0n, 56);
    out.writeBigUInt64BE(f.entityId || 0n, 64);
    out.writeUInt32BE(f.sequence || 0, 72);
    out.writeUInt32BE(f.tick || 0, 76);
    out.writeBigUInt64BE(f.timestampMicros || BigInt(Date.now()) * 1000n, 80);
    payload.copy(out, HEADER_SIZE);

    return out;
}

// Encode a UTF-8 string with a 2-byte length prefix (used in several payload types).
function encodeString(str, maxBytes) {
    const bytes = Buffer.from(str, 'utf8').subarray(0, maxBytes);
    const out = Buffer.allocUnsafe(2 + bytes.length);
    out.writeUInt16BE(bytes.length, 0);
    bytes.copy(out, 2);
    return out;
}

// Decode a length-prefixed string at a given offset into a Buffer.
function decodeString(buf, offset = 0) {
    const len = buf.readUInt16BE(offset);
    return buf.subarray(offset + 2, offset + 2 + len).toString('utf8');
}

// ── Entity / inventory constants ──────────────────────────────────────────────

const EntityType = Object.freeze({ LOOT_ITEM: 0, BUILDING_PIECE: 1, CONTAINER: 2 });
const InteractionType = Object.freeze({ BUILD: 1, LOOT: 2, USE: 3 });

module.exports = {
    FRAME_MAGIC, PROTOCOL_VERSION, HEADER_SIZE, MAX_PAYLOAD,
    MsgType, EntityType, InteractionType,
    decodeFrame, encodeFrame, encodeString, decodeString,
};
