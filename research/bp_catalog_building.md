# BP Catalog: BuildingSystem (placement/base-building)

Static catalog from FModel JSON export. No bytecode — function name + `FunctionFlags`/`PropertyFlags`
only. Scope: `Content/Blueprints/BuildingSystem/**` (191 files, 133 parsed as `BlueprintGeneratedClass`;
the rest are Materials/Textures/an Enum/Widgets with no class of their own). `[RPC]` = function flags
contain `FUNC_NetServer`/`FUNC_NetMulticast`/`FUNC_NetClient`.

## Notable findings

Almost the entire class tree funnels through one base, `Buildable_MASTER_C` (Actor, 66 functions,
`NumReplicatedProperties: 2`) — every buildable in the game (walls, foundations, doors, lights,
traps, turrets, containers, tents, generators, furniture, ornaments) subclasses it either directly or
via a handful of category masters (`Buildable_ModularMaster_C`, `BuildableMaster_Container_C`,
`BuildableMaster_Tent_C`, `BuildableMaster_Display_C`, `Buildable_GeneratorMaster_C`,
`Buildable_TurretMASTER_C`). This mirrors the `BP_JigMultiplayer_C`-hub pattern documented for the
inventory system in `bp_catalog_inventory_jigsaw.md`, and indeed `Buildable_MASTER_C` embeds a
`BP_JigMultiplayer_C` component and re-exposes its `JigMP_On*` interface events directly, so every
buildable is also a Jig multiplayer item/container. **Health/durability is present but NOT
replicated**: `Buildable_MASTER_C` has a `Health` `DoubleProperty` (`Edit | BlueprintVisible |
DisableEditOnInstance | SaveGame` — no `Net` flag) alongside `UpdateUIHealth`/`ReceiveAnyDamage`
functions with no RPC of their own — same "no property-replication fallback, logic rides on
event/RPC calls" pattern flagged for player clothing appearance in `bp_catalog_player_core.md`. Only
3 real replicated properties exist in the whole scope (`PoweredOn?` w/ `RepNotify` on the master,
`CurrentFuel`/`TurnedOn?` on the generator, `CooldownRunning?` on the decontamination shower) — none
are health/durability/ownership/snap-state. **No repair, upgrade, ownership/permission, or lock
function names appear anywhere in this scope** — searched for `owner|permission|health|durab|snap|
upgrade|repair|lock|auth` across all 133 classes' function names and found only `UpdateUIHealth`,
`UpdateHealth` (turret), and `GetSnappedObjects` (a query-only getter on `Buildable_ModularMaster_C`,
no snap-write logic exported). This means repair/upgrade/durability/ownership for `PlacedStructure`
extension work is either handled entirely in native C++ (outside this export) or doesn't exist yet in
the base game — nothing to reuse at the Blueprint layer for those features. Snapping itself is a
single read-only getter; the actual snap-placement math is not in these Blueprints (likely native or
in the `BP_NoBuildArea_C`/`Enum_BuildingSnapTypes` support assets, or purely client-side construction
preview code not exported here).

**RPC set (only 4 classes in the whole 191-file scope define their own RPCs — everything else
inherits `Buildable_MASTER_C`'s):**

| Class | Function | FunctionFlags |
|---|---|---|
| `Buildable_MASTER_C` | `Svr_Spawn` | `FUNC_Net \| FUNC_NetReliable \| FUNC_NetServer \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `Buildable_MASTER_C` | `MC_Destroy` | `FUNC_Net \| FUNC_NetMulticast \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `Buildable_MASTER_C` | `MC_OnInteractBuildable` | `FUNC_Net \| FUNC_NetMulticast \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `Buildable_GeneratorMaster_C` | `Client_Notification` | `FUNC_Net \| FUNC_NetClient \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |
| `Buildable_GeneratorMaster_C` | `Svr_UpdateFuel` | `FUNC_Net \| FUNC_NetMulticast` (note: named `Svr_` but flagged Multicast, not Server — naming/flag mismatch like the `Svr_DestroyComponent`/`NetClient` one flagged in the player-core catalog) |
| `Buildable_Turret_C`, `Buildable_SmallTurret_C` | `Multicast Spawn Emitter`, `Multicast Turret Idle Start`, `Multicast Turret Idle Stop` | all `FUNC_Net \| FUNC_NetReliable \| FUNC_NetMulticast \| FUNC_BlueprintCallable \| FUNC_BlueprintEvent` |

`Svr_Spawn` is the closest existing analog to the mod's own `Svr_SpawnBuild` — worth diffing param
signatures (not available in this export; would need live/IDA lookup) against the mod's hook.

---

## Base/master classes (own real logic)

### `Buildable_MASTER_C`
`Actors/Buildable_MASTER.json` — root of the entire building system. Super: `Actor` (native). 66
functions, `NumReplicatedProperties: 2`. Embeds `BP_JigMultiplayer_C` (same MP-item-sync hub as
inventory/pickups) plus `BoxComponent`, `AIOSubjectComponent`, `ChildActorComponent`.

**RPCs:** see table above (`Svr_Spawn`, `MC_Destroy`, `MC_OnInteractBuildable`).

**Interact/lifecycle (BPI_MpInteractInterface-style contract, not RPCs themselves):**
`OnBeginInteract`, `OnEndInteract`, `OnExecuteInteract`, `OnExecuteInteractDialogue`,
`OnExecuteInteractEnded`, `OnRequestServerInteract`, `OnServerExecuteInteract`,
`OnStopExecuteInteract`, `JigCanInteract`, `JigSetCanInteract`, `GetInteractOptions`,
`SetInteractOption`, `SetInteractDistance`, `CheckDistanceFromActor`.

**Save/load lifecycle:** `ActorLoaded`, `ActorPreLoad`, `ActorPreSave`, `ActorSaved`,
`ComponentsToSave`.

**Power:** `Power On`, `Power Off`, `Requires Power?`, `OnRep_PoweredOn?`, delegates
`PoweredOn__DelegateSignature`/`PoweredOff__DelegateSignature`. Backed by the only `RepNotify`
bool in this class, `PoweredOn?`.

**Health/damage:** `ReceiveAnyDamage` (`FUNC_BlueprintAuthorityOnly` — server-only, native damage
event), `UpdateUIHealth`. Backed by unreplicated `Health` (`DoubleProperty`, `SaveGame` only, no
`Net`).

**Pickup/destroy/misc:** `Pickup`, `PickupBuildFromGround`, `SetPickupCount`, `OnDestroy`,
`SpawnItemOnDestroy`, `SpawnCraftingMaterials`, `RemoveFoliageComp`, `RemoveModularBuilds`,
`ReplacingObject`, `OverrideBuildName`, `GetBuildType`, `GetMainSceneComp`,
`GetJigMultiplayerComponent`, `GetItemInfo`, `GetAllItemsInContainer`, timer helpers
(`Event_HoldTimer`/`Event_StopTimer`/`Event_UpdateTimer`).

**Jig multiplayer container events (inherited contract, re-exposed):** `JigMP_OnItemAdded/Removed/
Moved/Consumed/Dropped`, `JigMP_OnMainContainerItemAdded/Removed`, `JigMP_OnContainersSwap`,
`JigMP_OnTwoContainersSwap`, `JigMP_OnPickupAdded/Equipped`, `JigMP_OnRequestDropItem`,
`JigMP_OnUpdateChamberUID`, `JigMP_GetLootWidget`.

**Replicated properties:**
| Property | Type | Flags |
|---|---|---|
| `PoweredOn?` | BoolProperty | `Edit \| BlueprintVisible \| Net \| DisableEditOnInstance \| RepNotify` |
| `InteractWithObject` | MulticastInlineDelegateProperty | `Edit \| BlueprintVisible \| Net \| DisableEditOnInstance \| BlueprintAssignable \| BlueprintCallable` |
| `Health` | DoubleProperty | **not replicated** — `Edit \| BlueprintVisible \| DisableEditOnInstance \| SaveGame` |

**REAL OFFSETS CONFIRMED** (`research/CXXHeaderDump/Buildable_MASTER.hpp`, matched `ABuildable_MASTER_C : AActor`, size 0x430):
- **`Health` (0x0380, size 0x8, double)** — confirms the unreplicated health field's exact native offset; a future local-write proxy-sync approach could read/write this directly once you have the actor pointer.
- `RequiresPower?` (0x0388, size 0x1)
- `PoweredOn?` (0x03D0, size 0x1) — the replicated bool itself; `PoweredOn` delegate struct sits at 0x03D8 (size 0x10)
- `CurrentActor` (0x0390, size 0x8), `InteractingActorLoc` (0x0398, size 0x18), `IsHeld` (0x03B0, size 0x1), `OffSetDespawn` (0x03B8, size 0x8), `CraftingMaterials` (0x03C0, size 0x10), `ShowGenerator` (0x03F8, size 0x1), `SpecialDestroy?` (0x0410, size 0x1), `PickupTimer` (0x0428, size 0x8)
- `NavObstacleBox` (0x02A0, size 0x8), `DummySnapTester` (0x02A8, size 0x8), `SnapPoints` (0x02B0, size 0x8), `AIOSubject` (0x02B8, size 0x8), `StaticMesh` (0x02C0, size 0x8), `Scene` (0x02C8, size 0x8), `BP_JigMultiplayer` (0x02D0, size 0x8), `PickupDataAsset` (0x02D8, size 0x8)
- `InteractOptions` (0x02E0, size 0x50), `StatOptions` (0x0330, size 0x50)

### `Buildable_ModularMaster_C`
`Actors/Modular/Buildable_ModularMaster.json` — super of all wood modular pieces (foundations,
walls, floors, roofs, stairs). Super: `Buildable_MASTER_C`. Only 3 own functions: `GetSnappedObjects`
(query-only getter, `HasOutParms`), `RemoveModularBuilds` (override), `ExecuteUbergraph...`. No
snap-write/placement-validation logic exported — snapping decisions happen elsewhere (native or
non-exported client construction-preview code). No replicated properties of its own.

**Real offsets** (`CXXHeaderDump/Buildable_ModularMaster.hpp`, matched `ABuildable_ModularMaster_C : ABuildable_MASTER_C`, size 0x438): adds **no own properties** beyond the inherited `UberGraphFrame` (0x0430, size 0x8) — confirms this class is pure logic-override with zero new state, matching the catalog's "3 own functions" finding.

### `Buildable_TurretMASTER_C`
`Actors/Turret/Buildable_TurretMASTER.json` — super of both turret classes. Super:
`Buildable_MASTER_C`. 19 functions: AI-combat behavior tree hooks (`Actor Aim Focus`, `Actor Attack
Target`, `AI Alert`/`AI End Alert`, `AI Animation Switch`, `AI Block`, `AI Camera Activate`, `AI Can
Interact?`, `AI Is Dead?`, `AI Dead`), reload/idle state (`Turret Start/End Reload`, `Turret Idle
Start/Stop`), `UpdateHealth`, `Turret Destroyed Effect`, `Alert Actor Defenders`,
`ReceiveAnyDamage` (override). No RPCs of its own; no replicated properties. `Buildable_Turret_C`
and `Buildable_SmallTurret_C` (both direct subclasses, 16/13 functions) add the 3 `Multicast Turret
Idle Start/Stop`/`Multicast Spawn Emitter` RPCs plus overlap-sphere bound events and a construction
timeline — near-identical implementations, `SmallTurret` looks like a lighter reskin.

**Real offsets** (`CXXHeaderDump/Buildable_TurretMASTER.hpp`, matched `ABuildable_TurretMASTER_C : ABuildable_MASTER_C`, size 0x480): `Widget` (0x0438, size 0x8), `BP_BuildableTurretAIComponent` (0x0440, size 0x8), `Weapon Muzzle` (0x0448, size 0x8), `Forward Direction` (0x0450, size 0x8), `FOV Arrow Right/Up/Down/Left` (0x0458/0x0460/0x0468/0x0470, size 0x8 each), `Turret` (0x0478, size 0x8). Note `UpdateHealth(int32 Health)` takes an int32 parameter but there is no matching `Health` scalar property on this class — health is still the inherited `Health` (double, 0x0380) from `Buildable_MASTER_C` above; `UpdateHealth` is presumably a UI-only pass-through.

### `BuildableMaster_Container_C`
`Actors/Containers/BuildableMaster_Container.json` — super of all storage containers (crates,
lockers, safes, cupboards, etc.). Super: `Buildable_MASTER_C`. 16 functions: `DropAll`,
`PickupContainer`, `UpdateStorageStat`, `GetName`, interact overrides, `JigMP_OnMainContainerItem
Added/Removed`. No own RPCs, no replicated properties — all 15 leaf container blueprints
(`Buildable_StorageCrate_C` etc.) are empty 0-function subclasses.

**Real offsets** (`CXXHeaderDump/BuildableMaster_Container.hpp`, matched `ABuildableMaster_Container_C : ABuildable_MASTER_C`, size 0x488): `DropItemLocation` (0x0438, size 0x8), `MainJigContainers` (0x0440, size 0x10), `MainUIDs` (0x0450, size 0x10), `Weights` (0x0460, size 0x10), `ContainerName` (0x0470, size 0x18).

### `Buildable_GeneratorMaster_C`
`Actors/Generator/Buildable_GeneratorMaster.json` — super of all 3 generators. Super:
`Buildable_MASTER_C`. 15 functions incl. the `Client_Notification`/`Svr_UpdateFuel` RPCs (see table),
`FuelCost`, `Shake`, `GetObjects`/`GetObjects_ForceOff`, `OnRep_CurrentFuel`, `OnRep_TurnedOn?`.
**Only class in scope with its own replicated data properties**: `CurrentFuel` (DoubleProperty,
`Net | SaveGame`) and `TurnedOn?` (BoolProperty, `Net | SaveGame | RepNotify`). All 3 leaf generator
blueprints are empty 0-function subclasses (config-only).

**Real offsets** (`CXXHeaderDump/Buildable_GeneratorMaster.hpp`, matched `ABuildable_GeneratorMaster_C : ABuildable_MASTER_C`, size 0x498): `Audio` (0x0438, size 0x8), `Sphere` (0x0440, size 0x8), `Collision` (0x0448, size 0x8), **`CurrentFuel` (0x0450, size 0x8, double)**, `MaxFuel` (0x0458, size 0x8, double), **`TurnedOn?` (0x0460, size 0x1)**, `ShakeTimer` (0x0468, size 0x8), `FuelTimer` (0x0470, size 0x8), `FindObjectsTimer` (0x0478, size 0x8), `Player` (0x0480, size 0x8), `PlayerClicked?` (0x0488, size 0x1), `InteractWidget` (0x0490, size 0x8). These are the exact byte offsets for the only replicated fuel-state fields found anywhere in the building scope.

### `BuildableMaster_Tent_C` / `BuildableMaster_Display_C`
`Actors/Tents/BuildableMaster_Tent.json` (13 fns), `Actors/Display/BuildableMaster_Display.json`
(14 fns) — same shape as the container master: `DropAll`/`PickupContainer`/`UpdateStorageStat` on
the tent side, `DisplayItem`/`InstallAttachments`/`RemoveIronsights`/`GetActiveComponent` on the
display (weapon-rack) side. No own RPCs, no replicated properties. Leaf blueprints (4 tents, 1
plaque) are empty 0-function subclasses.

**Real offsets:**
- `BuildableMaster_Tent_C` (`CXXHeaderDump/BuildableMaster_Tent.hpp`, `: ABuildable_MASTER_C`, size 0x498): `TentInventory` (0x0438, size 0x30), `MainJigContainers` (0x0468, size 0x10), `MainContainersIDs` (0x0478, size 0x10), `Inventory Weight` (0x0488, size 0x10).
- `BuildableMaster_Display_C` (`CXXHeaderDump/BuildableMaster_Display.hpp`, `: ABuildable_MASTER_C`, size 0x4A1): `MainJigContainers` (0x0438, size 0x10), `MainUIDs` (0x0448, size 0x10), `Weights` (0x0458, size 0x10), `ContainerName` (0x0468, size 0x18), `SKMComp` (0x0480, size 0x8), `SMComp` (0x0488, size 0x8), `CurrentAttachments` (0x0490, size 0x10), `Displayed` (0x04A0, size 0x1).

### `Buildable_BatteryCharger_C`
`Actors/Benches/Buildable_BatteryCharger.json` — 14 functions, direct `Buildable_MASTER_C` subclass
(not through a shared "bench" master — `Buildable_CraftingWorkbench_C`/`ConstructionBench_C`/
`WeaponsEquipmentWorkbench_C`/`DisassemblyWorkbench_C` are separate direct subclasses, 5-7 functions
each, no shared bench base). Power on/off timer events, `JigMP_OnMainContainerItemAdded/Removed`,
`UpdateStorageStat`. No own RPCs/replicated properties.

**Real offsets** (`CXXHeaderDump/Buildable_BatteryCharger.hpp`, matched `ABuildable_BatteryCharger_C : ABuildable_MASTER_C`, size 0x478): `BatteryUIDs` (0x0438, size 0x10), `MainJigContainers` (0x0448, size 0x10), `MainUIDs` (0x0458, size 0x10), `Weights` (0x0468, size 0x10).

---

## Mid-size classes with their own light logic (direct `Buildable_MASTER_C` subclasses, no RPCs, no replicated properties)

| Class | File | Fns | What it does |
|---|---|---|---|
| `Buildable_GarageDoor_C` | `Actors/Doors/Buildable_GarageDoor.json` | 17 | Timeline-driven `On`/`Off` open/close, anim notifies, `OnServerExecuteInteract` override. |
| `Buildable_DecontaminationShower_C` | `Actors/Other/Buildable_DecontaminationShower.json` | 12 | `DecontaminatePlayer`, cooldown timer, `OnRep_CooldownRunning?` (backed by the class's own `CooldownRunning?` replicated bool). |
| `Buildable_WaterWell_C` | `Actors/Other/Buildable_WaterWell.json` | 12 | `DrinkWater`, `WaterWellTrace`/`SurfaceCheck` (placement/surface validation trace — the closest thing to "placement validation logic" found in this scope), timer-driven water regen. |
| `Buildable_Jukebox_C` / `Buildable_Radio_C` | `Actors/Furniture/*.json` | 11 / 9 | Power on/off, `OnDestroy`/`ReceiveDestroyed` cleanup. Near-identical pair. |
| `Buildable_WoodenGate_C` | `Actors/Doors/Buildable_WoodenGate.json` | 10 | Two timelines for gate swing open/close, `Event_OpenDoor`. |
| `Buildable_CampingLamp_C` | `Actors/Lights/Buildable_CampingLamp.json` | 9 | `LightCallback`, `GetPickupName` (pure), on/off. |
| `Buildable_BarricadedMetalDoor_C`, `Buildable_BunkerDoor_C`, `Buildable_WoodenDoor_C`, `Buildable_WoodenDoor2_C` | `Actors/Doors/*.json` | 8 each | Same door-open/close pattern as `GarageDoor`/`WoodenGate` but simpler (no timeline pair, likely single anim montage). |
| `Buildable_SleepingBag_C`, `Buildable_ConstructionLight_C`, `Buildable_Lamp_C`, `Buildable_LandMine_C` | various | 8 each | Standard interact + on/off/trigger overrides, nothing durability/ownership-specific. |
| `Buildable_LargeConcreteWall1/2_C`, `Buildable_MetalFence_C`, `Buildable_SmallConcreteWall_C` | `Actors/Walls/*.json` | 8 each | Same as other 8-fn leaf classes; no unique wall-specific health/snap logic beyond the inherited master. |
| `Buildable_DisassemblyWorkbench_C`, `Buildable_Bed_C`, `Buildable_CeilingLight_C`, `Buildable_CeilingLight2_C`, `Buildable_WallLight_C`, `Buildable_BearTrap_C` | various | 7 each | Same pattern. |
| `Buildable_Campfire_C`, `Buildable_CraftingWorkbench_C`, `Buildable_TrashBin_C` | `Actors/Benches/*.json` | 6 each | Same pattern (crafting-station interact overrides). |
| `Buildable_ConstructionBench_C`, `Buildable_WeaponsEquipmentWorkbench_C`, `Buildable_BaseMarker_C` | various | 5 each | Same pattern; `BaseMarker` is likely the base-claim/spawn-point marker but exposes no ownership-specific function — worth a native/IDA look if base-ownership matters for the mod. |
| `Buildable_Spikes_C` | `Actors/Traps/Buildable_Spikes.json` | 4 | Trap trigger, minimal. |
| `Buildable_WatchTower_C`, `Buildable_LogWire_C`, `Buildable_RazorWire_C`, `Buildable_SpikedWall_C`, `Buildable_WoodenFence_C`, `Buildable_WoodenFence2_C` | various | 3 each | Minimal interact overrides only. |
| ~20 furniture/ornament decoration classes (`Buildable_Chair*_C`, `Buildable_Table*_C`, `Buildable_Couch_C`, mounts, ornaments) | `Actors/Furniture/`, `Actors/Ornaments/` | 2 each | Trivial — just `OnInteractBuildable` + ubergraph. Purpose is decorative, sit/display only. |

Every class in this section: **no own RPCs, no own replicated properties** (all networking/health
inherited unchanged from `Buildable_MASTER_C`).

---

## Interfaces / support assets

- `BPI_Buildables_C` (`BPI_Buildables.json`) — the buildable interface contract: `GetBuildType`,
  `OnDestroy`, `OnInteractBuildable`, `OverrideBuildName`, `Power On`/`Power Off`,
  `RemoveModularBuilds`, `Requires Power?`. Matches `Buildable_MASTER_C`'s own function set 1:1,
  confirming `Buildable_MASTER_C` implements this interface.
- `BP_NoBuildArea_C` (`BP_NoBuildArea.json`) — 2 functions, just `ReceiveBeginPlay` +
  ubergraph. Presumably a volume actor whose real no-build-zone logic is in its (non-exported)
  component/collision setup or native code — nothing here to read.
- `Enum_BuildingSnapTypes.json` — enum only, not parsed for functions (would need direct read if the
  member list matters for snap-type work).

---

## No own logic (empty 0-function `BlueprintGeneratedClass` subclasses — config/mesh/material only)

All ~53 remaining classes in scope are thin leaf subclasses with **zero own `Function` entries**,
inheriting everything from the category master listed:

- **Containers** (`BlueprintGeneratedClass'BuildableMaster_Container_C'`): `Buildable_AmmoCrate_C`,
  `Buildable_ClothingCupboard_C`, `Buildable_Dresser_C`, `Buildable_FridgeContainer_C`,
  `Buildable_GunLocker_C`, `Buildable_MilitaryCrate_C`, `Buildable_MilitarySupplyCrates_C`,
  `Buildable_MoneySafe_C`, `Buildable_OldChest_C`, `Buildable_ShippingContainer_C`,
  `Buildable_StorageCrate_C/2/3/4`, `Buildable_ToolsCabinet_C`.
- **Display**: `Buildable_Display_Plaque_C`.
- **Generators** (`Buildable_GeneratorMaster_C`): `Buildable_IndustrialGenerator_C`,
  `Buildable_LargeGenerator_C`, `Buildable_SmallGenerator_C`.
- **Tents** (`BuildableMaster_Tent_C`): `Buildable_CampsiteTent_C`, `Buildable_Tent1/2/3/4_C`.
- **Modular wood** (`Buildable_ModularMaster_C` or a wood-wall variant thereof): all of
  `Actors/Modular/Wood/{Floor,Foundation,Roof,Stairs,Wall}/*.json` — `Buildable_WoodenFloor_C(+Half/
  Quarter/Triangle)`, `Buildable_WoodenFoundation_C(+Half/Quarter/Triangle)`,
  `Buildable_WoodenRoof45_C`, `Buildable_WoodenRoofCorner_C`, `Buildable_WoodenRoofTop_C`,
  `Buildable_WoodenStairsCorner_C`, `Buildable_WoodenStairsHalf_C`, and the full wall family
  (`Buildable_WoodenWall_C` and its `2`/`Door`/`Door2`/`Gable`/`GableTop`/`Half`/`Half2`/`HalfMid`/
  `HalfMid2`/`HalfShort`/`HalfShort2`/`Mid`/`Mid2`/`Short`/`Short2`/`Window`/`Window2` variants —
  each numbered `2` variant subclasses the base variant, e.g. `Buildable_WoodenWallMid2_C` : 
  `Buildable_WoodenWallMid_C`, itself 0-function).
- **Furniture/ornament**: `FishingRodPickupComponent_C`-style decoration pieces already listed with
  2 functions above are the *non-empty* ones; no additional 0-function furniture found beyond the
  modular/container/generator/tent lists.
- The non-Blueprint `Modular/Wood/**` mesh-only base assets (e.g. `Base_wood_stairs_corner.json`,
  `Base_wood_foundationTriangle.json`) and all `Materials/`, `Materials/Textures/`, `Widgets/`
  files were skipped entirely — pure visual/UI assets, not gameplay Blueprint classes.

---

## Notes / limitations

- No bytecode in this export — all "what it does" descriptions are inferred from function/property
  names and flags, not verified logic. Say-so on `WaterWellTrace`/`SurfaceCheck` being "placement
  validation" is a naming inference, not confirmed.
- `Health`'s lack of a `Net` flag strongly suggests damage/health is either server-authoritative with
  clients never seeing live values (unlikely, since `UpdateUIHealth` implies client display) or is
  synced via some other channel (an RPC, or a property on a component not captured here, or native
  replication not visible in this BP-only export) — worth a live/IDA check before assuming health is
  simply unsynced.
- Struct/enum internals (`Enum_BuildingSnapTypes`, any snap-related structs referenced by
  `GetSnappedObjects`) were not drilled into — flag for follow-up if exact snap-type values matter.
- This pass did not open `Widgets/**` (13 files, all UMG UI, out of scope per task) or
  `Materials/**`.
