'use strict';

const crypto = require('node:crypto');
const fs     = require('node:fs');
const path   = require('node:path');

// Load settings.json from the server root (one level up from src/).
// Environment variables always override the file.
let file = {};
const filePath = path.join(__dirname, '..', 'settings.json');
if (fs.existsSync(filePath)) {
    try { file = JSON.parse(fs.readFileSync(filePath, 'utf8')); }
    catch (e) { console.warn('SDB: could not parse settings.json:', e.message); }
}

function str(envName, fileKey, def) {
    const e = process.env[envName];
    if (e && e.trim()) return e.trim();
    const f = file[fileKey];
    if (f && String(f).trim()) return String(f).trim();
    return def;
}
function int(envName, fileKey, def) {
    const e = parseInt(process.env[envName], 10);
    if (Number.isFinite(e) && e > 0) return e;
    const f = parseInt(file[fileKey], 10);
    if (Number.isFinite(f) && f > 0) return f;
    return def;
}

const worldIdStr = str('SDB_WORLD_ID', 'worldId', crypto.randomUUID());

module.exports = {
    gatewayPort:    int('SDB_GATEWAY_PORT',  'gatewayPort',  42200),
    gatewayBind:    str('SDB_GATEWAY_BIND',  'gatewayBind',  '0.0.0.0'),

    httpPort:       int('SDB_HTTP_PORT',     'httpPort',     42201),
    httpBind:       str('SDB_HTTP_BIND',     'httpBind',     '0.0.0.0'),
    adminToken:     str('SDB_ADMIN_TOKEN',   'adminToken',   ''),

    hostSecret:     str('SDB_HOST_SECRET',   'hostSecret',   ''),
    ticketSecret:   str('SDB_TICKET_SECRET', 'ticketSecret', ''),

    ticketTtlMs:    int('SDB_TICKET_TTL_MS', 'ticketTtlMs',  120_000),
    maxPlayers:     int('SDB_MAX_PLAYERS',   'maxPlayers',   32),

    worldIdStr,
    worldId: Buffer.from(worldIdStr.replace(/-/g, ''), 'hex'),

    heartbeatMs:          int('SDB_HEARTBEAT_MS',           'heartbeatMs',          500),
    worldStateIntervalMs: int('SDB_WORLD_STATE_INTERVAL_MS','worldStateIntervalMs', 2000),
    clientTimeoutMs:      int('SDB_CLIENT_TIMEOUT_MS',      'clientTimeoutMs',      15_000),
};
