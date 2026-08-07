'use strict';
// Run once to create settings.json with auto-generated secrets.
// Re-run any time to add missing fields without touching existing ones.

const crypto = require('node:crypto');
const fs     = require('node:fs');
const path   = require('node:path');

const file = path.join(__dirname, 'settings.json');

let existing = {};
if (fs.existsSync(file)) {
    try { existing = JSON.parse(fs.readFileSync(file, 'utf8')); }
    catch { console.error('Could not parse existing settings.json — starting fresh.'); }
}

const settings = {
    // ── Required secrets (auto-generated, do not share) ──────────────────────
    hostSecret:   existing.hostSecret   || crypto.randomBytes(32).toString('hex'),
    ticketSecret: existing.ticketSecret || crypto.randomBytes(32).toString('hex'),

    // ── World identity (keep stable across restarts) ──────────────────────────
    worldId:      existing.worldId      || crypto.randomUUID(),

    // ── Access control ────────────────────────────────────────────────────────
    // Leave adminToken blank to allow anyone to connect.
    // Set it to a password and share it with your players.
    adminToken:   existing.adminToken   ?? '',

    // ── Limits ────────────────────────────────────────────────────────────────
    maxPlayers:   existing.maxPlayers   ?? 32,

    // ── Ports ─────────────────────────────────────────────────────────────────
    // gatewayPort: TCP port clients connect to (must be open/forwarded)
    // httpPort:    HTTP port the launcher uses to fetch join tickets
    gatewayPort:  existing.gatewayPort  ?? 42200,
    httpPort:     existing.httpPort     ?? 42201,
};

fs.writeFileSync(file, JSON.stringify(settings, null, 4), 'utf8');

const created = !fs.existsSync(file + '.bak');
console.log(`\n${created ? 'Created' : 'Updated'} settings.json\n`);
console.log('  Edit adminToken to password-protect your server (or leave blank for public).');
console.log('  Edit maxPlayers, gatewayPort, httpPort if needed.');
console.log('\nThen start the server:');
console.log('  node src/index.js\n');
