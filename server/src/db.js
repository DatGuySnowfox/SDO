'use strict';
const Database = require('better-sqlite3');
const path     = require('node:path');

const db = new Database(path.join(__dirname, '..', 'players.db'));

db.exec(`
    CREATE TABLE IF NOT EXISTS players (
        playerId    TEXT PRIMARY KEY,
        displayName TEXT NOT NULL DEFAULT '',
        firstSeen   INTEGER NOT NULL,
        lastSeen    INTEGER NOT NULL
    );

    -- Player inventory, stats and last position (raw payload blob, client-authoritative).
    -- Server overwrites on every ProfileRevision; only keeps highest revision.
    CREATE TABLE IF NOT EXISTS player_progress (
        playerId TEXT PRIMARY KEY,
        revision INTEGER NOT NULL DEFAULT 0,
        data     BLOB    NOT NULL,
        savedAt  INTEGER NOT NULL
    );

    -- Live world entities: dropped items, placed building pieces, containers.
    -- Keyed by entityId (string of BigInt) so we can delete by entityId on despawn.
    -- spawnFrame is the complete encoded wire frame — replayed verbatim to late joiners.
    CREATE TABLE IF NOT EXISTS world_entities (
        entityId  TEXT PRIMARY KEY,
        spawnFrame BLOB    NOT NULL,
        spawnedAt  INTEGER NOT NULL
    );

    -- Misc persistent key/value for the host-agent (timeOfDay, revision, etc.)
    CREATE TABLE IF NOT EXISTS world_state (
        key   TEXT PRIMARY KEY,
        value TEXT NOT NULL
    );
`);

// ── Players ───────────────────────────────────────────────────────────────────

const _upsertPlayer = db.prepare(`
    INSERT INTO players (playerId, displayName, firstSeen, lastSeen)
    VALUES (@playerId, @displayName, @now, @now)
    ON CONFLICT(playerId) DO UPDATE SET
        displayName = excluded.displayName,
        lastSeen    = excluded.lastSeen
`);
const _getPlayer = db.prepare('SELECT * FROM players WHERE playerId = ?');

// ── Player progress ───────────────────────────────────────────────────────────

const _saveProgress = db.prepare(`
    INSERT INTO player_progress (playerId, revision, data, savedAt)
    VALUES (@playerId, @revision, @data, @savedAt)
    ON CONFLICT(playerId) DO UPDATE SET
        revision = CASE WHEN excluded.revision >= revision THEN excluded.revision ELSE revision END,
        data     = CASE WHEN excluded.revision >= revision THEN excluded.data     ELSE data     END,
        savedAt  = CASE WHEN excluded.revision >= revision THEN excluded.savedAt  ELSE savedAt  END
`);
const _getProgress = db.prepare('SELECT data FROM player_progress WHERE playerId = ?');

// ── World entities ────────────────────────────────────────────────────────────

const _spawnEntity   = db.prepare(
    'INSERT OR REPLACE INTO world_entities (entityId, spawnFrame, spawnedAt) VALUES (?, ?, ?)');
// Appends a follow-up EntityState frame's bytes onto the stored descriptor
// frame so a late joiner's replay (see gateway.js) gets both frames back to
// back — the client's own frame decoder splits them apart exactly as if
// they'd arrived as two separate writes. No-op if the entity isn't tracked
// (e.g. an EntityState for something other than a world entity).
//
// Concatenated in JS, not via SQL `spawnFrame || ?` — that raw-SQL BLOB
// concat got coerced through SQLite's TEXT/UTF-8 affinity, replacing any
// byte sequence that wasn't valid UTF-8 with U+FFFD (0xEF 0xBF 0xBD),
// corrupting the binary frame and breaking every later replay of that
// entity (decodeFrame: bad_magic) — which broke EVERY subsequent client
// join, not just drops, once any entity had been dropped (2026-08-12).
const _getEntityFrame = db.prepare('SELECT spawnFrame FROM world_entities WHERE entityId = ?');
const _setEntityFrame = db.prepare('UPDATE world_entities SET spawnFrame = ? WHERE entityId = ?');
const _despawnEntity = db.prepare('DELETE FROM world_entities WHERE entityId = ?');
const _allEntities   = db.prepare(
    'SELECT spawnFrame FROM world_entities ORDER BY spawnedAt ASC');

// ── World state key/value ─────────────────────────────────────────────────────

const _setWS = db.prepare('INSERT OR REPLACE INTO world_state (key, value) VALUES (?, ?)');
const _getWS = db.prepare('SELECT value FROM world_state WHERE key = ?');

// ── Exports ───────────────────────────────────────────────────────────────────

module.exports = {
    upsertPlayer(playerId, displayName) {
        _upsertPlayer.run({ playerId, displayName, now: Date.now() });
    },
    getPlayer(playerId) {
        return _getPlayer.get(playerId) ?? null;
    },

    saveProgress(playerId, revision, data) {
        _saveProgress.run({ playerId, revision, data, savedAt: Date.now() });
    },
    getProgress(playerId) {
        const r = _getProgress.get(playerId);
        return r ? r.data : null;
    },

    spawnEntity(entityId, frameBytes) {
        _spawnEntity.run(String(entityId), frameBytes, Date.now());
    },
    appendEntityState(entityId, frameBytes) {
        const idStr = String(entityId);
        const row = _getEntityFrame.get(idStr);
        if (!row) return; // not a tracked world entity
        _setEntityFrame.run(Buffer.concat([row.spawnFrame, frameBytes]), idStr);
    },
    despawnEntity(entityId) {
        _despawnEntity.run(String(entityId));
    },
    getAllEntities() {
        return _allEntities.all().map(r => r.spawnFrame);
    },

    setWorldState(key, value) {
        _setWS.run(key, String(value));
    },
    getWorldState(key, def = null) {
        const r = _getWS.get(key);
        return r ? r.value : def;
    },
};
