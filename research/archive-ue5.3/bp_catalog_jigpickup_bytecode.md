# BP_JigPickupComponent_C — Decoded Function Catalog

Quick-reference table for this component's actual bytecode logic (not just class metadata — see
`bp_catalog_player_core.md` for the property/flag-only catalogs). Every entry here has been
disassembled from a live bytecode dump (`research/bytecode/kismet_disasm.py`) and cross-referenced via
live `resolve_fname`/`resolve_ptr` round-trips against the SAME process instance that produced the dump
— see `04_ida_investigation_log.md`'s running commentary for the full narrative, live-test context, and
hedges on anything not 100% pinned down. This file is the "what does X do, in one line" index; the log
is the "how do we know that, and what's still open" narrative.

Status legend: ✅ fully decoded & understood · 🟡 decoded, some detail unconfirmed · ⬜ not yet decoded ·
🚫 not a standalone UFunction (Ubergraph-internal label only, no direct dump possible)

| Function | Bytes | Status | What it does |
|---|---|---|---|
| `ExecuteUbergraph_BP_JigPickupComponent` | 3406 | 🟡 | Shared graph every pure-Blueprint-graph function on this component trampolines into at a fixed byte offset. Fully disassembled; not every entry point's body individually traced yet. |
| `UpdateAttachments` | 486 | ✅ | Validates (`IsValidPickup` + 2 gates), populates `RepAttachments` from `GetListOfAttachments`, `MarkPropertyDirtyFromRepIndex(Self, 5, "RepAttachments")`, manually calls own `OnRep_RepAttachments()`, then `ProcessAttachments()`. |
| `ProcessAttachments` | 4305 | 🟡 | The real spawn/attach/cleanup loop — see below. Overall shape and every call target confirmed; exact branch wiring around the reverse stale-cleanup scan not pinned to the byte. |
| `SERVER_UpdateAttachmentState` | 54 | ✅ | Thin RPC trampoline → Ubergraph entry offset `0x0cf2` (decimal 3314). Body read: calls `UpdateAttachmentStateByUID` locally first (client prediction), then makes a real RPC-dispatched (`EX_VirtualFunction`, not Local) call to itself — classic "predict locally, tell the server" shape — then branches on a bool. Same 54-byte trampoline size as the already-known `OnExecuteInteract`. |
| `OnInteractActorOverDistance` | — | ✅ (call site fully understood) | NOT a method on this component — it's called on `CurrentActor` (the interacting *character*) cast to `BP_JigCharacterInterface`, with this component's own `GetOwner()` passed as the item-actor param. The character implements/handles it, not this component. See `CheckDistanceFromActor` below — full call site confirmed, no need to chase the receiver's own body for the attachment investigation (see correction note there). |
| `UpdateAttachmentStateByUID` | 663 | 🟡 | Loops the `RepAttachments` array searching for an entry matching a target UID (input param). On match: reads the found entry, checks `Array_Length(...) == 1` (possibly "is this the last/only attachment"), sets an out-bool. On no match: out-bool defaults false. Reads like removal-adjacent bookkeeping rather than the render/attach step itself — exact semantics of the length==1 check not pinned down. |
| `Jig_SetAttachmentActiveState` | 36 (trampoline) | ✅ | Found via a live attachment pointer (`local>...` child from `attach_health` logging, resolved to `BP_MilitarySuppressorLocalAttachment_C`) rather than a name guess — the real spawned-attachment class naming convention is `BP_<Item>LocalAttachment_C`, not `..._Pickup_C`. Trampolines into shared base class `BP_AMainLocalAttachment_C::ExecuteUbergraph_...` (129 bytes, fully decoded): sets an instance var (`IsActive`-equivalent) to the incoming bool, then calls `OnActiveStateChanged()` (an overridable hook, e.g. for a laser's on/off beam). Purely a functional on/off toggle — not positional, unrelated to the render-desync bug. |
| `FindLocalAttachmentByUID` | 679 | ✅ | Confirmed simple: linear search over `InstanceVariable 0x133482980` (the tracking array) from `i=0` to `Array_Length`, calling `Jig_GetAttachmentInfo` on each entry and comparing its UID via `EqualEqual_GuidGuid`. Nothing unexpected. |
| `GetListOfAttachments` | 1213 | ✅ | Actually lives on **`BP_JigMultiplayer_C`** (traced by finding `IsValidPickup`'s host class first — both are called through `InstanceVariable 0x1244b4500` in `UpdateAttachments`, a direct property read, almost certainly the known `OwnerMPComp` field per `BP_JigPickupComponent.hpp`). Calls `GetAllContainerByItemUID(itemUID, &containers)` then loops the result filtering for attachment-type entries into the output list. Pure data-gathering, nothing new for the render-desync mechanism. |
| `IsValidPickup` | 95 | ✅ | Also on `BP_JigMultiplayer_C` (see above). Simple: validity-checks a struct member of a cached instance var (a tracked pickup/UID reference) and returns it alongside the bool. |
| `OnAttachmentsUpdated` | — | ✅ | Real multicast delegate, broadcast from inside `ProcessAttachments`. **This project's own repair code never broadcasts it** — leading suspect for the render-desync bug. Live-checked its `InvocationList` (new `dump_delegate.flag` diagnostic): **`count=1`**, a real subscriber, bound function also named `OnAttachmentsUpdated` (self-bind Custom Event pattern) — real logic reacts to this broadcast, not a dead delegate. Exact owning object/class not resolved (would need a `GUObjectArray` walk from the raw `TWeakObjectPtr` index, not yet set up), but not needed to justify trying the broadcast-based fix. |
| `CheckDistanceFromActor` | 492 | ✅ | **Fully decoded, corrects an earlier assumption.** Every 0.1s (via the timer `OnExecuteInteract` starts): if `CurrentActor` invalid, clears its own timer and returns. Else computes distance from `InteractingActorLoc` to `CurrentActor`'s current location; if ≤300 units, no-op. If >300: clears its own timer, casts `CurrentActor` to `BP_JigCharacterInterface`, calls `.OnInteractActorOverDistance(Self.GetOwner(), bool)` on it, then sets its own `CurrentActor = null`. **Touches no attachment/socket/render state at all** — this is the loot-interaction-prompt-cancel system (walk away from an item you were about to interact with), not anything about an already-equipped item's physical state. Was an open thread from earlier in this session on the assumption it might explain the attachment bug; now closed with real ground truth — it doesn't. |
| `SetCanInteract` | 576 | ✅ | Writes `CanInteract`/`EnablePhysics` params straight into two instance vars (near-certainly `CanBePicked?`/`EnablePhysics?`), then manually invokes each one's own `OnRep_` callback. |
| `CheckMismatch` | 1601 | ✅ | Container-config validation/diagnostic loop, logs a mismatch warning. Not attach-related. |
| `OnRep_CanBePicked?` | 310 | ✅ | `if (!CanBePicked && IsValid(CurrentActor))`: runs the identical interaction-cancel sequence as `CheckDistanceFromActor`'s over-distance branch (clears its timer, casts to `BP_JigCharacterInterface`, calls `OnInteractActorOverDistance`, clears `CurrentActor`). Confirms the interaction-prompt-lifecycle theory, not attachment-related. |
| `OnRep_EnablePhysics?` | 3 | ✅ | Effectively empty stub. |
| `CanInteract?` | 22 | ✅ | Trivial getter (too small to matter). |
| `ValidateAttachedActor` | 488 | ✅ | **Best lead for the render-desync fix.** If the item's attach parent is a `Character`: reads the CURRENT socket transform via `GetSocketTransform` (same call `ProcessAttachments` uses) and directly `K2_SetActorTransform(socketTransform, bSweep=false, hit, bTeleport=true)`s the item onto it. A real, already-existing, game-authored repair mechanism — structurally different from (and possibly more reliable than) both approaches already tried tonight (`K2_AttachToComponent` re-parent; `UpdateAttachments()`/`OnAttachmentsUpdated`). Not yet tested live. |
| `OnExecuteInteract` | 54 | ✅ | Trampoline → Ubergraph entry 2365. Real body: sets `CurrentActor` = interacting actor param, starts the 0.1s `CheckDistanceFromActor` timer. |

## Resolved CI table (this component's Ubergraph + the two standalone functions above)

Only valid for cross-referencing NEW decodes made in the **same live process instance** these were
resolved against (2026-08-16, PC1, session starting ~15:24). A relaunch invalidates all of these — see
the log's "always re-dump before resolving" lesson.

**STALE as of ~20:15** — the game crashed and was relaunched (new process, `StartTime 8/16/2026 8:15:31
PM`). Every CI below is only valid for the OLD process; anything decoded after the relaunch needs a
fresh resolve, not a lookup against this table.

`92404`=`SetSimulatePhysics`, `93336`=`K2_DestroyActor`, `1729580`=`Jig.InteractOptions.Option1`,
`1790607`=`Jig_GetAttachmentInfo`, `1790657`=`Jig_GetPrimitiveComponent`, `1790698`=
`Jig_SetAttachmentActiveState`, `1790713`=`Jig_SetAttachmentInfo`, `1797933`=`GetCapacity`,
`1844012`=`AddJigWidgetToContent`, `1844651`=`CanLootPickup?`, `1844671`=`CheckMismatch`, `1844793`=
`ExecuteUbergraph_BP_JigPickupComponent`, `1844888`=`FindLocalAttachmentByUID`, `1844912`=
`GetAttachSocketByInContainerIndex`, `1844938`=`GetListOfAttachments`, `1844949`=`GetMainSceneComp`,
`1845044`=`IsInventoryVisible?`, `1845055`=`IsValidPickup`, `1845494`=`OnInteractActorOverDistance`,
`1845509`=`OnPickupInteractExecuted`, `1845543`=`OnRep_RepAttachments`, `1845629`=`ProcessAttachments`,
`1845650`=`ProcessStartingItems`, `1845670`=`RepAttachments`, `1845678`=`RepCapacity`, `1845711`=
`RequestServerData`, `1845721`=`SERVER_UpdateAttachmentState`, `1845768`=`SetCount`, `1845773`=
`SetInteractDistance`, `1845796`=`SetupInventoryWidget`, `1845868`=`UpdateAttachments`, `1845878`=
`UpdateAttachmentStateByUID`, `1845901`=`ValidateAttachedActor`.

**Fresh-process CIs (post-relaunch, ~20:15+ session)**, from the follow-up round: `1791296`=
`OnActiveStateChanged` (on `BP_AMainLocalAttachment_C`), `1851543`=`GetAllContainerByItemUID` (on
`BP_JigMultiplayer_C`). Also only valid for this specific post-relaunch process instance.

## Status: table effectively complete

Every function that was reachable via a name-based lookup or a live pointer is now decoded — the
`Jig_SetAttachmentActiveState`/`GetListOfAttachments`/`IsValidPickup` items that were blocked earlier
got unblocked by (a) pulling a live attachment-actor pointer straight out of the running
`attach_health` diagnostic's log lines and using it to discover the real class naming convention
(`BP_<Item>LocalAttachment_C`), and (b) tracing `InstanceVariable 0x1244b4500` back to its real host
class (`BP_JigMultiplayer_C`, via the known `OwnerMPComp` field) instead of assuming it meant an
interface cast. Only two rows remain 🟡 rather than ✅, both genuinely minor:
`ExecuteUbergraph_BP_JigPickupComponent` (fully disassembled as raw bytecode, just not every one of its
many entry points individually traced — only the ones actually reached by a named function this session)
and `ProcessAttachments` (every call target confirmed, just the exact branch wiring around the reverse
stale-cleanup scan not pinned to the byte).

## Next targets (in priority order for the render-desync investigation)

1. **Actually test the fix** — this is now the real bottleneck, not more decoding. Two candidate repair
   calls are both fully understood and ready to try: `ValidateAttachedActor`'s
   `GetSocketTransform`+`K2_SetActorTransform(bTeleport=true)` pattern (best lead), or calling
   `UpdateAttachments()`/broadcasting `OnAttachmentsUpdated` instead of a bare `K2_AttachToComponent`.
   Neither has been live-tested — tonight's live-testing budget went to two failed unconditional-repair
   experiments before these were found. Should be tried at a conservative, low frequency given that
   history.
2. Find who else (if anyone/anything) is bound to `OnAttachmentsUpdated` — would need either a live
   memory scan of the delegate's bound-function array (`InstanceVariable 0x1244b6200` on a live
   `BP_JigPickupComponent_C`, offset/layout not yet mapped), or checking other classes' bytecode for
   `AddDynamic`/`Bind`-style calls targeting this delegate. Lower priority now — mostly relevant only if
   the `OnAttachmentsUpdated`-broadcast fix path (rather than `ValidateAttachedActor`'s) turns out to be
   the one that matters.
3. `OnInteractActorOverDistance`'s own body (still Ubergraph-internal on `BP_PlayerCharacter_C`, not
   found) — low priority, since it's confirmed to be the loot-prompt-cancel system, unrelated to the
   attachment bug.
