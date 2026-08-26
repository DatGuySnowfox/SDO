import { createHash, createHmac, randomBytes, randomUUID, timingSafeEqual } from 'node:crypto';
import { execFile, spawn } from 'node:child_process';
import { access } from 'node:fs/promises';
import { mkdir, readFile, rename, writeFile } from 'node:fs/promises';
import { createServer } from 'node:http';
import { dirname, join } from 'node:path';
import { promisify } from 'node:util';
import { issueJoinTicket } from "../shared-protocol/index.js";
import { hashPassword, loadRegistry, saveRegistry, verifyPassword } from "./store.js";
import { loadOwnership, ownedServerIds } from "./ownership.js";
import { applyDiscordBoost, hasBoostCredit, loadTrials, normalizeSteamId, saveTrials, trialForServer, TRIAL_BOOSTED_DURATION_MS, TRIAL_STANDARD_DURATION_MS } from "./trials.js";
import { buildServerIconUrl, deleteServerIcon, parseServerIconUpload, readServerIcon, saveServerIcon, serverIconEtag } from "./icons.js";
import { isHomeHeartbeatFresh, loadHomeHeartbeats, saveHomeHeartbeats } from "./home-heartbeats.js";
import { loadModAssignments, loadModDrafts, loadModRegistry, mergeRules, registerMod, saveModAssignments, saveModDrafts, saveModRegistry, saveServerModRules, upsertModDraft } from "./mods.js";
const executeFile = promisify(execFile);
function parseDotEnv(raw) {
    const values = {};
    for (const line of raw.split(/\r?\n/u)) {
        const trimmed = line.trim();
        if (!trimmed || trimmed.startsWith('#'))
            continue;
        const index = trimmed.indexOf('=');
        if (index < 1)
            continue;
        values[trimmed.slice(0, index).trim()] = trimmed.slice(index + 1).trim();
    }
    return values;
}
export class ServerDirectoryService {
    config;
    passwordFailures = new Map();
    steamDevices = new Map();
    controlActions = new Map();
    homeHeartbeatOperation = Promise.resolve();
    server;
    trialTimer;
    trialOperation = Promise.resolve();
    modOperation = Promise.resolve();
    registryOperation = Promise.resolve();
    constructor(config) {
        this.config = config;
    }
    async start() {
        await loadRegistry(this.config.registryPath);
        this.server = createServer((request, response) => {
            void this.handle(request, response).catch((error) => {
                console.error(JSON.stringify({
                    timestamp: new Date().toISOString(),
                    level: 'error',
                    event: 'directory_request_error',
                    protocolVersion: 3,
                    method: request.method,
                    path: (request.url ?? '').split('?')[0],
                    error: error instanceof Error ? error.message : 'unknown_error'
                }));
                this.send(response, 500, { ok: false, error: 'directory_error' });
            });
        });
        await new Promise((resolve, reject) => {
            this.server.once('error', reject);
            this.server.listen(this.config.port ?? 31100, this.config.host ?? '127.0.0.1', resolve);
        });
        if (this.config.trialPath && this.customerControlEnabled()) {
            this.trialTimer = setInterval(() => void this.expireTrials(), 1_000);
            this.trialTimer.unref();
            void this.expireTrials();
        }
        return { port: this.server.address().port };
    }
    async stop() {
        if (this.trialTimer)
            clearInterval(this.trialTimer);
        this.trialTimer = undefined;
        if (!this.server)
            return;
        await new Promise((resolve) => this.server.close(() => resolve()));
        this.server = undefined;
    }
    async handle(request, response) {
        const url = new URL(request.url ?? '/', 'http://directory.local');
        if (request.method === 'GET' && url.pathname === '/health') {
            this.send(response, 200, { ok: true, service: 'sdo-server-directory', version: 1 });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/auth/device') {
            if (!this.publicAuthEnabled()) {
                return void this.send(response, 404, { ok: false, error: 'steam_auth_not_configured' });
            }
            let requestedPublicBase;
            try {
                const raw = await this.readBody(request, 1024);
                if (raw.trim()) {
                    const body = JSON.parse(raw);
                    requestedPublicBase = body.publicBaseUrl;
                }
            }
            catch {
                return void this.send(response, 400, { ok: false, error: 'auth_device_invalid' });
            }
            const authPublicBase = this.resolveAuthPublicBase(requestedPublicBase, this.inferAuthPublicBaseFromRequest(request));
            const deviceId = randomUUID();
            const pollToken = randomBytes(32).toString('base64url');
            const browserNonce = randomBytes(24).toString('base64url');
            const expiresAtMs = Date.now() + 5 * 60_000;
            this.steamDevices.set(deviceId, {
                pollHash: createHash('sha256').update(pollToken).digest('hex'),
                browserNonce,
                expiresAtMs,
                publicBase: authPublicBase
            });
            this.pruneDevices();
            this.send(response, 201, {
                ok: true,
                deviceId,
                pollToken,
                expiresAtMs,
                loginUrl: `${authPublicBase}/v1/auth/steam/start?device=${encodeURIComponent(deviceId)}&nonce=${browserNonce}`
            });
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/auth/steam/start') {
            const device = this.validBrowserDevice(url);
            if (!device)
                return void this.send(response, 400, { ok: false, error: 'auth_device_invalid' });
            const authPublicBase = device.publicBase;
            const returnTo = `${authPublicBase}/v1/auth/steam/callback?device=${encodeURIComponent(url.searchParams.get('device'))}&nonce=${encodeURIComponent(device.browserNonce)}`;
            const parameters = new URLSearchParams({
                'openid.ns': 'http://specs.openid.net/auth/2.0',
                'openid.mode': 'checkid_setup',
                'openid.return_to': returnTo,
                'openid.realm': `${new URL(authPublicBase).origin}/`,
                'openid.identity': 'http://specs.openid.net/auth/2.0/identifier_select',
                'openid.claimed_id': 'http://specs.openid.net/auth/2.0/identifier_select'
            });
            response.writeHead(302, {
                location: `https://steamcommunity.com/openid/login?${parameters}`,
                'cache-control': 'no-store'
            }).end();
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/auth/steam/callback') {
            const device = this.validBrowserDevice(url);
            if (!device)
                return void this.sendHtml(response, 400, 'Steam login expired. Return to the launcher.');
            const verification = new URLSearchParams();
            for (const [key, value] of url.searchParams) {
                if (key.startsWith('openid.'))
                    verification.set(key, value);
            }
            verification.set('openid.mode', 'check_authentication');
            const checked = await fetch('https://steamcommunity.com/openid/login', {
                method: 'POST',
                headers: { 'content-type': 'application/x-www-form-urlencoded' },
                body: verification,
                signal: AbortSignal.timeout(10_000)
            });
            const result = await checked.text();
            const claimed = url.searchParams.get('openid.claimed_id') ?? '';
            const match = /^https?:\/\/steamcommunity\.com\/openid\/id\/(7656119[0-9]{10})$/.exec(claimed);
            if (!checked.ok || !/(?:^|\n)is_valid:true(?:\n|$)/.test(result) || !match) {
                return void this.sendHtml(response, 403, 'Steam could not verify this account.');
            }
            device.steamId = `steam_${match[1]}`;
            this.sendHtml(response, 200, 'Steam account verified. You can return to SD-Online.');
            return;
        }
        const deviceMatch = /^\/v1\/auth\/device\/([0-9a-f-]{36})$/.exec(url.pathname);
        if (request.method === 'GET' && deviceMatch) {
            const device = this.steamDevices.get(deviceMatch[1]);
            const pollToken = this.bearer(request);
            if (!device ||
                device.expiresAtMs <= Date.now() ||
                !safeEqual(device.pollHash, createHash('sha256').update(pollToken).digest('hex'))) {
                return void this.send(response, 403, { ok: false, error: 'auth_device_invalid' });
            }
            if (!device.steamId)
                return void this.send(response, 202, { ok: true, pending: true });
            const identityToken = this.signIdentity(device.steamId, Date.now() + 15 * 60_000);
            const refreshExpiresAtMs = Date.now() + 30 * 24 * 60 * 60_000;
            const refreshToken = this.signAuthToken(device.steamId, 'refresh', refreshExpiresAtMs);
            this.steamDevices.delete(deviceMatch[1]);
            this.send(response, 200, {
                ok: true,
                identityToken,
                refreshToken,
                refreshExpiresAtMs,
                steamId: device.steamId
            });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/auth/refresh') {
            if (!this.publicAuthEnabled()) {
                return void this.send(response, 404, { ok: false, error: 'steam_auth_not_configured' });
            }
            const steamId = this.verifyAuthToken(this.bearer(request), 'refresh');
            if (!steamId) {
                return void this.send(response, 401, { ok: false, error: 'steam_refresh_invalid' });
            }
            this.send(response, 200, {
                ok: true,
                identityToken: this.signIdentity(steamId, Date.now() + 15 * 60_000),
                steamId
            });
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/servers') {
            const registry = await loadRegistry(this.config.registryPath);
            const trials = this.config.trialPath
                ? await loadTrials(this.config.trialPath)
                : { version: 1, leases: {} };
            const visible = [];
            for (const server of registry.servers) {
                if (/^trial-world-[0-9]{1,3}$/.test(server.id)) {
                    const lease = trialForServer(trials, server.id);
                    if (lease?.status === 'active' || lease?.status === 'converted')
                        visible.push(server);
                    continue;
                }
                if (/^home-world-[0-9]{1,3}$/.test(server.id)) {
                    if (await this.isHomeWorldPublic(server.id))
                        visible.push(server);
                    continue;
                }
                visible.push(server);
            }
            const servers = await Promise.all(visible
                .map(async (server) => {
                const listed = await this.publicServer(server);
                const lease = trialForServer(trials, server.id);
                return lease?.status === 'active'
                    ? {
                        ...listed,
                        name: lease.name,
                        description: 'Active 48-hour community trial',
                        isTrial: true,
                        trial: this.publicTrial(lease)
                    }
                    : listed;
            }));
            servers.sort((left, right) => Number(right.pinned) - Number(left.pinned) || left.name.localeCompare(right.name));
            this.send(response, 200, { ok: true, servers });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/home-host/heartbeat') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.homeHeartbeatsPath || !this.config.ownershipPath) {
                return void this.send(response, 503, { ok: false, error: 'home_host_unavailable' });
            }
            const body = JSON.parse(await this.readBody(request, 1_024));
            const serverId = typeof body.serverId === 'string' ? body.serverId.trim() : '';
            if (!/^home-world-[0-9]{1,3}$/.test(serverId)) {
                return void this.send(response, 400, { ok: false, error: 'server_id_invalid' });
            }
            const ownership = await loadOwnership(this.config.ownershipPath);
            if (!ownedServerIds(ownership, steamId).includes(serverId)) {
                return void this.send(response, 403, { ok: false, error: 'owned_server_not_found' });
            }
            const now = Date.now();
            await this.serializedHomeHeartbeat(async () => {
                const store = await loadHomeHeartbeats(this.config.homeHeartbeatsPath);
                store.heartbeats[serverId] = { serverId, steamId, lastSeenMs: now };
                await saveHomeHeartbeats(this.config.homeHeartbeatsPath, store);
            });
            this.send(response, 200, {
                ok: true,
                serverId,
                heartbeatAcceptedAtMs: now
            });
            return;
        }
        const iconMatch = /^\/v1\/servers\/([a-z0-9][a-z0-9-]{2,47})\/icon$/.exec(url.pathname);
        if (request.method === 'GET' && iconMatch) {
            if (!this.config.iconsPath) {
                return void this.send(response, 404, { ok: false, error: 'server_icon_not_found' });
            }
            const serverId = iconMatch[1];
            const registry = await loadRegistry(this.config.registryPath);
            const server = registry.servers.find((candidate) => candidate.id === serverId);
            if (!server?.iconUpdatedAtMs) {
                return void this.send(response, 404, { ok: false, error: 'server_icon_not_found' });
            }
            const icon = await readServerIcon(this.config.iconsPath, serverId);
            if (!icon) {
                return void this.send(response, 404, { ok: false, error: 'server_icon_not_found' });
            }
            this.sendBinary(response, 200, icon.buffer, icon.contentType, {
                'cache-control': 'public, max-age=300',
                etag: serverIconEtag(icon.buffer)
            });
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/mods') {
            if (!this.config.modRegistryPath) {
                return void this.send(response, 503, { ok: false, error: 'mod_registry_unavailable' });
            }
            const registry = await loadModRegistry(this.config.modRegistryPath);
            const mods = registry.mods.map(({ publisherSteamId: _publisher, ...mod }) => mod);
            this.send(response, 200, { ok: true, mods });
            return;
        }
        if (url.pathname === '/v1/mods/drafts' &&
            (request.method === 'GET' || request.method === 'POST')) {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.modDraftPath) {
                return void this.send(response, 503, { ok: false, error: 'mod_drafts_unavailable' });
            }
            if (request.method === 'GET') {
                const store = await loadModDrafts(this.config.modDraftPath);
                this.send(response, 200, {
                    ok: true,
                    drafts: store.drafts
                        .filter((draft) => draft.ownerSteamId === steamId)
                        .map(({ ownerSteamId: _owner, ...draft }) => draft)
                });
                return;
            }
            const input = JSON.parse(await this.readBody(request, 256 * 1024));
            const manifest = input && typeof input === 'object' && 'manifest' in input
                ? input.manifest
                : input;
            const result = await this.serializedMod(async () => {
                const store = await loadModDrafts(this.config.modDraftPath);
                const updated = upsertModDraft(store, manifest, steamId);
                await saveModDrafts(this.config.modDraftPath, updated.store);
                return updated;
            });
            this.send(response, result.replaced ? 200 : 201, {
                ok: true,
                replaced: result.replaced,
                draft: {
                    key: result.draft.key,
                    id: result.draft.id,
                    name: result.draft.name,
                    version: result.draft.version,
                    hash: result.draft.hash,
                    expiresAtMs: result.draft.expiresAtMs,
                    hidden: true
                }
            });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/mods/submit') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.modRegistryPath) {
                return void this.send(response, 503, { ok: false, error: 'mod_registry_unavailable' });
            }
            const manifest = JSON.parse(await this.readBody(request, 32_768));
            const result = await this.serializedMod(async () => {
                const registry = await loadModRegistry(this.config.modRegistryPath);
                const registered = registerMod(registry, manifest, steamId);
                if (registered.created) {
                    await saveModRegistry(this.config.modRegistryPath, registered.registry);
                }
                return registered;
            });
            this.send(response, result.created ? 201 : 200, {
                ok: true,
                created: result.created,
                mod: {
                    key: result.mod.key,
                    id: result.mod.id,
                    name: result.mod.name,
                    version: result.mod.version,
                    author: result.mod.author,
                    description: result.mod.description,
                    rules: result.mod.rules,
                    hash: result.mod.hash
                },
                scan: {
                    passed: true,
                    executableCode: false,
                    nativeCode: false,
                    networkAccess: false,
                    filesystemAccess: false,
                    schemaVersion: result.mod.schemaVersion
                }
            });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/mods/submit-package') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.modRegistryPath || !this.config.modPackageRoot) {
                return void this.send(response, 503, { ok: false, error: 'mod_packages_unavailable' });
            }
            const body = JSON.parse(await this.readBody(request, 48 * 1024 * 1024));
            if (typeof body.code !== 'string' || Buffer.byteLength(body.code) > 128 * 1024) {
                return void this.send(response, 400, { ok: false, error: 'mod_code_invalid' });
            }
            try {
                const codeValue = JSON.parse(body.code);
                this.validateModCode(codeValue);
            }
            catch {
                return void this.send(response, 400, { ok: false, error: 'mod_code_invalid' });
            }
            if (!Array.isArray(body.files) || body.files.length > 64) {
                return void this.send(response, 400, { ok: false, error: 'mod_files_invalid' });
            }
            const supplied = new Map();
            let total = 0;
            for (const entry of body.files) {
                if (!entry || typeof entry.path !== 'string' || typeof entry.dataBase64 !== 'string' ||
                    !/^[a-zA-Z0-9][a-zA-Z0-9._/-]{0,159}$/.test(entry.path) ||
                    entry.path.includes('..') || entry.path.includes('\\') ||
                    supplied.has(entry.path))
                    return void this.send(response, 400, { ok: false, error: 'mod_files_invalid' });
                const bytes = Buffer.from(entry.dataBase64, 'base64');
                if (bytes.toString('base64') !== entry.dataBase64 || bytes.length > 32 * 1024 * 1024) {
                    return void this.send(response, 400, { ok: false, error: 'mod_files_invalid' });
                }
                total += bytes.length;
                supplied.set(entry.path, { path: entry.path, dataBase64: entry.dataBase64 });
            }
            if (total > 32 * 1024 * 1024) {
                return void this.send(response, 413, { ok: false, error: 'mod_content_too_large' });
            }
            const result = await this.serializedMod(async () => {
                const registry = await loadModRegistry(this.config.modRegistryPath);
                const registered = registerMod(registry, body.manifest, steamId);
                const content = registered.mod.content;
                if (!content || createHash('sha256').update(body.code).digest('hex') !==
                    content.codeSha256 || content.files.length !== supplied.size) {
                    throw new Error('mod_package_manifest_mismatch');
                }
                for (const expected of content.files) {
                    const file = supplied.get(expected.path);
                    if (!file)
                        throw new Error('mod_package_manifest_mismatch');
                    const bytes = Buffer.from(file.dataBase64, 'base64');
                    if (bytes.length !== expected.size ||
                        createHash('sha256').update(bytes).digest('hex') !== expected.sha256)
                        throw new Error('mod_package_manifest_mismatch');
                }
                const packagePath = join(this.config.modPackageRoot, `${registered.mod.key}.sdomod`);
                await mkdir(dirname(packagePath), { recursive: true });
                const temporary = `${packagePath}.${process.pid}.tmp`;
                await writeFile(temporary, `${JSON.stringify({
                    schemaVersion: 1,
                    manifest: registered.mod,
                    code: body.code,
                    files: body.files
                })}\n`, { encoding: 'utf8', mode: 0o600 });
                await rename(temporary, packagePath);
                if (registered.created) {
                    await saveModRegistry(this.config.modRegistryPath, registered.registry);
                }
                return registered;
            });
            this.send(response, result.created ? 201 : 200, {
                ok: true,
                created: result.created,
                mod: {
                    key: result.mod.key,
                    name: result.mod.name,
                    version: result.mod.version,
                    hash: result.mod.hash,
                    reviewRequired: result.mod.content?.reviewRequired ?? false
                },
                scan: {
                    passed: true,
                    executableFiles: false,
                    packageHashVerified: true,
                    reviewRequired: result.mod.content?.reviewRequired ?? false
                }
            });
            return;
        }
        const packageMatch = /^\/v1\/mods\/([a-z0-9-]{2,32}\.[a-z0-9-]{2,32}@[0-9]+\.[0-9]+\.[0-9]+)\/package$/
            .exec(url.pathname);
        if (request.method === 'GET' && packageMatch) {
            if (!this.config.modPackageRoot) {
                return void this.send(response, 503, { ok: false, error: 'mod_packages_unavailable' });
            }
            try {
                const bytes = await readFile(join(this.config.modPackageRoot, `${packageMatch[1]}.sdomod`));
                response.writeHead(200, {
                    'content-type': 'application/vnd.sd-online.mod+json',
                    'content-length': bytes.length,
                    'cache-control': 'public, max-age=31536000, immutable',
                    'x-content-type-options': 'nosniff'
                });
                response.end(bytes);
            }
            catch {
                this.send(response, 404, { ok: false, error: 'mod_package_not_found' });
            }
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/trials/status') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.trialPath || !this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'trials_unavailable' });
            }
            await this.expireTrials();
            const store = await loadTrials(this.config.trialPath);
            const lease = store.leases[steamId];
            const registry = await loadRegistry(this.config.registryPath);
            const used = new Set(Object.values(store.leases).map((value) => value.serverId));
            let capacityAvailable = false;
            for (const server of registry.servers) {
                if (!this.isTrialSlotCandidate(server.id, used))
                    continue;
                if (await this.trialSlotProvisioned(server.id)) {
                    capacityAvailable = true;
                    break;
                }
            }
            this.send(response, 200, {
                ok: true,
                eligible: !lease,
                capacityAvailable,
                boostCreditAvailable: hasBoostCredit(store, steamId),
                trial: lease ? this.publicTrial(lease) : null
            });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/internal/trials/discord-boost') {
            if (!this.verifyDiscordBotSecret(request)) {
                return void this.send(response, 401, { ok: false, error: 'discord_bot_unauthorized' });
            }
            if (!this.config.trialPath || !this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'trials_unavailable' });
            }
            const body = JSON.parse(await this.readBody(request, 512));
            const steamId = typeof body.steamId === 'string' ? normalizeSteamId(body.steamId) : undefined;
            const discordUserId = typeof body.discordUserId === 'string'
                ? body.discordUserId.trim()
                : '';
            if (!steamId || !/^[0-9]{17,20}$/.test(discordUserId)) {
                return void this.send(response, 400, { ok: false, error: 'discord_boost_request_invalid' });
            }
            const result = await this.serializedTrial(async () => {
                const store = await loadTrials(this.config.trialPath);
                const applied = applyDiscordBoost(store, steamId, discordUserId);
                if (!applied.ok)
                    return applied;
                await saveTrials(this.config.trialPath, store);
                return applied;
            });
            if (!result.ok) {
                const status = result.error === 'discord_boost_already_claimed' ||
                    result.error === 'trial_boost_already_applied' ||
                    result.error === 'trial_boost_credit_pending' ||
                    result.error === 'trial_already_used'
                    ? 409
                    : 400;
                return void this.send(response, status, { ok: false, error: result.error });
            }
            this.send(response, 200, {
                ok: true,
                mode: result.mode,
                trial: result.lease ? this.publicTrial(result.lease) : null,
                boostCreditAvailable: result.mode === 'credit'
            });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/trials/activate') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.trialPath || !this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'trials_unavailable' });
            }
            const body = JSON.parse(await this.readBody(request, 1_024));
            const trialName = typeof body.name === 'string' ? body.name.trim() : '';
            if (trialName.length < 1 ||
                trialName.length > 64 ||
                /[\u0000-\u001F\u007F<>|]/.test(trialName)) {
                return void this.send(response, 400, { ok: false, error: 'trial_name_invalid' });
            }
            const result = await this.serializedTrial(async () => {
                const store = await loadTrials(this.config.trialPath);
                if (store.leases[steamId])
                    return { error: 'trial_already_used' };
                const registry = await loadRegistry(this.config.registryPath);
                const used = new Set(Object.values(store.leases).map((value) => value.serverId));
                const server = await this.findProvisionedTrialServer(registry.servers, used);
                if (!server)
                    return { error: 'trial_capacity_unavailable' };
                const startedAtMs = Date.now();
                const boostCredit = store.boostCredits?.[steamId];
                const boosted = Boolean(boostCredit);
                const durationMs = boosted ? TRIAL_BOOSTED_DURATION_MS : TRIAL_STANDARD_DURATION_MS;
                const lease = {
                    serverId: server.id,
                    steamId,
                    name: trialName,
                    startedAtMs,
                    expiresAtMs: startedAtMs + durationMs,
                    status: 'active',
                    discordBoostApplied: boosted || undefined,
                    discordUserId: boostCredit?.discordUserId
                };
                if (boostCredit) {
                    delete store.boostCredits[steamId];
                    if (store.boostCredits && Object.keys(store.boostCredits).length === 0) {
                        delete store.boostCredits;
                    }
                }
                try {
                    await this.runControl('start', server.id);
                }
                catch {
                    return { error: 'trial_start_failed' };
                }
                store.leases[steamId] = lease;
                try {
                    await saveTrials(this.config.trialPath, store);
                }
                catch (error) {
                    await this.runControl('stop', server.id).catch(() => undefined);
                    throw error;
                }
                return { server, lease };
            });
            if ('error' in result) {
                const status = result.error === 'trial_already_used'
                    ? 409
                    : result.error === 'trial_start_failed'
                        ? 503
                        : 503;
                return void this.send(response, status, { ok: false, error: result.error });
            }
            this.send(response, 201, {
                ok: true,
                server: await this.publicServer(result.server),
                trial: this.publicTrial(result.lease)
            });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/trials/name') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.trialPath) {
                return void this.send(response, 503, { ok: false, error: 'trials_unavailable' });
            }
            const body = JSON.parse(await this.readBody(request, 1_024));
            const trialName = typeof body.name === 'string' ? body.name.trim() : '';
            if (trialName.length < 1 ||
                trialName.length > 64 ||
                /[\u0000-\u001F\u007F<>|]/.test(trialName)) {
                return void this.send(response, 400, { ok: false, error: 'trial_name_invalid' });
            }
            const lease = await this.serializedTrial(async () => {
                const store = await loadTrials(this.config.trialPath);
                const current = store.leases[steamId];
                if (!current || current.status !== 'active')
                    return undefined;
                current.name = trialName;
                await saveTrials(this.config.trialPath, store);
                return current;
            });
            if (!lease) {
                return void this.send(response, 409, { ok: false, error: 'active_trial_not_found' });
            }
            this.send(response, 200, { ok: true, trial: this.publicTrial(lease) });
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/my-servers') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'customer_control_unavailable' });
            }
            const registry = await loadRegistry(this.config.registryPath);
            const ownership = await loadOwnership(this.config.ownershipPath);
            const ids = new Set(ownedServerIds(ownership, steamId));
            const trials = this.config.trialPath
                ? await loadTrials(this.config.trialPath)
                : { version: 1, leases: {} };
            const lease = trials.leases[steamId];
            if (lease)
                ids.add(lease.serverId);
            const servers = await Promise.all(registry.servers
                .filter((server) => ids.has(server.id))
                .map(async (server) => {
                const listed = await this.publicServer(server);
                const state = await this.readControlStatus(server.id);
                const serverTrial = trialForServer(trials, server.id);
                return {
                    ...listed,
                    name: serverTrial && serverTrial.status !== 'converted'
                        ? serverTrial.name
                        : listed.name,
                    control: state,
                    trial: serverTrial ? this.publicTrial(serverTrial) : null
                };
            }));
            this.send(response, 200, { ok: true, servers });
            return;
        }
        const accessMatch = /^\/v1\/my-servers\/((?:customer|trial|home)-world-[0-9]{1,3})\/access$/
            .exec(url.pathname);
        if (accessMatch && request.method === 'POST') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'customer_control_unavailable' });
            }
            const serverId = accessMatch[1];
            const ownership = await loadOwnership(this.config.ownershipPath);
            const trials = this.config.trialPath
                ? await loadTrials(this.config.trialPath)
                : { version: 1, leases: {} };
            const lease = trials.leases[steamId];
            const paidAccess = ownedServerIds(ownership, steamId).includes(serverId);
            const trialAccess = lease?.serverId === serverId;
            if (!paidAccess && !trialAccess) {
                return void this.send(response, 404, { ok: false, error: 'owned_server_not_found' });
            }
            if (trialAccess && lease.status === 'expired') {
                return void this.send(response, 403, {
                    ok: false, error: 'trial_expired', trial: this.publicTrial(lease)
                });
            }
            const body = JSON.parse(await this.readBody(request, 1_024));
            if (body.visibility !== 'public' && body.visibility !== 'private' ||
                body.password !== undefined && typeof body.password !== 'string') {
                return void this.send(response, 400, { ok: false, error: 'server_access_invalid' });
            }
            const password = typeof body.password === 'string' ? body.password.trim() : '';
            if (password.length > 128 || /[\u0000-\u001F\u007F]/.test(password)) {
                return void this.send(response, 400, { ok: false, error: 'password_invalid' });
            }
            if (body.visibility === 'private' && password.length > 0 && password.length < 8) {
                return void this.send(response, 400, { ok: false, error: 'password_too_short' });
            }
            let updated;
            try {
                updated = await this.serializedRegistry(async () => {
                    const registry = await loadRegistry(this.config.registryPath);
                    const server = registry.servers.find((candidate) => candidate.id === serverId);
                    if (!server)
                        return undefined;
                    if (body.visibility === 'private') {
                        if (password.length > 0)
                            server.passwordHash = hashPassword(password);
                        else if (!server.passwordHash)
                            return null;
                        server.visibility = 'private';
                    }
                    else {
                        server.visibility = 'public';
                        delete server.passwordHash;
                    }
                    await this.runAccessUpdate(serverId, server.visibility, server.passwordHash);
                    const persisted = await loadRegistry(this.config.registryPath);
                    const saved = persisted.servers.find((candidate) => candidate.id === serverId);
                    if (!saved || saved.visibility !== server.visibility) {
                        throw new Error('access_persist_failed');
                    }
                    if (server.visibility === 'private') {
                        const expectedHash = server.passwordHash;
                        if (!expectedHash || saved.passwordHash !== expectedHash) {
                            throw new Error('access_persist_failed');
                        }
                    }
                    else if (saved.passwordHash !== undefined) {
                        throw new Error('access_persist_failed');
                    }
                    return server;
                });
            }
            catch (error) {
                const message = error instanceof Error ? error.message : 'access_update_failed';
                if (message.includes('private_password_required') || message.includes('password_hash_invalid')) {
                    return void this.send(response, 400, {
                        ok: false, error: 'private_password_required'
                    });
                }
                if (message.includes('server_id_invalid') ||
                    message.includes('server_unit_missing') ||
                    message.includes('access_persist_failed')) {
                    return void this.send(response, 503, {
                        ok: false, error: 'customer_control_unavailable'
                    });
                }
                throw error;
            }
            if (updated === null) {
                return void this.send(response, 400, {
                    ok: false, error: 'private_password_required'
                });
            }
            if (!updated) {
                return void this.send(response, 404, { ok: false, error: 'server_not_found' });
            }
            this.send(response, 200, {
                ok: true,
                server: {
                    id: updated.id,
                    visibility: updated.visibility,
                    passwordRequired: Boolean(updated.passwordHash)
                }
            });
            return;
        }
        const iconUploadMatch = /^\/v1\/my-servers\/((?:customer|trial|home)-world-[0-9]{1,3})\/icon$/
            .exec(url.pathname);
        if (iconUploadMatch && (request.method === 'POST' || request.method === 'DELETE')) {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'customer_control_unavailable' });
            }
            if (!this.config.iconsPath) {
                return void this.send(response, 503, { ok: false, error: 'server_icons_unavailable' });
            }
            const serverId = iconUploadMatch[1];
            if (!(await this.canManageServer(steamId, serverId))) {
                return void this.send(response, 404, { ok: false, error: 'owned_server_not_found' });
            }
            if (request.method === 'DELETE') {
                await deleteServerIcon(this.config.iconsPath, serverId);
                await this.serializedRegistry(async () => {
                    const registry = await loadRegistry(this.config.registryPath);
                    const server = registry.servers.find((candidate) => candidate.id === serverId);
                    if (!server)
                        return;
                    delete server.iconUpdatedAtMs;
                    await saveRegistry(this.config.registryPath, registry);
                });
                this.send(response, 200, { ok: true, removed: true, serverId });
                return;
            }
            let payload;
            try {
                payload = JSON.parse(await this.readBody(request, 768 * 1024));
            }
            catch {
                return void this.send(response, 400, { ok: false, error: 'server_icon_invalid_payload' });
            }
            let upload;
            try {
                upload = parseServerIconUpload(payload);
            }
            catch (error) {
                const reason = error instanceof Error ? error.message : 'server_icon_invalid_payload';
                return void this.send(response, 400, { ok: false, error: reason });
            }
            const iconUpdatedAtMs = await saveServerIcon(this.config.iconsPath, serverId, upload.buffer, upload.contentType);
            await this.serializedRegistry(async () => {
                const registry = await loadRegistry(this.config.registryPath);
                const server = registry.servers.find((candidate) => candidate.id === serverId);
                if (!server)
                    throw new Error('server_not_found');
                server.iconUpdatedAtMs = iconUpdatedAtMs;
                await saveRegistry(this.config.registryPath, registry);
            });
            this.send(response, 200, {
                ok: true,
                serverId,
                iconUrl: buildServerIconUrl(this.config.publicBaseUrl, serverId, iconUpdatedAtMs),
                iconUpdatedAtMs
            });
            return;
        }
        const modsMatch = /^\/v1\/my-servers\/((?:customer|trial|home)-world-[0-9]{1,3})\/mods$/
            .exec(url.pathname);
        if (modsMatch && (request.method === 'GET' || request.method === 'POST')) {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.config.modRegistryPath ||
                !this.config.modAssignmentsPath ||
                !this.config.modRulesRoot) {
                return void this.send(response, 503, { ok: false, error: 'mods_unavailable' });
            }
            const serverId = modsMatch[1];
            const ownership = await loadOwnership(this.config.ownershipPath);
            const trials = this.config.trialPath
                ? await loadTrials(this.config.trialPath)
                : { version: 1, leases: {} };
            const lease = trials.leases[steamId];
            if (!ownedServerIds(ownership, steamId).includes(serverId) &&
                lease?.serverId !== serverId) {
                return void this.send(response, 404, { ok: false, error: 'owned_server_not_found' });
            }
            if (lease?.serverId === serverId && lease.status === 'expired') {
                return void this.send(response, 403, { ok: false, error: 'trial_expired' });
            }
            const servers = await loadRegistry(this.config.registryPath);
            const selected = servers.servers.find((server) => server.id === serverId);
            if (!selected) {
                return void this.send(response, 404, { ok: false, error: 'server_not_found' });
            }
            const registry = await loadModRegistry(this.config.modRegistryPath);
            const assignments = await loadModAssignments(this.config.modAssignmentsPath);
            const draftStore = this.config.modDraftPath
                ? await loadModDrafts(this.config.modDraftPath)
                : { version: 1, drafts: [] };
            const ownerDrafts = draftStore.drafts.filter((draft) => draft.ownerSteamId === steamId);
            if (request.method === 'GET') {
                this.send(response, 200, {
                    ok: true,
                    serverId,
                    enabled: assignments.servers[serverId] ?? [],
                    mods: registry.mods.map(({ publisherSteamId: _publisher, ...mod }) => mod),
                    drafts: ownerDrafts.map(({ ownerSteamId: _owner, ...draft }) => ({
                        ...draft, hidden: true
                    }))
                });
                return;
            }
            const body = JSON.parse(await this.readBody(request, 16_384));
            if (!Array.isArray(body.modKeys) ||
                body.modKeys.length > 16 ||
                body.modKeys.some((key) => typeof key !== 'string') ||
                new Set(body.modKeys).size !== body.modKeys.length) {
                return void this.send(response, 400, { ok: false, error: 'mod_selection_invalid' });
            }
            const keys = body.modKeys;
            const selectedMods = keys.map((key) => registry.mods.find((mod) => mod.key === key) ??
                ownerDrafts.find((draft) => draft.key === key));
            if (selectedMods.some((mod) => !mod)) {
                return void this.send(response, 400, { ok: false, error: 'mod_not_found' });
            }
            const rules = mergeRules(selectedMods);
            await this.serializedMod(async () => {
                const current = await loadModAssignments(this.config.modAssignmentsPath);
                current.servers[serverId] = keys;
                await saveServerModRules(this.config.modRulesRoot, serverId, selected.worldId, keys, rules);
                await saveModAssignments(this.config.modAssignmentsPath, current);
            });
            const state = await this.runControl('restart', serverId);
            this.send(response, 200, {
                ok: true,
                serverId,
                enabled: keys,
                rules,
                control: JSON.parse(state)
            });
            return;
        }
        const homeBootstrapMatch = /^\/v1\/my-servers\/(home-world-[0-9]{1,3})\/home-host-bootstrap$/
            .exec(url.pathname);
        if (homeBootstrapMatch && request.method === 'GET') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'customer_control_unavailable' });
            }
            const serverId = homeBootstrapMatch[1];
            const ownership = await loadOwnership(this.config.ownershipPath);
            if (!ownedServerIds(ownership, steamId).includes(serverId)) {
                return void this.send(response, 404, { ok: false, error: 'owned_server_not_found' });
            }
            try {
                const bootstrap = await this.readHomeHostBootstrap(serverId);
                this.send(response, 200, { ok: true, ...bootstrap });
            }
            catch (error) {
                this.send(response, 503, {
                    ok: false,
                    error: 'home_host_bootstrap_unavailable',
                    message: error instanceof Error ? error.message : 'bootstrap_failed'
                });
            }
            return;
        }
        const controlMatch = /^\/v1\/my-servers\/((?:customer|trial|home)-world-[0-9]{1,3})\/(start|stop|restart|logs)$/
            .exec(url.pathname);
        if (controlMatch && ((request.method === 'POST' && controlMatch[2] !== 'logs') ||
            (request.method === 'GET' && controlMatch[2] === 'logs'))) {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            if (!this.customerControlEnabled()) {
                return void this.send(response, 503, { ok: false, error: 'customer_control_unavailable' });
            }
            const serverId = controlMatch[1];
            const action = controlMatch[2];
            const ownership = await loadOwnership(this.config.ownershipPath);
            const trials = this.config.trialPath
                ? await loadTrials(this.config.trialPath)
                : { version: 1, leases: {} };
            const lease = trials.leases[steamId];
            const paidAccess = ownedServerIds(ownership, steamId).includes(serverId);
            const trialAccess = lease?.serverId === serverId;
            if (!paidAccess && !trialAccess) {
                return void this.send(response, 404, { ok: false, error: 'owned_server_not_found' });
            }
            if (trialAccess && lease.status === 'expired') {
                return void this.send(response, 403, {
                    ok: false,
                    error: 'trial_expired',
                    trial: this.publicTrial(lease)
                });
            }
            const registry = await loadRegistry(this.config.registryPath);
            if (!registry.servers.some((server) => server.id === serverId)) {
                return void this.send(response, 404, { ok: false, error: 'server_not_found' });
            }
            if (/^home-world-[0-9]{1,3}$/.test(serverId)) {
                if (action === 'logs') {
                    this.send(response, 200, {
                        ok: true,
                        serverId,
                        logs: 'Home Host logs are on your PC. Use Install Home Host from My Servers.',
                        control: await this.readHomeControlStatus(serverId),
                        generatedAtMs: Date.now()
                    });
                    return;
                }
                return void this.send(response, 409, {
                    ok: false,
                    error: 'home_host_local_control',
                    message: 'Home Host worlds are started on your PC with Install Home Host.'
                });
            }
            if (action === 'logs') {
                const lines = Math.min(Math.max(Number(url.searchParams.get('lines') ?? 200), 1), 500);
                const output = await this.runControl('logs', serverId, lines);
                const state = await this.runControl('status', serverId);
                this.send(response, 200, {
                    ok: true,
                    serverId,
                    logs: sanitizeLogs(output),
                    control: JSON.parse(state),
                    generatedAtMs: Date.now()
                });
                return;
            }
            const rateKey = `${steamId}:${serverId}`;
            const lastAction = this.controlActions.get(rateKey) ?? 0;
            if (Date.now() - lastAction < 10_000) {
                return void this.send(response, 429, { ok: false, error: 'control_rate_limited' });
            }
            this.controlActions.set(rateKey, Date.now());
            const output = await this.runControl(action, serverId);
            this.send(response, 202, {
                ok: true,
                serverId,
                action,
                control: JSON.parse(output)
            });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/join') {
            const body = JSON.parse(await this.readBody(request, 8_192));
            const registry = await loadRegistry(this.config.registryPath);
            const selected = registry.servers.find((server) => server.id === body.serverId);
            if (!selected)
                return void this.send(response, 404, { ok: false, error: 'server_not_found' });
            const authenticatedSteamId = this.publicAuthEnabled()
                ? this.verifyIdentity(this.bearer(request))
                : undefined;
            if (this.publicAuthEnabled() && !authenticatedSteamId) {
                return void this.send(response, 401, { ok: false, error: 'steam_auth_required' });
            }
            const playerId = authenticatedSteamId ??
                (typeof body.playerId === 'string' ? body.playerId.trim() : '');
            const displayName = typeof body.displayName === 'string' ? body.displayName.trim() : '';
            if (!/^steam_7656119[0-9]{10}$/.test(playerId) ||
                displayName.length < 1 ||
                displayName.length > 64 ||
                /[\r\n\t<>|]/.test(displayName)) {
                return void this.send(response, 400, { ok: false, error: 'identity_invalid' });
            }
            if (/^trial-world-[0-9]{1,3}$/.test(selected.id)) {
                if (!this.config.trialPath) {
                    return void this.send(response, 403, { ok: false, error: 'trial_unavailable' });
                }
                const trials = await loadTrials(this.config.trialPath);
                const lease = trialForServer(trials, selected.id);
                if (!lease) {
                    return void this.send(response, 404, { ok: false, error: 'server_not_found' });
                }
                if (lease.status === 'expired' || (lease.status === 'active' && lease.expiresAtMs <= Date.now())) {
                    void this.expireTrials();
                    return void this.send(response, 403, {
                        ok: false,
                        error: 'trial_expired',
                        trial: this.publicTrial({ ...lease, status: 'expired' })
                    });
                }
            }
            if (selected.passwordHash) {
                const isOwner = authenticatedSteamId &&
                    this.config.ownershipPath &&
                    ownedServerIds(await loadOwnership(this.config.ownershipPath), authenticatedSteamId).includes(selected.id);
                if (!isOwner) {
                    const failureKey = this.passwordRateLimitKey(request, authenticatedSteamId, selected.id);
                    const now = Date.now();
                    const failures = this.passwordFailures.get(failureKey);
                    if (failures && failures.resetAtMs > now && failures.count >= 5) {
                        return void this.send(response, 429, { ok: false, error: 'password_rate_limited' });
                    }
                    const suppliedPassword = typeof body.password === 'string' ? body.password.trim() : '';
                    if (!verifyPassword(suppliedPassword, selected.passwordHash)) {
                        this.passwordFailures.set(failureKey, {
                            count: failures && failures.resetAtMs > now ? failures.count + 1 : 1,
                            resetAtMs: now + 60_000
                        });
                        return void this.send(response, 403, { ok: false, error: 'password_invalid' });
                    }
                    this.passwordFailures.delete(failureKey);
                }
            }
            const ticket = issueJoinTicket({
                playerId,
                displayName,
                worldId: selected.worldId,
                expiresAtMs: Date.now() + 120_000
            }, selected.ticketSecret);
            if (/^home-world-[0-9]{1,3}$/.test(selected.id)) {
                let hostConnected = false;
                try {
                    const healthResponse = await fetch(this.internalHealthUrl(selected), { signal: AbortSignal.timeout(1_500) });
                    if (healthResponse.ok) {
                        const health = await healthResponse.json();
                        hostConnected = health.hostConnected === true;
                    }
                }
                catch {
                    hostConnected = false;
                }
                if (!hostConnected) {
                    return void this.send(response, 503, {
                        ok: false,
                        error: 'home_host_offline',
                        message: 'This Home Host world is not connected yet. ' +
                            'The server owner must open My Servers and click RUN SERVER or START & PLAY, ' +
                            'then wait until the world is online before anyone can join.'
                    });
                }
            }
            this.send(response, 200, {
                ok: true,
                server: {
                    id: selected.id,
                    name: selected.name,
                    gatewayHost: selected.gatewayHost,
                    gatewayPort: selected.gatewayPort,
                    gatewayEndpoints: selected.gatewayEndpoints,
                    profileUrl: selected.profileUrl,
                    worldId: selected.worldId
                },
                ticket
            });
            return;
        }
        this.send(response, 404, { ok: false, error: 'not_found' });
    }
    internalHealthUrl(server) {
        if (!server.healthUrl)
            return server.healthUrl;
        try {
            const url = new URL(server.healthUrl);
            if (url.hostname === 'sdo.harzgg.space' ||
                url.hostname === '134.195.89.248' ||
                url.hostname.endsWith('.harzgg.space')) {
                url.hostname = '127.0.0.1';
            }
            return url.toString();
        }
        catch {
            return server.healthUrl;
        }
    }
    async publicServer(server) {
        const isHomeHost = /^home-world-[0-9]{1,3}$/.test(server.id);
        let health = {};
        let online = false;
        try {
            const response = await fetch(this.internalHealthUrl(server), { signal: AbortSignal.timeout(1_500) });
            if (response.ok) {
                health = await response.json();
                online = health.ok === true;
            }
        }
        catch {
            online = false;
        }
        if (isHomeHost) {
            online = await this.isHomeWorldPublic(server.id);
        }
        return {
            id: server.id,
            name: server.name,
            description: server.description,
            visibility: server.visibility,
            passwordRequired: Boolean(server.passwordHash),
            pinned: Boolean(server.pinned),
            online,
            worldOnline: health.worldOnline === true,
            players: typeof health.playerCount === 'number' ? health.playerCount : 0,
            maxPlayers: server.maxPlayers ?? 32,
            iconUrl: buildServerIconUrl(this.config.publicBaseUrl, server.id, server.iconUpdatedAtMs)
        };
    }
    async canManageServer(steamId, serverId) {
        if (!this.config.ownershipPath)
            return false;
        const ownership = await loadOwnership(this.config.ownershipPath);
        const trials = this.config.trialPath
            ? await loadTrials(this.config.trialPath)
            : { version: 1, leases: {} };
        const lease = trials.leases[steamId];
        if (ownedServerIds(ownership, steamId).includes(serverId))
            return true;
        return lease?.serverId === serverId && lease.status !== 'expired';
    }
    publicAuthEnabled() {
        return Boolean(this.config.publicBaseUrl && this.config.sessionSecret);
    }
    customerControlEnabled() {
        return Boolean(this.publicAuthEnabled() &&
            this.config.ownershipPath &&
            (this.config.controlRunner ||
                this.config.controlHelperPath ||
                this.config.accessRunner));
    }
    isTrialSlotCandidate(serverId, used) {
        return /^trial-world-[0-9]{1,3}$/.test(serverId) && !used.has(serverId);
    }
    async trialSlotProvisioned(serverId) {
        if (!/^trial-world-[0-9]{1,3}$/.test(serverId))
            return false;
        if (this.config.controlRunner)
            return true;
        try {
            await access(`/etc/systemd/system/sdo-instance-${serverId}-gateway.service`);
            return true;
        }
        catch {
            return false;
        }
    }
    async findProvisionedTrialServer(servers, used) {
        for (const server of servers) {
            if (!this.isTrialSlotCandidate(server.id, used))
                continue;
            if (await this.trialSlotProvisioned(server.id))
                return server;
        }
        return undefined;
    }
    verifyDiscordBotSecret(request) {
        const configured = this.config.discordBotSecret?.trim();
        if (!configured)
            return false;
        const provided = request.headers['x-sdo-discord-bot-secret'];
        return typeof provided === 'string' && safeEqual(provided.trim(), configured);
    }
    requireIdentity(request, response) {
        const steamId = this.publicAuthEnabled()
            ? this.verifyIdentity(this.bearer(request))
            : undefined;
        if (!steamId) {
            this.send(response, 401, { ok: false, error: 'steam_auth_required' });
            return undefined;
        }
        return steamId;
    }
    async runControl(action, serverId, lines = 200) {
        if (this.config.controlRunner) {
            return this.config.controlRunner(action, serverId, lines);
        }
        const result = await executeFile('/usr/bin/sudo', ['-n', this.config.controlHelperPath, action, serverId, String(lines)], { timeout: action === 'logs' ? 10_000 : 45_000, maxBuffer: 256 * 1024 });
        return result.stdout.trim();
    }
    async readControlStatus(serverId) {
        if (/^home-world-[0-9]{1,3}$/.test(serverId)) {
            return this.readHomeControlStatus(serverId);
        }
        try {
            return JSON.parse(await this.runControl('status', serverId));
        }
        catch {
            return {
                ok: false,
                serverId,
                online: false,
                gateway: 'unknown',
                profile: 'unknown',
                host: 'unknown'
            };
        }
    }
    async readHomeHostBootstrap(serverId) {
        const root = this.config.homeInstancesRoot ?? '/etc/sdo-v3/instances';
        const envPath = join(root, serverId, 'gateway.env');
        const raw = await readFile(envPath, 'utf8');
        const env = parseDotEnv(raw);
        const worldId = env.SDO_WORLD_ID;
        const hostSecret = env.SDO_HOST_SECRET;
        const gatewayHost = env.SDO_PUBLIC_GATEWAY_HOST ?? 'sdo.harzgg.space';
        const gatewayPort = Number(env.SDO_PUBLIC_GATEWAY_PORT ?? env.SDO_GATEWAY_PORT);
        const slotMatch = /^home-world-(\d{1,3})$/.exec(serverId);
        const slot = slotMatch ? Number(slotMatch[1]) : 1;
        const hostStatusPort = 39_000 + slot * 10;
        if (!worldId || !hostSecret || !Number.isFinite(gatewayPort)) {
            throw new Error('home_host_bootstrap_invalid');
        }
        return {
            serverId,
            worldId,
            gatewayHost,
            gatewayPort,
            hostSecret,
            hostStatusPort,
            hostingMode: 'home-host'
        };
    }
    async readHomeControlStatus(serverId) {
        const online = await this.isHomeWorldPublic(serverId);
        return {
            ok: true,
            serverId,
            gateway: 'active',
            profile: 'active',
            host: online ? 'active' : 'inactive',
            online,
            hostingMode: 'home-host'
        };
    }
    async isHomeWorldPublic(serverId) {
        if (!this.config.homeHeartbeatsPath)
            return false;
        const store = await loadHomeHeartbeats(this.config.homeHeartbeatsPath);
        return isHomeHeartbeatFresh(store, serverId);
    }
    async serializedHomeHeartbeat(operation) {
        const previous = this.homeHeartbeatOperation;
        let release;
        this.homeHeartbeatOperation = new Promise((resolve) => { release = resolve; });
        await previous;
        try {
            return await operation();
        }
        finally {
            release();
        }
    }
    async runAccessUpdate(serverId, visibility, passwordHash) {
        if (this.config.accessRunner) {
            await this.config.accessRunner(serverId, visibility, passwordHash);
            return;
        }
        if (!this.config.controlHelperPath) {
            const registry = await loadRegistry(this.config.registryPath);
            const server = registry.servers.find((candidate) => candidate.id === serverId);
            if (!server)
                throw new Error('server_not_found');
            server.visibility = visibility;
            if (visibility === 'private' && passwordHash)
                server.passwordHash = passwordHash;
            else if (visibility === 'public')
                delete server.passwordHash;
            await saveRegistry(this.config.registryPath, registry);
            return;
        }
        await new Promise((resolve, reject) => {
            const child = spawn('/usr/bin/sudo', [
                '-n', this.config.controlHelperPath, 'access', serverId, visibility
            ], { stdio: ['pipe', 'pipe', 'pipe'] });
            let stdout = '';
            let stderr = '';
            const timer = setTimeout(() => {
                child.kill('SIGTERM');
                reject(new Error('access_update_timeout'));
            }, 10_000);
            child.stdout.on('data', (chunk) => {
                if (stdout.length < 16_384)
                    stdout += chunk.toString('utf8');
            });
            child.stderr.on('data', (chunk) => {
                if (stderr.length < 16_384)
                    stderr += chunk.toString('utf8');
            });
            child.once('error', (error) => {
                clearTimeout(timer);
                reject(error);
            });
            child.once('close', (code) => {
                clearTimeout(timer);
                if (code === 0)
                    resolve();
                else
                    reject(new Error(`access_update_failed_${code ?? 'signal'}:${sanitizeLogs(stderr || stdout)}`));
            });
            child.stdin.end(`${passwordHash ?? '-'}\n`);
        });
    }
    publicTrial(lease) {
        const boosted = Boolean(lease.discordBoostApplied);
        return {
            serverId: lease.serverId,
            name: lease.name,
            status: lease.status,
            startedAtMs: lease.startedAtMs,
            expiresAtMs: lease.expiresAtMs,
            boosted,
            durationHours: boosted ? 96 : 48,
            remainingSeconds: lease.status === 'active'
                ? Math.max(0, Math.ceil((lease.expiresAtMs - Date.now()) / 1_000))
                : 0
        };
    }
    async serializedTrial(operation) {
        const previous = this.trialOperation;
        let release;
        this.trialOperation = new Promise((resolve) => { release = resolve; });
        await previous;
        try {
            return await operation();
        }
        finally {
            release();
        }
    }
    async serializedMod(operation) {
        const previous = this.modOperation;
        let release;
        this.modOperation = new Promise((resolve) => { release = resolve; });
        await previous;
        try {
            return await operation();
        }
        finally {
            release();
        }
    }
    async serializedRegistry(operation) {
        const previous = this.registryOperation;
        let release;
        this.registryOperation = new Promise((resolve) => { release = resolve; });
        await previous;
        try {
            return await operation();
        }
        finally {
            release();
        }
    }
    async expireTrials() {
        if (!this.config.trialPath)
            return;
        await this.serializedTrial(async () => {
            const store = await loadTrials(this.config.trialPath);
            const expired = [];
            for (const lease of Object.values(store.leases)) {
                if (lease.status === 'active' && lease.expiresAtMs <= Date.now()) {
                    lease.status = 'expired';
                    expired.push(lease);
                }
            }
            if (expired.length === 0)
                return;
            await saveTrials(this.config.trialPath, store);
            for (const lease of expired) {
                try {
                    await this.runControl('stop', lease.serverId);
                }
                catch (error) {
                    console.error(JSON.stringify({
                        timestamp: new Date().toISOString(),
                        level: 'error',
                        event: 'trial_expiry_stop_error',
                        protocolVersion: 3,
                        playerId: lease.steamId,
                        worldId: lease.serverId,
                        error: error instanceof Error ? error.message : 'unknown_error'
                    }));
                }
            }
        });
    }
    publicBase() {
        return this.config.publicBaseUrl.replace(/\/+$/, '');
    }
    resolveAuthPublicBase(requested, inferred) {
        const configured = this.publicBase();
        const allowed = new Set([configured]);
        for (const mirror of this.config.authMirrorBaseUrls ?? []) {
            const normalized = mirror.replace(/\/+$/, '');
            if (normalized)
                allowed.add(normalized);
        }
        for (const candidate of [requested, inferred]) {
            if (!candidate)
                continue;
            const normalized = candidate.replace(/\/+$/, '');
            if (allowed.has(normalized))
                return normalized;
        }
        return configured;
    }
    inferAuthPublicBaseFromRequest(request) {
        const publicHost = request.headers['x-sdo-auth-public-host'];
        if (typeof publicHost !== 'string' || !publicHost.trim())
            return undefined;
        const host = publicHost.split(',')[0].trim().toLowerCase();
        for (const mirror of this.config.authMirrorBaseUrls ?? []) {
            try {
                if (new URL(mirror).host.toLowerCase() === host) {
                    return mirror.replace(/\/+$/, '');
                }
            }
            catch {
                continue;
            }
        }
        return undefined;
    }
    clientAddress(request) {
        const remote = request.socket.remoteAddress ?? 'unknown';
        const isLoopback = remote === '127.0.0.1' ||
            remote === '::1' ||
            remote === '::ffff:127.0.0.1';
        if (!isLoopback)
            return remote;
        const realIp = request.headers['x-real-ip'];
        if (typeof realIp === 'string' && realIp.trim())
            return realIp.trim();
        const forwarded = request.headers['x-forwarded-for'];
        if (typeof forwarded === 'string' && forwarded.trim()) {
            return forwarded.split(',')[0].trim();
        }
        return remote;
    }
    passwordRateLimitKey(request, steamId, serverId) {
        if (steamId)
            return `steam:${steamId}:${serverId}`;
        return `${this.clientAddress(request)}:${serverId}`;
    }
    validBrowserDevice(url) {
        const id = url.searchParams.get('device') ?? '';
        const nonce = url.searchParams.get('nonce') ?? '';
        const device = this.steamDevices.get(id);
        return device && device.expiresAtMs > Date.now() && safeEqual(device.browserNonce, nonce)
            ? device
            : undefined;
    }
    signIdentity(steamId, expiresAtMs) {
        return this.signAuthToken(steamId, 'identity', expiresAtMs);
    }
    signAuthToken(steamId, purpose, expiresAtMs) {
        const body = Buffer.from(JSON.stringify({
            version: 1,
            purpose,
            steamId,
            expiresAtMs
        })).toString('base64url');
        const signature = createHmac('sha256', this.config.sessionSecret).update(body).digest('base64url');
        return `${body}.${signature}`;
    }
    verifyIdentity(token) {
        return this.verifyAuthToken(token, 'identity');
    }
    verifyAuthToken(token, expectedPurpose) {
        const [body, supplied, extra] = token.split('.');
        if (!body || !supplied || extra)
            return undefined;
        const expected = createHmac('sha256', this.config.sessionSecret).update(body).digest('base64url');
        if (!safeEqual(expected, supplied))
            return undefined;
        try {
            const value = JSON.parse(Buffer.from(body, 'base64url').toString('utf8'));
            return value.version === 1 &&
                value.purpose === expectedPurpose &&
                typeof value.steamId === 'string' &&
                /^steam_7656119[0-9]{10}$/.test(value.steamId) &&
                typeof value.expiresAtMs === 'number' &&
                value.expiresAtMs > Date.now()
                ? value.steamId
                : undefined;
        }
        catch {
            return undefined;
        }
    }
    bearer(request) {
        const authorization = request.headers.authorization ?? '';
        return authorization.startsWith('Bearer ') ? authorization.slice(7) : '';
    }
    pruneDevices() {
        const now = Date.now();
        for (const [id, device] of this.steamDevices) {
            if (device.expiresAtMs <= now)
                this.steamDevices.delete(id);
        }
    }
    async readBody(request, limit) {
        const chunks = [];
        let total = 0;
        for await (const chunk of request) {
            const bytes = Buffer.isBuffer(chunk) ? chunk : Buffer.from(chunk);
            total += bytes.length;
            if (total > limit)
                throw new Error('body_too_large');
            chunks.push(bytes);
        }
        return Buffer.concat(chunks).toString('utf8');
    }
    validateModCode(value, depth = 0) {
        if (depth > 12)
            throw new Error('mod_code_too_deep');
        if (value === null || typeof value === 'boolean' || typeof value === 'number')
            return;
        if (typeof value === 'string') {
            if (value.length > 4_096 || /(?:https?:\/\/|file:|require\s*\(|process\.|powershell|cmd\.exe)/i.test(value)) {
                throw new Error('mod_code_unsafe');
            }
            return;
        }
        if (Array.isArray(value)) {
            if (value.length > 512)
                throw new Error('mod_code_too_large');
            for (const entry of value)
                this.validateModCode(entry, depth + 1);
            return;
        }
        if (!value || typeof value !== 'object')
            throw new Error('mod_code_invalid');
        const entries = Object.entries(value);
        if (entries.length > 256)
            throw new Error('mod_code_too_large');
        for (const [key, entry] of entries) {
            if (!/^[a-zA-Z][a-zA-Z0-9_-]{0,63}$/.test(key) ||
                /^(?:execute|command|process|network|filesystem|native|dll|exe|shell|hook)$/i.test(key))
                throw new Error('mod_code_unsafe');
            this.validateModCode(entry, depth + 1);
        }
    }
    send(response, status, value) {
        if (response.headersSent)
            return;
        const body = JSON.stringify(value);
        response.writeHead(status, {
            'content-type': 'application/json',
            'content-length': Buffer.byteLength(body),
            'cache-control': 'no-store',
            'x-content-type-options': 'nosniff'
        });
        response.end(body);
    }
    sendBinary(response, status, body, contentType, headers = {}) {
        if (response.headersSent)
            return;
        response.writeHead(status, {
            'content-type': contentType,
            'content-length': body.length,
            'x-content-type-options': 'nosniff',
            ...headers
        });
        response.end(body);
    }
    sendHtml(response, status, message) {
        const body = `<!doctype html><meta charset="utf-8"><title>SD-Online</title><body style="background:#0e0f34;color:white;font:20px Segoe UI;padding:48px"><h1>SD-Online</h1><p>${message}</p></body>`;
        response.writeHead(status, {
            'content-type': 'text/html; charset=utf-8',
            'content-length': Buffer.byteLength(body),
            'cache-control': 'no-store',
            'x-content-type-options': 'nosniff'
        });
        response.end(body);
    }
}
function safeEqual(left, right) {
    const leftBytes = Buffer.from(left);
    const rightBytes = Buffer.from(right);
    return leftBytes.length === rightBytes.length && timingSafeEqual(leftBytes, rightBytes);
}
function sanitizeLogs(value) {
    return value
        .replace(/\x1b\[[0-9;]*[A-Za-z]/g, '')
        .replace(/[\u0000-\u0008\u000B\u000C\u000E-\u001F\u007F]/g, '')
        .replace(/\b(authorization|password|secret|ticket|token)(\s*[=:]\s*)[^\s,;]+/gi, '$1$2[redacted]')
        .slice(-128 * 1024);
}
//# sourceMappingURL=service.js.map