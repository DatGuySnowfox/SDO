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
    if ((value.schemaVersion !== 1 && value.schemaVersion !== 2 && value.schemaVersion !== 3) ||
        typeof value.id !== 'string' || !ID.test(value.id) ||
        typeof value.name !== 'string' || value.name.length < 1 || value.name.length > 64 ||
        typeof value.version !== 'string' || !VERSION.test(value.version) ||
        typeof value.author !== 'string' || value.author.length < 1 || value.author.length > 64 ||
        typeof value.description !== 'string' || value.description.length > 300 ||
        !safeText(value.name) || !safeText(value.author) || !safeText(value.description) ||
        !value.rules || typeof value.rules !== 'object' || Array.isArray(value.rules)) {
        throw new Error('mod_manifest_invalid');
    }
    exactKeys(value, value.schemaVersion === 3
        ? ['schemaVersion', 'id', 'name', 'version', 'author', 'description', 'rules', 'content']
        : ['schemaVersion', 'id', 'name', 'version', 'author', 'description', 'rules']);
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
    let content;
    if (value.schemaVersion === 3 && value.content !== undefined) {
        const candidate = value.content;
        if (!candidate || typeof candidate !== 'object' || Array.isArray(candidate)) {
            throw new Error('mod_content_invalid');
        }
        exactKeys(candidate, [
            'codeLanguage', 'codeSha256', 'files', 'reviewRequired'
        ]);
        if (candidate.codeLanguage !== 'sdo-json-v1' ||
            typeof candidate.codeSha256 !== 'string' ||
            !/^[a-f0-9]{64}$/.test(candidate.codeSha256) ||
            !Array.isArray(candidate.files) ||
            candidate.files.length > 64 ||
            typeof candidate.reviewRequired !== 'boolean')
            throw new Error('mod_content_invalid');
        const seen = new Set();
        const files = candidate.files.map((file) => {
            if (!file || typeof file !== 'object' || Array.isArray(file)) {
                throw new Error('mod_content_invalid');
            }
            exactKeys(file, [
                'path', 'kind', 'size', 'sha256'
            ]);
            if (typeof file.path !== 'string' ||
                !/^[a-zA-Z0-9][a-zA-Z0-9._/-]{0,159}$/.test(file.path) ||
                file.path.includes('..') || file.path.includes('\\') ||
                seen.has(file.path) ||
                !['config', 'texture', 'audio', 'unreal'].includes(file.kind) ||
                !Number.isSafeInteger(file.size) || file.size < 0 || file.size > 32 * 1024 * 1024 ||
                typeof file.sha256 !== 'string' || !/^[a-f0-9]{64}$/.test(file.sha256))
                throw new Error('mod_content_invalid');
            seen.add(file.path);
            return { path: file.path, kind: file.kind, size: file.size, sha256: file.sha256 };
        });
        if (files.reduce((total, file) => total + file.size, 0) > 32 * 1024 * 1024) {
            throw new Error('mod_content_too_large');
        }
        if (candidate.reviewRequired !== files.some((file) => file.kind === 'unreal')) {
            throw new Error('mod_content_review_invalid');
        }
        content = {
            codeLanguage: 'sdo-json-v1',
            codeSha256: candidate.codeSha256,
            files,
            reviewRequired: candidate.reviewRequired
        };
    }
    if (!rules.time && !rules.weather && !content)
        throw new Error('mod_rules_empty');
    return {
        schemaVersion: value.schemaVersion,
        id: value.id,
        name: value.name,
        version: value.version,
        author: value.author,
        description: value.description,
        rules,
        ...(content ? { content } : {})
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
            rules: entry.rules,
            ...(entry.content ? { content: entry.content } : {})
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
export async function loadModDrafts(path, now = Date.now()) {
    const value = await readJsonOr(path, { version: 1, drafts: [] });
    if (!value || typeof value !== 'object' || value.version !== 1 ||
        !Array.isArray(value.drafts) ||
        value.drafts.length > 500)
        throw new Error('mod_drafts_invalid');
    const drafts = value.drafts.map((entry) => {
        const manifest = validateManifest({
            schemaVersion: entry.schemaVersion, id: entry.id, name: entry.name,
            version: entry.version, author: entry.author, description: entry.description,
            rules: entry.rules, ...(entry.content ? { content: entry.content } : {})
        });
        if (!/^draft:[a-f0-9]{20}$/.test(entry.key) ||
            !/^[a-f0-9]{64}$/.test(entry.hash) ||
            !/^steam_7656119[0-9]{10}$/.test(entry.ownerSteamId) ||
            !Number.isSafeInteger(entry.updatedAtMs) ||
            !Number.isSafeInteger(entry.expiresAtMs) ||
            entry.expiresAtMs <= entry.updatedAtMs ||
            entry.expiresAtMs - entry.updatedAtMs > 48 * 60 * 60_000)
            throw new Error('mod_drafts_invalid');
        return { ...manifest, key: entry.key, hash: entry.hash,
            ownerSteamId: entry.ownerSteamId, updatedAtMs: entry.updatedAtMs,
            expiresAtMs: entry.expiresAtMs };
    }).filter((entry) => entry.expiresAtMs > now);
    return { version: 1, drafts };
}
export async function saveModDrafts(path, value) {
    await atomicJson(path, value);
}
export function upsertModDraft(store, manifestInput, ownerSteamId, now = Date.now()) {
    const manifest = validateManifest(manifestInput);
    const ownerDrafts = store.drafts.filter((entry) => entry.ownerSteamId === ownerSteamId && entry.expiresAtMs > now);
    const existing = ownerDrafts.find((entry) => entry.id === manifest.id);
    if (!existing && ownerDrafts.length >= 10)
        throw new Error('mod_draft_limit');
    const key = existing?.key ?? `draft:${createHash('sha256')
        .update(`${ownerSteamId}\0${manifest.id}`).digest('hex').slice(0, 20)}`;
    const draft = {
        ...manifest,
        key,
        hash: createHash('sha256').update(JSON.stringify(manifest)).digest('hex'),
        ownerSteamId,
        updatedAtMs: now,
        expiresAtMs: now + 48 * 60 * 60_000
    };
    return {
        store: {
            version: 1,
            drafts: [...store.drafts.filter((entry) => entry.expiresAtMs > now && entry.key !== key), draft]
        },
        draft,
        replaced: Boolean(existing)
    };
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