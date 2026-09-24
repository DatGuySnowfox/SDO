# mod.cpp ProcessEvent parameter audit (2026-08-15)

Read-only audit of every `ProcessEvent` call site in `src/mod.cpp` (~5005 lines),
comparing hand-written `Params` structs against real UFunction signatures found
in `research/CXXHeaderDump/*.hpp` and, where relevant, `decoded_*.txt` Kismet
bytecode dumps.

## Mismatches found

### 1. `K2_SetActorRotation` — missing return-value field (line ~3717-3720)

```cpp
struct SetRotParams { FRotator NewRotation; bool bTeleportPhysics; } rotParams{};
rotParams.NewRotation      = newRot;
rotParams.bTeleportPhysics = true;
pawn->ProcessEvent(setRotFn, &rotParams);
```

Real signature (`research/CXXHeaderDump/Engine.hpp:8550`):

```cpp
bool K2_SetActorRotation(FRotator NewRotation, bool bTeleportPhysics);
```

`K2_SetActorRotation` returns `bool`. Kismet appends the return value as an
implicit trailing parameter in the packed struct, exactly like the sibling
call three lines above it in the same function (`SetLocParams`, which DOES
correctly include a trailing `bool ReturnValue` matching `K2_SetActorLocation`'s
real signature). `SetRotParams` omits this trailing byte entirely, so the
struct passed to `ProcessEvent` is one byte short of what the function expects
to write into. When the engine writes the return value, it writes one byte
past the end of `rotParams` on the stack — adjacent stack memory (e.g. the
next local, `postCount`, or saved registers depending on codegen) gets
clobbered. This is the same *class* of bug as tonight's two confirmed fixes
(wrong struct shape passed to a native call), on a call site the comment
itself flags as "HIGHER RISK than any other change tonight" and "not yet
live-verified" — this runs on the local player's own pawn on every join-time
teleport.

**Fix:** add a trailing `bool ReturnValue = false;` field to `SetRotParams`,
matching the pattern already used correctly in `SetLocParams` immediately
above it.

## Verified correct

- `GetActiveWeaponSlot` (line 528/534) — `RawFGameplayTag` (8-byte
  ComparisonIndex+Number) return value; matches FGameplayTag raw layout.
- `GetControlRotation` (line 602/608) — `FRotator ReturnValue`; APawn native,
  no params, single struct return.
- `GetAnimInstance` (lines 656/658, 1826/1828, 1906/1908, 1977/1979,
  2881/2883, 3150/3152, 4810/4812) — all identical `UObject* ReturnValue`
  single-pointer pattern; USkeletalMeshComponent::GetAnimInstance() returns a
  single UAnimInstance* with no other params.
- `GetOwner` (line 788/790) — single `AActor*` out-param, matches
  `Engine.hpp:8593`/`10636` `AActor* GetOwner()`.
- `UpdateBodyParts` (line 2264/2300) — real signature is
  `void UpdateBodyParts(FName Name)` (`BP_PlayerCharacter.hpp:327`).
  `struct Params { int32_t ComparisonIndex; int32_t Number; }` is the raw
  FName layout (ComparisonIndex+Number, 8 bytes) — structurally correct even
  though field names don't literally say "FName".
- Clothing `OnRep_...` calls (line 2311/2313) — called with `nullptr` params;
  these are parameterless OnRep callbacks, correct.
- `GetCurrentActiveWeapon` (line 2798/2800) — real signature
  `void GetCurrentActiveWeapon(AActor*& EquippedWeapon)`
  (`BP_PlayerCharacter.hpp:365`); matches single-pointer out-param struct.
- `SetIsSpawningStopped` (line 3258/3260) — real signature
  `void SetIsSpawningStopped(bool Stop)` (`BP_AISpawner_Master.hpp:67`);
  matches. Called only on `BP_AISpawner_{Zombies,ZombieHounds,ZombieBosses}_C`
  instances, all subclasses of `BP_AISpawner_Master`, so the 1-bool overload
  (not the 0-arg `BP_AISpawningVolume_*` variant) is the correct one.
- `KillSpawnedActors` (line 3264/3266) — real signature (on
  `BP_AISpawner_Master`) `void KillSpawnedActors(bool AllowRespawn)`
  (`BP_AISpawner_Master.hpp:88`); matches, same class-targeting reasoning as
  above.
- `Equip Actor to Socket` (line 3353/3396, two call sites 3406/3425) — real
  signature `void Equip Actor to Socket(AActor* ActorRef, bool IsSecondary)`
  (`BP_JigHelperComp.hpp:57`, confirmed also via `decoded_EquipActorToSocket.txt`).
  `struct EquipParams { AActor* ActorRef; bool IsSecondary; }` matches field
  order/types/arity exactly. (This is the same function whose ScaleRule-arg
  sibling, `K2_AttachToComponent`, had the confirmed bug tonight in
  `proxy_manager.cpp` — but that bug is in a different function entirely;
  `Equip Actor to Socket` itself is correct at both mod.cpp call sites.)
- `K2_SetActorLocation` (line 3695/3711-3715) — real signature
  `bool K2_SetActorLocation(FVector NewLocation, bool bSweep, FHitResult&
  SweepHitResult, bool bTeleport)` (`Engine.hpp:8555`).
  `SetLocParams{FVector NewLocation; bool bSweep; FHitResult SweepHitResult;
  bool bTeleport; bool ReturnValue;}` matches field order, types, and includes
  the trailing return-value bool. Correct.
- `GetSkeletalMeshComponent` / `K2_GetRootComponent` fallback (line
  1668-1671) — both single-pointer-return functions; `UObject* root` out
  param matches either.
- `GetItemID` (line 4055/4060-4061) — real signature
  `void GetItemID(FName& ItemId)` (`JSI_Slot.hpp:141`); the anonymous
  `{int32_t ComparisonIndex, Number}` struct is the raw FName layout, correct.
- `GetOwningActor` (line 4828/4830-4831) — real signature
  `AActor* GetOwningActor()` (`Engine.hpp:11001`); `OwnerParams{AActor*
  ReturnValue}` matches.
- Generic user-driven "call ClassName.FuncName()" debug command (line
  3061-3067) and MenuWidget `ContinueGame` button-click delegate (line
  3089-3099) — both invoke with `nullptr` params; the button-click delegate
  is a standard parameterless `OnButtonClickedEvent` signature, correct as
  written. (The generic debug "call" command's target function is
  user-specified at runtime and inherently unverifiable at audit time — see
  below.)

## Unverifiable — no reference found

- Generic runtime "call &lt;ClassName&gt; &lt;FuncName&gt;" debug command
  (line 3061-3067): always calls with `nullptr` params regardless of what
  function is named at runtime. This is correct ONLY if the operator only
  ever targets genuinely parameterless functions; there is no way to audit
  this generically since the target isn't fixed in source. Flagging as a
  standing risk for misuse rather than a concrete bug.
- `bytecode_dump` utility (line ~1506): resolves arbitrary widget/function by
  name for raw bytecode extraction only; does not call `ProcessEvent` with a
  constructed params struct, so there's nothing to check against a real
  signature.
- `BuildTransformParams` (line 4085) and the `handle_pickup_hook`/
  `handle_drop_hook` param readers (lines ~4173-4185): these read INCOMING
  hook parameter blocks (pre-callback interception of a native call already
  in flight), not outgoing `ProcessEvent` calls this project constructs —
  out of scope for this audit's "hand-written Params struct passed to
  ProcessEvent" focus, but worth a follow-up pass if hook-param
  misinterpretation is ever suspected as a bug source.

## Summary

Call sites audited: 20 distinct `ProcessEvent` invocations (some functions
called from multiple sites, e.g. `Equip Actor to Socket` at 3 sites,
`GetAnimInstance` at 7 sites) across the file.

- Mismatches found: **1** — `K2_SetActorRotation`'s `SetRotParams` struct
  (line ~3717) is missing the trailing `bool ReturnValue` field the real
  `bool K2_SetActorRotation(FRotator, bool)` signature requires, causing a
  1-byte stack overwrite on every join-time teleport call.
- Verified correct: 15 functions / call-site groups.
- Unverifiable: 3 (generic runtime "call" debug command, bytecode-dump
  utility, and incoming-hook param readers that aren't outgoing ProcessEvent
  calls).
