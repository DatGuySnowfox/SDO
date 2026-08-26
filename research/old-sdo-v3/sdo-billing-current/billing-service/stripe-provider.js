import Stripe from 'stripe';
import { createHash } from 'node:crypto';
export class StripeProvider {
    stripe;
    webhookSecret;
    returnUrl;
    plansByPrice;
    automaticTax;
    constructor(config) {
        this.stripe = new Stripe(config.secretKey);
        this.webhookSecret = config.webhookSecret;
        this.returnUrl = config.returnUrl;
        this.plansByPrice = new Map(config.plans.map((plan) => [plan.providerPriceId, plan]));
        this.automaticTax = config.automaticTax === true;
    }
    async createCheckout(input) {
        const metadata = {
            sdoSteamId: input.steamId,
            sdoServerId: input.serverId,
            sdoPlanId: input.plan.id,
            sdoServerName: input.serverName
        };
        const session = await this.stripe.checkout.sessions.create({
            mode: 'subscription',
            line_items: [{ price: input.plan.providerPriceId, quantity: 1 }],
            client_reference_id: input.steamId,
            metadata,
            subscription_data: { metadata },
            automatic_tax: { enabled: this.automaticTax },
            billing_address_collection: 'auto',
            success_url: `${this.returnUrl}?checkout=success`,
            cancel_url: `${this.returnUrl}?checkout=canceled`,
            expires_at: Math.floor(Date.now() / 1_000) + 31 * 60
        });
        if (!session.url)
            throw new Error('stripe_checkout_url_missing');
        return { id: session.id, url: session.url };
    }
    async createPortal(customerId) {
        const session = await this.stripe.billingPortal.sessions.create({
            customer: customerId,
            return_url: this.returnUrl
        });
        return session.url;
    }
    async changePlan(subscriptionId, plan) {
        const subscription = await this.stripe.subscriptions.retrieve(subscriptionId);
        const item = subscription.items.data[0];
        if (!item)
            throw new Error('stripe_subscription_item_missing');
        if (item.price.id === plan.providerPriceId)
            return;
        await this.stripe.subscriptions.update(subscriptionId, {
            items: [{ id: item.id, price: plan.providerPriceId }],
            proration_behavior: 'create_prorations',
            metadata: { ...subscription.metadata, sdoPlanId: plan.id }
        });
    }
    async parseWebhook(payload, signature) {
        const event = this.stripe.webhooks.constructEvent(payload, signature, this.webhookSecret);
        const timestampMs = event.created * 1_000;
        if (event.type === 'checkout.session.expired') {
            const session = event.data.object;
            return {
                key: webhookKey(event.id), kind: 'checkout_expired', timestampMs,
                checkoutSessionId: session.id,
                steamId: metadataValue(session.metadata, 'sdoSteamId'),
                serverId: metadataValue(session.metadata, 'sdoServerId')
            };
        }
        if (event.type === 'checkout.session.completed' ||
            event.type === 'checkout.session.async_payment_succeeded') {
            const session = event.data.object;
            if (!['paid', 'no_payment_required'].includes(session.payment_status)) {
                return { key: webhookKey(event.id), kind: 'ignored', timestampMs };
            }
            return {
                key: webhookKey(event.id), kind: 'checkout_paid', timestampMs,
                checkoutSessionId: session.id,
                steamId: metadataValue(session.metadata, 'sdoSteamId'),
                serverId: metadataValue(session.metadata, 'sdoServerId'),
                planId: metadataValue(session.metadata, 'sdoPlanId'),
                subscriptionId: idOf(session.subscription),
                customerId: idOf(session.customer)
            };
        }
        if (event.type === 'customer.subscription.updated' ||
            event.type === 'customer.subscription.deleted') {
            const subscription = event.data.object;
            const priceId = subscription.items.data[0]?.price.id;
            const plan = priceId ? this.plansByPrice.get(priceId) : undefined;
            const status = subscription.status;
            let kind = 'ignored';
            if (event.type === 'customer.subscription.deleted' ||
                ['canceled', 'incomplete_expired'].includes(status)) {
                kind = 'subscription_canceled';
            }
            else if (['past_due', 'unpaid', 'paused'].includes(status)) {
                kind = 'subscription_past_due';
            }
            else if (['active', 'trialing'].includes(status)) {
                kind = 'subscription_active';
            }
            return {
                key: webhookKey(event.id), kind, timestampMs,
                steamId: metadataValue(subscription.metadata, 'sdoSteamId'),
                serverId: metadataValue(subscription.metadata, 'sdoServerId'),
                planId: plan?.id,
                subscriptionId: subscription.id,
                customerId: idOf(subscription.customer)
            };
        }
        return { key: webhookKey(event.id), kind: 'ignored', timestampMs };
    }
}
function metadataValue(metadata, key) {
    const value = metadata?.[key];
    return typeof value === 'string' && value.length > 0 ? value : undefined;
}
function idOf(value) {
    if (typeof value === 'string')
        return value;
    return value?.id;
}
function webhookKey(eventId) {
    return createHash('sha256').update(eventId).digest('hex');
}
//# sourceMappingURL=stripe-provider.js.map