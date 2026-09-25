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
    catch (e) { console.warn('SDO: could not parse settings.json:', e.message); }
}

function str(envName, fileKey, def) {
    const e = process.env[envName];
    if (e && e.trim()) return e.trim();
    const f = file[fileKey];
    // `f &&` used to treat a literal JSON `false` (falsy, but very much a
    // deliberate, defined value - e.g. ticketReplayProtection: false) as
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

const worldIdStr = str('SDO_WORLD_ID', 'worldId', crypto.randomUUID());

module.exports = {
    gatewayPort:    int('SDO_GATEWAY_PORT',  'gatewayPort',  42200),
    gatewayBind:    str('SDO_GATEWAY_BIND',  'gatewayBind',  '0.0.0.0'),

    httpPort:       int('SDO_HTTP_PORT',     'httpPort',     42201),
    httpBind:       str('SDO_HTTP_BIND',     'httpBind',     '0.0.0.0'),
    adminToken:     str('SDO_ADMIN_TOKEN',   'adminToken',   ''),

    hostSecret:     str('SDO_HOST_SECRET',   'hostSecret',   ''),
    ticketSecret:   str('SDO_TICKET_SECRET', 'ticketSecret', ''),

    ticketTtlMs:    int('SDO_TICKET_TTL_MS', 'ticketTtlMs',  120_000),
    maxPlayers:     int('SDO_MAX_PLAYERS',   'maxPlayers',   32),

    // Per-client inbound frames/sec before the gateway drops the connection
    // as abusive. 2026-08-13: the default (120) was tuned before montage
    // sync existed - movement alone is already ~20/s at the default
    // SDO_MOVE_INTERVAL_MS, and a real melee combo now fires several
    // PlayMontage sends in well under a second on top of that, plus
    // whatever periodic equipment/appearance/attachment resyncs land in
    // the same window. Both PC1 and PC2 got server-side "rate limited"
    // and dropped mid-combat tonight at the old default - was previously
    // misdiagnosed as a crash/network issue before the rate-limit logging
    // added this session (gateway.js's _onData) made the real cause
    // visible. Bumped way up for this dev server; a production deployment
    // should retune this deliberately rather than inherit either number
    // blindly.
    clientRateLimit: int('SDO_CLIENT_RATE_LIMIT', 'clientRateLimit', 120),

    // Dev/LAN-testing escape hatch: tickets are normally single-use
    // (replay-protected), which means a client that already used its
    // ticket has no way to reconnect after any drop short of fetching a
    // brand new one out of band and relaunching the whole game - the
    // launcher is meant to own that refresh flow, but it doesn't exist
    // yet. Until it does, allow the same ticket to be replayed so the
    // built-in reconnect-with-backoff (tcp_client.cpp) can actually
    // recover on its own. Defaults to protected/off - only disable this
    // on a local or otherwise trusted dev server, never in production.
    ticketReplayProtection: str('SDO_TICKET_REPLAY_PROTECTION', 'ticketReplayProtection', 'true') !== 'false',

    worldIdStr,
    worldId: Buffer.from(worldIdStr.replace(/-/g, ''), 'hex'),

    heartbeatMs:          int('SDO_HEARTBEAT_MS',           'heartbeatMs',          500),
    worldStateIntervalMs: int('SDO_WORLD_STATE_INTERVAL_MS','worldStateIntervalMs', 2000),
    zombieTickIntervalMs: int('SDO_ZOMBIE_TICK_INTERVAL_MS','zombieTickIntervalMs', 2000),
    clientTimeoutMs:      int('SDO_CLIENT_TIMEOUT_MS',      'clientTimeoutMs',      15_000),

    // Server-directory discovery (directory-worker/, a free Cloudflare
    // Worker) - opt-in: unset directoryUrl entirely disables it, so an
    // existing LAN-only setup is unaffected. directoryKey must match the
    // Worker's DIRECTORY_KEY secret. publicHost overrides auto-detection
    // (via a public IP-echo call, see gateway.js) for a DDNS hostname or a
    // NAT'd host where the detected IP isn't the one players should use.
    directoryUrl:          str('SDO_DIRECTORY_URL',           'directoryUrl',          ''),
    directoryKey:          str('SDO_DIRECTORY_KEY',           'directoryKey',          ''),
    directoryHeartbeatMs:  int('SDO_DIRECTORY_HEARTBEAT_MS',  'directoryHeartbeatMs',  60_000),
    serverName:            str('SDO_SERVER_NAME',             'serverName',            'SurrounDead Server'),
    publicHost:            str('SDO_PUBLIC_HOST',             'publicHost',            ''),

    // Ground-item lifecycle and BUILD-placement validation - ported from the
    // old SDO v3 alpha's host-agent, which had this world tuned from real
    // play (that codebase is not distributed with this repo). TTL/max-count
    // defaults match that alpha's
    // production values verbatim; buildMaxDistance reuses the same reach
    // distance that alpha used for both vehicle- and structure-placement
    // requests (this server has no player-initiated vehicle placement, only
    // BUILD, so it's applied there only).
    groundItemTtlMs:       int('SDO_GROUND_ITEM_TTL_MS',      'groundItemTtlMs',      6 * 60 * 60_000),
    groundItemMaxCount:    int('SDO_GROUND_ITEM_MAX_COUNT',   'groundItemMaxCount',   128),
    groundItemPruneMs:     int('SDO_GROUND_ITEM_PRUNE_MS',    'groundItemPruneMs',    60_000),
    buildMaxDistance:      int('SDO_BUILD_MAX_DISTANCE',      'buildMaxDistance',     600),
};
