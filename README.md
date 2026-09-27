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
> As of 2026-09-24 two clients connect, spawn, see each other, move, and render each other
> correctly - clothed, with body parts attached and head-look tracking. Getting there took fixing,
> in order: two hardcoded executable addresses in the proxy spawn path (both clients died within
> seconds of the second player joining), then roughly forty stale struct offsets across ten
> separate sites, and finally two logic bugs the offsets had been masking - a resume marker
> committed before the work it marked, which silently turned any transient failure into a permanent
> one, and two separate writers fighting over the same body-part mesh property.
>
> **Still unverified: everything past "two dressed players standing next to each other."** Combat,
> vehicles, building, zombies and death/respawn have not been re-tested on 5.6 with two clients.
> Treat the table below as "worked on 5.3, unproven on 5.6."

Verified = observed working in a live two-client test *on UE 5.3*. Everything else is called out.

### Worked on 5.3 - not yet re-verified on 5.6

| Area | Notes |
| --- | --- |
| Player movement + proxy spawning | Other players appear and move. The core loop. |
| Equipment / appearance sync | Clothing, armour, weapons, character-creator appearance. Appearance reads are ported to name lookups and resolving real meshes again. |
| Weapon grip poses | Via `CombatState`. Confirmed for shotgun + pistol, and the rifle stance is confirmed on 5.6 (proxy and local both read `BlendSpaceInt=1`). See the `ActiveWeapon` entry under 5.6 rough edges for what was actually wrong. |
| Weapon firing | Muzzle flash / recoil on proxies, single and full-auto. |
| Weapon-mounted lights | Proxy sync works (`SpotLight.SetIntensity`). |
| NVG deploy animation | Goggles visibly lower on proxies when activated. |
| Inventory, damage, death, respawn | Server-confirmed death/respawn. |
| Ground items | Drop/pickup, persisted, with TTL expiry and a max-count budget. |
| Building placement | See `SpawnBuild` below - the hook needs rehoming on 5.6. |
| World state | Time-of-day, persisted and broadcast. |
| Zombie simulation (server side) | Spawning, AI-relevance scoping, roaming, damage/death. 16/16 unit tests plus end-to-end integration. Server-side only, so unaffected by the engine bump. |
| Native zombie suppression | Client-side spawners stopped so the server owns zombies. Still working on 5.6 (926 spawners found and stopped, up from 857 on 5.3). |
| Server directory | Cloudflare Worker; servers heartbeat in, clients list what is up. Unaffected by the engine bump. |
| Launcher | WPF app: lists servers, pings, launches the game with a join ticket. |

### Broken / disabled

| Area | Notes |
| --- | --- |
| **Zombie proxy rendering** | **Disabled in code** (`spawn_zombie_actor` returns `nullptr` unconditionally). Zombies simulate correctly server-side but are invisible to clients. Five live crashes across three attempted fixes; root cause never found. Do not re-enable casually - read the doc comment on that function first. |
| `EquipActorToSocket` | Looked up under its Blueprint *display* name (`"Equip Actor to Socket"`). This was long believed never to resolve, and was left unfixed on the grounds that enabling a never-executed `ProcessEvent` call once cost a save. **The 2026-09-24 logs disprove that**: it resolves and fires - 1,859 re-attach calls in one session, because a stale `AttachParent` offset made every equipment slot look orphaned. The offset is fixed; the display-name lookup still wants verifying. |
| `SpawnBuild` / `Svr_SpawnBuild` | Moved to `BuildingComponent` on 5.6; still looked up on the pawn, so building placement will not fire. |
| Attachment toggle-**off** | Turning a tactical light or NVG *off* did not revert on proxies (only the on-path existed). A fix is in the tree, **unverified**. |
| `research/Exports/` + `world-data.json` | Re-extracted from the .8 pak (2026-09-24). 982 spawn zones, up from 913. Regenerate with `tools/asset-export` followed by `extract-zombie-data.js`. **The directory is a mix of two engine versions**: that run refreshed 4754 files, and 9343 pre-September 5.3-era files were neither overwritten nor deleted. Nothing in the layout distinguishes them, so check a file's mtime before trusting it. A file being present here does not mean the asset exists in the current pak: `Player_AnimBP.json` is stale and the 5.6 pak has no such asset at all. For names and offsets, `research/CXXHeaderDump/` is the clean 5.6 reference and is ground truth. |

### Known rough edges on 5.6

| Area | Notes |
| --- | --- |
| Proxy dress latency | A joining player's proxy takes ~2.3s to finish dressing. Measured: 1.99s of that is the fixed `proxySpawnedAtUs` grace gate, and only 325ms is the actual 20-slot burst (~16ms/slot). The gate was added after reproducible load crashes from touching not-yet-ready proxy state, so it is left alone deliberately. The real fix is to gate on the component tree actually being populated rather than on wall-clock - now safe to attempt, since a not-ready write returns false harmlessly and retries instead of latching. |
| `Clothing_Armor` pairing | Every other clothing slot clears the bare body part underneath it. Armour has no confirmed pairing: a dressed local player reads `Clothing_Armor = MISSING` while visibly wearing a plate carrier, which is not yet understood. |
| `Biceps` / `LowerLegs` / `LowerThighs` | Covered by a real torso or legs item, but no dressed local player has been observed clearing them, so they are excluded from the clothing/body-part table rather than guessed at. They are in the per-component diagnostic dump. |
| Proxy clothing, FIXED 2026-09-26 | Root cause was `kSlotTagComparisonIndex`: 21 hardcoded FName ComparisonIndex values, from the wrong tag family AND captured against 5.3. Equipment writes addressed slots by a tag the game does not use for this, so a proxy's equipped state never became true, so every `ClothingXEquipped?` flag stayed false, so every `OnRep_ClothingXEquipped?` took its else branch and called `UpdateBodyParts` to restore bare skin over the overlay just applied. Tags are now resolved by name at runtime from `Jig.PlayerSlot.*`, which is necessary rather than tidier: the same tag resolved to ci=1572105 in one run and ci=1572206 in the next, on the same build. Also needed `BodyPartVisibility`, the half of `MC_AttachClothing` this mod never called. Worth knowing for the next visibility bug: `bVisible` is **bit 5** of the packed flag byte at `USceneComponent+0x01CA`, not bit 0. Masking bit 0 reported a healthy `bVisible=1` on components that were hidden, through nine rounds of probing. |
| Stale AnimBP names, FIXED 2026-09-26 | The player mesh's AnimBP was renamed between engine versions, `Player_AnimBP_C` to `AnimBP_PlayerCharacter`, and its variables renamed with it. The mod kept the 5.3 names, so those `GetValuePtrByPropertyNameInChain` calls returned null and the writes silently did nothing at **both** ends: `read_local_movement_flags` never read a flag, so `movState` was always 0, and `do_aim_write` never wrote one. Crouch, ADS, falling, aim pitch/yaw and left-hand IK were all affected. Renames: `IsADS`->`ADS?`, `IsCrouching`->`Crouched?`, `Falling`->`PlayerFalling?`, `InMeleeStance`->`MeleeStance?`, `Pitch`/`Yaw`->`AOPitch`/`AOYaw`, `LeftHandWeaponLocation`->`PlayerLeftHandWeaponLocation`. `CActiveSlot`, `HeadQuat` and `LeftHandWeaponRotator` have no 5.6 counterpart. `BlendSpaceInt` and `HeadRotation` kept their names, which is exactly why this stayed invisible: some lookups on the same object worked. Note the character's own bool is `IsADS?` on `ABP_PlayerCharacter_C` while the AnimBP's copy is `ADS?` - two objects, two spellings, and the AnimBP pulls from the character. |
| `ActiveWeapon` tracking, FIXED 2026-09-26 | Proxies stood in the empty-handed idle with a rifle visibly in hand. `BP_PlayerCharacter_C.GetAnimationInfo` gets what the character is holding from `BP_JigHelperComp.ActiveWeapon`: `ActiveSlot` from the tag directly, `EquippedDA` from `GetItemInfo(GetActiveWeapon())`, where `GetActiveWeapon` walks `RepActorsData` for the entry whose `Slot` equals that tag. `sync_equipment` set the tag once per weapon slot as each was first written, in slot order, so it ended up naming whichever of Primary/Secondary/Sidearm/Melee was written last and never moved. A player carrying four weapons posed by the hatchet for the rest of the session. It now follows the slot actually drawn, in `sync_active_weapon_hand`. |
| ADS on proxies, FIXED 2026-09-26 | Separately from the rename above, the mod only ever wrote `IsADS` on the AnimInstance. That is not the source of truth: `GetAnimationInfo` hands out ADS straight from the **character's** `IsADS?` bool and the AnimBP pulls it in every update, so each pull overwrote the write with false. Nothing sets `IsADS?` on a proxy, since the real one comes from local input and the `Svr_SetADS`/`MC_ADS` replication pair that a locally spawned stand-in takes no part in. Now writes the character's bool too. Deliberately not routed through `MC_ADS`/`Svr_SetADS`: both jump into `BP_PlayerCharacter`'s ubergraph (entry points 173204 and 173044), which carries camera, FOV and server-RPC work a proxy has no business running. |
| Dead name lookups, FIXED 2026-09-26 | `GetSkeletalMeshComponent` was looked up as a UFUNCTION at five sites. `ASkeletalMeshActor` has no such getter; it has a `SkeletalMeshComponent` **property** at 0x02B0 (`research/CXXHeaderDump/Engine.hpp:11207`), so the lookup always failed and always took the `K2_GetRootComponent` fallback. That happened to work because the skeletal mesh is the root, but the two only agree while no Blueprint reparents its root. Now reads the property first. Separately, `EquipClothingToMesh` does not exist anywhere in 5.6 and sat in a resolve-retry condition that could never be satisfied, so `find_local_pawn()` - a full reflection scan over every UObject - ran once a second for the whole session chasing it. |
| Proxy laser, FIXED 2026-09-26 | Three stacked problems. `ActivateState` is a persistent device **mode**, not "aiming now" - the local combo reported `StateADS` for minutes after aiming stopped - so gating the emitter on the mode alone left it lit permanently. `sync_weapon_attachments` only runs when the attachment payload changes, so no gate there could ever track aiming; the emitter now lives in `sync_attachment_laser`, driven every tick and lit only when the device is in laser mode **and** `movState & 0x02`. Finally the beam pointed the wrong way because `LaserEndPoint` returns a stale cached value on a proxy no matter how often `Event_Laser` is called - a capture showed `end=(94073,49043,-2662)` byte-identical across every sample while the emitter moved. The emitter's own transform is live and correct, so `NS_LaserSight`'s `User.Beam End` Niagara parameter is now written directly from its forward vector. **Known limit**: that projects a fixed distance instead of tracing, so the dot does not stop on walls. |
| Proxy left elbow does not bend | **Located, not explained.** Measured with every control passing (both at `BlendSpaceInt=1`, the three fixed left-arm bone lengths identical, bone names confirmed by enumeration): `upperarm_l`-`hand_l` reads 58.7-59.3 on the proxy against a 61.06 maximum, so the arm is 97% straight, while the local player reads 41.5-43.2, clearly bent. Ruled out by measurement: `PlayerLeftHandWeaponLocation` (identical constant both sides), every animation-state boolean including `InFirstPerson?`, velocity/direction/lean, the base mesh and skeleton, and any montage (`IsAnyMontagePlaying`=0 on both). Three techniques for reading anim node state all failed, see the investigation log for why each is a dead end. |
| Proxy laser endpoint | **Open.** The beam follows the OBSERVER's view, not the proxy's weapon: `LaserEndPoint` computes from `GetPlayerCharacter()`, the documented local-player-gating pattern. All three available options are wrong (follow the observer, freeze, or project a fixed distance through walls). The fix is to send the owner's own computed endpoint, which needs movement-rate cadence and so a protocol addition plus a JS relay change. |
| Proxy light and laser, FIXED 2026-09-26 | The state tag cannot express the outputs. A capture reading the local player's `SpotLight.Intensity` and the emitter's `IsActive()` beside the tag shows `StateADS` producing `light=0 laser=0`, `light=0 laser=1` AND `light=1 laser=1`, so the light is an independent toggle the tag does not encode and no mapping from it could work. The sender now packs the real outputs into spare high bits of the existing state byte (bit 6 light, bit 7 laser) and the receiver applies them. Two traps recorded: `bVisible` is useless as a laser signal because for a Niagara component visibility is not activation, and the forced `SetIntensity` on the receiver IS load-bearing, since `Jig_SetAttachmentActiveState` alone does not light a proxy. |
| Montage replay detection | Montage change detection compared asset **pointers**, so replaying a montage that was still running looked identical to it merely continuing and was dropped: two reloads in a row sent one. It now also reads `Montage_GetPosition` and treats a fall in position as a replay, and the poll moved out of the 50ms movement rate limit to every tick so a montage shorter than that window is not missed. **Unverified**: the replay path has not been observed firing. Note that firing plays no montage at all, it goes through `MuzzleEffects`/`StartRecoil`, so that was never the gap. |
| Proxy visual actors mis-placed | A spawned backpack renders up near the head. Separate mechanism from the clothing overlays: these go through `spawn_and_equip_item_visual` and `EquipActorToSocket`, not `SetSkinnedAssetAndUpdate`. Not investigated. |
| Hair / beard sync | The hair/beard write path used UE 5.3 offsets (`0x7C0`/`0x7C8` - `AIOInvoker` and `RadiationComponent` on 5.6) and therefore silently did nothing for the entire port. Now resolved by name, but **unverified**: whatever hair and beard currently render come from some other path, so fixing this may change appearance rather than merely restore it. |

### Built but never tested

| Area | Notes |
| --- | --- |
| Vehicle sync | Server side complete, client side compiles. Never run in a live test, not once. |
| Melee grip mapping | Primary/Secondary -> two-handed and Sidearm -> one-handed are confirmed; **Melee is a guess.** |
| Launcher end-to-end | Builds and runs; the full fetch-ticket-and-launch path has not been confirmed against a live server. |

### Not implemented

- **Automatic mod installation** in the launcher - it detects whether the mod is installed, but you install it by hand (see below).
- **Concurrent character creation** - two players creating characters simultaneously is a known unresolved blocker.
- Any anti-cheat worth the name. Clients are trusted for far more than they should be. Run this with people you know.

### Porting to a new engine version

Most of the raw offsets are gone - a systematic pass on 2026-09-24 took the live count from
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

- **Names in this game contain spaces and punctuation** - `Hair Color`, `IsPlayerMale?`,
  `Hunger&ThirstComponent`. Use the exact FName from the dump, not a C++-ified version.
- **Names can change too.** The 5.6 bump renamed `HungerThirstComponent` -> `Hunger&ThirstComponent`
  and `head` -> `Head`, and moved `BP_JigMultiplayer`'s class to `UBP_JigComponent_C`. A name lookup
  that silently returns null is the symptom.

Regenerate the dump against the running game with **Ctrl+H** (and **Ctrl+Num6** for `Mappings.usmap`)
via UE4SS's `Keybinds` mod, ideally with this mod disabled and a save loaded so the gameplay classes
are actually in memory.

Three more things the 5.6 port taught, each of which cost real time:

- **Fixing one offset exposes the next.** `AttachChildren` was wrong, so every attachment walk bailed
  on a garbage count and did nothing. Correcting it made the walk run for the first time - straight
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
of the matching name in `%APPDATA%\SDO\` - prefer the file:

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
afterwards, and the game inherits Steam's environment from whenever Steam launched - so setting a
variable and relaunching the game silently tests the **old** value. That invalidated a whole bisect
round before it was noticed. `session.cfg` exists for the same reason.

Each switch logs a line when it engages. If that line is absent, the switch did not reach the mod and
the result means nothing - check that before trusting any outcome.

---

## Setup

### Prerequisites

- **SurrounDead** on Steam (Windows).
- **UE4SS** - supplies the C++ modding runtime the mod loads under. **Use the rolling
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
maintained** - the two have drifted before.

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

**UE4SS changed this layout after v3.0.1** - `UE4SS.dll`, the settings file and `Mods/` all moved
into a `ue4ss/` subdirectory. On v3.0.1 and earlier they sit directly in `Win64/` instead. Putting
the mod in the wrong one fails silently: UE4SS loads, your mod simply never appears.

`enabled.txt` is what actually enables a mod - `mods.txt` alone will not do it.

### Engine version

The game moved to **UE 5.6** in the 2026-09-24 update (previously 5.3). UE4SS support for 5.6 landed
in `main` but is **not in any tagged release** - the newest tag predates 5.6 - so use the rolling
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
3. Command-line arguments - `-sdo_host=`, `-sdo_port=`, `-sdo_ticket=`

Joining needs a ticket from the server's HTTP API:

```sh
curl -X POST http://<host>:31001/v1/tickets \
  -H 'content-type: application/json' \
  -d '{"playerId":"<any stable id>","displayName":"Name"}'
```

Then launch with `-sdo_host=<host> -sdo_port=31000 -sdo_ticket=<ticket>`. Tickets are short-lived and
single-use by default, so **a fresh one is needed for every launch** - including reconnects. This is
the single most common cause of a join mysteriously failing.

### 5. Server directory (optional)

A Cloudflare Worker letting servers advertise themselves and clients discover them. Free tier is
sufficient. See [`directory-worker/README.md`](directory-worker/README.md). Skip it entirely for a
LAN or a single known host - set `SDO_DIRECTORY_URL` empty and it stays out of the way.

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
| `src/` | The C++ UE4SS mod - hooks, proxy rendering, protocol client. |
| `server/` | Gateway + host-agent. Authoritative world state, SQLite-backed. |
| `directory-worker/` | Cloudflare Worker server directory. |
| `launcher/` | WPF launcher. |
| `research/` | Reverse-engineering notes, decoded Blueprint bytecode, header dumps. The bulk of the repo. Split into current (5.6) material and `archive-ue5.3/`; see `research/README.md`. `research/check_names.py` audits the names this mod hands to the engine against the header dump - **run it after any engine upgrade**, since a renamed property returns null and is silently skipped rather than failing. Two passes: call sites (precise, prints the owning class beside the receiver, blind to names reaching a lookup through a variable) and every wide-string literal (catches names in tables and loops, at the cost of noise). Either alone misses things. |
| `scripts/` | Build/deploy/launch helpers. |

The `research/` directory is the actual substance of the project - how the game's classes,
Blueprint functions and memory layout were worked out. `research/04_ida_investigation_log.md` is a
long chronological log and the best place to understand *why* something is built the way it is.

It is organised by engine version. Current 5.6 artifacts (`CXXHeaderDump/`, `Mappings.usmap`,
`Exports/`) sit at the top level; pre-port 5.3 material - including the old hardcoded-address table
and the Blueprint catalogs - is parked in `research/archive-ue5.3/`, kept for its reasoning rather
than its numbers. `research/README.md` says what regenerates each artifact and how.

---

## Contributing

The highest-value open problems, roughly in order:

1. **Verify gameplay with two clients.** They connect, spawn and see each other as of
   2026-09-24, which is further than the port had got before. Whether movement, equipment and
   inventory actually behave is still open, and it gates everything else.
2. **Identify the two offsets left unresolved.** The `JigsawItem_DataAsset` torso mesh pair
   (`+0x448`, now `MaxWeight`) and the equip-socket read (`+0x280`, where the type looks wrong
   too). Both were deliberately not guessed at - see the "Broken / disabled" table.
3. **Rehome `SpawnBuild`/`Svr_SpawnBuild`** onto `BuildingComponent`, and verify
   `EquipActorToSocket`'s display-name lookup. Note the standing warning that this call "has never
   run" is wrong: the logs show it firing thousands of times.
4. **Zombie proxy rendering** - a genuinely hard crash bug, and the biggest single feature gap.
5. **Verify the head-look and attachment-toggle fixes.** Both are written and unverified; confirming
   or refuting them is cheap.
6. **Test vehicle sync at all.** It has never been run.
7. **Re-check the zombie archetype stats after each update.** The .8 update renamed four of them
   and turned the boss's attack damage into a range; `extract-zombie-data.js` handles both, but a
   future rename would show up as silent nulls rather than an error.
8. **Automatic mod installation** in the launcher.

A note on method, learned the hard way: for anything touching Blueprint behaviour, prefer the
flag-file diagnostics (`bytecode_dump`, `resolve_fprop`, `mem_dump`, ... - drop a `.flag` file in the
mod's log directory), the CXX header dump, and the bytecode disassembler in `research/bytecode/`
over guessing or live debugging. Several bugs here cost multiple sessions of guess-and-redeploy and
then fell over in minutes once someone logged the actual values. Two were only solved by comparing a
proxy against the local player's own correct data, and the 5.6 port only converged once the header
dump was regenerated instead of reasoned about.

The mod's own log is `debug.log`, in `%APPDATA%\SDO` by default or wherever
`SDO_LOG_DIR` points. UE4SS owns the other two (`SDO.log`, `UE4SS.log`) and puts them in its own
directory - since v3.0.1 that is `Binaries/Win64/ue4ss/`. Checking the wrong one of the three is a
recurring trap.

---

## Legal

Licensed under **AGPL-3.0-or-later** - see [LICENSE](LICENSE). The network clause matters here:
running a modified gateway or directory as a service for other players obliges you to publish that
modified source.

This project is unaffiliated with and unendorsed by the developers of SurrounDead. It ships no game
assets or game code. `research/` contains reverse-engineering notes derived from the game for
interoperability; whether that is acceptable where you live is your call to make, not this file's.
UE4SS is separately licensed by its own authors.
