'use strict';

// One-time data extraction script (not part of the live server) — mines
// Exports/ (FModel property dumps) for zombie/vehicle archetype stats,
// difficulty multipliers, and spawn-zone/spawn-point world positions, and
// writes them into a single compact JSON file the server loads at startup.
// See research/04_ida_investigation_log.md Session 58 and the approved
// rewrite plan for the full rationale — this replaces re-parsing a 210MB
// level export at runtime.
//
// Usage: node server/scripts/extract-zombie-data.js
// Output: server/src/data/world-data.json

const fs   = require('node:fs');
const path = require('node:path');
const readline = require('node:readline');

const EXPORTS_ROOT = path.join(__dirname, '..', '..', 'Exports', 'SurrounDead', 'Content');
const LEVEL_FILE    = path.join(EXPORTS_ROOT, 'Levels', 'LongdownValley.json');
const OUT_DIR        = path.join(__dirname, '..', 'src', 'data');
const OUT_FILE       = path.join(OUT_DIR, 'world-data.json');

const SPAWNER_KINDS = {
    BP_AISpawner_Zombies_C:       'zombie',
    BP_AISpawner_ZombieHounds_C:  'zombieHound',
    BP_AISpawner_ZombieBosses_C:  'zombieBoss',
    BP_VehicleSpawner_C:          'vehicle',
};

// ── Pass helper: stream the level file, yielding one parsed top-level array
// element (object) at a time via brace-depth tracking. FModel's own
// pretty-printer never emits a literal '{'/'}' inside a string value in this
// export (class paths/names don't contain braces), so simple counting is
// safe here — verified against known-good entries during development.
async function* streamLevelObjects(filePath) {
    const rl = readline.createInterface({
        input: fs.createReadStream(filePath, { encoding: 'utf8' }),
        crlfDelay: Infinity,
    });

    let depth = 0;
    let buf = null; // lines of the object currently being accumulated

    for await (const line of rl) {
        const opens  = (line.match(/\{/g) || []).length;
        const closes = (line.match(/\}/g) || []).length;

        if (depth === 0) {
            if (opens > 0) {
                buf = [line];
                depth += opens - closes;
            }
            continue;
        }

        buf.push(line);
        depth += opens - closes;

        if (depth === 0) {
            // Strip a trailing comma left over from the array context before parsing.
            let text = buf.join('\n');
            text = text.replace(/,\s*$/, '');
            try {
                yield JSON.parse(text);
            } catch (e) {
                console.error(`[extract] failed to parse an object ending near a line starting "${line.slice(0, 60)}": ${e.message}`);
            }
            buf = null;
        }
    }
}

// Outer.ObjectName format: "<Class>'LongdownValley:PersistentLevel.<InstanceName>'"
// (sometimes with a trailing ".<Component>" for nested components, which we
// don't need to strip here since we only match top-level actor Outers).
function actorInstanceNameFromOuter(outerObjectName) {
    const m = /PersistentLevel\.([^'.]+)/.exec(outerObjectName || '');
    return m ? m[1] : null;
}

async function extractSpawnZones() {
    console.log(`[extract] scanning ${LEVEL_FILE} (this takes a while — 210MB, two passes)...`);

    // Pass 1: collect every target spawner actor's own Properties.
    const spawners = new Map(); // instanceName -> { kind, ...props }
    let total = 0;
    for await (const obj of streamLevelObjects(LEVEL_FILE)) {
        total++;
        const kind = SPAWNER_KINDS[obj.Type];
        if (!kind || !obj.Name) continue;
        const p = obj.Properties || {};
        spawners.set(obj.Name, {
            kind,
            name: obj.Name,
            boxExtentX: p['Box Extent X'] ?? null,
            boxExtentY: p['Box Extent Y'] ?? null,
            boxExtentZ: p['Box Extent Z'] ?? null,
            spawnAmount: p.SpawnAmount ?? null,
            spawningInterval: p.SpawningInterval ?? null,
            vehicleSelection: p.VehicleSelection ?? null,
            x: null, y: null, z: null, yaw: null, // filled in pass 2
        });
    }
    console.log(`[extract] pass 1: scanned ${total} objects, found ${spawners.size} target spawners`);

    // Pass 2: find each spawner's DefaultSceneRoot SceneComponent entry
    // (separate array element, Outer.ObjectName references the spawner's
    // instance name) and pull its RelativeLocation/RelativeRotation.Yaw.
    let matched = 0;
    for await (const obj of streamLevelObjects(LEVEL_FILE)) {
        if (obj.Type !== 'SceneComponent' || obj.Name !== 'DefaultSceneRoot') continue;
        const instanceName = actorInstanceNameFromOuter(obj.Outer && obj.Outer.ObjectName);
        if (!instanceName || !spawners.has(instanceName)) continue;

        const p = obj.Properties || {};
        const loc = p.RelativeLocation || {};
        const rot = p.RelativeRotation || {};
        const entry = spawners.get(instanceName);
        entry.x = loc.X ?? 0;
        entry.y = loc.Y ?? 0;
        entry.z = loc.Z ?? 0;
        entry.yaw = rot.Yaw ?? 0;
        matched++;
    }
    console.log(`[extract] pass 2: matched positions for ${matched}/${spawners.size} spawners`);

    const missing = [...spawners.values()].filter(s => s.x === null);
    if (missing.length > 0) {
        console.warn(`[extract] WARNING: ${missing.length} spawners have no matched position (left null) — e.g. ${missing.slice(0, 3).map(s => s.name).join(', ')}`);
    }

    return [...spawners.values()];
}

// ── Zombie archetype stats + difficulty multipliers ───────────────────────
// Pulled from individual class-default JSON exports, not the level file.

function readJson(relPath) {
    const p = path.join(EXPORTS_ROOT, relPath);
    if (!fs.existsSync(p)) { console.warn(`[extract] missing: ${relPath}`); return null; }
    return JSON.parse(fs.readFileSync(p, 'utf8'));
}

// Class-default exports are also flat arrays; the object we want is the one
// whose Type matches the class name itself (its CDO / archetype defaults).
function findClassDefaults(exported, typeName) {
    if (!Array.isArray(exported)) return null;
    const obj = exported.find(o => o.Type === typeName) || exported[0];
    return obj ? (obj.Properties || {}) : null;
}

// Health lives on a SEPARATE array entry in the same file (a DamageComponent_C
// component-template override), not on the archetype's own Properties —
// correlated by Outer.ObjectName containing "BlueprintGeneratedClass'<Class>_C'",
// the same "position/value lives on a related entry, not the actor itself"
// shape as the spawn-zone DefaultSceneRoot lookup above, just within one
// small per-archetype file instead of the 210MB level export.
function findDamageComponentHealth(exported, className) {
    const needle = `BlueprintGeneratedClass'${className}_C'`;
    const dc = exported.find(o => o.Type === 'DamageComponent_C' &&
        o.Outer && o.Outer.ObjectName === needle);
    if (!dc) return null;
    const p = dc.Properties || {};
    return {
        standardHealth: p.StandardHealthValue ?? null,
        currentHealth:  p.CurrentHealth ?? null,
        maxHealth:      p.MaxHealth ?? null,
        healthDeviation: p.HealthRandomDeviation ?? null,
    };
}

function extractZombieStats() {
    const archetypes = {
        BP_MasterZombie:    'AI/Zombies/BP_MasterZombie.json',
        BP_Zombie_Roamer:   'AI/Zombies/Roamer/BP_Zombie_Roamer.json',
        BP_Zombie_Infected: 'AI/Zombies/Infected/BP_Zombie_Infected.json',
        BP_Zombie_Radiated: 'AI/Zombies/Radiated/BP_Zombie_Radiated.json',
        BP_Zombie_Crawler:  'AI/Zombies/Crawler/BP_Zombie_Crawler.json',
        BP_Zombie_Dog:      'AI/Zombies/Dog/BP_Zombie_Dog.json',
        BP_ZombieBoss:      'AI/Zombies/Boss/BP_ZombieBoss.json',
    };

    const stats = {};
    for (const [name, relPath] of Object.entries(archetypes)) {
        const exported = readJson(relPath);
        if (!exported) continue;
        const props = findClassDefaults(exported, `${name}_C`);
        if (!props) continue;
        stats[name] = {
            health:       findDamageComponentHealth(exported, name),
            damageToDo:   props['DamageToDo'] ?? null,
            roamingSpeed: props['Roaming Speed'] ?? null,
            alertSpeed:   props['Alert Speed'] ?? null,
            attackSpeed:  props['Attack Speed'] ?? null,
        };
    }
    return stats;
}

// FModel suffixes struct-property keys with an internal id + hash (e.g.
// "ZombieHealthMultiplier_9_C2B4BF884AFA926137E0B6900A8761BF") — stable
// within one export but not something to hardcode against, and liable to
// change on a re-export. Strip back to the plain field name.
function stripPropertySuffix(key) {
    return key.replace(/_\d+_[0-9A-F]{32}$/, '');
}

function extractDifficulty() {
    const exported = readJson('Blueprints/Other/DT_Difficulty.json');
    if (!exported) return null;
    // DataTables export as { Type: "DataTable", Rows: { <RowName>: {...} } } (per DT_Difficulty.json shape).
    const dt = exported.find(o => o.Type === 'DataTable');
    if (!dt || !dt.Rows) return null;

    const cleaned = {};
    for (const [tier, row] of Object.entries(dt.Rows)) {
        const cleanRow = {};
        for (const [key, value] of Object.entries(row)) {
            cleanRow[stripPropertySuffix(key)] = value;
        }
        cleaned[tier] = cleanRow;
    }
    return cleaned;
}

async function main() {
    fs.mkdirSync(OUT_DIR, { recursive: true });

    const spawnZones = await extractSpawnZones();
    const zombieStats = extractZombieStats();
    const difficulty = extractDifficulty();

    const out = {
        generatedAt: new Date().toISOString(),
        spawnZones,
        zombieStats,
        difficulty,
    };
    fs.writeFileSync(OUT_FILE, JSON.stringify(out, null, 2));
    console.log(`[extract] wrote ${OUT_FILE} (${spawnZones.length} spawn zones, ${Object.keys(zombieStats).length} zombie archetypes)`);
}

main().catch(e => { console.error(e); process.exit(1); });
