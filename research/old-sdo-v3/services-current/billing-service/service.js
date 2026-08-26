import { createHmac, timingSafeEqual } from 'node:crypto';
import { createServer } from 'node:http';
import { loadBillingStore, saveBillingStore } from "./store.js";
const STEAM = /^steam_7656119[0-9]{10}$/;
const SERVER = /^(?:customer|trial)-world-[0-9]{1,3}$/;
export class BillingService {
    config;
    server;
    operation = Promise.resolve();
    now;
    constructor(config) {
        this.config = config;
        if (config.sessionSecret.length < 32)
            throw new Error('billing_session_secret_too_short');
        if (config.plans.length < 1 || config.plans.length > 8)
            throw new Error('billing_plans_invalid');
        if (new Set(config.plans.map((plan) => plan.id)).size !== config.plans.length) {
            throw new Error('billing_plans_invalid');
        }
        if (config.pool.length < 1 || config.pool.some((id) => !SERVER.test(id))) {
            throw new Error('billing_pool_invalid');
        }
        this.now = config.now ?? Date.now;
    }
    async start() {
        await loadBillingStore(this.config.storePath);
        this.server = createServer((request, response) => {
            void this.handle(request, response).catch((error) => {
                console.error(JSON.stringify({
                    timestamp: new Date().toISOString(), level: 'error',
                    event: 'billing_request_error', protocolVersion: 3,
                    path: (request.url ?? '').split('?')[0],
                    error: error instanceof Error ? error.message : 'unknown_error'
                }));
                if (!response.headersSent)
                    this.json(response, 500, { ok: false, error: 'billing_error' });
            });
        });
        await new Promise((resolve, reject) => {
            this.server.once('error', reject);
            this.server.listen(this.config.port ?? 31200, this.config.host ?? '127.0.0.1', resolve);
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
        const url = new URL(request.url ?? '/', 'http://billing.local');
        if (request.method === 'GET' && url.pathname === '/health') {
            return void this.json(response, 200, { ok: true, service: 'sdo-billing', version: 1 });
        }
        if (request.method === 'GET' && url.pathname === '/complete') {
            const canceled = url.searchParams.get('checkout') === 'canceled';
            response.writeHead(200, {
                'content-type': 'text/html; charset=utf-8',
                'cache-control': 'no-store',
                'content-security-policy': "default-src 'none'; style-src 'unsafe-inline'",
                'x-content-type-options': 'nosniff'
            }).end(`<!doctype html><meta charset="utf-8"><title>SD-Online Billing</title>
<body style="margin:40px;background:#0e0f34;color:white;font:18px Segoe UI">
<h1>SD-ONLINE</h1><p>${canceled
                ? 'Checkout was canceled. You were not provisioned from this checkout.'
                : 'Stripe received your checkout. Return to the launcher; your server appears after payment confirmation.'}</p></body>`);
            return;
        }
        if (request.method === 'GET' && url.pathname === '/v1/plans') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            const store = await loadBillingStore(this.config.storePath);
            this.releaseExpired(store);
            await saveBillingStore(this.config.storePath, store);
            const lease = store.leases[steamId];
            return void this.json(response, 200, {
                ok: true,
                plans: this.config.plans.map(({ providerPriceId: _private, ...plan }) => plan),
                subscription: lease ? this.publicLease(lease) : null
            });
        }
        if (request.method === 'POST' && url.pathname === '/v1/checkout-sessions') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            const body = JSON.parse(await this.body(request, 4_096));
            const plan = this.plan(String(body.planId ?? ''));
            const serverName = typeof body.serverName === 'string' ? body.serverName.trim() : '';
            if (!plan || !validName(serverName)) {
                return void this.json(response, 400, { ok: false, error: 'checkout_invalid' });
            }
            // Compatibility for launchers that decide "buy vs change" from the
            // currently selected owned server (for example, a selected free trial).
            // Never create a second subscription: route an existing subscriber into
            // Stripe's hosted management portal instead.
            const current = (await loadBillingStore(this.config.storePath)).leases[steamId];
            if (current && ['active', 'past_due'].includes(current.state) &&
                current.customerId && current.subscriptionId) {
                const portalUrl = await this.config.provider.createPortal(current.customerId);
                return void this.json(response, 200, {
                    ok: true, checkoutUrl: portalUrl, existingSubscription: true
                });
            }
            const reservation = await this.serialized(async () => {
                const store = await loadBillingStore(this.config.storePath);
                this.releaseExpired(store);
                const existing = store.leases[steamId];
                if (existing && existing.state !== 'canceled' && existing.state !== 'reserved') {
                    return { error: 'subscription_exists' };
                }
                if (existing?.state === 'reserved' && !existing.subscriptionId) {
                    delete store.leases[steamId];
                }
                // Canceled servers retain customer saves and are never silently recycled.
                const used = new Set(Object.values(store.leases).map((lease) => lease.serverId));
                // A returning canceled customer reclaims that exact retained world.
                const serverId = existing?.serverId ??
                    this.config.pool.find((candidate) => !used.has(candidate));
                if (!serverId)
                    return { error: 'billing_capacity_unavailable' };
                const now = this.now();
                store.leases[steamId] = {
                    steamId, serverId, serverName, planId: plan.id, state: 'reserved',
                    createdAtMs: now, reservationExpiresAtMs: now + 32 * 60_000
                };
                await saveBillingStore(this.config.storePath, store);
                return { lease: { ...store.leases[steamId] } };
            });
            if ('error' in reservation) {
                return void this.json(response, 409, { ok: false, error: reservation.error });
            }
            const checkout = await this.config.provider.createCheckout({
                steamId,
                serverId: reservation.lease.serverId,
                serverName,
                plan
            });
            await this.serialized(async () => {
                const store = await loadBillingStore(this.config.storePath);
                const lease = store.leases[steamId];
                if (!lease || lease.serverId !== reservation.lease.serverId || lease.state !== 'reserved') {
                    throw new Error('billing_reservation_changed');
                }
                lease.checkoutSessionId = checkout.id;
                await saveBillingStore(this.config.storePath, store);
            });
            return void this.json(response, 201, {
                ok: true, checkoutUrl: checkout.url
            });
        }
        if (request.method === 'POST' && url.pathname === '/v1/portal-sessions') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            const lease = (await loadBillingStore(this.config.storePath)).leases[steamId];
            if (!lease || !lease.customerId || !lease.subscriptionId ||
                !['active', 'past_due'].includes(lease.state)) {
                return void this.json(response, 404, { ok: false, error: 'subscription_not_found' });
            }
            const portalUrl = await this.config.provider.createPortal(lease.customerId);
            return void this.json(response, 201, { ok: true, portalUrl });
        }
        if (request.method === 'POST' && url.pathname === '/v1/plan-changes') {
            const steamId = this.requireIdentity(request, response);
            if (!steamId)
                return;
            const body = JSON.parse(await this.body(request, 4_096));
            const plan = this.plan(String(body.planId ?? ''));
            const lease = (await loadBillingStore(this.config.storePath)).leases[steamId];
            if (!plan)
                return void this.json(response, 400, { ok: false, error: 'plan_invalid' });
            if (!lease || lease.state !== 'active' || !lease.subscriptionId) {
                return void this.json(response, 409, { ok: false, error: 'subscription_not_active' });
            }
            if (lease.planId === plan.id) {
                return void this.json(response, 200, {
                    ok: true, state: 'active', planId: plan.id, unchanged: true
                });
            }
            await this.config.provider.changePlan(lease.subscriptionId, plan);
            await this.serialized(async () => {
                const store = await loadBillingStore(this.config.storePath);
                const current = store.leases[steamId];
                if (!current || current.subscriptionId !== lease.subscriptionId ||
                    current.state !== 'active')
                    throw new Error('billing_subscription_changed');
                current.pendingPlanId = plan.id;
                await saveBillingStore(this.config.storePath, store);
            });
            return void this.json(response, 202, {
                ok: true, state: 'pending', planId: plan.id
            });
        }
        if (request.method === 'POST' && url.pathname === '/v1/webhooks/stripe') {
            const payload = await this.body(request, 512 * 1024);
            const rawSignature = request.headers['stripe-signature'] ?? '';
            const signature = Array.isArray(rawSignature) ? rawSignature[0] : rawSignature;
            if (!signature || !payload)
                return void this.json(response, 400, { ok: false });
            const event = await this.config.provider.parseWebhook(payload, signature);
            await this.applyWebhook(event);
            response.writeHead(204).end();
            return;
        }
        this.json(response, 404, { ok: false, error: 'not_found' });
    }
    async applyWebhook(event) {
        await this.serialized(async () => {
            const store = await loadBillingStore(this.config.storePath);
            if (store.webhookKeys.includes(event.key))
                return;
            const lease = Object.values(store.leases).find((candidate) => (event.subscriptionId && candidate.subscriptionId === event.subscriptionId) ||
                (event.checkoutSessionId && candidate.checkoutSessionId === event.checkoutSessionId) ||
                (event.steamId && candidate.steamId === event.steamId));
            if (!lease)
                return;
            if ((lease.lastWebhookAtMs ?? 0) > event.timestampMs)
                return;
            if (event.serverId && event.serverId !== lease.serverId) {
                throw new Error('billing_webhook_server_mismatch');
            }
            if (event.kind === 'checkout_paid') {
                if (!event.subscriptionId || !event.customerId ||
                    !event.planId || event.planId !== lease.planId) {
                    throw new Error('billing_checkout_metadata_invalid');
                }
                lease.subscriptionId = event.subscriptionId;
                lease.customerId = event.customerId;
                if (lease.state !== 'active') {
                    await this.config.provisioner.activate(lease, this.plan(lease.planId).maxPlayers);
                }
                lease.state = 'active';
            }
            else if (event.kind === 'subscription_active') {
                const plan = this.plan(event.planId ?? '');
                if (!plan)
                    throw new Error('billing_webhook_plan_invalid');
                if (event.subscriptionId)
                    lease.subscriptionId = event.subscriptionId;
                if (event.customerId)
                    lease.customerId = event.customerId;
                if (lease.state === 'active' && lease.planId !== plan.id) {
                    await this.config.provisioner.resize(lease, plan.maxPlayers);
                }
                else if (lease.state !== 'active') {
                    await this.config.provisioner.activate(lease, plan.maxPlayers);
                }
                lease.planId = plan.id;
                delete lease.pendingPlanId;
                lease.state = 'active';
            }
            else if (['subscription_past_due', 'subscription_canceled'].includes(event.kind)) {
                if (event.subscriptionId)
                    lease.subscriptionId = event.subscriptionId;
                if (event.customerId)
                    lease.customerId = event.customerId;
                const next = event.kind === 'subscription_past_due' ? 'past_due' : 'canceled';
                if (!['past_due', 'canceled'].includes(lease.state)) {
                    await this.config.provisioner.suspend(lease);
                }
                lease.state = next;
            }
            else if (event.kind === 'checkout_expired') {
                if (lease.state === 'reserved' && !lease.subscriptionId) {
                    delete store.leases[lease.steamId];
                }
            }
            else {
                return;
            }
            if (store.leases[lease.steamId])
                lease.lastWebhookAtMs = event.timestampMs;
            store.webhookKeys.push(event.key);
            if (store.webhookKeys.length > 2_000)
                store.webhookKeys.splice(0, 500);
            await saveBillingStore(this.config.storePath, store);
            console.log(JSON.stringify({
                timestamp: new Date().toISOString(), level: 'info',
                event: 'billing_webhook_applied', protocolVersion: 3,
                playerId: lease.steamId, worldId: lease.serverId,
                subscriptionState: lease.state, webhookKind: event.kind
            }));
        });
    }
    requireIdentity(request, response) {
        const value = request.headers.authorization ?? '';
        const token = value.startsWith('Bearer ') ? value.slice(7) : '';
        const [body, supplied, extra] = token.split('.');
        if (!body || !supplied || extra) {
            this.json(response, 401, { ok: false, error: 'steam_auth_required' });
            return undefined;
        }
        const expected = createHmac('sha256', this.config.sessionSecret).update(body).digest('base64url');
        if (!safeEqual(expected, supplied)) {
            this.json(response, 401, { ok: false, error: 'steam_auth_required' });
            return undefined;
        }
        try {
            const data = JSON.parse(Buffer.from(body, 'base64url').toString());
            if (data.version === 1 && data.purpose === 'identity' &&
                typeof data.steamId === 'string' && STEAM.test(data.steamId) &&
                typeof data.expiresAtMs === 'number' && data.expiresAtMs > this.now())
                return data.steamId;
        }
        catch { }
        this.json(response, 401, { ok: false, error: 'steam_auth_required' });
        return undefined;
    }
    releaseExpired(store) {
        for (const [steamId, lease] of Object.entries(store.leases)) {
            if (lease.state === 'reserved' && !lease.subscriptionId &&
                lease.reservationExpiresAtMs <= this.now())
                delete store.leases[steamId];
        }
    }
    plan(id) {
        return this.config.plans.find((candidate) => candidate.id === id);
    }
    publicLease(lease) {
        return {
            serverId: lease.serverId, planId: lease.planId, state: lease.state,
            maxPlayers: this.plan(lease.planId)?.maxPlayers ?? 0
        };
    }
    async serialized(callback) {
        const previous = this.operation;
        let release;
        this.operation = new Promise((resolve) => { release = resolve; });
        await previous;
        try {
            return await callback();
        }
        finally {
            release();
        }
    }
    async body(request, limit) {
        const chunks = [];
        let length = 0;
        for await (const chunk of request) {
            const bytes = Buffer.isBuffer(chunk) ? chunk : Buffer.from(chunk);
            length += bytes.length;
            if (length > limit)
                throw new Error('request_too_large');
            chunks.push(bytes);
        }
        return Buffer.concat(chunks).toString('utf8');
    }
    json(response, status, value) {
        response.writeHead(status, {
            'content-type': 'application/json; charset=utf-8', 'cache-control': 'no-store',
            'x-content-type-options': 'nosniff'
        }).end(JSON.stringify(value));
    }
}
function safeEqual(left, right) {
    const a = Buffer.from(left);
    const b = Buffer.from(right);
    return a.length === b.length && timingSafeEqual(a, b);
}
function validName(value) {
    return value.length >= 1 && value.length <= 64 && !/[\u0000-\u001F\u007F<>|]/.test(value);
}
//# sourceMappingURL=service.js.map