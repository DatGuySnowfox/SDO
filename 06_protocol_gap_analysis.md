# SD-Online Protocol Gap Analysis

Comparing `protocol.hpp`, `state.hpp`, and `mod.cpp` against all research findings
(Sessions 1–19). Gaps ordered by severity.

---

## CRITICAL — Breaks Correctness

### 1. `itemId` is `uint32_t` everywhere — must be string

**Affected**: `InventorySlot.itemId`, `LocalSlot.itemId`, `WorldEntity.itemId`,
`EntitySpawnData.itemId`, `ItemPickupResult.itemId`, `ItemDropReq` (implicit).

**Research finding**: Item identity is an FName string from
`JigsawItem_DataAsset_C.ItemId` at +0x30, e.g. `"DA_AK74"`, `"DA_Bandages"`.
There is no integer ID in the Jigsaw system — using uint32 requires a server-side
lookup table that doesn't exist in the game.

**Fix**: Change all `itemId` fields to `std::string` (C++) / `string` (JS).
Protocol encoding: length-prefixed UTF-8 string. Average DA_ name is ~8–20 chars.

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

### 3. `RemotePlayer` has no equipment or appearance data

**Affected**: `state.hpp:RemotePlayer`.

**Research finding**: Proxy actors (remote players) need to display correct clothing
and held weapon. This requires reading `BP_JigHelperComp_C.ServerEquippedItems`
(+0xF8) on the sending player and transmitting at minimum:
- Primary weapon DA_ name (slot +0x528, ItemID ObjectProperty at slot+0x0)
- Torso clothing (slot +0x1E0)
- Headwear (slot +0x78)

The `Equipment` MsgType (21) is defined but never sent or dispatched.

**Fix**: Add equipment fields to `RemotePlayer`. Hook `OnEquipmentUpdated` delegate
on `BP_JigHelperComp_C` to send `Equipment` frames. Dispatch `Equipment` frames
to update `RemotePlayer` appearance and drive proxy actor mesh/anim updates.

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
| `respawnX/Y/Z` | `PlayerController` | `ctrl+0x930` (FVector3d) |
| `zombieKills` | `PlayerController` | `ctrl+0x90C` |
| `daysSurvived` | `PlayerController` | `ctrl+0x91C` |
| Passive skills (10) | `PassiveSkillsComponent` | `ctrl+0x878` → various |
| Equipment (21 slots) | `BP_JigHelperComp_C` | `helper+0xF8` |

At minimum, add `level`, `stamina`, `radiation`, `forename`/`surname`, and
`respawnX/Y/Z`. The full passive skill and equipment sync can be Phase 2.

---

### 5. `EntityType` missing `VEHICLE`

**Affected**: `protocol.hpp:EntityType`.

**Research finding**: Session 12 confirmed 17 vehicle types all inheriting from
`BP_VehicleMaster_C`. Vehicles are `WorldEntityKind::Vehicle` in the JS server
protocol but the C++ `EntityType` enum only has `LOOT_ITEM`, `BUILDING_PIECE`,
`CONTAINER`.

**Fix**: Add `VEHICLE = 3` to `EntityType`.

---

### 6. No velocity reading in `send_movement()`

**Affected**: `mod.cpp:send_movement()` line ~114.

**Research finding**: `AActor::GetVelocity()` is a standard UE5 method available
via UE4SS (`pawn->GetVelocity()`). The current code falls back to zeros.

**Fix**:
```cpp
const FVector vel = pawn->GetVelocity();
mv.velocityX = static_cast<float>(vel.X);
mv.velocityY = static_cast<float>(vel.Y);
mv.velocityZ = static_cast<float>(vel.Z);
```

---

### 7. `BridgeState` missing vital and progression fields

**Affected**: `state.hpp:BridgeState`.

Fields present: `playerHealth`, `playerHunger`, `playerThirst`.
Fields missing: `playerStamina`, `playerRadiation`, `playerLevel`, `playerXP`,
passive skill levels, `playerForename`/`Surname`, `respawnLoc`.

---

### 8. `Equipment` MsgType never sent or dispatched

**Affected**: `protocol.hpp:MsgType::Equipment` (21), `mod.cpp:dispatch_frame()`.

The message type is defined but has no encode/decode, no send path, and no dispatch
handler. The hook point is `OnEquipmentUpdated` delegate on `BP_JigHelperComp_C`
at +0xC30.

---

### 9. `PlayerDamage` dispatch is a TODO

**Affected**: `mod.cpp:dispatch_frame()` line ~288.

```cpp
case sdb::MsgType::PlayerDamage:
    // TODO: forward to HUD
    break;
```

**Fix**: Read `HUD.Widget` from `BP_PlayerController_C + 0x880` and call a
Blueprint event to update the health bar. Or write directly to
`MedicalComponent.Health` at `pawn+0x7D0 → +0xD0`.

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

---

### 12. `AllUIDs` is `TArray<int32>` — entityId mapping needed

**Research finding**: `BP_SurroundeadGameState_C.AllUIDs` at +0x338 tracks world
item UIDs as `int32`. The protocol uses `uint64_t entityId`. The server must map
its uint64 entity IDs to the game's int32 UIDs when coordinating loot despawn.

---

## What's Already Correct

- Frame header format, magic, version ✓
- TCP transport + HMAC ticket auth ✓
- Movement encode/decode ✓ (minus velocity and float vs double)
- WorldState fields ✓
- Death/Respawn/DeathRequest/RespawnRequest flow ✓
- EntitySpawn/EntityDespawn lifecycle ✓
- ItemPickup/Drop request/result flow ✓ (minus string itemId)
- InteractionRequest BUILD ✓
- PlayerConnected/Disconnected + proxy lifecycle ✓
- Auto-open world via MenuWidget_C ✓
- 30-second profile revision cadence ✓

---

## Recommended Fix Order

1. **itemId string** — touches protocol, state, mod, server JS, all tests. Do first.
2. **read_local_progress()** — read vitals/level/equipment from game directly.
3. **PlayerProgress extended fields** — add level, stamina, radiation, names, respawnLoc.
4. **Equipment frame** — hook OnEquipmentUpdated, encode/send, update proxy appearance.
5. **Velocity** — one-liner fix in send_movement().
6. **EntityType::VEHICLE** — one-liner enum addition.
7. **PlayerDamage dispatch** — write health to MedicalComponent.
8. **RemotePlayer equipment fields** — for proxy visual fidelity.
