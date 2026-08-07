'use strict';

// Host-agent: authoritative game-state process.
//
// Responsibilities:
//   • Authenticate with the gateway as the host
//   • Accept/reject player joins; assign entity IDs
//   • Manage world entities: dropped items, placed building pieces
//   • Process ItemDropRequest / ItemPickupRequest
//   • Process InteractionRequest (BUILD)
//   • Confirm Death/Respawn requests
//   • Broadcast WorldState periodically (advancing time-of-day)
//   • Persist entity and world-state changes to SQLite via db.js
//   • Reconnect automatically on gateway disconnect

const net    = require('node:net');
const crypto = require('node:crypto');

const cfg = require('./config');
const db  = require('./db');
const { FrameDecoder } = require('./lib/frame-decoder');
const {
    MsgType,
    EntityType,
    InteractionType,
    encodeFrame,
    encodeString,
    decodeString,
    encodeEntitySpawn,
    encodeLootState,
    decodeLootState,
    encodeBuildingState,
    decodeBuildingState,
    decodeEntitySpawn,
    decodeItemDropRequest,
    decodeItemPickupRequest,
    encodeItemPickupResult,
    encodeItemDropResult,
    decodeInteractionRequest,
    encodeInteractionResult,
} = require('./lib/protocol');
const { stableNumericId } = require('./lib/id');

const SESSION_ID     = Buffer.from(crypto.randomUUID().replace(/-/g, ''), 'hex');
const MAX_INV_SLOTS  = 40;   // client-side inventory size
const AUTO_SLOT      = 0xFF; // targetSlot sentinel meaning "first free"

// ── HostAgent ─────────────────────────────────────────────────────────────────

class HostAgent {
    constructor() {
        this._socket     = null;
        this._decoder    = new FrameDecoder();
        this._state      = 'disconnected';
        this._stopped    = false;
        this._reconnectMs = 250;

        // Timers
        this._heartbeatTimer  = null;
        this._worldStateTimer = null;
        this._reconnectTimer  = null;

        this._tick     = 0;
        this._revision = parseInt(db.getWorldState('revision', '0'), 10);
        this._timeOfDay = parseFloat(db.getWorldState('timeOfDay', '600'));

        // playerId(BigInt) → { connectionId, entityId, inventory }
        // inventory: Array(MAX_INV_SLOTS) of null | { itemId, quantity }
        this._players = new Map();

        // entityId(BigInt) → { entityType, posX, posY, posZ, yaw, state }
        // Loaded from DB on start; kept in sync with DB as changes happen.
        this._entities = new Map();
        this._loadEntities();
    }

    start() {
        this._stopped = false;
        this._connect();
    }

    stop() {
        this._stopped = true;
        this._clearTimers();
        if (this._socket) { this._socket.destroy(); this._socket = null; }
        this._state = 'disconnected';
    }

    // ── private ────────────────────────────────────────────────────────────

    // Parse raw EntitySpawn frame bytes back into in-memory entity state.
    _loadEntities() {
        const { decodeFrame } = require('./lib/protocol');
        for (const frameBytes of db.getAllEntities()) {
            try {
                const frame = decodeFrame(frameBytes);
                if (!frame || frame.type !== MsgType.EntitySpawn) continue;
                const e = decodeEntitySpawn(frame.payload);
                this._entities.set(frame.entityId, e);
            } catch { /* corrupt row — ignore */ }
        }
        console.log(`[host] loaded ${this._entities.size} world entities from DB`);
    }

    _connect() {
        if (this._stopped) return;
        this._state = 'connecting';
        this._decoder.reset();

        const sock = net.createConnection(cfg.gatewayPort, '127.0.0.1');
        this._socket = sock;
        sock.on('connect', () => this._onConnect());
        sock.on('data',    (c) => this._onData(c));
        sock.on('error',   (e) => console.error(`[host] ${e.message}`));
        sock.on('close',   ()  => this._onClose());
    }

    _onConnect() {
        this._reconnectMs = 250;
        this._state = 'auth';
        console.log('[host] connected to gateway, authenticating …');
        this._socket.write(encodeFrame({
            type:      MsgType.HostAuthenticate,
            sessionId: SESSION_ID,
            worldId:   cfg.worldId,
            payload:   encodeString(cfg.hostSecret, 512),
        }));
    }

    _onData(chunk) {
        let frames;
        try { frames = this._decoder.push(chunk); }
        catch (e) { console.error(`[host] decode: ${e.message}`); this._socket.destroy(); return; }
        for (const f of frames) this._dispatch(f);
    }

    _dispatch(f) {
        if (this._state === 'auth') {
            if (f.type === MsgType.AuthenticationAccepted) {
                this._state = 'active';
                console.log('[host] authenticated – session ready');
                this._startTimers();
            } else if (f.type === MsgType.AuthenticationRejected) {
                const r = f.payload.length >= 2 ? decodeString(f.payload) : '';
                console.error(`[host] auth rejected: ${r}`);
                this._socket.destroy();
            }
            return;
        }

        if (this._state !== 'active') return;

        switch (f.type) {

        // ── Join / leave ──────────────────────────────────────────────────────

        case MsgType.JoinRequest: {
            const entityId = stableNumericId(`player-entity:${f.playerId}`);
            this._players.set(f.playerId, {
                connectionId: f.connectionId,
                entityId,
                inventory:    Array(MAX_INV_SLOTS).fill(null),
            });
            this._socket.write(encodeFrame({
                type:         MsgType.JoinAccepted,
                connectionId: f.connectionId,
                sessionId:    SESSION_ID,
                worldId:      cfg.worldId,
                playerId:     f.playerId,
                entityId,
                tick:         this._tick,
                payload:      encodeString('', 64),
            }));
            console.log(`[host] player ${f.playerId} joined  eid=${entityId}`);
            break;
        }

        case MsgType.PlayerDisconnected: {
            const p = this._players.get(f.playerId);
            if (!p) break;
            this._players.delete(f.playerId);
            this._socket.write(encodeFrame({
                type:         MsgType.PlayerDisconnected,
                connectionId: p.connectionId,
                sessionId:    SESSION_ID,
                worldId:      cfg.worldId,
                playerId:     f.playerId,
                entityId:     p.entityId,
                payload:      encodeString('player_left', 128),
            }));
            console.log(`[host] player ${f.playerId} left`);
            break;
        }

        // ── Death / respawn ───────────────────────────────────────────────────

        case MsgType.DeathRequest: {
            const p = this._players.get(f.playerId);
            if (!p) break;
            this._socket.write(encodeFrame({
                type:         MsgType.Death,
                connectionId: p.connectionId,
                sessionId:    SESSION_ID,
                worldId:      cfg.worldId,
                playerId:     f.playerId,
                entityId:     p.entityId,
                payload:      encodeString('host_confirmed_death', 128),
            }));
            break;
        }

        case MsgType.RespawnRequest: {
            const p = this._players.get(f.playerId);
            if (!p) break;
            this._socket.write(encodeFrame({
                type:         MsgType.Respawn,
                connectionId: p.connectionId,
                sessionId:    SESSION_ID,
                worldId:      cfg.worldId,
                playerId:     f.playerId,
                entityId:     p.entityId,
                payload:      encodeString('host_confirmed_respawn', 128),
            }));
            break;
        }

        // ── Player profile (client pushes its state so server can persist it) ─

        case MsgType.ProfileRevision:
            // The gateway already stored the raw payload to DB.
            // The host-agent updates the in-memory inventory by parsing it.
            this._applyProfileRevision(f);
            break;

        case MsgType.Equipment:
            // Equipment updates are informational for the host (future: validate loadout).
            break;

        // ── Item drop ─────────────────────────────────────────────────────────

        case MsgType.ItemDropRequest: {
            const p = this._players.get(f.playerId);
            if (!p) break;
            try {
                const req  = decodeItemDropRequest(f.payload);
                const slot = p.inventory[req.slotIndex];

                if (!slot || slot.itemId === 0) {
                    p.connectionId && this._sendTo(p.connectionId, MsgType.ItemDropResult,
                        f.playerId, p.entityId,
                        encodeItemDropResult({ success: false, reason: 0 })); // 0 = slot_empty
                    break;
                }

                const qty = Math.min(req.quantity, slot.quantity);
                slot.quantity -= qty;
                if (slot.quantity <= 0) p.inventory[req.slotIndex] = null;

                // Spawn a world entity for the dropped item.
                const entityId = this._randomEntityId();
                const spawnPayload = encodeEntitySpawn(
                    EntityType.LOOT_ITEM,
                    req.posX, req.posY, req.posZ, 0,
                    encodeLootState(slot.itemId, qty),
                );
                this._entities.set(entityId, {
                    entityType: EntityType.LOOT_ITEM,
                    posX: req.posX, posY: req.posY, posZ: req.posZ, yaw: 0,
                    state: encodeLootState(slot.itemId, qty),
                });

                const spawnFrame = encodeFrame({
                    type:      MsgType.EntitySpawn,
                    sessionId: SESSION_ID,
                    worldId:   cfg.worldId,
                    entityId,
                    payload:   spawnPayload,
                });
                // Gateway intercepts this and stores to DB automatically.
                this._socket.write(spawnFrame);

                this._sendTo(p.connectionId, MsgType.ItemDropResult,
                    f.playerId, p.entityId,
                    encodeItemDropResult({ success: true }));

                console.log(`[host] player ${f.playerId} dropped item ${slot.itemId}×${qty}  eid=${entityId}`);
            } catch (e) {
                console.error(`[host] ItemDropRequest: ${e.message}`);
            }
            break;
        }

        // ── Item pickup ───────────────────────────────────────────────────────

        case MsgType.ItemPickupRequest: {
            const p = this._players.get(f.playerId);
            if (!p) break;
            try {
                const req    = decodeItemPickupRequest(f.payload);
                const entity = this._entities.get(f.entityId);

                if (!entity || entity.entityType !== EntityType.LOOT_ITEM) {
                    this._sendTo(p.connectionId, MsgType.ItemPickupResult,
                        f.playerId, p.entityId,
                        encodeItemPickupResult({ success: false, reason: 0 })); // entity_gone
                    break;
                }

                const { itemId, quantity } = decodeLootState(entity.state);
                const targetSlot = req.targetSlot === AUTO_SLOT
                    ? p.inventory.findIndex(s => s === null)
                    : req.targetSlot;

                if (targetSlot < 0 || targetSlot >= MAX_INV_SLOTS) {
                    this._sendTo(p.connectionId, MsgType.ItemPickupResult,
                        f.playerId, p.entityId,
                        encodeItemPickupResult({ success: false, reason: 1 })); // inv_full
                    break;
                }

                // Add to inventory
                p.inventory[targetSlot] = { itemId, quantity };
                this._entities.delete(f.entityId);
                db.despawnEntity(f.entityId);

                // Tell all clients the entity is gone.
                this._socket.write(encodeFrame({
                    type:      MsgType.EntityDespawn,
                    sessionId: SESSION_ID,
                    worldId:   cfg.worldId,
                    entityId:  f.entityId,
                    payload:   Buffer.from([1]), // reason=1: picked_up
                }));

                this._sendTo(p.connectionId, MsgType.ItemPickupResult,
                    f.playerId, p.entityId,
                    encodeItemPickupResult({ success: true, slot: targetSlot, itemId, quantity }));

                console.log(`[host] player ${f.playerId} picked up item ${itemId}×${quantity}  slot=${targetSlot}`);
            } catch (e) {
                console.error(`[host] ItemPickupRequest: ${e.message}`);
            }
            break;
        }

        // ── Building / interactions ───────────────────────────────────────────

        case MsgType.InteractionRequest: {
            const p = this._players.get(f.playerId);
            if (!p) break;
            try {
                const req = decodeInteractionRequest(f.payload);

                if (req.interactionType === InteractionType.BUILD) {
                    const entityId = this._randomEntityId();
                    const state    = encodeBuildingState(req.pieceTypeId, 255);
                    const spawnPayload = encodeEntitySpawn(
                        EntityType.BUILDING_PIECE,
                        req.posX, req.posY, req.posZ, req.yaw,
                        state,
                    );

                    this._entities.set(entityId, {
                        entityType: EntityType.BUILDING_PIECE,
                        posX: req.posX, posY: req.posY, posZ: req.posZ, yaw: req.yaw,
                        state,
                    });

                    this._socket.write(encodeFrame({
                        type:      MsgType.EntitySpawn,
                        sessionId: SESSION_ID,
                        worldId:   cfg.worldId,
                        entityId,
                        payload:   spawnPayload,
                    }));

                    this._sendTo(p.connectionId, MsgType.InteractionResult,
                        f.playerId, p.entityId,
                        encodeInteractionResult({ success: true, interactionType: InteractionType.BUILD }));

                    console.log(`[host] player ${f.playerId} placed piece ${req.pieceTypeId}  eid=${entityId}`);
                }
            } catch (e) {
                console.error(`[host] InteractionRequest: ${e.message}`);
            }
            break;
        }

        case MsgType.Movement:
            // Already broadcast by gateway; host receives a copy for future server-side logic.
            break;

        } // switch
    }

    // ── Helpers ───────────────────────────────────────────────────────────────

    _sendTo(connectionId, type, playerId, entityId, payload) {
        this._socket.write(encodeFrame({
            type, connectionId, sessionId: SESSION_ID,
            worldId: cfg.worldId, playerId, entityId, payload,
        }));
    }

    _randomEntityId() {
        const bytes = crypto.randomBytes(8);
        const id    = bytes.readBigUInt64BE(0);
        return id === 0n ? 1n : id;
    }

    _applyProfileRevision(f) {
        const p = this._players.get(f.playerId);
        if (!p || f.payload.length < 51) return;
        try {
            const { slots } = decodePlayerProgress(f.payload);
            const inv = Array(MAX_INV_SLOTS).fill(null);
            for (const s of slots) {
                if (s.slotIndex < MAX_INV_SLOTS)
                    inv[s.slotIndex] = { itemId: s.itemId, quantity: s.quantity };
            }
            p.inventory = inv;
        } catch { /* malformed payload */ }
    }

    // ── Timers ────────────────────────────────────────────────────────────────

    _startTimers() {
        this._heartbeatTimer = setInterval(() => {
            if (this._state !== 'active') return;
            this._tick++;
            this._socket.write(encodeFrame({
                type:      MsgType.HostHeartbeat,
                sessionId: SESSION_ID,
                worldId:   cfg.worldId,
                flags:     1,
                tick:      this._tick,
            }));
        }, cfg.heartbeatMs);

        this._worldStateTimer = setInterval(() => {
            if (this._state !== 'active') return;
            this._broadcastWorldState();
        }, cfg.worldStateIntervalMs);
    }

    _broadcastWorldState() {
        const msPerGameHour = 3_600_000 / 4;
        this._timeOfDay = (this._timeOfDay + (cfg.worldStateIntervalMs / msPerGameHour) * 100) % 2400;
        this._revision++;

        // Persist every ~60 s (avoid thrashing on every 2 s interval).
        if (this._revision % 30 === 0) {
            db.setWorldState('timeOfDay', this._timeOfDay);
            db.setWorldState('revision',  this._revision);
        }

        const payload = Buffer.allocUnsafe(41);
        let o = 0;
        payload.writeUInt8(1, o);                               o += 1;
        payload.writeUInt32BE(this._revision, o);               o += 4;
        payload.writeBigUInt64BE(BigInt(Date.now()), o);        o += 8;
        payload.writeFloatBE(this._timeOfDay, o);               o += 4;
        payload.writeFloatBE(0.0, o);                           o += 4;
        payload.writeFloatBE(0.0, o);                           o += 4;
        payload.writeFloatBE(0.1, o);                           o += 4;
        payload.writeFloatBE(0.4, o);                           o += 4;
        payload.writeFloatBE(0.1, o);                           o += 4;
        payload.writeFloatBE(0.0, o);

        this._socket.write(encodeFrame({
            type:      MsgType.WorldState,
            sessionId: SESSION_ID,
            worldId:   cfg.worldId,
            sequence:  this._revision,
            tick:      this._tick,
            payload,
        }));
    }

    _onClose() {
        console.log('[host] disconnected from gateway');
        this._clearTimers();
        this._socket = null;
        this._state  = 'disconnected';
        this._decoder.reset();
        if (!this._stopped) {
            console.log(`[host] reconnecting in ${this._reconnectMs}ms`);
            this._reconnectTimer = setTimeout(() => {
                this._reconnectMs = Math.min(this._reconnectMs * 2, 5_000);
                this._connect();
            }, this._reconnectMs);
        }
    }

    _clearTimers() {
        if (this._heartbeatTimer)  { clearInterval(this._heartbeatTimer);  this._heartbeatTimer  = null; }
        if (this._worldStateTimer) { clearInterval(this._worldStateTimer); this._worldStateTimer = null; }
        if (this._reconnectTimer)  { clearTimeout(this._reconnectTimer);   this._reconnectTimer  = null; }
    }
}

module.exports = { HostAgent };
