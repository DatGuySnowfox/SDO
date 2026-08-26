import { mkdir, readFile, rename, writeFile } from 'node:fs/promises';
import { dirname } from 'node:path';
const STEAM_ID = /^steam_7656119[0-9]{10}$/;
const SERVER_ID = /^trial-world-[0-9]{1,3}$/;
export async function loadTrials(path) {
    let text;
    try {
        text = await readFile(path, 'utf8');
    }
    catch (error) {
        if (error.code === 'ENOENT') {
            return { version: 1, leases: {} };
        }
        throw error;
    }
    return validateTrials(JSON.parse(text.replace(/^\uFEFF/, '')));
}
export async function saveTrials(path, value) {
    const validated = validateTrials(value);
    await mkdir(dirname(path), { recursive: true });
    const temporary = `${path}.${process.pid}.tmp`;
    await writeFile(temporary, `${JSON.stringify(validated, null, 2)}\n`, {
        encoding: 'utf8',
        mode: 0o600
    });
    await rename(temporary, path);
}
export function validateTrials(input) {
    if (!input || typeof input !== 'object' || Array.isArray(input)) {
        throw new Error('trial_store_invalid');
    }
    const value = input;
    if (value.version !== 1 || !value.leases || typeof value.leases !== 'object') {
        throw new Error('trial_store_invalid');
    }
    const leases = {};
    const serverIds = new Set();
    for (const [steamId, lease] of Object.entries(value.leases)) {
        const name = typeof lease?.name === 'string'
            ? lease.name
            : '48-HOUR TRIAL SERVER';
        if (!STEAM_ID.test(steamId) ||
            !lease ||
            lease.steamId !== steamId ||
            name.length < 1 ||
            name.length > 64 ||
            /[\u0000-\u001F\u007F<>|]/.test(name) ||
            !SERVER_ID.test(lease.serverId) ||
            serverIds.has(lease.serverId) ||
            !Number.isSafeInteger(lease.startedAtMs) ||
            !Number.isSafeInteger(lease.expiresAtMs) ||
            lease.expiresAtMs !== lease.startedAtMs + 48 * 60 * 60 * 1_000 ||
            !['active', 'expired', 'converted'].includes(lease.status)) {
            throw new Error('trial_store_invalid');
        }
        serverIds.add(lease.serverId);
        leases[steamId] = { ...lease, name };
    }
    return { version: 1, leases };
}
export function trialForServer(store, serverId) {
    return Object.values(store.leases).find((lease) => lease.serverId === serverId);
}
//# sourceMappingURL=trials.js.map