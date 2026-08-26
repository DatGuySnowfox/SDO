import { GatewayService } from "./service.js";
const required = (name) => {
    const value = process.env[name];
    if (!value)
        throw new Error(`${name} is required`);
    return value;
};
const gateway = new GatewayService({
    host: process.env.SDO_GATEWAY_HOST ?? '0.0.0.0',
    port: Number(process.env.SDO_GATEWAY_PORT ?? 31000),
    healthHost: process.env.SDO_GATEWAY_HEALTH_HOST ?? '127.0.0.1',
    healthPort: Number(process.env.SDO_GATEWAY_HEALTH_PORT ?? 31001),
    healthTlsCertPath: process.env.SDO_GATEWAY_HEALTH_TLS_CERT,
    healthTlsKeyPath: process.env.SDO_GATEWAY_HEALTH_TLS_KEY,
    worldId: process.env.SDO_WORLD_ID ?? '11111111-1111-4111-8111-111111111111',
    hostSecret: required('SDO_HOST_SECRET'),
    ticketSecret: required('SDO_TICKET_SECRET'),
    publicGatewayHost: process.env.SDO_PUBLIC_GATEWAY_HOST,
    publicGatewayPort: Number(process.env.SDO_PUBLIC_GATEWAY_PORT ?? process.env.SDO_GATEWAY_PORT ?? 31000),
    identityValidationUrl: process.env.SDO_IDENTITY_VALIDATION_URL,
    stagingIssuerSecret: process.env.SDO_STAGING_ISSUER_SECRET,
    ticketTtlMs: Number(process.env.SDO_TICKET_TTL_MS ?? 120_000),
    hostTimeoutMs: Number(process.env.SDO_HOST_TIMEOUT_MS ?? 5_000),
    clientIdleMs: Number(process.env.SDO_CLIENT_IDLE_MS ?? 15_000),
    malformedLimit: Number(process.env.SDO_MALFORMED_LIMIT ?? 3),
    maxFramesPerSecond: Number(process.env.SDO_MAX_FRAMES_PER_SECOND ?? 120),
    maxHostFramesPerSecond: Number(process.env.SDO_MAX_HOST_FRAMES_PER_SECOND ?? 4_096)
});
await gateway.start();
const shutdown = async () => {
    await gateway.stop();
    process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map