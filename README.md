# SurrounDead Online (SDO)

Experimental multiplayer for [SurrounDead](https://store.steampowered.com/app/1645820/SurrounDead/),
a single-player UE5 survival game. A [UE4SS](https://github.com/UE4SS-RE/RE-UE4SS) C++ mod hooks the
game client and talks to a dedicated Node.js server, which owns world state and relays player state
between clients.

It is a research project, not a product. Plenty works; plenty does not. The status table below is
kept deliberately blunt so nobody wastes an evening on something already known to be broken.

---

## Status

> **Mid-port to UE 5.6.** The game updated from UE 5.3 to **UE 5.6.1** on 2026-09-24, which
> invalidated most of the mod's assumptions about the game's memory.
>
> As of 2026-09-24 two clients connect, spawn, and see each other. Getting there took fixing, in
> order: two hardcoded executable addresses in the proxy spawn path (both clients died within
> seconds of the second player joining), then a series of stale struct offsets that each broke
> something different once the one in front of it was fixed. **Still unverified: whether gameplay
> actually works with two clients.** Treat the "Working" table as "worked on 5.3, unproven on 5.6."

Verified = observed working in a live two-client test *on UE 5.3*. Everything else is called out.

### Worked on 5.3 — not yet re-verified on 5.6

| Area | Notes |
| --- | --- |
| Player movement + proxy spawning | Other players appear and move. The core loop. |
| Equipment / appearance sync | Clothing, armour, weapons, character-creator appearance. Appearance reads are ported to name lookups and resolving real meshes again. |
| Weapon grip poses | Via `CombatState`. Confirmed for shotgun + pistol. |
| Weapon firing | Muzzle flash / recoil on proxies, single and full-auto. |
| Weapon-mounted lights | Proxy sync works (`SpotLight.SetIntensity`). |
| NVG deploy animation | Goggles visibly lower on proxies when activated. |
| Inventory, damage, death, respawn | Server-confirmed death/respawn. |
| Ground items | Drop/pickup, persisted, with TTL expiry and a max-count budget. |
| Building placement | See `SpawnBuild` below — the hook needs rehoming on 5.6. |
| World state | Time-of-day, persisted and broadcast. |
| Zombie simulation (server side) | Spawning, AI-relevance scoping, roaming, damage/death. 16/16 unit tests plus end-to-end integration. Server-side only, so unaffected by the engine bump. |
| Native zombie suppression | Client-side spawners stopped so the server owns zombies. Still working on 5.6 (926 spawners found and stopped, up from 857 on 5.3). |
| Server directory | Cloudflare Worker; servers heartbeat in, clients list what is up. Unaffected by the engine bump. |
| Launcher | WPF app: lists servers, pings, launches the game with a join ticket. |

### Broken / disabled

| Area | Notes |
| --- | --- |
| **Zombie proxy rendering** | **Disabled in code** (`spawn_zombie_actor` returns `nullptr` unconditionally). Zombies simulate correctly server-side but are invisible to clients. Five live crashes across three attempted fixes; root cause never found. Do not re-enable casually — read the doc comment on that function first. |
| `EquipActorToSocket` | Looked up under its Blueprint *display* name (`"Equip Actor to Socket"`). This was long believed never to resolve, and was left unfixed on the grounds that enabling a never-executed `ProcessEvent` call once cost a save. **The 2026-09-24 logs disprove that**: it resolves and fires — 1,859 re-attach calls in one session, because a stale `AttachParent` offset made every equipment slot look orphaned. The offset is fixed; the display-name lookup still wants verifying. |
| `SpawnBuild` / `Svr_SpawnBuild` | Moved to `BuildingComponent` on 5.6; still looked up on the pawn, so building placement will not fire. |
| Proxy body meshes | `JigsawItem_DataAsset +0x448` is read as a male/female torso mesh pair. On 5.6 that offset is `MaxWeight`, and no field in the header dump matches the shape the code wants. Left alone rather than guessed at — expect wrong or missing proxy meshes. |
| Equip socket | Read as an `FGameplayTag` at `JigsawItem_DataAsset +0x280`. On 5.6 that is `EquippedTransform`, and `EquipSocket` is an `FName` at `0x02E0` — so both the offset *and* the type look wrong. Needs tracing before it is touched. |
| Head-look sync | Proxy heads do not track where a player is looking. A fix is in the tree but **was never verified** — it was deployed as a session ended. Diagnostic logging (`proxy_head_write`) is still present, which is the tell. |
| Attachment toggle-**off** | Turning a tactical light or NVG *off* did not revert on proxies (only the on-path existed). A fix is in the tree, **unverified**. |
| `research/Exports/` + `world-data.json` | Re-extracted from the .8 pak (2026-09-24). 982 spawn zones, up from 913. Regenerate with `tools/asset-export` followed by `extract-zombie-data.js`. |

### Built but never tested

| Area | Notes |
| --- | --- |
| Vehicle sync | Server side complete, client side compiles. Never run in a live test, not once. |
| Melee grip mapping | Primary/Secondary → two-handed and Sidearm → one-handed are confirmed; **Melee is a guess.** |
| Launcher end-to-end | Builds and runs; the full fetch-ticket-and-launch path has not been confirmed against a live server. |

### Not implemented

- **Automatic mod installation** in the launcher — it detects whether the mod is installed, but you install it by hand (see below).
- **Concurrent character creation** — two players creating characters simultaneously is a known unresolved blocker.
- Any anti-cheat worth the name. Clients are trusted for far more than they should be. Run this with people you know.

### Porting to a new engine version

Most of the raw offsets are gone — a systematic pass on 2026-09-24 took the live count from
119/45/4 (`mod.cpp`/`proxy_manager.cpp`/`entity_manager.cpp`) to 39/11/0. Almost all of what remains
*cannot* be name-resolved and is correct as written: `TArray` count fields at `+0x08`, ProcessEvent
parameter-block offsets, and offsets into plain structs rather than reflected `UObject`s.

If a new one does surface, the loop is worth knowing:

1. The crash dump names a file and line (Unreal symbolicates against the mod's PDB).
2. Look the property up in `research/CXXHeaderDump/` to get its **name**.
3. Replace the offset with `obj_prop(owner, STR("Name"))` or
   `GetValuePtrByPropertyNameInChain`.

Property names survive engine bumps where offsets do not, which is why the conversion goes to names
rather than to corrected offsets. Two cautions learned the hard way:

- **Names in this game contain spaces and punctuation** — `Hair Color`, `IsPlayerMale?`,
  `Hunger&ThirstComponent`. Use the exact FName from the dump, not a C++-ified version.
- **Names can change too.** The 5.6 bump renamed `HungerThirstComponent` → `Hunger&ThirstComponent`
  and `head` → `Head`, and moved `BP_JigMultiplayer`'s class to `UBP_JigComponent_C`. A name lookup
  that silently returns null is the symptom.

Regenerate the dump against the running game with **Ctrl+H** (and **Ctrl+Num6** for `Mappings.usmap`)
via UE4SS's `Keybinds` mod, ideally with this mod disabled and a save loaded so the gameplay classes
are actually in memory.

Three more things the 5.6 port taught, each of which cost real time:

- **Fixing one offset exposes the next.** `AttachChildren` was wrong, so every attachment walk bailed
  on a garbage count and did nothing. Correcting it made the walk run for the first time — straight
  into a second stale offset (`ItemDataAsset`) that faulted on every cycle. A "fix" that makes things
  visibly worse usually means dead code just came alive.
- **Defensive guards hide the bug they catch.** That fault was swallowed by an SEH handler that logged
  *"stale pointer, likely a concurrent native equip/unequip change"* and discarded the cycle. A hard,
  repeatable offset bug read as a benign race for a long time. Guards are worth having; their messages
  should not assert a cause they have not established.
- **Silent no-ops are worse than crashes.** The vitals write-back chased four stale component pointers
  and wrote doubles into whatever now lives there, two seconds after every join. Nothing logged an
  error. A name lookup that returns null at least says so.

### Bisect switches

When something breaks and the cause is not obvious, these disable groups of work so one launch
eliminates a whole class of cause. Each is enabled by an environment variable **or** by an empty file
of the matching name in `%APPDATA%\SDO\` — prefer the file:

| Switch | Disables |
| --- | --- |
| `safe_mode.flag` | everything below marked (*) at once |
| `no_tick_work.flag` | every per-tick step that touches the local player (network + proxies keep running) |
| `no_world_state.flag` (*) | UltraDynamicSky time/weather writes |
| `no_local_repair.flag` (*) | component-drift repair writes (the scan stays, read-only) |
| `no_vitals_write.flag` (*) | deferred vitals write-back |
| `no_equip_restore.flag` | step 3c, equip-restore retry |
| `no_equip_send.flag` | step 8, equipment / attachment / appearance sends |
| `no_pickup_resolve.flag` | step 6b, pickup resolve |
| `no_profile_rev.flag` | step 7, profile revision |
| `no_misc_sync.flag` | steps 9/9b/9c, lights, weapon-fire edge, head-look diagnostic |

The file form matters. Windows applies a User-scope environment variable only to processes started
afterwards, and the game inherits Steam's environment from whenever Steam launched — so setting a
variable and relaunching the game silently tests the **old** value. That invalidated a whole bisect
round before it was noticed. `session.cfg` exists for the same reason.

Each switch logs a line when it engages. If that line is absent, the switch did not reach the mod and
the result means nothing — check that before trusting any outcome.

---

## Setup

### Prerequisites

- **SurrounDead** on Steam (Windows).
- **UE4SS** — supplies the C++ modding runtime the mod loads under. **Use the rolling
  `experimental-latest` nightly**, not a tagged release: UE 5.6 support is merged in `main` but the
  newest tag predates it, and v3.0.1 cannot pattern-scan 5.6 at all.
- **Node.js 22+** for the server. Not 18 or 20: `better-sqlite3` segfaults on Node 20 here, despite what its own engines field once claimed.
- **xmake** + MSVC (Visual Studio Build Tools, C++ workload) to build the mod.
- **.NET 9 SDK** for the launcher (optional).

### 1. Build the mod

```sh
xmake build
```

Produces `build/out/main.dll`. A CMake path exists too, but **xmake is the one that is actually
maintained** — the two have drifted before.

### 2. Install the mod

`scripts/deploy.ps1` does this for you and detects which UE4SS layout is installed. Manually, for
current UE4SS builds:

```
Win64/
├─ dwmapi.dll                 # UE4SS loader, stays next to the game exe
└─ ue4ss/
   ├─ UE4SS.dll
   ├─ UE4SS-settings.ini
   └─ Mods/
      └─ SDO/
         ├─ enabled.txt       # empty file; its presence is what enables the mod
         └─ dlls/main.dll     # the build output
```

**UE4SS changed this layout after v3.0.1** — `UE4SS.dll`, the settings file and `Mods/` all moved
into a `ue4ss/` subdirectory. On v3.0.1 and earlier they sit directly in `Win64/` instead. Putting
the mod in the wrong one fails silently: UE4SS loads, your mod simply never appears.

`enabled.txt` is what actually enables a mod — `mods.txt` alone will not do it.

### Engine version

The game moved to **UE 5.6** in the 2026-09-24 update (previously 5.3). UE4SS support for 5.6 landed
in `main` but is **not in any tagged release** — the newest tag predates 5.6 — so use the rolling
`experimental-latest` nightly asset, which is rebuilt continuously. If UE4SS fails to detect the
engine version, `[EngineVersionOverride]` in its settings file is the escape hatch.

Note that an engine-version change invalidates every hardcoded offset in `src/` (about 107 of them)
and every generated artifact under `research/`. Name-based lookups (roughly 276 of them) survive as
long as the property names themselves did not change.

### 3. Run the server

```sh
cd server
npm install
cp .env.example .env     # then fill it in
npm start
```

Generate the secrets it asks for:

```sh
node -e "console.log(require('crypto').randomBytes(24).toString('base64url'))"   # each secret
node -e "console.log(require('crypto').randomUUID())"                            # SDO_WORLD_ID
```

Listens on `31000` (game TCP) and `31001` (HTTP: tickets, health). Both need forwarding to host for
players outside your network.

`settings.json` works as an alternative to `.env`; it is gitignored. Environment variables win.

### 4. Connect

The mod reads its target, in ascending order of precedence:

1. `%APPDATA%\SDO\session.cfg`
2. Environment variables
3. Command-line arguments — `-sdo_host=`, `-sdo_port=`, `-sdo_ticket=`

Joining needs a ticket from the server's HTTP API:

```sh
curl -X POST http://<host>:31001/v1/tickets \
  -H 'content-type: application/json' \
  -d '{"playerId":"<any stable id>","displayName":"Name"}'
```

Then launch with `-sdo_host=<host> -sdo_port=31000 -sdo_ticket=<ticket>`. Tickets are short-lived and
single-use by default, so **a fresh one is needed for every launch** — including reconnects. This is
the single most common cause of a join mysteriously failing.

### 5. Server directory (optional)

A Cloudflare Worker letting servers advertise themselves and clients discover them. Free tier is
sufficient. See [`directory-worker/README.md`](directory-worker/README.md). Skip it entirely for a
LAN or a single known host — set `SDO_DIRECTORY_URL` empty and it stays out of the way.

### 6. Launcher (optional)

```sh
cd launcher
dotnet build -c Release
```

Point it at a directory before it is useful:

```
setx SDO_DIRECTORY_URL https://your-directory.example.com
```

(or `HKCU\Software\SDO\DirectoryUrl`). No directory address ships in this repo.

---

## Layout

| Path | What |
| --- | --- |
| `src/` | The C++ UE4SS mod — hooks, proxy rendering, protocol client. |
| `server/` | Gateway + host-agent. Authoritative world state, SQLite-backed. |
| `directory-worker/` | Cloudflare Worker server directory. |
| `launcher/` | WPF launcher. |
| `research/` | Reverse-engineering notes, decoded Blueprint bytecode, header dumps. The bulk of the repo. Split into current (5.6) material and `archive-ue5.3/`; see `research/README.md`. |
| `scripts/` | Build/deploy/launch helpers. |

The `research/` directory is the actual substance of the project — how the game's classes,
Blueprint functions and memory layout were worked out. `research/04_ida_investigation_log.md` is a
long chronological log and the best place to understand *why* something is built the way it is.

It is organised by engine version. Current 5.6 artifacts (`CXXHeaderDump/`, `Mappings.usmap`,
`Exports/`) sit at the top level; pre-port 5.3 material — including the old hardcoded-address table
and the Blueprint catalogs — is parked in `research/archive-ue5.3/`, kept for its reasoning rather
than its numbers. `research/README.md` says what regenerates each artifact and how.

---

## Contributing

The highest-value open problems, roughly in order:

1. **Verify gameplay with two clients.** They connect, spawn and see each other as of
   2026-09-24, which is further than the port had got before. Whether movement, equipment and
   inventory actually behave is still open, and it gates everything else.
2. **Identify the two offsets left unresolved.** The `JigsawItem_DataAsset` torso mesh pair
   (`+0x448`, now `MaxWeight`) and the equip-socket read (`+0x280`, where the type looks wrong
   too). Both were deliberately not guessed at — see the "Broken / disabled" table.
3. **Rehome `SpawnBuild`/`Svr_SpawnBuild`** onto `BuildingComponent`, and verify
   `EquipActorToSocket`'s display-name lookup. Note the standing warning that this call "has never
   run" is wrong: the logs show it firing thousands of times.
4. **Zombie proxy rendering** — a genuinely hard crash bug, and the biggest single feature gap.
5. **Verify the head-look and attachment-toggle fixes.** Both are written and unverified; confirming
   or refuting them is cheap.
6. **Test vehicle sync at all.** It has never been run.
7. **Re-check the zombie archetype stats after each update.** The .8 update renamed four of them
   and turned the boss's attack damage into a range; `extract-zombie-data.js` handles both, but a
   future rename would show up as silent nulls rather than an error.
8. **Automatic mod installation** in the launcher.

A note on method, learned the hard way: for anything touching Blueprint behaviour, prefer the
flag-file diagnostics (`bytecode_dump`, `resolve_fprop`, `mem_dump`, … — drop a `.flag` file in the
mod's log directory), the CXX header dump, and the bytecode disassembler in `research/bytecode/`
over guessing or live debugging. Several bugs here cost multiple sessions of guess-and-redeploy and
then fell over in minutes once someone logged the actual values. Two were only solved by comparing a
proxy against the local player's own correct data, and the 5.6 port only converged once the header
dump was regenerated instead of reasoned about.

The mod's own log is `debug.log`, in `%APPDATA%\SDO` by default or wherever
`SDO_LOG_DIR` points. UE4SS owns the other two (`SDO.log`, `UE4SS.log`) and puts them in its own
directory — since v3.0.1 that is `Binaries/Win64/ue4ss/`. Checking the wrong one of the three is a
recurring trap.

---

## Legal

Licensed under **AGPL-3.0-or-later** — see [LICENSE](LICENSE). The network clause matters here:
running a modified gateway or directory as a service for other players obliges you to publish that
modified source.

This project is unaffiliated with and unendorsed by the developers of SurrounDead. It ships no game
assets or game code. `research/` contains reverse-engineering notes derived from the game for
interoperability; whether that is acceptable where you live is your call to make, not this file's.
UE4SS is separately licensed by its own authors.
