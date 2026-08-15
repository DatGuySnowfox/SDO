'use strict';

// Standalone test for ZombieSimulation — pure state machine, no socket/
// gateway/live-client dependency at all (see its own file-header comment).
// Uses a small synthetic world instead of the real 913-zone extracted data
// so test outcomes are deterministic and don't depend on Exports/ being
// present.

const { ZombieSimulation } = require('../server/src/world/zombie-simulation');

let total = 0, failures = 0;
function ok(cond, label) {
    total++;
    if (cond) { console.log(`  PASS  ${label}`); }
    else      { console.log(`  FAIL  ${label}`); failures++; }
}

function makeWorld() {
    return {
        spawnZones: [
            { kind: 'zombie', name: 'ZoneA', x: 0, y: 0, z: 0, boxExtentX: 500, boxExtentY: 500, spawnAmount: 2, spawningInterval: 1 },
            { kind: 'zombie', name: 'ZoneFar', x: 1_000_000, y: 1_000_000, z: 0, boxExtentX: 500, boxExtentY: 500, spawnAmount: 2, spawningInterval: 1 },
            { kind: 'zombieBoss', name: 'ZoneBoss', x: 100, y: 100, z: 0, boxExtentX: 200, boxExtentY: 200, spawnAmount: 1, spawningInterval: 1 },
        ],
        zombieStats: {
            BP_Zombie_Roamer: { health: { maxHealth: 100 }, roamingSpeed: 100 },
            BP_ZombieBoss:    { health: { maxHealth: 4000 }, roamingSpeed: 50 },
        },
        difficulty: { Standard: { ZombieHealthMultiplier: 1, ZombieSpeedMultiplier: 1, ZombieSpawnAmountMultiplier: 1 } },
    };
}

let idCounter = 1n;
const fakeRandomId = () => idCounter++;

console.log('── Spawning respects relevance radius ───────');
{
    const sim = new ZombieSimulation(makeWorld(), fakeRandomId);
    // Player near ZoneA/ZoneBoss but nowhere near ZoneFar.
    const r1 = sim.tick([{ x: 0, y: 0, z: 0 }], 1000);
    ok(r1.spawns.length > 0, 'at least one zombie spawned near the player');
    ok(r1.spawns.every(s => s.archetype === 'BP_Zombie_Roamer' || s.archetype === 'BP_ZombieBoss'),
       'spawned archetypes match zone kind mapping');
    ok(!r1.spawns.some(s => s.x > 900_000), 'nothing spawned in the far-away zone');
}

console.log('\n── Spawning respects capacity ────────────────');
{
    const sim = new ZombieSimulation(makeWorld(), fakeRandomId);
    let nowMs = 1000;
    let totalSpawns = 0;
    // Tick many times well past every zone's spawningInterval (1s) —
    // capacity (ZoneA=2, ZoneBoss=1) must still cap the count.
    for (let i = 0; i < 20; i++) {
        nowMs += 2000;
        const r = sim.tick([{ x: 0, y: 0, z: 0 }], nowMs);
        totalSpawns += r.spawns.length;
    }
    ok(totalSpawns === 3, `spawn count caps at zone capacity (ZoneA=2 + ZoneBoss=1 = 3), got ${totalSpawns}`);
}

console.log('\n── No players online -> no spawns ────────────');
{
    const sim = new ZombieSimulation(makeWorld(), fakeRandomId);
    const r = sim.tick([], 1000);
    ok(r.spawns.length === 0, 'nothing spawns with zero connected players');
}

console.log('\n── Roam movement moves toward a target over time ─');
{
    const sim = new ZombieSimulation(makeWorld(), fakeRandomId);
    const r1 = sim.tick([{ x: 0, y: 0, z: 0 }], 1000);
    const zombie = r1.spawns.find(s => s.archetype === 'BP_Zombie_Roamer');
    ok(zombie !== undefined, 'a roamer zombie exists to track movement on');
    if (zombie) {
        const startX = zombie.x, startY = zombie.y;
        const r2 = sim.tick([{ x: 0, y: 0, z: 0 }], 6000); // 5s later, past ROAM_INTERVAL
        const moved = r2.updates.some(u => u.entityId === zombie.entityId &&
            (u.x !== startX || u.y !== startY));
        ok(moved, 'zombie position changed after enough elapsed time');
    }
}

console.log('\n── Damage application and death ──────────────');
{
    const sim = new ZombieSimulation(makeWorld(), fakeRandomId);
    const r1 = sim.tick([{ x: 100, y: 100, z: 0 }], 1000);
    const boss = r1.spawns.find(s => s.archetype === 'BP_ZombieBoss');
    ok(boss !== undefined, 'boss zombie spawned for damage test');
    if (boss) {
        const hit1 = sim.applyDamage(boss.entityId, 1000);
        ok(hit1 !== null, 'applyDamage returns a result for a tracked zombie');
        ok(hit1.newHealth === 3000, `health reduced correctly (4000-1000=3000), got ${hit1.newHealth}`);
        ok(hit1.dead === false, 'not dead yet at 3000/4000 hp');

        const hit2 = sim.applyDamage(boss.entityId, 5000); // overkill
        ok(hit2.newHealth === 0, 'health clamps at 0, not negative');
        ok(hit2.dead === true, 'dies when health reaches 0');
        ok(sim.get(boss.entityId) === null, 'dead zombie is removed from tracking');

        const hit3 = sim.applyDamage(boss.entityId, 10);
        ok(hit3 === null, 'applying damage to an already-dead/removed entity returns null');
    }

    const bogus = sim.applyDamage(999999n, 10);
    ok(bogus === null, 'applyDamage on an unknown entityId returns null, does not throw');
}

console.log('\n══════════════════════════════════════════');
console.log(`  ${total - failures} / ${total} tests passed`);
if (failures > 0) console.log(`  ${failures} FAILED`);
console.log('══════════════════════════════════════════');
process.exit(failures > 0 ? 1 : 0);
