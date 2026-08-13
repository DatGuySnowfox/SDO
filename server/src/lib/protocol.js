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
    WeaponAttachments:     43,
    PawnAppearance:        44,
    PlayMontage:           45,
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

// ── Entity kind constants ──────────────────────────────────────────────────────
// Must match src/protocol.hpp's `enum class EntityKind` numeric values exactly
// (the client decodes this byte directly into that enum) — NOT the same
// numbering as the old, unused EntityType/WorldEntityKind design.
const EntityKind = Object.freeze({
    Unknown:         0,
    Zombie:          1,
    GroundItem:      2,   // dropped or spawned item pickup
    Vehicle:         3,
    PlacedStructure: 4,   // player-placed building piece
});

// ── Interaction type constants ────────────────────────────────────────────────
// These values appear in InteractionRequest payload byte 1.
const InteractionType = Object.freeze({
    BUILD: 1,   // client requests placement of a building piece
    LOOT:  2,   // client opens a container
    USE:   3,   // generic use / activate
});

// ── EntitySpawn payload (entity descriptor — NO position, see EntityState) ────
// Must match src/protocol.cpp's decode_entity_descriptor exactly.
// Byte layout:
//   0      uint8   format version (1)
//   1      uint8   kind (EntityKind constant)
//   2–5    uint32  revision
//   6–7    uint16  quantity
//   8–15   uint64  ownerPlayerId
//   16–17  uint16  classPathLen
//   18…    bytes   classPath utf8
//   …–…+1  uint16  itemIdLen
//   …      bytes   itemId utf8
//
// classPath is left empty for items — the client resolves the pickup
// Blueprint locally from itemId (it already has the DataAsset loaded), so
// the server doesn't need an itemId->asset-path table at all.
function encodeEntityDescriptor({ kind, revision = 1, quantity = 0, ownerPlayerId = 0n,
                                   classPath = '', itemId = '' }) {
    const classPathBuf = Buffer.from(classPath, 'utf8');
    const itemIdBuf    = Buffer.from(itemId, 'utf8');
    const buf = Buffer.allocUnsafe(18 + classPathBuf.length + 2 + itemIdBuf.length);
    buf.writeUInt8(1, 0);
    buf.writeUInt8(kind, 1);
    buf.writeUInt32BE(revision, 2);
    buf.writeUInt16BE(quantity, 6);
    buf.writeBigUInt64BE(BigInt(ownerPlayerId), 8);
    buf.writeUInt16BE(classPathBuf.length, 16);
    classPathBuf.copy(buf, 18);
    buf.writeUInt16BE(itemIdBuf.length, 18 + classPathBuf.length);
    itemIdBuf.copy(buf, 18 + classPathBuf.length + 2);
    return buf;
}

// ── EntityState payload (position/health — exactly 27 bytes) ──────────────────
// Must match src/protocol.cpp's decode_entity_state exactly.
// Byte layout: [tag=1][kind:u8][revision:u32BE][x/y/z/yaw/health:5×f32BE][state:u8]
function encodeEntityState({ kind, revision = 1, x, y, z, yaw = 0, health = 0, state = 0 }) {
    const buf = Buffer.allocUnsafe(27);
    buf.writeUInt8(1, 0);
    buf.writeUInt8(kind, 1);
    buf.writeUInt32BE(revision, 2);
    buf.writeFloatBE(x,      6);
    buf.writeFloatBE(y,     10);
    buf.writeFloatBE(z,     14);
    buf.writeFloatBE(yaw,   18);
    buf.writeFloatBE(health, 22);
    buf.writeUInt8(state, 26);
    return buf;
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
//   49–50  uint16  containerCount
//   51…    containers: [uint16 columns, uint16 rows, uint16 itemCount,
//                        items: [uint8 slotIndex, uint16 itemIdLen, itemId utf8, uint16 quantity] × itemCount
//                       ] × containerCount
//   …      extended stats trailer (gap 4/7), optional — a payload persisted before this trailer
//          existed just ends after the containers, and decode must tolerate that (see decodePlayerProgress):
//            uint16  forenameLen, forename utf8
//            uint16  surnameLen,  surname utf8
//            uint32  zombieKills
//            uint32  daysSurvived
//            uint32  bossZombieKills
//            uint32  animalKills
//            uint32  humanKills
//            float32 distanceTravelled
//            uint32  infestationsDestroyed
//
// Gap 11 (2026-08-10): containers replaced a flat, globally-indexed slot list
// capped at 40 — the game's own inventory has no fixed slot count and each
// container is independently resizable, so a flat cap was solving the wrong
// problem. Breaking wire-format change — payloads persisted under the old
// flat format will not decode correctly against this container list (dev-stage
// mod, no migration provided).

const PLAYER_PROGRESS_HEADER_SIZE = 51;

function encodePlayerProgress({ revision, health, hunger, thirst, stamina, radiation, level, xp,
                                 posX, posY, posZ, yaw, containers,
                                 forename = '', surname = '', zombieKills = 0, daysSurvived = 0,
                                 bossZombieKills = 0, animalKills = 0, humanKills = 0,
                                 distanceTravelled = 0, infestationsDestroyed = 0 }) {
    const containerItemIdBufs = containers.map(c => c.items.map(it => Buffer.from(it.itemId, 'utf8')));
    const containersSize = containers.reduce((sum, c, ci) =>
        sum + 2 + 2 + 2 + containerItemIdBufs[ci].reduce((s, b) => s + 1 + 2 + b.length + 2, 0), 0);
    const forenameBuf = Buffer.from(forename, 'utf8');
    const surnameBuf  = Buffer.from(surname, 'utf8');
    const trailerSize = 2 + forenameBuf.length + 2 + surnameBuf.length + 4 * 6 + 4;
    const buf = Buffer.allocUnsafe(PLAYER_PROGRESS_HEADER_SIZE + containersSize + trailerSize);
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
    buf.writeUInt16BE(containers.length, 49);

    let o = PLAYER_PROGRESS_HEADER_SIZE;
    for (let ci = 0; ci < containers.length; ci++) {
        const container = containers[ci];
        const itemIdBufs = containerItemIdBufs[ci];
        buf.writeUInt16BE(container.columns, o); o += 2;
        buf.writeUInt16BE(container.rows,    o); o += 2;
        buf.writeUInt16BE(container.items.length, o); o += 2;
        for (let i = 0; i < container.items.length; i++) {
            buf.writeUInt8(container.items[i].slotIndex, o); o += 1;
            buf.writeUInt16BE(itemIdBufs[i].length, o); o += 2;
            itemIdBufs[i].copy(buf, o); o += itemIdBufs[i].length;
            buf.writeUInt16BE(container.items[i].quantity, o); o += 2;
        }
    }

    buf.writeUInt16BE(forenameBuf.length, o); o += 2;
    forenameBuf.copy(buf, o); o += forenameBuf.length;
    buf.writeUInt16BE(surnameBuf.length, o); o += 2;
    surnameBuf.copy(buf, o); o += surnameBuf.length;
    buf.writeUInt32BE(zombieKills, o);              o += 4;
    buf.writeUInt32BE(daysSurvived, o);              o += 4;
    buf.writeUInt32BE(bossZombieKills, o);           o += 4;
    buf.writeUInt32BE(animalKills, o);               o += 4;
    buf.writeUInt32BE(humanKills, o);                o += 4;
    buf.writeFloatBE(distanceTravelled, o);          o += 4;
    buf.writeUInt32BE(infestationsDestroyed, o);     o += 4;
    return buf;
}

function decodePlayerProgress(payload) {
    if (payload.length < PLAYER_PROGRESS_HEADER_SIZE) throw new Error('progress_too_short');

    const containerCount = payload.readUInt16BE(49);
    const containers = [];
    let o = PLAYER_PROGRESS_HEADER_SIZE;
    for (let c = 0; c < containerCount; c++) {
        if (o + 6 > payload.length) throw new Error('progress_container_truncated');
        const columns   = payload.readUInt16BE(o); o += 2;
        const rows      = payload.readUInt16BE(o); o += 2;
        const itemCount = payload.readUInt16BE(o); o += 2;

        const items = [];
        for (let i = 0; i < itemCount; i++) {
            if (o + 3 > payload.length) throw new Error('progress_slot_truncated');
            const slotIndex = payload.readUInt8(o); o += 1;
            const idLen     = payload.readUInt16BE(o); o += 2;
            if (o + idLen + 2 > payload.length) throw new Error('progress_slot_truncated');
            const itemId    = payload.toString('utf8', o, o + idLen); o += idLen;
            const quantity  = payload.readUInt16BE(o); o += 2;
            items.push({ slotIndex, itemId, quantity });
        }
        containers.push({ columns, rows, items });
    }

    // Extended stats trailer (gap 4/7) — optional. A payload persisted before
    // this trailer existed just ends here, so leave defaults on short input
    // rather than throwing.
    let forename = '', surname = '';
    let zombieKills = 0, daysSurvived = 0, bossZombieKills = 0, animalKills = 0, humanKills = 0;
    let distanceTravelled = 0, infestationsDestroyed = 0;
    if (o + 2 <= payload.length) {
        const forenameLen = payload.readUInt16BE(o); o += 2;
        if (o + forenameLen + 2 <= payload.length) {
            forename = payload.toString('utf8', o, o + forenameLen); o += forenameLen;
            const surnameLen = payload.readUInt16BE(o); o += 2;
            if (o + surnameLen + 28 <= payload.length) {
                surname = payload.toString('utf8', o, o + surnameLen); o += surnameLen;
                zombieKills           = payload.readUInt32BE(o); o += 4;
                daysSurvived          = payload.readUInt32BE(o); o += 4;
                bossZombieKills       = payload.readUInt32BE(o); o += 4;
                animalKills           = payload.readUInt32BE(o); o += 4;
                humanKills            = payload.readUInt32BE(o); o += 4;
                distanceTravelled     = payload.readFloatBE(o);  o += 4;
                infestationsDestroyed = payload.readUInt32BE(o); o += 4;
            }
        }
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
        containers,
        forename, surname, zombieKills, daysSurvived, bossZombieKills, animalKills, humanKills,
        distanceTravelled, infestationsDestroyed,
    };
}

// ── ItemDropRequest payload ───────────────────────────────────────────────────
// itemId-based rather than slotIndex-based: the client's real drop hook
// (BP_JigHelperComp_C::RequestDropAsPickup) hands us the dropped item's
// identity directly, not a container slot index — and since p.inventory's
// slotIndex is a flattened-across-containers bookkeeping array (gap 11, can
// collide across containers), matching by itemId+quantity server-side is
// actually more correct here than trusting a slot number the client can't
// cleanly compute from this hook anyway.
// Byte layout:
//   0      uint8   format version (1)
//   1–2    uint16  quantity
//   3–6    float32 posX  (world position to drop at)
//   7–10   float32 posY
//   11–14  float32 posZ
//   15–16  uint16  itemIdLen
//   17…    bytes   itemId utf8

function decodeItemDropRequest(payload) {
    if (payload.length < 17) throw new Error('drop_request_too_short');
    const itemIdLen = payload.readUInt16BE(15);
    if (payload.length < 17 + itemIdLen) throw new Error('drop_request_truncated');
    return {
        quantity:  payload.readUInt16BE(1),
        posX:      payload.readFloatBE(3),
        posY:      payload.readFloatBE(7),
        posZ:      payload.readFloatBE(11),
        itemId:    payload.subarray(17, 17 + itemIdLen).toString('utf8'),
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
// Same JSON-via-encodeString codec as ItemDropResult (mod.cpp decodes both
// with decode_world_action) — the previous fixed-binary layout also wrote
// itemId (a string, see p.inventory) through writeUInt32BE, which would have
// thrown at runtime on any successful pickup.

function encodeItemPickupResult({ success, slot, itemId, quantity, reason }) {
    return encodeString(JSON.stringify(
        success ? { success: true, slot, itemId, quantity }
                : { success: false, reason: reason ?? 0 }
    ), 512);
}

// ── ItemDropResult payload ────────────────────────────────────────────────────
// The client decodes this via decode_world_action (mod.cpp: "JSON via
// encodeWorldAction") — [uint16BE length][utf8 JSON], no tag byte — not a
// fixed binary layout, so this must go through encodeString, matching the
// same JSON codec ItemPickupResult was already written to expect.
// reason on fail: 0=slot_empty, 1=bad_slot

function encodeItemDropResult({ success, reason }) {
    return encodeString(JSON.stringify({ success: !!success, reason: reason ?? 0 }), 512);
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
    EntityKind,
    InteractionType,
    decodeFrame, encodeFrame, encodeString, decodeString,
    encodeEntityDescriptor, encodeEntityState,
    encodePlayerProgress, decodePlayerProgress,
    decodeItemDropRequest, decodeItemPickupRequest,
    encodeItemPickupResult, encodeItemDropResult,
    decodeInteractionRequest, encodeInteractionResult,
};
