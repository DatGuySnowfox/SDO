# AI + Vehicle Blueprint Catalog (FModel export analysis)

Source: `Exports/SurrounDead/Content/AI/**` and `Exports/SurrounDead/Content/Blueprints/Vehicles/**`.
Export is flags-only (no bytecode) — function bodies are unknown, only name/flags/params exist.

## Top-line findings

- **Damage/health is component-driven, not RPC-driven.** Nearly every AI character (zombies, animals, bandits, military, scavengers, traders) owns a `DamageComponent_C` (defined at `Blueprints/Components/DamageComponent.json`, outside the declared scope but directly referenced by name everywhere here). It exposes `SetHealth`, `DamageTaken`, `FireDamageTaken`, `IsAlive?`, `GetValues`, and delegates `Death__DelegateSignature` / `StandardDamage__DelegateSignature` / `SetCurrent&MaxHealth__DelegateSignature` — none of these are marked `FUNC_Net*`, so health sync is almost certainly via **replicated properties on the component** (health value replicates, client reacts via `OnRep`), not multicast RPCs. `BP_MasterZombie`/`BP_ZombieBoss` both have an explicit `OnRep_IsDead?` function, confirming a replicated bool drives the death state — a clean pattern to mirror for proxy zombie death.
- **Vehicles use the identical split-component pattern**: `BP_VehicleMaster_C` owns `VehicleFuelComponent_C` (`AddFuel`, `FuelSpend`, `FuelUpdate__DelegateSignature`) and `VehicleHealthComponent_C` (`AddHealth`, `UpdateUIHealth`, `FuelUpdate__DelegateSignature` — likely reused/misnamed delegate). `VehicleUI_C` widget binds directly to `FuelBar`/`HealthBar` progress bars, confirming these are simple 0–1 normalized values. `BP_VehicleMaster` itself has two real server RPCs: `Svr_UpdateEngine` and `Svr_UpdateFuel` (`FUNC_Net | FUNC_NetServer`, not reliable) — these are the actual authoritative-input entry points, useful for a future vehicle input/state sync feature.
- **Turret/camera "AI" (BP_BanditTurret_AI, BP_SurvivorTurret_AI, BP_SecurityCamera_AI)** are not full characters — they use `BP_MasterTurret_C`/`BP_MasterCamera_C` + `BP_TurretAIComponent_C`, and rely on `Multicast Spawn Emitter` / `Multicast Turret Idle Start/Stop` (`FUNC_Net|NetReliable|NetMulticast`) for cosmetic sync — a good simpler reference pattern for multicast-driven visual/animation sync vs. the property-replication pattern used for health.
- **Zombie/vehicle spawners are server-authoritative by construction**: `BP_VehicleSpawner_C`/`BP_VehicleUniqueSpawner_C` have a single `Svr_Spawn` (`FUNC_Net|NetReliable|NetServer`) entry point; `BP_AISpawner_Master_C` has no net-flagged functions at all (spawning fully server-side, replication is implicit via actor spawn), consistent with the project's existing server-authoritative approach.
- All BP-declared per-instance variables found in scope are components (no plain replicated `bool`/`float` health fields show up at the BP layer) — actual `Replicated`-flagged scalar properties (health value, fuel value, IsDead) live either as native C++ UPROPERTYs on `BP_MasterAICharacter_C`/`DamageComponent`/`VehicleHealthComponent`/`VehicleFuelComponent`'s native base classes, or as BP variables not captured by this export's property dump. Only inferred from function/delegate names — **not directly verified**, flag this before wiring an offset.

- **UPDATE — real offsets confirmed from `research/CXXHeaderDump/*.hpp` (actual native member-offset dump, not the structure-only FModel export above).** Highest-value find: `VehicleFuelComponent_C`'s `CurrentFuel` sits at **0x00C8** and `VehicleHealthComponent_C`'s `CurrentHealth` at **0x00C0** — both simple `double` fields, directly readable/writable via raw pointer arithmetic once you have the component pointer (itself at `BP_VehicleMaster_C::FuelComponent` 0x03D0 / `VehicleHealthComponent` 0x0390). This unblocks the pending vehicle fuel/health sync feature. See per-class sections below for the full property/offset lists added inline as `(0xNNNN, size 0xN)`.

---

## AI — Core / Shared

### BPI_AI (`AI/BPI_AI.json`)
Blueprint Interface implemented by AI characters. Purpose: shared AI query/marker/burn API.
| Function | Flags |
|---|---|
| AddMarker | Public, BlueprintCallable, BlueprintEvent |
| AIHealthBarInfo | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| CanAddMarkerToAI? | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| CanStompAI? | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| GetMarker | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| GetMesh | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| GetMovementSpeeds | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| HitByVehicle | Public, BlueprintCallable, BlueprintEvent |
| IsAIBurning? | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| OverrideName | Public, BlueprintCallable, BlueprintEvent |
| SetBurning | Public, BlueprintCallable, BlueprintEvent |

No net-flagged functions — `AIHealthBarInfo` (client health-bar UI query) and `HitByVehicle` are notable for a vehicle-hits-zombie sync feature.

### BPI_AIController (`AI/BPI_AIController.json`)
Tiny interface: `InCombat?` (Public, HasOutParms, BlueprintCallable, BlueprintEvent). Purpose unclear beyond combat-state query.

### BP_AI (`AI/BP_AI.json`)
Root AI character base (parent of `BP_Animal_C`; also implicit root of the AI-character tree). Components: CapsuleComponent, CharacterMovementComponent, SceneComponent, SkeletalMeshComponent.
Functions: `Event_BloodSplatter`, `ExecuteUbergraph_BP_AI` (graph-only entry, no RPCs). Purpose unclear beyond "generic pawn skeleton" — most gameplay logic lives in subclasses.

### BFL_AI (`AI/BFL_AI.json`)
Blueprint Function Library. One static function: `SpawnFloatingDamageText` (Static, Public, BlueprintCallable, BlueprintEvent). Useful for client-side damage-number popups without touching gameplay state.

### SD_AIComponent (`AI/SD_AIComponent.json`)
Actor component added to `BP_Animal_C` (and zombies via inheritance) — drives AI behaviour-tree blackboard values.
| Function | Flags |
|---|---|
| CheckBehaviour | Public, BlueprintCallable, BlueprintEvent |
| SetBB_Behaviour | Public, BlueprintCallable, BlueprintEvent |
| SetBB_ForceAttack | Public, BlueprintCallable, BlueprintEvent |
| SetBB_Location | Public, BlueprintCallable, BlueprintEvent |
| SetBB_LockOnTarget | Public, BlueprintCallable, BlueprintEvent |
| ReceiveBeginPlay | Event, Public, BlueprintEvent |

No net flags — blackboard writes are presumably server-only (behavior tree runs on server, animation/visual state replicates separately).

**Real offsets** (`CXXHeaderDump/SD_AIComponent.hpp`, matched `USD_AIComponent_C : UActorComponent`, size 0xC8): `CanSystemDespawnAI?` (0x00A8, size 0x1), `Behaviour` (0x00A9, size 0x1), `MeleeAttackDistance` (0x00B0, size 0x8), `Waypoint` (0x00B8, size 0x8), `MovementBlendspace` (0x00C0, size 0x8).

### BPI_Vehicle (`Blueprints/Vehicles/BPI_Vehicle.json`)
Blueprint Interface for vehicle interaction UI hooks.
| Function | Flags |
|---|---|
| GetVehicleMesh | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| OnBeginFuelInteract / OnEndFuelInteract | Public, BlueprintCallable, BlueprintEvent |
| OnBeginRepairInteract / OnEndRepairInteract | Public, BlueprintCallable, BlueprintEvent |
| OnBeginStorageInteract / OnEndStorageInteract | Public, BlueprintCallable, BlueprintEvent |

### DamageComponent (`Blueprints/Components/DamageComponent.json` — referenced by nearly every AI class; technically outside declared scope but included since it's directly load-bearing)
The shared health/damage engine for AI (and possibly players/objects generally).
| Function | Flags |
|---|---|
| DamageTaken | Public, BlueprintCallable, BlueprintEvent |
| FireDamageTaken | Public, HasDefaults, BlueprintCallable, BlueprintEvent |
| SetHealth | Public, BlueprintCallable, BlueprintEvent |
| IsAlive? | Public, HasOutParms, BlueprintCallable, BlueprintEvent, BlueprintPure |
| GetValues | Public, HasOutParms, BlueprintCallable, BlueprintEvent, BlueprintPure |
| Death__DelegateSignature | Public, Delegate, BlueprintCallable, BlueprintEvent |
| StandardDamage__DelegateSignature | Public, Delegate, BlueprintCallable, BlueprintEvent |
| SetBurning__DelegateSignature | Public, Delegate, BlueprintCallable, BlueprintEvent |
| SetCurrent&MaxHealth__DelegateSignature / _Event | Public/(Public), Delegate/Callable, BlueprintEvent |
| BurningFinished | Public, BlueprintCallable, BlueprintEvent |
| Event_BloodSplatterFX | BlueprintCallable, BlueprintEvent |

No net-flagged functions — health value itself is presumably a replicated property (not shown in this shallow export) that these functions read/write locally, broadcasting local delegates on both sides after replication updates it.

**Real offsets** (`CXXHeaderDump/DamageComponent.hpp`, matched `UDamageComponent_C : UActorComponent`, size 0x161):
- `CanDamage?` (0x00A8, size 0x1)
- `StandardHealthValue` (0x00B0, size 0x8)
- `CurrentHealth` (0x00B8, size 0x8)
- `MaxHealth` (0x00C0, size 0x8)
- `SaveValues?` (0x00E8, size 0x1)
- `TimeOfBurning` (0x0108, size 0x10)
- `HealthRandomDeviation` (0x0138, size 0x8)
- `CanScaleHealth?` (0x0150, size 0x1)
- `CanHeadshot1Shot?` (0x0160, size 0x1)

No BP-visible `Health` scalar was previously confirmed for this component — `CurrentHealth`/`MaxHealth` above are the real native fields, resolving the "not directly verified" flag above for this class.

---

## AI — Spawners

### BP_AISpawner_Master (`AI/AISpawners/BP_AISpawner_Master.json`)
Base class for all AI spawn-volume actors (super: `BP_MasterObject_C`). Components: BoxComponent (spawn volume), SceneComponent. 46 functions — spawn-point generation, radius/invoker checks, group spawn/respawn bookkeeping. No net-flagged functions (fully server-side; actor replication handles the rest).
Key functions (terse): `GenerateSpawnPoints`, `GetRandomSpawnPoint`, `SpawnActor`, `TryToSpawnGroup`, `SpawningLoop`, `KillSpawnedActors`, `UpdateSpawnerAfterActorWasKilled`, `UpdateSpawnerAfterSpawnActor`, `OnActorSpawned__DelegateSignature` (Delegate), `OnSpawnedActorKilled`, `IsAnyInvokerInRadius`/`IsAnyInvokerInSpawnRegion` (uses `AIOInvokerComponent`-style proximity check — same system as the player's own invoker component), `IsSpawnedClassReplicated` (Pure — spawner is aware of which classes replicate), `CanSpawnActors`/`CanSpawnerRespawnActors` (Pure gating).
Property: `SpawningBox_GEN_VARIABLE` (BoxComponent).
Subclasses (thin, only override construction/config, no new functions of note): `BP_AISpawner_Animals` (AISpawners/Animals), `BP_AISpawner_SettlementSurvivors` (AISpawners/Settlement), `BP_AISpawner_ZombieBosses`, `BP_AISpawner_ZombieHounds`, `BP_AISpawner_Zombies` (AISpawners/Zombies).

**Real offsets** (`CXXHeaderDump/BP_AISpawner_Master.hpp`, matched `ABP_AISpawner_Master_C : ABP_MasterObject_C`, size 0x411): `Billboard` (0x02A8, size 0x8), `SpawnRadius_DebugSphere` (0x02B0, size 0x8), `SpawningBox` (0x02B8, size 0x8), `SpawnRadius` (0x0310, size 0x8), `SpawnAmount` (0x0330, size 0x4), `RandomisedSpawnCount` (0x0334, size 0x1), `SpawnType` (0x0335, size 0x1), `SpawnedActorClass` (0x0338, size 0x8), `InitialSpawnDelay` (0x0340, size 0x8), `SpawningInterval` (0x0348, size 0x8), `bIsSpawning` (0x0350, size 0x1), `TotalAliveActors` (0x0354, size 0x4), `TotalSpawnedActors` (0x0358, size 0x4), `SpawnRetries` (0x0370, size 0x4), `DontSpawnCloseToPlayerRange` (0x0374, size 0x4), `IsSpawningStopped` (0x03A0, size 0x1), `InvokersInsideRegion` (0x03A4, size 0x4), `SpawningCapacity` (0x03A8, size 0x4).

### BP_AISpawningVolume_HumanQuest / _Prefab / _RandomChanceToSpawn (`AI/Humans/*.json`)
Super: `BP_MasterObject_C`. Human-specific spawn volumes (quest-triggered, prefab-group, and probability-gated variants). 17–21 functions each, largely overlapping with `BP_AISpawner_Master`'s spawn-point/volume logic plus variant-specific gating (`RandomChanceToSpawn` adds probability checks). Property: `AI Spawn Volume_GEN_VARIABLE` (BoxComponent). No net-flagged functions.

### BP_VehicleSpawner (`Blueprints/Vehicles/Spawner/BP_VehicleSpawner.json`)
Standalone vehicle spawn point (random selection among vehicle types). Components: BillboardComponent (editor marker), SceneComponent.
| Function | Flags |
|---|---|
| Svr_Spawn | **Net, NetReliable, NetServer**, BlueprintCallable, BlueprintEvent |
| SpawnSelectVehicles | Public, HasOutParms, HasDefaults, BlueprintCallable, BlueprintEvent, BlueprintPure |
| RandomChance | Public, HasOutParms, BlueprintCallable, BlueprintEvent, BlueprintPure |
| ComponentsToSave / ActorLoaded / ActorPreLoad / ActorPreSave / ActorSaved | Event, Public, BlueprintEvent (save-system hooks) |

**Real offsets** (`CXXHeaderDump/BP_VehicleSpawner.hpp`, matched `ABP_VehicleSpawner_C : AActor`, size 0x2B2): `Billboard` (0x02A0, size 0x8), `DefaultSceneRoot` (0x02A8, size 0x8), `SpawnerUsed?` (0x02B0, size 0x1), `VehicleSelection` (0x02B1, size 0x1).

### BP_VehicleUniqueSpawner (`Blueprints/Vehicles/Spawner/BP_VehicleUniqueSpawner.json`)
Super: `BP_MasterObject_C`. Spawns a specific (non-random) unique vehicle instance once. Same `Svr_Spawn` (Net|NetReliable|NetServer) RPC pattern as `BP_VehicleSpawner`, plus the same save-system hooks.

---

## AI — Animals

### BP_Animal (`AI/Animals/BP_Animal.json`)
Super: `BP_AI_C`. Shared base for Bear/Deer/Dog/Rabbit/Wolf. Components: CapsuleComponent, CharacterMovementComponent, SD_AIComponent_C, SkeletalMeshComponent.
Functions (17): `AI Alert`/`AI End Alert`, `AI Dead`, `AI Is Dead?` (Const, Pure-style), `Actor Aim Focus`, `Actor Attack Target`, `AI Animation Switch`, `AI Block`, `AI Camera Activate`, `AI Can Interact?`, `Alert Actor Defenders`, plus leftover `Turret Destroyed Effect`/`Turret Idle Start/Stop`/`Turret End/Start Reload` (inherited stub names, likely dead code carried from a shared base — not meaningful for animals). No net-flagged functions.

**Real offsets** (`CXXHeaderDump/BP_Animal.hpp`, matched `ABP_Animal_C : ABP_AI_C`, size 0x6A0): only own property is `SD_AIComponent` (0x0698, size 0x8) — the class adds no scalar fields of its own, confirming the catalog's inference above.

### Per-species animal classes (Bear/Deer/Dog/Rabbit/Wolf)
All follow the same shape: `BP_Animal<Species>_C` (super `BP_Animal_C`, 21–74 functions — mostly duplicated interact/Jig-inventory/marker boilerplate shared with humans, see BP_MasterBandit below for the common function set) + `BP_Animal<Species>_AIController_C` (super `BP_MainNeutralAIController_C`, 4–5 functions, perception-only) + `AnimBP_<Species>_C` (anim graph, 4–8 functions, no gameplay logic) + small BT task/service graphs (`<Species>_FindPlayerLocation`, `<Species>_FindRandomLocation`, `<Species>_MovementSpeed`, `<Species>_RunAwayFromPlayer`, `Attack<Species>` / `AttackPlayerWolf`, `<Species>_StopAnimation` — each 2 functions, `ExecuteUbergraph_*` + `ReceiveExecute` style, pure BT logic, no replication). No net-flagged functions anywhere in the animal tree. Purpose of the extra function volume on `BP_AnimalRabbit_C` (74) / `BP_AnimalWolf_C` (64) / `BP_AnimalBear_C` (60) vs `BP_Animal_C`'s 17 is unclear from export alone — likely per-species attack/flee state machine additions; bytecode not available to confirm.

---

## AI — Humans (Bandits / Military / Scavengers / Traders)

### BP_MasterBandit / BP_MasterMilitary / BP_MasterScavenger / BP_MasterTrader
(`AI/Humans/Bandits/BP_MasterBandit.json`, `AI/Humans/Military/BP_MasterMilitary.json`, `AI/Humans/Scavengers/BP_MasterScavenger.json`, `AI/Humans/Trader/BP_MasterTrader.json`)
All four share super `BP_MasterAICharacter_C` and an (almost) identical 57–58 function set — same archetype, different loot/behavior config. Components: AIOSubjectComponent (LOD/optimization subject), BP_JigMultiplayer_C (inventory/loot networking), BP_SmartAIComponent_C, CapsuleComponent, CharacterMovementComponent, NavigationInvokerComponent, ParticleSystemComponent, SkeletalMeshComponentBudgeted, StaticMeshComponent.

Grouped function summary (applies to all 4 classes):
| Group | Functions | Flags |
|---|---|---|
| Damage/health | `AI_Health`, `HealthMultiplier`, `Death` | BlueprintCallable/Event, HasDefaults (Death) — **not net-flagged**, driven via `DamageComponent` |
| Damage entry | `ReceiveAnyDamage` | BlueprintAuthorityOnly, Event, Public, BlueprintEvent (server-only native damage hook) |
| Burning | `SetBurning`, `StopBurning`, `IsAIBurning?` | BlueprintCallable/Event |
| Combat | `Event_Fire`, `CheckDistanceFromActor` | BlueprintCallable/Event |
| Markers/UI | `AddMarker`, `GetMarker`, `CanAddMarkerToAI?`, `Event_Marker`, `AIHealthBarInfo`, `CanStompAI?` | BlueprintCallable/Event |
| Interact (Jig framework) | `OnBeginInteract`, `OnEndInteract`, `OnExecuteInteract(Dialogue/Ended)`, `OnRequestServerInteract`, `OnServerExecuteInteract`, `OnStopExecuteInteract`, `JigCanInteract`, `JigSetCanInteract`, `SetInteractDistance`, `SetInteractOption`, `GetInteractOptions` | BlueprintCallable/Event, mostly HasOutParms — **no net flags visible even though "Server" is in the name**, so server-gating is presumably done inside the function body (`HasAuthority` branch) rather than via UFUNCTION replication |
| Inventory (Jig multiplayer) | `JigMP_On{ContainersSwap,ItemAdded,ItemConsumed,ItemDropped,ItemMoved,ItemRemoved,MainContainerItemAdded,MainContainerItemRemoved,PickupAdded,PickupEquipped,RequestDropItem,TwoContainersSwap,UpdateChamberUID}`, `JigMP_GetLootWidget` | BlueprintCallable/Event, HasOutParms |
| Loadout/appearance | `SetWeapons`, `SetMeshAndMaterial`, `OverrideName` | BlueprintCallable/Event |
| Misc | `HitByVehicle`, `GetMovementSpeeds`, `GetMesh`, `GetMainSceneComp`, `PickupBuildFromGround`, `SetPickupCount`, `SetItemShadowVisibility` (Bandit only), `RandomTraderType` (Trader only) | BlueprintCallable/Event |

Note: `HitByVehicle` present on every human/animal/zombie class — a single, consistent entry point across the whole AI roster for a vehicle-strikes-AI sync feature.

Subclasses: `BP_Bandit_Ranged_Boss` (super `BP_MasterBandit_C`, 1 function only — cosmetic/stat override, no new logic) and `BP_Military_Boss` (super `BP_MasterMilitary_C`, same shape).

**Real offsets** — all 4 header dumps exist (`CXXHeaderDump/BP_MasterBandit.hpp`, `BP_MasterMilitary.hpp`, `BP_MasterScavenger.hpp`, `BP_MasterTrader.hpp`, each `: ABP_MasterAICharacter_C`). Note: **none of these classes has a bare `Health`/`CurrentHealth` scalar of its own** — health lives entirely on the (not-yet-located-in-headers) `DamageComponent` instance these classes reference by name only in the FModel export; the per-class layouts below are otherwise near-identical, offset by a few bytes depending on each class's extra cosmetic-mesh fields.
- `BP_MasterBandit_C` (size 0x9C9): `BP_JigMultiplayer` (0x0878, size 0x8), `AIOSubject` (0x0880, size 0x8), `LootTable` (0x08A0, size 0x1), `Name` (0x08A8, size 0x18), `Boss?` (0x08C0, size 0x1), `RangedWeapon` (0x08C8, size 0x10), `MeleeWeapon` (0x08D8, size 0x10), `CanAddMarker?` (0x0998, size 0x1), `Marker` (0x09A0, size 0x8), `Is Burning?` (0x09B0, size 0x1), `Looted` (0x09C8, size 0x1).
- `BP_MasterMilitary_C` (size 0xA49-ish, ends ~0x0A48): `LootTable` (0x08A0, size 0x1), `Name` (0x08A8, size 0x18), `Boss?` (0x08C0, size 0x1), `RangedWeapon` (0x08C8, size 0x10), `MeleeWeapon` (0x08D8, size 0x10), `CanAddMarker?` (0x0A18, size 0x1), `Marker` (0x0A20, size 0x8), `Is Burning?` (0x0A30, size 0x1), `Looted` (0x0A48, size 0x1).
- `BP_MasterScavenger_C`: `Name` (0x08A0, size 0x18), `Gender?` (0x08B8, size 0x1), `RangedWeapon` (0x08C0, size 0x10), `MeleeWeapon` (0x08D0, size 0x10), `CanAddMarker?` (0x0988, size 0x1), `Marker` (0x0990, size 0x8), `Is Burning?` (0x09A0, size 0x1), `Looted` (0x09B8, size 0x1). (No `LootTable`/`Boss?` fields on this class, unlike Bandit/Military.)
- `BP_MasterTrader_C`: `Dialogue` (0x08A0, size 0x8), `Gender?` (0x08A8, size 0x1), `RangedWeapon` (0x08B0, size 0x10), `MeleeWeapon` (0x08C0, size 0x10), `CanAddMarker?` (0x0998, size 0x1), `Marker` (0x0990, size 0x8), `Is Burning?` (0x09A8, size 0x1), `Looted` (0x09C0, size 0x1), `CurrentlyInteracting?` (0x09C1, size 0x1).

### BP_AIWeapon_Master (`AI/Humans/Weapons/BP_AIWeapon_Master.json`)
AI-held weapon actor (ranged + melee). Components: ArrowComponent, BillboardComponent, SceneComponent, SkeletalMeshComponent, StaticMeshComponent.
| Function | Flags |
|---|---|
| Server Melee Attack | **Net, NetReliable, NetServer**, BlueprintCallable, BlueprintEvent |
| Server End Melee Attack | **Net, NetReliable, NetServer**, BlueprintCallable, BlueprintEvent |
| Server Play Shoot Montage | **Net, NetReliable, NetServer**, BlueprintCallable, BlueprintEvent |
| Server Play Skeletal Mesh Montage | **Net, NetReliable, NetServer**, BlueprintCallable, BlueprintEvent |
| Server Range Attack Muzzle Effect | **Net, NetReliable, NetServer**, BlueprintCallable, BlueprintEvent |
| Multicast Melee Emitter | **Net, NetReliable, NetMulticast**, BlueprintCallable, BlueprintEvent |
| Multicast Play Skeletal Mesh Montage | **Net, NetReliable, NetMulticast**, BlueprintCallable, BlueprintEvent |
| Multicast Range Attack Effects | **Net, NetReliable, NetMulticast**, BlueprintCallable, BlueprintEvent |
| Melee Hit / Update Melee Hit | Public, BlueprintCallable/Event |
| Add/Remove Current Ammo, Remove Total Ammo, Current/Total Ammo Check | Public, BlueprintCallable/Event(some Pure) |
| SpawnTrail | Public, BlueprintCallable, BlueprintEvent |

This is the cleanest **Server-RPC-in → Multicast-out** cosmetic-effect pattern found in scope: client-visible attack effects go through explicit reliable Server*/Multicast* pairs, unlike the health system which relies on property replication. Directly reusable pattern for zombie/AI attack-effect sync.

---

## AI — Turrets / Cameras

### BP_BanditTurret_AI / BP_SurvivorTurret_AI (`AI/BP_BanditTurret_AI.json`, `AI/BP_SurvivorTurret_AI.json`)
Super: `BP_MasterTurret_C`. Placed defensive turret actors. Components: AIOSubjectComponent, BP_TurretAIComponent_C, SphereComponent (detection), StaticMeshComponent.
| Function | Flags |
|---|---|
| Multicast Spawn Emitter | **Net, NetReliable, NetMulticast**, BlueprintCallable, BlueprintEvent |
| Multicast Turret Idle Start | **Net, NetReliable, NetMulticast**, BlueprintCallable, BlueprintEvent |
| Multicast Turret Idle Stop | **Net, NetReliable, NetMulticast**, BlueprintCallable, BlueprintEvent |
| Turret Idle Start/Stop, Turret Destroyed Effect | Public, BlueprintCallable/Event (local wrappers presumably calling the Multicast_ versions) |
| IsObjectDamageable? | Public, HasOutParms, BlueprintCallable, BlueprintEvent |
| CheckBehindTurret (Bandit only) | Public, BlueprintCallable, BlueprintEvent |

`BP_BanditTurret_AI` additionally carries the full Jig interact/inventory function set (loot on destroy). `BP_SurvivorTurret_AI` is a leaner variant (no Jig set, fewer functions).

**Real offsets** (`CXXHeaderDump/BP_MasterTurret.hpp`, matched `ABP_MasterTurret_C : AActor`, size 0x311 — the shared base both `BP_BanditTurret_AI`/`BP_SurvivorTurret_AI` build on): `BP_TurretAIComponent` (0x02D8, size 0x8), `FOV Down` (0x02F0, size 0x8), `FOV Up` (0x02F8, size 0x8), `FOV Right` (0x0300, size 0x8), `FOV Left` (0x0308, size 0x8), `Debug Mode` (0x0310, size 0x1). No health/damage scalar on this class itself — damage routes through `ReceiveAnyDamage`/`Damage_Object` only, consistent with the catalog's function-only findings above.

### BP_SecurityCamera_AI (`AI/BP_SecurityCamera_AI.json`)
Super: `BP_MasterCamera_C`. Same `BP_TurretAIComponent_C` base as turrets but a camera, not a weapon. Functions: `Multicast Spawn Emitter` (Net|NetReliable|NetMulticast), `ReceiveAnyDamage` (BlueprintAuthorityOnly), `IsObjectDamageable?`, `Turret Destroyed Effect`. Purpose: destructible security camera, presumably feeds an alert/detection system elsewhere (not in scope).

---

## AI — Zombies

### BP_MasterZombie (`AI/Zombies/BP_MasterZombie.json`)
Super: `BP_Zombie_C` (base not in scope). The core zombie archetype — 105 functions, largest class in the AI tree. Components: AIOSubjectComponent, AudioComponent, BP_JigMultiplayer_C, CCCollisionHandlerComponent, DamageComponent_C, NavigationInvokerComponent, SD_AIComponent_C, SkeletalMeshComponentBudgeted.

Grouped functions:
| Group | Functions | Flags notes |
|---|---|---|
| Death/health | `Death`, `ActorDead?`, `AI Dead`, `AI Is Dead? `(Const), `OnRep_IsDead?`, `DeathEvent__DelegateSignature` (Delegate), `Event_HealthMultiplier`, `Event_SpecialDeath` | **`OnRep_IsDead?` confirms a replicated bool `IsDead` property drives death state** — no net-flagged functions otherwise |
| Damage entry | `ReceiveAnyDamage` (BlueprintAuthorityOnly), `Damage_Object`, `Damage_Shoved`, `IsObjectDamageable?`, `HitByVehicle`, `Event_OnHit`, `Event_DamageMultiplier`, `Event_Rad` (radiation) | none net-flagged |
| Archetype variants | `Archetype_Acidic`, `Archetype_Bomber`, `Archetype_Sprinter`, `Archetype_ToughenedCrawler`, `Archetype_UltraRadiated` | BlueprintCallable/Event — behavior-modifier setup for zombie subtype (called once presumably at spawn) |
| Combat | `AttackPlayer`, `AttackTrace`, `LeftArmTrace`, `RightArmTrace`, `PlayZombieAttack`, `Actor Attack Target`, `Actor Aim Focus` | BlueprintCallable/Event |
| Alert/AI | `AI Alert`, `AI End Alert`, `AI Block`, `AI Animation Switch`, `AI Camera Activate`, `AI Can Interact?`, `Alert Actor Defenders`, `CheckDistanceFromActor`, `Change Speed`, `GetMovementSpeeds` | BlueprintCallable/Event |
| Lifecycle | `Event_Despawn`, `Event_BloodMoon`, `Event_XP`, `GetQuestArgument` | BlueprintCallable/Event |
| Burning | `SetBurning`, `StopBurning`, `IsAIBurning?` | BlueprintCallable/Event |
| Interact/inventory | full Jig interact + `JigMP_*` set (identical shape to humans, see above) | — |
| Markers/UI | `AddMarker`, `GetMarker`, `CanAddMarkerToAI?`, `AIHealthBarInfo`, `CanStompAI?`, `Event_Marker`, `OverrideName` | BlueprintCallable/Event |
| Misc | `Outline`, `SetMeshAndMaterial`, `GetMesh`, `GetMainSceneComp`, various `Turret *` stub names (dead/inherited, not meaningful here) | — |

Property: `DamageComponent_GEN_VARIABLE` (DamageComponent_C).

**Real offsets** (`CXXHeaderDump/BP_MasterZombie.hpp`, matched `ABP_MasterZombie_C : ABP_Zombie_C`, size 0x8A9): `DamageComponent` (0x0698, size 0x8), `SD_AIComponent` (0x06A0, size 0x8), `DamageToDo` (0x0710, size 0x8), `IsDead?` (0x0718, size 0x1) — confirms the catalog's inference that a replicated bool drives death state; this is the exact `OnRep_IsDead?`-backed field. Also: `HealthDeviation` (0x0808, size 0x8), `DamageMultiplier` (0x07CC, size 0x4), `SpeedMultiplier` (0x07D0, size 0x4). Note there is no bare `Health`/`CurrentHealth` field directly on `BP_MasterZombie_C` — actual health lives on the `DamageComponent` instance (see `DamageComponent.hpp` offsets above: `CurrentHealth` 0x00B8, `MaxHealth` 0x00C0 relative to that component's own base).

Zombie subclasses (all super `BP_MasterZombie_C`, thin overrides, 2–8 functions each): `BP_Zombie_Crawler` (Crawler/), `BP_Zombie_Dog` (Dog/), `BP_Zombie_Infected` + `BP_Zombie_Infected_Horde` (Infected/), `BP_Zombie_Radiated` (Radiated/), `BP_Zombie_Roamer` + `BP_Zombie_Roamer_Tutorial` (super `BP_Zombie_Roamer_C`) (Roamer/). None add net-flagged functions — archetype config only (mesh, stat multipliers, spawn hookup).

### BP_ZombieAIController (`AI/Zombies/BP_ZombieAIController.json`)
Super: `BP_MainEnemyAIController_C`. Components: AIPerceptionComponent, AISenseConfig_Hearing/Sight, PathFollowingComponent.
Functions: `PerceptionUpdate`, `PerceptionUpdateCheck`, `GetCurrentlyPerceived`, `GetZombiesInRadius`, `InCombat?`, `ReceivePossess`. No net-flagged functions — perception runs server-side only, as expected for an AI controller (never exists on clients for AI-controlled pawns).

### BP_ZombieBoss (`AI/Zombies/Boss/BP_ZombieBoss.json`)
Super: `BP_MainEnemy_C`. Boss-tier zombie, 88 functions — same shape as `BP_MasterZombie` (Jig/interact/marker sets duplicated) but with two genuine net-flagged actions:
| Function | Flags |
|---|---|
| Death | **Net, NetServer**, BlueprintCallable, BlueprintEvent |
| Despawn | **Net, NetServer**, BlueprintCallable, BlueprintEvent |
| BossDeath__DelegateSignature | Public, Delegate, BlueprintCallable, BlueprintEvent |
| OnRep_IsDead? | Public, HasDefaults, BlueprintCallable, BlueprintEvent |
| RadiationDamage, MeleeAttack, RangedAttack, PlayAttack | Public, BlueprintCallable/Event |

Unlike `BP_MasterZombie`'s `Death` (no net flags), the boss's `Death`/`Despawn` are explicit server RPCs — likely because boss death triggers global/UI-visible events (loot drop announcement, boss-kill broadcast) that must originate authoritatively. Property: `DamageComponent_GEN_VARIABLE`.

**Real offsets** (`CXXHeaderDump/BP_ZombieBoss.hpp`, matched `ABP_ZombieBoss_C : ABP_MainEnemy_C`, size 0x7B1): `DamageComponent` (0x0698, size 0x8), `SD_AIComponent` (0x06A0, size 0x8), `IsDead?` (0x06E1, size 0x1), `DamageToDo` (0x0718, size 0x8), `DamageMultiplier` (0x076C, size 0x4), `SpeedMultiplier` (0x0770, size 0x4), `HealthBarUI` (0x0769, size 0x1), `HealthBarEnabled` (0x076B, size 0x1). Same pattern as `BP_MasterZombie` — no direct `Health` scalar, health lives on the `DamageComponent`.

Boss subclasses (all super `BP_ZombieBoss_C`, 1 function = cosmetic override only): `BP_ZombieBoss_Butcher_Small`, `BP_ZombieBoss_Military_Small`, `BP_ZombieBoss_Scientist_Small`, `BP_ZombieBoss_Slobber_Small`, `BP_ZombieBoss_Survivor_Small` (all in `Boss/Types/`). Horde variants (`Boss/Types/Horde/`): `BP_ZombieBossHorde_Infestation`, `BP_ZombieBossHorde_Military`, `BP_ZombieBossHorde_Military_Small` — each adds 3 functions (`ExecuteUbergraph_*` + 2 more, likely horde-spawn-on-death hooks); exact purpose unclear from export alone. `BP_ZombieBossRock` (Boss/, 2 functions) — purpose unclear from export, possibly an environmental hazard tied to a boss fight.

### Zombie Behavior-Tree Tasks/Services (`AI/Zombies/Tasks/`, `AI/Zombies/Services/`)
All are 2-function graphs (`ExecuteUbergraph_*` + one receive/tick function), no replication relevance — pure server-side BT logic: `AI_FindRandomLocation`, `AI_FindWaypoint`, `AI_MoveToLocation`, `AI_MoveToSpecificLoc` (8 functions — largest of this set, likely has extra validation branches), `AI_SetBehaviour`, `AI_SetBoolValue`, `Zombie_Attack`, `Zombie_Chase`, `Zombie_EnemyInRange`, `Zombie_FindPlayerLocation`, `Zombie_MovementSpeed`, `ZombieBoss_Attack`, `ZombieBoss_Chase`, `ZombieBoss_EnemyInRange`, `ZombieBoss_MovementSpeed`.

### AnimBP_Zombie / AnimBP_ZombieBossBrute / AnimBP_ZombieDog (Anim Blueprints)
Animation graphs only (8–10 functions, all anim-state related, e.g. `ExecuteUbergraph_*`, blend/notify handlers). No gameplay logic, no net flags. Relevant to client rendering (visual state machine) but not to data sync.

---

## AI — Safe Zone / Civilians

### BP_RandomCivilian (`AI/SafeZoneAndCivilians/RandomCivilian/BP_RandomCivilian.json`)
Super: `BP_MainFriendly_C`. Non-hostile settlement NPC. Components: AIOSubjectComponent, CapsuleComponent, CharacterMovementComponent, NavigationInvokerComponent, SkeletalMeshComponentBudgeted, WidgetComponent (nameplate). 22 functions — full Jig interact set + `SetName`, `SetMeshAndMaterial`, `SetPickupCount`, `PickupBuildFromGround`. No damage/health functions present (friendly NPC — presumably invulnerable or health handled entirely by a parent not in scope). No net-flagged functions.

`BP_RandomCivilian_Controller` (super `BP_MainFriendlyAIController_C`, 3 functions) — thin possess/perception wrapper.

---

## Vehicles

### BP_VehicleMaster (`Blueprints/Vehicles/BP_VehicleMaster.json`)
The shared base for every drivable vehicle (all `Vehicle_<Type>_C` classes below have `super: BP_VehicleMaster_C`). Components: AIOSubjectComponent, AudioComponent, BoxComponent (×2: `FuelBoxCollision`, `HitBoxCollision`), BP_JigMultiplayer_C (trunk/storage inventory), **ChaosWheeledVehicleMovementComponent** (native UE Chaos vehicle physics), MediaSoundComponent, ParticleSystemComponent, PhysicsConstraintComponent (towing), RadioComponent_C, SkeletalMeshComponent, SpotLightComponent (headlights), **VehicleFuelComponent_C**, **VehicleHealthComponent_C**, WidgetComponent (fuel gauge widget).

Net-flagged functions:
| Function | Flags |
|---|---|
| Svr_UpdateEngine | Net, NetServer, BlueprintCallable, BlueprintEvent |
| Svr_UpdateFuel | Net, NetServer, BlueprintCallable, BlueprintEvent |

(Neither is marked `NetReliable` — unlike the AI weapon RPCs — consistent with these being high-frequency per-tick driving-input updates where occasional drops are acceptable.)

Other notable functions:
| Group | Functions |
|---|---|
| Damage | `ApplyDamageToAIBasedOnSpeed`, `ApplyDamageToVehicleBasedOnSpeed`, `ReceiveAnyDamage` (BlueprintAuthorityOnly), `VehicleHealth`, `VehicleHitAudio` |
| Fuel | `UseGasCan`, `BndEvt__*_FuelBoxCollision_*OverlapSignature*` (fuel pickup trigger volume) |
| Enter/exit | `ExitVehicle`, `Event_ExitVehicle`, `CheckCanExitCarDriverSide`, `CheckCanExitCarPassengerSide`, `Interact_GetInVehicle` |
| Storage | `Interact_OpenStorage`, `UpdateStorageStat`, full Jig interact/inventory set |
| Towing | `AddTowingComp`, `RemoveTowingComp`, `TraceForVehicleToTow` |
| Physics/state | `IsInAir?`, `CheckWater`, `Event_FlipVehicle`, `Event_Landed`, `Event_Noise` |
| Lights/sound | `TurnOnOffLights`, `Horn`, `VehicleSound`, `VehicleSmoke`, `VehicleFire` |
| Save system | `ActorLoaded`, `ActorPreLoad`, `ActorPreSave`, `ActorSaved`, `ComponentsToSave`, `SetSaveProperties` |

Properties (all component references — no bare scalar health/fuel float visible at the BP layer in this export):
- `FuelBoxCollision_GEN_VARIABLE` (BoxComponent)
- `HitBoxCollision_GEN_VARIABLE` (BoxComponent)
- `FuelComponent_GEN_VARIABLE` (VehicleFuelComponent_C)
- `VehicleHealthComponent_GEN_VARIABLE` (VehicleHealthComponent_C)
- `FuelWidget_GEN_VARIABLE` (WidgetComponent)

**Real offsets** (`CXXHeaderDump/BP_VehicleMaster.hpp`, matched `ABP_VehicleMaster_C : AWheeledVehiclePawn`, size 0x530 — this resolves the "no bare scalar visible" gap above, confirming fuel/health are component pointers on this class, with the actual scalars one level down inside those components):
- `VehicleHealthComponent` (0x0390, size 0x8) — pointer to `UVehicleHealthComponent_C`
- `RepairWidget` (0x0398, size 0x8), `RepairBoxCollision` (0x03A0, size 0x8)
- `FuelWidget` (0x03A8, size 0x8), `FuelBoxCollision` (0x03B0, size 0x8)
- `HitBoxCollision` (0x03B8, size 0x8)
- `FuelComponent` (0x03D0, size 0x8) — pointer to `UVehicleFuelComponent_C`
- `VehicleUI` (0x03E8, size 0x8)
- `LightsOn?` (0x0400, size 0x1), `UniqueVehicle?` (0x0401, size 0x1)
- `Flipped` (0x0468, size 0x1), `InRefuellingArea?` (0x0469, size 0x1), `InRepairingArea?` (0x046A, size 0x1), `InStorageArea?` (0x046B, size 0x1)
- `EngineOn?` (0x04D8, size 0x1)
- `VehicleColor` (0x04F0, size 0x4)
- `DmgOnVehicleCollision` (0x0520, size 0x10, FVector2D — likely min/max speed-based damage thresholds used by `ApplyDamageToVehicleBasedOnSpeed`)

### VehicleFuelComponent (`Blueprints/Components/VehicleFuelComponent.json`, referenced by every vehicle — outside declared scope, included as directly load-bearing)
| Function | Flags |
|---|---|
| AddFuel | BlueprintCallable, BlueprintEvent |
| FuelSpend | BlueprintCallable, BlueprintEvent |
| StartFuelSpend / StopSpendFuel | BlueprintCallable, BlueprintEvent |
| FuelUpdate__DelegateSignature | Public, Delegate, BlueprintCallable, BlueprintEvent |
| UpdateUIFuel | Public, BlueprintCallable, BlueprintEvent |
| LoadComponent | BlueprintCallable, BlueprintEvent (save-system restore) |

**REAL OFFSETS CONFIRMED — highest-priority find of this pass** (`CXXHeaderDump/VehicleFuelComponent.hpp`, matched `UVehicleFuelComponent_C : UBaseComponent_C`, size 0x108):
- `FuelRestore_Timer` (0x00C0, size 0x8)
- **`CurrentFuel` (0x00C8, size 0x8, double)** — the actual live fuel value
- `MaxFuel` (0x00D0, size 0x8, double)
- `WasteTime` (0x00D8, size 0x8)
- `WasteOfFuelPerTime` (0x00E0, size 0x8)
- `Empty` (0x00E8, size 0x1, bool)
- `Vehicle` (0x0100, size 0x8) — back-pointer to owning `ABP_VehicleMaster_C`

### VehicleHealthComponent (`Blueprints/Components/VehicleHealthComponent.json`, same scope note as above)
| Function | Flags |
|---|---|
| AddHealth | BlueprintCallable, BlueprintEvent |
| UpdateUIHealth | Public, BlueprintCallable, BlueprintEvent |
| FuelUpdate__DelegateSignature | Public, Delegate, BlueprintCallable, BlueprintEvent (likely a copy/paste from the fuel component — name doesn't match this component's purpose) |
| LoadComponent | BlueprintCallable, BlueprintEvent |

**REAL OFFSETS CONFIRMED — highest-priority find of this pass** (`CXXHeaderDump/VehicleHealthComponent.hpp`, matched `UVehicleHealthComponent_C : UBaseComponent_C`, size 0xE8):
- **`CurrentHealth` (0x00C0, size 0x8, double)** — the actual live health value
- `MaxHealth` (0x00C8, size 0x8, double)
- `Vehicle` (0x00E0, size 0x8) — back-pointer to owning `ABP_VehicleMaster_C`

This resolves the "would need a deeper dump" gap flagged directly below — the real member-offset dump in `research/CXXHeaderDump/` provides exactly that. Neither component has a `Net`/replicated flag visible in the header (UE4SS header dumps don't surface `PropertyFlags`), so replication status for `CurrentFuel`/`CurrentHealth` is still not directly confirmed by this file alone — cross-check against the FModel export or a live property-flag read before assuming replication.

### VehicleUI (`Blueprints/Vehicles/VehicleUI.json`)
Widget blueprint bound to the vehicle HUD. Properties: `FuelBar`, `HealthBar` (both `ProgressBar`) — confirms fuel/health are consumed client-side as normalized 0–1 values for display.

**Real offsets** (`CXXHeaderDump/VehicleUI.hpp`, matched `UVehicleUI_C : UUserWidget`, size 0x2F8): `FuelBar` (0x02D0, size 0x8), `HealthBar` (0x02D8, size 0x8), `Name` (0x02E0, size 0x8), `SpeedText` (0x02E8, size 0x8), `Vehicle` (0x02F0, size 0x8, back-pointer to `ABP_VehicleMaster_C`).

### Per-type vehicle classes (`Blueprints/Vehicles/Types/*/`)
Each vehicle type folder has 3 files, all `super: BP_VehicleMaster_C` (except where noted) and near-zero added logic:
- `Vehicle_<Type>_C` — the drivable actor. Usually 1 function (`ExecuteUbergraph_*` only, pure config); `Vehicle_Humvee_C`, `Vehicle_RV_C`, `Vehicle_SwatVan_C` have 3–4 (extra seat/turret hookup, not detailed further — export doesn't disambiguate). Components mirror `BP_VehicleMaster` (FuelBoxCollision, HitBoxCollision, and — inconsistently — `FuelComponent_GEN_VARIABLE`/`VehicleHealthComponent_GEN_VARIABLE`: **Buggy, PickupTruck, RV, SUV, Van omit the `VehicleHealthComponent_GEN_VARIABLE`/`FuelComponent_GEN_VARIABLE` overrides in their own class** — meaning those types inherit the parent's default component rather than reconfiguring it; not a functional difference, just whether the BP re-declares the component).
- `BPAnim_<Type>_C` — anim blueprint (2 functions, graph-only).
- `SC_<Type>_C` — a `WidgetBlueprintGeneratedClass` (18 functions) — this is the **vehicle's storage/inventory container UI** (functions: `GetAllAttachments`, `GetContainerByAttachmentType`, `GetListOfContainers`, `GetLootContent`, `JigSetLootContent`, `SetActorOwner`, `SetItemReference`, etc. — Jig storage-container framework, not vehicle stats).

Types present: 4x4Jeep, Ambulance, BigRig, Buggy, Charger, DamagedSedan, FlatbedTruck, GolfCart, Humvee, ModifiedPickupTruck, PickupTruck, RV, SUV, SwatVan, Truck, UtilityPickupTruck, Van. All follow the identical 3-file shape above; no per-type gameplay logic beyond mesh/component config, confirmed by the flat 1-function `Vehicle_*_C` bodies.

---

## Data-only assets skipped (no Blueprint class, per task scope)
`AI/AILootTables/*` (DataTable/CurveTable loot definitions), `AI/AISpawners/Enum_SpawnType.json` (enum, noted above only because it's referenced by spawners), `Blueprints/Vehicles/Spawner/Enum_VehicleSpawner.json` and `Struct_VehicleSpawner.json` (enum/struct backing the vehicle spawner's type selection — structure not dumped here, worth a follow-up read if the spawner's per-type weighting needs to be replicated).
