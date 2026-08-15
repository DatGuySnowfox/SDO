# Blueprint Catalog — Player Core

Reference catalog of gameplay-relevant Blueprint classes under `Blueprints/` (top-level player/game
files plus `Components/`, `Events/`, `Interfaces/`, `MasterObjects/`). Generated from FModel JSON
exports (`Exports/SurrounDead/Content/Blueprints/...`); no bytecode available, so this covers
names/flags/properties only — not implementation. `[RPC]` tag = function has `NetServer`,
`NetMulticast`, or `NetClient` in its flags.

## Notable findings

`BP_PlayerCharacter_C`'s appearance-mesh properties (`Clothing_Torso/Legs/Feet/Gloves/Armor`,
`HairMesh`, `BeardMesh`, `EyebrowsMesh`, `SkinColor`) are **not** replicated (`InstancedReference |
NonTransactional` / `DisableEditOnInstance` only, no `Net` flag) — only the paired `*Equipped?`
booleans (`ClothingTorsoEquipped?`, `BodyArmorEquipped?`, etc., ~14 of them) carry `Net |
RepNotify`. This matches the ongoing investigation's finding that actual mesh application rides on
RPCs (`Svr_AttachClothing` / `MC_AttachClothing`) and `OnRep_*Equipped?` callbacks, not property
replication of the mesh references themselves — there is no single "SetClothing" replicated
variable, so any load-order gap in those RPCs/OnReps has no property-replication fallback.
`BP_PlayerCharacter_C::EquipClothingToMesh` also exists as a distinct function (Blueprint-level;
per the IDA log it's suspected to hold/relate to the same `SetSkinnedAssetAndUpdate` mesh-apply
logic as the RPCs). Also notable: `Svr_DestroyComponent` is flagged `NetClient` despite the
`Svr_` naming convention used everywhere else for server RPCs in this class — likely a
copy-paste/naming mistake worth double-checking against its actual call sites before relying on
name-based assumptions. `NumReplicatedProperties` on `BP_PlayerCharacter_C` is 22, but the child
properties list only turns up ~14 `RepNotify` equip bools + a few others — the rest are presumably
declared on the `BP_MasterPlayer_C` parent (out of this catalog's scope, `Other/More/`).

---

## BP_PlayerCharacter_C
`Blueprints/BP_PlayerCharacter.json` — the player Character class. Super: `BP_MasterPlayer_C`
(`Other/More/BP_MasterPlayer`, out of scope for this catalog but holds a large share of base
functionality — worth reading directly for anything not found here). `NumReplicatedProperties: 22`.
789 raw `Function` entries; ~560 after filtering auto-generated per-node noise (`InpActEvt_*`
enhanced-input event stubs, `OnBlendOut_<hash>`/`OnCompleted_<hash>` per-node
montage/timeline callbacks). Below is the curated, gameplay-relevant subset — full raw list is in
the JSON if something's missing here.

### RPCs (server + client + multicast) — verbatim flags
```
Client_ADSCameraShake        FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_AddUI                 FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_CameraShake           FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_HitEffects            FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_MapToggle             FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_RollRotation          FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_SetCombatCrosshair    FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_SetInteractCrosshair  FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_StopAllCameraShake    FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_Suicide                FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_UpdateHealthUI         FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_UpdateHungerUI         FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_UpdateOxygenUI         FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_UpdateRadiationUI      FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_UpdateStaminaUI        FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_UpdateThirstUI         FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Event_ADS UnCrouch            FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Grass                         FUNC_Net | FUNC_NetReliable | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_ADS                        FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_ADSCrouch                  FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_ADSUnCrouch                FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_AttachClothing             FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent   [appearance]
MC_BloodSplatter              FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_Crouch                     FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_CrouchSprint                FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_Distract                    FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_Flashlight                  FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_FlashlightRotation          FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_MeleeAim                    FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_Montage                     FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_PlaySoundAtLocation          FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_PlayerDead                   FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_SetCapsuleSize               FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_StopADS                      FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_StopMeleeAim                 FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_StopMontage                  FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_SwimFast                     FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_UnCrouch                     FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_UpdateCombatState            FUNC_Net | FUNC_NetReliable | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_UpdateSpeed                  FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
MC_UpdateWeaponSpeed            FUNC_Net | FUNC_NetMulticast | FUNC_BlueprintCallable | FUNC_BlueprintEvent
SERVER_ChangeActiveSlot          FUNC_Net | FUNC_NetReliable | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_ADS                          FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_ADSCrouch                    FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_AttachClothing               FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent   [appearance — confirmed native stub, see IDA log]
Svr_BloodSplatter                FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_Crouch                       FUNC_Net | FUNC_NetReliable | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_CrouchSprint                 FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_DestroyComponent             FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent   [NOTE: named Svr_ but flagged NetClient, not NetServer — verify before assuming]
Svr_Distract                     FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_Flashlight                   FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_FlashlightRotation           FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_MeleeAim                     FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_Montage                      FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_PlayerRoll                   FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_ReduceStamina                FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_SetADS                       FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_SetActorLocation             FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_SetActorRotation             FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_SetJumpVelocity              FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_SetMovement                  FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_StopADS                      FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_StopMeleeAim                 FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_StopMontage                  FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_SwimFast                     FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_UnCrouch                     FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_UpdateCombatState             FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_UpdateSpeed                   FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_UpdateSprintSpeed              FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_UpdateWalkSpeed                FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_UpdateWeaponSpeed               FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_WeaponShove                     FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
```
(72 RPCs total.)

### Appearance / equipment / clothing
- `CheckClothing`, `EquipClothingToMesh`, `EquipActorToSocket`, `BodyPartVisibility`,
  `HideAttachments` — apply/toggle body meshes and attachments.
- `EquipFishingRod`, `EquipMelee`, `EquipPrimary`, `EquipSecondary`, `EquipSidearm` — weapon-slot
  equips (local Blueprint events, not RPCs themselves — presumably route through the Jig
  inventory system's own RPCs, see `JigMP_On*` below).
- `MarkEnemy`, `GetArmorDamageReduction` — combat-relevant, reads armor state.
- `JigMP_OnPickupEquipped`, `JigMP_OnItemAdded/Removed/Moved/Consumed/Dropped`,
  `JigMP_OnMainContainerItemAdded/Removed`, `JigMP_OnTwoContainersSwap`, `JigMP_OnContainersSwap`,
  `JigMP_OnUpdateChamberUID`, `JigMP_OnRequestDropItem`, `JigMP_GetLootWidget` — inventory/loot
  sync events for the "Jig" inventory subsystem (components `BP_JigHelperComp_C`,
  `BP_JigMultiplayer_C`, `BP_JigContextMenuComp_C`, `JigItemOverItemComp_C` are attached to this
  class — see component list in the raw JSON).

### Combat / damage / health
`CalculatePlayerDamage` (BlueprintPure), `Deathbringer`, `AI Dead`, `AI Is Dead?`, `ActorDead?`,
`Actor Attack Target`, `ChamberWeapon`, `GetCurrentActiveWeapon`, `GrenadeThrowSpeed`,
`JigCheckWeaponAmmoCount`.

### Movement / camera / input handling
`CalcFPCamera`, `ChangePlayerPerspective`, `FreeLookMaths` (Pure), `Crouch Sprint`,
`Footstep Sound`, `AutoScopeZoom`, `NewFOVAiming`, plus the large `Event_*` family
(`Event_AimFirearm`, `Event_Sprint`, `Event_Crouch`, `Event_Jump`, `Event_HoldBreath`,
`Event_MeleeAim`, `Event_WeaponCycle`, etc.) which are the local input-driven handlers that in turn
call the `Svr_*`/`MC_*` RPCs above. ~90 `InpActEvt_IA_*_K2Node_EnhancedInputActionEvent_N` stubs
(Enhanced Input per-binding trampolines, one per input action/context) were filtered from this
list as noise — see raw JSON if a specific input binding matters.

### Save/load lifecycle
`ActorLoaded`, `ActorPreLoad`, `ActorPreSave`, `ActorSaved`, `ComponentsToSave`,
`InitiateGameSave`, `LoadPlayerInventory`, `LoadKeepPlayerInventoryFromDeath`,
`KeepInventoryOnDeath__DelegateSignature`, `LoadInventory__DelegateSignature`.
Per the IDA log, `ActorLoaded`/`ActorPreLoad`/`Event_LoadPlayer` are confirmed-empty
Blueprint-side stubs (single instrumentation call each) — any real load logic is native.

### Misc gameplay
`NightVisionOn/Check/BatteryCheck`, `IsGPSEquipped?`, `FlashlightOff/Toggle`,
`GetInteractOptions`, `OnBeginInteract`, `ExecuteExfil`/`CancelExfil`, `IsPlayerInVehicle?`,
`GetInVehicle`.

### Notable properties (ChildProperties on the class)
Non-replicated appearance/mesh refs (`InstancedReference | NonTransactional`, no `Net`):
```
Clothing_Feet   (ObjectProperty)   — 0x0760 (size 0x8), USkeletalMeshComponent*
Clothing_Legs   (ObjectProperty)   — 0x0768 (size 0x8), USkeletalMeshComponent*
Clothing_Torso  (ObjectProperty)   — 0x0770 (size 0x8), USkeletalMeshComponent*
Clothing_Gloves (ObjectProperty)   — 0x0780 (size 0x8), USkeletalMeshComponent*
Clothing_Armor  (ObjectProperty)   — 0x07B8 (size 0x8), USkeletalMeshComponent*
HairMesh        (ObjectProperty)   — 0x07C0 (size 0x8), UStaticMeshComponent*
BeardMesh       (ObjectProperty)   — 0x07C8 (size 0x8), UStaticMeshComponent*
EyebrowsMesh    (ObjectProperty)   — 0x0790 (size 0x8), UStaticMeshComponent*
SkinColor        (ObjectProperty, Edit|BlueprintVisible|DisableEditOnInstance) — 0x15A8 (size 0x8), UMaterialInstance*
ServerEquippedItems (StructProperty) — 0x0960 (size 0x9D8), FS_ServerEquippedItems
EquipmentUIDs   (MapProperty) — 0x1338 (size 0x50), TMap<uint8, int32>
EquipmentIDSlots (MapProperty) — 0x1388 (size 0x50), TMap<FName, uint8>
```
All confirmed live via `CXXHeaderDump/BP_PlayerCharacter.hpp`.

**`FS_ServerEquippedItems` layout** (`CXXHeaderDump/S_ServerEquippedItems.hpp`, size `0x9D8`) — 21 fields, each an `FRepItemInfo` (size `0x78`), one slot per equip category in a flat contiguous array-of-structs: `EquippedFacewear` 0x0000, `EquippedHeadwear` 0x0078, `EquippedEyewear` 0x00F0, `EquippedAccessory` 0x0168, `EquippedTorso` 0x01E0, `EquippedGloves` 0x0258, `EquippedLegs` 0x02D0, `EquippedFeet` 0x0348, `EquippedContainer` 0x03C0, `EquippedBodyArmor` 0x0438, `EquippedBackpack` 0x04B0, `EquippedPrimary` 0x0528, `EquippedSecondary` 0x05A0, `EquippedSidearm` 0x0618, `EquippedMelee` 0x0690, `EquippedThrowable` 0x0708, `EquippedFlashlight` 0x0780, `EquippedBinoculars` 0x07F8, `EquippedGPS` 0x0870, `EquippedCompass` 0x08E8, `EquippedFishingRod` 0x0960 (each offset relative to the struct's own base, i.e. add `0x0960` — `ServerEquippedItems`'s own class offset — to get the absolute in-object offset, e.g. absolute `EquippedFishingRod` = `0x0960 + 0x0960 = 0x12C0`). This is a much richer, per-category-named struct than the catalog's prior "StructProperty, unknown fields" note — every equip slot has its own dedicated, individually-addressable `FRepItemInfo` here, distinct from the `*Equipped?` bools which are just presence flags.

Note the large offset gap between the
mesh pointers (0x0760-0x07C8) and `ServerEquippedItems`/`EquipmentUIDs`/`EquipmentIDSlots`
(0x0960-0x13D8) — `WeaponEquipped?` sits at 0x13E3 right after that gap, and the whole equip-bool
block below doesn't start until 0x1DC0, meaning there's roughly 0x9E8 bytes of other class state
(inherited `BP_MasterPlayer_C`/native fields aren't visible here, but the intervening 0x1DC0-0x1DC8
range within this class itself is other, non-appearance member data) between the mesh refs and the
equip-state bools — i.e. these are NOT laid out as one contiguous "appearance struct" in memory,
consistent with the doc's existing observation that there's no single replicated appearance struct.

Replicated equip-state bools (`Edit | BlueprintVisible | Net | DisableEditOnInstance | RepNotify`), all confirmed via `CXXHeaderDump/BP_PlayerCharacter.hpp` (contiguous block, each 1 byte):
```
PrimaryWeaponEquipped?    0x1DC0 (0x1)      SecondaryWeaponEquipped? 0x1DC1 (0x1)
SidearmWeaponEquipped?    0x1DC2 (0x1)      MeleeWeaponEquipped?     0x1DC3 (0x1)
FacewearEquipped?         0x1DC4 (0x1)      HeadwearEquipped?        0x1DC5 (0x1)
EyewearEquipped?          0x1DC6 (0x1)      AccessoryEquipped?       0x1DC7 (0x1)
ClothingTorsoEquipped?    0x1DC8 (0x1)      ClothingGlovesEquipped?  0x1DC9 (0x1)
ClothingLegsEquipped?     0x1DCA (0x1)      ClothingFeetEquipped?    0x1DCB (0x1)
ContainerEquipped?        0x1DCC (0x1)      BodyArmorEquipped?       0x1DCD (0x1)
BackpackEquipped?         0x1DCE (0x1)      ThrowableEquipped?       0x1DCF (0x1)
FlashlightEquipped?       0x1DD0 (0x1)      FishingRodEquipped?      0x1DD1 (0x1)
```
(No `Net`/`Replicated` flags found on any other property in this class besides these — 22
`NumReplicatedProperties` reported by the class header doesn't fully reconcile with what's visible
in `ChildProperties` here; the remainder likely live on the `BP_MasterPlayer_C` parent — a matching
`BP_MasterPlayer.hpp` exists in `CXXHeaderDump/` and was not opened this pass; worth a follow-up if
exact offsets on the parent are needed.)

### Components attached (from raw JSON, informational)
`CapsuleComponent`, `CharacterMovementComponent`, `SkeletalMeshComponent`, `CameraComponent`,
`SpringArmComponent`, `SpotLightComponent` (flashlight), `WidgetComponent`,
`ParticleSystemComponent`, `MediaSoundComponent`, `PostProcessComponent`,
`SceneCaptureComponent2D`, `AIPerceptionStimuliSourceComponent`, `AIOInvokerComponent`,
`BuildingComponent_C`, `HungerThirstComponent_C`, `LockPickingComponent_C`, `MedicalComponent_C`,
`RadiationComponent_C`, `RadioComponent_C`, `StaminaComponent_C`, `SwimmingComponent_C`,
`VehicleDrivingComponent_C`, `BPC_MinimapSystem_C`, `PhotoModeComponent_C`,
`BP_JigHelperComp_C`, `BP_JigMultiplayer_C`, `JigContextMenuComp_C`, `JigItemOverItemComp_C`,
`DLWE_Interaction_C` (interaction system).

---

## BP_PlayerCharacter_Child_C
`Blueprints/BP_PlayerCharacter_Child.json` — thin subclass of `BP_PlayerCharacter_C`. 0 functions,
0 net properties in its own JSON (all inherited). Purpose unclear from export beyond "a variant
Character BP" — possibly used for a specific spawn context (NPC-driven player double, cutscene,
or similar). Not enough here to say more without opening the file in-editor.

---

## BP_PlayerController_C
`Blueprints/BP_PlayerController.json` — PlayerController. Super: `BP_MasterPlayerController_C`
(`Other/More/`, out of scope). No replicated properties found in `ChildProperties`.

### RPCs
```
CameraShakeClient          FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_Died                FUNC_Net | FUNC_NetReliable | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Client_SUICIDE              FUNC_Net | FUNC_NetClient | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_RequestRespawn_Random   FUNC_Net | FUNC_NetReliable | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_RequestRespawn_SpawnPoint  FUNC_Net | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
Svr_RequestRespawnSuicide    FUNC_Net | FUNC_NetReliable | FUNC_NetServer | FUNC_BlueprintCallable | FUNC_BlueprintEvent
```
### Gameplay / lifecycle
`BindPlayerRespawn`, `Death_PlayerStats`, `LoadGameFromSlot`, `LoadPlayerInventoryFromSlot`,
`SaveGameToSlot`, `ReceiveBeginPlay`, `ReceivePossess`, `CameraShake`, `EventLightShafts`,
`EventVolFog`, `GetJoystickDirection`, `SensitivityChanged`, `ComponentsToSave`.
Also several `OnCompleted_<hash>`/`OnFailed_<hash>` async-load-node callbacks (kept — only 8, not
filtered as noise here).

---

## BP_SurroundeadGameMode_C
`Blueprints/BP_SurroundeadGameMode.json` — GameMode. No declared parent shown (`Super: None` in
this export, i.e. likely inherits the native `AGameModeBase`/project default rather than another
Blueprint). No replicated properties found. Small: `AutoSaveGame`, `Event_AutoSave`,
`Event_BindDaysSurvived`, `EventBindMidnight`, `EventSave`, `ReceiveBeginPlay`, `ReceiveEndPlay`,
`ResetStats`. No RPCs (GameMode only exists on server, unsurprising).

## BP_SurroundeadGameState_C
`Blueprints/BP_SurroundeadGameState.json` — GameState. No declared BP parent, no replicated
properties found in `ChildProperties`. Functions center on a screenshot/snapshot ("Snap") system
and an item-capture queue: `AddItemToQueue`, `CanItemBeCaptured`, `GetSnap`, `UpdateSnap`,
`UpdateSnapCustom`, `HandleQueue`, `HandleNext`, `HandleSnapTaken`, `OnSnapTaken`,
`SpawnSnapshotCaptor`, `GetLocalAttachmentByID`, `UpdateAttachments`,
`InspectorGetPrimitiveComponent`. Purpose reads as photo-mode/inventory-thumbnail rendering
infrastructure living on GameState (likely so it can be triggered/queried from any client) rather
than core survival gameplay state — worth confirming if this is actually load-bearing for MP sync
or purely cosmetic (PhotoMode-adjacent) before spending time here.

## SD_GameInstance_C
`Blueprints/SD_GameInstance.json` — GameInstance. No BP parent shown, no replicated properties.
Functions split cleanly into two groups:
- **Settings/graphics**: `AmbientVolumeChanged`, `MasterVolumeChanged`, `MusicVolumeChanged`,
  `EffectsVolumeChanged`, `UIVolumeChanged`, `RadioMusicVolumeChanged`, `GamepadIconsChanged`,
  `ChangedAMDFrameGen`, `ChangedUpscalingMethod`, `ChangedUpscalingQuality`.
- **Level/spawn/respawn flow**: `Event_LoadLevel`, `SmoothLoadLevel__DelegateSignature`,
  `GameMode_SpawnPlayer`, `Survival_SpawnPlayer`, `Survival_Respawn`, `Survival_SuicideRespawn`,
  `PlayerRespawned__DelegateSignature`, `PlayerSpawnedInLevel__DelegateSignature`,
  `Event_ResetAIOSpawner`, `Debug_SpawnPlayer`, `Debug_SpawnCamera`. This is likely the entry
  point worth checking for anything related to the join/spawn-time appearance-load race noted in
  the IDA investigation log (`RepActorsData` timing issue) — `GameMode_SpawnPlayer` /
  `Survival_SpawnPlayer` are the natural place a load-order gate would live.

No RPCs (GameInstance doesn't replicate).

---

## GameFunctionLibrary_C
`Blueprints/GameFunctionLibrary.json` — static Blueprint function library (all functions
`FUNC_Static`). No properties, no RPCs (static libraries can't have RPCs). 43 functions, mostly
getters/utility: `GetGameMode`, `GetGameInstance`, `GetGameHUD`, `GetCharacterFromController`,
`CastToCharacter`, `CastToController`, `GetLevellingComponent`, `GetPassiveSkillsComponent`,
`GetLockPickingComponent`, `GetSkillTreeManager`, `GetRadiationLevel`, difficulty/loot scaling
math (`LootDifficultySetting`, `LootTypeMultiplier`, `LootDegredationMultiplier`,
`ScalingDamageMultiplier`, `ScalingHealthMultiplier`, `XpMultiplierCalc`), and misc
(`ApplyMedicalEffect`, `ApplyRadiationDamage`, `ChangeDifficulty`, `CreateNotificationUI`,
`DestroyAllActors`, `GetRandomActorOfClass`, `IsKeyDown`, `LookForMapping`). Reference/utility
layer, not stateful.

## GameMacroLibrary_C / GameMacroLibraryPublic_C
`Blueprints/GameMacroLibrary.json`, `GameMacroLibraryPublic.json` — both empty in this export (0
functions, macros aren't exported as `Function` entries by FModel). Purpose unclear from export;
presumably Blueprint macro graphs used across the project, not inspectable via this method.

---

## Components/ (all `BaseComponent_C` subclasses unless noted)

Most attach to `BP_PlayerCharacter_C` (see its component list above) and follow a common pattern:
`ComponentLoaded`/`ComponentPreSave`/`LoadComponent` save-integration events, plus a
`Svr_Initialise` RPC on the base class.

| Class | Parent | Repl. props | RPCs | Notes |
|---|---|---|---|---|
| `BaseComponent_C` | — | 0 | `Svr_Initialise` (NetServer) | Base class for all below |
| `BatteryComponent_C` | BaseComponent | 0 | `Svr_Initialise` | `CheckBatteryLevel`, `UseBattery` |
| `BuildingComponent_C` | BaseComponent | 0 | `Event_LaunchBuildMode` (NetClient), `Svr_SpawnBuild` (NetServer) | Player-side building placement (ghost preview, snap) |
| `DamageComponent_C` | — | 0 | none | Generic health/damage: `SetHealth`, `DamageTaken`, `IsAlive?`, `Death__DelegateSignature` |
| `HungerThirstComponent_C` | BaseComponent | **4** | `UpdateHunger` (NetReliable+NetServer), `UpdateThirst` (NetServer) | Repl: `MaxHunger`, `CurrentHunger` (+SaveGame), `MaxThirst`, `CurrentThirst` (+SaveGame) |
| `LevellingComponent_C` | BaseComponent | 0 | `Event_LevelBarTimer` (NetMulticast) | XP/level system, `AddXP`, `LevelUp` |
| `LockPickingComponent_C` | BaseComponent | 0 | `Svr_Initialise` | Minimal — logic likely elsewhere (`BP_LockPickObject_C`) |
| `MedicalComponent_C` | BaseComponent | **4** | `Client_CreateMedUI`, `Client_RemoveEffect`, `Event_TimeToStopBleed/BrokenBone/HeavyBleed` (all NetClient); `Svr_Damage` (NetServer) | Repl (all `RepNotify`): `Bleed?`, `HeavyBleed?`, `BrokenBone?`, `RadiationSickness?`. Owns player Send*ToClient functions (health/hunger/oxygen/radiation/stamina/thirst) |
| `PassiveSkillsComponent_C` | BaseComponent | 0 | none | Skill XP tracking (FirstAid/Fishing/Fitness/Marksmanship/Reloading/Scavenging/Sneaking/Strength/Thief/Toughness), each with `AddXP_*`/`XPMultiplier_*` pair |
| `RadiationComponent_C` | BaseComponent | **2** | `CheckStats` (NetServer), `UpdateGeiger` (NetClient), `UpdateRadiation` (NetReliable+NetServer) | Repl: `MaxRadiation`, `CurrentRadiation` (+SaveGame) |
| `RadioComponent_C` | — | 0 | none | In-world radio playback, no RPCs — likely relies on replicated actor state elsewhere |
| `StaminaComponent_C` | BaseComponent | **3** | `Server_ToggleSprint` (NetReliable+NetServer) | Repl: `MaxStamina`, `CurrentStamina` (+SaveGame), `CurrentlySprinting` |
| `SwimmingComponent_C` | BaseComponent | **2** | none | Repl (`RepNotify`): `IsSwimming?`, `UnderWater?` |
| `TechTreeComponent_C` | — | 0 | none | Tech-tree unlock cost/points |
| `VehicleDrivingComponent_C` | BaseComponent | 0 | none | Vehicle input handling (many `InpActEvt_IA_Vehicle*` stubs, filtered) |
| `VehicleFuelComponent_C` | BaseComponent | 0 | none | `AddFuel`, `FuelSpend`, delegate `FuelUpdate__DelegateSignature` |
| `VehicleHealthComponent_C` | BaseComponent | 0 | none | `AddHealth`, delegate `FuelUpdate__DelegateSignature` (reused name, likely a copy-paste from FuelComponent) |
| `WorldObjectComponent_C` | — | 0 | none | Just 3 delegate signatures: `ObjectDamaged/Destroyed/Interacted` |

---

## Events/

| Class | Parent | Notes |
|---|---|---|
| `BP_EventManager_C` | `BP_EventMaster_C` | Central random-event dispatcher. RPCs: `AirdropSpawn`, `BloodMoonEvent`, `DynamicRandomEventSpawn`, `Explosion_Spawn` (all NetServer), `Jet_Spawn` (NetMulticast). Delegate `BloodMoon__DelegateSignature` |
| `BP_EventMaster_C` | — | Empty base (0 functions) — pure base class for event actors |
| `AirdropAircraft_C` | `BP_MasterObject_C` | Repl (1 prop): `Airdrops` (ArrayProperty). RPC: `MC_Lights` (NetMulticast) |
| `BP_Event_AirdropGoToLocation_C` | `BP_EventMaster_C` | Repl: `RegionRadius`. Just a `UserConstructionScript` |
| `BP_Event_JetActor_C` | `BP_EventMaster_C` | No RPCs, just `ReceiveTick` |
| `Jet_Fighter_Fly_By.json`, `samolety_039__2_.json` | — | Non-Blueprint assets (0 entries relevant) caught by the folder glob — skip |

---

## Interfaces/

Pure interface function lists (no implementation, no properties, by definition). All functions
`FUNC_Public | FUNC_BlueprintCallable | FUNC_BlueprintEvent` (interface calls aren't RPCs
themselves — the implementing class's override may or may not be).

- **`BPI_Damage_C`**: `Damage_Object`, `Damage_Shoved`, `IsObjectDamageable?`
- **`BPI_FishingRod_C`**: `CastRod`, `CatchFish`, `StartFishing`, `StopFishing`
- **`BPI_Lockpick_C`**: `LockpickSuccessful`
- **`BPI_Player_C`**: the player-facing contract — `GetPlayerRef`, `GetInGameUI`,
  `CreateNotificationUI`, `PlayerDeath`, `PlayMontage`/`StopMontage`, `SetBurning`/`IsBurning?`,
  `Send{Health,Hunger,Oxygen,Radiation,Stamina,Thirst}ToClient`, `IsGPSEquipped?`,
  `IsPlayerInVehicle?`, `VehicleInteraction`, `UpdatePlayerSpeed`, `SetTraceToWorld` — this is the
  interface `MedicalComponent_C` and `BP_PlayerCharacter_C` both implement to talk to each other
  generically
- **`BPI_UI_C`**: `Get/Show{Flashlight,NightVision,Respirator}Durability` — HUD durability display
- **`BPI_WorldObjectPower_C`**: `CallInteractOnObject`, `SetPower_TurnedOn`, `SetPower_Warning`
- **`BP_PlayerControllerInterface_C`**: just `CameraShake`
- **`Int_PassiveSkills_C`**: mirrors `PassiveSkillsComponent_C`'s `AddXP_*`/`XPMultiplier_*` pairs
  — the interface `PassiveSkillsComponent_C` implements

---

## MasterObjects/

- **`BP_MasterObject_C`**: base class for placeable world objects, 0 functions of its own (base
  hook point only — 1 property, no RPCs). `AirdropAircraft_C`, `BP_AudioPoint_C`,
  `BP_LockPickObject_C` all derive from it.
- **`BP_AudioPoint_C`**: derives `BP_MasterObject_C`. Just `ReceiveBeginPlay` +
  `UserConstructionScript` — ambient audio trigger volume, no gameplay logic.
- **`BP_LockPickObject_C`**: derives `BP_MasterObject_C`. `LockpickSuccessful` (implements
  `BPI_Lockpick_C`) + `ReceiveBeginPlay`.
- **`BP_MasterDoor_C`**: 0 functions, purpose unclear from export beyond "door base class" (1
  property only, no RPCs visible at this level — actual door logic likely in a subclass not in
  scope, e.g. under `Other/Doors/`).

---

## Player_AnimBP.json (Player_AnimBP_C — AnimInstance)

Not a `BlueprintGeneratedClass` in the usual actor sense (an AnimInstance), so no `Super`/replicated
property data extracted the same way. 35 functions, mostly per-node `EvaluateGraphExposedInputs_*`
IK-solver plumbing (Fabrik/DragonFeetSolver/DragonSpineSolver/TransitionResult — filtered mentally,
listed below only if gameplay-relevant) plus:
- `BlueprintThreadSafeUpdateAnimation`, `BlueprintBeginPlay`, `AnimGraph` — core per-frame update
- `GetAnimationInfoFromCharacter` — pulls state from the owning `BP_PlayerCharacter_C` each frame
  (this is the anim-side half of `BP_PlayerCharacter_C::GetAnimationInfo`)
- `CombatState`, `DeathState` — state-machine drivers
- `GetSpeed&Direction`, `GetLean`, `GetAimOffset`, `GetHeadRot`, `GetLeftHandLoc`,
  `GetThreadSafeBooleans`, `GetVehicleVars` — locomotion/IK inputs
- `AnimNotify_Footstep`, `AnimNotify_Swim`, `AnimNotify_SwimFast` — anim notify handlers

No RPCs (AnimInstances don't have their own network role independent of the owning actor). No
appearance/clothing-specific functions found here — this file only drives skeletal *animation*,
not which meshes are attached, consistent with the appearance-mesh logic living on
`BP_PlayerCharacter_C`/native code instead.
