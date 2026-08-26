import { ServerDirectoryService } from "./service.js";
const path = process.env.SDO_SERVER_REGISTRY;
if (!path)
    throw new Error('SDO_SERVER_REGISTRY is required');
const service = new ServerDirectoryService({
    registryPath: path,
    host: process.env.SDO_DIRECTORY_HOST ?? '127.0.0.1',
    port: Number(process.env.SDO_DIRECTORY_PORT ?? 31100),
    publicBaseUrl: process.env.SDO_DIRECTORY_PUBLIC_BASE_URL,
    sessionSecret: process.env.SDO_DIRECTORY_SESSION_SECRET,
    ownershipPath: process.env.SDO_SERVER_OWNERS,
    trialPath: process.env.SDO_SERVER_TRIALS,
    modRegistryPath: process.env.SDO_MOD_REGISTRY,
    modAssignmentsPath: process.env.SDO_MOD_ASSIGNMENTS,
    modRulesRoot: process.env.SDO_MOD_RULES_ROOT,
    controlHelperPath: process.env.SDO_CUSTOMER_CONTROL_HELPER
});
await service.start();
const shutdown = async () => {
    await service.stop();
    process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map