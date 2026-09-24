# Lua Script Analysis — SD-Online Client Bridge

Source package: `SD-Online-Player-3.0.0-alpha.138`
Path: `payload/src/client-bridge/`
Scripts read: `main.lua`, `phase5h21_appearance_only.lua`, `fixed_day.lua`,
              `world_event_probe.lua`, `targeted_world_probe.lua`,
              `visual_capability_probe.lua`, `world_capability_probe.lua`

---

## Architecture Overview

```
main.lua (orchestrator)
  ├─ dofile(phase5h21_appearance_only.lua)   — always
  ├─ dofile(visual_capability_probe.lua)     — always
  ├─ dofile(targeted_world_probe.lua)        — if SDO_WORLD_PROBE_ENABLED=1
  ├─ dofile(world_event_probe.lua)           — if SDO_WORLD_PROBE_ENABLED=1
  └─ dofile(fixed_day.lua)                   — if NOT protocol-v3 mode

LoopAsync(180ms)   — heartbeat, legacy capture, native hook registration
LoopAsync(250ms)   — spawn alignment
LoopAsync(500ms)   — remote player count log
```

---

## Environment Variables

| Variable | Default | Purpose |
|----------|---------|---------|
| `SDO_GAME_DATA_DIR` | `<Win64>\SurrounDeadOnline` | File IPC directory |
| `SDO_GAME_BRIDGE_MODE` | `legacy-file` | `legacy-file` or `protocol-v3` |
| `SDO_WORLD_PROBE_ENABLED` | `0` | Enables world probe scripts |

---

## File IPC Layout (all under `dataPath`)

### Status / Handshake Files
| File | Writer | Content |
|------|--------|---------|
| `lua_native_bridge_loaded.txt` | Lua | Capability manifest on startup |
| `lua_native_bridge_error.txt` | Lua | Error messages |
| `game_heartbeat.txt` | Lua | `os.time()` timestamp, written every 180ms |
| `lua_capture_status.txt` | Lua | Version, captureCount, source, map — updated once/sec |
| `lua_capture_ready.txt` | Lua | Written on first successful capture |

### Local Player State (legacy-file mode)
| File | Format |
|------|--------|
| `local_state.tsv` | `X\tY\tZ\tYaw\tmapName` |

### Remote Players (legacy-file mode)
| File | Format |
|------|--------|
| `remote_players.tsv` | `id\tname\tX\tY\tZ\tYaw\tmapName` per line |

Validation: coordinates must be `|v| < 10,000,000` and not all-zero.

### Native Item Events (protocol-v3 + `native_drop_hook.enable.txt`)
| File | Content |
|------|---------|
| `local_native_drop_<requestId>.txt` | Drop event: version, requestId, actor, classPath, itemId, sourceUid, dataAsset, quantity, x, y, z, yaw |
| `local_native_pickup_<requestId>.txt` | Pickup event: requestId, actor, classPath, x, y, z |
| `local_native_zombie_attack_<requestId>.txt` | Zombie hit: requestId, actor, damage, lethal |
| `local_native_drop_status.txt` | Hook registration count + last drop info |
| `local_native_pickup_status.txt` | Last pickup info |
| `local_native_drop_actor_capabilities.txt` | Property dump of dropped actor |
| `local_held_item_status.txt` | Currently equipped weapon (version 2) |

### Spawn Alignment
| File | Direction | Content |
|------|-----------|---------|
| `spawn_alignment_once.txt` | runtime→Lua | Trigger: presence means "align now" |
| `spawn_alignment_target.tsv` | runtime→Lua | `X\tY\tZ\tYaw\tmapName\tmode` |
| `lua_spawn_alignment_result.txt` | Lua→runtime | Success: spawnAligned=1, actual position, errorDistance |
| `lua_spawn_alignment_error.txt` | Lua→runtime | Failure reason |

Alignment validation:
- Map must contain `"PersistentLevel"` (rejects non-game-world levels)
- `errorDistance < 1500` UU accepted
- Uses `K2_SetActorLocationAndRotation(target, rot, false, hitResult, true)`

### Asset Loading
| File | Direction | Content |
|------|-----------|---------|
| `world_asset_load_request.txt` | runtime→Lua | Class path (must be `/Game/Inventory/Items/..._C`) |
| `world_asset_load_status.txt` | Lua→runtime | loaded=true/false + error |

Loads via `LoadAsset(assetPath)` in game thread. Only `_C` suffixed paths under `/Game/Inventory/Items/` are accepted.

---

## Hook Paths — Native Item System

Only registered in `protocol-v3` mode with `native_drop_hook.enable.txt` present.
All via `RegisterHook(path, callback)`.

| Path | Event | Callback args of interest |
|------|-------|--------------------------|
| `.../BP_JigMultiplayer_C:ItemDropRequest_Event_0` | Before drop | arg 3 = quantity |
| `.../BP_PlayerCharacter_C:JigMP_OnItemDropped` | After drop | arg 1+ = actor candidates; item guid from `select(2,...)` |
| `.../BP_PlayerCharacter_C:OnPickupInteractExecuted` | Pickup | arg 2 = actor, arg 4 = result bool |
| `.../BP_MasterZombie_C:ReceiveAnyDamage` | Zombie hit | arg 1 = zombie, arg 2 = damage |
| `.../BP_MasterZombie_C:Death` | Zombie death | arg 1 = zombie (damage=50 synthetic) |
| `.../BP_PlayerCharacter_C:JigMP_OnPickupEquipped` | Equip | arg 2=actor, 3=container, 4=uid, 8=overrideActor, 10=itemId |
| `.../BP_PlayerCharacter_C:Unequip` | Unequip | clears held item status |

Full paths:
```
/Game/JigSInventory/Jigsaw/Components/BP_JigMultiplayer.BP_JigMultiplayer_C:ItemDropRequest_Event_0
/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C:JigMP_OnItemDropped
/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C:OnPickupInteractExecuted
/Game/AI/Zombies/BP_MasterZombie.BP_MasterZombie_C:ReceiveAnyDamage
/Game/AI/Zombies/BP_MasterZombie.BP_MasterZombie_C:Death
/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C:JigMP_OnPickupEquipped
/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C:Unequip
```

`nativeHooksReady` = true only when all 7 hooks registered.

Drop publication is delayed 150ms to allow `ItemDropRequest_Event_0` to
complete first and set the quantity (game event ordering).

---

## Item Identity System

Items are identified by two properties on the actor or its `BP_JigPickupComponent`:

| Property | Type | Format |
|----------|------|--------|
| `UID` | FGuid (struct with A,B,C,D) | `"A:B:C:D"` as integers |
| `DA` | UObject reference (DataAsset) | Full UE path string |

Class path extraction:
```lua
actor:GetClass():GetFullName()  -- e.g. "BlueprintGeneratedClass /Game/Inventory/Items/BP_Rifle.BP_Rifle_C"
-- pattern: match "(/Game/[^%s]+)"
```

Item ID from class path: last path segment, strip dot extension.
- `/Game/Inventory/Items/BP_Rifle.BP_Rifle_C` → `BP_Rifle_C`

Allowed ground item class path prefixes:
- `/Game/Inventory/`
- `/Game/JigSInventory/`
- `/Game/Items/`
- `/Game/Blueprints/Items/`

---

## Player Capture — Local State

```lua
pawn:K2_GetActorLocation()   -- {X, Y, Z}
pawn:K2_GetActorRotation()   -- {Pitch, Yaw, Roll}
pawn:GetWorld():GetFName():ToString()  -- map name
```

Player acquisition order:
1. `UEHelpers:GetPlayerController()` → `.Pawn`
2. `FindFirstOf("BP_PlayerController_C")` → `.Pawn` / `:GetPawn()`
3. `FindFirstOf("BP_PlayerCharacter_C")` fallback

---

## Remote Players — Proxy Placement (legacy-file mode)

Each line in `remote_players.tsv` becomes a proxy:
```
id\tname\tX\tY\tZ\tYaw\tmapName
```
The Lua mod reads this file on the 500ms loop to log counts. The C++ mod
handles proxy rendering; Lua only provides diagnostics.

---

## Proxy Setup (phase5h21_appearance_only.lua)

1. `FindAllOf("BP_PlayerCharacter_C")` — get all character actors
2. Filter: address != local pawn
3. Wait 25 ticks (5 seconds at 200ms) for stability
4. Apply per proxy:
   ```lua
   proxy:LoadPlayerInventory()
   proxy:SetActorHiddenInGame(false)
   proxy:SetActorEnableCollision(false)
   proxy:SetActorTickEnabled(false)
   ```
5. Appearance slots applied: CharacterMesh0/Mesh/CharacterMesh, then:
   `Clothing_Feet`, `Clothing_Legs`, `Clothing_Torso`, `Clothing_Gloves`,
   `Clothing_Armor`, `HairMesh`, `BeardMesh`, `EyebrowsMesh`

Triggers: `phase5h21.appearance_once.txt` (enable), `phase5h21.disable_appearance.txt` (disable)

---

## Weather / Time of Day (fixed_day.lua)

Class: `Ultra_Dynamic_Sky_C`
```lua
skyObject["Time Of Day"] = 1200.0     -- noon (0-2400 scale)
skyObject["Animate Time of Day"] = false
```
Trigger file: `fixed_day.enable.txt`
In `protocol-v3` mode this file is NOT loaded (weather sync handled by C++ DLL via `WorldState` msg 30).

---

## Event Hooks (world_event_probe.lua)

### Inventory Hooks
| Path | Purpose |
|------|---------|
| `.../BP_JigMultiplayer_C:HandleDropRequest` | Drop initiated |
| `.../BP_JigMultiplayer_C:HandleServerDropRequest` | Server-authoritative drop |
| `.../BP_JigMultiplayer_C:ItemDropRequest_Event_0` | Actual drop event |
| `.../BP_JigMultiplayer_C:PickupInfoFromActor` | Pickup probe |
| `.../BP_JigMultiplayer_C:ServerFunc_HandleRequestAddActorToContainer` | Container add |
| `.../BP_LootContainer_C:JigMP_OnRequestDropItem` | Container drop |
| `.../BP_LootContainer_C:JigMP_OnPickupAdded` | Container pickup |
| `.../BP_PlayerCharacter_C:JigMP_OnItemDropped` | Item dropped from player |
| `.../BP_PlayerCharacter_C:JigMP_OnPickupAdded` | Item added to player |
| `.../BP_PlayerCharacter_C:OnPickupInteractExecuted` | Pickup executed |
| `.../BP_PlayerCharacter_C:PickupBuildFromGround` | Build piece pickup |

---

## World / Entity Probe (targeted_world_probe.lua)

### Zombie Classes
```
BP_Zombie_Roamer_C
BP_Zombie_C
BP_Zombie_AI_C
BP_MasterZombie_C
BP_MasterAI_C
```

### Zombie Spawner
Class: `BP_AISpawner_Zombies_C`  
Key properties: `SpawnedActors` (array), `SpawnedActorClass`

### Vitals Hooks Probed on BP_PlayerCharacter
```
ReceiveAnyDamage
CalculatePlayerDamage
SendHealthToClient
Client_UpdateHealthUI
Client_HitEffects
```

---

## Weapon / Visual State (visual_capability_probe.lua)

```lua
pawn.CurrentFiringWeapon          -- current weapon actor
pawn.WeaponEquipped               -- bool
pawn.PrimaryWeaponEquipped        -- bool
pawn.SecondaryWeaponEquipped      -- bool
pawn.SidearmWeaponEquipped        -- bool
pawn.MeleeWeaponEquipped          -- bool
pawn:GetAttachedActors()          -- all attached actors
pawn:GetCurrentActiveWeapon()     -- function
pawn:EquipActorToSocket()         -- function
pawn:SetCurrentWeaponVisibility() -- function
```

---

## Key UE4SS API Usage

| API | Usage |
|-----|-------|
| `FindFirstOf(className)` | Get single object by class |
| `FindAllOf(className)` | Get all objects by class |
| `StaticFindObject(path)` | Check blueprint function exists before hooking |
| `RegisterHook(path, fn)` | Hook blueprint function (returns preId, postId) |
| `RegisterKeyBind(Key.F11, fn)` | Debug keybind |
| `LoopAsync(ms, fn)` | Async timer loop |
| `ExecuteInGameThread(fn)` | Run on game thread |
| `ExecuteWithDelay(ms, fn)` | Delayed game-thread execution |
| `LoadAsset(path)` | Dynamically load UAsset |
| `ForEachUObject(fn)` | Iterate all live UObjects |

---

## Protocol-v3 vs Legacy-file Mode

| Feature | legacy-file | protocol-v3 |
|---------|------------|-------------|
| Local capture | Lua writes TSV every 180ms | C++ DLL (Lua disabled) |
| Remote proxies | (C++ reads TSV) | C++ DLL native |
| Native item hooks | disabled | enabled if `native_drop_hook.enable.txt` |
| fixed_day.lua | loaded | NOT loaded (C++ handles weather) |
| Bridge start | Lua runs `Start-Bridge.ps1` | No auto-start |
| Heartbeat | every 180ms | every 180ms |

---

## Confirmed Class / Path Index

| Class | Package path |
|-------|-------------|
| Player character | `/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C` |
| Player controller | `BP_PlayerController_C` (FindFirstOf, no full path needed) |
| JigSaw inventory | `/Game/JigSInventory/Jigsaw/Components/BP_JigMultiplayer.BP_JigMultiplayer_C` |
| Loot container | `/Game/Inventory/Containers/BP_LootContainer.BP_LootContainer_C` |
| Zombie master | `/Game/AI/Zombies/BP_MasterZombie.BP_MasterZombie_C` |
| Zombie spawner | `BP_AISpawner_Zombies_C` |
| Sky / weather | `Ultra_Dynamic_Sky_C` |
