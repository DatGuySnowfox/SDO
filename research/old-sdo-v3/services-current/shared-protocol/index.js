import { createHash, createHmac, randomUUID, timingSafeEqual } from 'node:crypto';
export const PROTOCOL_VERSION = 3;
export const MAGIC = 0x53444f33;
export const HEADER_BYTES = 88;
export const MAX_PAYLOAD_BYTES = 64 * 1024;
export const MAX_FRAME_BYTES = HEADER_BYTES + MAX_PAYLOAD_BYTES;
export const MessageType = {
    HostAuthenticate: 1,
    ClientAuthenticate: 2,
    AuthenticationAccepted: 3,
    AuthenticationRejected: 4,
    HostHeartbeat: 5,
    ClientHeartbeat: 6,
    JoinRequest: 10,
    JoinAccepted: 11,
    JoinRejected: 12,
    PlayerConnected: 13,
    PlayerDisconnected: 14,
    Movement: 20,
    Equipment: 21,
    Death: 22,
    Respawn: 23,
    ProfileRevision: 24,
    SaveAcknowledgement: 25,
    InteractionRequest: 26,
    InteractionResult: 27,
    DeathRequest: 28,
    RespawnRequest: 29,
    WorldState: 30,
    EntitySpawn: 31,
    EntityState: 32,
    EntityDespawn: 33,
    ItemDropRequest: 34,
    ItemPickupRequest: 35,
    ItemPickupResult: 36,
    ZombieAttackRequest: 37,
    ZombieDamageResult: 38,
    ItemDropResult: 39,
    PlayerDamage: 40,
    PlayerProgressRestore: 42,
    Error: 255
};
export class ProtocolError extends Error {
    code;
    constructor(code, message) {
        super(message);
        this.name = 'ProtocolError';
        this.code = code;
    }
}
function uuidToBytes(value) {
    if (!value)
        return Buffer.alloc(16);
    const compact = value.replaceAll('-', '');
    if (!/^[0-9a-fA-F]{32}$/.test(compact)) {
        throw new ProtocolError('invalid_uuid', `Invalid UUID: ${value}`);
    }
    return Buffer.from(compact, 'hex');
}
function bytesToUuid(bytes) {
    if (bytes.every((value) => value === 0))
        return '00000000-0000-0000-0000-000000000000';
    const hex = bytes.toString('hex');
    return `${hex.slice(0, 8)}-${hex.slice(8, 12)}-${hex.slice(12, 16)}-${hex.slice(16, 20)}-${hex.slice(20)}`;
}
export function encodeFrame(frame) {
    const payload = frame.payload ?? Buffer.alloc(0);
    if (payload.length > MAX_PAYLOAD_BYTES) {
        throw new ProtocolError('payload_too_large', `Payload is ${payload.length} bytes`);
    }
    if (!Object.values(MessageType).includes(frame.type)) {
        throw new ProtocolError('unknown_message_type', `Unknown message type ${frame.type}`);
    }
    const output = Buffer.allocUnsafe(HEADER_BYTES + payload.length);
    output.writeUInt32BE(MAGIC, 0);
    output.writeUInt16BE(PROTOCOL_VERSION, 4);
    output.writeUInt16BE(frame.type, 6);
    output.writeUInt16BE(frame.flags ?? 0, 8);
    output.writeUInt16BE(HEADER_BYTES, 10);
    output.writeUInt32BE(payload.length, 12);
    output.writeBigUInt64BE(frame.connectionId ?? 0n, 16);
    uuidToBytes(frame.sessionId).copy(output, 24);
    uuidToBytes(frame.worldId).copy(output, 40);
    output.writeBigUInt64BE(frame.playerId ?? 0n, 56);
    output.writeBigUInt64BE(frame.entityId ?? 0n, 64);
    output.writeUInt32BE((frame.sequence ?? 0) >>> 0, 72);
    output.writeUInt32BE((frame.tick ?? 0) >>> 0, 76);
    output.writeBigUInt64BE(frame.timestampMicros ?? BigInt(Date.now()) * 1000n, 80);
    payload.copy(output, HEADER_BYTES);
    return output;
}
export function decodeFrame(input) {
    if (input.length < HEADER_BYTES)
        throw new ProtocolError('truncated_header', 'Frame header is incomplete');
    if (input.readUInt32BE(0) !== MAGIC)
        throw new ProtocolError('bad_magic', 'Frame magic does not match');
    if (input.readUInt16BE(4) !== PROTOCOL_VERSION) {
        throw new ProtocolError('unsupported_version', `Expected protocol ${PROTOCOL_VERSION}`);
    }
    const type = input.readUInt16BE(6);
    if (!Object.values(MessageType).includes(type))
        throw new ProtocolError('unknown_message_type', `Unknown type ${type}`);
    if (input.readUInt16BE(10) !== HEADER_BYTES)
        throw new ProtocolError('bad_header_length', 'Invalid header length');
    const payloadLength = input.readUInt32BE(12);
    if (payloadLength > MAX_PAYLOAD_BYTES)
        throw new ProtocolError('payload_too_large', 'Payload exceeds limit');
    if (input.length !== HEADER_BYTES + payloadLength)
        throw new ProtocolError('bad_frame_length', 'Frame length mismatch');
    return {
        type,
        flags: input.readUInt16BE(8),
        connectionId: input.readBigUInt64BE(16),
        sessionId: bytesToUuid(input.subarray(24, 40)),
        worldId: bytesToUuid(input.subarray(40, 56)),
        playerId: input.readBigUInt64BE(56),
        entityId: input.readBigUInt64BE(64),
        sequence: input.readUInt32BE(72),
        tick: input.readUInt32BE(76),
        timestampMicros: input.readBigUInt64BE(80),
        payload: Buffer.from(input.subarray(HEADER_BYTES))
    };
}
export class FrameDecoder {
    pending = Buffer.alloc(0);
    push(chunk) {
        if (this.pending.length + chunk.length > MAX_FRAME_BYTES * 4) {
            throw new ProtocolError('receive_buffer_exceeded', 'Receive buffer exceeded');
        }
        this.pending = Buffer.concat([this.pending, chunk]);
        const frames = [];
        while (this.pending.length >= HEADER_BYTES) {
            if (this.pending.readUInt32BE(0) !== MAGIC)
                throw new ProtocolError('bad_magic', 'Frame magic does not match');
            const payloadLength = this.pending.readUInt32BE(12);
            if (payloadLength > MAX_PAYLOAD_BYTES)
                throw new ProtocolError('payload_too_large', 'Payload exceeds limit');
            const frameLength = HEADER_BYTES + payloadLength;
            if (this.pending.length < frameLength)
                break;
            frames.push(decodeFrame(this.pending.subarray(0, frameLength)));
            this.pending = this.pending.subarray(frameLength);
        }
        return frames;
    }
}
export function encodeString(value, maximumBytes = 4096) {
    const bytes = Buffer.from(value, 'utf8');
    if (bytes.length > maximumBytes || bytes.length > 65535) {
        throw new ProtocolError('string_too_large', 'String exceeds limit');
    }
    const result = Buffer.allocUnsafe(2 + bytes.length);
    result.writeUInt16BE(bytes.length, 0);
    bytes.copy(result, 2);
    return result;
}
export function decodeString(payload, maximumBytes = 4096) {
    if (payload.length < 2)
        throw new ProtocolError('truncated_string', 'String length missing');
    const length = payload.readUInt16BE(0);
    if (length > maximumBytes || payload.length !== length + 2) {
        throw new ProtocolError('invalid_string_length', 'String length invalid');
    }
    return payload.subarray(2).toString('utf8');
}
export function encodeJoinRequest(playerKey, displayName) {
    const key = encodeString(playerKey, 128);
    const name = encodeString(displayName, 64);
    return Buffer.concat([key, name]);
}
export function decodeJoinRequest(payload) {
    if (payload.length < 4)
        throw new ProtocolError('invalid_join_request', 'Join request is truncated');
    const keyLength = payload.readUInt16BE(0);
    const keyEnd = 2 + keyLength;
    if (keyLength > 128 || keyEnd + 2 > payload.length)
        throw new ProtocolError('invalid_join_request', 'Player key is invalid');
    const nameLength = payload.readUInt16BE(keyEnd);
    if (nameLength > 64 || keyEnd + 2 + nameLength !== payload.length) {
        throw new ProtocolError('invalid_join_request', 'Display name is invalid');
    }
    return {
        playerKey: payload.subarray(2, keyEnd).toString('utf8'),
        displayName: payload.subarray(keyEnd + 2).toString('utf8')
    };
}
export function stableNumericId(value) {
    const bytes = createHash('sha256').update(value).digest().subarray(0, 8);
    const id = bytes.readBigUInt64BE(0);
    return id === 0n ? 1n : id;
}
export function encodeMovement(value) {
    const result = Buffer.allocUnsafe(39);
    const numbers = [
        value.x, value.y, value.z, value.yaw, value.aimYaw,
        value.velocityX, value.velocityY, value.velocityZ,
        value.movementDirection
    ];
    if (numbers.some((item) => !Number.isFinite(item)))
        throw new ProtocolError('invalid_movement', 'Movement contains non-finite value');
    numbers.forEach((item, index) => result.writeFloatBE(item, index * 4));
    result.writeUInt8(value.movementState & 0xff, 36);
    result.writeUInt8(value.aimState & 0xff, 37);
    result.writeUInt8(value.animationState & 0xff, 38);
    return result;
}
export function decodeMovement(payload) {
    if (payload.length !== 39)
        throw new ProtocolError('invalid_movement_length', 'Movement payload must be 39 bytes');
    const values = Array.from({ length: 9 }, (_, index) => payload.readFloatBE(index * 4));
    if (values.some((item) => !Number.isFinite(item)))
        throw new ProtocolError('invalid_movement', 'Movement contains non-finite value');
    return {
        x: values[0], y: values[1], z: values[2], yaw: values[3],
        aimYaw: values[4],
        velocityX: values[5], velocityY: values[6], velocityZ: values[7],
        movementDirection: values[8],
        movementState: payload.readUInt8(36),
        aimState: payload.readUInt8(37),
        animationState: payload.readUInt8(38)
    };
}
const WORLD_STATE_BYTES = 41;
export function encodeWorldState(value) {
    const numbers = [
        value.timeOfDay, value.rain, value.snow, value.fog,
        value.cloudCoverage, value.wind, value.thunder
    ];
    if (!Number.isSafeInteger(value.revision) ||
        value.revision < 0 ||
        value.revision > 0xffff_ffff ||
        value.authorityTimeMs < 0n ||
        numbers.some((item) => !Number.isFinite(item))) {
        throw new ProtocolError('invalid_world_state', 'World state contains an invalid value');
    }
    const output = Buffer.allocUnsafe(WORLD_STATE_BYTES);
    output.writeUInt8(1, 0);
    output.writeUInt32BE(value.revision >>> 0, 1);
    output.writeBigUInt64BE(value.authorityTimeMs, 5);
    numbers.forEach((item, index) => output.writeFloatBE(item, 13 + index * 4));
    return output;
}
export function decodeWorldState(payload) {
    if (payload.length !== WORLD_STATE_BYTES || payload.readUInt8(0) !== 1) {
        throw new ProtocolError('invalid_world_state_length', 'World state payload is invalid');
    }
    const numbers = Array.from({ length: 7 }, (_, index) => payload.readFloatBE(13 + index * 4));
    if (numbers.some((item) => !Number.isFinite(item))) {
        throw new ProtocolError('invalid_world_state', 'World state contains a non-finite value');
    }
    return {
        revision: payload.readUInt32BE(1),
        authorityTimeMs: payload.readBigUInt64BE(5),
        timeOfDay: numbers[0],
        rain: numbers[1],
        snow: numbers[2],
        fog: numbers[3],
        cloudCoverage: numbers[4],
        wind: numbers[5],
        thunder: numbers[6]
    };
}
export const WorldEntityKind = {
    Zombie: 1,
    GroundItem: 2,
    Vehicle: 3,
    PlacedStructure: 4
};
/** movementState byte: occupant is driving or seated in a vehicle pawn */
export const MOVEMENT_STATE_IN_VEHICLE = 3;
export function encodeWorldEntityDescriptor(value) {
    if (!Object.values(WorldEntityKind).includes(value.kind) ||
        !Number.isSafeInteger(value.revision) ||
        value.revision < 0 ||
        value.revision > 0xffff_ffff ||
        !Number.isSafeInteger(value.quantity) ||
        value.quantity < 0 ||
        value.quantity > 0xffff ||
        value.ownerPlayerId < 0n) {
        throw new ProtocolError('invalid_world_entity_descriptor', 'World entity descriptor is invalid');
    }
    const classPath = encodeString(value.classPath, 1024);
    const itemId = encodeString(value.itemId, 512);
    const fixed = Buffer.allocUnsafe(16);
    fixed.writeUInt8(1, 0);
    fixed.writeUInt8(value.kind, 1);
    fixed.writeUInt32BE(value.revision >>> 0, 2);
    fixed.writeUInt16BE(value.quantity, 6);
    fixed.writeBigUInt64BE(value.ownerPlayerId, 8);
    return Buffer.concat([fixed, classPath, itemId]);
}
export function decodeWorldEntityDescriptor(payload) {
    if (payload.length < 20 || payload.readUInt8(0) !== 1) {
        throw new ProtocolError('invalid_world_entity_descriptor_length', 'World entity descriptor is invalid');
    }
    const kind = payload.readUInt8(1);
    if (!Object.values(WorldEntityKind).includes(kind)) {
        throw new ProtocolError('invalid_world_entity_descriptor', 'World entity kind is invalid');
    }
    const classLength = payload.readUInt16BE(16);
    const classEnd = 18 + classLength;
    if (classLength > 1024 || classEnd + 2 > payload.length) {
        throw new ProtocolError('invalid_world_entity_descriptor', 'World entity class path is invalid');
    }
    const itemLength = payload.readUInt16BE(classEnd);
    if (itemLength > 512 || classEnd + 2 + itemLength !== payload.length) {
        throw new ProtocolError('invalid_world_entity_descriptor', 'World entity item id is invalid');
    }
    return {
        kind,
        revision: payload.readUInt32BE(2),
        quantity: payload.readUInt16BE(6),
        ownerPlayerId: payload.readBigUInt64BE(8),
        classPath: payload.subarray(18, classEnd).toString('utf8'),
        itemId: payload.subarray(classEnd + 2).toString('utf8')
    };
}
export function encodeWorldEntityState(value) {
    const numbers = [value.x, value.y, value.z, value.yaw, value.health];
    if (!Object.values(WorldEntityKind).includes(value.kind) ||
        !Number.isSafeInteger(value.revision) ||
        value.revision < 0 ||
        value.revision > 0xffff_ffff ||
        !Number.isSafeInteger(value.state) ||
        value.state < 0 ||
        value.state > 255 ||
        numbers.some((item) => !Number.isFinite(item))) {
        throw new ProtocolError('invalid_world_entity_state', 'World entity state is invalid');
    }
    const output = Buffer.allocUnsafe(27);
    output.writeUInt8(1, 0);
    output.writeUInt8(value.kind, 1);
    output.writeUInt32BE(value.revision >>> 0, 2);
    numbers.forEach((item, index) => output.writeFloatBE(item, 6 + index * 4));
    output.writeUInt8(value.state, 26);
    return output;
}
export function decodeWorldEntityState(payload) {
    if (payload.length !== 27 || payload.readUInt8(0) !== 1) {
        throw new ProtocolError('invalid_world_entity_state_length', 'World entity state payload is invalid');
    }
    const kind = payload.readUInt8(1);
    const numbers = Array.from({ length: 5 }, (_, index) => payload.readFloatBE(6 + index * 4));
    if (!Object.values(WorldEntityKind).includes(kind) ||
        numbers.some((item) => !Number.isFinite(item))) {
        throw new ProtocolError('invalid_world_entity_state', 'World entity state is invalid');
    }
    return {
        kind,
        revision: payload.readUInt32BE(2),
        x: numbers[0],
        y: numbers[1],
        z: numbers[2],
        yaw: numbers[3],
        health: numbers[4],
        state: payload.readUInt8(26)
    };
}
const PLAYER_DAMAGE_BYTES = 13;
export function encodePlayerDamage(value) {
    if (!Number.isFinite(value.damage) ||
        value.damage <= 0 ||
        value.damage > 1_000 ||
        !Number.isFinite(value.health) ||
        value.health < 0 ||
        value.health > 1_000 ||
        !Number.isSafeInteger(value.attackSequence) ||
        value.attackSequence < 0 ||
        value.attackSequence > 0xffff_ffff) {
        throw new ProtocolError('invalid_player_damage', 'Player damage payload is invalid');
    }
    const output = Buffer.allocUnsafe(PLAYER_DAMAGE_BYTES);
    output.writeUInt8(1, 0);
    output.writeFloatBE(value.damage, 1);
    output.writeFloatBE(value.health, 5);
    output.writeUInt32BE(value.attackSequence >>> 0, 9);
    return output;
}
export function decodePlayerDamage(payload) {
    if (payload.length !== PLAYER_DAMAGE_BYTES || payload.readUInt8(0) !== 1) {
        throw new ProtocolError('invalid_player_damage_length', 'Player damage payload is invalid');
    }
    const value = {
        damage: payload.readFloatBE(1),
        health: payload.readFloatBE(5),
        attackSequence: payload.readUInt32BE(9)
    };
    if (!Number.isFinite(value.damage) ||
        value.damage <= 0 ||
        value.damage > 1_000 ||
        !Number.isFinite(value.health) ||
        value.health < 0 ||
        value.health > 1_000) {
        throw new ProtocolError('invalid_player_damage', 'Player damage payload is invalid');
    }
    return value;
}
export function encodeWorldAction(value) {
    return encodeString(JSON.stringify(value), 4096);
}
export function decodeWorldAction(payload) {
    let value;
    try {
        value = JSON.parse(decodeString(payload, 4096));
    }
    catch {
        throw new ProtocolError('invalid_world_action', 'World action JSON is invalid');
    }
    if (!value || typeof value !== 'object' || Array.isArray(value)) {
        throw new ProtocolError('invalid_world_action', 'World action must be an object');
    }
    return value;
}
export function issueJoinTicket(input, secret) {
    if (secret.length < 16)
        throw new ProtocolError('weak_secret', 'Ticket secret must be at least 16 characters');
    const ticket = { ...input, ticketId: randomUUID(), protocolVersion: PROTOCOL_VERSION };
    const body = Buffer.from(JSON.stringify(ticket)).toString('base64url');
    const signature = createHmac('sha256', secret).update(body).digest('base64url');
    return `${body}.${signature}`;
}
export function validateJoinTicket(token, secret, nowMs = Date.now()) {
    const parts = token.split('.');
    if (parts.length !== 2 || token.length > 4096)
        throw new ProtocolError('invalid_ticket', 'Malformed join ticket');
    const body = parts[0];
    const signature = parts[1];
    const expected = createHmac('sha256', secret).update(body).digest();
    let supplied;
    try {
        supplied = Buffer.from(signature, 'base64url');
    }
    catch {
        throw new ProtocolError('invalid_ticket', 'Malformed signature');
    }
    if (supplied.length !== expected.length || !timingSafeEqual(supplied, expected)) {
        throw new ProtocolError('invalid_ticket_signature', 'Join ticket signature is invalid');
    }
    let value;
    try {
        value = JSON.parse(Buffer.from(body, 'base64url').toString('utf8'));
    }
    catch {
        throw new ProtocolError('invalid_ticket', 'Join ticket body is invalid');
    }
    const ticket = value;
    if (typeof ticket.ticketId !== 'string' || typeof ticket.playerId !== 'string' ||
        typeof ticket.displayName !== 'string' || typeof ticket.worldId !== 'string' ||
        typeof ticket.expiresAtMs !== 'number' || ticket.protocolVersion !== PROTOCOL_VERSION)
        throw new ProtocolError('invalid_ticket_claims', 'Join ticket claims are invalid');
    if (ticket.expiresAtMs <= nowMs)
        throw new ProtocolError('expired_ticket', 'Join ticket has expired');
    if (ticket.expiresAtMs > nowMs + 5 * 60_000)
        throw new ProtocolError('ticket_ttl_too_long', 'Join ticket lifetime is too long');
    uuidToBytes(ticket.worldId);
    return ticket;
}
export function isNewerSequence(sequence, previous) {
    const difference = (sequence - previous) >>> 0;
    return difference !== 0 && difference < 0x80000000;
}
export const ZERO_UUID = '00000000-0000-0000-0000-000000000000';
//# sourceMappingURL=index.js.map