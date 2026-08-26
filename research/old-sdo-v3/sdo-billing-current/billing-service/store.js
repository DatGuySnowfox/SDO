import { mkdir, readFile, rename, writeFile } from 'node:fs/promises';
import { dirname } from 'node:path';
const STEAM = /^steam_7656119[0-9]{10}$/;
const SERVER = /^(?:(?:customer|trial|home)-world-[0-9]{1,3})$/;
const PLAN = /^[A-Za-z0-9_-]{1,64}$/;
export async function loadBillingStore(path) {
    try {
        const text = await readFile(path, 'utf8');
        return validateBillingStore(JSON.parse(text.replace(/^\uFEFF/, '')));
    }
    catch (error) {
        if (error.code === 'ENOENT') {
            return { version: 1, leases: {}, webhookKeys: [] };
        }
        throw error;
    }
}
export async function saveBillingStore(path, value) {
    const validated = validateBillingStore(value);
    await mkdir(dirname(path), { recursive: true });
    const temporary = `${path}.${process.pid}.tmp`;
    await writeFile(temporary, `${JSON.stringify(validated, null, 2)}\n`, {
        encoding: 'utf8',
        mode: 0o600
    });
    await rename(temporary, path);
}
export function validateBillingStore(input) {
    if (!input || typeof input !== 'object' || Array.isArray(input)) {
        throw new Error('billing_store_invalid');
    }
    const value = input;
    if (value.version !== 1 || !value.leases || typeof value.leases !== 'object' ||
        !Array.isArray(value.webhookKeys) || value.webhookKeys.length > 2_000) {
        throw new Error('billing_store_invalid');
    }
    const leases = {};
    const servers = new Set();
    const subscriptions = new Set();
    for (const [steamId, lease] of Object.entries(value.leases)) {
        if (!STEAM.test(steamId) || !lease || lease.steamId !== steamId ||
            !SERVER.test(lease.serverId) || servers.has(lease.serverId) ||
            !validName(lease.serverName) || !PLAN.test(lease.planId) ||
            (lease.pendingPlanId !== undefined && !PLAN.test(lease.pendingPlanId)) ||
            (lease.checkoutSessionId !== undefined &&
                !/^cs_(?:test_|live_)?[A-Za-z0-9_]{6,120}$/.test(lease.checkoutSessionId)) ||
            !['reserved', 'active', 'past_due', 'canceled'].includes(lease.state) ||
            !Number.isSafeInteger(lease.createdAtMs) ||
            !Number.isSafeInteger(lease.reservationExpiresAtMs) ||
            (lease.subscriptionId !== undefined &&
                (lease.subscriptionId.length < 1 || lease.subscriptionId.length > 128)) ||
            (lease.customerId !== undefined &&
                (lease.customerId.length < 1 || lease.customerId.length > 128))) {
            throw new Error('billing_store_invalid');
        }
        if (lease.subscriptionId && subscriptions.has(lease.subscriptionId)) {
            throw new Error('billing_store_invalid');
        }
        servers.add(lease.serverId);
        if (lease.subscriptionId)
            subscriptions.add(lease.subscriptionId);
        leases[steamId] = { ...lease };
    }
    const webhookKeys = value.webhookKeys.filter((key) => typeof key === 'string' && /^[A-Fa-f0-9]{64}$/.test(key));
    if (webhookKeys.length !== value.webhookKeys.length)
        throw new Error('billing_store_invalid');
    return { version: 1, leases, webhookKeys: [...new Set(webhookKeys)] };
}
function validName(value) {
    return typeof value === 'string' && value.length >= 1 && value.length <= 64 &&
        !/[\u0000-\u001F\u007F<>|]/.test(value);
}
//# sourceMappingURL=store.js.map