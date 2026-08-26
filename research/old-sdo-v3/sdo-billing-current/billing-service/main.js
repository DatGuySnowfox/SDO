import { spawn } from 'node:child_process';
import { StripeProvider } from "./stripe-provider.js";
import { BillingService } from "./service.js";
const required = (name) => {
    const value = process.env[name];
    if (!value)
        throw new Error(`${name} is required`);
    return value;
};
const plans = JSON.parse(required('SDO_BILLING_PLANS'));
const vpsHelper = required('SDO_BILLING_HELPER');
const homeHelper = process.env.SDO_BILLING_HELPER_HOME ?? `${vpsHelper}-home`;
const homePool = (process.env.SDO_BILLING_POOL_HOME ?? '')
    .split(',')
    .map((value) => value.trim())
    .filter(Boolean);
if (plans.some((plan) => plan.hostingMode === 'home-host') && homePool.length < 1) {
    throw new Error('SDO_BILLING_POOL_HOME is required when home-host plans are configured');
}
const provisioner = {
    activate: (lease, maxPlayers) => invoke('activate', lease, maxPlayers),
    resize: (lease, maxPlayers) => invoke('resize', lease, maxPlayers),
    suspend: (lease) => invoke('suspend', lease, 0)
};
async function invoke(action, lease, maxPlayers) {
    const helper = lease.serverId.startsWith('home-world-') ? homeHelper : vpsHelper;
    await new Promise((resolve, reject) => {
        const child = spawn('/usr/bin/sudo', [
            '-n', helper, action, lease.serverId, lease.steamId, String(maxPlayers)
        ], { stdio: ['pipe', 'pipe', 'pipe'] });
        let output = '';
        const timer = setTimeout(() => {
            child.kill('SIGTERM');
            reject(new Error('billing_provision_timeout'));
        }, 90_000);
        child.stdout.on('data', (chunk) => {
            if (output.length < 256 * 1024)
                output += chunk.toString('utf8');
        });
        child.stderr.on('data', (chunk) => {
            if (output.length < 256 * 1024)
                output += chunk.toString('utf8');
        });
        child.once('error', (error) => { clearTimeout(timer); reject(error); });
        child.once('close', (code) => {
            clearTimeout(timer);
            if (code === 0 && output.includes('"ok":true'))
                resolve();
            else
                reject(new Error(`billing_provision_failed_${code ?? 'signal'}`));
        });
        child.stdin.end(`${lease.serverName}\n`);
    });
}
const service = new BillingService({
    host: process.env.SDO_BILLING_HOST ?? '127.0.0.1',
    port: Number(process.env.SDO_BILLING_PORT ?? 27934),
    publicBaseUrl: required('SDO_BILLING_PUBLIC_BASE_URL'),
    sessionSecret: required('SDO_DIRECTORY_SESSION_SECRET'),
    storePath: required('SDO_BILLING_STORE'),
    pool: required('SDO_BILLING_POOL').split(',').map((value) => value.trim()),
    homePool,
    plans,
    provider: new StripeProvider({
        secretKey: required('STRIPE_SECRET_KEY'),
        webhookSecret: required('STRIPE_WEBHOOK_SECRET'),
        returnUrl: required('SDO_BILLING_RETURN_URL'),
        plans,
        automaticTax: process.env.SDO_STRIPE_AUTOMATIC_TAX === 'true'
    }),
    provisioner
});
await service.start();
const shutdown = async () => { await service.stop(); process.exit(0); };
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map