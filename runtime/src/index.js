'use strict';

// SurrounDead Bridge – client-side runtime
//
// Relays binary protocol v3 frames between the game DLL (UDP) and the
// gateway server (TCP).  Set env vars before launching:
//
//   SDB_GATEWAY_HOST    gateway TCP address           (default 127.0.0.1)
//   SDB_GATEWAY_PORT    gateway TCP port              (default 42200)
//   SDB_JOIN_TICKET     pre-issued join ticket        (required)
//   SDB_DISPLAY_NAME    name shown to other players   (default "Player")
//   SDB_RUNTIME_PORT    UDP port DLL sends frames to  (default 42101)
//   SDB_BIND_PORT       UDP port DLL receives frames  (default 42100)

const cfg         = require('./config');
const { UdpBridge }  = require('./udp-bridge');
const { TcpSession } = require('./tcp-session');

if (!cfg.joinTicket) {
    console.error('SDB: SDB_JOIN_TICKET is required. Obtain one from the gateway operator.');
    process.exit(1);
}

console.log('SDB bridge runtime');
console.log(`  udp  bind  127.0.0.1:${cfg.runtimePort}  (← game DLL sends here)`);
console.log(`  udp  game  ${cfg.gameHost}:${cfg.bindPort}  (→ game DLL receives here)`);
console.log(`  tcp  gw    ${cfg.gatewayHost}:${cfg.gatewayPort}`);

// tcp and udp reference each other via callbacks; both are defined before
// either callback can fire (sockets open asynchronously).
const tcp = new TcpSession((buf) => udp.send(buf));
const udp = new UdpBridge( (buf) => tcp.onGameFrame(buf));

udp.open();
tcp.start();

function shutdown() {
    console.log('\nSDB: shutting down');
    tcp.stop();
    udp.close();
    process.exit(0);
}

process.on('SIGINT',  shutdown);
process.on('SIGTERM', shutdown);
