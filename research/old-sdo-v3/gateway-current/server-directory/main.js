import { ServerDirectoryService } from "./service.js";
const path = process.env.SDO_SERVER_REGISTRY;
if (!path)
    throw new Error('SDO_SERVER_REGISTRY is required');
const service = new ServerDirectoryService({
    registryPath: path,
    host: process.env.SDO_DIRECTORY_HOST ?? '127.0.0.1',
    port: Number(process.env.SDO_DIRECTORY_PORT ?? 31100),
    publicBaseUrl: process.env.SDO_DIRECTORY_PUBLIC_BASE_URL,
    sessionSecret: process.env.SDO_DIRECTORY_SESSION_SECRET
});
await service.start();
const shutdown = async () => {
    await service.stop();
    process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map