'use strict';

// Server-authoritative zombie simulation core (approved rewrite plan, Phase 2).
// Pure state machine — no socket/protocol knowledge at all, so it's fully
// testable without a live client or gateway connection (see
// tests/zombie-simulation.js). host-agent.js drives it with player positions
// each tick and turns the returned spawn/update/despawn events into
// EntitySpawn/EntityState/EntityDespawn frames.
//
// V1 scope (documented, not an oversight):
//   - Roam-only movement (small periodic drift within the spawn zone).
//     Realistic chase/pathing needs real navmesh data this project doesn't
//     have yet (see research/04_ida_investigation_log.md Session 58 / the
//     approved plan's Phase 0) — deferred, not attempted here.
//   - One representative archetype per spawner kind, not the game's real
//     population-mix logic (which needs further, not-yet-done research into
//     SpawnedActorClass/weighted spawn tables). 'zombie' zones -> Roamer,
//     'zombieHound' zones -> Dog, 'zombieBoss' zones -> Boss.
//   - Relevance-scoped: only zones within RELEVANCE_RADIUS of at least one
//     tracked player position are actively spawned/ticked, matching the
//     approved plan's area-of-interest requirement (~900 zones total across
//     the map — ticking/broadcasting all of them regardless of player
//     position would be wasteful and pointless).

const RELEVANCE_RADIUS   = 15000;  // UU (~150m) around a player, zones outside this don't spawn/tick
const ROAM_INTERVAL_MS   = 4000;   // how often a live zombie picks a new roam target
const ROAM_SPEED_UU_S    = 100;    // fallback if archetype has no roamingSpeed override

const ZONE_KIND_TO_ARCHETYPE = {
    zombie:      'BP_Zombie_Roamer',
    zombieHound: 'BP_Zombie_Dog',
    zombieBoss:  'BP_ZombieBoss',
};

function dist2(x1, y1, x2, y2) {
    const dx = x1 - x2, dy = y1 - y2;
    return dx * dx + dy * dy;
}

class ZombieSimulation {
    constructor(worldData, randomEntityId) {
        this._randomEntityId = randomEntityId;
        this.zones = (worldData.spawnZones || []).filter(z => ZONE_KIND_TO_ARCHETYPE[z.kind]);
        this.archetypeStats = worldData.zombieStats || {};
        this.difficulty = (worldData.difficulty && worldData.difficulty.Standard) || {};

        // entityId(BigInt) -> zombie state
        this.zombies = new Map();
        // zoneName -> { lastSpawnMs, aliveCount }
        this.zoneState = new Map();
        for (const z of this.zones) this.zoneState.set(z.name, { lastSpawnMs: 0, aliveCount: 0 });
        this._lastTickMs = null;
    }

    _archetypeFor(zone) {
        const name = ZONE_KIND_TO_ARCHETYPE[zone.kind];
        return { name, stats: this.archetypeStats[name] || {} };
    }

    _maxHealthFor(stats) {
        const base = (stats.health && stats.health.maxHealth) || 100;
        const mult = this.difficulty.ZombieHealthMultiplier || 1;
        return base * mult;
    }

    _spawnCapacityFor(zone) {
        const base = zone.spawnAmount || 3; // most zones don't set an explicit override — small sane default
        const mult = this.difficulty.ZombieSpawnAmountMultiplier || 1;
        return Math.max(1, Math.round(base * mult));
    }

    _spawnIntervalMsFor(zone) {
        return (zone.spawningInterval || 30) * 1000;
    }

    // Random point inside the zone's box extent, ignoring the box's own yaw
    // rotation (a real oriented-box placement is a further refinement, not
    // needed for a first working version — box extents here are typically
    // large enough relative to zombie count that this doesn't look wrong).
    _randomPointInZone(zone) {
        const rx = (Math.random() * 2 - 1) * (zone.boxExtentX || 200);
        const ry = (Math.random() * 2 - 1) * (zone.boxExtentY || 200);
        return { x: zone.x + rx, y: zone.y + ry, z: zone.z };
    }

    // playerPositions: [{x,y,z}, ...]. Returns { spawns, updates, despawns },
    // each an array of { entityId, ...fields } ready for the caller to
    // encode into wire frames.
    tick(playerPositions, nowMs = Date.now()) {
        const dtMs = this._lastTickMs === null ? 0 : Math.max(0, nowMs - this._lastTickMs);
        this._lastTickMs = nowMs;

        const spawns = [], updates = [], despawns = [];
        if (!playerPositions || playerPositions.length === 0) {
            // No one online to be relevant to — nothing to do. Existing
            // zombies are left as-is rather than despawned, so they're still
            // there (from the server's perspective) if someone reconnects
            // shortly after; a real idle-cleanup pass could be added later
            // if that turns out to matter.
            return { spawns, updates, despawns };
        }

        const isRelevant = (x, y) => playerPositions.some(
            p => dist2(x, y, p.x, p.y) <= RELEVANCE_RADIUS * RELEVANCE_RADIUS);

        // ── Spawning ──────────────────────────────────────────────────────
        for (const zone of this.zones) {
            if (!isRelevant(zone.x, zone.y)) continue;
            const zs = this.zoneState.get(zone.name);
            if (zs.aliveCount >= this._spawnCapacityFor(zone)) continue;
            if (nowMs - zs.lastSpawnMs < this._spawnIntervalMsFor(zone)) continue;

            const { name: archetype, stats } = this._archetypeFor(zone);
            const maxHealth = this._maxHealthFor(stats);
            const pos = this._randomPointInZone(zone);
            const entityId = this._randomEntityId();

            const zombie = {
                entityId, archetype, zoneName: zone.name,
                x: pos.x, y: pos.y, z: pos.z, yaw: 0,
                health: maxHealth, maxHealth,
                aiState: 'roam',
                roamTargetX: pos.x, roamTargetY: pos.y,
                nextRoamMs: nowMs + ROAM_INTERVAL_MS,
            };
            this.zombies.set(entityId, zombie);
            zs.aliveCount++;
            zs.lastSpawnMs = nowMs;

            spawns.push({ entityId, archetype, x: zombie.x, y: zombie.y, z: zombie.z, yaw: 0, health: maxHealth, maxHealth });
        }

        // ── Movement (roam only — see class doc for why) ────────────────
        for (const z of this.zombies.values()) {
            if (z.aiState !== 'roam') continue;
            if (nowMs >= z.nextRoamMs) {
                const zone = this.zones.find(zz => zz.name === z.zoneName);
                const target = zone ? this._randomPointInZone(zone) : { x: z.x, y: z.y };
                z.roamTargetX = target.x;
                z.roamTargetY = target.y;
                z.nextRoamMs = nowMs + ROAM_INTERVAL_MS;
            }

            const dx = z.roamTargetX - z.x, dy = z.roamTargetY - z.y;
            const d = Math.hypot(dx, dy);
            if (d > 1 && dtMs > 0) {
                const stats = this.archetypeStats[z.archetype] || {};
                const speed = (stats.roamingSpeed || ROAM_SPEED_UU_S) * (this.difficulty.ZombieSpeedMultiplier || 1);
                const step = Math.min(d, speed * (dtMs / 1000));
                z.x += (dx / d) * step;
                z.y += (dy / d) * step;
                z.yaw = Math.atan2(dy, dx) * 180 / Math.PI;
                updates.push({ entityId: z.entityId, x: z.x, y: z.y, z: z.z, yaw: z.yaw, health: z.health, maxHealth: z.maxHealth });
            }
        }

        return { spawns, updates, despawns };
    }

    // Returns { entityId, newHealth, dead } or null if the entity isn't a
    // zombie this simulation is tracking.
    applyDamage(entityId, damage) {
        const z = this.zombies.get(entityId);
        if (!z) return null;
        z.health = Math.max(0, z.health - Math.max(0, damage));
        const dead = z.health <= 0;
        if (dead) {
            z.aiState = 'dead';
            const zs = this.zoneState.get(z.zoneName);
            if (zs) zs.aliveCount = Math.max(0, zs.aliveCount - 1);
            this.zombies.delete(entityId);
        }
        return { entityId, newHealth: z.health, dead };
    }

    get(entityId) {
        return this.zombies.get(entityId) || null;
    }
}

module.exports = { ZombieSimulation, RELEVANCE_RADIUS };
