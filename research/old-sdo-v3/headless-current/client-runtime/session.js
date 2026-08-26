import { createConnection } from 'node:net';
import { decodeMovement, decodeString, encodeFrame, encodeString, FrameDecoder, MessageType } from "../shared-protocol/index.js";
import { log } from "../shared/log.js";
export class ClientSession {
    config;
    socket;
    decoder = new FrameDecoder();
    stopped = true;
    authenticated = false;
    joined = false;
    connectionId = 0n;
    playerId = 0n;
    entityId = 0n;
    sessionId = '00000000-0000-0000-0000-000000000000';
    sequence = 0;
    reconnectDelay;
    reconnect;
    heartbeat;
    pendingProgressRestore;
    suppressMovementUntilMs = 0;
    constructor(config) {
        this.config = config;
        this.reconnectDelay = config.reconnectMinMs ?? 250;
    }
    async start() {
        this.stopped = false;
        await this.config.gameBridge.start((frame) => this.fromGame(frame));
        this.connect();
        this.heartbeat = setInterval(() => {
            if (this.authenticated)
                this.send({
                    type: MessageType.ClientHeartbeat,
                    connectionId: this.connectionId,
                    playerId: this.playerId
                });
        }, 1_000);
        this.heartbeat.unref();
    }
    status() {
        return {
            protocolVersion: 3,
            authenticated: this.authenticated,
            joined: this.joined,
            connectionId: this.connectionId.toString(),
            playerId: this.playerId.toString(),
            entityId: this.entityId.toString(),
            sessionId: this.sessionId,
            worldId: this.config.worldId,
            gameBridge: this.config.gameBridge.status()
        };
    }
    async waitUntilJoined(timeoutMs = 5_000) {
        const started = Date.now();
        while (!this.joined) {
            if (Date.now() - started > timeoutMs) {
                throw new Error('Timed out waiting for client session join');
            }
            await new Promise((resolve) => setTimeout(resolve, 20));
        }
    }
    async stop() {
        if (this.stopped)
            return;
        this.stopped = true;
        if (this.reconnect)
            clearTimeout(this.reconnect);
        if (this.heartbeat)
            clearInterval(this.heartbeat);
        this.socket?.destroy();
        await this.config.gameBridge.stop();
    }
    connect() {
        if (this.stopped)
            return;
        const socket = createConnection({
            host: this.config.gatewayHost,
            port: this.config.gatewayPort
        });
        this.socket = socket;
        this.decoder = new FrameDecoder();
        socket.setNoDelay(true);
        socket.setKeepAlive(true, 2_000);
        socket.once('connect', () => this.send({
            type: MessageType.ClientAuthenticate,
            payload: encodeString(this.config.ticket, 4096)
        }));
        socket.on('data', (chunk) => {
            try {
                const bytes = typeof chunk === 'string' ? Buffer.from(chunk) : chunk;
                for (const frame of this.decoder.push(bytes))
                    this.fromGateway(frame);
            }
            catch (error) {
                log('error', 'client_session_protocol_error', {
                    error: error instanceof Error ? error.message : 'unknown'
                });
                socket.destroy();
            }
        });
        socket.on('error', (error) => log('warning', 'client_session_gateway_error', {
            error: error.message
        }));
        socket.once('close', () => {
            if (this.socket !== socket)
                return;
            const wasJoined = this.joined;
            this.authenticated = false;
            this.joined = false;
            this.pendingProgressRestore = undefined;
            this.suppressMovementUntilMs = 0;
            if (wasJoined && !this.stopped)
                this.requestCheckpoint('disconnect', true);
            if (!this.stopped)
                this.scheduleReconnect();
        });
    }
    fromGateway(frame) {
        if (frame.type === MessageType.AuthenticationAccepted) {
            this.authenticated = true;
            this.connectionId = frame.connectionId;
            this.playerId = frame.playerId;
            log('info', 'client_session_authenticated', {
                connectionId: this.connectionId, playerId: this.playerId,
                worldId: this.config.worldId
            });
            return;
        }
        if (frame.type === MessageType.AuthenticationRejected) {
            log('error', 'client_session_authentication_rejected', {
                reason: decodeString(frame.payload, 128)
            });
            this.socket?.destroy();
            return;
        }
        if (frame.type === MessageType.JoinAccepted) {
            this.joined = true;
            this.entityId = frame.entityId;
            this.sessionId = frame.sessionId;
            log('info', 'client_session_joined', {
                connectionId: this.connectionId, playerId: this.playerId,
                entityId: this.entityId, sessionId: this.sessionId,
                worldId: this.config.worldId
            });
            return;
        }
        if (!this.joined)
            return;
        if (frame.worldId !== this.config.worldId || frame.sessionId !== this.sessionId) {
            log('warning', 'client_session_cross_session_frame_rejected', {
                connectionId: frame.connectionId, playerId: frame.playerId,
                entityId: frame.entityId
            });
            return;
        }
        if (frame.type === MessageType.Movement ||
            frame.type === MessageType.PlayerProgressRestore) {
            decodeMovement(frame.payload);
            if (frame.type === MessageType.Movement && frame.playerId === this.playerId)
                return;
            if (frame.type === MessageType.PlayerProgressRestore) {
                this.pendingProgressRestore = frame;
                log('info', 'client_player_progress_restore_queued', {
                    connectionId: this.connectionId,
                    playerId: this.playerId,
                    worldId: this.config.worldId,
                    saveRevision: frame.sequence
                });
                return;
            }
        }
        if (frame.type === MessageType.PlayerConnected && frame.playerId === this.playerId)
            return;
        this.config.gameBridge.send(frame);
    }
    fromGame(frame) {
        if (!this.joined)
            return;
        if (this.pendingProgressRestore) {
            this.config.gameBridge.send(this.pendingProgressRestore);
            log('info', 'client_player_progress_restore_delivered', {
                connectionId: this.connectionId,
                playerId: this.playerId,
                worldId: this.config.worldId,
                saveRevision: this.pendingProgressRestore.sequence
            });
            this.pendingProgressRestore = undefined;
            this.suppressMovementUntilMs = Date.now() + 1_000;
        }
        if (frame.type === MessageType.Movement) {
            decodeMovement(frame.payload);
            if (Date.now() < this.suppressMovementUntilMs)
                return;
            this.send({
                type: MessageType.Movement,
                connectionId: this.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: this.playerId,
                entityId: this.entityId,
                sequence: ++this.sequence,
                tick: frame.tick,
                timestampMicros: frame.timestampMicros,
                payload: frame.payload
            });
            return;
        }
        if (frame.type === MessageType.DeathRequest || frame.type === MessageType.RespawnRequest) {
            this.send({
                type: frame.type,
                connectionId: this.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: this.playerId,
                entityId: this.entityId,
                sequence: ++this.sequence,
                payload: frame.payload
            });
            const reason = frame.type === MessageType.DeathRequest ? 'death' : 'respawn';
            void Promise.resolve(this.config.requestGameSave?.(reason, reason === 'death' ? 8_000 : 6_000)).catch(() => undefined);
            return;
        }
        if (frame.type === MessageType.Equipment) {
            if (frame.payload.length < 2 || frame.payload.length > 4096)
                return;
            this.send({
                type: MessageType.Equipment,
                connectionId: this.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: this.playerId,
                entityId: this.entityId,
                sequence: ++this.sequence,
                tick: frame.tick,
                timestampMicros: frame.timestampMicros,
                payload: frame.payload
            });
            void Promise.resolve(this.config.requestGameSave?.('equipment-changed', 1_500)).catch(() => undefined);
            return;
        }
        if ([
            MessageType.ItemDropRequest,
            MessageType.ItemPickupRequest,
            MessageType.ZombieAttackRequest
        ].includes(frame.type)) {
            if (frame.payload.length < 2 || frame.payload.length > 4098)
                return;
            this.send({
                type: frame.type,
                connectionId: this.connectionId,
                sessionId: this.sessionId,
                worldId: this.config.worldId,
                playerId: this.playerId,
                entityId: this.entityId,
                sequence: ++this.sequence,
                tick: frame.tick,
                timestampMicros: frame.timestampMicros,
                payload: frame.payload
            });
            if (frame.type === MessageType.ItemDropRequest ||
                frame.type === MessageType.ItemPickupRequest) {
                const reason = frame.type === MessageType.ItemDropRequest
                    ? 'inventory-item-dropped'
                    : 'inventory-item-picked-up';
                void Promise.resolve(this.config.requestGameSave?.(reason, 1_500)).catch(() => undefined);
            }
        }
    }
    send(frame) {
        if (this.socket && !this.socket.destroyed)
            this.socket.write(encodeFrame(frame));
    }
    requestCheckpoint(reason, force = false) {
        if (!this.config.checkpoint)
            return;
        void Promise.resolve(this.config.checkpoint(reason, force)).catch((error) => {
            log('error', 'client_profile_checkpoint_failed', {
                worldId: this.config.worldId,
                reason,
                error: error instanceof Error ? error.message : 'unknown'
            });
        });
    }
    scheduleReconnect() {
        if (this.reconnect || this.stopped)
            return;
        const delay = this.reconnectDelay;
        this.reconnectDelay = Math.min(this.reconnectDelay * 2, this.config.reconnectMaxMs ?? 5_000);
        this.reconnect = setTimeout(() => {
            this.reconnect = undefined;
            this.connect();
        }, delay);
        this.reconnect.unref();
    }
}
//# sourceMappingURL=session.js.map