import { mkdir, readFile, rename, writeFile } from 'node:fs/promises';
import { dirname } from 'node:path';
export const TRIAL_STANDARD_DURATION_MS = 48 * 60 * 60 * 1_000;
export const TRIAL_BOOSTED_DURATION_MS = 4 * 24 * 60 * 60 * 1_000;
const STEAM_ID = /^steam_7656119[0-9]{10}$/;
const SERVER_ID = /^trial-world-[0-9]{1,3}$/;
const DISCORD_USER_ID = /^[0-9]{17,20}$/;
export function normalizeSteamId(value) {
    const trimmed = value.trim();
    if (STEAM_ID.test(trimmed))
        return trimmed;
    if (/^7656119[0-9]{10}$/.test(trimmed))
        return `steam_${trimmed}`;
    return undefined;
}
export function trialDurationMs(lease) {
    return lease.expiresAtMs - lease.startedAtMs;
}
export function isValidTrialDurationMs(durationMs, boosted) {
    return durationMs === (boosted ? TRIAL_BOOSTED_DURATION_MS : TRIAL_STANDARD_DURATION_MS);
}
export function hasBoostCredit(store, steamId) {
    return Boolean(store.boostCredits?.[steamId]);
}
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
        const boosted = Boolean(lease?.discordBoostApplied);
        const durationMs = lease ? lease.expiresAtMs - lease.startedAtMs : 0;
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
            !isValidTrialDurationMs(durationMs, boosted) ||
            (lease.discordUserId !== undefined && !DISCORD_USER_ID.test(lease.discordUserId)) ||
            !['active', 'expired', 'converted'].includes(lease.status)) {
            throw new Error('trial_store_invalid');
        }
        serverIds.add(lease.serverId);
        leases[steamId] = {
            ...lease,
            name,
            discordBoostApplied: boosted || undefined,
            discordUserId: lease.discordUserId
        };
    }
    const boostCredits = {};
    for (const [steamId, credit] of Object.entries(value.boostCredits ?? {})) {
        if (!STEAM_ID.test(steamId) ||
            !credit ||
            !DISCORD_USER_ID.test(credit.discordUserId) ||
            !Number.isSafeInteger(credit.grantedAtMs)) {
            throw new Error('trial_store_invalid');
        }
        boostCredits[steamId] = credit;
    }
    const discordBoostClaims = {};
    for (const [discordUserId, claimedSteamId] of Object.entries(value.discordBoostClaims ?? {})) {
        if (!DISCORD_USER_ID.test(discordUserId) || !STEAM_ID.test(claimedSteamId)) {
            throw new Error('trial_store_invalid');
        }
        discordBoostClaims[discordUserId] = claimedSteamId;
    }
    return {
        version: 1,
        leases,
        boostCredits: Object.keys(boostCredits).length > 0 ? boostCredits : undefined,
        discordBoostClaims: Object.keys(discordBoostClaims).length > 0 ? discordBoostClaims : undefined
    };
}
export function trialForServer(store, serverId) {
    return Object.values(store.leases).find((lease) => lease.serverId === serverId);
}
export function applyDiscordBoost(store, steamId, discordUserId) {
    store.boostCredits ??= {};
    store.discordBoostClaims ??= {};
    const claimedSteamId = store.discordBoostClaims[discordUserId];
    if (claimedSteamId && claimedSteamId !== steamId) {
        return { ok: false, error: 'discord_boost_already_claimed' };
    }
    const existing = store.leases[steamId];
    if (existing) {
        if (existing.discordBoostApplied) {
            return { ok: false, error: 'trial_boost_already_applied' };
        }
        if (existing.status === 'expired' || existing.status === 'converted') {
            return { ok: false, error: 'trial_already_used' };
        }
        if (existing.status !== 'active') {
            return { ok: false, error: 'trial_not_active' };
        }
        existing.discordBoostApplied = true;
        existing.discordUserId = discordUserId;
        existing.expiresAtMs = existing.startedAtMs + TRIAL_BOOSTED_DURATION_MS;
        store.discordBoostClaims[discordUserId] = steamId;
        return { ok: true, mode: 'extended', lease: existing };
    }
    if (store.boostCredits[steamId]) {
        return { ok: false, error: 'trial_boost_credit_pending' };
    }
    store.boostCredits[steamId] = { discordUserId, grantedAtMs: Date.now() };
    store.discordBoostClaims[discordUserId] = steamId;
    return { ok: true, mode: 'credit' };
}
//# sourceMappingURL=trials.js.map