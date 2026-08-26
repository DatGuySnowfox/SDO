import braintree from 'braintree';
import { createHash } from 'node:crypto';
export class BraintreeProvider {
    gateway;
    constructor(config) {
        this.gateway = new braintree.BraintreeGateway({
            environment: config.environment === 'production'
                ? braintree.Environment.Production : braintree.Environment.Sandbox,
            merchantId: config.merchantId,
            publicKey: config.publicKey,
            privateKey: config.privateKey
        });
    }
    async clientToken(customerId) {
        const result = await this.gateway.clientToken.generate(customerId ? { customerId } : {});
        return result.clientToken;
    }
    async createSubscription(input) {
        const customerId = input.steamId.replace(/^steam_/, 'sdo_');
        let customer;
        try {
            customer = await this.gateway.customer.find(customerId);
        }
        catch (error) {
            if (error.type !== 'notFoundError')
                throw error;
            const created = await this.gateway.customer.create({ id: customerId });
            if (!created.success || !created.customer)
                throw new Error('braintree_customer_failed');
            customer = created.customer;
        }
        const method = await this.gateway.paymentMethod.create({
            customerId: customer.id,
            paymentMethodNonce: input.nonce,
            options: { makeDefault: true, verifyCard: true }
        });
        if (!method.success || !method.paymentMethod?.token) {
            throw new Error('braintree_payment_method_failed');
        }
        const subscription = await this.gateway.subscription.create({
            paymentMethodToken: method.paymentMethod.token,
            planId: input.plan.braintreePlanId,
            price: input.plan.price
        });
        if (!subscription.success || !subscription.subscription) {
            throw new Error('braintree_subscription_failed');
        }
        return {
            id: subscription.subscription.id,
            customerId: customer.id,
            status: subscription.subscription.status
        };
    }
    async updateSubscription(input) {
        const result = await this.gateway.subscription.update(input.subscriptionId, {
            planId: input.plan.braintreePlanId,
            price: input.plan.price,
            options: {
                prorateCharges: true,
                revertSubscriptionOnProrationFailure: true
            }
        });
        if (!result.success)
            throw new Error('braintree_subscription_update_failed');
    }
    async parseWebhook(signature, payload) {
        const value = await this.gateway.webhookNotification.parse(signature, payload);
        const notification = value;
        return {
            key: createHash('sha256').update(`${signature}\0${payload}`).digest('hex'),
            kind: camelToSnake(String(notification.kind)),
            timestampMs: new Date(notification.timestamp).getTime(),
            subscriptionId: notification.subscription?.id
        };
    }
}
function camelToSnake(value) {
    return value.replace(/([a-z0-9])([A-Z])/g, '$1_$2').toLowerCase();
}
//# sourceMappingURL=braintree-provider.js.map