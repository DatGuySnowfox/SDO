import { mkdir, readFile, rename, rm, writeFile } from 'node:fs/promises';
import { dirname } from 'node:path';
import { WorldEntityKind } from "../shared-protocol/index.js";
import { validSteamIdentity } from "../profile-service/bundle.js";
const MAX_ENTITIES = 10_000;
const MAX_LEDGER_ENTRIES = 20_000;
const MAX_SAVED_PLAYERS = 10_000;
const MAX_COORDINATE = 10_000_000;
function finite(value) {
    return typeof value === 'number' && Number.isFinite(value);
}
function uint32(value) {
    return Number.isSafeInteger(value) && Number(value) >= 0 && Number(value) <= 0xffff_ffff;
}
function uint64Number(value) {
    return Number.isSafeInteger(value) && Number(value) >= 0;
}
function parseUnsignedBigInt(value, allowZero = false) {
    if (typeof value !== 'string' || !/^[0-9]{1,20}$/.test(value))
        return undefined;
    try {
        const parsed = BigInt(value);
        const minimum = allowZero ? 0n : 1n;
        return parsed >= minimum && parsed <= 0xffffffffffffffffn ? parsed : undefined;
    }
    catch {
        return undefined;
    }
}
function validClassPath(value) {
    return typeof value === 'string' &&
        value.length >= 1 && value.length <= 1024 &&
        value.startsWith('/Game/') && !/[\r\n\t\0]/.test(value);
}
function validText(value, maximum) {
    return typeof value === 'string' && value.length <= maximum && !/[\r\n\0]/.test(value);
}
function parseLatestWorldState(value) {
    if (value === undefined)
        return undefined;
    if (!value || typeof value !== 'object' || Array.isArray(value))
        return undefined;
    const state = value;
    const authorityTimeMs = parseUnsignedBigInt(state.authorityTimeMs, true);
    const numbers = [
        state.timeOfDay,
        state.rain,
        state.snow,
        state.fog,
        state.cloudCoverage,
        state.wind,
        state.thunder
    ];
    if (authorityTimeMs === undefined ||
        numbers.some((item) => !finite(item)) ||
        Number(state.timeOfDay) < 0 ||
        Number(state.timeOfDay) > 2400) {
        return undefined;
    }
    return {
        authorityTimeMs,
        timeOfDay: Number(state.timeOfDay),
        rain: Number(state.rain),
        snow: Number(state.snow),
        fog: Number(state.fog),
        cloudCoverage: Number(state.cloudCoverage),
        wind: Number(state.wind),
        thunder: Number(state.thunder)
    };
}
function parseEntity(value) {
    if (!value || typeof value !== 'object' || Array.isArray(value))
        return undefined;
    const entity = value;
    const entityId = parseUnsignedBigInt(entity.entityId);
    const ownerPlayerId = entity.ownerPlayerId === undefined
        ? undefined
        : parseUnsignedBigInt(entity.ownerPlayerId);
    const kind = entity.kind;
    if (!entityId ||
        !Object.values(WorldEntityKind).includes(kind) ||
        !uint32(entity.revision) ||
        !finite(entity.x) || Math.abs(entity.x) >= MAX_COORDINATE ||
        !finite(entity.y) || Math.abs(entity.y) >= MAX_COORDINATE ||
        !finite(entity.z) || Math.abs(entity.z) >= MAX_COORDINATE ||
        !finite(entity.yaw) ||
        !finite(entity.health) ||
        !Number.isSafeInteger(entity.state) || Number(entity.state) < 0 || Number(entity.state) > 255 ||
        !validClassPath(entity.classPath) ||
        (entity.itemId !== undefined && !validText(entity.itemId, 512)) ||
        (entity.requestId !== undefined && !validText(entity.requestId, 80)) ||
        (entity.quantity !== undefined &&
            (!Number.isSafeInteger(entity.quantity) || Number(entity.quantity) < 0 || Number(entity.quantity) > 0xffff)) ||
        (entity.ownerPlayerId !== undefined && !ownerPlayerId))
        return undefined;
    return {
        entityId,
        kind,
        revision: Number(entity.revision),
        x: entity.x,
        y: entity.y,
        z: entity.z,
        yaw: entity.yaw,
        health: entity.health,
        state: Number(entity.state),
        classPath: entity.classPath,
        itemId: entity.itemId,
        quantity: entity.quantity,
        requestId: entity.requestId,
        ownerPlayerId
    };
}
function parseDrop(value) {
    if (!value || typeof value !== 'object' || Array.isArray(value))
        return undefined;
    const item = value;
    const entityId = parseUnsignedBigInt(item.entityId);
    if (!entityId || !validText(item.key, 220) || item.key.length === 0 ||
        (item.status !== 'active' && item.status !== 'consumed') ||
        !Number.isSafeInteger(item.updatedAtMs) || Number(item.updatedAtMs) < 0)
        return undefined;
    return {
        key: item.key,
        entityId,
        status: item.status,
        updatedAtMs: Number(item.updatedAtMs)
    };
}
function parsePickup(value) {
    if (!value || typeof value !== 'object' || Array.isArray(value))
        return undefined;
    const item = value;
    const entityId = parseUnsignedBigInt(item.entityId, true);
    if (entityId === undefined || !validText(item.key, 220) || item.key.length === 0 ||
        typeof item.accepted !== 'boolean' || !validText(item.reason, 128) ||
        (item.itemId !== undefined && !validText(item.itemId, 512)) ||
        (item.quantity !== undefined &&
            (!Number.isSafeInteger(item.quantity) || Number(item.quantity) < 0 || Number(item.quantity) > 0xffff)) ||
        !Number.isSafeInteger(item.updatedAtMs) || Number(item.updatedAtMs) < 0)
        return undefined;
    return {
        key: item.key,
        entityId,
        accepted: item.accepted,
        reason: item.reason,
        itemId: item.itemId,
        quantity: item.quantity,
        updatedAtMs: Number(item.updatedAtMs)
    };
}
function parsePlayer(value) {
    if (!value || typeof value !== 'object' || Array.isArray(value))
        return undefined;
    const item = value;
    const playerId = parseUnsignedBigInt(item.playerId);
    const playerKey = typeof item.playerKey === 'string' ? item.playerKey : '';
    if (!playerId ||
        !validSteamIdentity(playerKey) ||
        !finite(item.x) || Math.abs(item.x) >= MAX_COORDINATE ||
        !finite(item.y) || Math.abs(item.y) >= MAX_COORDINATE ||
        !finite(item.z) || Math.abs(item.z) >= MAX_COORDINATE ||
        !finite(item.yaw) ||
        !finite(item.health) || item.health < 0 || item.health > 100_000 ||
        typeof item.dead !== 'boolean' ||
        !uint32(item.profileRevision) ||
        !Number.isSafeInteger(item.updatedAtMs) || Number(item.updatedAtMs) < 0)
        return undefined;
    return {
        playerKey,
        playerId,
        x: item.x,
        y: item.y,
        z: item.z,
        yaw: item.yaw,
        health: item.health,
        dead: item.dead,
        profileRevision: Number(item.profileRevision),
        updatedAtMs: Number(item.updatedAtMs)
    };
}
export class WorldSnapshotStore {
    path;
    worldId;
    constructor(path, worldId) {
        this.path = path;
        this.worldId = worldId;
    }
    async load() {
        let raw;
        try {
            raw = await readFile(this.path, 'utf8');
        }
        catch (error) {
            if (error.code === 'ENOENT')
                return undefined;
            throw error;
        }
        let value;
        try {
            value = JSON.parse(raw);
        }
        catch {
            throw new Error('Authoritative world snapshot contains invalid JSON');
        }
        if (!value || typeof value !== 'object' || Array.isArray(value)) {
            throw new Error('Authoritative world snapshot root is invalid');
        }
        const snapshot = value;
        if (![1, 2, 3].includes(Number(snapshot.version)) || snapshot.worldId !== this.worldId ||
            !uint32(snapshot.worldRevision) || !uint32(snapshot.simulationTick) ||
            (snapshot.weatherCycleElapsedMs !== undefined && !uint64Number(snapshot.weatherCycleElapsedMs)) ||
            (snapshot.nextZombieSpawnAtMs !== undefined && !uint64Number(snapshot.nextZombieSpawnAtMs)) ||
            (snapshot.zombieSpawnSerial !== undefined && !uint32(snapshot.zombieSpawnSerial)) ||
            !Array.isArray(snapshot.entities) || snapshot.entities.length > MAX_ENTITIES ||
            !Array.isArray(snapshot.dropRequests) || snapshot.dropRequests.length > MAX_LEDGER_ENTRIES ||
            !Array.isArray(snapshot.pickupRequests) || snapshot.pickupRequests.length > MAX_LEDGER_ENTRIES ||
            (snapshot.players !== undefined &&
                (!Array.isArray(snapshot.players) || snapshot.players.length > MAX_SAVED_PLAYERS))) {
            throw new Error('Authoritative world snapshot metadata is invalid or belongs to another world');
        }
        const latestWorldState = parseLatestWorldState(snapshot.latestWorldState);
        if (snapshot.latestWorldState !== undefined && !latestWorldState) {
            throw new Error('Authoritative world snapshot contains an invalid world state');
        }
        const entities = snapshot.entities.map(parseEntity);
        const dropRequests = snapshot.dropRequests.map(parseDrop);
        const pickupRequests = snapshot.pickupRequests.map(parsePickup);
        const players = (snapshot.players ?? [])
            .map(parsePlayer)
            .filter((item) => Boolean(item));
        if (entities.some((item) => !item) ||
            dropRequests.some((item) => !item) ||
            pickupRequests.some((item) => !item)) {
            throw new Error('Authoritative world snapshot contains an invalid entry');
        }
        return {
            worldRevision: snapshot.worldRevision,
            simulationTick: snapshot.simulationTick,
            latestWorldState,
            weatherCycleElapsedMs: snapshot.weatherCycleElapsedMs,
            nextZombieSpawnAtMs: snapshot.nextZombieSpawnAtMs,
            zombieSpawnSerial: snapshot.zombieSpawnSerial,
            entities: entities,
            dropRequests: dropRequests,
            pickupRequests: pickupRequests,
            players: players
        };
    }
    async save(snapshot) {
        const serialized = {
            version: 3,
            worldId: this.worldId,
            savedAtMs: Date.now(),
            worldRevision: snapshot.worldRevision >>> 0,
            simulationTick: snapshot.simulationTick >>> 0,
            latestWorldState: snapshot.latestWorldState
                ? {
                    authorityTimeMs: snapshot.latestWorldState.authorityTimeMs.toString(),
                    timeOfDay: snapshot.latestWorldState.timeOfDay,
                    rain: snapshot.latestWorldState.rain,
                    snow: snapshot.latestWorldState.snow,
                    fog: snapshot.latestWorldState.fog,
                    cloudCoverage: snapshot.latestWorldState.cloudCoverage,
                    wind: snapshot.latestWorldState.wind,
                    thunder: snapshot.latestWorldState.thunder
                }
                : undefined,
            weatherCycleElapsedMs: snapshot.weatherCycleElapsedMs,
            nextZombieSpawnAtMs: snapshot.nextZombieSpawnAtMs,
            zombieSpawnSerial: snapshot.zombieSpawnSerial,
            entities: snapshot.entities.map((entity) => ({
                ...entity,
                entityId: entity.entityId.toString(),
                ownerPlayerId: entity.ownerPlayerId?.toString()
            })),
            dropRequests: snapshot.dropRequests.map((item) => ({
                ...item,
                entityId: item.entityId.toString()
            })),
            pickupRequests: snapshot.pickupRequests.map((item) => ({
                ...item,
                entityId: item.entityId.toString()
            })),
            players: snapshot.players.map((player) => ({
                ...player,
                playerId: player.playerId.toString()
            }))
        };
        await mkdir(dirname(this.path), { recursive: true });
        const temporary = `${this.path}.${process.pid}.tmp`;
        const backup = `${this.path}.${process.pid}.bak`;
        await writeFile(temporary, `${JSON.stringify(serialized, null, 2)}\n`, {
            encoding: 'utf8',
            mode: 0o600
        });
        let previousMoved = false;
        try {
            try {
                await rename(this.path, backup);
                previousMoved = true;
            }
            catch (error) {
                if (error.code !== 'ENOENT')
                    throw error;
            }
            await rename(temporary, this.path);
            if (previousMoved)
                await rm(backup, { force: true });
        }
        catch (error) {
            await rm(temporary, { force: true }).catch(() => undefined);
            if (previousMoved) {
                await rename(backup, this.path).catch(() => undefined);
            }
            throw error;
        }
    }
}
//# sourceMappingURL=world-store.js.map