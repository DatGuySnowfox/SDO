import { GROUND_ITEM_MAX_COUNT, GROUND_ITEM_TTL_MS, HostAgentService, ZOMBIE_JOIN_GRACE_MS } from "./service.js";
import { existsSync, readFileSync } from 'node:fs';
import { join } from 'node:path';
const required = (name) => {
    const value = process.env[name];
    if (!value)
        throw new Error(`${name} is required`);
    return value;
};
const parseZombieSpawnCatalog = (parsed, maximum, source) => {
    if (!Array.isArray(parsed) || parsed.length < 1 || parsed.length > maximum) {
        throw new Error(`${source} must contain 1 to ${maximum} spawn points`);
    }
    return parsed.map((candidate, index) => {
        if (!candidate || typeof candidate !== 'object') {
            throw new Error(`Zombie spawn point ${index} in ${source} must be an object`);
        }
        const point = candidate;
        const x = Number(point.x);
        const y = Number(point.y);
        const z = Number(point.z);
        const yaw = Number(point.yaw ?? 0);
        if (![x, y, z, yaw].every(Number.isFinite) ||
            Math.max(Math.abs(x), Math.abs(y), Math.abs(z)) > 10_000_000) {
            throw new Error(`Zombie spawn point ${index} in ${source} has invalid coordinates`);
        }
        let classPath;
        if (point.classPath !== undefined) {
            if (typeof point.classPath !== 'string') {
                throw new Error(`Zombie spawn point ${index} in ${source} has an invalid classPath`);
            }
            classPath = point.classPath;
            if (!classPath.startsWith('/Game/AI/Zombies/') ||
                !classPath.endsWith('_C')) {
                throw new Error(`Zombie spawn point ${index} in ${source} has an unsupported classPath`);
            }
        }
        return { x, y, z, yaw, classPath };
    });
};
const zombieSpawnPoints = (() => {
    const catalogPath = process.env.SDO_ZOMBIE_SPAWNER_CATALOG_PATH;
    if (catalogPath) {
        if (!existsSync(catalogPath)) {
            throw new Error(`SDO_ZOMBIE_SPAWNER_CATALOG_PATH was not found: ${catalogPath}`);
        }
        const parsed = JSON.parse(readFileSync(catalogPath, 'utf8').replace(/^\uFEFF/, ''));
        return parseZombieSpawnCatalog(parsed, 1024, catalogPath);
    }
    const encoded = process.env.SDO_ZOMBIE_SPAWN_POINTS;
    if (!encoded)
        return undefined;
    const parsed = JSON.parse(encoded);
    return parseZombieSpawnCatalog(parsed, 64, 'SDO_ZOMBIE_SPAWN_POINTS');
})();
const worldId = process.env.SDO_WORLD_ID ?? '11111111-1111-4111-8111-111111111111';
const modRules = (() => {
    const path = process.env.SDO_MOD_RULES_PATH ?? join(process.env.SDO_MOD_RULES_ROOT ?? '/var/lib/sdo-v3/directory/server-mods', `${worldId}.json`);
    if (!existsSync(path))
        return undefined;
    const value = JSON.parse(readFileSync(path, 'utf8').replace(/^\uFEFF/, ''));
    if (![1, 2].includes(Number(value.schemaVersion)) ||
        value.worldId !== worldId || !value.rules) {
        throw new Error('SDO mod rules file is invalid');
    }
    const dayLengthMinutes = value.rules.time?.dayLengthMinutes;
    const weatherPreset = value.rules.weather?.preset;
    const timeMode = value.rules.time?.mode;
    const startTimeOfDay = value.rules.time?.startTime;
    const weatherCycle = value.rules.weather?.cycle;
    const weatherPhaseMinutes = value.rules.weather?.phaseMinutes;
    const weatherTransition = value.rules.weather?.transition;
    if (dayLengthMinutes !== undefined &&
        (!Number.isSafeInteger(dayLengthMinutes) || Number(dayLengthMinutes) < 30 ||
            Number(dayLengthMinutes) > 720))
        throw new Error('SDO mod day length is invalid');
    if (weatherPreset !== undefined &&
        !['clear', 'overcast', 'rain', 'storm', 'snow'].includes(String(weatherPreset)))
        throw new Error('SDO mod weather preset is invalid');
    if (timeMode !== undefined && !['cycle', 'frozen'].includes(String(timeMode))) {
        throw new Error('SDO mod time mode is invalid');
    }
    if (startTimeOfDay !== undefined && (!Number.isSafeInteger(startTimeOfDay) || Number(startTimeOfDay) < 0 ||
        Number(startTimeOfDay) > 2399))
        throw new Error('SDO mod start time is invalid');
    if (weatherCycle !== undefined && (!Array.isArray(weatherCycle) || weatherCycle.length < 2 || weatherCycle.length > 8 ||
        weatherCycle.some((preset) => !['clear', 'overcast', 'rain', 'storm', 'snow'].includes(String(preset)))))
        throw new Error('SDO mod weather cycle is invalid');
    if (weatherPhaseMinutes !== undefined && (!Number.isSafeInteger(weatherPhaseMinutes) || Number(weatherPhaseMinutes) < 1 ||
        Number(weatherPhaseMinutes) > 120))
        throw new Error('SDO mod weather phase is invalid');
    if (weatherTransition !== undefined &&
        !['smooth', 'instant'].includes(String(weatherTransition))) {
        throw new Error('SDO mod weather transition is invalid');
    }
    return {
        dayLengthMinutes: dayLengthMinutes === undefined ? undefined : Number(dayLengthMinutes),
        weatherPreset: weatherPreset === undefined ? undefined : String(weatherPreset),
        timeMode: timeMode === undefined ? undefined : String(timeMode),
        startTimeOfDay: startTimeOfDay === undefined ? undefined : Number(startTimeOfDay),
        weatherCycle: weatherCycle === undefined ? undefined :
            weatherCycle.map((preset) => String(preset)),
        weatherPhaseMinutes: weatherPhaseMinutes === undefined ? undefined :
            Number(weatherPhaseMinutes),
        weatherTransition: weatherTransition === undefined ? undefined :
            String(weatherTransition)
    };
})();
function parseWeatherCycle(raw) {
    if (!raw)
        return undefined;
    const parts = raw.split(',').map((entry) => entry.trim()).filter(Boolean);
    return parts.length >= 2 ? parts : undefined;
}
const dayLengthMinutes = modRules?.dayLengthMinutes ??
    (process.env.SDO_DAY_LENGTH_MINUTES
        ? Number(process.env.SDO_DAY_LENGTH_MINUTES)
        : undefined);
const weatherCycle = modRules?.weatherCycle ??
    parseWeatherCycle(process.env.SDO_WEATHER_CYCLE);
const weatherPhaseMinutes = modRules?.weatherPhaseMinutes ??
    (process.env.SDO_WEATHER_PHASE_MINUTES
        ? Number(process.env.SDO_WEATHER_PHASE_MINUTES)
        : undefined);
const agent = new HostAgentService({
    gatewayHost: process.env.SDO_GATEWAY_HOST ?? '127.0.0.1',
    gatewayPort: Number(process.env.SDO_GATEWAY_PORT ?? 31000),
    hostSecret: required('SDO_HOST_SECRET'),
    worldId,
    statusPort: Number(process.env.SDO_HOST_STATUS_PORT ?? 32001),
    readinessUrl: process.env.SDO_HOST_READINESS_URL,
    gameDataDir: process.env.SDO_HOST_GAME_DATA_DIR,
    readinessMaxAgeMs: Number(process.env.SDO_HOST_READINESS_MAX_AGE_MS ?? 5_000),
    buildId: process.env.SDO_HOST_BUILD_ID ?? 'unknown',
    authoritativeZombie: process.env.SDO_AUTHORITATIVE_ZOMBIE === '1',
    zombieDamageEnabled: process.env.SDO_ZOMBIE_DAMAGE_ENABLED !== '0',
    worldSimulationMs: Number(process.env.SDO_WORLD_SIMULATION_MS ?? 50),
    worldStatePath: process.env.SDO_WORLD_STATE_PATH,
    worldSaveIntervalMs: Number(process.env.SDO_WORLD_SAVE_INTERVAL_MS ?? 5_000),
    worldStatePublishIntervalMs: Number(process.env.SDO_WORLD_STATE_PUBLISH_INTERVAL_MS ?? 2_000),
    movementRelayIntervalMs: Number(process.env.SDO_MOVEMENT_RELAY_INTERVAL_MS ?? 33),
    groundItemTtlMs: Number(process.env.SDO_GROUND_ITEM_TTL_MS ?? GROUND_ITEM_TTL_MS),
    groundItemMaxCount: Number(process.env.SDO_GROUND_ITEM_MAX_COUNT ?? GROUND_ITEM_MAX_COUNT),
    zombieRespawnMs: Number(process.env.SDO_ZOMBIE_RESPAWN_MS ?? 30_000),
    zombieSpawnIntervalMs: Number(process.env.SDO_ZOMBIE_SPAWN_INTERVAL_MS ?? 1_000),
    zombiesPerPlayer: Number(process.env.SDO_ZOMBIES_PER_PLAYER ?? 1),
    zombieMaxCount: Number(process.env.SDO_ZOMBIE_MAX_COUNT ?? 1),
    zombieSpawnDistance: Number(process.env.SDO_ZOMBIE_SPAWN_DISTANCE ?? 700),
    zombieSpawnerActivationRadius: Number(process.env.SDO_ZOMBIE_SPAWNER_ACTIVATION_RADIUS ?? 18_000),
    zombieJoinGraceMs: Number(process.env.SDO_ZOMBIE_JOIN_GRACE_MS ?? ZOMBIE_JOIN_GRACE_MS),
    zombieSpawnPoints,
    zombieClassPath: process.env.SDO_ZOMBIE_CLASS_PATH,
    dayLengthMinutes,
    weatherPreset: modRules?.weatherPreset,
    timeMode: modRules?.timeMode,
    startTimeOfDay: modRules?.startTimeOfDay,
    weatherCycle,
    weatherPhaseMinutes,
    weatherTransition: modRules?.weatherTransition
});
await agent.start();
const shutdown = async () => {
    await agent.stop();
    process.exit(0);
};
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);
//# sourceMappingURL=main.js.map