import { HostAgentService } from "./service.js";
const required = (name) => {
    const value = process.env[name];
    if (!value)
        throw new Error(`${name} is required`);
    return value;
};
const agent = new HostAgentService({
    gatewayHost: process.env.SDO_GATEWAY_HOST ?? '127.0.0.1',
    gatewayPort: Number(process.env.SDO_GATEWAY_PORT ?? 31000),
    hostSecret: required('SDO_HOST_SECRET'),
    worldId: process.env.SDO_WORLD_ID ?? '11111111-1111-4111-8111-111111111111',
    statusPort: Number(process.env.SDO_HOST_STATUS_PORT ?? 32001),
    readinessUrl: process.env.SDO_HOST_READINESS_URL,
    gameDataDir: process.env.SDO_HOST_GAME_DATA_DIR,
    readinessMaxAgeMs: Number(process.env.SDO_HOST_READINESS_MAX_AGE_MS ?? 5_000),
    buildId: process.env.SDO_HOST_BUILD_ID ?? 'unknown',
    authoritativeZombie: process.env.SDO_AUTHORITATIVE_ZOMBIE === '1',
    worldSimulationMs: Number(process.env.SDO_WORLD_SIMULATION_MS ?? 100),
    worldStatePath: process.env.SDO_WORLD_STATE_PATH,
    worldSaveIntervalMs: Number(process.env.SDO_WORLD_SAVE_INTERVAL_MS ?? 5_000),
    worldStatePublishIntervalMs: Number(process.env.SDO_WORLD_STATE_PUBLISH_INTERVAL_MS ?? 2_000),
    zombieRespawnMs: Number(process.env.SDO_ZOMBIE_RESPAWN_MS ?? 30_000),
    zombieClassPath: process.env.SDO_ZOMBIE_CLASS_PATH
});
await agent.start();
const shutdown = async () => {
    await agent.stop();
    process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map