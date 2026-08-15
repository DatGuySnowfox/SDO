# Bytecode/header audit of src/proxy_manager.cpp ProcessEvent call sites

Audited every `ProcessEvent(...)` call site in `src/proxy_manager.cpp` (32 call
sites, some sharing an identical pattern e.g. repeated `SetVisibility` calls).
Each was checked against either the real native function's declared signature
in `research/CXXHeaderDump/*.hpp`, or a decoded Kismet bytecode dump in
`%APPDATA%/SurrounDeadBridge/decoded_*.txt` for Blueprint-only functions.

Two real bugs (SetSkinnedAssetAndUpdate bReinitPose, K2_AttachToComponent
ScaleRule) were already found and fixed earlier tonight and are visible in the
current source as corrected — call sites at lines 1404-1409, 2353,
1305-1313 (spawn_and_attach_weapon_attachment) confirmed now matching the
reference shape.

## Mismatches found

### 1. `do_apply_proxy_actor_rotation` — `K2_SetActorRotation`, line 2739-2751

```cpp
struct ActorRotationParams { FRotator NewRotation; bool bSweep; };
```

Real signature (`research/CXXHeaderDump/Engine.hpp:8550`):

```cpp
bool K2_SetActorRotation(FRotator NewRotation, bool bTeleportPhysics);
```

The function **returns bool**, but `ActorRotationParams` has no `ReturnValue`
field, unlike every other bool-returning ProcessEvent call in this file
(`K2_AttachToComponent`, `SetStaticMesh` both explicitly declare
`bool ReturnValue`). `ProcessEvent` will write the return value into the
params buffer at the offset immediately after `bSweep` (offset 0x19).

Severity: **low / likely benign, but a real structural mismatch.** Because
`FRotator` is 24 bytes (3×double, no native padding) and the struct's own
C++ layout pads `{FRotator; bool;}` up to a 32-byte, 8-byte-aligned size, the
return byte at offset 0x19 lands inside the struct's own trailing padding
(bytes 0x19-0x1F), not past the end of `sizeof(ActorRotationParams)`. So this
specific case does not appear to actually corrupt adjacent stack memory today
— but it is fragile (relies on incidental compiler padding rather than an
explicit, verified layout) and inconsistent with this project's own stated
convention elsewhere in the same file. Also note: the field is named
`bSweep` in code/comments but the real parameter is `bTeleportPhysics` —
purely a naming issue (Kismet packs positionally, not by name), not a
functional bug, but worth aligning for future readers.

**Recommended fix:** add an explicit `bool ReturnValue = false;` field after
`bSweep` (rename to `bTeleportPhysics` for clarity) with a `static_assert` on
offsets, matching the pattern used for every other bool-returning call in
this file.

## Verified correct

- `dump_clothing_table` → `GetDataTableRowNames(UDataTable*, TArray<FName>&)` (Engine.hpp:13126) — matches.
- `get_equipped_info_by_slot` → `GetEquippedInfoBySlot(FGameplayTag, FRepItemInfo&, bool&)` (BP_JigHelperComp.hpp:63) — matches, offsets asserted.
- `set_equipped_info_by_slot` → `SetEquippedInfoBySlot(FGameplayTag, FRepItemInfo, FGuid, bool)` (BP_JigHelperComp.hpp:64) — matches, offsets asserted.
- `set_active_weapon_slot` → `SetActiveWeaponSlot(FGameplayTag)` (BP_JigHelperComp.hpp:54) — matches.
- `equip_actor_to_socket` → `Equip Actor to Socket` (`(AActor*, bool)`) — matches decoded bytecode reference (already the site of the fixed ScaleRule bug's sibling checks); ActorRef/IsSecondary order and offsets correct.
- `call_combat_state` → `GetAnimInstance()` (Engine.hpp:22051, no params, returns UObject*) and `CombatState(int32)` — both match; CombatState's simple body confirmed via decoded bytecode per existing code comment.
- `call_on_rep_active_weapon` → `OnRep_ActiveWeapon()` — no params, matches (nullptr passed).
- `get_current_active_weapon` → `GetCurrentActiveWeapon(AActor*&)` (BP_PlayerCharacter.hpp:365) — matches.
- `get_helper_active_weapon` → `GetActiveWeapon(AActor*&)` (BP_JigHelperComp.hpp:40) — matches.
- `get_helper_equipped_actor_by_slot` → `GetEquippedActorBySlot(FGameplayTag, bool, AActor*&, int32&)` (BP_JigHelperComp.hpp:53) — matches, offsets asserted.
- `set_pickup_item_data` → `SetCount(int32)` (BP_JigPickupComponent.hpp:51) — matches.
- `reassert_no_interact` → `JigSetCanInteract(bool, bool, bool&)` (BP_SkeletalMeshPickup.hpp:28, identical across all Jigsaw-interactable classes) — matches.
- `spawn_and_equip_item_visual`: `K2_GetActorLocation()` (Engine.hpp:8562, FVector return, no params) — matches; `SetSimulatePhysics(bool)` (Engine.hpp:21007) — matches; `K2_AttachToComponent` backpack re-attach — matches reference shape (LocationRule/RotationRule/ScaleRule=1/WeldSimulatedBodies=true); `K2_GetComponentToWorld()` (Engine.hpp:21743, FTransform return) — `NativeFTransform` layout (32/32/32-byte quat+loc+pad+scale+pad) matches `FTransform`'s real 0x60-byte layout with its alignment padding (CoreUObject.hpp:754), confirmed byte-for-byte.
- `spawn_and_attach_weapon_attachment`: `Jig_SetAttachmentInfo(FGuid, UJigsawItem_DataAsset_C*, bool&)` (BP_AMainLocalAttachment.hpp:19) — matches; `K2_AttachToComponent` — matches (this is the already-fixed ScaleRule bug, now correct).
- `equip_clothing_to_mesh` → `SetSkinnedAssetAndUpdate(UObject*, bool)` — matches now-fixed reference shape (bReinitPose=false); `SetVisibility(bool, bool)` (Engine.hpp:21723) — matches.
- `call_on_rep_primary_weapon_equipped` → `OnRep_PrimaryWeaponEquipped?()` — no params, matches.
- `call_on_active_weapon_slot_changed` → `OnActiveWeaponSlotChanged_Event_0(FGameplayTag)` (BP_PlayerCharacter.hpp:862) — matches.
- `sync_equipment`'s repeated `SetVisibility(bool,bool)` calls (gloves/hands hide-show, clothing clear) — all match.
- `reattach_weapon_visual_to_socket` → `GetSkeletalMeshComponent`/`K2_GetRootComponent` fallback + `K2_AttachToComponent` — matches reference shape.
- `sync_active_weapon_hand`'s `findItemRoot` lambda → `GetSkeletalMeshComponent`/`K2_GetRootComponent` — matches (defensive fallback; see note below).
- `sync_pawn_appearance`: `SetVisibility` (all instances) — matches; `SetStaticMesh(UStaticMesh*)` (Engine.hpp:23089, returns bool) — matches, includes ReturnValue field; `SetMaterial(int32, UMaterialInterface*)` (Engine.hpp:21024) — matches, including both the hair/beard-color and skin-color loop call sites; `SetSkinnedAssetAndUpdate` (body-part mesh loop) — matches now-fixed reference shape.
- `on_play_montage` → `PlayMontage(UAnimMontage*, double)` (BPI_Player.hpp:17 / BP_PlayerCharacter.hpp:969 / MedicalComponent.hpp:41, all identical) — matches.
- `do_apply_proxy_speed` → `GetAnimInstance()` — matches.

## Unverifiable — no reference found

- `GetSkeletalMeshComponent` (queried as the first choice at 4 call sites:
  `spawn_and_equip_item_visual`, `spawn_and_attach_weapon_attachment`,
  `reattach_weapon_visual_to_socket`, `sync_active_weapon_hand`'s
  `findItemRoot`) does not appear anywhere in the 2407-file
  `research/CXXHeaderDump/` tree under any class — it is not a real UFUNCTION
  name in this codebase's header dump. Not flagged as a mismatch because the
  code already treats it defensively (`GetFunctionByNameInChain` returning
  null falls back to `K2_GetRootComponent`, which *is* verified,
  Engine.hpp:8559), so this is functionally safe, just worth noting the
  primary lookup is presumably always a no-op/dead branch on this build.

## Summary

32 ProcessEvent call sites audited. 1 mismatch found (K2_SetActorRotation
missing an explicit ReturnValue field — low severity, appears memory-safe due
to incidental struct padding, but structurally inconsistent with the rest of
the file and worth fixing for robustness). 30 verified correct against either
Engine.hpp/class-specific headers or decoded bytecode. 1 call target
(GetSkeletalMeshComponent) has no reference anywhere in the header dump,
though it's used defensively with a verified fallback so it isn't a
functional risk.
