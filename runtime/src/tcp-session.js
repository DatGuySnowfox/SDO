'use strict';

// Manages the TCP connection to the gateway.
//
// State machine:
//   disconnected → connecting → authenticating → joining → active
//
// On any error/close: back to disconnected, schedule reconnect with
// exponential backoff up to maxReconnectMs.

const net = require('node:net');
const cfg          = require('./config');
const { FrameDecoder } = require('./frame-decoder');
const {
    MsgType, decodeFrame, encodeFrame, encodeString, decodeString,
} = require('./protocol');

// Frame types we forward from the game DLL to the gateway.
const GAME_TO_GW = new Set([
    MsgType.Movement,
    MsgType.DeathRequest,
    MsgType.RespawnRequest,
    MsgType.Equipment,
    MsgType.ProfileRevision,
    MsgType.InteractionRequest,
    MsgType.ItemDropRequest,
    MsgType.ItemPickupRequest,
    MsgType.ZombieAttackRequest,
]);

// Frame types we forward from the gateway to the game DLL.
const GW_TO_GAME = new Set([
    MsgType.PlayerConnected,
    MsgType.PlayerDisconnected,
    MsgType.Movement,
    MsgType.Death,
    MsgType.Respawn,
    MsgType.WorldState,
    MsgType.EntitySpawn,
    MsgType.EntityState,
    MsgType.EntityDespawn,
    MsgType.InteractionResult,
    MsgType.ItemDropResult,
    MsgType.ItemPickupResult,
    MsgType.ZombieDamageResult,
    MsgType.SaveAcknowledgement,
    MsgType.PlayerProgressRestore,
    MsgType.PlayerDamage,
]);

class TcpSession {
    constructor(sendToGame) {
        this._sendToGame     = sendToGame; // (buf: Buffer) => void
        this._socket         = null;
        this._decoder        = new FrameDecoder();
        this._state          = 'disconnected';
        this._session        = null;  // populated on AuthenticationAccepted + JoinAccepted
        this._heartbeatTimer = null;
        this._reconnectTimer = null;
        this._reconnectMs    = cfg.reconnectMs;
        this._stopped        = false;
    }

    start() {
        this._stopped = false;
        this._connect();
    }

    stop() {
        this._stopped = true;
        this._clearTimers();
        if (this._socket) { this._socket.destroy(); this._socket = null; }
        this._state   = 'disconnected';
        this._session = null;
        this._decoder.reset();
    }

    // Called by the UDP bridge when the game DLL sends a frame.
    onGameFrame(rawBuf) {
        if (this._state !== 'active') return;

        let frame;
        try { frame = decodeFrame(rawBuf); }
        catch { return; } // malformed – ignore
        if (!frame || !GAME_TO_GW.has(frame.type)) return;

        // Overwrite session fields with our authoritative values from the gateway.
        const s = this._session;
        this._socketWrite(encodeFrame({
            ...frame,
            connectionId:    s.connectionId,
            sessionId:       s.sessionId,
            worldId:         s.worldId,
            playerId:        s.playerId,
            entityId:        s.entityId,
            timestampMicros: BigInt(Date.now()) * 1000n,
        }));
    }

    // ── private ────────────────────────────────────────────────────────────

    _connect() {
        if (this._stopped) return;
        this._state = 'connecting';
        this._decoder.reset();
        this._session = null;

        const sock = net.createConnection(cfg.gatewayPort, cfg.gatewayHost);
        this._socket = sock;

        sock.on('connect', () => this._onConnect());
        sock.on('data',    (chunk) => this._onData(chunk));
        sock.on('error',   (err) => console.error(`[tcp] ${err.message}`));
        sock.on('close',   () => this._onClose());
    }

    _onConnect() {
        console.log(`[tcp] connected to ${cfg.gatewayHost}:${cfg.gatewayPort}`);
        this._reconnectMs = cfg.reconnectMs; // reset backoff on successful connect
        this._state = 'authenticating';

        const ticket = cfg.joinTicket;
        if (!ticket) {
            console.error('[tcp] SDB_JOIN_TICKET is not set – cannot authenticate');
            this._socket.destroy();
            return;
        }

        this._socketWrite(encodeFrame({
            type:    MsgType.ClientAuthenticate,
            payload: encodeString(ticket, 4096),
        }));
    }

    _onData(chunk) {
        let frames;
        try { frames = this._decoder.push(chunk); }
        catch (err) {
            console.error(`[tcp] decode error: ${err.message}`);
            this._socket.destroy();
            return;
        }
        for (const f of frames) this._dispatch(f);
    }

    _dispatch(f) {
        switch (this._state) {
            case 'authenticating': this._handleAuth(f);   break;
            case 'joining':        this._handleJoin(f);   break;
            case 'active':         this._handleActive(f); break;
        }
    }

    _handleAuth(f) {
        if (f.type === MsgType.AuthenticationAccepted) {
            this._session = {
                connectionId: f.connectionId,
                sessionId:    f.sessionId,
                worldId:      f.worldId,
                playerId:     f.playerId,
                entityId:     0n,
            };
            console.log(`[tcp] authenticated  cid=${f.connectionId}  pid=${f.playerId}`);
            this._state = 'joining';
            return;
        }
        if (f.type === MsgType.AuthenticationRejected) {
            const reason = f.payload.length >= 2 ? decodeString(f.payload) : 'unknown';
            console.error(`[tcp] authentication rejected: ${reason}`);
            this._socket.destroy();
        }
    }

    _handleJoin(f) {
        if (f.type === MsgType.JoinAccepted) {
            this._session.entityId = f.entityId;
            console.log(`[tcp] joined  eid=${f.entityId}`);
            this._state = 'active';
            this._startHeartbeat();
            // Seed the game DLL's session context so it can annotate outbound frames.
            this._seedGameSession();
            return;
        }
        if (f.type === MsgType.JoinRejected) {
            const reason = f.payload.length >= 2 ? decodeString(f.payload) : 'unknown';
            console.error(`[tcp] join rejected: ${reason}`);
            this._socket.destroy();
            return;
        }
        // Relay any frames that arrive before JoinAccepted (e.g. early WorldState).
        if (GW_TO_GAME.has(f.type)) this._forwardToGame(f);
    }

    _handleActive(f) {
        if (!GW_TO_GAME.has(f.type)) return;

        // Suppress our own movement echo and our own PlayerConnected (DLL already knows).
        if (f.playerId === this._session.playerId) {
            if (f.type === MsgType.Movement)        return;
            if (f.type === MsgType.PlayerConnected) return;
        }

        this._forwardToGame(f);
    }

    // Send a PlayerConnected frame to the game DLL so it latches session fields.
    // After this, the DLL will correctly annotate all outbound frames.
    _seedGameSession() {
        const s = this._session;
        this._sendToGame(encodeFrame({
            type:         MsgType.PlayerConnected,
            connectionId: s.connectionId,
            sessionId:    s.sessionId,
            worldId:      s.worldId,
            playerId:     s.playerId,
            entityId:     s.entityId,
            payload:      encodeString(cfg.displayName, 64),
        }));
    }

    _forwardToGame(f) {
        this._sendToGame(encodeFrame(f));
    }

    _startHeartbeat() {
        this._heartbeatTimer = setInterval(() => {
            if (this._state !== 'active') return;
            const s = this._session;
            this._socketWrite(encodeFrame({
                type:         MsgType.ClientHeartbeat,
                connectionId: s.connectionId,
                sessionId:    s.sessionId,
                worldId:      s.worldId,
                playerId:     s.playerId,
                entityId:     s.entityId,
            }));
        }, cfg.heartbeatMs);
    }

    _socketWrite(buf) {
        if (this._socket && !this._socket.destroyed)
            this._socket.write(buf);
    }

    _onClose() {
        console.log('[tcp] disconnected');
        this._clearTimers();
        this._socket  = null;
        this._state   = 'disconnected';
        this._session = null;
        this._decoder.reset();
        if (!this._stopped) this._scheduleReconnect();
    }

    _scheduleReconnect() {
        console.log(`[tcp] reconnecting in ${this._reconnectMs}ms`);
        this._reconnectTimer = setTimeout(() => {
            this._reconnectMs = Math.min(this._reconnectMs * 2, cfg.maxReconnectMs);
            this._connect();
        }, this._reconnectMs);
    }

    _clearTimers() {
        if (this._heartbeatTimer) { clearInterval(this._heartbeatTimer); this._heartbeatTimer = null; }
        if (this._reconnectTimer) { clearTimeout(this._reconnectTimer);  this._reconnectTimer = null; }
    }
}

module.exports = { TcpSession };
