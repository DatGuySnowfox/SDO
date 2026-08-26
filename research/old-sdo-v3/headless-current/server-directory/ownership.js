import { readFile } from 'node:fs/promises';
const SERVER_ID = /^(?:customer|trial)-world-[0-9]{1,3}$/;
const STEAM_ID = /^steam_7656119[0-9]{10}$/;
export async function loadOwnership(path) {
    const text = await readFile(path, 'utf8');
    const input = JSON.parse(text.replace(/^\uFEFF/, ''));
    if (input.version !== 1 || !input.owners || typeof input.owners !== 'object') {
        throw new Error('ownership_invalid');
    }
    const owners = {};
    for (const [serverId, values] of Object.entries(input.owners)) {
        if (!SERVER_ID.test(serverId) || !Array.isArray(values) || values.length > 8) {
            throw new Error('ownership_invalid');
        }
        const unique = [...new Set(values)];
        if (unique.length !== values.length || unique.some((value) => !STEAM_ID.test(value))) {
            throw new Error('ownership_invalid');
        }
        owners[serverId] = unique;
    }
    return { version: 1, owners };
}
export function ownedServerIds(ownership, steamId) {
    return Object.entries(ownership.owners)
        .filter(([, owners]) => owners.includes(steamId))
        .map(([serverId]) => serverId);
}
//# sourceMappingURL=ownership.js.map