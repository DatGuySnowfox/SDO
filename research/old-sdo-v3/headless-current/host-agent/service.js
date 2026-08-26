import { randomUUID } from 'node:crypto';
import { readFile, stat } from 'node:fs/promises';
import { createServer as createHttpServer } from 'node:http';
import { createConnection } from 'node:net';
import { join } from 'node:path';
import { decodeJoinRequest, decodeMovement, decodeString, decodeWorldAction, encodeFrame, encodeMovement, encodePlayerDamage, encodeString, encodeWorldAction, encodeWorldEntityDescriptor, encodeWorldEntityState, encodeWorldState, FrameDecoder, isNewerSequence, MessageType, MOVEMENT_STATE_IN_VEHICLE, stableNumericId, WorldEntityKind } from "../shared-protocol/index.js";
import { validSteamIdentity } from "../profile-service/bundle.js";
import { log } from "../shared/log.js";
import { WorldSnapshotStore } from "./world-store.js";
/** Matches the client bridge zombie proxy interest radius in dllmain.cpp. */
export const CLIENT_ZOMBIE_INTEREST_RADIUS = 15_000;
/** Keep authoritative zombies alive while any living player is still in combat range. */
export const ZOMBIE_COMBAT_LEASH_RADIUS = 18_000;
/** Hold map spawners active until players move beyond activation plus this margin. */
export const ZOMBIE_SPAWNER_DEACTIVATION_MARGIN = 2_000;
/** Matches the client bridge ground-item proxy interest radius in dllmain.cpp. */
export const CLIENT_GROUND_ITEM_INTEREST_RADIUS = 2_500;
/** Default lifetime for unconsumed authoritative ground loot. */
export const GROUND_ITEM_TTL_MS = 6 * 60 * 60_000;
/** Hard cap on persisted authoritative ground loot entities. */
export const GROUND_ITEM_MAX_COUNT = 128;
/** Never spawn dynamic zombies closer than this to a living player. */
export const ZOMBIE_PLAYER_EXCLUSION_RADIUS = 5_500;
/** Block only true pop-in placements directly on top of a player. */
export const ZOMBIE_SPAWN_POP_IN_GUARD = 1_500;
/** Minimum distance for dynamic/exploration zombie placements. */
export const ZOMBIE_MIN_SPAWN_DISTANCE_FROM_PLAYER = 3_500;
/** Pause new zombie placements near a player after combat starts. */
export const ZOMBIE_COMBAT_SPAWN_SUPPRESSION_MS = 3 * 60 * 1000;
/** Prefer map spawners in this outward band so threats appear before POIs. */
export const ZOMBIE_SPAWN_AHEAD_MIN_RADIUS = 5_500;
export const ZOMBIE_SPAWN_AHEAD_MAX_RADIUS = 15_000;
/** After join or respawn, keep a wider clear area and fewer nearby spawns. */
export const ZOMBIE_JOIN_GRACE_MS = 60_000;
export const ZOMBIE_JOIN_EXCLUSION_RADIUS = 8_000;
/** Minimum nearby authoritative zombies per living player before exploration fill stops. */
export const EXPLORATION_ZOMBIE_TARGET = 8;
/** Host simulation engages zombies toward players within this planar distance. */
export const ZOMBIE_CHASE_RADIUS = 12_000;
/** Minimum delay before a dead zombie slot may spawn again. */
export const ZOMBIE_RESPAWN_AFTER_DEATH_MS = 7 * 60 * 1000;
/** Keep remote player proxies alive even when clients stand still after joining. */
export const PLAYER_SNAPSHOT_REBROADCAST_MS = 500;
const WEATHER_PRESETS = {
    clear: { rain: 0, snow: 0, fog: 0.03, cloudCoverage: 0.1, wind: 0.1, thunder: 0 },
    overcast: { rain: 0, snow: 0, fog: 0.2, cloudCoverage: 0.85, wind: 0.35, thunder: 0 },
    rain: { rain: 0.7, snow: 0, fog: 0.35, cloudCoverage: 1, wind: 0.55, thunder: 0.15 },
    storm: { rain: 1, snow: 0, fog: 0.45, cloudCoverage: 1, wind: 1, thunder: 1 },
    snow: { rain: 0, snow: 0.8, fog: 0.4, cloudCoverage: 0.9, wind: 0.45, thunder: 0 }
};
export class HostAgentService {
    sessionId = randomUUID();
    config;
    socket;
    decoder = new FrameDecoder();
    statusServer;
    heartbeat;
    worldSimulationTimer;
    reconnect;
    stopped = true;
    authenticated = false;
    reconnectDelay;
    tick = 0;
    worldRevision = 0;
    gameReady = true;
    currentMap = '';
    players = new Map();
    worldEntities = new Map();
    dropRequests = new Map();
    pickupRequests = new Map();
    playerProgress = new Map();
    entityDespawnTombstones = new Map();
    worldStore;
    worldSaveTimer;
    worldSaveInFlight;
    worldDirty = false;
    worldPersistenceHealthy = true;
    worldPersistenceBlocked = false;
    latestWorldState;
    weatherCycleElapsedMs = 0;
    lastWorldStatePublishAt = 0;
    lastMovementRelayAt = new Map();
    lastPlayerSnapshotBroadcastAt = new Map();
    lastWorldSimulationAt = Date.now();
    nextZombieSpawnAt = 0;
    zombieSpawnSerial = 0;
    zombieAttackSequence = 0;
    nextZombieAttackAt = new Map();
    zombieAttackStateUntil = new Map();
    nextPlayerZombieDamageAt = new Map();
    lastPlayerZombieDamageAt = new Map();
    zombieCombatUntil = new Map();
    relocatedZombieEntities = new Set();
    configuredZombieRespawnAt = new Map();
    playerZombieRespawnPausedUntil = new Map();
    playerCombatSpawnSuppressedUntil = new Map();
    zombieCorpseExpiresAt = new Map();
    lastVehicleBroadcastAt = new Map();
    constructor(config) {
        this.config = config;
        if (config.hostSecret.length < 16)
            throw new Error('Host secret must be at least 16 characters');
        if (config.zombiesPerPlayer !== undefined &&
            (!Number.isSafeInteger(config.zombiesPerPlayer) ||
                config.zombiesPerPlayer < 1 ||
                config.zombiesPerPlayer > 16)) {
            throw new Error('zombiesPerPlayer must be an integer from 1 through 16');
        }
        if (config.zombieMaxCount !== undefined &&
            (!Number.isSafeInteger(config.zombieMaxCount) ||
                config.zombieMaxCount < 1 ||
                config.zombieMaxCount > 128)) {
            throw new Error('zombieMaxCount must be an integer from 1 through 128');
        }
        if (config.dayLengthMinutes !== undefined &&
            (!Number.isSafeInteger(config.dayLengthMinutes) ||
                config.dayLengthMinutes < 30 ||
                config.dayLengthMinutes > 720))
            throw new Error('dayLengthMinutes must be an integer from 30 through 720');
        if (config.weatherPreset !== undefined &&
            !(config.weatherPreset in WEATHER_PRESETS))
            throw new Error('weatherPreset is invalid');
        if (config.startTimeOfDay !== undefined &&
            (!Number.isSafeInteger(config.startTimeOfDay) ||
                config.startTimeOfDay < 0 || config.startTimeOfDay > 2399))
            throw new Error('startTimeOfDay must be an integer from 0 through 2399');
        if (config.timeMode === 'frozen' && config.startTimeOfDay === undefined) {
            throw new Error('frozen time requires startTimeOfDay');
        }
        if (config.weatherCycle !== undefined && (config.weatherCycle.length < 2 || config.weatherCycle.length > 8 ||
            config.weatherCycle.some((preset) => !(preset in WEATHER_PRESETS))))
            throw new Error('weatherCycle is invalid');
        if (config.weatherPhaseMinutes !== undefined &&
            (!Number.isSafeInteger(config.weatherPhaseMinutes) ||
                config.weatherPhaseMinutes < 1 || config.weatherPhaseMinutes > 120))
            throw new Error('weatherPhaseMinutes must be an integer from 1 through 120');
        if (config.weatherPreset && config.weatherCycle) {
            throw new Error('weatherPreset and weatherCycle are mutually exclusive');
        }
        this.reconnectDelay = config.reconnectMinMs ?? 100;
        this.gameReady = !config.readinessUrl && !config.gameDataDir;
        if (config.worldStatePath) {
            this.worldStore = new WorldSnapshotStore(config.worldStatePath, config.worldId);
        }
    }
    async start() {
        this.stopped = false;
        await this.restoreWorld();
        this.statusServer = createHttpServer((request, response) => {
            if (request.method === 'POST' &&
                request.url === '/shutdown' &&
                ['127.0.0.1', '::1', '::ffff:127.0.0.1'].includes(request.socket.remoteAddress ?? '')) {
                response.writeHead(202).end();
                setImmediate(() => void this.stop());
                return;
            }
            if (request.method !== 'GET' || request.url !== '/status') {
                response.writeHead(404).end();
                return;
            }
            const body = JSON.stringify(this.status());
            response.writeHead(200, {
                'content-type': 'application/json',
                'content-length': Buffer.byteLength(body),
                'cache-control': 'no-store'
            });
            response.end(body);
        });
        await new Promise((resolve, reject) => {
            this.statusServer.once('error', reject);
            this.statusServer.listen(this.config.statusPort ?? 0, this.config.statusHost ?? '127.0.0.1', resolve);
        });
        this.connect();
        this.heartbeat = setInterval(() => {
            if (this.config.readinessUrl || this.config.gameDataDir)
                void this.refreshReadiness();
            if (this.authenticated)
                this.send({
                    type: MessageType.HostHeartbeat,
                    flags: this.gameReady ? 1 : 0,
                    tick: ++this.tick,
                    payload: encodeString(JSON.stringify({
                        protocolVersion: 3,
                        buildId: this.config.buildId ?? 'unknown',
                        map: this.currentMap,
                        ready: this.gameReady
                    }), 512)
                });
            if (this.authenticated && this.gameReady) {
                this.publishWorldStateIfDue();
            }
        }, this.config.heartbeatMs ?? 500);
        this.heartbeat.unref();
        this.worldSimulationTimer = setInterval(() => {
            if (this.authenticated && this.gameReady)
                this.simulateWorld();
        }, Math.max(50, this.config.worldSimulationMs ?? 100));
        this.worldSimulationTimer.unref();
        if (this.worldStore) {
            this.worldSaveTimer = setInterval(() => void this.flushWorld(), Math.max(1_000, this.config.worldSaveIntervalMs ?? 5_000));
            this.worldSaveTimer.unref();
        }
        const statusPort = this.statusServer.address().port;
        log('info', 'host_agent_started', { sessionId: this.sessionId, worldId: this.config.worldId, statusPort });
        return { statusPort };
    }
    status() {
        return {
            ok: true,
            service: 'sdo-host-agent',
            protocolVersion: 3,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            gatewayConnected: Boolean(this.socket && !this.socket.destroyed),
            authenticated: this.authenticated,
            gameReady: this.gameReady,
            map: this.currentMap,
            buildId: this.config.buildId ?? 'unknown',
            playerCount: this.players.size,
            savedPlayerCount: this.playerProgress.size,
            worldEntityCount: this.worldEntities.size,
            zombieCount: [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Zombie).length,
            zombies: [...this.worldEntities.values()]
                .filter((entity) => entity.kind === WorldEntityKind.Zombie)
                .map((entity) => ({
                entityId: entity.entityId.toString(),
                health: entity.health,
                revision: entity.revision
            })),
            playerHealth: [...this.players.values()].map((player) => ({
                playerId: player.playerId.toString(),
                health: player.health,
                dead: player.dead
            })),
            groundItemCount: [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.GroundItem).length,
            vehicleCount: [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Vehicle).length,
            placedStructureCount: [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.PlacedStructure).length,
            worldRevision: this.worldRevision,
            worldPersistenceEnabled: Boolean(this.worldStore),
            worldPersistenceHealthy: this.worldPersistenceHealthy,
            worldPersistenceBlocked: this.worldPersistenceBlocked,
            dropRequestLedgerSize: this.dropRequests.size,
            pickupRequestLedgerSize: this.pickupRequests.size,
            latestWorldState: this.latestWorldState
                ? {
                    revision: this.latestWorldState.revision,
                    timeOfDay: this.latestWorldState.timeOfDay,
                    rain: this.latestWorldState.rain,
                    snow: this.latestWorldState.snow,
                    fog: this.latestWorldState.fog,
                    cloudCoverage: this.latestWorldState.cloudCoverage,
                    wind: this.latestWorldState.wind,
                    thunder: this.latestWorldState.thunder
                }
                : undefined,
            authority: {
                timeAndWeather: true,
                groundLoot: true,
                zombieSimulation: this.config.authoritativeZombie === true,
                zombieDamage: this.config.zombieDamageEnabled !== false
            },
            simulationTick: this.tick
        };
    }
    async waitUntilAuthenticated(timeoutMs = 5_000) {
        const started = Date.now();
        while (!this.authenticated) {
            if (Date.now() - started > timeoutMs)
                throw new Error('Timed out waiting for host authentication');
            await new Promise((resolve) => setTimeout(resolve, 20));
        }
    }
    disconnectTransportForTest() {
        this.socket?.destroy();
    }
    async stop() {
        this.stopped = true;
        this.authenticated = false;
        if (this.heartbeat)
            clearInterval(this.heartbeat);
        if (this.worldSimulationTimer)
            clearInterval(this.worldSimulationTimer);
        if (this.worldSaveTimer)
            clearInterval(this.worldSaveTimer);
        if (this.reconnect)
            clearTimeout(this.reconnect);
        await this.flushWorld(true);
        this.socket?.destroy();
        if (this.statusServer)
            await new Promise((resolve) => this.statusServer.close(() => resolve()));
        log('info', 'host_agent_stopped', { sessionId: this.sessionId, worldId: this.config.worldId });
    }
    connect() {
        if (this.stopped)
            return;
        const socket = createConnection({
            host: this.config.gatewayHost ?? '127.0.0.1',
            port: this.config.gatewayPort
        });
        this.socket = socket;
        this.decoder = new FrameDecoder();
        socket.setNoDelay(true);
        socket.setKeepAlive(true, 2_000);
        socket.once('connect', () => {
            this.reconnectDelay = this.config.reconnectMinMs ?? 100;
            socket.write(encodeFrame({
                type: MessageType.HostAuthenticate,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                payload: encodeString(this.config.hostSecret, 512)
            }));
        });
        socket.on('data', (chunk) => {
            try {
                const bytes = typeof chunk === 'string' ? Buffer.from(chunk) : chunk;
                for (const frame of this.decoder.push(bytes))
                    this.handle(frame);
            }
            catch (error) {
                log('error', 'host_protocol_error', { error: error instanceof Error ? error.message : 'unknown' });
                socket.destroy();
            }
        });
        socket.on('error', (error) => log('warning', 'host_gateway_error', { error: error.message }));
        socket.once('close', () => {
            if (this.socket !== socket)
                return;
            this.authenticated = false;
            if (this.stopped)
                return;
            // Gateway connection IDs cannot survive a gateway disconnect. Keeping
            // these entries would duplicate a player after their next admission.
            for (const player of this.players.values()) {
                this.capturePlayerProgress(player);
                log('info', 'host_player_removed', {
                    connectionId: player.connectionId,
                    playerId: player.playerId,
                    entityId: player.entityId,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId,
                    disconnectReason: 'gateway_disconnected'
                });
            }
            this.players.clear();
            void this.flushWorld(true);
            log('warning', 'host_gateway_disconnected', { sessionId: this.sessionId, worldId: this.config.worldId });
            this.scheduleReconnect();
        });
    }
    scheduleReconnect() {
        if (this.stopped || this.reconnect)
            return;
        const delay = this.reconnectDelay;
        this.reconnectDelay = Math.min(this.reconnectDelay * 2, this.config.reconnectMaxMs ?? 5_000);
        this.reconnect = setTimeout(() => {
            this.reconnect = undefined;
            this.connect();
        }, delay);
        this.reconnect.unref();
    }
    handle(frame) {
        if (this.stopped)
            return;
        if (frame.type === MessageType.AuthenticationAccepted) {
            this.authenticated = true;
            log('info', 'host_authenticated', { sessionId: this.sessionId, worldId: this.config.worldId });
            return;
        }
        if (frame.type === MessageType.AuthenticationRejected) {
            log('critical', 'host_authentication_rejected', { reason: decodeString(frame.payload, 128) });
            this.stopped = true;
            this.socket?.destroy();
            return;
        }
        if (!this.authenticated)
            throw new Error('Gateway message arrived before authentication');
        if (frame.type === MessageType.JoinRequest) {
            const request = decodeJoinRequest(frame.payload);
            if (!validSteamIdentity(request.playerKey)) {
                log('error', 'host_player_join_rejected_invalid_identity', {
                    connectionId: frame.connectionId,
                    playerId: frame.playerId,
                    worldId: this.config.worldId,
                    playerKey: request.playerKey
                });
                this.send({
                    type: MessageType.JoinRejected,
                    connectionId: frame.connectionId,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId,
                    playerId: frame.playerId,
                    entityId: 0n,
                    payload: encodeString('steam_identity_required', 128)
                });
                return;
            }
            const entityId = stableNumericId(`player-entity:${request.playerKey}`);
            const saved = this.playerProgress.get(request.playerKey);
            const player = {
                connectionId: frame.connectionId,
                playerId: frame.playerId,
                entityId,
                playerKey: request.playerKey,
                displayName: request.displayName,
                dead: saved?.dead ?? false,
                health: saved?.health ?? 100,
                profileRevision: saved?.profileRevision ?? 0,
                x: saved?.x,
                y: saved?.y,
                z: saved?.z,
                yaw: saved?.yaw,
                admittedAtMs: Date.now()
            };
            this.players.set(frame.connectionId, player);
            this.send({
                type: MessageType.JoinAccepted,
                connectionId: frame.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: frame.playerId,
                entityId,
                tick: ++this.tick,
                payload: encodeString(request.displayName, 64)
            });
            if (saved) {
                this.send({
                    type: MessageType.PlayerProgressRestore,
                    connectionId: frame.connectionId,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId,
                    playerId: frame.playerId,
                    entityId,
                    tick: ++this.tick,
                    sequence: saved.profileRevision,
                    payload: encodeMovement({
                        x: saved.x,
                        y: saved.y,
                        z: saved.z,
                        yaw: saved.yaw,
                        aimYaw: saved.yaw,
                        velocityX: 0,
                        velocityY: 0,
                        velocityZ: 0,
                        movementDirection: 0,
                        movementState: 0,
                        aimState: 0,
                        animationState: 0
                    })
                });
                log('info', 'host_player_progress_restore_sent', {
                    connectionId: frame.connectionId,
                    playerId: frame.playerId,
                    worldId: this.config.worldId,
                    saveRevision: saved.profileRevision
                });
            }
            log('info', 'host_player_admitted', {
                connectionId: frame.connectionId, playerId: frame.playerId, entityId,
                sessionId: this.sessionId, worldId: this.config.worldId
            });
            this.clearZombiesNearPlayer(player, 'player_join_clearance');
            if (this.latestWorldState)
                this.sendWorldState(this.latestWorldState);
            this.replayEntitiesToConnection(frame.connectionId, player);
            this.broadcastPositionedPlayerSnapshots(Date.now(), true);
            return;
        }
        if (frame.type === MessageType.PlayerDisconnected) {
            const player = this.players.get(frame.connectionId);
            this.players.delete(frame.connectionId);
            this.lastMovementRelayAt.delete(frame.connectionId);
            if (player) {
                this.capturePlayerProgress(player);
                void this.flushWorld(true);
                this.lastPlayerSnapshotBroadcastAt.delete(player.connectionId);
                const reason = decodeString(frame.payload, 128);
                // The host owns the player-to-entity mapping. Echo the authoritative
                // entity ID so every client can destroy the exact proxy even when an
                // older gateway emitted a zero-ID disconnect first.
                this.send({
                    type: MessageType.PlayerDisconnected,
                    connectionId: player.connectionId,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId,
                    playerId: player.playerId,
                    entityId: player.entityId,
                    tick: ++this.tick,
                    payload: encodeString(reason, 128)
                });
                log('info', 'host_player_removed', {
                    connectionId: player.connectionId, playerId: player.playerId, entityId: player.entityId,
                    sessionId: this.sessionId, worldId: this.config.worldId,
                    disconnectReason: reason
                });
            }
            return;
        }
        if (frame.type === MessageType.Movement) {
            const player = this.players.get(frame.connectionId);
            if (!player)
                return;
            const movement = decodeMovement(frame.payload);
            if (player.lastSequence !== undefined && !isNewerSequence(frame.sequence, player.lastSequence)) {
                log('debug', 'stale_movement_rejected', {
                    connectionId: player.connectionId, playerId: player.playerId,
                    sequence: frame.sequence, previousSequence: player.lastSequence
                });
                return;
            }
            player.lastSequence = frame.sequence;
            player.x = movement.x;
            player.y = movement.y;
            player.z = movement.z;
            player.yaw = movement.yaw;
            player.aimYaw = movement.aimYaw;
            player.velocityX = movement.velocityX;
            player.velocityY = movement.velocityY;
            player.velocityZ = movement.velocityZ;
            player.movementDirection = movement.movementDirection;
            player.movementState = movement.movementState;
            player.aimState = movement.aimState;
            player.animationState = movement.animationState;
            const inVehicle = movement.movementState >= MOVEMENT_STATE_IN_VEHICLE;
            player.mountedInVehicle = inVehicle;
            if (inVehicle && player.vehicleEntityId) {
                this.updateVehicleEntityFromMovement(player.vehicleEntityId, movement);
            }
            else if (!inVehicle && player.vehicleEntityId) {
                player.vehicleEntityId = undefined;
                player.mountedInVehicle = false;
            }
            this.capturePlayerProgress(player);
            const relayIntervalMs = Math.max(16, this.config.movementRelayIntervalMs ?? 20);
            const relayNow = Date.now();
            const lastRelayAt = this.lastMovementRelayAt.get(frame.connectionId) ?? 0;
            if (relayNow - lastRelayAt < relayIntervalMs) {
                return;
            }
            this.lastMovementRelayAt.set(frame.connectionId, relayNow);
            this.relayPlayerMovementSnapshot(player, movement, frame.sequence);
            return;
        }
        if (frame.type === MessageType.ItemDropRequest) {
            this.handleItemDropRequest(frame);
            return;
        }
        if (frame.type === MessageType.ItemPickupRequest) {
            this.handleItemPickupRequest(frame);
            return;
        }
        if (frame.type === MessageType.ZombieAttackRequest) {
            this.handleZombieAttackRequest(frame);
            return;
        }
        if (frame.type === MessageType.DeathRequest) {
            const player = this.players.get(frame.connectionId);
            if (!player || player.dead)
                return;
            player.dead = true;
            player.health = 0;
            player.profileRevision += 1;
            this.capturePlayerProgress(player);
            this.send({
                type: MessageType.Death,
                connectionId: player.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: player.playerId,
                entityId: player.entityId,
                tick: ++this.tick,
                sequence: player.profileRevision,
                payload: encodeString('host_confirmed_lifecycle_death', 128)
            });
            log('info', 'host_death_confirmed', {
                connectionId: player.connectionId, playerId: player.playerId,
                entityId: player.entityId, sessionId: this.sessionId,
                worldId: this.config.worldId, profileRevision: player.profileRevision
            });
            return;
        }
        if (frame.type === MessageType.RespawnRequest) {
            const player = this.players.get(frame.connectionId);
            if (!player || !player.dead)
                return;
            player.dead = false;
            player.health = 100;
            player.profileRevision += 1;
            player.admittedAtMs = Date.now();
            this.capturePlayerProgress(player);
            this.clearZombiesNearPlayer(player, 'player_respawn_clearance');
            this.send({
                type: MessageType.Respawn,
                connectionId: player.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: player.playerId,
                entityId: player.entityId,
                tick: ++this.tick,
                sequence: player.profileRevision,
                payload: frame.payload
            });
            log('info', 'host_respawn_confirmed', {
                connectionId: player.connectionId, playerId: player.playerId,
                entityId: player.entityId, sessionId: this.sessionId,
                worldId: this.config.worldId, profileRevision: player.profileRevision
            });
            return;
        }
        if (frame.type === MessageType.InteractionRequest) {
            this.handleInteractionRequest(frame);
            return;
        }
        if ([
            MessageType.Equipment,
            MessageType.ProfileRevision
        ].includes(frame.type)) {
            const player = this.players.get(frame.connectionId);
            if (!player)
                return;
            this.send({
                ...frame,
                entityId: player.entityId,
                tick: ++this.tick
            });
            return;
        }
        throw new Error(`Unsupported gateway message ${frame.type}`);
    }
    sendWorldState(state) {
        this.send({
            type: MessageType.WorldState,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            tick: ++this.tick,
            sequence: state.revision,
            payload: encodeWorldState(state)
        });
    }
    createInitialWorldState(now = Date.now()) {
        const presets = this.weatherPresets();
        const phaseDurationMs = (this.config.weatherPhaseMinutes ?? 10) * 60_000;
        const cycleDurationMs = phaseDurationMs * presets.length;
        this.weatherCycleElapsedMs = now % cycleDurationMs;
        const cyclePosition = this.weatherCycleElapsedMs / phaseDurationMs;
        const phase = Math.floor(cyclePosition) % presets.length;
        const blend = this.config.weatherTransition === 'instant'
            ? 0
            : cyclePosition - Math.floor(cyclePosition);
        const from = presets[phase];
        const to = presets[(phase + 1) % presets.length];
        const interpolate = (start, end) => start + (end - start) * blend;
        const weather = this.config.weatherPreset
            ? WEATHER_PRESETS[this.config.weatherPreset]
            : {
                rain: interpolate(from.rain, to.rain),
                snow: interpolate(from.snow, to.snow),
                fog: interpolate(from.fog, to.fog),
                cloudCoverage: interpolate(from.cloudCoverage, to.cloudCoverage),
                wind: interpolate(from.wind, to.wind),
                thunder: interpolate(from.thunder, to.thunder)
            };
        const dayDurationMs = (this.config.dayLengthMinutes ?? 120) * 60_000;
        return {
            revision: this.worldRevision >>> 0,
            authorityTimeMs: BigInt(now),
            timeOfDay: this.config.startTimeOfDay ??
                (now % dayDurationMs) / dayDurationMs * 2400,
            ...weather
        };
    }
    advanceWorldState(elapsedMs) {
        const safeElapsedMs = Math.max(0, Math.min(30_000, elapsedMs));
        if (!this.latestWorldState) {
            this.latestWorldState = this.createInitialWorldState();
            this.markWorldDirty();
            return;
        }
        if (safeElapsedMs === 0)
            return;
        this.latestWorldState.authorityTimeMs += BigInt(Math.round(safeElapsedMs));
        const dayDurationMs = (this.config.dayLengthMinutes ?? 120) * 60_000;
        this.latestWorldState.timeOfDay = this.config.timeMode === 'frozen'
            ? this.config.startTimeOfDay
            : (this.latestWorldState.timeOfDay + safeElapsedMs * (2400 / dayDurationMs)) % 2400;
        const presets = this.weatherPresets();
        const phaseDurationMs = (this.config.weatherPhaseMinutes ?? 10) * 60_000;
        const cycleDurationMs = phaseDurationMs * presets.length;
        this.weatherCycleElapsedMs = (this.weatherCycleElapsedMs + safeElapsedMs) % cycleDurationMs;
        const cyclePosition = this.weatherCycleElapsedMs / phaseDurationMs;
        const phase = Math.floor(cyclePosition) % presets.length;
        const blend = this.config.weatherTransition === 'instant'
            ? 0
            : cyclePosition - Math.floor(cyclePosition);
        const from = presets[phase];
        const to = presets[(phase + 1) % presets.length];
        const interpolate = (start, end) => start + (end - start) * blend;
        const weather = this.config.weatherPreset
            ? WEATHER_PRESETS[this.config.weatherPreset]
            : {
                rain: interpolate(from.rain, to.rain),
                snow: interpolate(from.snow, to.snow),
                fog: interpolate(from.fog, to.fog),
                cloudCoverage: interpolate(from.cloudCoverage, to.cloudCoverage),
                wind: interpolate(from.wind, to.wind),
                thunder: interpolate(from.thunder, to.thunder)
            };
        Object.assign(this.latestWorldState, weather);
        this.markWorldDirty();
    }
    weatherPresets() {
        return this.config.weatherCycle
            ? this.config.weatherCycle.map((preset) => WEATHER_PRESETS[preset])
            : Object.values(WEATHER_PRESETS);
    }
    publishWorldStateIfDue(force = false) {
        const now = Date.now();
        if (!this.latestWorldState) {
            this.latestWorldState = this.createInitialWorldState(now);
            this.markWorldDirty();
        }
        const intervalMs = Math.max(250, this.config.worldStatePublishIntervalMs ?? 500);
        if (!force && now - this.lastWorldStatePublishAt < intervalMs)
            return;
        this.lastWorldStatePublishAt = now;
        this.worldRevision = (this.worldRevision + 1) >>> 0;
        const state = {
            ...this.latestWorldState,
            revision: this.worldRevision
        };
        this.latestWorldState = state;
        this.sendWorldState(state);
        this.replayEntityDespawnTombstones(now);
        this.markWorldDirty();
    }
    replayEntitiesToConnection(connectionId, player) {
        const now = Date.now();
        for (const entity of this.worldEntities.values()) {
            if (player && !this.shouldReplayEntityToPlayer(entity, player, now)) {
                continue;
            }
            this.sendEntitySpawn(entity, connectionId);
            this.sendEntityState(entity, connectionId);
        }
    }
    broadcastDynamicWorldEntities(now) {
        for (const entity of this.worldEntities.values()) {
            if (entity.kind !== WorldEntityKind.Vehicle || entity.health <= 0) {
                continue;
            }
            const lastAt = this.lastVehicleBroadcastAt.get(entity.entityId) ?? 0;
            if (now - lastAt < 100) {
                continue;
            }
            this.lastVehicleBroadcastAt.set(entity.entityId, now);
            this.broadcastEntityState(entity);
        }
    }
    shouldReplayEntityToPlayer(entity, player, now) {
        if (player.x === undefined ||
            player.y === undefined ||
            player.z === undefined) {
            return entity.kind !== WorldEntityKind.GroundItem;
        }
        const positioned = player;
        if (entity.kind === WorldEntityKind.Zombie) {
            return this.playerWithin(player, entity, CLIENT_ZOMBIE_INTEREST_RADIUS);
        }
        if (entity.kind === WorldEntityKind.GroundItem) {
            return this.playerWithin(player, entity, CLIENT_GROUND_ITEM_INTEREST_RADIUS);
        }
        // Persistent world props must replay at any distance so late joiners and
        // distant bases stay visible to every client.
        if (entity.kind === WorldEntityKind.Vehicle ||
            entity.kind === WorldEntityKind.PlacedStructure) {
            return true;
        }
        return true;
    }
    broadcastEntitySpawn(entity) {
        this.sendEntitySpawn(entity, 0n);
    }
    broadcastEntityState(entity) {
        this.sendEntityState(entity, 0n);
    }
    sendEntitySpawn(entity, connectionId = 0n) {
        this.send({
            type: MessageType.EntitySpawn,
            connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            entityId: entity.entityId,
            sequence: entity.revision,
            tick: ++this.tick,
            payload: encodeWorldEntityDescriptor({
                kind: entity.kind,
                revision: entity.revision,
                classPath: entity.classPath,
                itemId: entity.itemId ?? '',
                quantity: entity.quantity ?? 0,
                ownerPlayerId: entity.ownerPlayerId ?? 0n
            })
        });
    }
    sendEntityState(entity, connectionId = 0n) {
        this.send({
            type: MessageType.EntityState,
            connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            entityId: entity.entityId,
            sequence: entity.revision,
            tick: ++this.tick,
            payload: encodeWorldEntityState(entity)
        });
    }
    sendEntityDespawn(tombstone) {
        this.send({
            type: MessageType.EntityDespawn,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: tombstone.playerId,
            entityId: tombstone.entityId,
            sequence: tombstone.revision,
            tick: ++this.tick,
            payload: encodeWorldAction({
                reason: 'picked_up',
                requestId: tombstone.requestId
            })
        });
    }
    replayEntityDespawnTombstones(now) {
        for (const [entityId, tombstone] of this.entityDespawnTombstones) {
            if (tombstone.expiresAtMs <= now) {
                this.entityDespawnTombstones.delete(entityId);
                continue;
            }
            this.sendEntityDespawn(tombstone);
        }
    }
    sendZombiePlayerDamage(zombie, player, damage) {
        const attackSequence = ++this.zombieAttackSequence >>> 0;
        player.health = Math.max(0, player.health - damage);
        this.send({
            type: MessageType.PlayerDamage,
            connectionId: player.connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: player.playerId,
            entityId: zombie.entityId,
            sequence: attackSequence,
            tick: ++this.tick,
            payload: encodePlayerDamage({
                damage,
                health: player.health,
                attackSequence
            })
        });
        log('info', 'host_zombie_player_damage', {
            entityId: zombie.entityId,
            playerId: player.playerId,
            damage,
            health: player.health,
            attackSequence,
            sessionId: this.sessionId,
            worldId: this.config.worldId
        });
        if (player.health > 0 || player.dead)
            return;
        player.dead = true;
        player.profileRevision += 1;
        this.send({
            type: MessageType.Death,
            connectionId: player.connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: player.playerId,
            entityId: player.entityId,
            tick: ++this.tick,
            sequence: player.profileRevision,
            payload: encodeString('host_confirmed_zombie_death', 128)
        });
        log('info', 'host_zombie_player_death', {
            connectionId: player.connectionId,
            playerId: player.playerId,
            entityId: player.entityId,
            zombieEntityId: zombie.entityId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            profileRevision: player.profileRevision
        });
    }
    explorationEntityId(playerId, slot) {
        return stableNumericId(`zombie:${this.config.worldId}:explore:${playerId}:${slot}`);
    }
    explorationSlotFromEntityId(entityId) {
        for (const player of this.players.values()) {
            for (let slot = 0; slot < 8; slot += 1) {
                if (this.explorationEntityId(player.playerId, slot) === entityId) {
                    return { playerId: player.playerId, slot };
                }
            }
        }
        return undefined;
    }
    mapSpawnerEntityId(index) {
        return stableNumericId(`zombie:${this.config.worldId}:map-spawner:${index}`);
    }
    mapSpawnerIndexFromEntityId(entityId) {
        const catalog = this.config.zombieSpawnPoints ?? [];
        for (let index = 0; index < catalog.length; index += 1) {
            if (this.mapSpawnerEntityId(index) === entityId)
                return index;
        }
        return undefined;
    }
    nearestPlayerDistanceSquared(point, players) {
        let nearest = Number.POSITIVE_INFINITY;
        for (const player of players) {
            const dx = player.x - point.x;
            const dy = player.y - point.y;
            const dz = player.z - point.z;
            nearest = Math.min(nearest, dx * dx + dy * dy + dz * dz);
        }
        return nearest;
    }
    joinGraceMs() {
        return Math.max(0, this.config.zombieJoinGraceMs ?? ZOMBIE_JOIN_GRACE_MS);
    }
    zombieRespawnDelayMs() {
        return Math.max(60_000, this.config.zombieRespawnMs ?? ZOMBIE_RESPAWN_AFTER_DEATH_MS);
    }
    playerZombieSpawnsPaused(playerId, now) {
        return now < (this.playerZombieRespawnPausedUntil.get(playerId) ?? 0);
    }
    playerCombatSpawnsSuppressed(playerId, now) {
        return now < (this.playerCombatSpawnSuppressedUntil.get(playerId) ?? 0);
    }
    suppressPlayerCombatSpawns(playerId, now, durationMs = ZOMBIE_COMBAT_SPAWN_SUPPRESSION_MS) {
        const resumeAt = now + Math.max(30_000, durationMs);
        const previous = this.playerCombatSpawnSuppressedUntil.get(playerId) ?? 0;
        if (resumeAt > previous) {
            this.playerCombatSpawnSuppressedUntil.set(playerId, resumeAt);
        }
    }
    anyPlayerCombatSpawnsSuppressed(players, now) {
        return players.some((player) => this.playerCombatSpawnsSuppressed(player.playerId, now));
    }
    canPlaceZombieNearPlayers(point, players, now, options) {
        const mode = options?.mode ?? 'dynamic';
        if (mode === 'map_spawner') {
            return true;
        }
        const minDistance = Math.max(0, options?.minDistance ?? ZOMBIE_MIN_SPAWN_DISTANCE_FROM_PLAYER);
        const minimumSquared = minDistance * minDistance;
        for (const player of players) {
            if (player.dead)
                continue;
            const dx = player.x - point.x;
            const dy = player.y - point.y;
            const dz = player.z - point.z;
            if (dx * dx + dy * dy + dz * dz < minimumSquared) {
                return false;
            }
            if (this.isWithinPlayerSpawnExclusion(point, player, now)) {
                return false;
            }
        }
        return true;
    }
    isWithinZombiePopInGuard(point, player) {
        const dx = player.x - point.x;
        const dy = player.y - point.y;
        const dz = player.z - point.z;
        const guard = ZOMBIE_SPAWN_POP_IN_GUARD;
        return dx * dx + dy * dy + dz * dz < guard * guard;
    }
    notePlayerZombieCombat(playerId, now) {
        this.suppressPlayerCombatSpawns(playerId, now);
    }
    anyPlayerZombieSpawnsPaused(players, now) {
        return players.some((player) => this.playerZombieSpawnsPaused(player.playerId, now));
    }
    recordZombieDeathRespawnCooldown(zombie, players, now, corpseTtlMs) {
        const respawnAt = now + corpseTtlMs + this.zombieRespawnDelayMs();
        this.configuredZombieRespawnAt.set(zombie.entityId, respawnAt);
        this.nextZombieSpawnAt = Math.max(this.nextZombieSpawnAt, respawnAt);
        let nearestPlayerId;
        let nearestDistanceSquared = Number.POSITIVE_INFINITY;
        for (const player of players) {
            if (player.dead)
                continue;
            const dx = player.x - zombie.x;
            const dy = player.y - zombie.y;
            const dz = player.z - zombie.z;
            const distanceSquared = dx * dx + dy * dy + dz * dz;
            if (distanceSquared < nearestDistanceSquared) {
                nearestPlayerId = player.playerId;
                nearestDistanceSquared = distanceSquared;
            }
        }
        if (nearestPlayerId !== undefined &&
            nearestDistanceSquared <= ZOMBIE_CHASE_RADIUS * ZOMBIE_CHASE_RADIUS) {
            this.playerZombieRespawnPausedUntil.set(nearestPlayerId, respawnAt);
        }
        for (const player of players) {
            if (player.dead)
                continue;
            const dx = player.x - zombie.x;
            const dy = player.y - zombie.y;
            const dz = player.z - zombie.z;
            if (dx * dx + dy * dy + dz * dz <= ZOMBIE_CHASE_RADIUS * ZOMBIE_CHASE_RADIUS) {
                this.suppressPlayerCombatSpawns(player.playerId, now, respawnAt - now);
            }
        }
    }
    enforceAuthoritativeZombiePlacement(configuredSpawnPoints, activePlayers, now) {
        void configuredSpawnPoints;
        for (const zombie of [...this.worldEntities.values()]) {
            if (zombie.kind !== WorldEntityKind.Zombie || zombie.health <= 0)
                continue;
            if (zombie.state === 1 || zombie.state === 4)
                continue;
            if (this.isZombieInActiveCombat(zombie.entityId, now))
                continue;
            const spawnerIndex = this.mapSpawnerIndexFromEntityId(zombie.entityId);
            if (spawnerIndex !== undefined) {
                continue;
            }
            if (!activePlayers.some((player) => this.isWithinZombiePopInGuard(zombie, player))) {
                continue;
            }
            const explore = this.explorationSlotFromEntityId(zombie.entityId);
            if (explore) {
                this.worldEntities.delete(zombie.entityId);
                this.send({
                    type: MessageType.EntityDespawn,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId,
                    entityId: zombie.entityId,
                    sequence: (zombie.revision + 1) >>> 0,
                    tick: ++this.tick,
                    payload: encodeWorldAction({ reason: 'spawn_pop_in_violation' })
                });
                this.markWorldDirty();
                log('warning', 'host_zombie_removed_for_spawn_pop_in', {
                    entityId: zombie.entityId,
                    x: zombie.x,
                    y: zombie.y,
                    z: zombie.z,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId
                });
            }
        }
    }
    playerSpawnExclusionRadius(player, now) {
        if (player.admittedAtMs !== undefined &&
            now - player.admittedAtMs < this.joinGraceMs()) {
            return ZOMBIE_JOIN_EXCLUSION_RADIUS;
        }
        return ZOMBIE_PLAYER_EXCLUSION_RADIUS;
    }
    isWithinPlayerSpawnExclusion(point, player, now) {
        const dx = player.x - point.x;
        const dy = player.y - point.y;
        const dz = player.z - point.z;
        const exclusion = this.playerSpawnExclusionRadius(player, now);
        return dx * dx + dy * dy + dz * dz < exclusion * exclusion;
    }
    isWithinAnyPlayerSpawnExclusion(point, players, now) {
        return players.some((player) => this.isWithinPlayerSpawnExclusion(point, player, now));
    }
    isZombieNearAnyLivingPlayer(zombie, players, radius = ZOMBIE_COMBAT_LEASH_RADIUS) {
        const radiusSquared = radius * radius;
        for (const player of players) {
            if (player.dead)
                continue;
            const dx = player.x - zombie.x;
            const dy = player.y - zombie.y;
            const dz = player.z - zombie.z;
            if (dx * dx + dy * dy + dz * dz <= radiusSquared) {
                return true;
            }
        }
        return false;
    }
    isZombieInActiveCombat(entityId, now) {
        return now < (this.zombieCombatUntil.get(entityId) ?? 0);
    }
    reconcileZombieCombatPosition(entity, player) {
        const dx = entity.x - player.x;
        const dy = entity.y - player.y;
        const planarDistance = Math.hypot(dx, dy);
        if (planarDistance <= 500) {
            entity.z = player.z;
            return;
        }
        const desiredDistance = Math.min(planarDistance, 250);
        if (planarDistance > 0) {
            const scale = desiredDistance / planarDistance;
            entity.x = player.x + dx * scale;
            entity.y = player.y + dy * scale;
        }
        entity.z = player.z;
    }
    playerForwardVector(player) {
        const speedSquared = (player.velocityX ?? 0) ** 2 +
            (player.velocityY ?? 0) ** 2;
        if (speedSquared >= 25) {
            const speed = Math.sqrt(speedSquared);
            return {
                x: (player.velocityX ?? 0) / speed,
                y: (player.velocityY ?? 0) / speed
            };
        }
        const yawRadians = ((player.yaw ?? 0) * Math.PI) / 180;
        return {
            x: Math.cos(yawRadians),
            y: Math.sin(yawRadians)
        };
    }
    spawnerSelectionScore(point, player, now) {
        const dx = point.x - player.x;
        const dy = point.y - player.y;
        const dz = point.z - player.z;
        const distanceSquared = dx * dx + dy * dy + dz * dz;
        const exclusion = this.playerSpawnExclusionRadius(player, now);
        if (distanceSquared < exclusion * exclusion) {
            return Number.NEGATIVE_INFINITY;
        }
        const distance = Math.sqrt(distanceSquared);
        const aheadMin = ZOMBIE_SPAWN_AHEAD_MIN_RADIUS;
        const aheadMax = ZOMBIE_SPAWN_AHEAD_MAX_RADIUS;
        let bandScore = 0;
        if (distance >= aheadMin && distance <= aheadMax) {
            const midpoint = (aheadMin + aheadMax) / 2;
            bandScore = 1_000 - Math.abs(distance - midpoint);
        }
        else if (distance < aheadMin) {
            bandScore = distance - aheadMin;
        }
        else {
            bandScore = aheadMax - distance;
        }
        const planarDistance = Math.hypot(dx, dy);
        if (planarDistance <= 0)
            return bandScore;
        const forward = this.playerForwardVector(player);
        const aheadDot = (dx / planarDistance) * forward.x + (dy / planarDistance) * forward.y;
        const aheadScore = aheadDot >= 0.25 ? aheadDot * 2_500 : aheadDot * 900;
        return bandScore + aheadScore - distance * 0.05;
    }
    clearZombiesNearPlayer(player, reason) {
        if (player.x === undefined || player.y === undefined || player.z === undefined) {
            return;
        }
        const positioned = player;
        const now = Date.now();
        for (const zombie of [...this.worldEntities.values()]) {
            if (zombie.kind !== WorldEntityKind.Zombie || zombie.health <= 0)
                continue;
            if (!this.isWithinPlayerSpawnExclusion(zombie, positioned, now))
                continue;
            this.worldEntities.delete(zombie.entityId);
            this.send({
                type: MessageType.EntityDespawn,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                entityId: zombie.entityId,
                sequence: (zombie.revision + 1) >>> 0,
                tick: ++this.tick,
                payload: encodeWorldAction({ reason })
            });
            this.markWorldDirty();
            log('info', 'host_zombie_spawn_clearance', {
                entityId: zombie.entityId,
                playerId: player.playerId,
                reason,
                sessionId: this.sessionId,
                worldId: this.config.worldId
            });
        }
    }
    relayPlayerMovementSnapshot(player, movement, sequence) {
        this.send({
            type: MessageType.Movement,
            connectionId: player.connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: player.playerId,
            entityId: player.entityId,
            sequence,
            tick: ++this.tick,
            payload: encodeMovement(movement)
        });
    }
    broadcastPositionedPlayerSnapshots(now, force = false) {
        if (this.players.size < 2)
            return;
        for (const player of this.players.values()) {
            if (player.dead ||
                player.x === undefined ||
                player.y === undefined ||
                player.z === undefined) {
                continue;
            }
            const last = this.lastPlayerSnapshotBroadcastAt.get(player.connectionId) ?? 0;
            if (!force && now - last < PLAYER_SNAPSHOT_REBROADCAST_MS)
                continue;
            this.lastPlayerSnapshotBroadcastAt.set(player.connectionId, now);
            this.relayPlayerMovementSnapshot(player, {
                x: player.x,
                y: player.y,
                z: player.z,
                yaw: player.yaw ?? 0,
                aimYaw: player.aimYaw ?? player.yaw ?? 0,
                velocityX: player.velocityX ?? 0,
                velocityY: player.velocityY ?? 0,
                velocityZ: player.velocityZ ?? 0,
                movementDirection: player.movementDirection ?? 0,
                movementState: player.movementState ??
                    (player.mountedInVehicle ? MOVEMENT_STATE_IN_VEHICLE : 0),
                aimState: player.aimState ?? 0,
                animationState: player.animationState ?? 0
            }, player.lastSequence ?? 0);
        }
    }
    simulateWorld() {
        const now = Date.now();
        const elapsedSeconds = Math.min(1, Math.max(0, (now - this.lastWorldSimulationAt) / 1000));
        const elapsedMs = Math.max(0, now - this.lastWorldSimulationAt);
        this.lastWorldSimulationAt = now;
        this.broadcastPositionedPlayerSnapshots(now);
        this.advanceWorldState(elapsedMs);
        this.pruneStaleGroundItems(now);
        this.pruneExpiredZombieCorpses(now);
        this.broadcastDynamicWorldEntities(now);
        this.publishWorldStateIfDue();
        if (!this.config.authoritativeZombie)
            return;
        const activePlayers = [...this.players.values()].filter((player) => player.x !== undefined && player.y !== undefined && player.z !== undefined && !player.dead);
        const configuredSpawnPoints = this.config.zombieSpawnPoints ?? [];
        let liveZombies = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Zombie && entity.health > 0);
        this.enforceAuthoritativeZombiePlacement(configuredSpawnPoints, activePlayers, now);
        liveZombies = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Zombie && entity.health > 0);
        const spawnerActivationRadius = Math.max(CLIENT_ZOMBIE_INTEREST_RADIUS, this.config.zombieSpawnerActivationRadius ?? 18_000);
        const activationRadiusSquared = spawnerActivationRadius ** 2;
        const deactivationRadiusSquared = (spawnerActivationRadius + ZOMBIE_SPAWNER_DEACTIVATION_MARGIN) ** 2;
        const perPlayerSpawnerBudget = Math.max(4, Math.min(16, Math.floor((this.config.zombiesPerPlayer ?? 12) *
            (activePlayers.length >= 8
                ? 0.5
                : activePlayers.length >= 5
                    ? 0.65
                    : 1))));
        const maxActiveSpawners = Math.min(Math.max(64, this.config.zombieMaxCount ?? 128), Math.max(perPlayerSpawnerBudget, activePlayers.length * perPlayerSpawnerBudget));
        const interestRadiusSquared = CLIENT_ZOMBIE_INTEREST_RADIUS ** 2;
        const explorationTarget = Math.min(12, Math.max(EXPLORATION_ZOMBIE_TARGET, Math.floor((this.config.zombiesPerPlayer ?? 12) / 2)));
        if (configuredSpawnPoints.length > 0) {
            for (const zombie of liveZombies) {
                if (this.mapSpawnerIndexFromEntityId(zombie.entityId) !== undefined ||
                    this.explorationSlotFromEntityId(zombie.entityId) !== undefined) {
                    continue;
                }
                this.worldEntities.delete(zombie.entityId);
                this.send({
                    type: MessageType.EntityDespawn,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId,
                    entityId: zombie.entityId,
                    sequence: (zombie.revision + 1) >>> 0,
                    tick: ++this.tick,
                    payload: encodeWorldAction({ reason: 'legacy_zombie_population_removed' })
                });
                this.markWorldDirty();
            }
            liveZombies = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Zombie && entity.health > 0);
            if (activePlayers.length > 0) {
                for (const player of activePlayers) {
                    if (player.admittedAtMs !== undefined &&
                        now - player.admittedAtMs < this.joinGraceMs()) {
                        this.clearZombiesNearPlayer(player, 'player_join_grace_clearance');
                    }
                }
                liveZombies = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Zombie && entity.health > 0);
                const allowedSpawners = new Set();
                for (const player of activePlayers) {
                    const inJoinGrace = player.admittedAtMs !== undefined &&
                        now - player.admittedAtMs < this.joinGraceMs();
                    const playerBudget = inJoinGrace
                        ? Math.min(8, perPlayerSpawnerBudget)
                        : perPlayerSpawnerBudget;
                    const nearestForPlayer = configuredSpawnPoints
                        .map((point, index) => ({
                        index,
                        point,
                        distanceSquared: (player.x - point.x) ** 2 +
                            (player.y - point.y) ** 2 +
                            (player.z - point.z) ** 2
                    }))
                        .filter((entry) => entry.distanceSquared <= activationRadiusSquared &&
                        this.canPlaceZombieNearPlayers(entry.point, [player], now, {
                            mode: 'map_spawner'
                        }))
                        .sort((left, right) => this.spawnerSelectionScore(right.point, player, now) -
                        this.spawnerSelectionScore(left.point, player, now))
                        .slice(0, playerBudget);
                    for (const entry of nearestForPlayer) {
                        allowedSpawners.add(entry.index);
                    }
                }
                for (const zombie of liveZombies) {
                    const spawnerIndex = this.mapSpawnerIndexFromEntityId(zombie.entityId);
                    if (spawnerIndex === undefined || allowedSpawners.has(spawnerIndex)) {
                        continue;
                    }
                    const point = configuredSpawnPoints[spawnerIndex];
                    if (!point) {
                        continue;
                    }
                    if (this.nearestPlayerDistanceSquared(point, activePlayers) <=
                        deactivationRadiusSquared) {
                        allowedSpawners.add(spawnerIndex);
                    }
                }
                let activeSpawnerIndices = [...allowedSpawners];
                if (activeSpawnerIndices.length > maxActiveSpawners) {
                    activeSpawnerIndices = configuredSpawnPoints
                        .map((point, index) => ({
                        index,
                        distanceSquared: this.nearestPlayerDistanceSquared(point, activePlayers)
                    }))
                        .filter((entry) => allowedSpawners.has(entry.index))
                        .sort((left, right) => left.distanceSquared - right.distanceSquared)
                        .slice(0, maxActiveSpawners)
                        .map((entry) => entry.index);
                }
                const allowedSpawnerSet = new Set(activeSpawnerIndices);
                for (const zombie of liveZombies) {
                    const spawnerIndex = this.mapSpawnerIndexFromEntityId(zombie.entityId);
                    if (spawnerIndex === undefined || allowedSpawnerSet.has(spawnerIndex)) {
                        continue;
                    }
                    if (this.isZombieNearAnyLivingPlayer(zombie, activePlayers)) {
                        continue;
                    }
                    if (this.isZombieInActiveCombat(zombie.entityId, now)) {
                        continue;
                    }
                    this.worldEntities.delete(zombie.entityId);
                    this.send({
                        type: MessageType.EntityDespawn,
                        sessionId: this.sessionId,
                        worldId: this.config.worldId,
                        entityId: zombie.entityId,
                        sequence: (zombie.revision + 1) >>> 0,
                        tick: ++this.tick,
                        payload: encodeWorldAction({ reason: 'map_spawner_deactivated' })
                    });
                    this.markWorldDirty();
                    log('info', 'host_map_spawner_zombie_despawned', {
                        entityId: zombie.entityId,
                        spawnerIndex,
                        sessionId: this.sessionId,
                        worldId: this.config.worldId
                    });
                }
                for (const index of activeSpawnerIndices) {
                    const point = configuredSpawnPoints[index];
                    if (!this.canPlaceZombieNearPlayers(point, activePlayers, now, {
                        mode: 'map_spawner'
                    })) {
                        continue;
                    }
                    if (activePlayers.some((player) => this.playerZombieSpawnsPaused(player.playerId, now) ||
                        this.playerCombatSpawnsSuppressed(player.playerId, now))) {
                        continue;
                    }
                    const entityId = this.mapSpawnerEntityId(index);
                    if (this.worldEntities.has(entityId) ||
                        now < (this.configuredZombieRespawnAt.get(entityId) ?? 0)) {
                        continue;
                    }
                    const zombie = {
                        entityId,
                        kind: WorldEntityKind.Zombie,
                        revision: 1,
                        x: point.x,
                        y: point.y,
                        z: point.z,
                        yaw: point.yaw ?? 0,
                        health: 100,
                        state: 0,
                        classPath: point.classPath ??
                            this.config.zombieClassPath ??
                            '/Game/AI/Zombies/Roamer/BP_Zombie_Roamer.BP_Zombie_Roamer_C'
                    };
                    this.worldEntities.set(entityId, zombie);
                    this.configuredZombieRespawnAt.delete(entityId);
                    this.broadcastEntitySpawn(zombie);
                    this.broadcastEntityState(zombie);
                    this.markWorldDirty();
                    log('info', 'host_map_spawner_zombie_spawned', {
                        entityId,
                        spawnerIndex: index,
                        x: point.x,
                        y: point.y,
                        z: point.z,
                        sessionId: this.sessionId,
                        worldId: this.config.worldId
                    });
                }
                for (const zombie of liveZombies) {
                    const explore = this.explorationSlotFromEntityId(zombie.entityId);
                    if (!explore)
                        continue;
                    const owner = activePlayers.find((player) => player.playerId === explore.playerId);
                    if (!owner)
                        continue;
                    const dx = owner.x - zombie.x;
                    const dy = owner.y - zombie.y;
                    const dz = owner.z - zombie.z;
                    if (dx * dx + dy * dy + dz * dz <= activationRadiusSquared)
                        continue;
                    if (this.isZombieNearAnyLivingPlayer(zombie, activePlayers))
                        continue;
                    if (this.isZombieInActiveCombat(zombie.entityId, now))
                        continue;
                    this.worldEntities.delete(zombie.entityId);
                    this.send({
                        type: MessageType.EntityDespawn,
                        sessionId: this.sessionId,
                        worldId: this.config.worldId,
                        entityId: zombie.entityId,
                        sequence: (zombie.revision + 1) >>> 0,
                        tick: ++this.tick,
                        payload: encodeWorldAction({ reason: 'exploration_zombie_deactivated' })
                    });
                    this.markWorldDirty();
                }
                liveZombies = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Zombie && entity.health > 0);
                const explorationClasses = [
                    '/Game/AI/Zombies/Roamer/BP_Zombie_Roamer.BP_Zombie_Roamer_C',
                    '/Game/AI/Zombies/Crawler/BP_Zombie_Crawler.BP_Zombie_Crawler_C',
                    '/Game/AI/Zombies/Infected/BP_Zombie_Infected.BP_Zombie_Infected_C',
                    '/Game/AI/Zombies/Radiated/BP_Zombie_Radiated.BP_Zombie_Radiated_C'
                ];
                if (this.config.zombieExplorationFill === true) {
                    for (const player of activePlayers) {
                        const inJoinGrace = player.admittedAtMs !== undefined &&
                            now - player.admittedAtMs < this.joinGraceMs();
                        if (inJoinGrace)
                            continue;
                        if (this.playerZombieSpawnsPaused(player.playerId, now))
                            continue;
                        if (this.playerCombatSpawnsSuppressed(player.playerId, now))
                            continue;
                        const nearbyCount = liveZombies.filter((zombie) => {
                            const dx = player.x - zombie.x;
                            const dy = player.y - zombie.y;
                            const dz = player.z - zombie.z;
                            return dx * dx + dy * dy + dz * dz <= interestRadiusSquared;
                        }).length;
                        if (nearbyCount >= explorationTarget)
                            continue;
                        const needed = Math.min(explorationTarget - nearbyCount, 4);
                        const forward = this.playerForwardVector(player);
                        for (let slot = 0; slot < needed; slot += 1) {
                            const entityId = this.explorationEntityId(player.playerId, slot);
                            if (this.worldEntities.has(entityId) ||
                                now < (this.configuredZombieRespawnAt.get(entityId) ?? 0)) {
                                continue;
                            }
                            const baseDistance = Math.max(ZOMBIE_SPAWN_AHEAD_MIN_RADIUS, this.config.zombieSpawnDistance ?? 5_500);
                            const distance = baseDistance + slot * 1_500;
                            const spread = slot === 0 ? 0 : (slot % 2 === 0 ? 0.45 : -0.45);
                            const angle = Math.atan2(forward.y, forward.x) + spread;
                            const x = player.x + Math.cos(angle) * distance;
                            const y = player.y + Math.sin(angle) * distance;
                            const z = player.z;
                            if (!this.canPlaceZombieNearPlayers({ x, y, z }, activePlayers, now)) {
                                continue;
                            }
                            const zombie = {
                                entityId,
                                kind: WorldEntityKind.Zombie,
                                revision: 1,
                                x,
                                y,
                                z,
                                yaw: (angle * 180 / Math.PI + 180) % 360,
                                health: 100,
                                state: 0,
                                classPath: explorationClasses[slot % explorationClasses.length] ??
                                    this.config.zombieClassPath ??
                                    '/Game/AI/Zombies/Roamer/BP_Zombie_Roamer.BP_Zombie_Roamer_C'
                            };
                            this.worldEntities.set(entityId, zombie);
                            this.configuredZombieRespawnAt.delete(entityId);
                            this.broadcastEntitySpawn(zombie);
                            this.broadcastEntityState(zombie);
                            this.markWorldDirty();
                            log('info', 'host_exploration_zombie_spawned', {
                                entityId,
                                playerId: player.playerId,
                                slot,
                                x: zombie.x,
                                y: zombie.y,
                                z: zombie.z,
                                sessionId: this.sessionId,
                                worldId: this.config.worldId
                            });
                        }
                    }
                }
            }
            else {
                for (const zombie of liveZombies) {
                    const spawnerIndex = this.mapSpawnerIndexFromEntityId(zombie.entityId);
                    const explore = this.explorationSlotFromEntityId(zombie.entityId);
                    if (spawnerIndex === undefined && explore === undefined)
                        continue;
                    if (this.isZombieNearAnyLivingPlayer(zombie, activePlayers))
                        continue;
                    if (this.isZombieInActiveCombat(zombie.entityId, now))
                        continue;
                    this.worldEntities.delete(zombie.entityId);
                    this.send({
                        type: MessageType.EntityDespawn,
                        sessionId: this.sessionId,
                        worldId: this.config.worldId,
                        entityId: zombie.entityId,
                        sequence: (zombie.revision + 1) >>> 0,
                        tick: ++this.tick,
                        payload: encodeWorldAction({ reason: 'map_spawner_idle_despawn' })
                    });
                    this.markWorldDirty();
                }
            }
            liveZombies = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.Zombie && entity.health > 0);
        }
        else if (configuredSpawnPoints.length === 0 &&
            activePlayers.length > 0 &&
            !this.anyPlayerZombieSpawnsPaused(activePlayers, now) &&
            !this.anyPlayerCombatSpawnsSuppressed(activePlayers, now) &&
            liveZombies.length < Math.min(Math.max(1, this.config.zombieMaxCount ?? 1), activePlayers.length * Math.max(1, this.config.zombiesPerPlayer ?? 1)) &&
            now >= this.nextZombieSpawnAt) {
            const serial = ++this.zombieSpawnSerial;
            const anchor = activePlayers[(serial - 1) % activePlayers.length];
            const baseDistance = Math.max(ZOMBIE_PLAYER_EXCLUSION_RADIUS, this.config.zombieSpawnDistance ?? ZOMBIE_SPAWN_AHEAD_MIN_RADIUS);
            let spawned = false;
            let spawnedZombie;
            for (let attempt = 0; attempt < 8 && !spawned; attempt += 1) {
                const distance = baseDistance + ((serial + attempt) % 4) * 350;
                const angle = (serial + attempt) * 2.399963229728653;
                const x = anchor.x + Math.cos(angle) * distance;
                const y = anchor.y + Math.sin(angle) * distance;
                const z = anchor.z;
                if (!this.canPlaceZombieNearPlayers({ x, y, z }, activePlayers, now)) {
                    continue;
                }
                const zombie = {
                    entityId: stableNumericId(`zombie:${this.config.worldId}:population:${serial}`),
                    kind: WorldEntityKind.Zombie,
                    revision: 1,
                    x,
                    y,
                    z,
                    yaw: (angle * 180 / Math.PI + 180) % 360,
                    health: 100,
                    state: 0,
                    classPath: this.config.zombieClassPath ??
                        '/Game/AI/Zombies/Roamer/BP_Zombie_Roamer.BP_Zombie_Roamer_C'
                };
                this.worldEntities.set(zombie.entityId, zombie);
                this.broadcastEntitySpawn(zombie);
                this.broadcastEntityState(zombie);
                this.markWorldDirty();
                spawnedZombie = zombie;
                spawned = true;
            }
            if (!spawned) {
                this.nextZombieSpawnAt = now + Math.max(5_000, this.config.zombieSpawnIntervalMs ?? 1_000);
                return;
            }
            this.nextZombieSpawnAt = now + Math.max(250, this.config.zombieSpawnIntervalMs ?? 1_000);
            log('info', 'host_authoritative_zombie_spawned', {
                entityId: spawnedZombie?.entityId,
                population: liveZombies.length + 1,
                populationTarget: Math.min(Math.max(1, this.config.zombieMaxCount ?? 1), activePlayers.length * Math.max(1, this.config.zombiesPerPlayer ?? 1)),
                sessionId: this.sessionId,
                worldId: this.config.worldId
            });
            return;
        }
        if (configuredSpawnPoints.length === 0 &&
            liveZombies.length > 0 &&
            activePlayers.length > 0) {
            for (const zombie of liveZombies) {
                if (this.relocatedZombieEntities.has(zombie.entityId))
                    continue;
                const nearestDistanceSquared = Math.min(...activePlayers.map((player) => (player.x - zombie.x) ** 2 +
                    (player.y - zombie.y) ** 2 +
                    (player.z - zombie.z) ** 2));
                this.relocatedZombieEntities.add(zombie.entityId);
                if (nearestDistanceSquared <= 3_500 * 3_500)
                    continue;
                const anchor = activePlayers[0];
                zombie.x =
                    anchor.x + Math.max(700, this.config.zombieSpawnDistance ?? 700);
                zombie.y = anchor.y;
                zombie.z = anchor.z;
                zombie.yaw = 180;
                zombie.state = 0;
                zombie.revision = (zombie.revision + 1) >>> 0;
                this.broadcastEntityState(zombie);
                this.markWorldDirty();
                log('info', 'host_authoritative_zombie_relocated', {
                    entityId: zombie.entityId,
                    playerId: anchor.playerId,
                    sessionId: this.sessionId,
                    worldId: this.config.worldId
                });
            }
        }
        for (const zombie of liveZombies) {
            let target;
            let targetDistanceSquared = Number.POSITIVE_INFINITY;
            for (const player of activePlayers) {
                const dx = player.x - zombie.x;
                const dy = player.y - zombie.y;
                const dz = player.z - zombie.z;
                const distanceSquared = dx * dx + dy * dy + dz * dz;
                if (distanceSquared < targetDistanceSquared) {
                    target = player;
                    targetDistanceSquared = distanceSquared;
                }
            }
            if (!target || targetDistanceSquared > ZOMBIE_CHASE_RADIUS * ZOMBIE_CHASE_RADIUS) {
                if (zombie.state !== 0) {
                    zombie.state = 0;
                    zombie.revision = (zombie.revision + 1) >>> 0;
                    this.broadcastEntityState(zombie);
                    this.markWorldDirty();
                }
                continue;
            }
            const dx = target.x - zombie.x;
            const dy = target.y - zombie.y;
            const dz = target.z - zombie.z;
            const planarDistance = Math.hypot(dx, dy);
            const previousYaw = zombie.yaw;
            const previousState = zombie.state;
            let moved = false;
            const desiredYaw = Math.atan2(dy, dx) * 180 / Math.PI;
            const yawDelta = ((desiredYaw - previousYaw + 540) % 360) - 180;
            if (zombie.state === 1 || Math.abs(yawDelta) >= 1) {
                zombie.yaw = desiredYaw;
            }
            const attackRange = 175;
            const canAttack = planarDistance <= attackRange && Math.abs(dz) <= 150;
            if (!canAttack && planarDistance > 0) {
                zombie.state = 1;
                this.suppressPlayerCombatSpawns(target.playerId, now);
                const chaseSpeed = planarDistance <= 3_000 ? 350 : 105;
                const travel = Math.min(Math.max(0, planarDistance - attackRange), chaseSpeed * elapsedSeconds);
                zombie.x += dx / planarDistance * travel;
                zombie.y += dy / planarDistance * travel;
                moved = travel > 0;
                if (Math.abs(dz) > 1) {
                    const zTravel = Math.min(Math.abs(dz), 220 * elapsedSeconds);
                    zombie.z += Math.sign(dz) * zTravel;
                    moved = true;
                }
            }
            else {
                const nextAttackAt = this.nextZombieAttackAt.get(zombie.entityId) ?? 0;
                if (canAttack && now >= nextAttackAt) {
                    zombie.state = 4;
                    this.nextZombieAttackAt.set(zombie.entityId, now + 1_800);
                    this.zombieAttackStateUntil.set(zombie.entityId, now + 550);
                    if (this.config.zombieDamageEnabled !== false) {
                        this.sendZombiePlayerDamage(zombie, target, 14);
                    }
                }
                else {
                    zombie.state =
                        now < (this.zombieAttackStateUntil.get(zombie.entityId) ?? 0)
                            ? 4
                            : 0;
                }
            }
            const changed = moved ||
                Math.abs(yawDelta) >= 1 ||
                zombie.state !== previousState;
            if (!changed)
                continue;
            zombie.revision = (zombie.revision + 1) >>> 0;
            this.broadcastEntityState(zombie);
            this.markWorldDirty();
        }
    }
    groundItemAgeMs(entity) {
        if (entity.requestId && entity.ownerPlayerId) {
            const drop = this.dropRequests.get(this.actionKey(entity.ownerPlayerId, entity.requestId));
            if (drop)
                return drop.updatedAtMs;
        }
        for (const drop of this.dropRequests.values()) {
            if (drop.entityId === entity.entityId)
                return drop.updatedAtMs;
        }
        return 0;
    }
    despawnGroundItem(entity, reason) {
        this.worldEntities.delete(entity.entityId);
        if (entity.requestId && entity.ownerPlayerId) {
            const drop = this.dropRequests.get(this.actionKey(entity.ownerPlayerId, entity.requestId));
            if (drop) {
                drop.status = 'consumed';
                drop.updatedAtMs = Date.now();
            }
        }
        this.send({
            type: MessageType.EntityDespawn,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            entityId: entity.entityId,
            sequence: (entity.revision + 1) >>> 0,
            tick: ++this.tick,
            payload: encodeWorldAction({
                reason,
                requestId: entity.requestId ?? reason
            })
        });
        this.markWorldDirty();
        log('info', 'host_ground_item_despawned', {
            entityId: entity.entityId,
            reason,
            sessionId: this.sessionId,
            worldId: this.config.worldId
        });
    }
    pruneStaleGroundItems(now) {
        const ttlMs = Math.max(60_000, this.config.groundItemTtlMs ?? GROUND_ITEM_TTL_MS);
        const maxCount = Math.max(16, Math.min(512, this.config.groundItemMaxCount ?? GROUND_ITEM_MAX_COUNT));
        const groundItems = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.GroundItem);
        if (groundItems.length === 0)
            return;
        const ranked = groundItems
            .map((entity) => ({
            entity,
            ageMs: this.groundItemAgeMs(entity)
        }))
            .sort((left, right) => left.ageMs - right.ageMs);
        for (const entry of ranked) {
            if (entry.ageMs <= 0)
                continue;
            if (now - entry.ageMs >= ttlMs) {
                this.despawnGroundItem(entry.entity, 'ground_item_ttl_expired');
            }
        }
        const remaining = [...this.worldEntities.values()].filter((entity) => entity.kind === WorldEntityKind.GroundItem);
        if (remaining.length <= maxCount)
            return;
        const overflow = remaining
            .map((entity) => ({
            entity,
            sortKey: this.groundItemAgeMs(entity) || now
        }))
            .sort((left, right) => left.sortKey - right.sortKey)
            .slice(0, remaining.length - maxCount);
        for (const entry of overflow) {
            this.despawnGroundItem(entry.entity, 'ground_item_budget_exceeded');
        }
    }
    playerWithin(player, entity, maximumDistance) {
        if (player.dead || player.x === undefined || player.y === undefined || player.z === undefined) {
            return false;
        }
        const dx = player.x - entity.x;
        const dy = player.y - entity.y;
        const dz = player.z - entity.z;
        return dx * dx + dy * dy + dz * dz <= maximumDistance * maximumDistance;
    }
    decodeAction(payload) {
        try {
            return decodeWorldAction(payload);
        }
        catch (error) {
            log('warning', 'host_world_action_rejected', {
                error: error instanceof Error ? error.message : 'invalid_world_action',
                worldId: this.config.worldId
            });
            return undefined;
        }
    }
    actionKey(playerId, requestId) {
        return `${playerId.toString()}:${requestId}`;
    }
    validRequestId(value) {
        return /^[a-zA-Z0-9._:-]{1,80}$/.test(value);
    }
    allowedGroundItemClassPath(value) {
        if (!/^[a-zA-Z0-9_./:-]{1,512}$/.test(value))
            return false;
        return [
            '/Game/Inventory/',
            '/Game/JigSInventory/',
            '/Game/Items/',
            '/Game/Blueprints/Items/'
        ].some((prefix) => value.startsWith(prefix));
    }
    allowedVehicleClassPath(value) {
        if (!/^[a-zA-Z0-9_./:-]{1,512}$/.test(value) || !value.endsWith('_C'))
            return false;
        if (!value.startsWith('/Game/'))
            return false;
        return /Vehicle|Car|Truck|Bike|Boat|ATV|Motorcycle|Transport|HMMWV|Humvee|Jeep|SUV|Wheeled|Offroad|Buggy|UTV|Quad/i.test(value);
    }
    allowedStructureClassPath(value) {
        if (!/^[a-zA-Z0-9_./:-]{1,512}$/.test(value) || !value.endsWith('_C'))
            return false;
        if (!value.startsWith('/Game/'))
            return false;
        return /Build|Structure|Foundation|Wall|Door|Floor|Camp|Placeable|Construct|Barricade|Fence|Gate|Roof|Stairs|Workbench|Storage|Crate|Box|Shelf|Turret|Generator|Light|Trap|Bed|Chest|Container|Deploy|Place/i.test(value);
    }
    sendInteractionResult(player, result) {
        this.send({
            type: MessageType.InteractionResult,
            connectionId: player.connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: player.playerId,
            entityId: player.entityId,
            tick: ++this.tick,
            payload: encodeWorldAction(result)
        });
    }
    updateVehicleEntityFromMovement(entityId, movement) {
        const entity = this.worldEntities.get(entityId);
        if (!entity || entity.kind !== WorldEntityKind.Vehicle)
            return;
        entity.revision = (entity.revision + 1) >>> 0;
        entity.x = movement.x;
        entity.y = movement.y;
        entity.z = movement.z;
        entity.yaw = movement.yaw;
        entity.state = 1;
        this.broadcastEntityState(entity);
        this.markWorldDirty();
    }
    handleInteractionRequest(frame) {
        const player = this.players.get(frame.connectionId);
        if (!player || player.dead)
            return;
        const action = this.decodeAction(frame.payload);
        if (!action)
            return;
        const kind = typeof action.action === 'string' ? action.action : '';
        if (kind === 'vehicleState') {
            this.handleVehicleStateRequest(player, action);
            return;
        }
        if (kind === 'structurePlace') {
            this.handleStructurePlaceRequest(player, action);
            return;
        }
        this.sendInteractionResult(player, {
            accepted: false,
            reason: 'unsupported_interaction',
            action: kind
        });
    }
    handleVehicleStateRequest(player, action) {
        const requestId = typeof action.requestId === 'string' ? action.requestId : '';
        const nativeId = typeof action.nativeId === 'string' ? action.nativeId : '';
        const classPath = typeof action.classPath === 'string' ? action.classPath : '';
        const x = typeof action.x === 'number' ? action.x : player.x;
        const y = typeof action.y === 'number' ? action.y : player.y;
        const z = typeof action.z === 'number' ? action.z : player.z;
        const yaw = typeof action.yaw === 'number' ? action.yaw : (player.yaw ?? 0);
        const health = typeof action.health === 'number' ? action.health : 100;
        const positionValid = x !== undefined && y !== undefined && z !== undefined &&
            Number.isFinite(x) && Number.isFinite(y) && Number.isFinite(z) &&
            Math.abs(x) < 10_000_000 && Math.abs(y) < 10_000_000 && Math.abs(z) < 10_000_000 &&
            player.x !== undefined && player.y !== undefined && player.z !== undefined &&
            (x - player.x) ** 2 + (y - player.y) ** 2 + (z - player.z) ** 2 <= 600 ** 2;
        if (!this.validRequestId(requestId) ||
            nativeId.length < 1 || nativeId.length > 512 || /[\r\n\0]/.test(nativeId) ||
            !this.allowedVehicleClassPath(classPath) ||
            !Number.isFinite(yaw) || Math.abs(yaw) > 360_000 ||
            !Number.isFinite(health) || health < 0 || health > 100_000 ||
            !positionValid) {
            this.sendInteractionResult(player, {
                requestId,
                accepted: false,
                reason: 'invalid_vehicle_state',
                action: 'vehicleState'
            });
            return;
        }
        const entityId = stableNumericId(`vehicle:${this.config.worldId}:${nativeId}`);
        let entity = this.worldEntities.get(entityId);
        if (!entity) {
            entity = {
                entityId,
                kind: WorldEntityKind.Vehicle,
                revision: 1,
                x: x,
                y: y,
                z: z,
                yaw: ((yaw % 360) + 360) % 360,
                health,
                state: 1,
                classPath,
                itemId: nativeId,
                requestId,
                ownerPlayerId: player.playerId
            };
            this.worldEntities.set(entityId, entity);
            this.broadcastEntitySpawn(entity);
            this.broadcastEntityState(entity);
        }
        else {
            entity.revision = (entity.revision + 1) >>> 0;
            entity.x = x;
            entity.y = y;
            entity.z = z;
            entity.yaw = ((yaw % 360) + 360) % 360;
            entity.health = health;
            entity.state = 1;
            entity.itemId = nativeId;
            entity.ownerPlayerId = player.playerId;
            this.broadcastEntityState(entity);
        }
        player.vehicleEntityId = entityId;
        player.vehicleNativeId = nativeId;
        player.mountedInVehicle = true;
        this.markWorldDirty();
        this.sendInteractionResult(player, {
            requestId,
            accepted: true,
            action: 'vehicleState',
            entityId: entityId.toString(),
            duplicate: entity.revision > 1
        });
    }
    handleStructurePlaceRequest(player, action) {
        const requestId = typeof action.requestId === 'string' ? action.requestId : '';
        const classPath = typeof action.classPath === 'string' ? action.classPath : '';
        const structureId = typeof action.structureId === 'string' ? action.structureId : classPath;
        const x = typeof action.x === 'number' ? action.x : Number.NaN;
        const y = typeof action.y === 'number' ? action.y : Number.NaN;
        const z = typeof action.z === 'number' ? action.z : Number.NaN;
        const yaw = typeof action.yaw === 'number' ? action.yaw : 0;
        const health = typeof action.health === 'number' ? action.health : 100;
        const positionValid = Number.isFinite(x) && Number.isFinite(y) && Number.isFinite(z) &&
            Math.abs(x) < 10_000_000 && Math.abs(y) < 10_000_000 && Math.abs(z) < 10_000_000 &&
            player.x !== undefined && player.y !== undefined && player.z !== undefined &&
            (x - player.x) ** 2 + (y - player.y) ** 2 + (z - player.z) ** 2 <= 600 ** 2;
        if (!this.validRequestId(requestId) ||
            !this.allowedStructureClassPath(classPath) ||
            structureId.length < 1 || structureId.length > 512 || /[\r\n\0]/.test(structureId) ||
            !Number.isFinite(yaw) || Math.abs(yaw) > 360_000 ||
            !Number.isFinite(health) || health < 0 || health > 100_000 ||
            !positionValid) {
            this.sendInteractionResult(player, {
                requestId,
                accepted: false,
                reason: 'invalid_structure_place',
                action: 'structurePlace'
            });
            return;
        }
        const entityId = stableNumericId(`structure:${this.config.worldId}:${player.playerId.toString()}:${requestId}`);
        const existing = this.worldEntities.get(entityId);
        if (existing) {
            this.broadcastEntitySpawn(existing);
            this.broadcastEntityState(existing);
            this.sendInteractionResult(player, {
                requestId,
                accepted: true,
                duplicate: true,
                action: 'structurePlace',
                entityId: entityId.toString()
            });
            return;
        }
        const entity = {
            entityId,
            kind: WorldEntityKind.PlacedStructure,
            revision: 1,
            x,
            y,
            z,
            yaw: ((yaw % 360) + 360) % 360,
            health,
            state: 0,
            classPath,
            itemId: structureId,
            requestId,
            ownerPlayerId: player.playerId
        };
        this.worldEntities.set(entityId, entity);
        this.broadcastEntitySpawn(entity);
        this.broadcastEntityState(entity);
        this.markWorldDirty();
        this.sendInteractionResult(player, {
            requestId,
            accepted: true,
            action: 'structurePlace',
            entityId: entityId.toString()
        });
        log('info', 'host_structure_placed', {
            connectionId: player.connectionId,
            playerId: player.playerId,
            entityId: entityId.toString(),
            classPath,
            worldId: this.config.worldId
        });
    }
    sendDropResult(player, entityId, action) {
        this.send({
            type: MessageType.ItemDropResult,
            connectionId: player.connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: player.playerId,
            entityId,
            tick: ++this.tick,
            payload: encodeWorldAction(action)
        });
    }
    sendPickupResult(player, result, requestId) {
        this.send({
            type: MessageType.ItemPickupResult,
            connectionId: player.connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: player.playerId,
            entityId: result.entityId,
            tick: ++this.tick,
            payload: encodeWorldAction({
                requestId,
                accepted: result.accepted,
                reason: result.reason,
                itemId: result.itemId,
                quantity: result.quantity
            })
        });
    }
    handleItemDropRequest(frame) {
        const player = this.players.get(frame.connectionId);
        if (!player || player.dead || player.x === undefined || player.y === undefined || player.z === undefined)
            return;
        const action = this.decodeAction(frame.payload);
        if (!action)
            return;
        const requestId = typeof action.requestId === 'string' ? action.requestId : '';
        const itemId = typeof action.itemId === 'string' ? action.itemId : '';
        const classPath = typeof action.classPath === 'string' ? action.classPath : '';
        const quantity = typeof action.quantity === 'number' ? action.quantity : 1;
        const hasRequestedPosition = ['x', 'y', 'z'].some((key) => action[key] !== undefined);
        const requestedX = typeof action.x === 'number' ? action.x : Number.NaN;
        const requestedY = typeof action.y === 'number' ? action.y : Number.NaN;
        const requestedZ = typeof action.z === 'number' ? action.z : Number.NaN;
        const requestedYaw = typeof action.yaw === 'number' ? action.yaw : 0;
        const requestedPositionValid = Number.isFinite(requestedX) && Number.isFinite(requestedY) && Number.isFinite(requestedZ) &&
            Math.abs(requestedX) < 10_000_000 && Math.abs(requestedY) < 10_000_000 &&
            Math.abs(requestedZ) < 10_000_000 &&
            (requestedX - player.x) ** 2 + (requestedY - player.y) ** 2 +
                (requestedZ - player.z) ** 2 <= 250 ** 2;
        if (!this.validRequestId(requestId) ||
            itemId.length < 1 || itemId.length > 256 || /[\r\n\0]/.test(itemId) ||
            !this.allowedGroundItemClassPath(classPath) ||
            !Number.isSafeInteger(quantity) || quantity < 1 || quantity > 100 ||
            !Number.isFinite(requestedYaw) || Math.abs(requestedYaw) > 360_000 ||
            (hasRequestedPosition && !requestedPositionValid)) {
            this.sendDropResult(player, 0n, {
                requestId,
                accepted: false,
                reason: 'invalid_drop_request'
            });
            return;
        }
        const key = this.actionKey(player.playerId, requestId);
        const previous = this.dropRequests.get(key);
        if (previous) {
            const entity = this.worldEntities.get(previous.entityId);
            if (previous.status === 'active' && entity) {
                this.broadcastEntitySpawn(entity);
                this.broadcastEntityState(entity);
            }
            this.sendDropResult(player, previous.entityId, {
                requestId,
                accepted: true,
                duplicate: true,
                active: previous.status === 'active' && Boolean(entity),
                consumed: previous.status === 'consumed'
            });
            return;
        }
        const entityId = stableNumericId(`ground-item:${player.playerId.toString()}:${requestId}`);
        if (this.worldEntities.has(entityId)) {
            this.sendDropResult(player, entityId, {
                requestId,
                accepted: false,
                reason: 'entity_id_collision'
            });
            return;
        }
        const entity = {
            entityId,
            kind: WorldEntityKind.GroundItem,
            revision: 1,
            x: requestedPositionValid ? requestedX : player.x + 100,
            y: requestedPositionValid ? requestedY : player.y,
            z: requestedPositionValid ? requestedZ : player.z + 20,
            yaw: ((requestedYaw % 360) + 360) % 360,
            health: 1,
            state: 0,
            classPath,
            itemId,
            quantity,
            requestId,
            ownerPlayerId: player.playerId
        };
        this.worldEntities.set(entityId, entity);
        this.dropRequests.set(key, {
            key,
            entityId,
            status: 'active',
            updatedAtMs: Date.now()
        });
        this.pruneLedger(this.dropRequests);
        this.broadcastEntitySpawn(entity);
        this.broadcastEntityState(entity);
        this.sendDropResult(player, entityId, {
            requestId,
            accepted: true,
            active: true,
            itemId,
            quantity
        });
        this.markWorldDirty();
        log('info', 'host_ground_item_created', {
            entityId, playerId: player.playerId, quantity,
            sessionId: this.sessionId, worldId: this.config.worldId
        });
    }
    handleItemPickupRequest(frame) {
        const player = this.players.get(frame.connectionId);
        if (!player)
            return;
        const action = this.decodeAction(frame.payload);
        if (!action)
            return;
        const requestId = typeof action.requestId === 'string' ? action.requestId : '';
        const entityText = typeof action.entityId === 'string' ? action.entityId : '';
        if (!this.validRequestId(requestId))
            return;
        const key = this.actionKey(player.playerId, requestId);
        const previous = this.pickupRequests.get(key);
        if (previous) {
            this.sendPickupResult(player, previous, requestId);
            const tombstone = this.entityDespawnTombstones.get(previous.entityId);
            if (previous.accepted && tombstone)
                this.sendEntityDespawn(tombstone);
            return;
        }
        let entityId = 0n;
        try {
            entityId = BigInt(entityText);
        }
        catch { /* rejected below */ }
        const entity = this.worldEntities.get(entityId);
        const accepted = Boolean(entityId > 0n &&
            entity &&
            entity.kind === WorldEntityKind.GroundItem &&
            this.playerWithin(player, entity, 350));
        const result = {
            key,
            entityId: entityId > 0n ? entityId : 0n,
            accepted,
            reason: accepted ? 'accepted' : 'unavailable_or_out_of_range',
            itemId: accepted ? entity?.itemId : undefined,
            quantity: accepted ? entity?.quantity : undefined,
            updatedAtMs: Date.now()
        };
        this.pickupRequests.set(key, result);
        this.pruneLedger(this.pickupRequests);
        if (accepted && entity) {
            this.worldEntities.delete(entityId);
            if (entity.requestId && entity.ownerPlayerId) {
                const drop = this.dropRequests.get(this.actionKey(entity.ownerPlayerId, entity.requestId));
                if (drop) {
                    drop.status = 'consumed';
                    drop.updatedAtMs = Date.now();
                }
            }
        }
        this.sendPickupResult(player, result, requestId);
        this.markWorldDirty();
        if (!accepted || !entity)
            return;
        const tombstone = {
            entityId,
            revision: (entity.revision + 1) >>> 0,
            playerId: player.playerId,
            requestId,
            expiresAtMs: Date.now() + 15_000
        };
        this.entityDespawnTombstones.set(entityId, tombstone);
        this.sendEntityDespawn(tombstone);
        log('info', 'host_ground_item_picked_up', {
            entityId, playerId: player.playerId,
            sessionId: this.sessionId, worldId: this.config.worldId
        });
    }
    handleZombieAttackRequest(frame) {
        const player = this.players.get(frame.connectionId);
        if (!player)
            return;
        const action = this.decodeAction(frame.payload);
        if (!action)
            return;
        const requestId = typeof action.requestId === 'string' ? action.requestId : '';
        const entityText = typeof action.entityId === 'string' ? action.entityId : '';
        const requestedDamage = typeof action.damage === 'number' ? action.damage : 0;
        const lethal = action.lethal === true;
        let entityId = 0n;
        try {
            entityId = BigInt(entityText);
        }
        catch { /* rejected below */ }
        const entity = this.worldEntities.get(entityId);
        const attackKey = `${player.playerId}:${entityId}`;
        const now = Date.now();
        let rejectionReason = '';
        if (!this.validRequestId(requestId))
            rejectionReason = 'invalid_request_id';
        else if (!entity)
            rejectionReason = 'missing_entity';
        else if (entity.kind !== WorldEntityKind.Zombie)
            rejectionReason = 'invalid_entity_kind';
        else if (!Number.isFinite(requestedDamage) || requestedDamage <= 0) {
            rejectionReason = 'invalid_damage';
        }
        else if (!this.playerWithin(player, entity, 3_000)) {
            rejectionReason = 'server_position_out_of_range';
        }
        else if (lethal &&
            now - (this.lastPlayerZombieDamageAt.get(attackKey) ?? 0) > 5_000) {
            rejectionReason = 'lethal_without_recent_accepted_hit';
        }
        else if (!lethal &&
            now < (this.nextPlayerZombieDamageAt.get(attackKey) ?? 0)) {
            rejectionReason = 'damage_cooldown';
        }
        if (rejectionReason) {
            this.send({
                type: MessageType.ZombieDamageResult,
                connectionId: player.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: player.playerId,
                entityId,
                tick: ++this.tick,
                payload: encodeWorldAction({
                    requestId,
                    accepted: false,
                    reason: rejectionReason
                })
            });
            log('warning', 'host_zombie_damage_rejected', {
                entityId,
                playerId: player.playerId,
                requestedDamage,
                reason: rejectionReason,
                playerX: player.x,
                playerY: player.y,
                playerZ: player.z,
                zombieX: entity?.x,
                zombieY: entity?.y,
                zombieZ: entity?.z,
                requestId,
                sessionId: this.sessionId,
                worldId: this.config.worldId
            });
            return;
        }
        if (!entity)
            return;
        if (!lethal) {
            this.nextPlayerZombieDamageAt.set(attackKey, now + 250);
            this.lastPlayerZombieDamageAt.set(attackKey, now);
            this.notePlayerZombieCombat(player.playerId, now);
        }
        const damage = lethal ? entity.health : Math.min(requestedDamage, 50);
        entity.health = Math.max(0, entity.health - damage);
        entity.revision = (entity.revision + 1) >>> 0;
        entity.state = entity.health === 0 ? 3 : 2;
        this.zombieCombatUntil.set(entityId, now + 15_000);
        if (!lethal &&
            player.x !== undefined &&
            player.y !== undefined &&
            player.z !== undefined) {
            this.reconcileZombieCombatPosition(entity, player);
        }
        this.broadcastEntityState(entity);
        this.send({
            type: MessageType.ZombieDamageResult,
            connectionId: player.connectionId,
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            playerId: player.playerId,
            entityId,
            sequence: entity.revision,
            tick: ++this.tick,
            payload: encodeWorldAction({
                requestId,
                accepted: true,
                damage,
                health: entity.health,
                dead: entity.health === 0
            })
        });
        this.markWorldDirty();
        log('info', 'host_zombie_damage_accepted', {
            entityId: entity.entityId,
            playerId: player.playerId,
            damage,
            health: entity.health,
            dead: entity.health === 0,
            lethal,
            requestId,
            sessionId: this.sessionId,
            worldId: this.config.worldId
        });
        if (entity.health === 0) {
            this.nextPlayerZombieDamageAt.delete(attackKey);
            this.lastPlayerZombieDamageAt.delete(attackKey);
            this.zombieCombatUntil.delete(entityId);
            const corpseTtlMs = Math.max(1_000, this.config.zombieCorpseTtlMs ?? 90_000);
            this.zombieCorpseExpiresAt.set(entityId, now + corpseTtlMs);
            const activePlayers = [...this.players.values()].filter((player) => player.x !== undefined &&
                player.y !== undefined &&
                player.z !== undefined &&
                !player.dead);
            this.recordZombieDeathRespawnCooldown(entity, activePlayers, now, corpseTtlMs);
        }
    }
    pruneExpiredZombieCorpses(now) {
        for (const [entityId, expiresAtMs] of [...this.zombieCorpseExpiresAt.entries()]) {
            if (now < expiresAtMs)
                continue;
            const entity = this.worldEntities.get(entityId);
            this.zombieCorpseExpiresAt.delete(entityId);
            if (!entity || entity.kind !== WorldEntityKind.Zombie || entity.health > 0)
                continue;
            this.worldEntities.delete(entityId);
            this.send({
                type: MessageType.EntityDespawn,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                entityId,
                sequence: (entity.revision + 1) >>> 0,
                tick: ++this.tick,
                payload: encodeWorldAction({ reason: 'corpse_expired' })
            });
            this.markWorldDirty();
            log('info', 'host_zombie_corpse_despawned', {
                entityId,
                sessionId: this.sessionId,
                worldId: this.config.worldId
            });
        }
    }
    pruneLedger(ledger, maximum = 5_000) {
        if (ledger.size <= maximum)
            return;
        const oldest = [...ledger.entries()]
            .sort((left, right) => left[1].updatedAtMs - right[1].updatedAtMs)
            .slice(0, ledger.size - maximum);
        for (const [key] of oldest)
            ledger.delete(key);
    }
    markWorldDirty() {
        if (this.worldStore)
            this.worldDirty = true;
    }
    capturePlayerProgress(player) {
        if (player.x === undefined || player.y === undefined ||
            player.z === undefined || player.yaw === undefined)
            return;
        this.playerProgress.set(player.playerKey, {
            playerKey: player.playerKey,
            playerId: player.playerId,
            x: player.x,
            y: player.y,
            z: player.z,
            yaw: player.yaw,
            health: player.health,
            dead: player.dead,
            profileRevision: player.profileRevision,
            updatedAtMs: Date.now()
        });
        if (this.playerProgress.size > 10_000) {
            const oldest = [...this.playerProgress.values()]
                .sort((left, right) => left.updatedAtMs - right.updatedAtMs)
                .slice(0, this.playerProgress.size - 10_000);
            for (const item of oldest)
                this.playerProgress.delete(item.playerKey);
        }
        this.markWorldDirty();
    }
    worldSnapshot() {
        return {
            worldRevision: this.worldRevision,
            simulationTick: this.tick,
            latestWorldState: this.latestWorldState
                ? {
                    authorityTimeMs: this.latestWorldState.authorityTimeMs,
                    timeOfDay: this.latestWorldState.timeOfDay,
                    rain: this.latestWorldState.rain,
                    snow: this.latestWorldState.snow,
                    fog: this.latestWorldState.fog,
                    cloudCoverage: this.latestWorldState.cloudCoverage,
                    wind: this.latestWorldState.wind,
                    thunder: this.latestWorldState.thunder
                }
                : undefined,
            weatherCycleElapsedMs: this.weatherCycleElapsedMs,
            nextZombieSpawnAtMs: this.nextZombieSpawnAt,
            zombieSpawnSerial: this.zombieSpawnSerial,
            entities: [...this.worldEntities.values()].map((entity) => ({ ...entity })),
            dropRequests: [...this.dropRequests.values()].map((item) => ({ ...item })),
            pickupRequests: [...this.pickupRequests.values()].map((item) => ({ ...item })),
            players: [...this.playerProgress.values()].map((player) => ({ ...player }))
        };
    }
    async restoreWorld() {
        if (!this.worldStore)
            return;
        try {
            const snapshot = await this.worldStore.load();
            if (!snapshot)
                return;
            this.worldRevision = snapshot.worldRevision;
            this.tick = snapshot.simulationTick;
            this.latestWorldState = snapshot.latestWorldState
                ? {
                    revision: this.worldRevision,
                    authorityTimeMs: snapshot.latestWorldState.authorityTimeMs,
                    timeOfDay: snapshot.latestWorldState.timeOfDay,
                    rain: snapshot.latestWorldState.rain,
                    snow: snapshot.latestWorldState.snow,
                    fog: snapshot.latestWorldState.fog,
                    cloudCoverage: snapshot.latestWorldState.cloudCoverage,
                    wind: snapshot.latestWorldState.wind,
                    thunder: snapshot.latestWorldState.thunder
                }
                : this.createInitialWorldState();
            if (this.config.startTimeOfDay !== undefined) {
                this.latestWorldState.timeOfDay = this.config.startTimeOfDay;
            }
            this.weatherCycleElapsedMs = snapshot.weatherCycleElapsedMs ?? 0;
            this.nextZombieSpawnAt = snapshot.nextZombieSpawnAtMs ?? 0;
            this.zombieSpawnSerial = snapshot.zombieSpawnSerial ?? 0;
            this.worldEntities.clear();
            this.dropRequests.clear();
            this.pickupRequests.clear();
            this.playerProgress.clear();
            let quarantinedZombies = 0;
            for (const entity of snapshot.entities) {
                if (entity.kind === WorldEntityKind.Zombie &&
                    this.config.authoritativeZombie !== true) {
                    quarantinedZombies += 1;
                    continue;
                }
                this.worldEntities.set(entity.entityId, entity);
            }
            for (const item of snapshot.dropRequests)
                this.dropRequests.set(item.key, item);
            for (const item of snapshot.pickupRequests)
                this.pickupRequests.set(item.key, item);
            for (const player of snapshot.players) {
                this.playerProgress.set(player.playerKey, player);
            }
            if (quarantinedZombies > 0) {
                this.nextZombieSpawnAt = 0;
                this.worldDirty = true;
                log('warning', 'host_quarantined_zombies_removed', {
                    worldId: this.config.worldId,
                    entityCount: quarantinedZombies
                });
            }
            this.worldPersistenceHealthy = true;
            this.pruneStaleGroundItems(Date.now());
            log('info', 'host_world_restored', {
                worldId: this.config.worldId,
                worldRevision: this.worldRevision,
                worldTimeOfDay: this.latestWorldState.timeOfDay,
                entityCount: this.worldEntities.size,
                dropRequestCount: this.dropRequests.size,
                pickupRequestCount: this.pickupRequests.size,
                savedPlayerCount: this.playerProgress.size
            });
        }
        catch (error) {
            this.worldPersistenceHealthy = false;
            this.worldPersistenceBlocked = true;
            log('critical', 'host_world_restore_failed', {
                worldId: this.config.worldId,
                error: error instanceof Error ? error.message : 'unknown'
            });
        }
    }
    async flushWorld(force = false) {
        if (!this.worldStore || this.worldPersistenceBlocked)
            return;
        if (this.worldSaveInFlight)
            await this.worldSaveInFlight;
        if (!force && !this.worldDirty)
            return;
        const snapshot = this.worldSnapshot();
        this.worldDirty = false;
        const operation = this.worldStore.save(snapshot)
            .then(() => {
            this.worldPersistenceHealthy = true;
        })
            .catch((error) => {
            this.worldDirty = true;
            this.worldPersistenceHealthy = false;
            log('error', 'host_world_save_failed', {
                worldId: this.config.worldId,
                error: error instanceof Error ? error.message : 'unknown'
            });
        });
        this.worldSaveInFlight = operation;
        await operation;
        if (this.worldSaveInFlight === operation)
            this.worldSaveInFlight = undefined;
    }
    send(frame) {
        if (this.socket && !this.socket.destroyed)
            this.socket.write(encodeFrame(frame));
    }
    async refreshReadiness() {
        if (this.config.gameDataDir) {
            try {
                const heartbeatPath = join(this.config.gameDataDir, 'cpp_heartbeat.txt');
                const successPath = join(this.config.gameDataDir, 'cpp_native_bootstrap_success.txt');
                const [heartbeat, success, heartbeatStat] = await Promise.all([
                    readFile(heartbeatPath, 'utf8'),
                    readFile(successPath, 'utf8'),
                    stat(heartbeatPath)
                ]);
                const fresh = Date.now() - heartbeatStat.mtimeMs <=
                    (this.config.readinessMaxAgeMs ?? 5_000);
                const mapMatch = /^map=(.+)$/m.exec(success);
                this.currentMap = mapMatch?.[1]?.trim() ?? '';
                this.gameReady = fresh &&
                    heartbeat.includes('running=1') &&
                    heartbeat.includes('unrealReady=1') &&
                    heartbeat.includes('nativeBootstrapStage=completed') &&
                    this.currentMap.includes('PersistentLevel');
            }
            catch {
                this.gameReady = false;
                this.currentMap = '';
            }
            return;
        }
        try {
            const response = await fetch(this.config.readinessUrl, {
                signal: AbortSignal.timeout(750)
            });
            const value = await response.json();
            this.gameReady = response.ok &&
                value.joined === true &&
                Number(value.gameBridge?.localFrames ?? 0) > 0;
            this.currentMap = this.gameReady ? 'PersistentLevel' : '';
        }
        catch {
            this.gameReady = false;
            this.currentMap = '';
        }
    }
}
//# sourceMappingURL=service.js.map