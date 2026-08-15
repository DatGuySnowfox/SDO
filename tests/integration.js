'use strict';

// Full integration test — starts the server in-process and exercises every
// major protocol flow with two fake players.
//
// Covered:
//   1. Server health check
//   2. Ticket issuance + HMAC verification
//   3. Client TCP authentication (AuthenticationAccepted)
//   4. Player join handshake (JoinAccepted)
//   5. Late-joiner replay  (PlayerConnected for existing players)
//   6. Movement relay      (A → gateway → B)
//   7. DeathRequest        (A → host → Death back to A)
//   8. RespawnRequest      (A → host → Respawn back to A)
//   9. WorldState broadcast (host → all clients)
//  10. Disconnect broadcast (A leaves → B gets PlayerDisconnected)
//  11. Duplicate-ticket replay protection
//  12. Bad-ticket rejection

const net    = require('node:net');
const http   = require('node:http');
const crypto = require('node:crypto');

// ── Test ports (choose high numbers to avoid conflicts) ───────────────────────
process.env.SDB_HOST_SECRET          = 'test-host-secret-abc123';
process.env.SDB_TICKET_SECRET        = 'test-ticket-secret-xyz987';
process.env.SDB_GATEWAY_PORT         = '43700';
process.env.SDB_HTTP_PORT            = '43701';
process.env.SDB_WORLD_ID             = 'deadbeef-cafe-4000-8000-123456789abc';
process.env.SDB_WORLD_STATE_INTERVAL_MS = '1000'; // faster for tests
process.env.SDB_HEARTBEAT_MS         = '200';
// Isolated throwaway DB — must not touch the real server/players.db.
process.env.SDB_DB_PATH = require('node:path').join(
    require('node:os').tmpdir(), `sdo_integration_test_${Date.now()}.db`);
// Small synthetic world (one zone at the origin) instead of the real
// 913-zone extracted data — keeps the zombie-spawn test deterministic and
// independent of whether server/scripts/extract-zombie-data.js has been run.
process.env.SDB_ZOMBIE_TICK_INTERVAL_MS = '300';
{
    const fs = require('node:fs');
    const wdPath = require('node:path').join(
        require('node:os').tmpdir(), `sdo_worlddata_test_${Date.now()}.json`);
    fs.writeFileSync(wdPath, JSON.stringify({
        spawnZones: [
            { kind: 'zombie', name: 'TestZone', x: 500, y: 500, z: 0,
              boxExtentX: 100, boxExtentY: 100, spawnAmount: 1, spawningInterval: 1 },
            { kind: 'vehicle', name: 'TestVehicleSpawn', x: 1000, y: 2000, z: 50, yaw: 90 },
        ],
        zombieStats: { BP_Zombie_Roamer: { health: { maxHealth: 100 }, roamingSpeed: 100 } },
        difficulty: { Standard: { ZombieHealthMultiplier: 1, ZombieSpeedMultiplier: 1, ZombieSpawnAmountMultiplier: 1 } },
    }));
    process.env.SDB_WORLD_DATA_PATH = wdPath;
}

const { Gateway }      = require('../server/src/gateway');
const { HostAgent }    = require('../server/src/host-agent');
const { FrameDecoder } = require('../server/src/lib/frame-decoder');
const { MsgType, EntityKind, InteractionType, encodeFrame, encodeString } = require('../server/src/lib/protocol');
const { verifyTicket } = require('../server/src/lib/ticket');
const cfg = require('../server/src/config');
const db  = require('../server/src/db');

// ── Tiny assertion engine ─────────────────────────────────────────────────────

let total = 0, failures = 0;

const feq = (a, b, eps = 1e-4) => Math.abs(a - b) < eps;

// EntitySpawn/EntityState payloads both start [tag=1][kind:u8]... — with the
// zombie-simulation background tick now also emitting these, tests that care
// about a specific entity kind need to filter, not just match on MsgType.
const byKind = (kind) => (f) => f.payload && f.payload.length > 1 && f.payload.readUInt8(1) === kind;

function ok(cond, label) {
    total++;
    if (cond) {
        console.log(`  PASS  ${label}`);
    } else {
        console.log(`  FAIL  ${label}`);
        failures++;
    }
}

// ── HTTP helpers ──────────────────────────────────────────────────────────────

const httpGet = (url) => new Promise((res, rej) =>
    http.get(url, (r) => { let d = ''; r.on('data', c => d += c); r.on('end', () => res(JSON.parse(d))); })
        .on('error', rej));

const httpPost = (url, body, token) => new Promise((res, rej) => {
    const b    = JSON.stringify(body);
    const u    = new URL(url);
    const hdrs = { 'Content-Type': 'application/json', 'Content-Length': b.length };
    if (token) hdrs['Authorization'] = `Bearer ${token}`;
    const req = http.request(
        { host: u.hostname, port: u.port, path: u.pathname, method: 'POST', headers: hdrs },
        (r) => { let d = ''; r.on('data', c => d += c); r.on('end', () => res({ status: r.statusCode, body: JSON.parse(d) })); }
    );
    req.on('error', rej);
    req.end(b);
});

// ── FakeClient — speaks binary protocol over TCP ──────────────────────────────

class FakeClient {
    constructor(name) {
        this.name    = name;
        this.socket  = null;
        this.decoder = new FrameDecoder();
        this._queue  = [];           // received frames not yet consumed
        this._waiters = [];          // { type, resolve, reject, timer }
        // session fields populated after auth/join
        this.connectionId = 0n;
        this.sessionId    = null;
        this.worldId      = null;
        this.playerId     = 0n;
        this.entityId     = 0n;
    }

    connect() {
        return new Promise((resolve, reject) => {
            const sock = net.createConnection(cfg.gatewayPort, '127.0.0.1');
            this.socket = sock;
            sock.on('connect', resolve);
            sock.on('error',   reject);
            sock.on('data', (chunk) => {
                let frames;
                try { frames = this.decoder.push(chunk); }
                catch (e) { console.error(`[${this.name}] decode: ${e.message}`); return; }
                for (const f of frames) this._receive(f);
            });
            sock.on('close', () => {
                for (const w of this._waiters) { clearTimeout(w.timer); w.reject(new Error('socket closed')); }
                this._waiters = [];
            });
        });
    }

    send(f) { this.socket.write(encodeFrame(f)); }

    // Resolves with the next frame of the given type (optionally matching a
    // predicate too — needed once more than one live source can emit the
    // same MsgType, e.g. the zombie-simulation background tick alongside a
    // specific item-drop/build test's own EntitySpawn/EntityState frames;
    // matching by type alone is racy there). Checks the queue first.
    waitFor(type, predicateOrTimeout, timeoutMs = 4000) {
        const predicate = typeof predicateOrTimeout === 'function' ? predicateOrTimeout : null;
        if (typeof predicateOrTimeout === 'number') timeoutMs = predicateOrTimeout;

        const matches = f => f.type === type && (!predicate || predicate(f));
        const idx = this._queue.findIndex(matches);
        if (idx >= 0) { const [f] = this._queue.splice(idx, 1); return Promise.resolve(f); }
        return new Promise((resolve, reject) => {
            const timer = setTimeout(() => {
                this._waiters = this._waiters.filter(w => w.resolve !== resolve);
                reject(new Error(`[${this.name}] timeout waiting for MsgType ${type}`));
            }, timeoutMs);
            this._waiters.push({ type, predicate, resolve, reject, timer });
        });
    }

    _receive(f) {
        const idx = this._waiters.findIndex(w => w.type === f.type && (!w.predicate || w.predicate(f)));
        if (idx >= 0) {
            const [w] = this._waiters.splice(idx, 1);
            clearTimeout(w.timer);
            w.resolve(f);
        } else {
            this._queue.push(f);
        }
    }

    // Authenticate + join in one call.
    async join(ticket) {
        this.send({ type: MsgType.ClientAuthenticate, payload: encodeString(ticket, 4096) });
        const authAck = await this.waitFor(MsgType.AuthenticationAccepted);
        this.connectionId = authAck.connectionId;
        this.sessionId    = authAck.sessionId;
        this.worldId      = authAck.worldId;
        this.playerId     = authAck.playerId;
        const joinAck = await this.waitFor(MsgType.JoinAccepted);
        this.entityId = joinAck.entityId;
        return { authAck, joinAck };
    }

    sendMovement(seq) {
        const payload = Buffer.alloc(39);
        payload.writeFloatBE(100 * seq, 0);  // x
        payload.writeFloatBE(200 * seq, 4);  // y
        payload.writeFloatBE(50,        8);  // z
        this.send({
            type:         MsgType.Movement,
            connectionId: this.connectionId,
            sessionId:    this.sessionId,
            worldId:      this.worldId,
            playerId:     this.playerId,
            entityId:     this.entityId,
            sequence:     seq,
            payload,
        });
    }

    close() { if (this.socket && !this.socket.destroyed) this.socket.destroy(); }
}

// ── Issue ticket helper ───────────────────────────────────────────────────────

const issueTicket = (playerId, displayName) =>
    httpPost('http://127.0.0.1:43701/v1/tickets', { playerId, displayName })
        .then(r => r.body);

// ── Main ──────────────────────────────────────────────────────────────────────

async function runTests() {
    console.log('╔══════════════════════════════════════╗');
    console.log('║   SDB Integration Test Suite         ║');
    console.log('╚══════════════════════════════════════╝\n');

    // ── Start server ──────────────────────────────────────────────────────────
    const gw   = new Gateway();
    const host = new HostAgent();
    await new Promise(r => gw.listen(r));
    host.start();
    // Give host-agent time to authenticate with the gateway.
    await new Promise(r => setTimeout(r, 400));

    // ── 1. Health check ───────────────────────────────────────────────────────
    console.log('── 1. Server health ─────────────────────');
    const health = await httpGet('http://127.0.0.1:43701/v1/health');
    ok(health.ok === true,        'health.ok is true');
    ok(health.hostOnline === true,'host-agent is online');
    ok(health.players === 0,      'zero players at start');

    // ── 2. Ticket issuance ────────────────────────────────────────────────────
    console.log('\n── 2. Ticket issuance ───────────────────');
    const aliceTR = await issueTicket('alice', 'Alice');
    const bobTR   = await issueTicket('bob',   'Bob');

    ok(typeof aliceTR.ticket === 'string' && aliceTR.ticket.length > 20, 'alice ticket issued');
    ok(typeof bobTR.ticket   === 'string' && bobTR.ticket.length   > 20, 'bob ticket issued');
    ok(aliceTR.worldId === cfg.worldIdStr, 'ticket worldId correct');
    ok(aliceTR.gatewayPort === cfg.gatewayPort, 'ticket gatewayPort correct');

    // Verify ticket HMAC round-trip
    const aliceBody = verifyTicket(aliceTR.ticket, process.env.SDB_TICKET_SECRET, cfg.worldIdStr);
    ok(aliceBody.playerId     === 'alice', 'ticket playerId preserved');
    ok(aliceBody.displayName  === 'Alice', 'ticket displayName preserved');
    ok(aliceBody.expiresAtMs  >  Date.now(), 'ticket not already expired');

    // ── 3 + 4. Authentication & join ─────────────────────────────────────────
    console.log('\n── 3+4. Auth + join ─────────────────────');
    const alice = new FakeClient('alice');
    const bob   = new FakeClient('bob');
    await alice.connect();
    await bob.connect();

    const { authAck: aliceAuth, joinAck: aliceJoin } = await alice.join(aliceTR.ticket);
    ok(aliceAuth.connectionId !== 0n, 'alice got connectionId');
    ok(aliceAuth.playerId     !== 0n, 'alice got playerId');
    ok(aliceJoin.entityId     !== 0n, 'alice got entityId');

    const { authAck: bobAuth, joinAck: bobJoin } = await bob.join(bobTR.ticket);
    ok(bobAuth.playerId   !== 0n, 'bob got playerId');
    ok(bobJoin.entityId   !== 0n, 'bob got entityId');
    ok(alice.playerId !== bob.playerId, 'alice and bob have different playerIds');

    // ── 4b. Vehicle entity replay ─────────────────────────────────────────────
    // _broadcastVehicles() fires once at host-agent's own authentication,
    // before any real client has joined — nobody catches it live. Verifies
    // the vehicle still reaches a joining client via the persisted-entity
    // replay path (gateway.js's _replayTo, reconstructed from the unified
    // entities table), the same mechanism GroundItem/PlacedStructure/Zombie
    // entities already rely on for late joiners.
    console.log('\n── 4b. Vehicle entity replay ────────────');
    {
        const vSpawn = await alice.waitFor(MsgType.EntitySpawn, byKind(EntityKind.Vehicle), 3000);
        const vState = await alice.waitFor(MsgType.EntityState, byKind(EntityKind.Vehicle), 3000);
        ok(vSpawn.entityId !== 0n, 'alice received EntitySpawn for the vehicle via replay');
        ok(feq(vState.payload.readFloatBE(6), 1000),  'vehicle x preserved through replay');
        ok(feq(vState.payload.readFloatBE(10), 2000), 'vehicle y preserved through replay');
        ok(feq(vState.payload.readFloatBE(22), 100),  'vehicle health defaults to 100');

        const stored = db.getEntity(vSpawn.entityId);
        ok(stored !== null && stored.kind === EntityKind.Vehicle, 'vehicle entity landed in unified table with kind=Vehicle');
    }

    // ── 5. Late-joiner replay (Alice sees Bob's PlayerConnected) ──────────────
    console.log('\n── 5. Late-joiner replay ────────────────');
    // When Bob joined, the gateway broadcast PlayerConnected(Bob) to Alice.
    const aliceSeeBob = await alice.waitFor(MsgType.PlayerConnected);
    ok(aliceSeeBob.playerId === bob.playerId, 'alice received PlayerConnected for bob');
    ok(aliceSeeBob.entityId === bob.entityId, 'entityId correct in PlayerConnected');

    // Bob should have received PlayerConnected(Alice) via _replayTo().
    const bobSeeAlice = await bob.waitFor(MsgType.PlayerConnected);
    ok(bobSeeAlice.playerId === alice.playerId, 'bob received replayed PlayerConnected for alice');

    // ── 6. Movement relay ─────────────────────────────────────────────────────
    console.log('\n── 6. Movement relay ────────────────────');
    alice.sendMovement(1);
    const bobGotMov = await bob.waitFor(MsgType.Movement);
    ok(bobGotMov.playerId === alice.playerId,       'movement has alice playerId');
    ok(bobGotMov.payload.length === 39,             'movement payload 39 bytes');
    ok(bobGotMov.payload.readFloatBE(0) === 100.0,  'movement x=100 preserved');

    bob.sendMovement(1);
    const aliceGotMov = await alice.waitFor(MsgType.Movement);
    ok(aliceGotMov.playerId === bob.playerId, 'alice receives bob movement');

    // ── 7. DeathRequest → Death ───────────────────────────────────────────────
    console.log('\n── 7. Death flow ────────────────────────');
    alice.send({
        type: MsgType.DeathRequest,
        connectionId: alice.connectionId,
        sessionId:    alice.sessionId,
        worldId:      alice.worldId,
        playerId:     alice.playerId,
        entityId:     alice.entityId,
    });
    const aliceDeath = await alice.waitFor(MsgType.Death);
    ok(aliceDeath !== null,                        'alice received Death confirmation');
    ok(aliceDeath.playerId === alice.playerId,     'Death has correct playerId');

    // ── 8. RespawnRequest → Respawn ───────────────────────────────────────────
    console.log('\n── 8. Respawn flow ──────────────────────');
    alice.send({
        type: MsgType.RespawnRequest,
        connectionId: alice.connectionId,
        sessionId:    alice.sessionId,
        worldId:      alice.worldId,
        playerId:     alice.playerId,
        entityId:     alice.entityId,
    });
    const aliceRespawn = await alice.waitFor(MsgType.Respawn);
    ok(aliceRespawn !== null,                       'alice received Respawn confirmation');
    ok(aliceRespawn.playerId === alice.playerId,    'Respawn has correct playerId');

    // ── 9. WorldState broadcast ───────────────────────────────────────────────
    console.log('\n── 9. WorldState broadcast ──────────────');
    // Host sends WorldState every SDB_WORLD_STATE_INTERVAL_MS (1000 ms for tests).
    const aliceWS = await alice.waitFor(MsgType.WorldState, 3000);
    ok(aliceWS !== null,                  'alice received WorldState');
    ok(aliceWS.payload.length === 41,     'WorldState payload is 41 bytes');
    ok(aliceWS.payload.readUInt8(0) === 1,'WorldState format byte = 1');
    const timeOfDay = aliceWS.payload.readFloatBE(13);
    ok(timeOfDay >= 0 && timeOfDay < 2400,'WorldState timeOfDay in range 0-2400');

    const bobWS = await bob.waitFor(MsgType.WorldState, 500);
    ok(bobWS !== null, 'bob also received WorldState');

    // ── 9b. Item drop + pickup roundtrip (unified `entities` table) ──────────
    // Exercises the entity persistence path migrated onto db.js's new
    // structured `entities` table (gateway.js EntitySpawn/EntityState/
    // EntityDespawn handling) — not covered by any earlier section.
    console.log('\n── 9b. Item drop + pickup ───────────────');
    {
        const itemId = 'DA_TestWidget';
        const dropPayload = Buffer.alloc(17 + itemId.length);
        dropPayload.writeUInt8(1, 0);           // version
        dropPayload.writeUInt16BE(3, 1);        // quantity
        dropPayload.writeFloatBE(10, 3);        // posX
        dropPayload.writeFloatBE(20, 7);        // posY
        dropPayload.writeFloatBE(30, 11);       // posZ
        dropPayload.writeUInt16BE(itemId.length, 15);
        dropPayload.write(itemId, 17, 'utf8');

        bob.send({
            type: MsgType.ItemDropRequest,
            connectionId: bob.connectionId, sessionId: bob.sessionId, worldId: bob.worldId,
            playerId: bob.playerId, entityId: bob.entityId,
            payload: dropPayload,
        });

        const dropResult = await bob.waitFor(MsgType.ItemDropResult);
        const dropJson = JSON.parse(dropResult.payload.subarray(2).toString('utf8'));
        ok(dropJson.success === true, 'item drop succeeded');

        const spawnFrame = await alice.waitFor(MsgType.EntitySpawn, byKind(EntityKind.GroundItem));
        ok(spawnFrame.entityId !== 0n, 'alice received EntitySpawn for dropped item');
        const stateFrame = await alice.waitFor(MsgType.EntityState, byKind(EntityKind.GroundItem));
        ok(stateFrame.payload.readFloatBE(6)  === 10, 'EntityState x preserved through entities table');
        ok(stateFrame.payload.readFloatBE(10) === 20, 'EntityState y preserved through entities table');

        // Confirm it actually landed in the new structured table, not just on the wire.
        const stored = db.getEntity(spawnFrame.entityId);
        ok(stored !== null,                       'entity row exists in unified entities table');
        ok(stored.kind === EntityKind.GroundItem, 'stored kind is GroundItem');
        ok(stored.attributes.itemId === itemId,   'stored attributes.itemId matches dropped item');
        ok(stored.attributes.quantity === 3,      'stored attributes.quantity matches dropped quantity');
        ok(stored.x === 10 && stored.y === 20,    'stored position matches EntityState');

        // Alice picks it up.
        const pickupPayload = Buffer.from([1, 0xFF]); // version=1, targetSlot=AUTO_SLOT
        alice.send({
            type: MsgType.ItemPickupRequest,
            connectionId: alice.connectionId, sessionId: alice.sessionId, worldId: alice.worldId,
            playerId: alice.playerId, entityId: spawnFrame.entityId,
            payload: pickupPayload,
        });
        const pickupResult = await alice.waitFor(MsgType.ItemPickupResult);
        const pickupJson = JSON.parse(pickupResult.payload.subarray(2).toString('utf8'));
        ok(pickupJson.success === true,       'item pickup succeeded');
        ok(pickupJson.itemId === itemId,      'pickup result itemId matches');

        const despawnFrame = await alice.waitFor(MsgType.EntityDespawn, f => f.entityId === spawnFrame.entityId);
        ok(despawnFrame.entityId === spawnFrame.entityId, 'EntityDespawn matches picked-up entity');
        ok(db.getEntity(spawnFrame.entityId) === null,    'entity row removed from unified table after pickup');
    }

    // ── 9c. Building placement (InteractionRequest/BUILD, itemId-based) ──────
    // Verifies the migration off the old meaningless numeric pieceTypeId
    // onto a real itemId (research/04_ida_investigation_log.md Session 58 —
    // BuildActorClass resolves off the same DataAsset GroundItem already
    // uses) and that PlacedStructure lands correctly in the unified table.
    console.log('\n── 9c. Building placement ───────────────');
    {
        const itemId = 'DA_WoodenWall';
        const buildPayload = Buffer.alloc(20 + itemId.length);
        buildPayload.writeUInt8(1, 0);                       // version
        buildPayload.writeUInt8(InteractionType.BUILD, 1);   // interactionType
        buildPayload.writeFloatBE(50, 2);                    // posX
        buildPayload.writeFloatBE(60, 6);                    // posY
        buildPayload.writeFloatBE(0, 10);                    // posZ
        buildPayload.writeFloatBE(90, 14);                   // yaw
        buildPayload.writeUInt16BE(itemId.length, 18);
        buildPayload.write(itemId, 20, 'utf8');

        bob.send({
            type: MsgType.InteractionRequest,
            connectionId: bob.connectionId, sessionId: bob.sessionId, worldId: bob.worldId,
            playerId: bob.playerId, entityId: bob.entityId,
            payload: buildPayload,
        });

        const buildResult = await bob.waitFor(MsgType.InteractionResult);
        ok(buildResult.payload.readUInt8(1) === 1, 'build request succeeded');
        ok(buildResult.payload.readUInt8(2) === InteractionType.BUILD, 'InteractionResult.interactionType == BUILD');

        const spawnFrame = await alice.waitFor(MsgType.EntitySpawn, byKind(EntityKind.PlacedStructure));
        const stateFrame = await alice.waitFor(MsgType.EntityState, byKind(EntityKind.PlacedStructure));
        ok(feq(stateFrame.payload.readFloatBE(6), 50),  'placed piece x preserved');
        ok(feq(stateFrame.payload.readFloatBE(18), 90), 'placed piece yaw preserved');

        const stored = db.getEntity(spawnFrame.entityId);
        ok(stored !== null,                              'placed structure row exists in unified table');
        ok(stored.kind === EntityKind.PlacedStructure,    'stored kind is PlacedStructure');
        ok(stored.attributes.itemId === itemId,           'stored attributes.itemId is the real DataAsset itemId, not a numeric placeholder');
    }

    // ── 9d. Zombie simulation (spawn near a player, damage, death) ───────────
    // End-to-end wiring check on top of tests/zombie-simulation.js's own
    // pure-state-machine unit tests — exercises the actual host-agent tick
    // timer, frame encoding, and gateway relay/persistence path together.
    console.log('\n── 9d. Zombie simulation ────────────────');
    {
        // Move bob near the synthetic TestZone (500,500) so it's in relevance range.
        const movePayload = Buffer.alloc(39);
        movePayload.writeFloatBE(500, 0);
        movePayload.writeFloatBE(500, 4);
        movePayload.writeFloatBE(0, 8);
        bob.send({
            type: MsgType.Movement,
            connectionId: bob.connectionId, sessionId: bob.sessionId, worldId: bob.worldId,
            playerId: bob.playerId, entityId: bob.entityId,
            payload: movePayload,
        });
        // Drain the Movement relay both clients get from this, so it doesn't
        // get mistaken for the zombie's EntitySpawn by waitFor below.
        await alice.waitFor(MsgType.Movement);

        const zSpawn = await alice.waitFor(MsgType.EntitySpawn, byKind(EntityKind.Zombie), 3000);
        const zState = await alice.waitFor(MsgType.EntityState, byKind(EntityKind.Zombie), 3000);
        ok(zSpawn.entityId !== 0n, 'zombie EntitySpawn received near the moved player');
        ok(feq(zState.payload.readFloatBE(22), 100), `zombie spawns at full archetype health (100), got ${zState.payload.readFloatBE(22)}`);

        const stored = db.getEntity(zSpawn.entityId);
        ok(stored !== null && stored.kind === EntityKind.Zombie, 'zombie entity landed in unified table with kind=Zombie');

        // Attack it — entityId is the frame-header field, damage is JSON payload.
        const dmgJson = Buffer.from(JSON.stringify({ damage: 30 }), 'utf8');
        const dmgPayload = Buffer.alloc(2 + dmgJson.length);
        dmgPayload.writeUInt16BE(dmgJson.length, 0);
        dmgJson.copy(dmgPayload, 2);
        alice.send({
            type: MsgType.ZombieAttackRequest,
            connectionId: alice.connectionId, sessionId: alice.sessionId, worldId: alice.worldId,
            playerId: alice.playerId, entityId: zSpawn.entityId,
            payload: dmgPayload,
        });

        const dmgResult = await alice.waitFor(MsgType.ZombieDamageResult);
        const dmgResultJson = JSON.parse(dmgResult.payload.subarray(2).toString('utf8'));
        ok(dmgResultJson.newHealth === 70, `ZombieDamageResult reflects 100-30=70 health, got ${dmgResultJson.newHealth}`);
        ok(dmgResultJson.dead === false, 'not dead yet at 70/100 hp');

        // Finish it off.
        const dmgJson2 = Buffer.from(JSON.stringify({ damage: 1000 }), 'utf8');
        const dmgPayload2 = Buffer.alloc(2 + dmgJson2.length);
        dmgPayload2.writeUInt16BE(dmgJson2.length, 0);
        dmgJson2.copy(dmgPayload2, 2);
        alice.send({
            type: MsgType.ZombieAttackRequest,
            connectionId: alice.connectionId, sessionId: alice.sessionId, worldId: alice.worldId,
            playerId: alice.playerId, entityId: zSpawn.entityId,
            payload: dmgPayload2,
        });

        const killResult = await alice.waitFor(MsgType.ZombieDamageResult);
        const killResultJson = JSON.parse(killResult.payload.subarray(2).toString('utf8'));
        ok(killResultJson.dead === true, 'overkill damage reports dead=true');

        const zDespawn = await alice.waitFor(MsgType.EntityDespawn, f => f.entityId === zSpawn.entityId, 3000);
        ok(zDespawn.entityId === zSpawn.entityId, 'EntityDespawn matches the killed zombie');
        ok(db.getEntity(zSpawn.entityId) === null, 'killed zombie removed from unified table');
    }

    // ── 10. Disconnect broadcast ──────────────────────────────────────────────
    console.log('\n── 10. Disconnect broadcast ─────────────');
    alice.close();
    const bobPD = await bob.waitFor(MsgType.PlayerDisconnected, 3000);
    ok(bobPD !== null,                       'bob received PlayerDisconnected');
    ok(bobPD.playerId === alice.playerId,    'PlayerDisconnected has alice playerId');

    // Wait a moment to verify bob does NOT get a second PlayerDisconnected
    // (double-broadcast bug guard).
    await new Promise(r => setTimeout(r, 300));
    const extra = bob._queue.filter(f => f.type === MsgType.PlayerDisconnected);
    ok(extra.length === 0, 'no duplicate PlayerDisconnected sent');

    // ── 11. Duplicate ticket replay ───────────────────────────────────────────
    console.log('\n── 11. Ticket replay protection ─────────');
    const charlie1 = new FakeClient('charlie-1');
    const charlie2 = new FakeClient('charlie-2');
    const charlieTR = await issueTicket('charlie', 'Charlie');

    await charlie1.connect();
    charlie1.send({ type: MsgType.ClientAuthenticate, payload: encodeString(charlieTR.ticket, 4096) });
    await charlie1.waitFor(MsgType.AuthenticationAccepted);

    await charlie2.connect();
    charlie2.send({ type: MsgType.ClientAuthenticate, payload: encodeString(charlieTR.ticket, 4096) });
    // Should get AuthenticationRejected (ticket replay)
    const replayRej = await charlie2.waitFor(MsgType.AuthenticationRejected, 2000)
        .catch(() => null);
    ok(replayRej !== null, 'replayed ticket rejected');
    charlie1.close();
    charlie2.close();

    // ── 12. Invalid ticket rejection ──────────────────────────────────────────
    console.log('\n── 12. Bad ticket rejection ─────────────');
    const danRaw = await issueTicket('dan', 'Dan');
    // Tamper with the ticket (flip one char in the signature)
    const parts = danRaw.ticket.split('.');
    parts[1] = parts[1].slice(0, -1) + (parts[1].slice(-1) === 'A' ? 'B' : 'A');
    const badTicket = parts.join('.');

    const dan = new FakeClient('dan');
    await dan.connect();
    dan.send({ type: MsgType.ClientAuthenticate, payload: encodeString(badTicket, 4096) });
    const danRej = await dan.waitFor(MsgType.AuthenticationRejected, 2000).catch(() => null);
    ok(danRej !== null, 'tampered ticket rejected');
    dan.close();

    // ── Summary ───────────────────────────────────────────────────────────────
    bob.close();
    console.log('\n══════════════════════════════════════════');
    console.log(`  ${total - failures} / ${total} tests passed`);
    if (failures > 0) console.log(`  ${failures} FAILED`);
    console.log('══════════════════════════════════════════');

    host.stop();
    process.exit(failures > 0 ? 1 : 0);
}

runTests().catch((e) => {
    console.error('\nUnhandled test error:', e.message);
    console.error(e.stack);
    process.exit(1);
});
