'use strict';

// Same wire format as runtime/src/protocol.js — kept as a separate copy so
// the server directory is independently deployable without the runtime tree.

const FRAME_MAGIC      = 0x53444F33;
const PROTOCOL_VERSION = 3;
const HEADER_SIZE      = 88;
const MAX_PAYLOAD      = 65536;

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

function decodeFrame(buf) {
    if (buf.length < HEADER_SIZE) return null;
    const magic = buf.readUInt32BE(0);
    if (magic !== FRAME_MAGIC) throw new Error(`bad_magic: 0x${magic.toString(16)}`);
    const payloadLength = buf.readUInt32BE(12);
    if (payloadLength > MAX_PAYLOAD) throw new Error(`payload_too_large: ${payloadLength}`);
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
    if (f.sessionId) f.sessionId.copy(out, 24); else out.fill(0, 24, 40);
    if (f.worldId)   f.worldId.copy(out, 40);   else out.fill(0, 40, 56);
    out.writeBigUInt64BE(f.playerId || 0n, 56);
    out.writeBigUInt64BE(f.entityId || 0n, 64);
    out.writeUInt32BE(f.sequence || 0, 72);
    out.writeUInt32BE(f.tick || 0, 76);
    out.writeBigUInt64BE(f.timestampMicros || BigInt(Date.now()) * 1000n, 80);
    payload.copy(out, HEADER_SIZE);
    return out;
}

function encodeString(str, maxBytes) {
    const bytes = Buffer.from(str, 'utf8').subarray(0, maxBytes);
    const out = Buffer.allocUnsafe(2 + bytes.length);
    out.writeUInt16BE(bytes.length, 0);
    bytes.copy(out, 2);
    return out;
}

function decodeString(buf, offset = 0) {
    const len = buf.readUInt16BE(offset);
    return buf.subarray(offset + 2, offset + 2 + len).toString('utf8');
}

// ── Entity type constants ──────────────────────────────────────────────────────
// These values appear in EntitySpawn payload byte 1.
const EntityType = Object.freeze({
    LOOT_ITEM:      0,   // dropped or spawned item pickup
    BUILDING_PIECE: 1,   // player-placed structure
    CONTAINER:      2,   // loot crate / chest
});

// ── Interaction type constants ────────────────────────────────────────────────
// These values appear in InteractionRequest payload byte 1.
const InteractionType = Object.freeze({
    BUILD: 1,   // client requests placement of a building piece
    LOOT:  2,   // client opens a container
    USE:   3,   // generic use / activate
});

// ── EntitySpawn payload ───────────────────────────────────────────────────────
// Byte layout:
//   0      uint8   format version (1)
//   1      uint8   entityType (EntityType constant)
//   2–5    float32 posX
//   6–9    float32 posY
//   10–13  float32 posZ
//   14–17  float32 yaw
//   18–19  uint16  stateLength
//   20…    bytes   state[stateLength]   (entity-type-specific, see below)
//
// state for LOOT_ITEM:      uint32 itemId  +  uint16 quantity
// state for BUILDING_PIECE: uint32 pieceTypeId  +  uint8 health (0-255)
// state for CONTAINER:      uint32 containerTypeId

function encodeEntitySpawn(entityType, posX, posY, posZ, yaw, state) {
    const stateLen = state ? state.length : 0;
    const buf = Buffer.allocUnsafe(20 + stateLen);
    buf.writeUInt8(1, 0);
    buf.writeUInt8(entityType, 1);
    buf.writeFloatBE(posX,  2);
    buf.writeFloatBE(posY,  6);
    buf.writeFloatBE(posZ, 10);
    buf.writeFloatBE(yaw,  14);
    buf.writeUInt16BE(stateLen, 18);
    if (state && stateLen > 0) state.copy(buf, 20);
    return buf;
}

function decodeEntitySpawn(payload) {
    if (payload.length < 20) throw new Error('entity_spawn_too_short');
    const stateLen = payload.readUInt16BE(18);
    return {
        entityType: payload.readUInt8(1),
        posX:       payload.readFloatBE(2),
        posY:       payload.readFloatBE(6),
        posZ:       payload.readFloatBE(10),
        yaw:        payload.readFloatBE(14),
        state:      Buffer.from(payload.subarray(20, 20 + stateLen)),
    };
}

// Entity-type-specific state blobs
function encodeLootState(itemId, quantity) {
    const b = Buffer.allocUnsafe(6);
    b.writeUInt32BE(itemId,   0);
    b.writeUInt16BE(quantity, 4);
    return b;
}
function decodeLootState(state) {
    return { itemId: state.readUInt32BE(0), quantity: state.readUInt16BE(4) };
}

function encodeBuildingState(pieceTypeId, health = 255) {
    const b = Buffer.allocUnsafe(5);
    b.writeUInt32BE(pieceTypeId, 0);
    b.writeUInt8(health, 4);
    return b;
}
function decodeBuildingState(state) {
    return { pieceTypeId: state.readUInt32BE(0), health: state.readUInt8(4) };
}

// ── PlayerProgress payload (ProfileRevision / PlayerProgressRestore) ──────────
// Same format both directions: ProfileRevision is client→server, and the
// gateway persists that payload verbatim and replays it byte-for-byte as
// PlayerProgressRestore on rejoin (see gateway.js) — so both encode/decode
// here must stay in lockstep with the C++ side's encode_player_progress /
// decode_player_progress in src/protocol.cpp.
// Byte layout:
//   0      uint8   format version (1)
//   1–4    uint32  revision (monotonically increasing)
//   5–8    float32 health  (0.0–1.0)
//   9–12   float32 hunger
//   13–16  float32 thirst
//   17–20  float32 stamina
//   21–24  float32 radiation
//   25–28  uint32  level
//   29–32  float32 xp
//   33–36  float32 posX
//   37–40  float32 posY
//   41–44  float32 posZ
//   45–48  float32 yaw
//   49–50  uint16  slotCount
//   51…    slots: [uint8 slotIndex, uint16 itemIdLen, itemId utf8, uint16 quantity] × slotCount

const PLAYER_PROGRESS_HEADER_SIZE = 51;

function encodePlayerProgress({ revision, health, hunger, thirst, stamina, radiation, level, xp,
                                 posX, posY, posZ, yaw, slots }) {
    const itemIdBufs = slots.map(s => Buffer.from(s.itemId, 'utf8'));
    const slotsSize  = itemIdBufs.reduce((sum, b) => sum + 1 + 2 + b.length + 2, 0);
    const buf = Buffer.allocUnsafe(PLAYER_PROGRESS_HEADER_SIZE + slotsSize);
    buf.writeUInt8(1, 0);
    buf.writeUInt32BE(revision, 1);
    buf.writeFloatBE(health,    5);
    buf.writeFloatBE(hunger,    9);
    buf.writeFloatBE(thirst,   13);
    buf.writeFloatBE(stamina,  17);
    buf.writeFloatBE(radiation,21);
    buf.writeUInt32BE(level,   25);
    buf.writeFloatBE(xp,       29);
    buf.writeFloatBE(posX,     33);
    buf.writeFloatBE(posY,     37);
    buf.writeFloatBE(posZ,     41);
    buf.writeFloatBE(yaw,      45);
    buf.writeUInt16BE(slots.length, 49);

    let o = PLAYER_PROGRESS_HEADER_SIZE;
    for (let i = 0; i < slots.length; i++) {
        buf.writeUInt8(slots[i].slotIndex, o); o += 1;
        buf.writeUInt16BE(itemIdBufs[i].length, o); o += 2;
        itemIdBufs[i].copy(buf, o); o += itemIdBufs[i].length;
        buf.writeUInt16BE(slots[i].quantity, o); o += 2;
    }
    return buf;
}

function decodePlayerProgress(payload) {
    if (payload.length < PLAYER_PROGRESS_HEADER_SIZE) throw new Error('progress_too_short');

    const slotCount = payload.readUInt16BE(49);
    const slots = [];
    let o = PLAYER_PROGRESS_HEADER_SIZE;
    for (let i = 0; i < slotCount; i++) {
        if (o + 3 > payload.length) throw new Error('progress_slot_truncated');
        const slotIndex = payload.readUInt8(o); o += 1;
        const idLen     = payload.readUInt16BE(o); o += 2;
        if (o + idLen + 2 > payload.length) throw new Error('progress_slot_truncated');
        const itemId    = payload.toString('utf8', o, o + idLen); o += idLen;
        const quantity  = payload.readUInt16BE(o); o += 2;
        slots.push({ slotIndex, itemId, quantity });
    }
    return {
        revision:  payload.readUInt32BE(1),
        health:    payload.readFloatBE(5),
        hunger:    payload.readFloatBE(9),
        thirst:    payload.readFloatBE(13),
        stamina:   payload.readFloatBE(17),
        radiation: payload.readFloatBE(21),
        level:     payload.readUInt32BE(25),
        xp:        payload.readFloatBE(29),
        posX:      payload.readFloatBE(33),
        posY:      payload.readFloatBE(37),
        posZ:      payload.readFloatBE(41),
        yaw:       payload.readFloatBE(45),
        slots,
    };
}

// ── ItemDropRequest payload ───────────────────────────────────────────────────
// Byte layout:
//   0      uint8   format version (1)
//   1      uint8   slotIndex
//   2–3    uint16  quantity
//   4–7    float32 posX  (world position to drop at)
//   8–11   float32 posY
//   12–15  float32 posZ

function decodeItemDropRequest(payload) {
    if (payload.length < 16) throw new Error('drop_request_too_short');
    return {
        slotIndex: payload.readUInt8(1),
        quantity:  payload.readUInt16BE(2),
        posX:      payload.readFloatBE(4),
        posY:      payload.readFloatBE(8),
        posZ:      payload.readFloatBE(12),
    };
}

// ── ItemPickupRequest payload ─────────────────────────────────────────────────
// Byte layout:
//   0      uint8   format version (1)
//   1      uint8   targetSlot  (0xFF = auto-assign first empty slot)

function decodeItemPickupRequest(payload) {
    if (payload.length < 2) throw new Error('pickup_request_too_short');
    return { targetSlot: payload.readUInt8(1) };
}

// ── ItemPickupResult payload ──────────────────────────────────────────────────
// On success (9 bytes): version=1, success=1, slotIndex, uint32 itemId, uint16 quantity
// On failure (3 bytes): version=1, success=0, reason (0=entity_gone, 1=inv_full)

function encodeItemPickupResult({ success, slot, itemId, quantity, reason }) {
    if (success) {
        const b = Buffer.allocUnsafe(9);
        b.writeUInt8(1, 0); b.writeUInt8(1, 1); b.writeUInt8(slot, 2);
        b.writeUInt32BE(itemId, 3); b.writeUInt16BE(quantity, 7);
        return b;
    }
    const b = Buffer.allocUnsafe(3);
    b.writeUInt8(1, 0); b.writeUInt8(0, 1); b.writeUInt8(reason ?? 0, 2);
    return b;
}

// ── ItemDropResult payload ────────────────────────────────────────────────────
// 3 bytes: version=1, success (0/1), reason on fail (0=slot_empty, 1=bad_slot)

function encodeItemDropResult({ success, reason }) {
    const b = Buffer.allocUnsafe(3);
    b.writeUInt8(1, 0); b.writeUInt8(success ? 1 : 0, 1); b.writeUInt8(reason ?? 0, 2);
    return b;
}

// ── InteractionRequest payload ────────────────────────────────────────────────
// Byte layout:
//   0      uint8   format version (1)
//   1      uint8   interactionType (InteractionType constant)
//   2…     type-specific data
//
// For BUILD (interactionType=1):
//   2–5    uint32  pieceTypeId
//   6–9    float32 posX
//   10–13  float32 posY
//   14–17  float32 posZ
//   18–21  float32 yaw

function decodeInteractionRequest(payload) {
    if (payload.length < 2) throw new Error('interaction_too_short');
    const interactionType = payload.readUInt8(1);
    if (interactionType === InteractionType.BUILD) {
        if (payload.length < 22) throw new Error('build_request_too_short');
        return {
            interactionType,
            pieceTypeId: payload.readUInt32BE(2),
            posX:        payload.readFloatBE(6),
            posY:        payload.readFloatBE(10),
            posZ:        payload.readFloatBE(14),
            yaw:         payload.readFloatBE(18),
        };
    }
    return { interactionType };
}

// ── InteractionResult payload ─────────────────────────────────────────────────
// 3 bytes: version=1, success (0/1), interactionType

function encodeInteractionResult({ success, interactionType }) {
    const b = Buffer.allocUnsafe(3);
    b.writeUInt8(1, 0); b.writeUInt8(success ? 1 : 0, 1); b.writeUInt8(interactionType, 2);
    return b;
}

module.exports = {
    FRAME_MAGIC, PROTOCOL_VERSION, HEADER_SIZE, MAX_PAYLOAD,
    MsgType,
    EntityType,
    InteractionType,
    decodeFrame, encodeFrame, encodeString, decodeString,
    encodeEntitySpawn, decodeEntitySpawn,
    encodeLootState, decodeLootState,
    encodeBuildingState, decodeBuildingState,
    encodePlayerProgress, decodePlayerProgress,
    decodeItemDropRequest, decodeItemPickupRequest,
    encodeItemPickupResult, encodeItemDropResult,
    decodeInteractionRequest, encodeInteractionResult,
};
