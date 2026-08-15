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
    // `f &&` used to treat a literal JSON `false` (falsy, but very much a
    // deliberate, defined value — e.g. ticketReplayProtection: false) as
    // "unset", silently falling through to def instead. 2026-08-13: this
    // is why ticketReplayProtection stayed enabled all night despite
    // settings.json explicitly setting it to false.
    if (f !== undefined && f !== null && String(f).trim()) return String(f).trim();
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

    // Per-client inbound frames/sec before the gateway drops the connection
    // as abusive. 2026-08-13: the default (120) was tuned before montage
    // sync existed — movement alone is already ~20/s at the default
    // SDB_MOVE_INTERVAL_MS, and a real melee combo now fires several
    // PlayMontage sends in well under a second on top of that, plus
    // whatever periodic equipment/appearance/attachment resyncs land in
    // the same window. Both PC1 and PC2 got server-side "rate limited"
    // and dropped mid-combat tonight at the old default — was previously
    // misdiagnosed as a crash/network issue before the rate-limit logging
    // added this session (gateway.js's _onData) made the real cause
    // visible. Bumped way up for this dev server; a production deployment
    // should retune this deliberately rather than inherit either number
    // blindly.
    clientRateLimit: int('SDB_CLIENT_RATE_LIMIT', 'clientRateLimit', 120),

    // Dev/LAN-testing escape hatch: tickets are normally single-use
    // (replay-protected), which means a client that already used its
    // ticket has no way to reconnect after any drop short of fetching a
    // brand new one out of band and relaunching the whole game — the
    // launcher is meant to own that refresh flow, but it doesn't exist
    // yet. Until it does, allow the same ticket to be replayed so the
    // built-in reconnect-with-backoff (tcp_client.cpp) can actually
    // recover on its own. Defaults to protected/off — only disable this
    // on a local or otherwise trusted dev server, never in production.
    ticketReplayProtection: str('SDB_TICKET_REPLAY_PROTECTION', 'ticketReplayProtection', 'true') !== 'false',

    worldIdStr,
    worldId: Buffer.from(worldIdStr.replace(/-/g, ''), 'hex'),

    heartbeatMs:          int('SDB_HEARTBEAT_MS',           'heartbeatMs',          500),
    worldStateIntervalMs: int('SDB_WORLD_STATE_INTERVAL_MS','worldStateIntervalMs', 2000),
    zombieTickIntervalMs: int('SDB_ZOMBIE_TICK_INTERVAL_MS','zombieTickIntervalMs', 2000),
    clientTimeoutMs:      int('SDB_CLIENT_TIMEOUT_MS',      'clientTimeoutMs',      15_000),
};
