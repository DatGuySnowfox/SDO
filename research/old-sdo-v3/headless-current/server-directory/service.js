import { createHash, createHmac, randomBytes, randomUUID, timingSafeEqual } from 'node:crypto';
import { execFile } from 'node:child_process';
import { createServer } from 'node:http';
import { promisify } from 'node:util';
import { issueJoinTicket } from "../shared-protocol/index.js";
import { loadRegistry, verifyPassword } from "./store.js";
import { loadOwnership, ownedServerIds } from "./ownership.js";
import { loadTrials, saveTrials, trialForServer } from "./trials.js";
import { loadModAssignments, loadModRegistry, mergeRules, registerMod, saveModAssignments, saveModRegistry, saveServerModRules } from "./mods.js";
const executeFile = promisify(execFile);
export class ServerDirectoryService {
    config;
    passwordFailures = new Map();
    steamDevices = new Map();
    controlActions = new Map();
    server;
    trialTimer;
    trialOperation = Promise.resolve();
    modOperation = Promise.resolve();
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
            const deviceId = randomUUID();
            const pollToken = randomBytes(32).toString('base64url');
            const browserNonce = randomBytes(24).toString('base64url');
            const expiresAtMs = Date.now() + 5 * 60_000;
            this.steamDevices.set(deviceId, {
                pollHash: createHash('sha256').update(pollToken).digest('hex'),
                browserNonce,
                expiresAtMs
            });
            this.pruneDevices();
            this.send(response, 201, {
                ok: true,
                deviceId,
                pollToken,
                expiresAtMs,
                loginUrl: `${this.publicBase()}/v1/auth/steam/start?device=${encodeURIComponent(deviceId)}&nonce=${browserNonce}`
            });
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/auth/steam/start') {
            const device = this.validBrowserDevice(url);
            if (!device)
                return void this.send(response, 400, { ok: false, error: 'auth_device_invalid' });
            const returnTo = `${this.publicBase()}/v1/auth/steam/callback?device=${encodeURIComponent(url.searchParams.get('device'))}&nonce=${encodeURIComponent(device.browserNonce)}`;
            const parameters = new URLSearchParams({
                'openid.ns': 'http://specs.openid.net/auth/2.0',
                'openid.mode': 'checkid_setup',
                'openid.return_to': returnTo,
                'openid.realm': `${new URL(this.publicBase()).origin}/`,
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
            const servers = await Promise.all(registry.servers
                .filter((server) => {
                if (!/^trial-world-[0-9]{1,3}$/.test(server.id))
                    return true;
                const lease = trialForServer(trials, server.id);
                return lease?.status === 'active' || lease?.status === 'converted';
            })
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
        if (request.method === 'GET' && url.pathname === '/v1/mods') {
            if (!this.config.modRegistryPath) {
                return void this.send(response, 503, { ok: false, error: 'mod_registry_unavailable' });
            }
            const registry = await loadModRegistry(this.config.modRegistryPath);
            const mods = registry.mods.map(({ publisherSteamId: _publisher, ...mod }) => mod);
            this.send(response, 200, { ok: true, mods });
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
            const capacityAvailable = registry.servers.some((server) => /^trial-world-[0-9]{1,3}$/.test(server.id) && !used.has(server.id));
            this.send(response, 200, {
                ok: true,
                eligible: !lease,
                capacityAvailable,
                trial: lease ? this.publicTrial(lease) : null
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
                const server = registry.servers.find((value) => /^trial-world-[0-9]{1,3}$/.test(value.id) && !used.has(value.id));
                if (!server)
                    return { error: 'trial_capacity_unavailable' };
                const startedAtMs = Date.now();
                const lease = {
                    serverId: server.id,
                    steamId,
                    name: trialName,
                    startedAtMs,
                    expiresAtMs: startedAtMs + 48 * 60 * 60 * 1_000,
                    status: 'active'
                };
                await this.runControl('start', server.id);
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
                const status = result.error === 'trial_already_used' ? 409 : 503;
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
                const state = await this.runControl('status', server.id);
                const serverTrial = trialForServer(trials, server.id);
                return {
                    ...listed,
                    name: serverTrial && serverTrial.status !== 'converted'
                        ? serverTrial.name
                        : listed.name,
                    control: JSON.parse(state),
                    trial: serverTrial ? this.publicTrial(serverTrial) : null
                };
            }));
            this.send(response, 200, { ok: true, servers });
            return;
        }
        const modsMatch = /^\/v1\/my-servers\/((?:customer|trial)-world-[0-9]{1,3})\/mods$/
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
            if (request.method === 'GET') {
                this.send(response, 200, {
                    ok: true,
                    serverId,
                    enabled: assignments.servers[serverId] ?? [],
                    mods: registry.mods.map(({ publisherSteamId: _publisher, ...mod }) => mod)
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
            const selectedMods = keys.map((key) => registry.mods.find((mod) => mod.key === key));
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
        const controlMatch = /^\/v1\/my-servers\/((?:customer|trial)-world-[0-9]{1,3})\/(start|stop|restart|logs)$/
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
                const failureKey = `${request.socket.remoteAddress ?? 'unknown'}:${selected.id}`;
                const now = Date.now();
                const failures = this.passwordFailures.get(failureKey);
                if (failures && failures.resetAtMs > now && failures.count >= 5) {
                    return void this.send(response, 429, { ok: false, error: 'password_rate_limited' });
                }
                if (!verifyPassword(typeof body.password === 'string' ? body.password : '', selected.passwordHash)) {
                    this.passwordFailures.set(failureKey, {
                        count: failures && failures.resetAtMs > now ? failures.count + 1 : 1,
                        resetAtMs: now + 60_000
                    });
                    return void this.send(response, 403, { ok: false, error: 'password_invalid' });
                }
                this.passwordFailures.delete(failureKey);
            }
            const ticket = issueJoinTicket({
                playerId,
                displayName,
                worldId: selected.worldId,
                expiresAtMs: Date.now() + 120_000
            }, selected.ticketSecret);
            this.send(response, 200, {
                ok: true,
                server: {
                    id: selected.id,
                    name: selected.name,
                    gatewayHost: selected.gatewayHost,
                    gatewayPort: selected.gatewayPort,
                    profileUrl: selected.profileUrl,
                    worldId: selected.worldId
                },
                ticket
            });
            return;
        }
        this.send(response, 404, { ok: false, error: 'not_found' });
    }
    async publicServer(server) {
        let health = {};
        let online = false;
        try {
            const response = await fetch(server.healthUrl, { signal: AbortSignal.timeout(1_500) });
            if (response.ok) {
                health = await response.json();
                online = health.ok === true;
            }
        }
        catch {
            online = false;
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
            maxPlayers: server.maxPlayers ?? 32
        };
    }
    publicAuthEnabled() {
        return Boolean(this.config.publicBaseUrl && this.config.sessionSecret);
    }
    customerControlEnabled() {
        return Boolean(this.publicAuthEnabled() &&
            this.config.ownershipPath &&
            (this.config.controlRunner || this.config.controlHelperPath));
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
    publicTrial(lease) {
        return {
            serverId: lease.serverId,
            name: lease.name,
            status: lease.status,
            startedAtMs: lease.startedAtMs,
            expiresAtMs: lease.expiresAtMs,
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