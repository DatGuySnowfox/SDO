import { ServerDirectoryService } from "./service.js";
const path = process.env.SDO_SERVER_REGISTRY;
if (!path)
    throw new Error('SDO_SERVER_REGISTRY is required');
const authMirrorBaseUrls = (process.env.SDO_DIRECTORY_AUTH_MIRROR_URLS ??
    'https://sdo-online-edge.spencerharzgaming.workers.dev/directory-api')
    .split(/[,;]/)
    .map((value) => value.trim())
    .filter(Boolean);
const service = new ServerDirectoryService({
    registryPath: path,
    host: process.env.SDO_DIRECTORY_HOST ?? '127.0.0.1',
    port: Number(process.env.SDO_DIRECTORY_PORT ?? 31100),
    publicBaseUrl: process.env.SDO_DIRECTORY_PUBLIC_BASE_URL,
    authMirrorBaseUrls,
    sessionSecret: process.env.SDO_DIRECTORY_SESSION_SECRET,
    ownershipPath: process.env.SDO_SERVER_OWNERS,
    trialPath: process.env.SDO_SERVER_TRIALS,
    modRegistryPath: process.env.SDO_MOD_REGISTRY,
    modAssignmentsPath: process.env.SDO_MOD_ASSIGNMENTS,
    modRulesRoot: process.env.SDO_MOD_RULES_ROOT,
    modPackageRoot: process.env.SDO_MOD_PACKAGE_ROOT,
    modDraftPath: process.env.SDO_MOD_DRAFTS,
    iconsPath: process.env.SDO_SERVER_ICONS,
    controlHelperPath: process.env.SDO_CUSTOMER_CONTROL_HELPER,
    homeHeartbeatsPath: process.env.SDO_HOME_HOST_HEARTBEATS,
    homeInstancesRoot: process.env.SDO_HOME_INSTANCES_ROOT,
    discordBotSecret: process.env.SDO_DISCORD_BOT_SECRET
});
await service.start();
const shutdown = async () => {
    await service.stop();
    process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map