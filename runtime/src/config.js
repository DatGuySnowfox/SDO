'use strict';

function envStr(name, def) {
    const v = process.env[name];
    return (v && v.trim()) ? v.trim() : def;
}

function envInt(name, def) {
    const v = parseInt(process.env[name], 10);
    return Number.isFinite(v) && v > 0 ? v : def;
}

module.exports = {
    // UDP – local game DLL side
    runtimePort:    envInt('SDO_RUNTIME_PORT',      42101), // runtime binds here; DLL sends here
    gameHost:       envStr('SDO_GAME_HOST',     '127.0.0.1'),
    bindPort:       envInt('SDO_BIND_PORT',         42100), // DLL binds here; runtime sends here

    // TCP – gateway server
    gatewayHost:    envStr('SDO_GATEWAY_HOST',  '127.0.0.1'),
    gatewayPort:    envInt('SDO_GATEWAY_PORT',      42200),

    // Auth – pre-issued join ticket (see server/issue-ticket.js for how to generate one)
    joinTicket:     envStr('SDO_JOIN_TICKET',  ''),

    // Player identity forwarded in the PlayerConnected seed frame
    displayName:    envStr('SDO_DISPLAY_NAME', 'Player'),

    // Timings
    heartbeatMs:    envInt('SDO_HEARTBEAT_MS',      1000),
    reconnectMs:    envInt('SDO_RECONNECT_MS',        250), // initial backoff
    maxReconnectMs: envInt('SDO_MAX_RECONNECT_MS',   5000),
};
