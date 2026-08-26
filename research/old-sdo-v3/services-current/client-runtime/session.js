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
    gatewayEndpointIndex = 0;
    gatewayEndpointFailover = false;
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
    deliverPendingProgressRestore() {
        if (!this.pendingProgressRestore || !this.joined)
            return;
        this.config.gameBridge.send(this.pendingProgressRestore);
        log('info', 'client_player_progress_restore_delivered', {
            connectionId: this.connectionId,
            playerId: this.playerId,
            worldId: this.config.worldId,
            saveRevision: this.pendingProgressRestore.sequence
        });
        this.pendingProgressRestore = undefined;
        this.suppressMovementUntilMs = Date.now() + 3_000;
    }
    queueProgressRestore(frame) {
        this.pendingProgressRestore = frame;
        log('info', 'client_player_progress_restore_queued', {
            connectionId: this.connectionId,
            playerId: this.playerId,
            worldId: this.config.worldId,
            saveRevision: frame.sequence
        });
        this.deliverPendingProgressRestore();
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
        if (this.joined && this.authenticated && this.socket && !this.socket.destroyed) {
            try {
                this.send({
                    type: MessageType.ClientHeartbeat,
                    connectionId: this.connectionId,
                    playerId: this.playerId
                });
            }
            catch {
                // Best effort before the gateway observes the socket close.
            }
        }
        this.socket?.destroy();
        await this.config.gameBridge.stop();
    }
    resolveGatewayPorts() {
        const candidates = [
            ...(this.config.gatewayPorts ?? []),
            this.config.gatewayPort
        ];
        const seen = new Set();
        const ports = [];
        for (const port of candidates) {
            if (!Number.isInteger(port) || port < 1 || port > 65_535)
                continue;
            if (seen.has(port))
                continue;
            seen.add(port);
            ports.push(port);
        }
        return ports.length > 0 ? ports : [this.config.gatewayPort];
    }
    resolveGatewayEndpoints() {
        if (this.config.gatewayEndpoints && this.config.gatewayEndpoints.length > 0) {
            const seen = new Set();
            const endpoints = [];
            for (const endpoint of this.config.gatewayEndpoints) {
                if (!endpoint ||
                    typeof endpoint.host !== 'string' ||
                    !endpoint.host ||
                    !Number.isInteger(endpoint.port) ||
                    endpoint.port < 1 ||
                    endpoint.port > 65_535) {
                    continue;
                }
                const key = `${endpoint.host}:${endpoint.port}`;
                if (seen.has(key))
                    continue;
                seen.add(key);
                endpoints.push({ host: endpoint.host, port: endpoint.port });
            }
            if (endpoints.length > 0)
                return endpoints;
        }
        return this.resolveGatewayPorts().map((port) => ({
            host: this.config.gatewayHost,
            port
        }));
    }
    currentGatewayEndpoint() {
        const endpoints = this.resolveGatewayEndpoints();
        return endpoints[this.gatewayEndpointIndex % endpoints.length];
    }
    tryNextGatewayEndpoint() {
        const endpoints = this.resolveGatewayEndpoints();
        if (endpoints.length <= 1)
            return false;
        this.gatewayEndpointIndex = (this.gatewayEndpointIndex + 1) % endpoints.length;
        return true;
    }
    shouldFailoverGatewayEndpoint(reason) {
        if (this.authenticated || this.gatewayEndpointFailover)
            return false;
        if (reason) {
            return (reason === 'invalid_ticket_signature' ||
                reason === 'invalid_ticket' ||
                reason === 'invalid_ticket_claims' ||
                reason === 'bad_magic' ||
                reason.includes('Frame magic does not match'));
        }
        return true;
    }
    scheduleGatewayEndpointFailover(socket, reason) {
        if (!this.shouldFailoverGatewayEndpoint(reason) || !this.tryNextGatewayEndpoint())
            return false;
        this.gatewayEndpointFailover = true;
        socket.destroy();
        setImmediate(() => this.connect());
        return true;
    }
    connect() {
        if (this.stopped)
            return;
        const endpoint = this.currentGatewayEndpoint();
        const socket = createConnection({
            host: endpoint.host,
            port: endpoint.port
        });
        this.socket = socket;
        this.decoder = new FrameDecoder();
        socket.setNoDelay(true);
        socket.setKeepAlive(true, 2_000);
        socket.once('connect', () => {
            const endpoints = this.resolveGatewayEndpoints();
            this.gatewayEndpointIndex = endpoints.findIndex((candidate) => candidate.host === endpoint.host && candidate.port === endpoint.port);
            if (this.gatewayEndpointIndex < 0)
                this.gatewayEndpointIndex = 0;
            this.gatewayEndpointFailover = false;
            this.send({
                type: MessageType.ClientAuthenticate,
                payload: encodeString(this.config.ticket, 4096)
            });
        });
        socket.on('data', (chunk) => {
            try {
                const bytes = typeof chunk === 'string' ? Buffer.from(chunk) : chunk;
                for (const frame of this.decoder.push(bytes))
                    this.fromGateway(frame);
            }
            catch (error) {
                const message = error instanceof Error ? error.message : 'unknown';
                log('error', 'client_session_protocol_error', {
                    error: message,
                    host: endpoint.host,
                    port: endpoint.port
                });
                if (this.scheduleGatewayEndpointFailover(socket, message))
                    return;
                socket.destroy();
            }
        });
        socket.on('error', (error) => {
            log('warning', 'client_session_gateway_error', {
                error: error.message,
                host: endpoint.host,
                port: endpoint.port
            });
            if (this.scheduleGatewayEndpointFailover(socket))
                return;
        });
        socket.once('close', () => {
            if (this.socket !== socket)
                return;
            if (this.gatewayEndpointFailover) {
                this.gatewayEndpointFailover = false;
                return;
            }
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
            const reason = decodeString(frame.payload, 128);
            log('error', 'client_session_authentication_rejected', {
                reason,
                host: this.currentGatewayEndpoint().host,
                port: this.currentGatewayEndpoint().port
            });
            if (this.socket && this.shouldFailoverGatewayEndpoint(reason)) {
                if (this.scheduleGatewayEndpointFailover(this.socket))
                    return;
            }
            this.socket?.destroy();
            return;
        }
        if (frame.type === MessageType.PlayerProgressRestore) {
            decodeMovement(frame.payload);
            this.queueProgressRestore(frame);
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
            this.deliverPendingProgressRestore();
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
        if (frame.type === MessageType.Movement) {
            decodeMovement(frame.payload);
            if (frame.type === MessageType.Movement && frame.playerId === this.playerId)
                return;
        }
        if (frame.type === MessageType.PlayerConnected && frame.playerId === this.playerId)
            return;
        this.config.gameBridge.send(frame);
    }
    fromGame(frame) {
        if (!this.joined)
            return;
        this.deliverPendingProgressRestore();
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
        if (frame.type === MessageType.InteractionRequest) {
            if (frame.payload.length < 2 || frame.payload.length > 4098)
                return;
            this.send({
                type: MessageType.InteractionRequest,
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
            if (this.tryNextGatewayEndpoint()) {
                this.gatewayEndpointFailover = false;
            }
            this.connect();
        }, delay);
        this.reconnect.unref();
    }
}
//# sourceMappingURL=session.js.map