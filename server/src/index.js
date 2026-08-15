'use strict';

// SurrounDead Bridge – dedicated server
//
// Required env vars:
//   SDB_HOST_SECRET    pre-shared key the host-agent uses with the gateway
//   SDB_TICKET_SECRET  HMAC key for signing/verifying join tickets
//
// Useful env vars:
//   SDB_WORLD_ID       stable UUID for this world (generated once if unset)
//   SDB_GATEWAY_PORT   TCP port for client + host-agent connections (default 42200)
//   SDB_HTTP_PORT      HTTP port for the ticket API (default 42201)
//   SDB_GATEWAY_BIND   bind address (default 0.0.0.0)
//   SDB_ADMIN_TOKEN    if set, POST /v1/tickets requires Authorization: Bearer <token>
//   SDB_MAX_PLAYERS    maximum concurrent joined players (default 32)

// Timestamp every console line (log/warn/error) with local HH:MM:SS.mmm —
// applies globally to this process, so gateway.js/host-agent.js need no
// changes of their own. Added 2026-08-13 after a long session of trying to
// correlate this log against the client's separately-timestamped-nothing
// SDB.log/debug.log by line-number proximity alone.
for (const level of ['log', 'warn', 'error']) {
    const orig = console[level].bind(console);
    console[level] = (...args) => {
        const d = new Date();
        const ts = [d.getHours(), d.getMinutes(), d.getSeconds()]
            .map((n) => String(n).padStart(2, '0')).join(':')
            + '.' + String(d.getMilliseconds()).padStart(3, '0');
        orig(`[${ts}]`, ...args);
    };
}

const cfg       = require('./config');
const { Gateway }   = require('./gateway');
const { HostAgent } = require('./host-agent');

if (!cfg.hostSecret || !cfg.ticketSecret) {
    console.error('SDB: SDB_HOST_SECRET and SDB_TICKET_SECRET must be set');
    process.exit(1);
}

console.log('SDB dedicated server');
console.log(`  world   ${cfg.worldIdStr}`);
console.log(`  tcp     ${cfg.gatewayBind}:${cfg.gatewayPort}`);
console.log(`  http    ${cfg.httpBind}:${cfg.httpPort}`);
console.log(`  max     ${cfg.maxPlayers} players`);

const gw   = new Gateway();
const host = new HostAgent();

// Start gateway first; once its TCP port is bound, start the local host-agent.
// The host-agent will retry connecting on its own if the gateway isn't ready yet.
gw.listen(() => host.start());

function shutdown() {
    console.log('\nSDB: shutting down');
    host.stop();
    process.exit(0);
}
process.on('SIGINT',  shutdown);
process.on('SIGTERM', shutdown);
