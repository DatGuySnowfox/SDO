# Animations / Input / WIP Catalog (FModel export analysis)

Source: `Exports/SurrounDead/Content/Animations/**` (268 files), `Exports/SurrounDead/Content/Input/**` (65 files), `Exports/SurrounDead/Content/WIP/**` (82 files).
Export is flags-only (no bytecode) — function bodies are unknown, only name/flags/params exist.

**Summary:** Animations has a small, useful set of real gameplay classes — four Player AnimNotifyStates (melee hit detect x2, shove, stomp) and three Zombie/ZombieBoss attack AnimNotifyStates, plus one tiny interface (`INT_AnimBPPlayer`); everything else in that folder is inert AnimSequence/AnimMontage/BlendSpace data. Offset-enrichment note: matching `.hpp` files exist in `CXXHeaderDump/` for all 8 AnimNotifyState classes and `INT_AnimBPPlayer` (e.g. `AnimNotify_MeleeHitDetect.hpp`), but every one of them declares **zero member properties** — just the `Received_NotifyBegin`/`Received_NotifyEnd` function signatures (or, for the interface, its two trivial functions). There is nothing to annotate with byte offsets in this file; confirmed by reading the headers rather than assumed. No `Player_AnimBP`-equivalent AnimBP graph exists outside the one already cataloged elsewhere. Input is confirmed pure data — 64 `InputAction`/`InputMappingContext` assets with zero `Function` entries, nothing to reverse here. **WIP is confirmed dead/unused art content**: all 82 files are SkeletalMesh, StaticMesh, Texture, Material, and PhysicsAsset assets (a wandering-trader NPC's mesh/backpack, a ghillie suit clothing set, city/power-plant/shooting-range props, MP5/USP weapon meshes) — not one file contains a `BlueprintGeneratedClass` or `Function` entry, so there is no logic, Blueprint, or gameplay system to document; it's raw unused/placeholder art, not an unfinished system.

---

## Animations

### AnimNotify_MeleeHitDetect (`Animations/Notifies/Player/AnimNotify_MeleeHitDetect.json`)
AnimNotifyState on player melee montages — active window for melee hit detection.
| Function | Flags |
|---|---|
| Received_NotifyBegin | Event, Public, HasOutParms, HasDefaults, BlueprintCallable, BlueprintEvent, Const |
| Received_NotifyEnd | Event, Public, HasOutParms, BlueprintCallable, BlueprintEvent, Const |

No net flags. Graph calls `GetOwner`/`GetComponentByClass`(`BP_JigHelperComp_C`, `BP_WeaponsPickupComponent_C`)/`GetEquippedActorBySlot`/`K2_SetTimer` — looks up the player's equipped weapon and jig component to run hit-scan/timer logic locally (likely client or owning-actor side; no server RPC present at this layer — actual damage application would be elsewhere, e.g. `DamageComponent`).

### AnimNotify_MeleeHitDetect_Power (`Animations/Notifies/Player/AnimNotify_MeleeHitDetect_Power.json`)
Same shape/purpose as above (power-attack variant); identical call pattern (GetOwner, GetComponentByClass x2, GetEquippedActorBySlot, K2_SetTimer). No net flags.

### AnimNotify_Shove (`Animations/Notifies/Player/AnimNotify_Shove.json`)
AnimNotifyState for the shove action. `Received_NotifyBegin`/`Received_NotifyEnd`, both non-net. Casts owner to `BP_PlayerCharacter_C` and sets a timer — purely local logic gating the shove window.

### AnimNotify_Stomp (`Animations/Notifies/Player/AnimNotify_Stomp.json`)
Same pattern as Shove — casts to `BP_PlayerCharacter_C`, sets a timer. No net flags.

### AnimNotify_ZombieAttackGeneral / AnimNotify_ZombieAttackLeftArm / AnimNotify_ZombieAttackRightArm (`Animations/Notifies/Zombie/*.json`)
AnimNotifyStates on zombie attack montages, casting owner to `BP_MasterZombie_C` and running a timer window (identical shape across all three — General/LeftArm/RightArm are just different attack-window variants keyed to different animations/hitboxes). No net flags on any.

### AnimNotify_ZombieBossAttackRightArm (`Animations/Notifies/ZombieBoss/AnimNotify_ZombieBossAttackRightArm.json`)
Same shape as the regular zombie attack notifies, for boss variants. No net flags.

### INT_AnimBPPlayer (`Animations/Interfaces/INT_AnimBPPlayer.json`)
Blueprint Interface with two trivial functions: `CombatState(int BlendSpace)` and `DeathState(bool Dead)`. Both plain `BlueprintCallable|BlueprintEvent`, no net flags — pushes combat/death blend state into the player AnimBP graph, presumably called locally per-instance as animation state replicates via other means (pose/bool properties), not via this interface.

### Everything else (~260 files)
AnimSequence, AnimMontage, BlendSpace/BlendSpace1D, AnimCurveMetaData, and skeletal-mesh compression settings across `Anims/`, `Player/`, `Zombie/`, `Compression/` — pure data assets, no `Function` entries, nothing to reverse. No AnimBP graph asset (e.g. a `Player_AnimBP`-equivalent `AnimBlueprintGeneratedClass`) exists in this scope.

---

## Input

64 `InputAction` (`IA_*`) files across `Combat/`, `InventoryPlayerActions/`, `Movement/`, `Other/`, `Vehicle/`, plus one `InputMappingContext` (`IMC_General.json`). Confirmed via full-folder grep: **zero files contain a `"Type": "Function"` entry** — these are pure Enhanced Input data assets (trigger/modifier config only), no Blueprint logic anywhere in this folder.

---

## WIP

82 files, all art/data assets — **no Blueprint classes, no Functions, no net-flagged anything** anywhere in the folder (verified via full grep for `"Type": "Function"` and `FUNC_Net`, zero hits). Contents:
- `AI/WanderingTrader/` — `WanderingTrader.json` (SkeletalMesh) + `Backpack_WanderingTrader.json` (StaticMesh + BodySetup) for an apparently-unimplemented wandering-trader NPC. No AI Blueprint, controller, or logic asset accompanies the mesh anywhere in the export — just the character/prop meshes.
- `Ghillie/` — full ghillie-suit clothing set (boots/gloves/helmet/mask/pants/shirt/vest, x2 color variants, x2 genders) as skeletal meshes/physics assets/skeletons/textures. Likely an unreleased cosmetic set.
- `City/`, `PowerPlant/`, `ShootingRange/` — static meshes, materials, textures for building/prop dressing (corner building, front building, power-plant computer/copier props, BBQ shack, gun range/store buildings and furniture).
- `Weapon_MP5*` / `Weapon_USP*` — weapon skeletal meshes/physics/skeletons, no accompanying weapon Blueprint or pickup logic in this folder.
- `ZombieBoss_*.json` (Butcher/Infestation/Jugger/Scientist/Survivor) + `M_ZombieBoss.json`/`BossTexture.json` — boss skin materials/textures only (the actual `BP_ZombieBoss`/`BP_MasterZombie` logic lives under `AI/`, out of this scope, already cataloged elsewhere).

**Conclusion: WIP is confirmed dead/unused staging content — art assets for features (wandering trader NPC, ghillie cosmetics) that were never wired to any Blueprint logic present in this export.** Nothing here is live or relevant to networking/RPC work.
