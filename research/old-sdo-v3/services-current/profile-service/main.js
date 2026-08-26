import { ProfileService } from "./service.js";
const required = (name) => {
    const value = process.env[name];
    if (!value)
        throw new Error(`${name} is required`);
    return value;
};
const service = new ProfileService({
    host: process.env.SDO_PROFILE_HOST ?? '127.0.0.1',
    port: Number(process.env.SDO_PROFILE_PORT ?? 31002),
    root: required('SDO_PROFILE_ROOT'),
    ticketSecret: required('SDO_TICKET_SECRET'),
    sessionSecret: required('SDO_PROFILE_SESSION_SECRET'),
    sessionTtlMs: Number(process.env.SDO_PROFILE_SESSION_TTL_MS ?? 86_400_000),
    keepBackups: Number(process.env.SDO_PROFILE_KEEP_BACKUPS ?? 5),
    starterBundlePath: process.env.SDO_PROFILE_STARTER_BUNDLE
});
await service.start();
const shutdown = async () => {
    await service.stop();
    process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map