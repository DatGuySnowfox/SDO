'use strict';
const Database = require('better-sqlite3');
const path     = require('node:path');

// SDB_DB_PATH lets tests/integration.js (and any future isolated run) point
// this at a throwaway file instead of the real players.db.
const db = new Database(process.env.SDB_DB_PATH || path.join(__dirname, '..', 'players.db'));

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
    -- DEPRECATED — superseded by the entities table below (structured, queryable
    -- by kind/position, field-mutable). Kept only until every entity kind still
    -- reading/writing it (see host-agent.js) is migrated over; do not add new
    -- callers of world_entities.
    CREATE TABLE IF NOT EXISTS world_entities (
        entityId  TEXT PRIMARY KEY,
        spawnFrame BLOB    NOT NULL,
        spawnedAt  INTEGER NOT NULL
    );

    -- Unified world-entity store — every shared entity kind (GroundItem, Zombie,
    -- Vehicle, PlacedStructure, Container) lives here as one row, not a bespoke
    -- table/Map per kind. attributes is kind-specific JSON (health, inventory
    -- contents, archetype, owner, ...); x/y/z are plain columns (not buried in
    -- the JSON) specifically so getEntitiesNear() can filter in SQL instead of
    -- deserializing every row to check distance.
    CREATE TABLE IF NOT EXISTS entities (
        entityId      TEXT PRIMARY KEY,
        kind          INTEGER NOT NULL,
        x             REAL    NOT NULL DEFAULT 0,
        y             REAL    NOT NULL DEFAULT 0,
        z             REAL    NOT NULL DEFAULT 0,
        yaw           REAL    NOT NULL DEFAULT 0,
        attributes    TEXT    NOT NULL DEFAULT '{}',
        revision      INTEGER NOT NULL DEFAULT 0,
        ownerPlayerId TEXT,
        updatedAt     INTEGER NOT NULL
    );
    CREATE INDEX IF NOT EXISTS idx_entities_kind ON entities(kind);
    CREATE INDEX IF NOT EXISTS idx_entities_pos  ON entities(x, y);

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

// ── Unified entities ─────────────────────────────────────────────────────────
// Generic store for every shared world-entity kind. `attributes` is passed/
// returned as a plain JS object — JSON (de)serialization happens at this
// boundary so callers never touch a raw JSON string.

const _upsertEntity = db.prepare(`
    INSERT INTO entities (entityId, kind, x, y, z, yaw, attributes, revision, ownerPlayerId, updatedAt)
    VALUES (@entityId, @kind, @x, @y, @z, @yaw, @attributes, @revision, @ownerPlayerId, @updatedAt)
    ON CONFLICT(entityId) DO UPDATE SET
        kind          = excluded.kind,
        x             = excluded.x,
        y             = excluded.y,
        z             = excluded.z,
        yaw           = excluded.yaw,
        attributes    = excluded.attributes,
        revision      = excluded.revision,
        ownerPlayerId = excluded.ownerPlayerId,
        updatedAt     = excluded.updatedAt
`);
const _getEntity        = db.prepare('SELECT * FROM entities WHERE entityId = ?');
const _deleteEntity     = db.prepare('DELETE FROM entities WHERE entityId = ?');
const _entitiesByKind   = db.prepare('SELECT * FROM entities WHERE kind = ?');
const _allEntitiesFull  = db.prepare('SELECT * FROM entities');
// Square-bounding-box prefilter in SQL (cheap, index-friendly); callers that
// need an exact circular radius should re-filter the returned rows.
const _entitiesNear = db.prepare(`
    SELECT * FROM entities
    WHERE x BETWEEN @minX AND @maxX AND y BETWEEN @minY AND @maxY
`);

function rowToEntity(row) {
    if (!row) return null;
    let attributes = {};
    try { attributes = JSON.parse(row.attributes); } catch { /* leave {} */ }
    return {
        entityId: row.entityId,
        kind: row.kind,
        x: row.x, y: row.y, z: row.z, yaw: row.yaw,
        attributes,
        revision: row.revision,
        ownerPlayerId: row.ownerPlayerId,
        updatedAt: row.updatedAt,
    };
}

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

    // ── Unified entities ─────────────────────────────────────────────────────
    upsertEntity(entity) {
        _upsertEntity.run({
            entityId: String(entity.entityId),
            kind: entity.kind,
            x: entity.x ?? 0, y: entity.y ?? 0, z: entity.z ?? 0, yaw: entity.yaw ?? 0,
            attributes: JSON.stringify(entity.attributes ?? {}),
            revision: entity.revision ?? 0,
            ownerPlayerId: entity.ownerPlayerId != null ? String(entity.ownerPlayerId) : null,
            updatedAt: Date.now(),
        });
    },
    getEntity(entityId) {
        return rowToEntity(_getEntity.get(String(entityId)));
    },
    deleteEntity(entityId) {
        _deleteEntity.run(String(entityId));
    },
    getEntitiesByKind(kind) {
        return _entitiesByKind.all(kind).map(rowToEntity);
    },
    getAllEntitiesFull() {
        return _allEntitiesFull.all().map(rowToEntity);
    },
    // Exact circular radius, using a cheap SQL bounding-box prefilter first.
    getEntitiesNear(x, y, radius) {
        const rows = _entitiesNear.all({
            minX: x - radius, maxX: x + radius,
            minY: y - radius, maxY: y + radius,
        });
        const r2 = radius * radius;
        return rows
            .map(rowToEntity)
            .filter(e => (e.x - x) ** 2 + (e.y - y) ** 2 <= r2);
    },
};
