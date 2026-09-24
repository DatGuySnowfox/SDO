# Bytecode Decode Batch 1

Decoded 2026-08-15 from raw `.bin` captures in
`C:\Users\mccau\AppData\Roaming\SurrounDeadBridge\`, using
`research/bytecode/kismet_disasm.py`. All 11 files decoded cleanly (no
`STOPPED: unhandled opcode` in any output). None of the decoded functions
contain FName `ci=` values matching the known constants (Torso=1732710,
Legs=1732718, Feet=1732721) — grepped for all three across every file, zero
hits.

Caveat that applies to every entry below: this disassembler resolves raw
`UFunction*`/`UProperty*` pointers as bare hex addresses, not symbol names
(only `EX_NameConst`/`EX_VirtualFunction` FName references get a `ci=`
comparison-index). So for the large Ubergraph event graphs in particular,
"what it calls" can only be described structurally (how many branches, what
literal constants get passed), not by function name, unless a call already
happened to be identified in a prior session.

## BP_PlayerCharacter_C

### ExecuteUbergraph_BP_PlayerCharacter
Decoded: `decoded_BP_PlayerCharacter_C_ExecuteUbergraph_BP_PlayerCharacter.txt` (48,296 lines, ~fully decoded)

This is the character's entire compiled event graph (every event node's logic
concatenated into one function, standard for Blueprints) — 132
`PushExecutionFlow`, 526 `JumpIfNot` branches, 417 `FinalFunction` calls, 1182
Virtual/LocalVirtualFunction calls, 1263 CallMath calls, 8
CallMulticastDelegate invocations. Far too large to narrate call-by-call from
raw pointers alone; treated as a structural inventory rather than a full
narrative. No known `ci=` FName constants appear in it. Worth a targeted
future pass (grep for a specific hex offset or new `ci=` once we know which
constant we're hunting) rather than a blind read.

### GetAnimationInfo
Decoded: `decoded_BP_PlayerCharacter_C_GetAnimationInfo.txt` (710 bytes, fully decoded)

Interface-based animation query. Calls a bool-returning CallMath twice (looks
like an "IsValid"-style guard) gating two `LocalVirtualFunction FName(ci=1848673)`
calls on an instance variable — same FName hash both times, so this looks like
the same accessor called from two branches (a validity check pattern, not two
different calls). If the guard passes, it casts the result through
`EX_ObjToInterfaceCast` then `EX_Cast conv_type=2`, and if that succeeds calls
a 4-parameter `LocalVirtualFunction FName(ci=1846309)` through an
`InterfaceContext` — i.e. it's querying an interface implemented on some
object (likely the equipped item/weapon) for animation data. Falls through to
a second path (`ci=1848682`) that reads a bool off a global-ish instance var
at `0x3e046280` via `VirtualFunction FName(ci=138196)`, then packs several
output params (a float 0.0, two bools, an object ref) before returning. The
two `ci=1848673`/`ci=1846309`/`ci=1848682` FNames aren't in the known-constant
list — flagged as candidates for a `resolve_fname.flag` pass since they gate
real animation-info branching logic.

### MC_ADS
Decoded: `decoded_BP_PlayerCharacter_C_MC_ADS.txt` (18 bytes, fully decoded)

Trivial stub: single `EX_LocalFinalFunction` call passing one literal
`EX_IntConst 173204` as the only parameter, then return. No branching, no
property reads. This is almost certainly a "multicast RPC trampoline" pattern
— a raw multicast/replicated function that just forwards to a native/BP
function using a hardcoded integer literal as its argument. **Flagged**: a
single raw IntConst passed positionally with no visible type context is
exactly the shape called out in the task brief as worth a future
signature-mismatch audit (same shape as the equip-bug native-call patterns
already found this session) — the literal 173204 could be an enum value, an
FName hash truncated to int, or a genuinely mismatched argument slot. Given
ADS is live/extensible gameplay, this is worth resolving what `func=0x135f4e400`
actually is before extending ADS behavior.

### OnActiveWeaponSlotChanged_Event_0
Decoded: `decoded_BP_PlayerCharacter_C_OnActiveWeaponSlotChanged_Event_0.txt` (36 bytes, fully decoded)

Same trampoline shape as MC_ADS: writes a local variable onto the persistent
frame (`EX_LetValueOnPersistentFrame`), then calls one `LocalFinalFunction`
with a single literal `EX_IntConst 164582` parameter. Structurally this looks
like a Blueprint event-dispatcher stub forwarding "weapon slot changed" to a
handler using a raw int constant — flagged alongside MC_ADS as a
signature-mismatch audit candidate, since 164582 as a positional single-int
argument is the same suspicious shape.

## Jigsaw / Inventory System

### BP_JigHelperComp_C_OnRep_RepActorsData
Decoded: `decoded_BP_JigHelperComp_C_OnRep_RepActorsData.txt` (22 bytes, fully decoded)

Trivial OnRep stub: fires one multicast delegate (`EX_CallMulticastDelegate`)
off an instance-variable delegate property, no parameters, then returns. This
just broadcasts "RepActorsData changed" to whatever's bound — the actual sync
logic lives in the delegate's bound handlers, not here. Nothing further to
extract from this function alone.

### BP_JigMultiplayer_C_ExecuteUbergraph_BP_JigMultiplayer
Decoded: `decoded_BP_JigMultiplayer_C_ExecuteUbergraph_BP_JigMultiplayer.txt` (5,749 lines, fully decoded)

Same "concatenated event graph" pattern as the PlayerCharacter Ubergraph but
smaller: 20 PushExecutionFlow, 46 JumpIfNot, 67 FinalFunction, 204
Virtual/LocalVirtualFunction, 123 CallMath, 1 CallMulticastDelegate. This is
the Jigsaw multiplayer-sync component's full event logic (item equip/drop/
pickup coordination). No known-constant `ci=` hits. Given its role
(multiplayer item sync — directly adjacent to the equip-bug investigation),
this is a strong candidate for a deeper follow-up pass focused specifically
on branches around whatever functions eventually call `Svr_AttachClothing`/
`MC_AttachClothing`, but a full manual walk of 5,749 lines was out of scope
for this batch.

### BP_JigMultiplayer_C_ItemDropRequest_Event_0
Decoded: `decoded_BP_JigMultiplayer_C_ItemDropRequest_Event_0.txt` (72 bytes, fully decoded)

Trampoline stub, same shape as MC_ADS/OnActiveWeaponSlotChanged: three
`EX_LetValueOnPersistentFrame` writes (packing 3 local args onto the
persistent frame — presumably item ref, count, drop location/actor) followed
by one `LocalFinalFunction` call with a single literal `EX_IntConst 10278`
parameter. **Flagged** as another instance of the raw-single-int trampoline
pattern worth a future signature audit — this is the entry point of the
item-drop path the task specifically asked about, and it shares the exact
suspicious shape (hardcoded int, no visible parameter typing) seen in the
equip-side bugs.

### BP_WeaponsPickupComponent_C_ExecuteUbergraph_BP_WeaponsPickupComponent
Decoded: `decoded_BP_WeaponsPickupComponent_C_ExecuteUbergraph_BP_WeaponsPickupComponent.txt` (325 bytes, fully decoded)

Small, fully linear (no branches) construction-style routine. Two near-
identical blocks: get/construct an object via `FinalFunction func=0x23604060`
(likely a component getter, e.g. "GetOwner" or similar, called with no args),
then read a field off it via `FinalFunction func=0x24d33880`, then call
`VirtualFunction FName(ci=90211)` passing an `EX_NameConst` — first block uses
`ci=115005`, second uses `ci=3098` — before storing the result into an
instance variable. `ci=90211` recurs in several other files below with
different NameConst arguments each time, which strongly suggests it's a
generic "find/get by tag or name" function (e.g. `FindComponentByTag` or
similar) being called with two different tag names. Neither `ci=115005` nor
`ci=3098` matches a known constant — flagged for resolution, since they're
the "which tag" argument passed into a generic lookup, and knowing what they
resolve to would identify what this component fetches (looks like it's
locating two named sub-objects, plausibly weapon mesh/socket references, on
construction).

### JSIContainer_C_OnDrop
Decoded: `decoded_JSIContainer_C_OnDrop.txt` (9,137 bytes / 2,155 lines, fully decoded)

Large branchy function: 5 PushExecutionFlow, 33 JumpIfNot, 4 FinalFunction,
71 Virtual/LocalVirtualFunction calls, 63 CallMath, 4 CallMulticastDelegate.
Starts with a `DynamicCast`/`Cast` pair gating most of the logic behind an
`if NOT valid then skip` pattern — typical "is the dropped-on widget/actor the
right type" guard before proceeding. Ends by returning a bool
(`EX_LocalOutVariable`) after calling a function with 5 params built from
locally-tracked drop-position/index variables. This is the UI drag-and-drop
entry point for the inventory grid; the real business logic (moving/stacking
items) is delegated out to `PerfromDrop` (see below) and other called
functions rather than being inline here. No known-constant `ci=` hits;
too large for a full call-by-call narrative in this pass.

### JSIContainer_C_PerfromDrop
Decoded: `decoded_JSIContainer_C_PerfromDrop.txt` (2,265 bytes, fully decoded)

This is the actual drop-resolution logic (note: "Perfrom" is a typo in the
game's own function name, not a transcription error here). Structure:
validates two type checks via `CallMath` (`func=0xd0afe40`, `func=0x480c4300`
called twice) then combines them with boolean AND-style calls
(`func=0x4806d8a0`, `func=0x4806db40`) — this is a multi-condition guard
(likely: "is target slot valid AND is target slot different from source AND
compatible item type") before the drop is allowed. If the guard fails, it
returns `False` via two separate early-out branches (0x0355, 0x0365). On
success it enters a loop (`EX_Jump -> 0x03d0` / back to itself) that walks an
array via `CallMath func=0x4806b3e0` (index increment pattern, `param[1]:
EX_IntConst 1`) paired with `func=0x4808c020` (bounds/length check) —
classic "for each item in container" iteration — calling
`FinalFunction func=0x48065fe0`/`func=0x48066600` per element (get/set item at
index). Near the end it calls a `CallMulticastDelegate` (`func=0xf9ea4d80`)
with 4 params including `EX_Self` and the container reference, broadcasting
the drop result to listeners (this is presumably what the UI listens to for
refresh, and possibly what should also be triggering the server-side
attach/equip visual sync — worth checking against the equip-bug findings if
that thread reopens). No raw-single-int trampoline pattern found in this
function — the native calls here all take multiple typed-looking params,
unlike the MC_ADS/ItemDropRequest/OnActiveWeaponSlotChanged stubs. No known
`ci=` constants present (this function is entirely CallMath/FinalFunction,
few Virtual calls, and the few `ci=` values present — 1803885, 1805004,
1805493, 1806012 (x2), 1807807, 1807925, 1807933 — don't match Torso/Legs/
Feet). Flagged for `resolve_fname.flag`.

### BP_MeleePickup_C_MeleeTrace
Decoded: `decoded_BP_MeleePickup_C_MeleeTrace.txt` (1,313 bytes, fully decoded)

Clear, well-understood function despite no symbol names — this is a melee
weapon hit-trace. Two nearly-identical blocks (one per melee "edge"/pass,
likely a double swing-arc trace) each: resolve an object via
`VirtualFunction FName(ci=90211)` + `NameConst` (same generic-lookup pattern
as WeaponsPickupComponent above, here called with `ci=115005` and `ci=3098` —
identical two NameConst values to the WeaponsPickupComponent case, reinforcing
that `ci=90211` is a shared "find component/actor by name" utility and
115005/3098 are two specific, recurring, so-far-unresolved tag/socket names),
then call `func=0x23536280` with a large explicit parameter block: a start
point, an end point (offset by a `28.0` value — plausibly a trace length or
socket offset), a `ByteConst 5` (trace channel enum, plausibly
`ECC_Visibility`-adjacent), `False`, an actors-to-ignore array, `ByteConst 1`
(draw-debug-type enum), an out-hits array, `True`, two 16-byte StructConst
rotator/color blocks, and a final `FloatConst 5.0` (trace duration?). This
13-parameter shape strongly matches Blueprint's `MultiSphereTraceForObjects`
or a similar K2 trace node (BlueprintFunctionLibrary trace helpers are known
for this exact "many defaulted params" signature) — i.e. this is a standard
sphere/line multi-trace call, not a suspicious raw-native mismatch. After the
trace, results feed a small loop (`func=0x5652dfa0` length/bounds check,
`func=0x42d4d360` index increment) processing each hit and calling
`LocalVirtualFunction FName(ci=1900705)` per hit (likely "apply damage to
this hit actor" or "register hit"). Standard, well-formed trace+iterate
pattern; nothing flagged as suspicious in this one — the parameter block, while
large, matches a native engine trace signature rather than a hand-rolled
mismatched call.

## Summary of Flags for Future `resolve_fname.flag` / Audit Pass

- **Raw-single-int trampoline pattern** (same shape as prior equip-bug
  findings — worth a signature audit): `MC_ADS` (int 173204),
  `OnActiveWeaponSlotChanged_Event_0` (int 164582), `ItemDropRequest_Event_0`
  (int 10278).
- **Unresolved FName ci= values worth naming**: 1848673, 1846309, 1848682
  (GetAnimationInfo); 115005, 3098 (recurring — WeaponsPickupComponent AND
  MeleeTrace, tied to shared lookup `ci=90211`); 1803885, 1805004, 1805493,
  1806012, 1807807, 1807925, 1807933 (PerfromDrop).
- None of the 11 files reference the known Torso/Legs/Feet FName constants.
