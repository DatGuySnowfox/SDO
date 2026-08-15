# SmartAI Blueprint Catalog (FModel export analysis)

Source: `Exports/SurrounDead/Content/SmartAI/**` (274 files). Export is flags-only (no bytecode) — function bodies unknown, only name/flags/params exist.

## Summary

`Content/SmartAI` is the **base AI framework/plugin layer**, not a separate or competing NPC category. `Content/AI`'s game-specific classes build directly on top of it: `BP_MasterBandit_C`/`BP_MasterMilitary_C`/`BP_MasterScavenger_C`/`BP_MasterTrader_C`/`BP_MasterZombie_C` (all cataloged in `bp_catalog_ai_vehicles.md`) either inherit from `SmartAI/Blueprints/AI/AICharacters/BP_MasterAICharacter.json`'s `BP_MasterAICharacter_C`, or embed its `BP_SmartAIComponent_C` (imported here as `AI/Humans/*`'s "BP_SmartAIComponent" component). `BP_MasterAICharacter_C` provides the generic character shell (climb/vault/edge timelines, `BP_AIInterface` implementation, `AutoPossessAI` wired to `BP_MasterAIController_C`); `BP_SmartAIComponent_C` (a huge ~180-property, 100+ function component) is the actual generic combat/flee/interact/melee/patrol "brain" config that `SD_AIComponent` (in `Content/AI`, per the sibling catalog) sits alongside/wraps for the game's specific archetypes. `Content/SmartAI` also owns the whole BT/EQS/decorator/service/task library, generic AI controller/perception setup, a `BP_AIManager`(Interface), interaction-point framework, generic turret/camera masters, a projectile master, and an unrelated example/demo layer (`BP_ExampleCharacter`, `BP_ExampleProjectile`, mannequin skeleton, first-person gun meshes/animations) that looks like leftover marketplace-asset demo content, not shipped gameplay. No overlap/duplication with `Content/AI` — it's a clean base/derived split. This strongly resembles a purchased "Smart AI System" marketplace plugin (folder name, `BP_SmartAIComponent`, `BP_MasterAICharacter`/`BP_MasterAIController` naming, decorator/service/task/EQS scaffolding, and a self-contained example character all point this way) — not confirmed, but purpose is otherwise clear from the export.

---

## Core framework

### BP_MasterAICharacter (`Blueprints/AI/AICharacters/BP_MasterAICharacter.json`)
Base AI character (super: `Character`). The generic pawn shell every AI character in `Content/AI` ultimately derives from (directly, e.g. `BP_AICivilian_C`, or via further `BP_AI_C`/`BP_MasterZombie_C`-style chains). Components: NavigationInvokerComponent, `BP_SmartAIComponent_C` (`BP_AIComponent`), CapsuleComponent, CharacterMovementComponent, SkeletalMeshComponent, 4 TimelineComponents (Climb/Vault/Smooth Move/Edge). Implements `BP_AIInterface`, `BPI_Quests`, `BPI_GlobalActorInfo`. `AIControllerClass` = `BP_MasterAIController_C`. 1 replicated property (`Holdable Actor`, an `ObjectProperty` referencing `BP_AIWeapon_Master_C`, flagged `Net`) — real offset confirmed via `CXXHeaderDump/BP_MasterAICharacter.hpp` (`ABP_MasterAICharacter_C : public ACharacter`, size 0x869): `Holdable Actor (0x0708, size 0x8)`. Also present in the dump: `BP_AIComponent (0x0698, size 0x8)`, `NavigationInvoker (0x0688, size 0x8)`, `Edge Timeline (0x06A8, size 0x8)`, `Smooth Move (0x06B8, size 0x8)`, `Valt Timeline (0x06C8, size 0x8)`, `Climb Timeline (0x06E0, size 0x8)`.
No net-flagged functions. Groups: Climb/Vault/Edge/Smooth-Move timeline update/finished funcs (movement traversal), `AI Alert`/`AI End Alert`/`AI Block`/`AI Animation Switch`/`AI Camera Activate`/`Actor Aim Focus`/`Actor Attack Target`/`Alert Actor Defenders` (shared AI signaling API, mirrored on the controller), `AI Dead`/`AI Is Dead?`, `Turret Idle Start/Stop`/`Turret Start/End Reload`/`Turret Destroyed Effect` (inherited stub names carried onto every character, not meaningful for non-turret AI), `XP`, `GetQuestArgument`, `Event_BloodSplatter`, `Debug Text`.

### BP_MasterAIController (`Blueprints/AI/AICharacters/BP_MasterAIController.json`)
Super: `DetourCrowdAIController`. Generic AI controller — AIPerceptionComponent (Sight: LoseSightRadius 7500, Hearing: 16000, both detect neutrals+friendlies), CrowdFollowingComponent. Implements `BP_AIInterface`. 1 replicated property: `Sight Actors` (array of `Actor*`, `Net` flag) — the server-authoritative perceived-actors list, replicated down for client-visible effects (e.g. health bars, aggro indicators). Other properties: `React Time Actors` (struct array, `S_ReactionTime`), `Starting Perceved Actors`, `Starting Sight Radius`, `Respawn` (bool).
Real offsets confirmed via `CXXHeaderDump/BP_MasterAIController.hpp` (`ABP_MasterAIController_C : public ADetourCrowdAIController`, size 0x411): `AIPerception (0x03C8, size 0x8)`, `Smart AI Component (0x03D0, size 0x8)`, `React Time Actors (0x03D8, size 0x10)`, `Sight Actors (0x03E8, size 0x10)`, `Starting Perceved Actors (0x03F8, size 0x10)`, `Starting Sight Radius (0x0408, size 0x8)`, `Respawn (0x0410, size 0x1)`.
No net-flagged functions. Functions mirror the character's signaling API (`AI Alert/End Alert/Block/Animation Switch/Camera Activate`, `Alert Actor Defenders`, `Actor Aim Focus/Attack Target`, `AI Dead`/`AI Is Dead?` (Const)), plus `Perception Update Check`, `Reaction Time Update`, `Find Reaction Time`, `Starting Perception`, `ReceivePossess`, and `BndEvt__AIPerception_..._ActorPerceptionUpdatedDelegate` (bound to `AIPerception.OnTargetPerceptionUpdated`) — perception runs fully server-side as expected.

### BP_SmartAIComponent (`Blueprints/AI/Components/BP_SmartAIComponent.json`)
The generic AI "brain" actor component — by far the largest/most detailed class in scope (~180 properties, 100+ functions). Owns nearly all tunable AI behavior: combat (accuracy, equip weapon, ammo-vs-melee fallback), flee (low-health/found triggers, flee type, return-to-start), hide (montages, hide time), interaction points (specified/tag-based, range, animations, order), melee (radius, collision, damage, distance, switch distance, attack frequency, hit-actor tracking), search/patrol (wait time, search area, behavior index), roam/waypoint, and death (death animations). Properties `Health` (0x0134, size 0x4), `Dead` (0x0130, size 0x1), `Attackers` (array) (0x00B8, size 0x10), `Attack Target` (0x0288, size 0x8), `AI Current Behaviour` (0x00D0, size 0x1) are the core per-instance runtime state — not confirmed replicated at this shallow property scan depth (no `Net`/`Replicated` flag observed on `Health`/`Dead` themselves in the sampled region), but several **RPC functions confirm authoritative-server + multicast-cosmetic pattern**. Real offsets confirmed via `CXXHeaderDump/BP_SmartAIComponent.hpp` (`UBP_SmartAIComponent_C : public UActorComponent`, size 0x1C00 — the largest header in the dump, ~180 properties fully laid out with byte offsets from 0x00A8 through 0x1BF8). A few more high-value offsets for live reflection work: `Accuracy (0x0138, size 0x8)`, `Combat Equip Weapon (0x0140, size 0x1)`, `Interaction Point (0x00C8, size 0x8)`, `AI Controller (0x0108, size 0x8)`, `AI Character (0x0120, size 0x8)`, `Way Point (0x0128, size 0x8)`, `Flee Low Health (0x01A8, size 0x4)`, `Flee When Found (0x01AC, size 0x1)`, `Should Hide (0x01AE, size 0x1)`, `Can Melee Attack (0x01E0, size 0x1)`, `Melee Distance (0x0208, size 0x8)`, `Melee Switch Distance (0x0210, size 0x8)`, `Melee Attack Frequancy (0x0278, size 0x8)`, `Combat (0x02C0, size 0x1)`, `Current Behavior Index (0x02E0, size 0x4)`, `Move To Actor (0x02E8, size 0x8)`, `Reloading (0x0490, size 0x1)`, `Current Ammo (0x0494, size 0x4)`, `Ragdoll On Death (0x0510, size 0x1)`, `Max Magazine Ammo (0x11F0, size 0x4)`, `Max Total Ammo (0x1200, size 0x4)`, `Use Ammo (0x1204, size 0x1)`, `Range Projectile (0x1220, size 0x8)`, `Trace Range (0x1228, size 0x8)`, `Can Range Attack (0x1320, size 0x1)`, `Homing Projectile (0x1398, size 0x1)`, `Follow Actor (0x1508, size 0x8)`, `Max Follow Distance (0x1510, size 0x8)`, `Climbing (0x1698, size 0x1)`, `In Cover (0x1728, size 0x1)`, `Can Climb (0x1729, size 0x1)`, `Combat Stance (0x18D8, size 0x1)`, `AI Ready (0x18DA, size 0x1)`, `MaxHealth (0x1BF0, size 0x4)`, `Sight Actor (0x1BF8, size 0x8)`. Full ~180-field layout is in the `.hpp` for anything not itemized here.
| Function | Flags |
|---|---|
| Multicast AI Dead | Net, NetReliable, NetMulticast |
| Multicast Combat Stance | Net, NetReliable, NetMulticast |
| Multicast Despawn Body | Net, NetReliable, NetMulticast |
| Multicast Homing Projectile Spawn | Net, NetReliable, NetMulticast |
| Multicast Melee Emitter | Net, NetReliable, NetMulticast |
| Multicast Play Montage | Net, NetReliable, NetMulticast |
| Multicast Play Transition to Audio | Net, NetReliable, NetMulticast |
| Multicast Projectile Spawn | Net, NetReliable, NetMulticast (name truncated in scan — presumed same flags as siblings) |

Non-net functions include `Melee Trace Timer Events`, `Move Across`, and a large body of behavior/state helpers not fully enumerated here (function volume far exceeds this task's practical read budget — the multicast set above is the load-bearing net surface). This is the component that `Content/AI`'s `SD_AIComponent` (see sibling catalog) most likely layers game-specific config on top of, or replaces per-archetype — both coexist on the human/zombie archetypes per the AI catalog's component lists.

### BP_AIInterface / BPI_AIController-equivalent (`Blueprints/BP_AIInterface.json`)
Blueprint Interface implemented by `BP_MasterAICharacter_C` and `BP_MasterAIController_C` — the shared signaling contract (`AI Alert`, `Actor Attack Target`, `AI Dead`, `AI Block`, etc., matching the function names seen on both classes above). Not fully re-enumerated; matches the pattern of `BPI_AI` in `Content/AI`.

### BP_AIManager / BP_AIManagerInterface (`Blueprints/BP_AIManager.json`, `Blueprints/BP_AIManagerInterface.json`)
`BP_AIManager_C` super: `Actor`. Referenced by `BP_PlayerAIActivator_C` via `GetGameMode` + cast to `BP_AIManager_Interface` to read global AI settings (`Return AI Manager Settings -> AI Settings`) — a central per-level AI tuning source (spawn/activation radii, etc.). Likely a GameMode-attached singleton; not deeply explored (out of practical scope), but structurally the top of the AI activation hierarchy.

---

## AI activation / LOD

### BP_PlayerAIActivator (`Blueprints/AI/Components/BP_PlayerAIActivator.json`)
Actor component (likely attached to the player character) driving proximity-based AI activation/deactivation — an LOD/culling system. Properties: `AI Activation Proximity` (0x00B0, size 0x8), `AI Deactivation Proximity` (0x00A8, size 0x8), `Last Proximity Activation` (0x00B8, size 0x10) / `Last Proximity Deactivation` (0x00C8, size 0x10) (both `Net`-flagged timestamps — the only replicated properties found here). Real offsets confirmed via `CXXHeaderDump/BP_PlayerAIActivator.hpp` (`UBP_PlayerAIActivator_C : public UActorComponent`, size 0xD8).
| Function | Flags |
|---|---|
| Server Start Activator Timer | Net, NetReliable, NetServer |
| ReceiveBeginPlay | (local) |
| Update Proximity Activation / Update Proximity Deactivation | BlueprintCallable/Event — sphere-overlap actor scans (`SphereOverlapActors`) against object types, feeding the `BP_AIManager`-sourced radii |
Purpose: server-authoritative periodic sweep that activates/deactivates nearby AI for performance — directly relevant to any proxy/zombie culling work.

---

## Turrets / Cameras (generic masters)

### BP_MasterTurret (`Blueprints/AI/AICharacters/BP_MasterTurret.json`)
Base turret actor (super not captured in this pass — referenced elsewhere as parent of `Content/AI`'s `BP_BanditTurret_AI`/`BP_SurvivorTurret_AI`). No net-flagged functions found directly on this class in the scanned region — turret-specific RPCs live on the components below.

### BP_MasterCamera (`Blueprints/AI/AICharacters/BP_MasterCamera.json`)
Base security-camera actor (parent of `Content/AI`'s `BP_SecurityCamera_AI`). 
| Function | Flags |
|---|---|
| Multicast Turret Idle Start | Net, NetReliable, NetMulticast |
| Multicast Turret Idle Stop | Net, NetReliable, NetMulticast |
Cosmetic idle-state sync, consistent with the `Content/AI` turret pattern already documented.

### BP_TurretAIComponent (`Blueprints/AI/Components/BP_TurretAIComponent.json`)
The turret "brain" component (mirrors `BP_SmartAIComponent`'s role but turret-specific). Properties: `Reloading` (0x00A8, size 0x1), `Attacking` (0x00A9, size 0x1), `Attack Target` (0x00B8, size 0x8) / `Attack Targets` (0x00C0, size 0x10), `Current Ammo` (0x00D0, size 0x4) / `Starting Ammo` (0x00DC, size 0x4) / `Magazine Size` (0x00E0, size 0x4), `Current Health` (0x00D4, size 0x4) / `Starting Health` (0x0198, size 0x4) / `Max Health` (0x019C, size 0x4), `Dead` (0x00F0, size 0x1), `Detection Range` (0x0158, size 0x8), `Weapon Range` (0x0170, size 0x8), `Spread` (0x0178, size 0x8), `Alert AI` (0x0358, size 0x1) / `Alert Radius` (0x0360, size 0x8) / `Alert Actors` (0x0368, size 0x10) / `Alert Tags` (0x0378, size 0x10), `Reacted Actors` (0x0388, size 0x10), `Track Target` (0x0398, size 0x1). No `Net`/`Replicated` flags observed on these properties in the scan. Real offsets confirmed via `CXXHeaderDump/BP_TurretAIComponent.hpp` (`UBP_TurretAIComponent_C : public UActorComponent`, size 0x3C1).
| Function | Flags |
|---|---|
| Multicast Muzzle Effect | Net, NetReliable, NetMulticast |
| Multicast Projectile | Net, NetReliable, NetMulticast |
| Server Turret Damaged | Net, NetReliable, NetServer |
Other functions: `Reload`, `ReceiveTick`, `Activate Target Actors`, `Check Tags` — local logic; damage entry is server-authoritative (`Server Turret Damaged`) with muzzle/projectile visuals multicast out — same clean RPC-in/multicast-out pattern documented for `BP_AIWeapon_Master` in the AI/vehicle catalog.

### BP_BuildableTurretAIComponent (`Blueprints/AI/Components/BP_BuildableTurretAIComponent.json`)
Near-identical property/function shape to `BP_TurretAIComponent` (same property list verbatim) — a player-buildable-turret variant of the same brain component. Same `Multicast`/`Server` RPC pattern presumed (not independently re-verified per-function here given the identical structure). Confirmed via `CXXHeaderDump/BP_BuildableTurretAIComponent.hpp`: byte layout is identical offset-for-offset to `BP_TurretAIComponent.hpp` above (only the `AI Turret` field's pointee type differs — `ABuildable_TurretMASTER_C*` instead of `ABP_MasterTurret_C*` — plus two extra functions `ComponentLoaded`/`ComponentPreSave`), so the same offsets listed under `BP_TurretAIComponent` apply here.

---

## Interaction / Waypoints / Spawning

### BP_MasterInteractionPoint (`Blueprints/AI/InteractionPoint/BP_MasterInteractionPoint.json`)
Super: `Actor`. Components: SkeletalMeshComponent, BillboardComponent (editor), ArrowComponent, CapsuleComponent, SceneComponent. Holds an `AnimMontage` reference — a placed point AI walk to and play an interaction animation at (used by `BP_SmartAIComponent`'s interaction-point behavior settings). Not deeply function-scanned; data/placement actor more than logic-heavy.

### BP_MasterWayPoint / BP_AISpawnPoint / BP_AISpawningVolume (`Blueprints/AI/BP_MasterWayPoint.json`, `BP_AISpawnPoint.json`, `BP_AISpawningVolume.json`)
Placement-only actors for patrol waypoints and generic (non-archetype-specific) AI spawn points/volumes — the generic counterparts to `Content/AI`'s `BP_AISpawner_Master` family. Not deeply scanned (low expected logic per file size); worth a follow-up read if patrol-route or generic-spawn sync is needed.

### BP_AITrigger (`Blueprints/AI/Triggers/BP_AITrigger.json`)
Generic trigger volume actor for AI behavior events (name only sampled) — pairs with the BT decorators/services below.

---

## Behavior Tree / EQS scaffolding

`Blueprints/AI/Decorators/` (`BTD_Activated`, `BTD_CanStrafe`, `BTD_CanTargetInteract`, `BTD_FollowDistances`), `Blueprints/AI/Services/` (`BTS_FollowChecks`, `BTS_IsSearchDead`, `BTS_MeleeAttackLocation`, `BTS_PerceptionCheck`, `BTS_RangeAttackLocation`), `Blueprints/AI/Tasks/` (`BTT_AIInteract`, `BTT_AtWayPoint`, `BTT_BeginFlee`, `BTT_ClimbPath`, `BTT_EndFlee`, `BTT_EndHide`, `BTT_FindAIInteract`, `BTT_FindActionPoint`, `BTT_FindRoamLocation`, `BTT_FindWayPoint`, `BTT_Interact`, `BTT_MoveToTarget`, `BTT_RangeMoveTo`, `BTT_RoamWaitAnim`, `BTT_Search`, `BTT_StartHide`, `BTT_StillAnimation`, `BTT_StrafeLocation`, plus `BTS_CanPath`/`BTS_FaceDirectionAnimations` filed under Tasks), `Blueprints/AI/QueryContext/` (`QC_AIBase`, `QC_Attack`, `QC_AttackTarget`, `QC_Attackers`, `QC_TargetLocation`), `Blueprints/AI/EQS/` (`EQS_ClimbDown`, `EQS_Cover`, `EQS_Flee`, `EQS_RangeAttack`), and `Blueprints/AI/BT_AIMasterBehaviorTree.json` / `Blueprints/AI/BB_AIBlackBoard.json`.
This is a **standard, complete Behavior Tree + Environment Query System toolkit** — decorators (conditions), services (per-tick blackboard updates), tasks (leaf actions), query contexts and EQS generators for AI movement/targeting queries — all consumed by `BT_AIMasterBehaviorTree` against `BB_AIBlackBoard`. `BTT_AIInteract` sampled: properties `AI Component`, `Move To Actor AI Component`, `Time`, `Check Timer`, `Current Anim Index`, `Current Anims` — confirms it drives `BP_SmartAIComponent`'s interaction-point animation flow. All are pure server-side BT logic — no net flags expected or found in samples; not itemized further as none carry meaningful client-sync logic per the task's "skip pure BT tasks" guidance implied by the sibling catalog's approach.

---

## Example/demo layer (likely unused in shipped gameplay)

### BP_ExampleCharacter (`Blueprints/BP_ExampleCharacter.json`)
A first/third-person demo character (super not `BP_MasterAICharacter_C`-based per the interface/property shape — implements the same `BP_AIInterface`-style functions but as a standalone player-controllable demo). Notably has full player-style net RPCs, unlike anything else in scope:
| Function | Flags |
|---|---|
| Client Open Respawn Screen | Net, NetReliable, NetClient |
| Client Respawn Player | Net, NetClient |
| Server Aimed | (Net, presumably NetServer — truncated in scan) |
| Multicast Aimed | Net, NetReliable, NetMulticast |
| Multicast Player Death | Net, NetReliable, NetMulticast |
| Multicast Range Effect | Net, NetReliable, NetMulticast |
| Multicast Sound | Net, NetMulticast |
Also: `Kill Character`, `ReceiveAnyDamage` (BlueprintAuthorityOnly), input axis events, `ReceiveBeginPlay`. This is a **complete, independent respawn/death/combat RPC set** — a marketplace-plugin demo/tutorial character bundled with the asset, not part of SurrounDead's actual player pawn (the real player character is `BP_PlayerCharacter`, cataloged separately). Useful only as a reference implementation, not a live gameplay class.

### BP_ExampleProjectile / BP_MasterProjectile (`Blueprints/Projectiles/`)
`BP_MasterProjectile_C` (super `Actor`, SphereComponent + ProjectileMovementComponent) — generic hitscan/projectile base, likely what `BP_TurretAIComponent`'s `Multicast Projectile`/`Multicast Homing Projectile Spawn` spawn. `BP_ExampleProjectile` is a thin demo subclass.

### BP_TimeOfDay, BP_HUD, BP_RespawnHUD (`Blueprints/BP_TimeOfDay.json`, `Blueprints/Widgets/`)
Demo-layer support classes (day/night cycle, example HUD/respawn-screen widgets) paired with `BP_ExampleCharacter`'s respawn RPCs above. No net-flagged functions found on `BP_TimeOfDay` itself.

### BP_AIFunctions (`Blueprints/BP_AIFunctions.json`)
Blueprint Function Library — static helpers shared across the framework (not itemized; function-library pattern matches `BFL_AI` in `Content/AI`).

---

## Archetype example

### BP_AICivilian (`Blueprints/AI/AICharacters/Examples/Civilians/BP_AICivilian.json`)
Super: `BP_MasterAICharacter_C` directly (not through `Content/AI`'s human/animal chain) — a minimal example/reference archetype showing how to derive a concrete AI character from the framework base. Adds only a `DefaultSceneRoot` and construction-script scaffolding, no new functions — confirms this folder (`AICharacters/Examples/`) is demo/reference content, distinct from `Content/AI`'s actual shipped archetypes (bandits, zombies, traders, etc.).

---

## No own logic (appendix)

Skipped as pure data/placement/enum/struct or zero-function classes: `Blueprints/Enums/*` (`E_AIBehaviour`, `E_BlockType`, `E_DeadBodyReaction`, `E_FleeSetting`, `E_HitboxTypeAI`, `E_MovementSpeed`, `E_NoAmmo`, `E_OrderOptions`, `E_Radius`, `E_StartingAIBehaviours`, `E_StimuliReaction`), `Blueprints/Structs/*` (`S_AIAnims`, `S_AIInteractAnim`, `S_AIOptimization`, `S_AIRespawn`, `S_AISpawner`, `S_AllAIAnims`, `S_AnimStartEnd`, `S_AttachedHoldables`, `S_AudioStartEnd`, `S_BoneHitbox`, `S_ClimbAnims`, `S_EmoteAnimations`, `S_GeneralSettings`, `S_GenericAISettings`, `S_MeleeAnimations`, `S_ReactSettings`, `S_ReactedActors`, `S_ReactionTime`, `S_RoutineSettings`, `S_TriggerResponse` — shape backing for the properties cataloged above, worth a follow-up read only if exact byte layout is needed), `Blueprints/AI/EmoteProps/BP_EmoteProp.json` (placed prop, not scanned), all Animation Blueprints/Montages/Sequences/BlendSpaces under `Animations/` (client rendering only), and all `Materials/Meshes/Textures/Sounds/Effects` assets (non-logic content, first-person gun/mannequin demo art).
