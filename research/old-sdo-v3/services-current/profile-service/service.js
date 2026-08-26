import { createHmac, timingSafeEqual } from 'node:crypto';
import { readFile } from 'node:fs/promises';
import { createServer } from 'node:http';
import { validateJoinTicket } from "../shared-protocol/index.js";
import { validSteamIdentity, validateProfileBundle, isCompleteCanonicalServerBundle } from "./bundle.js";
import { PlayerProfileStore, ProfileConflictError, ProfileRecoveryError, ProfileRegressionError } from "./store.js";
export class ProfileService {
    config;
    store;
    starterBundle;
    server;
    constructor(config) {
        this.config = config;
        if (config.ticketSecret.length < 16 || config.sessionSecret.length < 16) {
            throw new Error('Profile service secrets must be at least 16 characters');
        }
        this.store = new PlayerProfileStore(config.root, config.keepBackups ?? 5);
    }
    async start() {
        if (this.config.starterBundlePath) {
            const raw = await readFile(this.config.starterBundlePath, 'utf8');
            this.starterBundle = validateProfileBundle(JSON.parse(raw));
        }
        this.server = createServer((request, response) => {
            void this.handle(request, response).catch((error) => {
                this.sendJson(response, 500, {
                    ok: false,
                    error: error instanceof Error ? error.message : 'profile_service_error'
                });
            });
        });
        await new Promise((resolve, reject) => {
            this.server.once('error', reject);
            this.server.listen(this.config.port ?? 0, this.config.host ?? '127.0.0.1', resolve);
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
        const url = new URL(request.url ?? '/', 'http://profile.local');
        const isLoopback = ['127.0.0.1', '::1', '::ffff:127.0.0.1'].includes(request.socket.remoteAddress ?? '');
        if (request.method === 'POST' && url.pathname === '/shutdown' && isLoopback) {
            response.writeHead(202).end();
            setImmediate(() => void this.stop());
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/admin/bootstrap' && isLoopback) {
            let input;
            try {
                input = JSON.parse(await this.readBody(request, 1024 * 1024));
            }
            catch {
                this.sendJson(response, 400, { ok: false, error: 'invalid_bootstrap_body' });
                return;
            }
            if (typeof input !== 'object' || input === null || !('steamId' in input) || !('profile' in input)) {
                this.sendJson(response, 400, { ok: false, error: 'invalid_bootstrap_params' });
                return;
            }
            const steamId = String(input.steamId);
            const profile = input.profile;
            if (!validSteamIdentity(steamId) || typeof profile !== 'object' || profile === null) {
                this.sendJson(response, 400, { ok: false, error: 'invalid_bootstrap_params' });
                return;
            }
            try {
                const bundle = validateProfileBundle({
                    version: 1,
                    steamId,
                    createdAtMs: Date.now(),
                    files: []
                }, steamId);
                const record = await this.store.save(steamId, 0, `admin-bootstrap:${steamId}:${Date.now()}`, bundle);
                this.sendJson(response, 201, {
                    ok: true,
                    revision: record.revision,
                    sha256: record.sha256,
                    savedAtMs: record.savedAtMs
                });
            }
            catch (error) {
                if (error instanceof ProfileConflictError) {
                    const existing = await this.store.load(steamId);
                    if (existing) {
                        this.sendJson(response, 200, {
                            ok: true,
                            revision: existing.revision,
                            sha256: existing.sha256,
                            savedAtMs: existing.savedAtMs,
                            alreadyExists: true
                        });
                        return;
                    }
                }
                throw error;
            }
            return;
        }
        if (request.method === 'POST' && url.pathname === '/admin/bootstrap' && isLoopback) {
            // Alternate path for backwards compatibility
            let input;
            try {
                input = JSON.parse(await this.readBody(request, 1024 * 1024));
            }
            catch {
                this.sendJson(response, 400, { ok: false, error: 'invalid_bootstrap_body' });
                return;
            }
            if (typeof input !== 'object' || input === null || !('steamId' in input) || !('profile' in input)) {
                this.sendJson(response, 400, { ok: false, error: 'invalid_bootstrap_params' });
                return;
            }
            const steamId = String(input.steamId);
            const profile = input.profile;
            if (!validSteamIdentity(steamId) || typeof profile !== 'object' || profile === null) {
                this.sendJson(response, 400, { ok: false, error: 'invalid_bootstrap_params' });
                return;
            }
            try {
                const bundle = validateProfileBundle({
                    version: 1,
                    steamId,
                    createdAtMs: Date.now(),
                    files: []
                }, steamId);
                const record = await this.store.save(steamId, 0, `admin-bootstrap:${steamId}:${Date.now()}`, bundle);
                this.sendJson(response, 201, {
                    ok: true,
                    revision: record.revision,
                    sha256: record.sha256,
                    savedAtMs: record.savedAtMs
                });
            }
            catch (error) {
                if (error instanceof ProfileConflictError) {
                    const existing = await this.store.load(steamId);
                    if (existing) {
                        this.sendJson(response, 200, {
                            ok: true,
                            revision: existing.revision,
                            sha256: existing.sha256,
                            savedAtMs: existing.savedAtMs,
                            alreadyExists: true
                        });
                        return;
                    }
                }
                throw error;
            }
            return;
        }
        if (request.method === 'GET' && url.pathname === '/health') {
            this.sendJson(response, 200, { ok: true, service: 'sdo-profile-service', version: 1 });
            return;
        }
        if (request.method === 'POST' && url.pathname === '/v1/sessions') {
            const ticket = this.bearer(request);
            let claims;
            try {
                claims = validateJoinTicket(ticket, this.config.ticketSecret);
            }
            catch {
                this.sendJson(response, 401, { ok: false, error: 'invalid_join_ticket' });
                return;
            }
            if (!validSteamIdentity(claims.playerId)) {
                this.sendJson(response, 400, { ok: false, error: 'steam_identity_required' });
                return;
            }
            const session = {
                steamId: claims.playerId,
                expiresAtMs: Date.now() + Math.max(60_000, this.config.sessionTtlMs ?? 24 * 60 * 60_000)
            };
            this.sendJson(response, 201, {
                ok: true,
                steamId: session.steamId,
                expiresAtMs: session.expiresAtMs,
                token: this.signSession(session)
            });
            return;
        }
        const match = /^\/v1\/profiles\/(steam_7656119[0-9]{10})(\/recover)?$/.exec(url.pathname);
        if (!match) {
            this.sendJson(response, 404, { ok: false, error: 'not_found' });
            return;
        }
        const steamId = match[1];
        const session = this.verifySession(this.bearer(request));
        if (!session || session.steamId !== steamId) {
            this.sendJson(response, 403, { ok: false, error: 'profile_identity_mismatch' });
            return;
        }
        const profileAction = match[2] ?? '';
        if (request.method === 'GET' && profileAction === '') {
            const record = await this.loadOrCreateProfile(steamId);
            if (!record) {
                this.sendJson(response, 404, { ok: false, error: 'profile_not_found' });
                return;
            }
            this.sendJson(response, 200, {
                ok: true,
                revision: record.revision,
                sha256: record.sha256,
                savedAtMs: record.savedAtMs,
                bundle: record.bundle
            });
            return;
        }
        if (request.method === 'PUT' && profileAction === '') {
            const expectedRevision = Number(request.headers['x-profile-base-revision'] ?? -1);
            const idempotencyKey = String(request.headers['x-idempotency-key'] ?? '');
            if (!Number.isSafeInteger(expectedRevision) ||
                expectedRevision < 0 ||
                !/^[A-Za-z0-9._:-]{8,128}$/.test(idempotencyKey)) {
                this.sendJson(response, 400, { ok: false, error: 'invalid_profile_headers' });
                return;
            }
            let input;
            try {
                input = JSON.parse(await this.readBody(request, 48 * 1024 * 1024));
            }
            catch {
                this.sendJson(response, 400, { ok: false, error: 'invalid_profile_body' });
                return;
            }
            try {
                const record = await this.store.save(steamId, expectedRevision, idempotencyKey, input);
                this.sendJson(response, 200, {
                    ok: true,
                    revision: record.revision,
                    sha256: record.sha256,
                    savedAtMs: record.savedAtMs
                });
            }
            catch (error) {
                if (error instanceof ProfileConflictError) {
                    this.sendJson(response, 409, {
                        ok: false,
                        error: error.message,
                        currentRevision: error.revision
                    });
                    return;
                }
                if (error instanceof ProfileRegressionError) {
                    this.sendJson(response, 422, {
                        ok: false,
                        error: error.message
                    });
                    return;
                }
                throw error;
            }
            return;
        }
        if (request.method === 'POST' && profileAction === '/recover') {
            let input;
            try {
                input = JSON.parse(await this.readBody(request, 4096));
            }
            catch {
                this.sendJson(response, 400, { ok: false, error: 'invalid_recovery_body' });
                return;
            }
            const expectedRevision = Number(input?.expectedRevision ?? -1);
            if (!Number.isSafeInteger(expectedRevision) || expectedRevision < 1) {
                this.sendJson(response, 400, { ok: false, error: 'invalid_recovery_revision' });
                return;
            }
            try {
                const recovered = await this.store.recoverPrevious(steamId, expectedRevision);
                this.sendJson(response, 200, {
                    ok: true,
                    revision: recovered.record.revision,
                    sha256: recovered.record.sha256,
                    savedAtMs: recovered.record.savedAtMs,
                    recoveredFromRevision: recovered.recoveredFromRevision
                });
            }
            catch (error) {
                if (error instanceof ProfileConflictError) {
                    this.sendJson(response, 409, {
                        ok: false,
                        error: error.message,
                        currentRevision: error.revision
                    });
                    return;
                }
                if (error instanceof ProfileRecoveryError) {
                    if (error.message === 'profile_recovery_unavailable' &&
                        this.starterBundle) {
                        const current = await this.store.load(steamId);
                        if (current &&
                            !isCompleteCanonicalServerBundle(current.bundle)) {
                            const bundle = validateProfileBundle({
                                ...this.starterBundle,
                                steamId,
                                createdAtMs: Date.now()
                            }, steamId);
                            const reset = await this.store.save(steamId, current.revision, `starter-recovery:${Date.now()}`, bundle);
                            this.sendJson(response, 200, {
                                ok: true,
                                revision: reset.revision,
                                sha256: reset.sha256,
                                savedAtMs: reset.savedAtMs,
                                recoveredFromRevision: 0
                            });
                            return;
                        }
                    }
                    this.sendJson(response, 409, { ok: false, error: error.message });
                    return;
                }
                throw error;
            }
            return;
        }
        this.sendJson(response, 405, { ok: false, error: 'method_not_allowed' });
    }
    async loadOrCreateProfile(steamId) {
        const existing = await this.store.load(steamId);
        if (existing || !this.starterBundle)
            return existing;
        const bundle = validateProfileBundle({
            ...this.starterBundle,
            steamId,
            createdAtMs: Date.now()
        }, steamId);
        try {
            return await this.store.save(steamId, 0, `server-starter-v1:${steamId}`, bundle);
        }
        catch (error) {
            if (error instanceof ProfileConflictError) {
                return this.store.load(steamId);
            }
            throw error;
        }
    }
    signSession(session) {
        const body = Buffer.from(JSON.stringify(session)).toString('base64url');
        const signature = createHmac('sha256', this.config.sessionSecret)
            .update(body)
            .digest('base64url');
        return `${body}.${signature}`;
    }
    verifySession(token) {
        const [body, suppliedText, extra] = token.split('.');
        if (!body || !suppliedText || extra)
            return undefined;
        const expected = createHmac('sha256', this.config.sessionSecret).update(body).digest();
        const supplied = Buffer.from(suppliedText, 'base64url');
        if (supplied.length !== expected.length || !timingSafeEqual(supplied, expected))
            return undefined;
        try {
            const value = JSON.parse(Buffer.from(body, 'base64url').toString('utf8'));
            return validSteamIdentity(value.steamId) &&
                Number.isSafeInteger(value.expiresAtMs) &&
                value.expiresAtMs > Date.now()
                ? value
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
    async readBody(request, limit) {
        const chunks = [];
        let size = 0;
        for await (const chunk of request) {
            const bytes = Buffer.isBuffer(chunk) ? chunk : Buffer.from(chunk);
            size += bytes.length;
            if (size > limit)
                throw new Error('request_too_large');
            chunks.push(bytes);
        }
        return Buffer.concat(chunks).toString('utf8');
    }
    sendJson(response, status, value) {
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
}
//# sourceMappingURL=service.js.map