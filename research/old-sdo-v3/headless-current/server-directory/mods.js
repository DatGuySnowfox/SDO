import { createHash } from 'node:crypto';
import { mkdir, readFile, rename, writeFile } from 'node:fs/promises';
import { dirname, join } from 'node:path';
const ID = /^[a-z0-9][a-z0-9-]{1,31}\.[a-z0-9][a-z0-9-]{1,31}$/;
const VERSION = /^[0-9]+\.[0-9]+\.[0-9]+$/;
const SERVER_ID = /^(?:customer|trial)-world-[0-9]{1,3}$/;
const WEATHER = new Set(['clear', 'overcast', 'rain', 'storm', 'snow']);
export function validateManifest(input) {
    if (!input || typeof input !== 'object' || Array.isArray(input)) {
        throw new Error('mod_manifest_invalid');
    }
    const value = input;
    exactKeys(value, [
        'schemaVersion', 'id', 'name', 'version', 'author', 'description', 'rules'
    ]);
    if ((value.schemaVersion !== 1 && value.schemaVersion !== 2) ||
        typeof value.id !== 'string' || !ID.test(value.id) ||
        typeof value.name !== 'string' || value.name.length < 1 || value.name.length > 64 ||
        typeof value.version !== 'string' || !VERSION.test(value.version) ||
        typeof value.author !== 'string' || value.author.length < 1 || value.author.length > 64 ||
        typeof value.description !== 'string' || value.description.length > 300 ||
        !safeText(value.name) || !safeText(value.author) || !safeText(value.description) ||
        !value.rules || typeof value.rules !== 'object' || Array.isArray(value.rules)) {
        throw new Error('mod_manifest_invalid');
    }
    exactKeys(value.rules, ['time', 'weather']);
    const rules = {};
    if (value.rules.time !== undefined) {
        const time = value.rules.time;
        exactKeys(time, value.schemaVersion === 1 ? ['dayLengthMinutes'] : ['dayLengthMinutes', 'mode', 'startTime']);
        if (time.dayLengthMinutes !== undefined && (!Number.isSafeInteger(time.dayLengthMinutes) ||
            time.dayLengthMinutes < 30 ||
            time.dayLengthMinutes > 720))
            throw new Error('mod_time_rule_invalid');
        if (time.mode !== undefined && !['cycle', 'frozen'].includes(time.mode) ||
            time.startTime !== undefined && (!Number.isSafeInteger(time.startTime) || time.startTime < 0 || time.startTime > 2399) ||
            time.mode === 'frozen' && time.startTime === undefined ||
            time.dayLengthMinutes === undefined && time.mode === undefined &&
                time.startTime === undefined)
            throw new Error('mod_time_rule_invalid');
        rules.time = {
            ...(time.dayLengthMinutes === undefined ? {} :
                { dayLengthMinutes: time.dayLengthMinutes }),
            ...(time.mode === undefined ? {} : { mode: time.mode }),
            ...(time.startTime === undefined ? {} : { startTime: time.startTime })
        };
    }
    if (value.rules.weather !== undefined) {
        const weather = value.rules.weather;
        exactKeys(weather, value.schemaVersion === 1 ? ['preset'] :
            ['preset', 'cycle', 'phaseMinutes', 'transition']);
        if (weather.preset !== undefined && !WEATHER.has(weather.preset)) {
            throw new Error('mod_weather_rule_invalid');
        }
        if (weather.cycle !== undefined && (!Array.isArray(weather.cycle) ||
            weather.cycle.length < 2 || weather.cycle.length > 8 ||
            weather.cycle.some((preset) => !WEATHER.has(preset))))
            throw new Error('mod_weather_rule_invalid');
        if (weather.preset !== undefined && weather.cycle !== undefined ||
            weather.preset === undefined && weather.cycle === undefined ||
            weather.phaseMinutes !== undefined && (!Number.isSafeInteger(weather.phaseMinutes) ||
                weather.phaseMinutes < 1 || weather.phaseMinutes > 120) ||
            weather.transition !== undefined &&
                !['smooth', 'instant'].includes(weather.transition))
            throw new Error('mod_weather_rule_invalid');
        rules.weather = weather.preset !== undefined
            ? { preset: weather.preset }
            : {
                cycle: [...weather.cycle],
                phaseMinutes: weather.phaseMinutes ?? 10,
                transition: weather.transition ?? 'smooth'
            };
    }
    if (!rules.time && !rules.weather)
        throw new Error('mod_rules_empty');
    return {
        schemaVersion: value.schemaVersion,
        id: value.id,
        name: value.name,
        version: value.version,
        author: value.author,
        description: value.description,
        rules
    };
}
export async function loadModRegistry(path) {
    const value = await readJsonOr(path, { version: 1, mods: [] });
    if (!value || typeof value !== 'object' ||
        value.version !== 1 ||
        !Array.isArray(value.mods) ||
        value.mods.length > 500)
        throw new Error('mod_registry_invalid');
    const keys = new Set();
    const mods = value.mods.map((entry) => {
        const manifest = validateManifest({
            schemaVersion: entry.schemaVersion,
            id: entry.id,
            name: entry.name,
            version: entry.version,
            author: entry.author,
            description: entry.description,
            rules: entry.rules
        });
        const key = `${manifest.id}@${manifest.version}`;
        if (entry.key !== key || keys.has(key) ||
            !/^[a-f0-9]{64}$/.test(entry.hash) ||
            !/^steam_7656119[0-9]{10}$/.test(entry.publisherSteamId) ||
            !Number.isSafeInteger(entry.publishedAtMs))
            throw new Error('mod_registry_invalid');
        keys.add(key);
        return { ...manifest, key, hash: entry.hash, publisherSteamId: entry.publisherSteamId,
            publishedAtMs: entry.publishedAtMs };
    });
    return { version: 1, mods };
}
export async function saveModRegistry(path, value) {
    await atomicJson(path, value);
}
export async function loadModAssignments(path) {
    const value = await readJsonOr(path, { version: 1, servers: {} });
    if (!value || typeof value !== 'object' ||
        value.version !== 1 ||
        !value.servers ||
        typeof value.servers !== 'object')
        throw new Error('mod_assignments_invalid');
    const servers = {};
    for (const [serverId, keys] of Object.entries(value.servers)) {
        if (!SERVER_ID.test(serverId) || !Array.isArray(keys) || keys.length > 16 ||
            keys.some((key) => typeof key !== 'string' || key.length > 80) ||
            new Set(keys).size !== keys.length)
            throw new Error('mod_assignments_invalid');
        servers[serverId] = [...keys];
    }
    return { version: 1, servers };
}
export async function saveModAssignments(path, value) {
    await atomicJson(path, value);
}
export function registerMod(registry, manifestInput, publisherSteamId, now = Date.now()) {
    const manifest = validateManifest(manifestInput);
    const key = `${manifest.id}@${manifest.version}`;
    const canonical = JSON.stringify(manifest);
    const hash = createHash('sha256').update(canonical).digest('hex');
    const existing = registry.mods.find((entry) => entry.key === key);
    if (existing) {
        if (existing.hash !== hash)
            throw new Error('mod_version_immutable');
        return { registry, mod: existing, created: false };
    }
    const mod = {
        ...manifest, key, hash, publisherSteamId, publishedAtMs: now
    };
    return { registry: { version: 1, mods: [...registry.mods, mod] }, mod, created: true };
}
export function mergeRules(mods) {
    const rules = {};
    for (const mod of mods) {
        if (mod.rules.time)
            rules.time = { ...mod.rules.time };
        if (mod.rules.weather)
            rules.weather = { ...mod.rules.weather };
    }
    return rules;
}
export async function saveServerModRules(root, serverId, worldId, modKeys, rules) {
    const path = join(root, `${worldId}.json`);
    await atomicJson(path, {
        schemaVersion: 1,
        serverId,
        worldId,
        modKeys,
        rules
    });
    return path;
}
function safeText(value) {
    return !/[\u0000-\u001F\u007F<>]/.test(value);
}
function exactKeys(value, allowed) {
    if (Object.keys(value).some((key) => !allowed.includes(key))) {
        throw new Error('mod_manifest_unknown_field');
    }
}
async function readJsonOr(path, fallback) {
    try {
        return JSON.parse((await readFile(path, 'utf8')).replace(/^\uFEFF/, ''));
    }
    catch (error) {
        if (error.code === 'ENOENT')
            return fallback;
        throw error;
    }
}
async function atomicJson(path, value) {
    await mkdir(dirname(path), { recursive: true });
    const temporary = `${path}.${process.pid}.tmp`;
    await writeFile(temporary, `${JSON.stringify(value, null, 2)}\n`, {
        encoding: 'utf8', mode: 0o600
    });
    await rename(temporary, path);
}
//# sourceMappingURL=mods.js.map