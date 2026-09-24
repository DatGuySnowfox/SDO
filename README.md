# SurrounDead Bridge (SDO) — game .7 / Unreal Engine 5.3

> **This branch is frozen and receives no fixes.**
>
> It is the last state of the tree that targeted SurrounDead **.7**, built on **Unreal Engine 5.3**.
> The 2026-09-24 `.8` update moved the game to UE 5.6.1 (`++UE5+Release-5.6-CL-44394996`), which
> invalidated every hardcoded address and struct offset here.
>
> **If you just want to play, go to [`master`](../../tree/master).** This branch is for people still
> on .7, and for anyone who wants to diff the pre-port tree against the current one.

Experimental multiplayer for [SurrounDead](https://store.steampowered.com/app/1645820/SurrounDead/),
a single-player UE5 survival game. A [UE4SS](https://github.com/UE4SS-RE/RE-UE4SS) C++ mod hooks the
game client and talks to a dedicated Node.js server, which owns world state and relays player state
between clients.

It is a research project, not a product. Plenty works; plenty does not. The status table below is
kept deliberately blunt so nobody wastes an evening on something already known to be broken.

---

## Why this branch exists

The status table below is the **last state that was actually verified in live two-client testing**.
`master` has since been ported to UE 5.6, but as of that port *none* of it has been re-confirmed in
a multiplayer session — the game loads and runs, and that is all anyone can honestly claim.

That makes this branch the better reference for two things:

- **What the mod is capable of**, demonstrated rather than assumed.
- **What the code looked like before name-based reflection.** The port replaced roughly 107
  hardcoded byte offsets with property-name lookups. Diffing `master` against this branch shows that
  conversion in full, which is the most useful thing here if you are porting a UE4SS mod across an
  engine bump yourself.

---

## Running this branch

Three things must line up. Getting any one of them wrong fails quietly rather than loudly.

### 1. The game must be on .7

Steam serves the current build and offers no rollback in its UI. The only route is
`download_depot <appid> <depotid> <manifestid>` from the Steam console, which needs the manifest ID
for the .7 build — tracking that down is left to you. On a current install this mod will load and
then misbehave or crash, because every offset it uses describes a different binary.

### 2. UE4SS must be **v3.0.1**, not a nightly

This is the one that will cost you an evening.

`vendor/ue4ss-stub/` is a hand-written mirror of UE4SS's `CppUserModBase`, and on this branch it
mirrors the **v3.0.1** class: `ModShortName`, `ModAuthor`, an `on_uninstall()` virtual, and a vtable
order that newer builds no longer use. UE4SS changed that class after v3.0.1 — members were added
and removed, and the virtuals were reordered.

A mod built against the wrong mirror does not fail to compile and does not print an error. It loads
against a mismatched vtable, calls the wrong function through it, and corrupts memory. Use v3.0.1.

For the same reason, do not copy this branch's stub onto `master` or the reverse. `master`'s stub
mirrors current UE4SS, and the two are not interchangeable.

### 3. Use the legacy UE4SS layout

v3.0.1 puts everything directly in `Win64/`:

```
Win64/
├─ dwmapi.dll                 # UE4SS loader
├─ UE4SS.dll
├─ UE4SS-settings.ini
└─ Mods/
   └─ SurrounDeadBridge/
      ├─ enabled.txt          # empty file; its presence is what enables the mod
      └─ dlls/main.dll        # the build output
```

Newer UE4SS moves `UE4SS.dll`, the settings file and `Mods/` into a `ue4ss/` subdirectory instead.
`scripts/deploy.ps1` detects which of the two is present, so prefer it over copying by hand. Putting
the mod in the wrong one fails silently: UE4SS loads, and your mod simply never appears.

`enabled.txt` is what actually enables a mod — `mods.txt` alone will not do it.

---

## Status

Verified = observed working in a live two-client test, on .7. Everything else is called out as such.

### Working

| Area | Notes |
| --- | --- |
| Player movement + proxy spawning | Other players appear and move. The core loop. |
| Equipment / appearance sync | Clothing, armour, weapons, character-creator appearance. |
| Weapon grip poses | Via `CombatState`. Confirmed for shotgun + pistol. |
| Weapon firing | Muzzle flash / recoil on proxies, single and full-auto. |
| Weapon-mounted lights | Proxy sync works (`SpotLight.SetIntensity`). |
| NVG deploy animation | Goggles visibly lower on proxies when activated. |
| Inventory, damage, death, respawn | Server-confirmed death/respawn. |
| Ground items | Drop/pickup, persisted, with TTL expiry and a max-count budget. |
| Building placement | Placed structures persist and replicate. |
| World state | Time-of-day, persisted and broadcast. |
| Zombie simulation (server side) | Spawning, AI-relevance scoping, roaming, damage/death. 16/16 unit tests plus end-to-end integration. |
| Native zombie suppression | Client-side spawners stopped so the server owns zombies (857/857 spawners). |
| Server directory | Cloudflare Worker; servers heartbeat in, clients list what is up. |
| Launcher | WPF app: lists servers, pings, launches the game with a join ticket. |

### Broken / disabled

| Area | Notes |
| --- | --- |
| **Zombie proxy rendering** | **Disabled in code** (`spawn_zombie_actor` returns `nullptr` unconditionally). Zombies simulate correctly server-side but are invisible to clients. Five live crashes across three attempted fixes; root cause never found. Do not re-enable casually — read the doc comment on that function first. |
| Head-look sync | Proxy heads do not track where a player is looking. A fix is in the tree but **was never verified** — it was deployed as the session ended. Diagnostic logging (`proxy_head_write`) is still present, which is the tell. |
| Attachment toggle-**off** | Turning a tactical light or NVG *off* did not revert on proxies (only the on-path existed). A fix is in the tree, **unverified**. |

### Built but never tested

| Area | Notes |
| --- | --- |
| Vehicle sync | Server side complete, client side compiles. Never run in a live test, not once. |
| Melee grip mapping | Primary/Secondary → two-handed and Sidearm → one-handed are confirmed; **Melee is a guess.** |
| `SpawnBuild` hook | Compiles, never confirmed to fire live. |
| Launcher end-to-end | Builds and runs; the full fetch-ticket-and-launch path has not been confirmed against a live server. |

### Not implemented

- **Automatic mod installation** in the launcher — it detects whether the mod is installed, but you install it by hand (see above).
- **Concurrent character creation** — two players creating characters simultaneously is a known unresolved blocker.
- Any anti-cheat worth the name. Clients are trusted for far more than they should be. Run this with people you know.

---

## Setup

### Prerequisites

- **SurrounDead** on Steam (Windows), rolled back to **.7** — see above.
- **UE4SS v3.0.1** — see above. The version matters.
- **Node.js 22+** for the server. Not 18 or 20: `better-sqlite3` segfaults on Node 20 here, despite what its own engines field once claimed.
- **xmake** + MSVC (Visual Studio Build Tools, C++ workload) to build the mod.
- **.NET 9 SDK** for the launcher (optional).

Only the mod in `src/` is engine-version-specific. The server, directory worker and launcher are
not — if you only care about the server side, ignore everything above about .7 and UE4SS.

### 1. Build the mod

```sh
xmake build
```

Produces `build/out/main.dll`. A CMake path exists too, but **xmake is the one that is actually
maintained** — the two have drifted before.

### 2. Install the mod

```powershell
.\scripts\deploy.ps1
```

It locates the game, builds, and installs into whichever UE4SS layout it finds. See "Running this
branch" above for the manual layout if you would rather do it yourself.

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

## Logs

The mod writes to `%APPDATA%\SurrounDeadBridge\`, overridable with `SDB_LOG_DIR` so a dev checkout
can collect logs next to the source. Output is split across three files, and mixing them up is a
recurring waste of time:

- `SDB.log` — the mod's own high-level log.
- `debug.log` — detailed diagnostics, rotated at 64 MB to `debug.log.prev`.
- `UE4SS.log` — UE4SS's own log, and where load failures show up. If the mod "does nothing", read
  this one first.

Flag-file diagnostics live in the same directory: drop an empty `.flag` file named for the probe you
want (`bytecode_dump`, `resolve_fprop`, `mem_dump`, …) and the mod picks it up.

---

## Layout

| Path | What |
| --- | --- |
| `src/` | The C++ UE4SS mod — hooks, proxy rendering, protocol client. **The engine-version-specific part.** |
| `server/` | Gateway + host-agent. Authoritative world state, SQLite-backed. |
| `directory-worker/` | Cloudflare Worker server directory. |
| `launcher/` | WPF launcher. |
| `research/` | Reverse-engineering notes, decoded Blueprint bytecode, header dumps. The bulk of the repo. |
| `scripts/` | Build/deploy/launch helpers. |
| `vendor/ue4ss-stub/` | Hand-written mirror of UE4SS's public ABI. **Pinned to v3.0.1 on this branch.** |

The `research/` directory is the actual substance of the project — how the game's classes, Blueprint
functions and memory layout were worked out. `research/04_ida_investigation_log.md` is a long
chronological log and the best place to understand *why* something is built the way it is.

Everything generated under `research/` — `CXXHeaderDump/`, `Mappings.usmap`, `Exports/` — describes
the 5.3 build. `master` reorganises this material by engine version: current artifacts at the top
level, and this branch's contents archived under `research/archive-ue5.3/`.

---

## Contributing

**Send changes to [`master`](../../tree/master), not here.** This branch is a reference snapshot, and
a fix landed on it helps nobody — anyone with a current install cannot run it.

The exception is a fix that is specifically about .7 and does not apply to 5.6 at all. Those are
rare, and worth opening an issue about first.

A note on method, learned the hard way and still true on `master`: for anything touching Blueprint
behaviour, prefer the flag-file diagnostics described under "Logs" above and the bytecode
disassembler in `research/bytecode/` over guessing or live debugging. Several bugs in this repo's
history cost multiple sessions of guess-and-redeploy, then fell over in minutes once someone logged
the actual values. Two of them were only solved by comparing a proxy against the local player's own
correct data.

---

## Legal

Licensed under **AGPL-3.0-or-later** — see [LICENSE](LICENSE). The network clause matters here:
running a modified gateway or directory as a service for other players obliges you to publish that
modified source.

This project is unaffiliated with and unendorsed by the developers of SurrounDead. It ships no game
assets or game code. `research/` contains reverse-engineering notes derived from the game for
interoperability; whether that is acceptable where you live is your call to make, not this file's.
UE4SS is separately licensed by its own authors.
