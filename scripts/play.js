'use strict';
// One-shot helper: fetch a ticket and launch SurrounDead via Steam.
// Usage: node play.js [nickname]
// Reads server config from server/settings.json.

const http   = require('http');
const crypto = require('crypto');
const { execSync, spawn } = require('child_process');

const STEAM_APP_ID   = '1645820';
const TICKET_URL     = 'http://127.0.0.1:42201/v1/tickets';
const NICKNAME       = process.argv[2] || 'Player';
const PLAYER_ID      = (() => {
    // Stable ID based on machine hostname so the same player ID is reused across sessions.
    return BigInt('0x' + crypto.createHash('sha256').update(require('os').hostname()).digest('hex').slice(0,16)).toString();
})();

function post(url, body) {
    return new Promise((resolve, reject) => {
        const data = JSON.stringify(body);
        const u = new URL(url);
        const req = http.request({
            host: u.hostname, port: u.port, path: u.pathname,
            method: 'POST',
            headers: { 'Content-Type': 'application/json', 'Content-Length': Buffer.byteLength(data) }
        }, (res) => {
            let s = '';
            res.on('data', c => s += c);
            res.on('end', () => {
                try { resolve({ status: res.statusCode, body: JSON.parse(s) }); }
                catch (e) { reject(new Error('Bad JSON: ' + s)); }
            });
        });
        req.on('error', reject);
        req.write(data);
        req.end();
    });
}

function setEnv(name, value) {
    execSync(`setx ${name} "${value}"`, { stdio: 'ignore' });
}

async function main() {
    console.log('Fetching ticket from server...');
    let res;
    try {
        res = await post(TICKET_URL, { playerId: PLAYER_ID, displayName: NICKNAME });
    } catch (e) {
        console.error('Could not reach server:', e.message);
        console.error('Make sure the server is running (run start-server.bat first).');
        process.exit(1);
    }

    if (res.status !== 200) {
        console.error('Server error:', res.body.error || res.status);
        process.exit(1);
    }

    const { ticket, gatewayHost, gatewayPort } = res.body;
    console.log(`Got ticket (host=${gatewayHost}:${gatewayPort})`);

    setEnv('SDB_JOIN_TICKET',  ticket);
    setEnv('SDB_GATEWAY_HOST', gatewayHost);
    setEnv('SDB_GATEWAY_PORT', String(gatewayPort));
    console.log('Environment variables set.');

    console.log('Launching SurrounDead...');
    spawn('cmd', ['/c', `start steam://rungameid/${STEAM_APP_ID}`], { detached: true, stdio: 'ignore' }).unref();
    console.log('Done. Game launching via Steam.');
}

main();
