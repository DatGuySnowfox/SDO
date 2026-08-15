# Bytecode Decode Status

Reference index cross-referencing the 9 static `bp_catalog_*.md` files (structural inventory,
built from the FModel export — names/flags only, no bytecode) against what has actually been
captured (`.bin` Kismet bytecode dumps in `SurrounDeadBridge/`) and decoded
(`decoded_*.txt` disassembly, produced by `research/bytecode/kismet_disasm.py`).

Status definitions:
- **Decoded** — a `decoded_<FunctionName>.txt` exists; bytecode has been disassembled and at
  least skimmed/understood.
- **Captured, not decoded** — a `.bin` capture exists but no matching `decoded_*.txt`. Ready to
  decode immediately offline with `research/bytecode/kismet_disasm.py` — no live game needed.
- **Not captured** — neither exists. This is the vast majority of the cataloged surface (static
  catalogs only list names/flags, never bytecode) — expected, not a gap to panic about.

Raw counts: **63** `.bin` files, **19** `decoded_*.txt` files (all 19 match one of the 63 `.bin`
files — every decode has a corresponding capture). 44 `.bin` files are captured-only.

## Summary counts by catalog/domain

Cataloged counts below are the curated function names extracted from each `bp_catalog_*.md`
(RPC tables + backtick-quoted names in prose sections) — approximate but representative; the
catalogs themselves note they already filtered out auto-generated per-node noise
(`InpActEvt_*`, `OnBlendOut_<hash>`, etc.).

| Domain (catalog file) | Cataloged functions | Decoded | Captured, not decoded | Not captured |
|---|---:|---:|---:|---:|
| player_core | 399 | 12 | 12 | 375 |
| inventory_jigsaw | 240 | 7 | 8 | 225 |
| ai_vehicles | 327 | 0 | 0 | 327 |
| building | 263 | 0 | 0 | 263 |
| smartai | 194 | 0 | 0 | 194 |
| misc_systems | 240 | 0 | 4 (3 PressAnyKeyWidget + MeleeTrace) | 236 |
| playermodel | 61 | 0 | 0 | 61 |
| anim_input_wip | 21 | 0 | 10 (Player_AnimBP_C) | 11 |
| sky_terrain_levels | 53 | 0 | 0 | 53 |
| **Total (cataloged)** | **1,798** | **19** | **34** | **1,745** |

(10 more captured `.bin` files — `armpose_*`, `fabrik_*`, `noisefloor_*`, `pbfg.bin`,
`ubergraph.bin` — are raw diagnostic/memory captures not tied to a single named catalog
function; not counted in the table above. 63 total bin − 19 decoded − 34 domain-attributed
captured-only = 10 raw/uncataloged captures.)

Decode coverage is currently concentrated entirely in two classes:
`BP_PlayerCharacter_C` (player_core) and `BP_JigHelperComp_C`/`BP_JigMultiplayer_C`
(inventory_jigsaw) — the appearance/equip-bug investigation's focus area. Seven of nine domains
have zero decoded functions.

## Decoded (19)

All from the appearance/equip investigation. One-line note is a skim of the disassembly, not a
re-derivation.

| Function | Class | Note |
|---|---|---|
| ActorLoaded | BP_PlayerCharacter_C | Confirmed-empty stub — single native instrumentation call, no real logic |
| ActorPreLoad | BP_PlayerCharacter_C | Confirmed-empty stub — single native instrumentation call, no real logic |
| Event_LoadPlayer | BP_PlayerCharacter_C | Confirmed-empty stub — single native instrumentation call, no real logic |
| BodyPartVisibility | BP_PlayerCharacter_C | Branches on local vars, calls a component visibility toggle via struct-member context |
| EquipClothingToMesh | BP_PlayerCharacter_C | Sets bool from a FinalFunction call taking an object const + local var param (mesh apply path) |
| UpdateBodyParts | BP_PlayerCharacter_C | Dispatches on body-part name (`"Torso"`/`"Legs"`/`"Feet"`) via CallMath + NameConst; only handles those three names — does not know clothing-overlay component names |
| LoadPlayerInventory | BP_PlayerCharacter_C | LetObj from CallMath(Self, local var) then LetBool assignment — inventory-load kickoff |
| OnLoadSavedDataRequested | BP_PlayerCharacter_C | Context call to a LocalVirtualFunction, sets out bool True, returns |
| OnRep_ClothingLegsEquipped | BP_PlayerCharacter_C | JumpIfNot on instance var, then LocalVirtualFunction call with StructConst param (mesh reapply) |
| OnRep_FacewearEquipped | BP_PlayerCharacter_C | Same shape as OnRep_ClothingLegsEquipped — PushExecutionFlow guard + LocalVirtualFunction w/ StructConst |
| OnRep_PrimaryWeaponEquipped | BP_PlayerCharacter_C | Same OnRep pattern — JumpIfNot guard, LocalVirtualFunction w/ StructConst + second param |
| Svr_AttachClothing | BP_PlayerCharacter_C | **Native stub** — pure LetValueOnPersistentFrame parameter marshaling into a native thunk (`func=0x124f13dc0`); no Blueprint-visible mesh-attach logic exists here |
| EquipActorToSocket ("Equip Actor to Socket") | BP_JigHelperComp_C | LetObj + FinalFunction call, then a LocalVirtualFunction dispatch (ci=1860028) — the actual native attach (`K2_AttachToComponent`, SnapToTarget/SnapToTarget/KeepWorld/weld) is inside that virtual call |
| OnLoadDataRequested | BP_JigHelperComp_C | PushExecutionFlow guard, calls a LocalVirtualFunction with a StructConst param, then sets two local int properties to 0 — init path |
| OnPickupEquipped | BP_JigHelperComp_C | PushExecutionFlow guards, LetObj from a FinalFunction call, then a VirtualFunction dispatch (ci=93673) on a second local object |
| OnRep_ActiveWeapon | BP_JigHelperComp_C | LetObj from FinalFunction call, then LetBool from a Context call reading a field off that same object |
| HandleActorEquipped | BP_JigMultiplayer_C | Two nested PushExecutionFlow guards, LetBool from CallMath, conditional PopExecutionFlowIfNot — branchy equip-result handling |
| HandleItemOverItem | BP_JigMultiplayer_C | Trivial — LetValueOnPersistentFrame + a single LocalFinalFunction call (int const 9060), return |
| ServerFuncHandleEquipActor | BP_JigMultiplayer_C | Largest decoded function (3458 bytes) — multiple nested PushExecutionFlow guards, several Let/LetBool assignments; the server-side equip-actor RPC handler body |

## Captured but not decoded (44 `.bin` files)

Ready to decode immediately offline via `research/bytecode/kismet_disasm.py` — no live game
capture needed, the raw bytecode is already on disk.

**player_core (BP_PlayerCharacter_C, 12):**
`ExecuteUbergraph_BP_PlayerCharacter.bin`, `GetAnimationInfo.bin`, `MC_ADS.bin`,
`OnActiveWeaponSlotChanged_Event_0.bin`, and 7 `InpActEvt_IA_PrimaryAction_K2Node_EnhancedInputActionEvent_{4,6,13,23,24,64,65}.bin`
(Enhanced Input trampoline stubs, filtered as noise in the catalog).

**inventory_jigsaw (8):**
`BP_JigHelperComp_C_OnRep_RepActorsData.bin`,
`BP_JigMultiplayer_C_ExecuteUbergraph_BP_JigMultiplayer.bin`,
`BP_JigMultiplayer_C_ItemDropRequest_Event_0.bin`,
`BP_WeaponsPickupComponent_C_ExecuteUbergraph_BP_WeaponsPickupComponent.bin`,
`JSIContainer_C_OnDrop.bin`, `JSIContainer_C_PerfromDrop.bin`,
plus `BP_MeleePickup_C_MeleeTrace.bin` (pickup-adjacent, grouped here).

**anim_input_wip (Player_AnimBP_C, 10):**
`BlueprintThreadSafeUpdateAnimation.bin`, `CombatState.bin`,
`EvaluateGraphExposedInputs_ExecuteUbergraph_Player_AnimBP_AnimGraphNode_Fabrik_31BAC05D486A8E2F5D5AFF87706BFF20.bin`,
`EvaluateGraphExposedInputs_ExecuteUbergraph_Player_AnimBP_AnimGraphNode_Fabrik_AF8CB55449FDEB26FA14F38357C17595.bin`,
`EvaluateGraphExposedInputs_ExecuteUbergraph_Player_AnimBP_AnimGraphNode_TransitionResult_1D52F779406FC4110BBFC4B70C211957.bin`,
`ExecuteUbergraph_Player_AnimBP.bin`, `GetAimOffset.bin`, `GetAnimationInfoFromCharacter.bin`,
`GetHeadRot.bin`, `GetLeftHandLoc.bin`, `GetSpeed&Direction.bin`, `GetThreadSafeBooleans.bin`
(11 files listed but grouped as 10 named functions — `GetThreadSafeBooleans` and
`GetSpeed&Direction` both present).

**misc_systems (PressAnyKeyWidget_C, 3):**
`ExecuteUbergraph_PressAnyKeyWidget.bin`, `OnKeyDown.bin`, `Tick.bin`.

**Uncataloged raw diagnostic captures (10)** — memory/animation snapshots, not tied to one
named catalog function; still on disk and re-inspectable:
`armpose_local_1h_pistol.bin`, `armpose_local_2h_shotgun.bin`, `armpose_proxy_2h_shotgun.bin`,
`fabrik_local_AnimGraphNode_Fabrik_6.bin`, `fabrik_local_AnimGraphNode_Fabrik_7.bin`,
`fabrik_proxy_AnimGraphNode_Fabrik_6.bin`, `fabrik_proxy_AnimGraphNode_Fabrik_7.bin`,
`noisefloor_take1.bin`, `noisefloor_take2.bin`, `pbfg.bin`, `ubergraph.bin`.

## High priority for next capture

Per `research/04_ida_investigation_log.md`'s latest entries — **the priority list has moved on
since the mid-session `RepPrimitiveActorsData` lead** (task brief's suggested list reflects that
now-superseded state, not the log's current end state). Timeline within the same log:

1. Mid-session (`## Static BP catalogs built`, `## CRITICAL FIND`): `RepPrimitiveActorsData`'s
   `OnRep_` handler, `ForceRepPrimitiveActorSpawns`, `UpdatePrevFromPrim` were flagged the
   **single highest-priority** live-capture targets, based on `RepActorsData` vs
   `RepPrimitiveActorsData` struct-shape analysis (extra `Primitive` field).
2. Later the same session (`## Live IDA attach, attempt 2`): a live IDA read during an active
   cascade found **every `RepPrimitiveActorsData` entry's `Primitive` pointer healthy** while
   `Torso.SkeletalMeshAsset` was simultaneously null — a live-confirmed negative result.
   Explicit log conclusion: **"Drop the RepPrimitiveActorsData theory. Do not chase it further
   without new evidence."**

So `OnRep_RepPrimitiveActorsData`, `ForceRepPrimitiveActorSpawns`, and `UpdatePrevFromPrim`
remain **not captured and not decoded**, but are now deprioritized, not high-priority, per the
log's own final word on that lead.

Still-open, not-yet-superseded candidates from the log (neither captured nor decoded):

- `BP_JigMultiplayer_C::WaitFullReplicationOfUIDs` — flagged mid-session as an existing
  replication-wait helper worth reading before building custom repair logic; never retracted,
  never captured.
- `BP_JigPickupComponent_C::CheckMismatch` — flagged same session as an existing consistency
  check; never retracted, never captured.
- `MC_AttachClothing` (BP_PlayerCharacter_C) — the multicast counterpart to
  `Svr_AttachClothing` (which decoded as a native stub); never itself captured, so still
  unconfirmed whether it's also a stub or holds real Blueprint logic.

The investigation's actual current leading edge (per the log's final entries) is **not** a new
Blueprint function to decode: `Svr_AttachClothing` is a confirmed native stub, and the
`RepPrimitiveActorsData` replication-data path was live-ruled-out, pointing the root cause toward
a **native asset-streaming/async-load issue** outside Blueprint bytecode's visibility — plus
several still-unverified live fixes (`bReinitPose=false` correction, `ScaleRule` mismatch fix,
positional-drift re-snap). None of those are bytecode-capture work.
