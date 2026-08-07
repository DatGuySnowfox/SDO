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

const { Gateway }      = require('../server/src/gateway');
const { HostAgent }    = require('../server/src/host-agent');
const { FrameDecoder } = require('../server/src/lib/frame-decoder');
const { MsgType, encodeFrame, encodeString } = require('../server/src/lib/protocol');
const { verifyTicket } = require('../server/src/lib/ticket');
const cfg = require('../server/src/config');

// ── Tiny assertion engine ─────────────────────────────────────────────────────

let total = 0, failures = 0;

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

    // Resolves with the next frame of the given type; checks queue first.
    waitFor(type, timeoutMs = 4000) {
        const idx = this._queue.findIndex(f => f.type === type);
        if (idx >= 0) { const [f] = this._queue.splice(idx, 1); return Promise.resolve(f); }
        return new Promise((resolve, reject) => {
            const timer = setTimeout(() => {
                this._waiters = this._waiters.filter(w => w.resolve !== resolve);
                reject(new Error(`[${this.name}] timeout waiting for MsgType ${type}`));
            }, timeoutMs);
            this._waiters.push({ type, resolve, reject, timer });
        });
    }

    _receive(f) {
        const idx = this._waiters.findIndex(w => w.type === f.type);
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
