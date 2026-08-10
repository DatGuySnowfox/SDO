# SD-Online Protocol Gap Analysis

Comparing `protocol.hpp`, `state.hpp`, and `mod.cpp` against all research findings
(Sessions 1–19). Gaps ordered by severity.

> **Session 26 update**: `BP_JigMultiplayer_C` (attached to every pickup and to the player) turns out to
> be a near-complete, already-built Server/Client/Multicast RPC replication system for the inventory,
> addressing items and containers by `FGuid` — not the `int32` UIDs in gap 12 or a plain `classPath`
> string. This doesn't invalidate gaps 1/11/12/13/14 below, but it means the *better* fix for several of
> them may be hooking the game's own `SERVER_*`/`CLIENT_*`/`MC_*` functions directly rather than
> reinventing drop/pickup/move wire formats from scratch. See `04_ida_investigation_log.md` Session 26
> before implementing any of the items/inventory gaps below. Not yet confirmed whether this replication
> path is live in single-player or dormant scaffolding.

---

## CRITICAL — Breaks Correctness

### 1. `itemId` is `uint32_t` everywhere — must be string — FIXED

**Status**: `InventorySlot.itemId`, `EntityDescriptorData.itemId`, and `EquipmentSlot.itemId` in
`protocol.hpp` are all `std::string` (length-prefixed UTF-8 on the wire), as recommended below.

**Affected**: `InventorySlot.itemId`, `LocalSlot.itemId`, `WorldEntity.itemId`,
`EntitySpawnData.itemId`, `ItemPickupResult.itemId`, `ItemDropReq` (implicit).

**Research finding**: Item identity is an FName string from
`JigsawItem_DataAsset_C.ItemId` at +0x30, e.g. `"DA_AK74"`, `"DA_Bandages"`.
There is no integer ID in the Jigsaw system — using uint32 requires a server-side
lookup table that doesn't exist in the game.

**Fix**: Change all `itemId` fields to `std::string` (C++) / `string` (JS).
Protocol encoding: length-prefixed UTF-8 string. Average DA_ name is ~8–20 chars.

> **Session 29 confirmation**: `FRepItemInfo.ItemID` (the game's own native per-item struct) is a raw
> `UJigsawItem_DataAsset_C*` pointer to the DA_ asset — confirms this fix is correct, not a guess: a
> pointer can't cross the network, and the DA_ name/path is the natural resolvable string on both ends.
> Note this is **item type**, distinct from **item instance** identity — the latter is
> `FContainerPickupsInfo.UniqueServerID` (`FGuid`, 16 bytes), which the protocol doesn't currently
> represent at all. Both are needed: type to know what it is, instance UID to know which specific one.

---

### 2. `send_profile_revision()` reads from `BridgeState`, not the game

**Affected**: `mod.cpp:send_profile_revision()`, `BridgeState.playerHealth/Hunger/Thirst`,
`BridgeState.inventory[]`.

**Research finding**: `BridgeState` vitals and inventory are populated only from
`PlayerProgressRestore` (server→client). There is no code that reads the actual game
state — `MedicalComponent`, `HungerThirstComponent`, or `BP_JigHelperComp_C`.

**Fix**: Add a `read_local_progress()` function using known offsets:
```cpp
// Health
UObject* med     = *(UObject**)(pawn + 0x7D0);
double health    = *(double*)(med + 0xD0);

// Hunger/Thirst
UObject* ht      = *(UObject**)(pawn + 0x7F8);
double hunger    = *(double*)(ht + 0xC8);
double thirst    = *(double*)(ht + 0xD8);

// Stamina
UObject* stam    = *(UObject**)(pawn + 0x800);
double stamina   = *(double*)(stam + 0xC8);

// Radiation
UObject* rad     = *(UObject**)(pawn + 0x7F0);
double radiation = *(double*)(rad + 0xC8);

// Level + XP (from PlayerController)
UObject* ctrl    = FindFirstOf("BP_PlayerController_C");
UObject* lvlComp = *(UObject**)(ctrl + 0x868);
int32_t  level   = *(int32_t*)(lvlComp + 0xC0);
double   xp      = *(double*)(lvlComp + 0xC8);

// Equipment (primary weapon item name)
UObject* helper  = FindFirstOf("BP_JigHelperComp_C");
uint8_t* equipped = (uint8_t*)(uintptr_t)helper + 0xF8; // S_ServerEquippedItems
UObject* itemDA  = *(UObject**)(equipped + 0x528);       // EquippedPrimary slot
// FName at itemDA + 0x30 = ItemId
```

---

### 3. `RemotePlayer` has no equipment or appearance data — DATA FIXED + LIVE-CONFIRMED, VISUALS PENDING (see gap 8)

**Affected**: `state.hpp:RemotePlayer`.

**Research finding**: Proxy actors (remote players) need to display correct clothing
and held weapon. This requires reading `BP_JigHelperComp_C.ServerEquippedItems`
(+0xF8) on the sending player and transmitting at minimum:
- Primary weapon DA_ name (slot +0x528, ItemID ObjectProperty at slot+0x0)
- Torso clothing (slot +0x1E0)
- Headwear (slot +0x78)

The `Equipment` MsgType (21) is defined but never sent or dispatched.

**Fix**: Add equipment fields to `RemotePlayer`. Hook `BP_JigHelperComp_C.SetEquippedInfoBySlot`
(NOT the `OnEquipmentUpdated` delegate — confirmed in Session 31 that hooking a dynamic multicast
delegate via `RegisterHook` registers cleanly but never actually fires; `SetEquippedInfoBySlot` is the
plain UFunction that actually performs the mutation and fires reliably) to send `Equipment` frames.
Dispatch `Equipment` frames to update `RemotePlayer` appearance and drive proxy actor mesh/anim updates.
Full 21-slot offset table for `FS_ServerEquippedItems` is in `04_ida_investigation_log.md` Session 30 —
not just the three slots listed above.

---

## IMPORTANT — Missing Features

### 4. `PlayerProgress` is missing most fields

**Affected**: `protocol.hpp:PlayerProgress`, `PLAYER_PROGRESS_BASE_SIZE`.

**Research finding**: The game tracks significantly more progression state:

| Field | Source | Offset |
|-------|--------|--------|
| `level` | `LevellingComponent` | `ctrl+0x868` → `+0xC0` |
| `currentXP` | `LevellingComponent` | `ctrl+0x868` → `+0xC8` |
| `stamina` | `StaminaComponent` | `pawn+0x800` → `+0xC8` |
| `radiation` | `RadiationComponent` | `pawn+0x7F0` → `+0xC8` |
| `forename` | `PlayerController` | `ctrl+0x8C8` (FString) |
| `surname` | `PlayerController` | `ctrl+0x8D8` (FString) |
| `respawnLoc` | `PlayerController` | `ctrl+0x930` — **`FTransform` (0x60 bytes), not just FVector3d** (Session 32 correction — includes facing rotation) |
| `zombieKills` | `PlayerController` | `ctrl+0x90C` |
| `daysSurvived` | `PlayerController` | `ctrl+0x91C` |
| Passive skills (10) | `PassiveSkillsComponent` | `ctrl+0x878` → various, full offset table in `04_ida_investigation_log.md` Session 32 |
| Equipment (21 slots) | `BP_JigHelperComp_C` | `helper+0xF8`, full table in Session 30 |

At minimum, add `level`, `stamina`, `radiation`, `forename`/`surname`, and
`respawnLoc`. The full passive skill and equipment sync can be Phase 2.

> **Session 32 addendum**: also confirmed and available if wanted — `Sex`/`Age`/`Occupation` (`ctrl+0x8E8`/`0x8F8`/`0x908`),
> `BossZombieKills`/`AnimalKills`/`HumanKills` (`ctrl+0x910`/`0x914`/`0x918`), `DistanceTravelled` (`ctrl+0x920`),
> `InfestationsDestroyed` (`ctrl+0x928`). Not in the original scope of this gap but sitting right next to the fields that are.

---

### 5. `EntityType` missing `VEHICLE` — FIXED

**Status**: `EntityKind::Vehicle = 3` exists in `protocol.hpp` (enum was renamed `EntityKind` along the
way, aligned to the JS `WorldEntityKind` values: `Unknown=0, Zombie=1, GroundItem=2, Vehicle=3,
PlacedStructure=4`).

**Affected**: `protocol.hpp:EntityType`.

**Research finding**: Session 12 confirmed 17 vehicle types all inheriting from
`BP_VehicleMaster_C`. Vehicles are `WorldEntityKind::Vehicle` in the JS server
protocol but the C++ `EntityType` enum only has `LOOT_ITEM`, `BUILDING_PIECE`,
`CONTAINER`.

**Fix**: Add `VEHICLE = 3` to `EntityType`.

---

### 6. ~~No velocity reading in `send_movement()`~~ — FIXED

**Affected**: `mod.cpp:send_movement()`.

**Original finding**: `AActor::GetVelocity()` isn't in the UE4SS stub headers, so the code fell back to
zeros.

**Fix applied**: read it directly instead of via UFunction — `ACharacter+0x328` is `CharacterMovement`
(already confirmed), and `UMovementComponent::Velocity` sits at `+0xB8` on that component (confirmed via
`research/CXXHeaderDump/Engine.hpp`), valid regardless of the exact `UCharacterMovementComponent`
subclass since base-class fields are always a prefix of the derived layout.

---

### 7. `BridgeState` missing vital and progression fields

**Affected**: `state.hpp:BridgeState`.

Fields present: `playerHealth`, `playerHunger`, `playerThirst`.
Fields missing: `playerStamina`, `playerRadiation`, `playerLevel`, `playerXP`,
passive skill levels, `playerForename`/`Surname`, `respawnLoc`.

---

### 8. `Equipment` MsgType never sent or dispatched — FIXED, LIVE-CONFIRMED

**Affected**: `protocol.hpp:MsgType::Equipment` (21), `mod.cpp:dispatch_frame()`.

**Fix applied**:
- `encode_equipment`/`decode_equipment` in `protocol.cpp`/`protocol.hpp`.
- `dispatch_frame()` decodes inbound `Equipment` frames and forwards them to
  `ProxyManager::on_equipment()`, which caches `RemotePlayer.equipment` (`state.hpp`). Appearance
  sync (mesh/anim per slot) is still a no-op — `spawn_proxy()` now spawns a real, visible proxy actor
  (see the resolved `ProxyManager` spawn gap below), but applying per-slot mesh/anim to it is a
  separate follow-up — the equipment data is cached and ready for when that lands.
- `read_local_equipment()` walks all 21 `FS_ServerEquippedItems` slots off `BP_JigHelperComp_C+0xF8`
  (Session 30 offset table) and resolves each occupied slot's `ItemID` DA pointer to its `ItemId`
  FName string via a new `native::fname_to_string()` helper.
- `send_equipment()` sends the result every 2 s from `do_game_tick()` (polling, not the
  `SetEquippedInfoBySlot` hook described below — this file has no per-UFunction hook filtering
  infrastructure yet, and polling 21 pointers is cheap).

**Live-confirmed (Session 34)**: this was the first fix in the codebase to call a raw native function
by address (`FName::ToString`, plus `FMemory::Free` to avoid leaking its output buffer — see
`04_ida_investigation_log.md` Session 34 for the full allocator trace) rather than a UE4SS stub method
or a plain offset read, so it carried real risk of a bad vtable-slot guess crashing or corrupting the
game. Deployed and run live: correctly resolved real item names (`AK15`, `BenelliM4`,
`MilitaryTacticalHelmet`, etc.) across ~20 occupied slots, tracked a live primary-weapon
equip/unequip via slot 11 appearing and disappearing between polls, and ran continuously for the
2 s poll cadence with no crash and stable memory (confirms the `GMalloc` vtable-slot math for
`Realloc`/`Free` is correct, `FString::Num` does include the null terminator on this build, and the
`GetModuleHandle`-based rebasing resolves the right runtime address).

For a future hook-driven send (event-based instead of polling), the documented real hook point is
`BP_JigHelperComp_C.SetEquippedInfoBySlot` (see gap 3 — `OnEquipmentUpdated` at +0xC30 exists and is
the "obvious" hook point but confirmed non-firing via `RegisterHook` in Session 31).

---

### 9. ~~`PlayerDamage` dispatch is a TODO~~ — FIXED

**Affected**: `mod.cpp:dispatch_frame()`.

**Original**:
```cpp
case sdb::MsgType::PlayerDamage:
    // TODO: forward to HUD
    break;
```

**Fix applied**: took the simpler of the two proposed options — write directly to
`MedicalComponent.Health`/`MaxHealth` at `pawn+0x7D0 → +0xD0`/`+0xD8` rather than routing through a
Blueprint HUD event. The frame carries the server-authoritative current/max health (a state sync, not a
delta), so a direct write is the correct semantics, not a shortcut.

> **Session 32 confirmation**: `BP_PlayerController_C+0x880` is exactly `UWidgetComponent* Widget` —
> the `+0x880` offset was correct, now confirmed against the authoritative class dump rather than a
> live property walk. Not used by the fix above, but confirmed in case a HUD-widget-event approach is
> wanted later instead.

---

## MINOR — Precision / Polish

### 10. `Movement` uses `float` — game uses `double`

UE5.3 uses large-world coordinates (LWC) with `FVector3d` (3 × double). Casting
to float loses ~7 decimal digits of precision, which matters for a large open
world. The existing 39-byte movement payload uses floats.

**Trade-off**: doubles would double the movement payload to 71 bytes. For a 50ms
interval this is still trivial bandwidth. Consider upgrading to double for X/Y/Z
while keeping yaw/velocity as float.

---

### 11. `MAX_INV_SLOTS = 40` is arbitrary

**Research finding**: `S_ServerEquippedItems` has 21 slots (equipment only). The
main inventory (backpack grid) size is unknown — the Jigsaw system uses a dynamic
TArray, not a fixed slot count. 40 may be too small or too large.

**Recommendation**: Make `MAX_INV_SLOTS` configurable at runtime, or split into
`MAX_EQUIP_SLOTS = 21` (known) and `MAX_MAIN_SLOTS = N` (to be determined).

> **Session 29 resolution — no fixed number exists, remove the constant.**
> `BP_JigMultiplayer_C.MainJigContainers` is `TArray<FS_ReplicatedContainerInfo>`, and each entry
> carries its own runtime `Columns`/`Rows` (resizable live via `ExpandContainer`) plus a plain
> `TArray<FContainerPickupsInfo> ContainerItems` — never a fixed slot count. `MAX_EQUIP_SLOTS = 21` is
> right and stays fixed (that's the only genuinely fixed-size part, `S_ServerEquippedItems`). For the
> main inventory, drop `MAX_MAIN_SLOTS` entirely and encode per-container `Columns`/`Rows` plus a
> variable-length item list in the wire format instead — trying to pick "the right N" is solving the
> wrong problem, since containers can be resized mid-game and the game itself never assumes a cap.

**CLOSED — 2026-08-10.** `PlayerProgress`'s inventory field changed from a flat, globally-indexed
`slots: InventorySlot[]` capped at `MAX_INV_SLOTS = 40` to `containers: InventoryContainer[]`, each
carrying its own `columns`/`rows` and an independent, uncapped item list (`protocol.hpp`/`protocol.cpp`,
mirrored in `server/src/lib/protocol.js`). `mod.cpp`'s `read_local_inventory()` now emits one
`InventoryContainer` per real container found in `MainJigContainers` (columns/rows read directly, same
placeholder-skipping logic as before), with no cap on container count or per-container item count.
`state.hpp`'s `LocalSlot inventory[MAX_INV_SLOTS]` array was removed outright — it was genuinely dead code
(declared, never read or written anywhere).

This is a breaking wire-format change; no migration was written for `ProfileRevision` payloads persisted
under the old flat format in the server's SQLite DB (dev-stage mod). `host-agent.js`'s separate, in-memory
`MAX_INV_SLOTS = 40` flat array for live `ItemPickupRequest`/`ItemDropRequest` slot-targeting was left
as-is and is unaffected — the authoritative save/restore path persists `ProfileRevision`'s raw payload
bytes verbatim (`gateway.js`), so the full container structure round-trips correctly regardless of that
separate, smaller live-session bookkeeping cap.

Verified via cross-language round-trip rather than a live session (this is a pure encoding change, not an
engine-behavior one): a standalone `protocol.cpp` build (no UE4SS dependency) encoded a `PlayerProgress`
with two containers, decoded correctly on both the C++ and JS sides, and the JS decoder correctly parsed
the raw C++-encoded bytes byte-for-byte; the reverse direction (JS-encoded bytes decoded by C++) was also
confirmed. All 37 existing `tests/integration.js` cases still pass.

---

### 12. ~~`AllUIDs` is `TArray<int32>` — entityId mapping needed~~ — CLOSED, not applicable

**Original assumption**: `BP_SurroundeadGameState_C.AllUIDs` at +0x338 tracks world
item UIDs as `int32`. The protocol uses `uint64_t entityId`. The server must map
its uint64 entity IDs to the game's int32 UIDs when coordinating loot despawn.

**Session 33 finding — this gap doesn't exist**: live-tested directly. `AllUIDs` stayed at 0 across
multiple confirmed drop and pickup actions — normal gameplay never touches it. It sits alongside
`BP_SurroundeadGameState_C`'s item-icon snapshot/thumbnail-capture system fields and functions
(`ItemsQueue`, `AllInspectedIDs`, `SpawnSnapshotCaptor`, `HandleSnapTaken`, etc.), and is almost
certainly internal bookkeeping for that unrelated system, not a world-item spawn registry. There is
nothing to map here. **Use `FContainerPickupsInfo.UniqueServerID` (`FGuid`) instead** — that's the
real per-instance world-item identity (see gap 1's addendum and `04_ida_investigation_log.md` Sessions
29/33).

---

## Session 21 Addendum — Wire Format Mismatches (ALL CRITICAL)

Full source review (`protocol.cpp`, `tcp_client.cpp`, `gateway/service.js`,
`host-agent/service.js`) reveals that several message types that appeared correct
are actually broken because the C++ bridge uses hand-rolled binary encoding while
the JS host-agent uses `encodeWorldAction` / `decodeWorldAction` (length-prefixed
JSON). The gateway is a pass-through — it does NOT validate or transcode payloads
beyond what is shown below.

### 13. Request/Result types: binary C++ vs JSON JS — FIXED

**Status**: `send_item_pickup_request`/`send_item_drop_request` (`mod.cpp`) build JSON via
`encode_world_action` with a `requestId`; `dispatch_frame()`'s `ItemPickupResult`/`ItemDropResult`/
`InteractionResult` cases all decode via `decode_world_action` + `json_bool`/`json_str`. No binary
structs remain on either side of this boundary.

**Affected C++ → JS (client to host):**

| Frame | C++ sends | JS expects | Effect |
|-------|-----------|-----------|--------|
| `ItemDropRequest (34)` | 16-byte binary struct | JSON `{requestId,itemId,classPath,quantity,x,y,z}` | silently dropped by host |
| `ItemPickupRequest (35)` | 2-byte binary | JSON `{requestId,entityId}` | silently dropped by host |
| `InteractionRequest (26)` | 22-byte binary BUILD struct | JSON `{action,…}` | silently dropped by host |

**Affected JS → C++ (host to client):**

| Frame | JS sends | C++ expects | Effect |
|-------|----------|-------------|--------|
| `ItemDropResult (39)` | JSON via `encodeWorldAction` | 3-byte binary `[tag,success,reason]` | decode returns nullopt; discarded |
| `ItemPickupResult (36)` | JSON via `encodeWorldAction` | 9 or 3-byte binary | decode returns nullopt; discarded |
| `InteractionResult (27)` | JSON via `encodeWorldAction` | 3-byte binary | decode returns nullopt; discarded |

**Fix**: Replace all binary encode/decode for these types with JSON
(`encodeWorldAction`/`decodeWorldAction`). The JS server side is already correct;
only the C++ side needs to change.

For **requests** the C++ must include a `requestId` (UUID or counter string) in
the JSON. The JS uses `requestId` for idempotency and deduplication. For
**results** the C++ must parse the JSON rather than a fixed-size binary struct.

---

### 14. `EntitySpawn` payload format mismatch — FIXED

**Status**: `protocol.hpp`'s `EntityDescriptorData` (kind/revision/quantity/ownerPlayerId/classPath/
itemId, variable length) and `EntityStateData` (kind/revision/x/y/z/yaw/health/state, fixed 27 bytes)
match the JS `encodeWorldEntityDescriptor`/`encodeWorldEntityState` shapes described below exactly;
`dispatch_frame()`'s `EntitySpawn`/`EntityState` cases decode them separately and forward to
`EntityManager`.

**Affected**: `decode_entity_spawn()` in `protocol.cpp`, `entity_manager.cpp`.

JS `encodeWorldEntityDescriptor` sends:
```
[tag=1, kind=uint8, revision=uint32, quantity=uint16, ownerPlayerId=uint64,
 classPathLen=uint16, classPath..., itemIdLen=uint16, itemId...]
```

C++ `decode_entity_spawn` reads:
```
[tag=1, entityType=uint8, posX=float, posY=float, posZ=float, yaw=float,
 stateLen=uint16, stateBlob...]
```

Consequences:
- Position bytes 2–17 contain `revision|quantity|ownerPlayerId` — garbage floats.
- `stateLen` reads partial `ownerPlayerId` bytes — garbage, causes potential
  overread.
- Entity kind mapping is wrong: JS `GroundItem=2` → C++ reads as `CONTAINER=2`
  (should be `LOOT_ITEM=0`); JS `Zombie=1` → C++ reads as `BUILDING_PIECE=1`.
- The JS sends a **separate** `EntityState` (27-byte binary, `encodeWorldEntityState`)
  for position. The C++ currently ignores `EntityState` (dispatch falls through
  to a no-op comment).

**Fix**:
1. `decode_entity_spawn` must parse `encodeWorldEntityDescriptor` format (variable
   length, no position).
2. Add `decode_entity_state` that parses `encodeWorldEntityState` (27 bytes: tag,
   kind, revision, x/y/z/yaw/health as floats, state byte).
3. Split `WorldEntity` into descriptor (from EntitySpawn) + state (from EntityState).
4. Add entity kind enum aligned to `WorldEntityKind` (Zombie=1, GroundItem=2,
   Vehicle=3, PlacedStructure=4).

---

### 15. `PlayerProgressRestore` payload is a Movement frame — FIXED

**Status**: `dispatch_frame()`'s `PlayerProgressRestore` case calls `decode_player_progress` (not
`decode_movement`), matching the gateway's actual behavior of replaying the last-saved
`ProfileRevision` payload verbatim. It applies the restored position via `pendingTeleport` /
`K2_SetActorLocationAndRotation` on the next tick, as recommended below.

JS sends `encodeMovement` (39 bytes) as the `PlayerProgressRestore` payload —
position/yaw only, no vitals or inventory. The C++ `decode_player_progress`
checks `p[0] != 1` (tag byte), but the first byte of a Movement payload is the
high byte of the X float — almost never 1. Result: `decode_player_progress`
returns `nullopt`, the restore is silently ignored, and position is never
restored on rejoin.

**Fix**:
- For `PlayerProgressRestore`, call `decode_movement` (not `decode_player_progress`).
- Apply the movement to teleport the pawn to the saved position using the already-documented
  `K2_SetActorLocationAndRotation` exec thunk (`0x142AC5500`, see `04_ida_investigation_log.md`
  Session 5/9): resolve `RootComponent` at `actor+0x1A0`, then dispatch through
  `RootComponent->vtable[1312/8]` (`MoveComponentImpl`) with `bTeleport = true` (passed through as
  `!bTeleport` to the teleport-type argument). This is a real engine call, not a raw memory write —
  it handles collision/sweep correctly and is already fully reverse-engineered, no further research
  needed.
- If vitals restore is needed later, design a separate extended payload.

---

## Current Status (updated after Session 34 audit — 2026-08-07)

Every gap numbered above is now **FIXED**: 1, 3, 5, 6, 8, 9, 12, 13, 14, 15 (12 was closed by
determining it never existed, not by writing code). What's actually working end-to-end:

- Frame header format, magic, version ✓
- TCP transport + HMAC ticket auth ✓
- `Movement` encode/decode ✓ including velocity (minor: still float, not double — see gap 10)
- `WorldState` decode ✓
- `Death`/`Respawn`/`DeathRequest`/`RespawnRequest` flow ✓
- `PlayerConnected`/`PlayerDisconnected` + proxy lifecycle (position/state tracking only — see below) ✓
- `PlayerDamage` decode + dispatch ✓ (writes straight to `MedicalComponent`)
- `EntitySpawn`/`EntityState`/`EntityDespawn` ✓ (correct JS-matching formats)
- `ItemPickup`/`ItemDrop`/`Interaction` request+result ✓ (JSON both directions)
- `PlayerProgressRestore` ✓ (correct decode + teleport-on-restore)
- `Equipment` — send (local read) and receive (cache on `RemotePlayer`) ✓, live-confirmed Session 34
- Auto-open world via `MenuWidget_C` ✓
- Profile revision every 30s, equipment every 2s, both reading live game state (not stale
  `BridgeState`) ✓

### Main inventory / container sync — READ SIDE FIXED + LIVE-CONFIRMED (Session 35), gap 11 still open

`send_profile_revision()`'s `PlayerProgress.slots` is no longer always empty: `read_local_inventory()`
walks `BP_JigMultiplayer_C.MainJigContainers` (Session 26/29) and flattens every real container's items
into the existing flat-slotIndex wire format — no protocol/JS changes needed, since it reuses the
`ProfileRevision` format that was already fully wired end-to-end. Live-confirmed across two play
sessions: resolved a real, varied inventory (ammo, meds, currency, keycards, etc.) with no crash, and
correctly filtered out 21 placeholder container entries (`Columns=Rows=-1`, one per equipment slot) that
would otherwise have duplicated equipment data into the inventory slots. Also fixed a latent correctness
bug found along the way: both `read_local_equipment()` and `read_local_inventory()` now read
`BP_JigHelperComp`/`BP_JigMultiplayer` directly off the pawn (`+0x700`/`+0x818`,
`CXXHeaderDump/BP_PlayerCharacter.hpp`) instead of `FindFirstOf` by class name — `FindFirstOf` only
happened to return the right instance in solo testing; `BP_JigMultiplayer_C` in particular exists on
every ground pickup too, so it would have been a real bug with more than one player in the world.

**Still open — gap 11 itself**: this is a v1 flattening, not a true fix. Session 35's second live test
directly confirmed the `MAX_INV_SLOTS = 40` cap is a real limitation, not just theoretical — an actual
loadout exceeded 40 items across its real containers and got silently truncated. The full fix Session 29
already specified (per-container `Columns`/`Rows` + a variable-length item list, dropping the fixed cap
entirely) needs a matching wire-format and decoder change in `server/src/lib/protocol.js` and
`host-agent.js` (which has its own independent `MAX_INV_SLOTS = 40`) — a cross-language change, left for
a future session.

### Other genuinely still open items

- **Gap 4/7 — "at minimum" scope FIXED + live-confirmed (Session 37)**: `PlayerProgress` now carries
  `forename`/`surname`, zombie/boss-zombie/animal/human kill counts, `daysSurvived`,
  `distanceTravelled`, and `infestationsDestroyed`, read off `ABP_PlayerController_C` at the offsets
  Session 32 confirmed and appended to the wire format as a trailer after the existing slot list (so
  old persisted saves without the trailer still decode fine — see `server/src/db.js`'s raw-byte
  persistence). Updated on both sides: `protocol.hpp`/`.cpp` and `server/src/lib/protocol.js` +
  `host-agent.js`'s `_applyProfileRevision` (stores the trailer onto `p.stats`). Live-tested:
  `forename`/`surname` resolved correctly (`John`/`Doe`) via a new `native::read_fstring_field()` — the
  first code in the repo reading a plain in-place `FString` rather than calling `FName::ToString` — no
  crash, stable. **Still open**: `respawnLoc` (needs the `FTransform`'s internal `FQuat`/`FVector`
  sub-offsets live-verified, not just its total size) and all 10 passive skills — both explicitly
  deferred as separate, larger follow-ups per Session 32's own framing.
- **Gap 10** — `Movement` position fields are `float`, engine uses `double` (LWC) — precision loss,
  not correctness-breaking at current world scale. Low priority.
- **`ProxyManager` — spawn attempted, rejected live for two different classes (Session 36)**:
  `spawn_proxy()` is no longer a stub — it resolves a `UClass*` (fixing a real linker bug in the
  vendored UE4SS stub's `GetClassPrivate()` along the way, which was mangled against the wrong owning
  class) and calls `UWorld::SpawnActor`. Live-tested with both `BP_PlayerCharacter_C` and `BP_Zombie_C`:
  every spawn attempt for either class was cleanly rejected (no crash, clean `nullptr`). Zombies are
  definitely not `CLASS_NotPlaceable` (the game's own spawner places them constantly), which rules out
  the leading hypothesis from Session 5's `SpawnActor` validation checklist — the blocker isn't which
  class is targeted, it's something more fundamental about how this mod calls the UE4SS `SpawnActor`
  wrapper. Needs IDA-level investigation of that export before another live attempt is worth making.
  Also fixed a real incident found during testing: `ProxyManager::tick()` had no cooldown between spawn
  retries, so a failing `spawn_proxy()` retried on every tick and visibly tanked the frame rate — now
  throttled to one attempt per player per 5s, confirmed effective (stable frame rate on the follow-up
  test) and correct regardless of what eventually fixes the spawn itself. `RemotePlayer.equipment`/
  `x,y,z,yaw`/`health` are all tracked correctly and ready to drive a proxy the moment one actually
  spawns — this remains the biggest gap toward the mod being visually functional in multiplayer.

  **Session 38 update**: full IDA trace of the native call chain (`04_ida_investigation_log.md` Session
  38) confirms this is a universal rejection (UE4SS's own Lua `SpawnActor` binding fails identically, not
  just our mod's call) happening somewhere inside 100%-stock Unreal Engine code — no SurrounDead-specific
  anti-cheat gate found anywhere in `BeginDeferredActorSpawnFromClass`, `GetWorldFromContextObject`, or
  the real native `SpawnActor`. Leading candidate is the virtual `GetWorld()` call `GetWorldFromContextObject`
  makes on the passed `UWorld*` (vtable+392) possibly hitting a shifted vtable slot for this engine build,
  but this can't be confirmed without live debugger value inspection — Shipping builds strip all `UE_LOG`
  output, so there's no log evidence either way. Next step needs either a live IDA debugger session with a
  breakpoint at the resolved addresses, or finding and hooking whatever simpler native function the game's
  own zombie/loot spawners use internally instead of going through this generic reflection path.

  **RESOLVED — Session 40**: root cause found and fixed via live tracing directly into `UE4SS.dll`
  (`04_ida_investigation_log.md` Session 40). The Session 38 hypothesis was on the wrong binary — a live
  breakpoint on the actual native `BeginDeferredActorSpawnFromClass`/`SpawnActor` chain in the game's own
  executable never fired despite confirmed, continuous real `spawn_proxy()` retries, meaning the Session
  38 trace had been watching an unrelated coincidental spawn (almost certainly the local player's own
  pawn) rather than this mod's call. The real call path is `UE4SS.dll`'s own exported
  `RC::Unreal::UWorld::SpawnActor`, found live by parsing the module's PE export table directly and
  resolving its RVA. Tracing that function's real internals (not the vendored SDK header, the actual
  compiled code) showed it calls UE4SS's own `BeginDeferredActorSpawnFromClass` and `FinishSpawningActor`
  implementations, both of which bottom out in the same internal helper — an FNV-1a hash + hashtable
  probe that never resolves on this build, permanently returning null regardless of class or world
  validity. This is a single shared bug in UE4SS's own reflection-dispatch cache for this engine build,
  not anything specific to this mod, this class, or the earlier `GetClassPrivate()` workaround.

  Fixed by bypassing UE4SS's wrapper entirely: `spawn_proxy()` now calls the game's own native
  `BeginDeferredActorSpawnFromClass` and `AActor::FinishSpawning` directly by resolved address (same
  by-address-workaround pattern already used for `GetClassPrivate()`), constructing the native 96-byte
  UE5 LWC `FTransform` by hand. `FinishSpawning`'s real implementation was identified by its signature
  match to UE5 source — a one-time `bHasFinishedSpawning`-style guard flag at offset+92, then a full
  parent-relative transform composition before applying it — found as the callee of the native
  "Finish Spawning Actor" Kismet exec thunk. Live-tested end to end: the synthetic test proxy spawns as a
  real, visible character actor with no crash and no UE4SS exceptions. This closes the biggest remaining
  gap toward the mod being visually functional in multiplayer. The Session 36 TEMP test hook in
  `mod.cpp` has been removed now that spawning is confirmed live.
