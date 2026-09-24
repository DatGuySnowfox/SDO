# Misc Systems Blueprint Catalog (FModel export analysis)

Source: `Exports/SurrounDead/Content/Blueprints/HordeSystem/**`, `Infestation/**`, `Quests/**`, `POI/**`, `Prefabs/**`, `Blueprints/Laboratory/**`, `Blueprints/Other/**`. Export is flags-only (no bytecode) — function bodies are unknown, only name/flags/params/Super exist.

## Top-line findings

- **HordeSystem** is nearly empty in this export — only a horde-defense beacon actor and a music-layering helper; the actual wave-spawning data (`DT_HordeSystem`) and `Struct_WaveInfo`/`Struct_EnemyWaveInfo` are DataTable/struct assets with no BP logic, skipped per scope. **Infestation**, **Quests**, **POI**, and **Prefabs** are heavily template-driven: most classes in each are either data dressing (0-function child actors) or duplicate the same "Jig interact/inventory" boilerplate seen across the rest of the codebase (`OnBeginInteract`, `OnServerExecuteInteract`, `JigMP_On*`, `GetInteractOptions`, etc.) rather than adding meaningfully new gameplay logic.
- **Laboratory** and **Other** are where the real net-relevant logic in this batch lives: a large family of interactive world props (doors, circuit boxes, light switches, harvestable resources, explosive barrels) all inherit the same "MasterObject"-derived interact pattern, and the doors specifically show the clearest **Svr_*/MC_*** RPC pairing found in this batch — `MC_OpenDoor` (`FUNC_Net|NetMulticast`) appears verbatim across `BP_BarrierGate`, `BP_ContainerDoors`, `BP_HangarDoor`, `BP_KeycardDoor`, `BP_KeypadDoor`, `BP_WarehouseDoors`, `BP_PoliceStationGarageDoor` — all children of `BP_MasterDoor_C` (base not in scope, but clearly the door superclass owns a `Svr_OpenDoor`→`MC_OpenDoor` pipeline, mirrored per-subclass only for the multicast half).
- **State sync elsewhere is `OnRep_*`-driven, not RPC-driven**, matching the pattern already documented for AI/vehicles: `BP_LaboratorySlidingDoor` has `OnRep_Locked`/`OnRep_Unlocked`/`OnRep_Warning`; `BP_LaboratoryLightSwitch` has `OnRep_Off?`/`OnRep_On?`/`OnRep_Warning?`; `BP_LaboratoryLight` has `OnRep_LightOn?`. These confirm replicated bool properties drive locked/powered/on-off state, with the OnRep function presumably just re-triggering the local visual (light color, door mesh). No bare `Replicated`-flagged scalar properties were visible at the shallow BP-property-array level in this export (same caveat as prior catalogs — actual UPROPERTY replication flags likely live in native C++ base classes not captured here).
- **Harvestable resources (Tree/Rocks/ScrapMetal) use the Svr_*/MC_* pattern for hit feedback**, not for the harvest state itself: `Svr_PlaySound`→`MC_PlaySound`, `Svr_SetMaxHits`, and (Tree only) `Svr_FallOverEffect`→`MC_FallOverEffect` (`NetReliable`). This is a clean, reusable reference for "networked one-shot cosmetic feedback on a hit" separate from the actual hit-count/destroy state (which is presumably a replicated int/bool, consistent with the `SetMaxHits` naming).
- No bare replicated (non-function) properties turned up anywhere in this batch's shallow export dump — all net-relevant state inference is via function names (`OnRep_*`, `Svr_*`, `MC_*`) only, same limitation noted in the AI/Vehicle catalog.

---

## HordeSystem (`Blueprints/HordeSystem/`)

### Buildable_HordeBeacon (`Beacon/Buildable_HordeBeacon.json`)
Super: `Buildable_MASTER_C` (not in scope). Placeable structure that starts/manages a horde-defense event. Components: SceneComponent, StaticMeshComponent.
21 functions: `Event_StartHorde`/`Event_StopHorde`/`Event_NextWave`, `EnemyDefeatedCheck`, `FinalWaveDefeated?`, `SpawnRewards`, `Event_PlayerDead`, `Event_SpawnBeacon`, `FindLocation`/`ProjectLocation`/`TraceForBuildables` (placement validation), `OnInteractBuildable`/`OnServerExecuteInteract` (Jig-style interact subset), `Event_DishMovement`, `Stop Music?`, timeline funcs. No net-flagged functions — wave progression presumably server-only with actor/property replication implicit.

### MusicLayeringComponent (`MusicLayeringComponent.json`)
Actor component (no Super shown). Horde-fight music layering: `Initialise`, `NextTrack`, `StopTrack`. No net flags — client-local audio.

### No own logic (appendix)
`DT_HordeSystem.json` (DataTable), `Struct_EnemyWaveInfo.json`, `Struct_WaveInfo.json` (structs — data only, no BP class).

---

## Infestation (`Infestation/`)

### BP_Infestation (`BP_Infestation.json`)
Super: `BP_MasterObject_C`. The core placeable infestation-growth node (wall/floor/ceiling/corner variants below are thin subclasses). Components: AIOSubjectComponent, NiagaraComponent, ParticleSystemComponent, PointLightComponent, PostProcessComponent, SphereComponent, StaticMeshComponent.
67 functions — largest class in this section. Groups: full **Jig interact/inventory set** (`OnBeginInteract`, `JigMP_On*`, etc. — identical shape to every other "MasterObject" prop across the codebase), **damage** (`Damage_Object`, `Damage_Shoved`, `IsObjectDamageable?`, `HitByVehicle` — same AI-shared damage vocabulary), **destruction state** (`Event_SecondaryDestroyed`, `AreSecondariesDestroyed?`, `InfestationDestroyed__DelegateSignature`), **markers** (`AddMarker`/`GetMarker`/`AIHealthBarInfo` — reuses the AI health-bar UI hook even though this isn't AI), **spawn** (`LocToSpawn`, `SpawnWave`, `TestSpawns`). No net-flagged functions — destruction state likely a replicated bool/counter.

### BP_InfestationManager (`BP_InfestationManager.json`)
Super: `BP_MasterObject_C`. Tracks a group of `BP_PrimaryInfestation` nodes: `AreAllPrimariesDestroyed`, `GetAllPrimaries`. No net flags.

### BP_InfestationSpline (`BP_InfestationSpline.json`)
Super: `BP_MasterObject_C`. Purely visual spline-mesh placement (`UserConstructionScript` only) — no gameplay logic.

### BP_RandomInfestationSpawnPoint (`BP_RandomInfestationSpawnPoint.json`)
Super: `BP_MasterObject_C`. Placement helper: `RandomSpawn`, `SpawnInfestation`, `TraceForActor`, delegate `Spawn__DelegateSignature`. No net flags.

### No own logic (appendix)
`BP_Infestation_Ceiling`, `BP_Infestation_Corner`, `BP_Infestation_Floor`, `BP_Infestation_Wall`, `BP_PrimaryInfestation` (all `super: BP_Infestation_C`, 0 functions — mesh/component config only). Meshes/, NS_InfestationDust.json — visual assets.

---

## Quests (`Quests/`)

Scope note: pure dialogue/text DataTable content skipped per task instructions; only quest-logic Blueprint classes covered below.

### BP_GuaranteedSpawner (`Blueprints/BP_GuaranteedSpawner.json`)
Super: `BP_LaptopEventMaster_C`. A quest-triggered spawn crate/container. Components: AIOSubjectComponent, ArrowComponent, StaticMeshComponent, WidgetComponent. 37 functions — Jig interact/loot set plus `HoldToInteract`/`Event_HoldToInteract`/`IsHeld?` (hold-to-open mechanic) and `DistanceOutline`/`Event_Outline` (highlight-at-range).
| Function | Flags |
|---|---|
| Svr_CrateOpen | Net, NetServer, BlueprintCallable, BlueprintEvent |
| MC_CrateOpen | Net, NetMulticast, BlueprintCallable, BlueprintEvent |

Clean Svr→MC pair for the crate-open cosmetic/state moment. `BP_GuaranteedSpawner_Classified` (super `BP_GuaranteedSpawner_C`, 0 functions) is a variant with no added logic.

### BP_QuestMarker / BP_StartQuestArea (`Blueprints/BP_QuestMarker.json`, `Blueprints/BP_StartQuestArea.json`)
Both super `BP_MasterQuestObject_C`. Marker placement (`AddMarker`) and a trigger-volume that fires `Event_Overlap` on `BeginPlay`/overlap. No net flags — quest-state changes presumably routed through the (out-of-scope) quest subsystem rather than these actors directly.

### BP_LaptopEventManager / BP_LaptopEventMaster (`Blueprints/LaptopEvents/*.json`)
Super `BP_MasterObject_C` / `BP_MasterQuestObject_C`. Random "laptop event" quest-trigger dispatch: `BeginQuest`, `CallRandomEvent`, delegates `RandomEvent__DelegateSignature`/`SelectCertainLocation__DelegateSignature`/`SpawnEvent__DelegateSignature`. No net flags — server-side event selection, presumably relies on actor spawn replication.

### BP_QuestGiver (`Blueprints/NPCs/BP_QuestGiver.json`)
Super: `BP_MasterQuestObject_C`. Quest-dispensing NPC. Components: AIOSubjectComponent, SkeletalMeshComponent, StaticMeshComponent. 20 functions, entirely the standard Jig interact set (no quest-specific functions beyond inheriting `GetQuestArgument` from `BPI_Quests`/`BP_MasterQuestObject`) — actual dialogue/quest-branch logic lives in the (out-of-scope) Narrative Pro plugin classes referenced by the UI below. `BP_QuestGiver_SettlementOfficer` (0 functions) is a config-only subclass.

### BP_MasterQuestObject (`Blueprints/Objects/BP_MasterQuestObject.json`)
Super: `BP_MasterObject_C`. Root of quest-placed actors (`BP_QuestMarker`, `BP_StartQuestArea`, `BP_QuestGiver`, `BP_LaptopEventMaster` all inherit from it). Just `GetQuestArgument` — thin shared interface implementation.

### Quest Events (`Events/*.json`, `Events/Reward/*.json`)
Nine small "quest action" classes (no Super shown — likely Narrative Pro `NarrativeEvent` base, out of scope): `Event_OnInteractNPC`, `Event_RandomLaptopQuest`, `Event_RemoveActor`, `Event_RemoveMarker`, `Event_SpawnAIHumanSpawner`, `Event_SpawnActor`, `Event_SpawnMarker`, `Event_SpawnPrefab`, `Event_RewardGiveItem`, `Event_RewardPlayerXP`. Each has exactly one function, `ExecuteEvent` — these are quest-graph "action node" plugins; the actual effect (spawn, give item, XP) happens inside that single function body (not visible without bytecode). No net flags — these fire server-side as part of quest-graph evaluation.

### Condition_CheckMoney (`Conditions/Condition_CheckMoney.json`)
Single `CheckCondition` function — a quest-graph "condition node" plugin (pure gate check, likely wraps a currency-balance query). No net flags.

### Quest UI (`UI/*.json`)
Widget blueprints for the dialogue/quest HUD, all local/client-only (no Super, no net flags): `BP_CustomDialogueOption` (reply button), `BP_CustomNarrativeDefaultUI` (40 functions — the main dialogue/quest-tracker HUD: notifications, waypoints, dialogue line/reply events, party join/leave hooks), `BP_CustomQuestBranch`, `BP_CustomQuestTask`, `BP_CustomWaypoint`, `QuestBranchUI`. These are UI event handlers reacting to an underlying (out-of-scope, likely Narrative Pro) quest-state system — not themselves state sources.

### No own logic / data-only (appendix)
`BPI_Quests` (interface, one function `GetQuestArgument` — included above as it's implemented everywhere). `AnimBP_StationaryQuestNPC` (anim graph only). Remaining ~50 files in `Quests/` are Narrative Pro dialogue/DataTable/text assets skipped per scope.

---

## POI (`POI/`)

### BP_POIManager (`BP_POIManager.json`)
Super: `BP_MasterObject_C`. Per-point-of-interest state/spawn manager (boss spawning, loot state). Components: SceneComponent, SphereComponent. 22 functions: `SpawnBosses`/`SpawnBossOnRadius`/`BossSpawnCheckDebug`/`BossDeath` (boss-fight lifecycle), `IsAnyInvokerInRadius` (same player-proximity-invoker pattern as AI spawners), `CheckLoc`/`CheckRadius`/`RandomiseSpawn`/`DistanceCheck` (placement), `Event_Looted`/`RespawnMarker`, save-system hooks (`ActorLoaded`/`ActorPreLoad`/`ActorPreSave`/`ActorSaved`/`ComponentsToSave`), delegate `EnteringPOI__DelegateSignature`. No net-flagged functions — server-authoritative spawn/loot-state, consistent with AI spawner pattern documented previously.

### BP_AlternatePOI / BP_AlternatePOI_Listener (`Alternates/*.json`)
Super `BP_MasterObject_C`. A POI-variant-swap system: `BP_AlternatePOI.SpawnPrefab` picks/spawns one of several prefab variants for a location; `BP_AlternatePOI_Listener.SetAlternate` presumably registers which variant is active. `BP_AlternatePOI_Listener_Observatory` (super `BP_AlternatePOI_Listener_C`) is a 2-function specialization with no clearly new logic beyond config. No net flags.

### No own logic (appendix)
`Alternates/Prefabs/Alternate_CoyotesObservatory`, `Alternate_ZombiesObservatory` (both `super: PrefabMaster_C`, 0 functions — pure dressing prefab variants selected by the listener above).

---

## Prefabs (`Prefabs/`)

Confirmed: this folder is almost entirely static composed-actor dressing, as expected from the task brief. All ~35 `Prefab_*`/`Alternate_*` classes share `super: PrefabMaster_C` and have **0 functions** — they're just `ChildActorComponent`/`StaticMeshComponent` compositions (camps, convoys, crashed vehicles, settlements, quarantine tents, quest-location dressing). A handful of crashed-vehicle prefabs (`Prefab_AbandonedMedicalSuppliesTruck`, `Prefab_AbandonedMilitarySuppliesTruck`, `Prefab_AbandonedRandomSuppliesTruck`, `Prefab_CrashedApache01`, `Prefab_CrashedHelicopter01`, `Prefab_CrashedHelicopter02`) have exactly 2 functions (`ExecuteUbergraph_*` + `ReceiveBeginPlay`) — construction-time particle-effect setup only, no gameplay logic.

### PrefabMaster (`PrefabMaster.json`)
Super: `BP_MasterObject_C`. The shared base for every prefab above. Components: SceneComponent, SphereComponent. 4 functions: `RemoveFoliageComp` (clears foliage under the placed prefab footprint), `UserConstructionScript`, `ReceiveBeginPlay`. No net flags — pure placement/dressing logic.

### BP_PrefabSpawner / BP_PrefabSpawner_Dynamic (`BP_PrefabSpawner.json`, `BP_PrefabSpawner_Dynamic.json`)
Super: `BP_MasterObject_C`. Random-event prefab placement logic: `CheckSurroundingArea`, `PrefabTypeChanceMulitplier` (Pure), `Random Check`, `SpawnRandomEvent`, `SpawnMarker`, `IsDynamicEvent?`/`IsEventSpawned?` (implements `BPI_RandomEvent`). No net flags. `_Dynamic` subclass adds 0 functions (config only).

### BPI_RandomEvent (`BPI_RandomEvent.json`)
Interface: `IsDynamicEvent?`, `IsEventSpawned?`, `SpawnRandomEvent`. Implemented by `BP_PrefabSpawner`.

---

## Laboratory (`Blueprints/Laboratory/`)

The Laboratory folder is a self-contained interactive-puzzle-room prop set: doors, a circuit box, a generator, light switches, cables, and a corpse prop, all sharing the base `BP_LaboratoryMaster_C` (no Super, components: AIOSubjectComponent, SceneComponent, 0 functions — pure marker base) and the same Jig interact-set boilerplate as everywhere else in the codebase. 145 files total; the vast majority beyond those listed below are meshes/materials/level-dressing with no BP class, or thin 0-function config subclasses (e.g. `BP_LaboratoryLight2`/`3` under `BP_LaboratoryLight`).

### BP_LaboratoryCircuitBox (`Blueprints/BP_LaboratoryCircuitBox.json`)
30 functions. Interact-driven circuit/lever puzzle panel: `Event_Door` (drives a connected door), `Damage_Object`/`Damage_Shoved`/`IsObjectDamageable?`, three timeline pairs (`Timeline`, `TL_DoorReverse`, `TL_Levers`). No net flags — state is presumably server-only + actor replication.

### BP_LaboratoryGenerator (`Blueprints/BP_LaboratoryGenerator.json`)
22 functions. Power source for the lab: `Event_TurnOn`, `Shake`, `TestTurnOnOff` (debug). Standard Jig interact set otherwise. No net flags.

### BP_LaboratoryEntranceDoors / BP_LaboratorySecureDoors / BP_LaboratorySlidingDoor (`Blueprints/Doors/*.json`)
All super `BP_LaboratoryMaster_C`, 22-33 functions each — door variants with `Event_Door`/`Event_AutoDoor`/`Event_Close`, multiple timeline pairs (mechanism/reverse/keycard animations), and **`BP_LaboratorySlidingDoor` uniquely exposes replicated state via `OnRep_Locked`, `OnRep_Unlocked`, `OnRep_Warning`** plus `SetPower_TurnedOn`/`SetPower_Warning` — the clearest confirmed replicated-property door in the whole batch. No `FUNC_Net*` RPCs on any lab door (unlike the `Other/Doors/` set below, which does use `MC_OpenDoor`). Real offsets confirmed via `CXXHeaderDump/BP_LaboratorySlidingDoor.hpp` (`ABP_LaboratorySlidingDoor_C : public ABP_LaboratoryMaster_C`, size 0x2FD): `Unlocked (0x02F8, size 0x1)`, `Locked (0x02F9, size 0x1)`, `Warning (0x02FA, size 0x1)`, `IsClosed (0x02E8, size 0x1)`, `Keycard (0x02F0, size 0x8)`, `XPGiven? (0x02FB, size 0x1)`, `CanOpenDoorFromBehind? (0x02FC, size 0x1)`, `Door (0x02C0, size 0x8)`, `Frame (0x02B8, size 0x8)`.

### BP_LaboratoryLight / BP_LaboratoryLightSwitch / BP_LaboratoryStairwellLight(Switch) (`Blueprints/Lights/**`)
`BP_LaboratoryLight` has `OnRep_LightOn?` (4 functions — confirms replicated bool; real offsets via `CXXHeaderDump/BP_LaboratoryLight.hpp`, size 0x2C6: `LightOn? (0x02C4, size 0x1)`, `EnableShadows? (0x02C5, size 0x1)`, `Intensity (0x02C0, size 0x4)`, `PointLight (0x02B0, size 0x8)`). `BP_LaboratoryLightSwitch` (26 functions) adds `OnRep_Off?`/`OnRep_On?`/`OnRep_Warning?` plus `SetPower_TurnedOn`/`SetPower_Warning` and the full Jig interact set — this is the canonical "replicated powered-device" pattern in this batch (real offsets via `CXXHeaderDump/BP_LaboratoryLightSwitch.hpp`, size 0x2DB: `On? (0x02D8, size 0x1)`, `Off? (0x02D9, size 0x1)`, `Warning? (0x02DA, size 0x1)`, `Lights (0x02B8, size 0x10)`, `Switches (0x02C8, size 0x10)`). `BP_LaboratoryStairwellLight`/`...Switch` are parallel variants with the same shape (Switch: 19 functions, same Jig set, no OnRep — likely inherits state differently or wasn't finished).

### No own logic (appendix)
`BP_LaboratoryCable`, `BP_LaboratoryCorpse` (both 3 functions — construction-script mesh/spline placement only, no interact/damage logic). `BP_LaboratoryLight2`, `BP_LaboratoryLight3` (0 functions, mesh/color config subclasses of `BP_LaboratoryLight`).

---

## Other (`Blueprints/Other/`)

The largest and most varied folder — general-purpose interactive world objects and environment props not filed elsewhere. 84 files; most non-trivial classes follow the same "MasterObject + Jig interact set" template already documented repeatedly above, so only the functionally distinct groups are detailed.

### Doors (`Doors/*.json`) — the clearest Svr/MC RPC family in this batch
All super `BP_MasterDoor_C` (base not in scope but clearly owns the `Svr_OpenDoor`-style entry point): `BP_BarrierGate`, `BP_ContainerDoors`, `BP_HangarDoor`, `BP_KeycardDoor`, `BP_WarehouseDoors`, `BP_PoliceStationGarageDoor`, `BP_KeypadDoor` (17-33 functions each). Every one implements:
| Function | Flags |
|---|---|
| MC_OpenDoor | Net, NetMulticast, BlueprintCallable, BlueprintEvent |

plus `Event_Door`/`Event_OpenDoor`/`Event_CloseDoor`/`Event_OpenDoorBehind`, multiple timeline pairs (open/close/auto animations), and the standard Jig interact set. `BP_KeypadDoor` additionally pairs with `BP_KeypadNote` (20 functions, a readable in-world note giving the code) and has the largest function count (33) — likely extra code-entry-attempt logic. `BP_LockedDoor` (super `BP_LockPickObject_C`, not in scope, 29 functions) adds `AdvancedLockCheck`/`LockpickSuccessful` (lockpicking minigame hooks) but no `MC_*`/`Svr_*` RPCs of its own — lockpick resolution presumably happens in the `BP_LockPickObject_C` base. `BP_SubwayTravelDoor` (super `BP_MasterObject_C`, 25 functions) adds `Event_Teleport` — a fast-travel door, not a physical-open door, hence no `MC_OpenDoor`. `BP_KeycardDoor_Child` is a 0-function mesh-swap subclass.

### BP_CookingStation / BP_CraftingStation (`BP_CookingStation.json`, `BP_CraftingStation.json`)
Both super `BP_MasterObject_C`, 36 functions each — full Jig interact + `JigMP_*` inventory set (these are storage-container-style crafting stations), plus `CheckDistanceFromActor`/`SetInteractDistance`. No net-flagged functions of their own — inventory sync handled by the shared Jig multiplayer component (out of scope, referenced everywhere).

### Harvestable Objects (`HarvestableObjects/*.json`)
`BPI_HarvestableObject` interface: `HitHarvestableObject`. `BP_HarvestableObject` (super `BP_MasterObject_C`, base, 3 functions) implements it. Subclasses add the **Svr/MC hit-feedback pattern**:
| Class | Net functions |
|---|---|
| BP_HarvestableObject_Rocks | Svr_PlaySound/MC_PlaySound (NetServer/NetMulticast), Svr_SetMaxHits (NetServer) |
| BP_HarvestableObject_ScrapMetal | Svr_PlaySound/MC_PlaySound |
| BP_HarvestableObject_Tree | Svr_PlaySound/MC_PlaySound, Svr_SetMaxHits, **Svr_FallOverEffect/MC_FallOverEffect (NetReliable)** |

`SetMaxHits`/`Svr_SetMaxHits` implies a server-authoritative hit-counter (likely a replicated int) gating when the resource depletes; the reliable fall-over multicast is the one-shot "tree finally falls" cosmetic moment.

### Explosive/Hazard objects
`BP_ExplosiveBarrelMaster` (super `BP_MasterObject_C`, 8 functions: `Event_Explode`, `Damage_Object`/`Damage_Shoved`/`IsObjectDamageable?` — no net flags, explosion presumably server-triggered via actor destroy + replicated FX actor spawn). `BP_ExplosiveBarrel_Child` (0 functions, mesh variant). `BP_Landmine` (super `BP_MasterObject_C`, 9 functions — trigger/explode logic, no net flags visible). `BP_BarbedWire` (super `BP_MasterObject_C`, 5 functions: `Event_Damage` on overlap — no net flags); `BP_BarbedWire_Child`/`_Child1` are 0-function mesh variants.

### Lighting (`Lights/*.json`)
`BP_MasterLight` (no Super, 0 functions — base marker). Subclasses (`BP_Lamp` 20 fn, `BP_Siren` 25 fn — actually under `Other/` root not `Lights/`, see below — `BP_FloodLights`, `BP_ResortLight`, `BP_StreetLight`, `BP_Lights` all ~9 functions, `BP_FlickeringLight`/`BP_FlickeringLightSparks` ~5-6 functions, `BP_RoadLightGenerator` 2 functions, `BP_Campfire`/`BP_BarrelFire` 2 functions each) are all power-toggle/flicker cosmetic props — none show net flags or `OnRep_*` in this export (unlike the Laboratory lights, which do), suggesting these may rely on a shared native lighting-power component not captured here. `BP_Firepit` (super `BP_Campfire_C`) and `BP_FlickeringLightSparks_MainMenu` are 0-function config variants.

### Player/Damage type bases (no own logic, appendix)
`BP_MasterPlayer`, `BP_MasterPlayerController` (both no Super, 0 functions — pure C++-backed marker BPs, real logic is native and out of scope). `SD_DamageType`, `Damage_Body`, `Damage_Headshot` (0 functions — `UDamageType` config assets, not gameplay logic).

### Root-level "Other" props (`Other/*.json` directly)
Notable non-trivial ones: `BP_Barber` (22 fn, appearance-editing station), `BP_ShootingRangeTarget` (20 fn, target hit/reset), `BP_Siren` (25 fn, alarm/alert prop), `BP_SleepingBag` (23 fn, respawn-point/bed), `BP_StandardNote` (20 fn, readable note), `BP_WaterWell` (28 fn, drinkable water source), `BP_Lever` (24 fn, mechanism trigger — likely drives lab/other doors via `Event_Door`-style calls), `BP_PowerGenerator` (22 fn), `BP_Radio`/`BP_Radio_Text` (20-21 fn, plays audio/shows text — `BP_Radio_Text_Military` is a 0-function content variant), `BP_SleepingBagTutorial` (19 fn) + `TutorialArrow` (9 fn, onboarding UI pointer). All follow the same Jig-interact-set template as the rest of the codebase; none of these showed `FUNC_Net*` or `OnRep_*` in this export — likely relying on generic replicated-actor-state or the shared MasterObject base for any needed sync. Smaller no-logic or trivial props: `BP_FishingBuoy`/`BP_Generator` (2 fn each), `BP_RadioTower`/`BP_RadioTower2`/`BP_ZombieDoor`/`BP_SmokingCigaretteAttachment` (0 fn), `BP_Keycard_Spawner` (2 fn), `BP_Ladder` (6 fn), `BP_SpecialItemContainerSpawner` (3 fn), `BP_ActorDestroyer` (3 fn, timer-based self-destruct), `BP_EasterEgg_Campfire`/`_GlowingEyes`/`_Nuke` (5-9 fn, cosmetic Easter eggs), `BP_SkySettings` (6 fn, day/night config), `RadioTextUI` (3 fn, widget for `BP_Radio_Text`).
