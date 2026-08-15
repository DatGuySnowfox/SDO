# Bytecode Batch 2: Player_AnimBP_C + PressAnyKeyWidget_C

Decoded from raw Kismet `.bin` captures in `C:\Users\mccau\AppData\Roaming\SurrounDeadBridge\` using
`research/bytecode/kismet_disasm.py`. All source/output files live in that AppData folder
(`decoded_<FunctionName>.txt`). No live game interaction was used — static bytecode analysis only.

All property/function addresses (`prop=0x...`, `func=0x...`) are raw pointers captured from a live
process and are **not stable across runs** — they're useful only for correlating reads within the
same capture session, not as permanent identifiers. `FName(ci=...)` values are ComparisonIndex
values into the live FName table; they need a `resolve_fname.flag` pass (per existing project
convention) to become readable names. They're listed below for that future pass.

---

## Player_AnimBP_C_BlueprintThreadSafeUpdateAnimation

Decoded: `decoded_Player_AnimBP_C_BlueprintThreadSafeUpdateAnimation.txt` (191 bytes, 39 lines)

This is the anim graph's `BlueprintThreadSafeUpdateAnimation` override — the standard UE anim
instance per-frame thread-safe update entry point. Despite being "the main per-frame anim update,"
the captured bytecode is short: it's a **dispatcher**, not the bulk of the logic. It:

1. Calls a math function (`func=0x42472c40`, likely a boolean AND/OR or comparison) on two instance
   variables, stores result in a local bool.
2. Branches (`EX_JumpIfNot`) on that bool.
3. If false: calls a chain of `EX_LocalVirtualFunction` calls with no params — `ci=2064717`,
   `ci=2064707`, `ci=2064688` — then one with a param (`ci=2064665`), then does an `EX_Cast`
   (conv_type=4, likely object→object or interface cast) and calls two more local virtual functions
   (`ci=2064694` with a param, `ci=2064699` with none).
4. If true: skips straight to `EX_Return`.

These `EX_LocalVirtualFunction` calls are almost certainly calls into the other captured
functions in this batch (`GetAimOffset`, `GetHeadRot`, `GetLeftHandLoc`, `GetSpeed&Direction`,
`GetThreadSafeBooleans`, `GetAnimationInfoFromCharacter`) — six local-virtual calls line up with
six other Get* functions captured. The leading bool check looks like a "should we even update
this frame" gate (e.g., only recompute expensive anim state if some flag/animation-info-valid
condition holds). Exact resolution requires FName resolution of the six `ci=` values against the
class's function table.

FName ci values seen: `2064665, 2064688, 2064694, 2064699, 2064707, 2064717`

---

## Player_AnimBP_C_CombatState

Decoded: `decoded_Player_AnimBP_C_CombatState.txt` (36 bytes, 11 lines)

Trivial function. Writes a local variable into a persistent-frame slot
(`EX_LetValueOnPersistentFrame`) — this is the standard Blueprint pattern for caching a pure-node
result across the anim graph's implicit "update once per frame" semantics. Then calls one local
function (`func=0x1239bbae0`) with a single int constant argument (`2085`) — likely an anim-graph
node ID / pin index passed for exposed-value evaluation (same pattern seen in the `Fabrik_*` /
`TransitionResult_*` node-input evaluators below, which also pass a single int const to a
`LocalFinalFunction`). No branching, no other reads. Purpose (probably: caches/exposes a
"combat state" enum or bool onto the persistent frame for the anim graph state machine) is
inferred, not certain from bytecode alone.

---

## Player_AnimBP_C_ExecuteUbergraph_Player_AnimBP

Decoded: `decoded_Player_AnimBP_C_ExecuteUbergraph_Player_AnimBP.txt` (1572 lines — by far the
largest function in this batch)

This is the compiled event graph (all Blueprint event nodes — BeginPlay, Tick-driven custom
events, delegate bindings, etc. — get flattened into one `ExecuteUbergraph_*` function with
jump-based dispatch). Given its size, this was scanned structurally rather than read line-by-line:

- Opcode histogram: 314 `EX_LocalVariable`, 64 `EX_LetObj`, 62 `EX_ObjectConst`, 61 `EX_ByteConst`,
  56 `EX_EndFunctionParms`, 52 `EX_InstanceVariable`, 43 `EX_CallMath`, 37 `EX_Let`,
  34 `EX_FloatConst`, 29 `EX_LetBool`, 28 `EX_PopExecutionFlow`, 18 `EX_StructMemberContext`,
  14 `EX_VectorConst`, 11 `EX_RotationConst`, 11 `EX_DoubleConst`, 10 `EX_NoObject`, 10 `EX_Jump`,
  9 `EX_Context`, 6 `EX_FinalFunction`, 5 `EX_Self`.
- 49 total function calls (`CallMath`/`FinalFunction`/`LocalFinalFunction` combined) — a large,
  multi-branch event graph, consistent with an AnimBP that wires up several state-machine
  transition events, notifies, and setup logic in its "event graph" (as opposed to the
  thread-safe update function, which is graph-node driven and mostly bypasses this).
- Heavy use of `EX_LetObj`/`EX_ObjectConst`/`EX_NoObject` and `EX_StructMemberContext` suggests a
  lot of object-reference assignment and struct-field access (e.g., caching character/mesh/
  component references, or building/reading FHitResult-like structs).
- Vector/Rotation/Float/Double consts (14+11+34+11 = 70 literal constants) point to a lot of
  numeric setup — thresholds, offsets, blend parameters — hardcoded in the graph.
- Only two `ci=` FName values seen at top level: `ci=102667` and `ci=1931066` (there may be more
  nested inside struct/context blocks not captured by the top-level grep pass).

This function's exact behavior (which events, what triggers what) is not resolvable from bytecode
structure alone without function-address/FName resolution; flagging for a future deeper pass if the
event graph's specific behavior becomes relevant.

FName ci values seen: `102667, 1931066`

---

## Player_AnimBP_C_GetAimOffset

Decoded: `decoded_Player_AnimBP_C_GetAimOffset.txt` (747 bytes, 166 lines)

Computes an aim offset — reads three instance variables via `CallMath func=0x42261c80` (likely a
"get actor/component rotation" or "make rotator" style call taking 3 params), converts to a second
value via `CallMath func=0x3f14ab20` (one param + one instance var — plausibly a delta/normalize
angle call), casts results, then builds a value via `CallMath func=0x3f1420e0` (3 params: a float
0.0 constant plus two locals — looks like a "MakeRotator" or "ClampAngle" pattern), combines via
`func=0x3f14f2e0` (4 params including a 0.0 float — could be `RInterpTo` given the float args seen
elsewhere for interpolation), then decomposes the resulting rotator/vector into components via
`func=0x3f112620` (4 out-params — consistent with "Break Rotator" giving pitch/yaw/roll/w or
similar). Casts one component (conv_type=4) and writes it back into an instance variable.

**The entire sequence above repeats a second time, near-identically**, writing to a second
instance variable at the end. This strongly suggests the function computes **two related aim
offset values** — most plausibly separate yaw and pitch aim-offset outputs (or a "current" and a
"target/smoothed" aim offset), each independently derived from the same three source rotation
instance variables. No branches — straight-line, always executes both blocks.

No FName ci values present (no virtual function calls by name in this function — all calls are by
raw pointer, i.e. `CallMath`/pure functions, not `LocalVirtualFunction`).

---

## Player_AnimBP_C_GetAnimationInfoFromCharacter

Decoded: `decoded_Player_AnimBP_C_GetAnimationInfoFromCharacter.txt` (345 bytes, 86 lines)

Reads the owning actor, casts it to an interface (`EX_ObjToInterfaceCast class=0x12ac9ac00`) —
almost certainly a project-defined "animation info provider" interface implemented by the player
character. Casts again (conv_type=2) and branches on whether that succeeded (`EX_JumpIfNot`).

- **If the interface cast succeeded**: calls an interface function (`EX_InterfaceContext` →
  `EX_LocalVirtualFunction FName(ci=1941812)`) with **6 output parameters** — a name/tag, a float,
  and four bools/objects — then writes two of those results (an FName-typed field and a bool) into
  instance variables.
- **If it failed** (character doesn't implement the interface, e.g. wrong actor class): falls back
  to hardcoded defaults — an empty struct (`EX_StructConst`, 12 bytes, single FName member
  `ci=0` i.e. "None"), `0.0` double, three `false` bools, and a null object reference — before
  writing those same fallback values into the instance variables.

This is a defensive "pull animation-relevant info from whatever the owning pawn is, falling back to
safe defaults if it's not the expected type" getter — likely feeding combat-state/aim-related tags,
a float (maybe aim pitch or health-derived value), and flags into the AnimBP's instance state for
the graph to read every frame.

FName ci values seen: `1941812` (the interface function called), `0` (None / default-struct
member).

---

## Player_AnimBP_C_GetHeadRot

Decoded: `decoded_Player_AnimBP_C_GetHeadRot.txt` (1611 bytes, 355 lines — second-largest in this
batch)

Computes a head-look rotation, gated by a bool instance variable (`prop=0x12f5a5d00`, checked via
`EX_JumpIfNot`) that looks like an "is aiming" / "should look at target" toggle:

- **Branch A (flag true)**: calls `func=0x424ba500` on an instance var (likely "GetActorRotation" or
  similar single-object rotation getter), checks the resulting bool, then does an `RInterpTo`-style
  call (`func=0x424bf120`, taking a rotator instance var, a zero `RotationConst`, and interp speed
  constants `1.0` / `0.05`) to smoothly interpolate a stored rotation value. Feeds that into another
  call (`func=0x42472460`, 4 params — likely "Break Rotator" into pitch/yaw/roll) and further
  processing (normalize-angle style calls with `-1.0`), eventually writes a single-value result
  (Roll or Yaw component) into instance var `0x12f5a5680`.
- **Branch B (flag false)**: uses two different source instance vars (`0x12f5a4300`/`0x12f5a4380`)
  via `func=0x424ba960` (2-param — plausibly "Delta Rotator" between two rotations), breaks that
  into components, then does a long chain of **clamping** calls (`func=0x424b1040`/`0x4247fe40`
  with constants like `90.0`, `-120.0`, `-60.0`/`60.0`, `-40.0`/`40.0`, `150.0`, `-90.0`/`30.0` —
  these read exactly like head/neck rotation limits, e.g. yaw clamp ±60°, pitch clamp ±40°, and a
  "is target behind/out of range" check against 90°/150°). Combines the clamped pitch/yaw back into
  a rotator (`func=0x424be780`), runs it through the same `RInterpTo`-style smoothing
  (`func=0x424bf120`, speed `1.0`/`0.05`) and "Break Rotator" (`func=0x42472460`) as branch A, and
  writes the final scalar result into the **same** instance variable `0x12f5a5680`.

This is clearly a **head-look-at IK rotation computation** with clamped rotation limits to keep the
character's head from rotating unnaturally far, smoothed via interpolation for a natural turn
speed. No FName ci values (all calls are by pointer).

---

## Player_AnimBP_C_GetLeftHandLoc

Decoded: `decoded_Player_AnimBP_C_GetLeftHandLoc.txt` (30 bytes, 9 lines)

Trivial one-line getter: copies one instance variable (`prop=0x13565c600`, presumably a cached
left-hand IK target location, likely set elsewhere e.g. from a weapon socket or animation notify)
directly into another instance variable (`prop=0x13565d680`, the exposed "left hand location"
output used by the anim graph's IK node). No computation, no branching, no ci= values.

---

## Player_AnimBP_C_GetSpeed&Direction

Decoded: `decoded_Player_AnimBP_C_GetSpeed&Direction.txt` (169 bytes, 38 lines)

Computes movement speed and direction for locomotion blending:

1. `CallMath func=0x421a52c0` on an instance var (likely "GetVelocity" on the owning
   pawn/character) → stores result into instance var `0x12c904580` (probably raw velocity vector).
2. `FinalFunction func=0x23a26980` with two instance-var params (likely "CalculateDirection",
   a common UE4/5 helper taking velocity + actor rotation and returning a signed movement-direction
   angle) → result cast (conv_type=4, float truncation/precision cast) and written into instance var
   `0x12c904380` (the exposed "Direction" float).

No explicit "speed" scalar computation is visible separately — speed is likely derived elsewhere
(e.g. as the magnitude of the velocity vector stored in `0x12c904580`, computed by the anim graph
itself or another function) or this function's name covers both but the speed part reduces to the
raw velocity read in step 1. No ci= values (calls by pointer only).

---

## Player_AnimBP_C_GetThreadSafeBooleans

Decoded: `decoded_Player_AnimBP_C_GetThreadSafeBooleans.txt` (229 bytes, 66 lines)

A flat sequence of `EX_PushExecutionFlow`/`EX_PopExecutionFlow` pairs — the compiled form of a
Blueprint "sequence" node with 6 parallel branches, all unconditional. Each branch does a simple
bool copy from one instance variable to another (7 total `EX_LetBool` operations across the 6
branches), except one branch which does a comparison (`CallMath func=0x3f0f2b60`, 2 bool params —
likely an equality or AND check) before storing the result. This function is a **thread-safe
snapshot/mirror**: it copies a batch of gameplay-side boolean flags (jumping, crouching, aiming,
sprinting, etc. — exact meaning unresolvable without FName/property resolution) into a parallel set
of instance variables that are safe to read from the animation thread, avoiding direct
cross-thread reads of the "live" gameplay booleans. No ci= values (all instance-variable copies,
one math call by pointer).

---

## Player_AnimBP_C_EvaluateGraphExposedInputs_..._AnimGraphNode_Fabrik_31BAC05D486A8E2F5D5AFF87706BFF20

Decoded: `decoded_..._Fabrik_31BAC05D486A8E2F5D5AFF87706BFF20.txt` (18 bytes, 8 lines)

Minimal exposed-input evaluator for a FABRIK IK node (two-bone/multi-bone IK solver, commonly used
for arm/leg IK to a target location — matches up with the `GetLeftHandLoc` getter above, likely
feeding this node's target). Body is just one `LocalFinalFunction` call with a single int constant
argument (`1877`, the anim-graph node's internal pin/link index) and a return. No other computation
is visible — the actual input value comes from whichever instance/pose-link property this node
pulls from (not captured in this tiny function body).

---

## Player_AnimBP_C_EvaluateGraphExposedInputs_..._AnimGraphNode_Fabrik_AF8CB55449FDEB26FA14F38357C17595

Decoded: `decoded_..._Fabrik_AF8CB55449FDEB26FA14F38357C17595.txt` (also 18 bytes, same 8-line
structure as the other Fabrik node above — identical shape, different int constant/node ID).
This is a second, separate FABRIK IK node's exposed-input evaluator (likely the other arm, since
one left-hand IK getter exists — this second node may drive a right-hand or other limb IK target).
Same caveat: the actual driving value is external to this stub.

---

## Player_AnimBP_C_EvaluateGraphExposedInputs_..._AnimGraphNode_TransitionResult_1D52F779406FC4110BBFC4B70C211957

Decoded: `decoded_..._TransitionResult_1D52F779406FC4110BBFC4B70C211957.txt` (18 bytes, 8 lines)

Same minimal shape as the Fabrik evaluators: one `LocalFinalFunction` call with int constant `999`
(the node's pin/link index) and a return. This is the exposed-input evaluator for an anim graph
**state-machine transition rule** node (a "Can Enter Transition" bool condition) — it evaluates
whatever boolean expression feeds that specific transition, but the expression itself lives
elsewhere (likely computed via the `GetThreadSafeBooleans`/`CombatState` results feeding the anim
graph, given this file's tiny size just forwards to node index 999).

---

## PressAnyKeyWidget_C_ExecuteUbergraph_PressAnyKeyWidget

Decoded: `decoded_PressAnyKeyWidget_C_ExecuteUbergraph_PressAnyKeyWidget.txt` (366 bytes, 116
lines)

Small UI widget event graph. Notable operations:
- Broadcasts a multicast delegate (`EX_CallMulticastDelegate func=0x1089e9540` on instance var
  `0x66e17380`), then calls a self virtual function `FName(ci=107075)` — likely something like
  "RemoveFromParent" or a widget-teardown call fired right after the delegate broadcast (fits a
  "press any key" prompt: broadcast that a key was pressed, then close/hide itself).
- A repeating block (looped via `EX_Jump -> 0x003f`) that calls two "play animation" style
  functions (`func=0x24ae8ac0`, `func=0x24ae8ba0`) on widget-animation instance vars with float
  playback-rate/start-time params, plus a `CallMath func=0x248776a0` taking `Self`, a float `0.5`
  (likely a delay), and a struct literal containing a `SkipOffsetConst`, an int const
  `1653633363` (possibly a packed GUID/hash, not a meaningful number directly), and
  `FName(ci=2249267)` — the classic Blueprint "Delay" latent-node pattern (skip offset + link ID +
  latent action name).
- A final `EX_Context` call into a `Key`-struct comparison (`FName(ci=2969)` — same ci value seen
  in `OnKeyDown` below, likely the widget's designated "trigger key" FName, e.g. "AnyKey" or a
  bound key name), gating whether the loop continues (`EX_PopExecutionFlowIfNot`) back to `0x0097`.

Overall: this looks like a looping "pulse/blink the prompt" animation cycle that also polls for a
key match, broadcasting a delegate and closing when triggered.

FName ci values seen: `107075, 2249267, 2969`

---

## PressAnyKeyWidget_C_OnKeyDown

Decoded: `decoded_PressAnyKeyWidget_C_OnKeyDown.txt` (173 bytes, 38 lines)

Standard UMG `OnKeyDown` override:
1. Calls `CallMath func=0x480f8660` on a local (likely `Get Key` from the `FKeyEvent` param) →
   stores result.
2. Calls `CallMath func=0x480f8820` comparing that key against a struct literal containing
   `FName(ci=2969)` (same ci as seen in the ubergraph above — the widget's target key, e.g. a key
   named "AnyKey" or a specific bound key) → bool result.
3. If matched (`EX_JumpIfNot` gate): calls `func=0x24964ae0` (no params — likely constructs an
   `EventReply::Handled()`) and returns that as the function's out-value; otherwise falls through
   to the default `Return EX_LocalOutVariable` (unhandled reply, implicitly built elsewhere or
   defaulted).

Straightforward: on any key press, check if the pressed key matches a target, and if so mark the
input as handled (closing/advancing the prompt via whatever `OnKeyDown`'s side effects trigger,
presumably tied to the ubergraph's delegate broadcast above).

FName ci values seen: `2969`

---

## PressAnyKeyWidget_C_Tick

Decoded: `decoded_PressAnyKeyWidget_C_Tick.txt` (54 bytes, 14 lines)

Trivial. Writes two locals (likely `MyGeometry` and `InDeltaTime`, the standard UMG `Tick`
parameters) into persistent-frame slots, then calls one local function (`func=0x1089e92a0`) with
int constant `253` — same pattern as `CombatState`'s single-int-arg call, i.e. an anim-graph-style
"evaluate exposed pin" dispatch, here presumably feeding the widget's Tick-bound animation/graph
update. No other logic — likely just forwards Tick into whatever bound graph node consumes it (a
widget animation progress read, matching the play-animation calls seen in the ubergraph). No ci=
values.

---

## Summary of notable animation-system behavior

- **State flows from gameplay → AnimBP via a getter layer**: `GetAnimationInfoFromCharacter` pulls
  data through an interface implemented by the pawn (with an explicit safe-default fallback path if
  the cast fails), and `GetThreadSafeBooleans` copies a batch of gameplay bools into
  thread-safe-mirror instance variables — both patterns exist specifically because
  `BlueprintThreadSafeUpdateAnimation` runs on the animation worker thread and can't safely touch
  live gameplay-thread state directly.
- **Head-look and aim-offset both use clamped, interpolated rotation math** (`GetHeadRot`,
  `GetAimOffset`) — explicit numeric clamp constants (e.g. ±40°/±60°/90°/150° in `GetHeadRot`) cap
  how far the head/aim can rotate, and `RInterpTo`-style calls (speed `1.0`, alpha `0.05`) smooth
  the result frame to frame — standard anti-snapping IK/look-at setup.
- **IK targets are simple pass-through reads**: `GetLeftHandLoc` and the two `Fabrik_*`
  exposed-input evaluators are all trivial one-op stubs, meaning the actual IK target computation
  happens upstream (event graph or gameplay side) and these functions just forward a cached value.
- `BlueprintThreadSafeUpdateAnimation` itself is a thin dispatcher gated by one bool check, chaining
  into the other Get* functions documented above — the real per-frame cost lives in those, not in
  the update function itself.
- The large `ExecuteUbergraph_Player_AnimBP` (1572 lines) was analyzed structurally (opcode
  histogram, call count) rather than fully hand-traced; its specific event-by-event behavior would
  need FName/function-address resolution to pin down further.
