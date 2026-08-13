'use strict';

// Gateway: TCP hub + HTTP ticket API.
//
// Two connection types share the same port:
//   • host-agent  — identified by HostAuthenticate (type 1) as first frame
//   • client      — identified by ClientAuthenticate (type 2) as first frame
//
// Frame routing:
//   client  → gateway → host       (Movement, DeathRequest, RespawnRequest, …)
//   gateway → clients (broadcast)  (PlayerConnected, Movement, WorldState, …)
//   host    → gateway → specific client  (JoinAccepted, Death, PlayerDamage, …)

const net    = require('node:net');
const http   = require('node:http');
const crypto = require('node:crypto');

const cfg = require('./config');
const db  = require('./db');
const { FrameDecoder }                               = require('./lib/frame-decoder');
const { MsgType, encodeFrame, encodeString, decodeString } = require('./lib/protocol');
const { verifyTicket, signTicket }                   = require('./lib/ticket');
const { stableNumericId }                            = require('./lib/id');

const CLIENT_RATE_LIMIT = 120;  // max frames/s per client
const HOST_RATE_LIMIT   = 4096;
const MAX_MALFORMED     = 3;

// ── Connection ────────────────────────────────────────────────────────────────

class Connection {
    constructor(id, socket) {
        this.id           = id;          // BigInt — gateway's internal handle
        this.socket       = socket;
        this.decoder      = new FrameDecoder();
        this.role         = null;        // 'host' | 'client' — set on first frame
        this.state        = 'auth';      // auth | joining | joined
        this.sessionId    = null;        // Buffer(16)
        this.worldId      = null;        // Buffer(16)
        this.playerId     = 0n;
        this.entityId     = 0n;
        this.displayName  = '';
        this.lastMovement = null;        // Buffer — cached for late-joiner replay
        this.malformed    = 0;
        this._rateStart   = Date.now();
        this._rateCount   = 0;
    }

    write(buf) {
        if (!this.socket.destroyed) this.socket.write(buf);
    }

    underRateLimit(limit) {
        const now = Date.now();
        if (now - this._rateStart >= 1000) { this._rateStart = now; this._rateCount = 0; }
        return ++this._rateCount <= limit;
    }
}

// ── Gateway ───────────────────────────────────────────────────────────────────

class Gateway {
    constructor() {
        this._host        = null;              // Connection — the host-agent
        this._clients     = new Map();         // connectionId → Connection (all clients)
        this._joined      = new Map();         // playerId     → Connection (joined only)
        this._usedTickets = new Map();         // ticketId     → expiresAtMs (replay guard)
        this._nextId      = 1n;

        // World session identity (filled in by host on HostAuthenticate)
        this._sessionId   = Buffer.alloc(16);
        this._worldId     = cfg.worldId;

        setInterval(() => this._pruneTickets(), 60_000).unref();
        setInterval(() => this._checkTimeouts(), 5_000).unref();
    }

    listen(onReady) {
        let started = 0;
        const check = () => { if (++started === 2 && onReady) onReady(); };

        this._tcp = net.createServer((sock) => this._onSocket(sock));
        this._tcp.listen(cfg.gatewayPort, cfg.gatewayBind, () => {
            console.log(`[gw] TCP  ${cfg.gatewayBind}:${cfg.gatewayPort}`);
            check();
        });

        this._http = http.createServer((req, res) => this._onHttp(req, res));
        this._http.listen(cfg.httpPort, cfg.httpBind, () => {
            console.log(`[gw] HTTP ${cfg.httpBind}:${cfg.httpPort}`);
            check();
        });
    }

    // ── TCP ──────────────────────────────────────────────────────────────────

    _onSocket(sock) {
        const conn = new Connection(this._nextId++, sock);
        sock.on('data',  (c) => this._onData(conn, c));
        sock.on('error', (e) => console.error(`[gw] ${conn.id}: ${e.message}`));
        sock.on('close', ()  => this._onClose(conn));
    }

    _onData(conn, chunk) {
        let frames;
        try { frames = conn.decoder.push(chunk); }
        catch (e) { console.error(`[gw] ${conn.id} decode: ${e.message}`); conn.socket.destroy(); return; }

        const rateLimit = conn.role === 'host' ? HOST_RATE_LIMIT : CLIENT_RATE_LIMIT;

        for (const f of frames) {
            if (!conn.underRateLimit(rateLimit)) {
                console.warn(`[gw] ${conn.id} rate limited`);
                conn.socket.destroy();
                return;
            }
            try { this._dispatch(conn, f); }
            catch (e) {
                if (++conn.malformed >= MAX_MALFORMED) {
                    console.error(`[gw] ${conn.id} too many malformed frames: ${e.message}`);
                    conn.socket.destroy();
                    return;
                }
            }
        }
    }

    _dispatch(conn, f) {
        if      (conn.role === null)   this._firstFrame(conn, f);
        else if (conn.role === 'host') this._fromHost(conn, f);
        else                           this._fromClient(conn, f);
    }

    // ── First frame determines role ───────────────────────────────────────────

    _firstFrame(conn, f) {
        if      (f.type === MsgType.HostAuthenticate)   { conn.role = 'host';   this._authHost(conn, f); }
        else if (f.type === MsgType.ClientAuthenticate) { conn.role = 'client'; this._authClient(conn, f); }
        else    conn.socket.destroy();
    }

    // ── Host-agent path ───────────────────────────────────────────────────────

    _authHost(conn, f) {
        if (!cfg.hostSecret) { this._authReject(conn, 'host_auth_not_configured'); return; }

        const secret = f.payload.length >= 2 ? decodeString(f.payload) : '';
        const match  = secret.length === cfg.hostSecret.length &&
            crypto.timingSafeEqual(Buffer.from(secret), Buffer.from(cfg.hostSecret));
        if (!match) { this._authReject(conn, 'host_authentication_failed'); return; }

        if (this._host) {
            console.log('[gw] evicting previous host connection');
            this._host.socket.destroy();
        }
        this._host = conn;
        conn.state = 'joined';

        // Accept the session UUID provided by the host (it owns the session identity).
        if (f.sessionId.some(b => b !== 0)) this._sessionId = f.sessionId;
        if (f.worldId.some(b => b !== 0))   this._worldId   = f.worldId;

        conn.sessionId = this._sessionId;
        conn.worldId   = this._worldId;

        conn.write(encodeFrame({
            type:      MsgType.AuthenticationAccepted,
            sessionId: this._sessionId,
            worldId:   this._worldId,
        }));
        console.log(`[gw] host authenticated`);

        // Resend JoinRequest for any clients waiting since before host connected.
        for (const [, c] of this._clients)
            if (c.state === 'joining') this._sendJoinRequest(c);
    }

    _fromHost(conn, f) {
        switch (f.type) {

        case MsgType.HostHeartbeat:
            break; // lastFrameMs already updated by caller

        // Routed to a specific client (connectionId = gateway's client conn id)
        case MsgType.JoinAccepted: {
            const client = this._clients.get(f.connectionId);
            if (!client) break;
            client.entityId = f.entityId;
            client.state    = 'joined';
            this._joined.set(client.playerId, client);

            client.write(encodeFrame({ ...f, connectionId: client.id }));

            // Restore saved player progress (inventory, stats, position) if available.
            const savedProgress = db.getProgress(String(client.playerId));
            if (savedProgress) {
                client.write(encodeFrame({
                    type:         MsgType.PlayerProgressRestore,
                    connectionId: client.id,
                    sessionId:    client.sessionId,
                    worldId:      client.worldId,
                    playerId:     client.playerId,
                    entityId:     client.entityId,
                    payload:      savedProgress,
                }));
            }

            // Tell all existing joined clients about the newcomer.
            const connectedBuf = encodeFrame({
                type:         MsgType.PlayerConnected,
                connectionId: client.id,
                sessionId:    client.sessionId,
                worldId:      client.worldId,
                playerId:     client.playerId,
                entityId:     client.entityId,
                payload:      encodeString(client.displayName, 64),
            });
            this._broadcast(client.playerId, connectedBuf);

            // Replay all currently joined players + persisted world entities to the newcomer.
            this._replayTo(client);

            console.log(`[gw] client ${client.id} joined  pid=${client.playerId}  eid=${client.entityId}`);
            break;
        }

        case MsgType.JoinRejected: {
            const client = this._clients.get(f.connectionId);
            if (!client) break;
            client.write(encodeFrame({ ...f, connectionId: client.id }));
            client.socket.destroy();
            break;
        }

        // Sent to a specific client
        case MsgType.Death:
        case MsgType.Respawn:
        case MsgType.PlayerDamage:
        case MsgType.InteractionResult:
        case MsgType.ItemPickupResult:
        case MsgType.ItemDropResult:
        case MsgType.ZombieDamageResult:
        case MsgType.PlayerProgressRestore: {
            const client = this._clients.get(f.connectionId);
            if (client) client.write(encodeFrame(f));
            break;
        }

        // Broadcast to all joined clients — entity spawns/despawns also persisted to DB
        case MsgType.EntitySpawn: {
            const buf = encodeFrame(f);
            db.spawnEntity(f.entityId, buf);
            for (const [, c] of this._joined) c.write(buf);
            break;
        }

        case MsgType.EntityDespawn: {
            const buf = encodeFrame(f);
            db.despawnEntity(f.entityId);
            for (const [, c] of this._joined) c.write(buf);
            break;
        }

        case MsgType.EntityState: {
            const buf = encodeFrame(f);
            // Append onto the stored descriptor frame so a late joiner's
            // replay (below) gets both frames — no-op for entityId 0 / not
            // a tracked world entity.
            if (f.entityId) db.appendEntityState(f.entityId, buf);
            for (const [, c] of this._joined) c.write(buf);
            break;
        }

        case MsgType.WorldState:
        case MsgType.SaveAcknowledgement: {
            const buf = encodeFrame(f);
            for (const [, c] of this._joined) c.write(buf);
            break;
        }

        // Host confirms a player disconnect; broadcast to remaining clients.
        case MsgType.PlayerDisconnected: {
            const buf = encodeFrame(f);
            this._broadcast(f.playerId, buf);
            break;
        }

        } // switch
    }

    // ── Client path ───────────────────────────────────────────────────────────

    _authClient(conn, f) {
        if (!cfg.ticketSecret) { this._authReject(conn, 'server_misconfigured'); return; }

        const ticketStr = f.payload.length >= 2 ? decodeString(f.payload) : '';
        let body;
        try { body = verifyTicket(ticketStr, cfg.ticketSecret, cfg.worldIdStr); }
        catch (e) { this._authReject(conn, e.message); return; }

        if (this._usedTickets.has(body.ticketId)) { this._authReject(conn, 'ticket_replay'); return; }
        if (this._joined.size >= cfg.maxPlayers)  { this._authReject(conn, 'server_full');   return; }

        this._usedTickets.set(body.ticketId, body.expiresAtMs);

        const playerId = stableNumericId(`player-entity:${body.playerId}`);

        // Evict stale duplicate connection for the same player.
        const stale = this._joined.get(playerId);
        if (stale) {
            stale.write(encodeFrame({
                type:    MsgType.AuthenticationRejected,
                payload: encodeString('replaced_by_new_connection', 128),
            }));
            stale.socket.destroy();
        }

        conn.playerId    = playerId;
        conn.sessionId   = this._sessionId;
        conn.worldId     = this._worldId;
        conn.displayName = String(body.displayName || 'Player').slice(0, 64);
        conn.state       = 'joining';

        this._clients.set(conn.id, conn);

        conn.write(encodeFrame({
            type:         MsgType.AuthenticationAccepted,
            connectionId: conn.id,
            sessionId:    conn.sessionId,
            worldId:      conn.worldId,
            playerId:     conn.playerId,
        }));

        console.log(`[gw] client ${conn.id} authenticated  pid=${conn.playerId}  name="${conn.displayName}"`);

        // Send JoinRequest now if host is online; otherwise client waits.
        if (this._host) this._sendJoinRequest(conn);
    }

    _fromClient(conn, f) {
        if (conn.state !== 'joined') return;

        switch (f.type) {

        case MsgType.ClientHeartbeat:
            break; // lastFrameMs tracking is enough

        case MsgType.Movement: {
            const out = encodeFrame({
                ...f,
                connectionId: conn.id,
                playerId:     conn.playerId,
                entityId:     conn.entityId,
            });
            conn.lastMovement = out;
            this._broadcast(conn.playerId, out);   // relay directly to other clients (low latency)
            if (this._host) this._host.write(out); // also send to host for server-side logic
            break;
        }

        // Intercept ProfileRevision to persist progress before forwarding to host.
        case MsgType.ProfileRevision:
            if (f.payload.length >= 5) {
                const revision = f.payload.readUInt32BE(1);
                db.saveProgress(String(conn.playerId), revision, f.payload);
            }
            if (this._host) this._host.write(encodeFrame({
                ...f, connectionId: conn.id, playerId: conn.playerId, entityId: conn.entityId,
            }));
            break;

        // Equipment is client-authoritative appearance data, same as Movement —
        // relay directly to other clients for low latency, host gets a copy
        // for future server-side validation (currently a no-op there; see
        // host-agent.js). Previously only forwarded to host, which doesn't
        // exist yet as a real player, so remote players' equipment never
        // propagated to anyone at all.
        case MsgType.Equipment: {
            const out = encodeFrame({
                ...f,
                connectionId: conn.id,
                playerId:     conn.playerId,
                entityId:     conn.entityId,
            });
            this._broadcast(conn.playerId, out);
            if (this._host) this._host.write(out);
            break;
        }

        // Same client-authoritative relay as Equipment above — which
        // attachments are installed on an equipped weapon, purely cosmetic.
        case MsgType.WeaponAttachments: {
            const out = encodeFrame({
                ...f,
                connectionId: conn.id,
                playerId:     conn.playerId,
                entityId:     conn.entityId,
            });
            this._broadcast(conn.playerId, out);
            if (this._host) this._host.write(out);
            break;
        }

        // Same client-authoritative relay as Equipment/WeaponAttachments —
        // gender/hair/beard appearance, purely cosmetic.
        case MsgType.PawnAppearance: {
            const out = encodeFrame({
                ...f,
                connectionId: conn.id,
                playerId:     conn.playerId,
                entityId:     conn.entityId,
            });
            this._broadcast(conn.playerId, out);
            if (this._host) this._host.write(out);
            break;
        }

        // Forward to host-agent for authoritative processing.
        // ItemPickupRequest is the odd one out: entityId here means "the
        // world entity being picked up" (set by the client to a value it
        // read from the entity list), not "my own entity" like every other
        // message in this block — normalizing it to conn.entityId silently
        // clobbered the pickup target with the sender's own id, so pickups
        // could never resolve server-side no matter what triggered the send
        // client-side (found 2026-08-12 after five ruled-out hook attempts
        // and a working polling-based sender that still didn't work end to
        // end — the bug was here all along).
        case MsgType.ItemPickupRequest:
            if (this._host) this._host.write(encodeFrame({
                ...f,
                connectionId: conn.id,
                playerId:     conn.playerId,
            }));
            break;

        case MsgType.DeathRequest:
        case MsgType.RespawnRequest:
        case MsgType.InteractionRequest:
        case MsgType.ItemDropRequest:
        case MsgType.ZombieAttackRequest:
            if (this._host) this._host.write(encodeFrame({
                ...f,
                connectionId: conn.id,
                playerId:     conn.playerId,
                entityId:     conn.entityId,
            }));
            break;

        } // switch
    }

    // ── Disconnect ────────────────────────────────────────────────────────────

    _onClose(conn) {
        if (conn.role === 'host' && this._host === conn) {
            this._host = null;
            console.log('[gw] host disconnected');
            return;
        }

        this._clients.delete(conn.id);
        if (conn.state !== 'joined') return;
        this._joined.delete(conn.playerId);

        const disconnFrame = encodeFrame({
            type:         MsgType.PlayerDisconnected,
            connectionId: conn.id,
            sessionId:    conn.sessionId,
            worldId:      conn.worldId,
            playerId:     conn.playerId,
            entityId:     conn.entityId,
            payload:      encodeString('connection_closed', 128),
        });

        if (this._host) {
            // Tell host so it can clean up; the host echoes back and we broadcast then.
            this._host.write(disconnFrame);
        } else {
            // No host online — broadcast directly since no one will echo it.
            this._broadcast(conn.playerId, disconnFrame);
        }

        console.log(`[gw] client ${conn.id} disconnected  pid=${conn.playerId}`);
    }

    // ── Helpers ───────────────────────────────────────────────────────────────

    _sendJoinRequest(conn) {
        const nameBuf = Buffer.from(conn.displayName, 'utf8');
        const keyBuf  = Buffer.from(String(conn.playerId), 'utf8');
        const payload = Buffer.allocUnsafe(2 + keyBuf.length + 2 + nameBuf.length);
        payload.writeUInt16BE(keyBuf.length, 0);
        keyBuf.copy(payload, 2);
        payload.writeUInt16BE(nameBuf.length, 2 + keyBuf.length);
        nameBuf.copy(payload, 2 + keyBuf.length + 2);

        this._host.write(encodeFrame({
            type:         MsgType.JoinRequest,
            connectionId: conn.id,
            sessionId:    conn.sessionId,
            worldId:      conn.worldId,
            playerId:     conn.playerId,
            payload,
        }));
    }

    // Broadcast buf to every joined client except the one with excludePlayerId.
    _broadcast(excludePlayerId, buf) {
        for (const [pid, c] of this._joined)
            if (pid !== excludePlayerId) c.write(buf);
    }

    // Replay PlayerConnected + last Movement for each joined player to a new client,
    // then replay all persisted world entities (dropped items, building pieces, etc.).
    _replayTo(newClient) {
        for (const [pid, c] of this._joined) {
            if (pid === newClient.playerId) continue;
            newClient.write(encodeFrame({
                type:         MsgType.PlayerConnected,
                connectionId: c.id,
                sessionId:    c.sessionId,
                worldId:      c.worldId,
                playerId:     c.playerId,
                entityId:     c.entityId,
                payload:      encodeString(c.displayName, 64),
            }));
            if (c.lastMovement) newClient.write(c.lastMovement);
        }

        // Replay every world entity so the newcomer sees the current world state.
        for (const frameBytes of db.getAllEntities())
            newClient.write(frameBytes);
    }

    _authReject(conn, reason) {
        conn.write(encodeFrame({ type: MsgType.AuthenticationRejected, payload: encodeString(reason, 128) }));
        conn.socket.end();
    }

    _pruneTickets() {
        const now = Date.now();
        for (const [id, exp] of this._usedTickets)
            if (exp < now) this._usedTickets.delete(id);
    }

    _checkTimeouts() {
        const now = Date.now();
        for (const [, c] of this._clients) {
            // (lastFrameMs not currently tracked per-frame here; left for future)
            void now; void c;
        }
    }

    // ── HTTP ticket API ───────────────────────────────────────────────────────

    _onHttp(req, res) {
        const json = (status, body) => {
            res.writeHead(status, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(body));
        };

        if (req.method === 'GET' && req.url === '/v1/health') {
            json(200, {
                ok:         true,
                players:    this._joined.size,
                maxPlayers: cfg.maxPlayers,
                hostOnline: !!this._host,
            });
            return;
        }

        if (req.method === 'GET' && req.url.startsWith('/v1/players/')) {
            const playerId = req.url.slice('/v1/players/'.length);
            if (!playerId) { json(400, { error: 'playerId required' }); return; }
            const player = db.getPlayer(playerId);
            if (!player) { json(404, { error: 'not_found' }); return; }
            json(200, { playerId: player.playerId, displayName: player.displayName,
                        firstSeen: player.firstSeen, lastSeen: player.lastSeen });
            return;
        }

        if (req.method === 'POST' && req.url === '/v1/tickets') {
            if (cfg.adminToken) {
                const auth     = Buffer.from((req.headers['authorization'] || '').replace(/^Bearer\s+/, ''));
                const expected = Buffer.from(cfg.adminToken);
                const match    = auth.length === expected.length &&
                                 crypto.timingSafeEqual(auth, expected);
                if (!match) {
                    json(401, { error: 'unauthorized' });
                    return;
                }
            }

            let raw = '';
            req.on('data', (c) => { raw += c; });
            req.on('end', () => {
                try {
                    const { playerId, displayName } = JSON.parse(raw);
                    if (!playerId) { json(400, { error: 'playerId required' }); return; }

                    const name = String(displayName || 'Player').slice(0, 64);
                    db.upsertPlayer(String(playerId), name);

                    const ticket = signTicket({
                        ticketId:        crypto.randomUUID(),
                        playerId:        String(playerId),
                        displayName:     name,
                        worldId:         cfg.worldIdStr,
                        expiresAtMs:     Date.now() + cfg.ticketTtlMs,
                        protocolVersion: 3,
                    }, cfg.ticketSecret);

                    const host = cfg.gatewayBind === '0.0.0.0' ? '127.0.0.1' : cfg.gatewayBind;
                    json(200, { ticket, gatewayHost: host, gatewayPort: cfg.gatewayPort, worldId: cfg.worldIdStr });
                } catch (e) {
                    json(400, { error: e.message });
                }
            });
            return;
        }

        json(404, { error: 'not_found' });
    }
}

module.exports = { Gateway };
