import { mkdir, readFile, rename, writeFile } from 'node:fs/promises';
import { dirname } from 'node:path';
const SERVER = /^home-world-[0-9]{1,3}$/;
const STEAM = /^steam_7656119[0-9]{10}$/;
export const HOME_HEARTBEAT_TTL_MS = 90_000;
export async function loadHomeHeartbeats(path) {
    try {
        const text = await readFile(path, 'utf8');
        return validateHomeHeartbeats(JSON.parse(text.replace(/^\uFEFF/, '')));
    }
    catch (error) {
        if (error.code === 'ENOENT') {
            return { version: 1, heartbeats: {} };
        }
        throw error;
    }
}
export async function saveHomeHeartbeats(path, value) {
    const validated = validateHomeHeartbeats(value);
    await mkdir(dirname(path), { recursive: true });
    const temporary = `${path}.${process.pid}.tmp`;
    await writeFile(temporary, `${JSON.stringify(validated, null, 2)}\n`, {
        encoding: 'utf8',
        mode: 0o600
    });
    await rename(temporary, path);
}
export function validateHomeHeartbeats(input) {
    if (!input || typeof input !== 'object' || Array.isArray(input)) {
        throw new Error('home_heartbeats_invalid');
    }
    const value = input;
    if (value.version !== 1 || !value.heartbeats || typeof value.heartbeats !== 'object') {
        throw new Error('home_heartbeats_invalid');
    }
    const heartbeats = {};
    for (const [serverId, heartbeat] of Object.entries(value.heartbeats)) {
        if (!SERVER.test(serverId) ||
            !heartbeat ||
            heartbeat.serverId !== serverId ||
            !STEAM.test(heartbeat.steamId) ||
            !Number.isSafeInteger(heartbeat.lastSeenMs)) {
            throw new Error('home_heartbeats_invalid');
        }
        heartbeats[serverId] = { ...heartbeat };
    }
    return { version: 1, heartbeats };
}
export function isHomeHeartbeatFresh(store, serverId, now = Date.now()) {
    const heartbeat = store.heartbeats[serverId];
    return Boolean(heartbeat && now - heartbeat.lastSeenMs <= HOME_HEARTBEAT_TTL_MS);
}
//# sourceMappingURL=home-heartbeats.js.map