# SurrounDead Bridge (SDO)

Experimental multiplayer for [SurrounDead](https://store.steampowered.com/app/1645820/SurrounDead/),
a single-player UE5 survival game. A [UE4SS](https://github.com/UE4SS-RE/RE-UE4SS) C++ mod hooks the
game client and talks to a dedicated Node.js server, which owns world state and relays player state
between clients.

It is a research project, not a product. Plenty works; plenty does not. The status table below is
kept deliberately blunt so nobody wastes an evening on something already known to be broken.

---

## Status

> **Mid-port to UE 5.6.** The game updated from UE 5.3 to **UE 5.6.1** on 2026-09-24, which
> invalidated most of the mod's assumptions about the game's memory. The mod now loads, and the
> game runs with a save loaded — but **nothing below has been re-verified with two clients since
> the port.** Treat the "Working" table as "worked on 5.3, expected to work, unproven on 5.6."

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
| `EquipActorToSocket` | Looked up under its Blueprint *display* name (`"Equip Actor to Socket"`) on the wrong object; the real FName is `EquipActorToSocket` on `BP_PlayerCharacter`. Broken before the 5.6 port too. Left deliberately unfixed: correcting it would newly *enable* a `ProcessEvent` call that has never executed, and this project has a save-corruption incident from exactly that. Fix it as an isolated, deliberate test. |
| `SpawnBuild` / `Svr_SpawnBuild` | Moved to `BuildingComponent` on 5.6; still looked up on the pawn, so building placement will not fire. |
| Head-look sync | Proxy heads do not track where a player is looking. A fix is in the tree but **was never verified** — it was deployed as a session ended. Diagnostic logging (`proxy_head_write`) is still present, which is the tell. |
| Attachment toggle-**off** | Turning a tactical light or NVG *off* did not revert on proxies (only the on-path existed). A fix is in the tree, **unverified**. |
| `Exports/` + `world-data.json` | Still generated from the 5.3 pak (2026-08-12). Server-side only, so it cannot crash anything, but zombie/vehicle spawn placement will be wrong until re-extracted. Note `extract-zombie-data.js` may need adjusting, not just re-running, if the level's structure changed. |

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

Roughly 100 raw memory offsets remain in `src/`, in paths not yet exercised since the port. They
will fail the same way the others did, so the loop is worth knowing:

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
      └─ SurrounDeadBridge/
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
node -e "console.log(require('crypto').randomUUID())"                            # SDB_WORLD_ID
```

Listens on `31000` (game TCP) and `31001` (HTTP: tickets, health). Both need forwarding to host for
players outside your network.

`settings.json` works as an alternative to `.env`; it is gitignored. Environment variables win.

### 4. Connect

The mod reads its target, in ascending order of precedence:

1. `%APPDATA%\SurrounDeadBridge\session.cfg`
2. Environment variables
3. Command-line arguments — `-sdb_host=`, `-sdb_port=`, `-sdb_ticket=`

Joining needs a ticket from the server's HTTP API:

```sh
curl -X POST http://<host>:31001/v1/tickets \
  -H 'content-type: application/json' \
  -d '{"playerId":"<any stable id>","displayName":"Name"}'
```

Then launch with `-sdb_host=<host> -sdb_port=31000 -sdb_ticket=<ticket>`. Tickets are short-lived and
single-use by default, so **a fresh one is needed for every launch** — including reconnects. This is
the single most common cause of a join mysteriously failing.

### 5. Server directory (optional)

A Cloudflare Worker letting servers advertise themselves and clients discover them. Free tier is
sufficient. See [`directory-worker/README.md`](directory-worker/README.md). Skip it entirely for a
LAN or a single known host — set `SDB_DIRECTORY_URL` empty and it stays out of the way.

### 6. Launcher (optional)

```sh
cd launcher
dotnet build -c Release
```

Point it at a directory before it is useful:

```
setx SDB_DIRECTORY_URL https://your-directory.example.com
```

(or `HKCU\Software\SDB\DirectoryUrl`). No directory address ships in this repo.

---

## Layout

| Path | What |
| --- | --- |
| `src/` | The C++ UE4SS mod — hooks, proxy rendering, protocol client. |
| `server/` | Gateway + host-agent. Authoritative world state, SQLite-backed. |
| `directory-worker/` | Cloudflare Worker server directory. |
| `launcher/` | WPF launcher. |
| `research/` | Reverse-engineering notes, decoded Blueprint bytecode, header dumps. The bulk of the repo. |
| `scripts/` | Build/deploy/launch helpers. |

The `research/` directory is the actual substance of the project — how the game's classes,
Blueprint functions and memory layout were worked out. `research/04_ida_investigation_log.md` is a
long chronological log and the best place to understand *why* something is built the way it is.

---

## Contributing

The highest-value open problems, roughly in order:

1. **Re-verify the 5.6 port with two clients.** The mod loads and the game runs, but no
   multiplayer session has been run since the engine bump. This is the gate on everything else.
2. **Finish the offset conversion.** ~100 raw offsets remain in paths not yet exercised — proxy
   sync, equipment visuals, entity spawning. See "Porting to a new engine version" above; the loop
   is mechanical once a crash names a line.
3. **Rehome `SpawnBuild`/`Svr_SpawnBuild`** onto `BuildingComponent`, and fix `EquipActorToSocket`'s
   lookup — the latter carefully and in isolation, since it enables a call that has never run.
4. **Zombie proxy rendering** — a genuinely hard crash bug, and the biggest single feature gap.
5. **Verify the head-look and attachment-toggle fixes.** Both are written and unverified; confirming
   or refuting them is cheap.
6. **Test vehicle sync at all.** It has never been run.
7. **Re-extract `Exports/`** and regenerate `world-data.json` from the 5.6 pak.
8. **Automatic mod installation** in the launcher.

A note on method, learned the hard way: for anything touching Blueprint behaviour, prefer the
flag-file diagnostics (`bytecode_dump`, `resolve_fprop`, `mem_dump`, … — drop a `.flag` file in the
mod's log directory), the CXX header dump, and the bytecode disassembler in `research/bytecode/`
over guessing or live debugging. Several bugs here cost multiple sessions of guess-and-redeploy and
then fell over in minutes once someone logged the actual values. Two were only solved by comparing a
proxy against the local player's own correct data, and the 5.6 port only converged once the header
dump was regenerated instead of reasoned about.

The mod's own log is `debug.log`, in `%APPDATA%\SurrounDeadBridge` by default or wherever
`SDB_LOG_DIR` points. UE4SS owns the other two (`SDB.log`, `UE4SS.log`) and puts them in its own
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
