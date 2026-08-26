import { createHash, createHmac, randomBytes, randomUUID, timingSafeEqual } from 'node:crypto';
import { createServer } from 'node:http';
import { issueJoinTicket } from "../shared-protocol/index.js";
import { loadRegistry, verifyPassword } from "./store.js";
export class ServerDirectoryService {
    config;
    passwordFailures = new Map();
    steamDevices = new Map();
    server;
    constructor(config) {
        this.config = config;
    }
    async start() {
        await loadRegistry(this.config.registryPath);
        this.server = createServer((request, response) => {
            void this.handle(request, response).catch(() => this.send(response, 500, { ok: false, error: 'directory_error' }));
        });
        await new Promise((resolve, reject) => {
            this.server.once('error', reject);
            this.server.listen(this.config.port ?? 31100, this.config.host ?? '127.0.0.1', resolve);
        });
        return { port: this.server.address().port };
    }
    async stop() {
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
            const servers = await Promise.all(registry.servers.map((server) => this.publicServer(server)));
            servers.sort((left, right) => Number(right.pinned) - Number(left.pinned) || left.name.localeCompare(right.name));
            this.send(response, 200, { ok: true, servers });
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
//# sourceMappingURL=service.js.map