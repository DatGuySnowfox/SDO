# Audit Findings — To Fix

Running list of confirmed issues found by auditing the mod's C++ against both general code-quality
review and tonight's bytecode-decode ground truth. Each entry: file/line, what's wrong, why, and a
suggested fix. Not yet applied unless marked `[FIXED]` — this is a to-do list, not a changelog.
Comment-only corrections (updating stale/disproven comments) are NOT tracked here — those were applied
directly since they carry no behavior risk; see `04_ida_investigation_log.md` for that list.

## From the general code-quality audit (earlier tonight, background-agent pass)

1. **`[FIXED]` Unvalidated network `slotIndex` used as a bit-shift amount** — `proxy_manager.cpp:1932`.
   `slot.slotIndex` comes straight off the wire (`protocol.cpp:587`) as a raw `uint8_t`, no bounds check
   against `EQUIPMENT_SLOT_COUNT` (21) before the shift, unlike every other consumer in this file (which
   all go through `slot_tag()`'s own check). Fixed 2026-08-16: `if (slot.slotIndex >= EQUIPMENT_SLOT_COUNT)
   continue;` before the shift, matching the established pattern. Built clean, deployed to PC1 and PC2.

2. **`[DOCUMENTED, NOT CHANGED]` `TcpClient::shutdown()` touches `sock_` off the TCP thread** —
   `tcp_client.cpp:91-97` / `tcp_client.hpp:82`. Traced 2026-08-16: the only cross-thread write is
   `shutdown()` → `close_socket()`, called exactly once at mod-unload, never during normal operation —
   confirmed via reading every call site. `closesocket()`/`close()` on a socket another thread is blocked
   on inside `select()`/`recv()` is a standard, well-defined way to unblock it (both Winsock and POSIX
   guarantee this), which is exactly what this is for. Converting `sock_` to `std::atomic` would touch 13
   call sites across `run_connected()`/`recv_chunk()`/`write_all()`/`close_socket()` for a benefit that's
   already covered by the one-shot, teardown-only nature of the exception — decided against it tonight
   specifically because a networking-code refactor isn't safely verifiable without a live game to test
   reconnect/teardown against. Documented precisely in `tcp_client.hpp` instead (was previously an
   inaccurate blanket "ONLY touched by the TCP thread" claim). Comment-only, no behavior change.

3. **`[INVESTIGATED, LEFT AS-IS]` `do_game_tick`'s reentrancy guard is a plain `bool`, not atomic** —
   `mod.cpp` (`static bool s_in_game_tick`). Still only reliably guards same-thread recursion; whether
   `do_game_tick(false)` can ever be reached from a second thread concurrently remains unconfirmed either
   way. Not touched tonight — a plain-bool→atomic swap here is mechanical and low-risk on its own, but
   the actual open question (can a second thread reach this function at all) is unverifiable without a
   live process to trace `ProcessEvent`'s calling threads against, so fixing the primitive wouldn't close
   the actual uncertainty. Worth a look if a future live session can confirm or rule out the second-thread
   path.

## New findings from tonight's decode-informed pass

4. **`OwnerMPComp` — confirmed never populated by this project's own code (zero references anywhere in
   `src/`)**. Traced tonight: `UpdateAttachments()`/`ProcessAttachments()` (the real native attachment
   pipeline) both read attachment data through this exact field (`InstanceVariable 0x1244b4500` in that
   decode, resolved to `BP_JigMultiplayer_C`, holding `IsValidPickup`/`GetListOfAttachments`). If it's
   null on this project's own spawned proxy-visual items, calling `UpdateAttachments()` on them would
   silently no-op at its own `IsValidPickup` gate. **Not yet verified whether the game's native spawn
   flow (`BeginDeferredActorSpawnFromClass`/construction script) populates it automatically as a side
   effect** — would need a live pointer to one of this project's own spawned item actors (have a
   `DefaultSceneRoot` component pointer from `attach_health` logging, e.g.
   `BP_MilitarySuppressorLocalAttachment_C_2147475269`, but not the owning actor pointer itself, and
   getting from one to the other needs either a new diagnostic or several more live round-trips) — parked
   rather than chased further right now. **Practical takeaway stands regardless**: `ValidateAttachedActor`
   (the other fix candidate) doesn't touch `OwnerMPComp`/`IsValidPickup` anywhere in its own decode, so
   it's unaffected by this question either way — one more reason to prefer it as the first fix to try.

5. **`[DEPLOYED, LIVE-TESTED — DETECTION GAP FOUND]` `ValidateAttachedActor`-style transform-snap for
   weapon-attachment drift.** `do_attach_health_scan`'s OFF-SOCKET repair (`mod.cpp`, the absolute-check
   block) calls `GetOwner()` → `GetSocketTransform` → `K2_SetActorTransform(..., bTeleport=true)` instead
   of a bare `K2_AttachToComponent`, matching the real game's own repair pattern exactly. Kept strictly
   reactive — gated on `offSocket`, same throttle as before, no unconditional/timer variant. Live-tested
   tonight (PC2 helmet, then PC1 shotgun/pistol): **the repair itself was never observed to fire, because
   its own `offSocket` gate never trips for this bug class** — `K2_GetComponentLocation` reads the correct,
   attached position even while the rendered pixels show the item sitting elsewhere entirely (confirmed
   directly: PC1's shotgun/pistol read `socket-dist` 0.0-3.0 the whole time a screenshot showed them lying
   on the ground). The fix logic itself is unverified as a *cure*, since it's never actually run — the real
   finding is that detection, not repair, is the bottleneck. See item 9 (forensic capture) and item 6
   (WndProc risk-calculus update) for the two paths toward actually closing this.

6. **`[RE-ENABLED, ONE OF TWO — NOT YET LIVE-TESTED]` Proactive leader-pose refresh re-enabled; unconditional
   weapon re-snap stays off.** Discovered the standing WndProc clean-GameThread-trigger plan is not just
   planned but fully implemented and running (`ensure_hwnd_ticker_started`/`sdb_wnd_proc`, `mod.cpp`
   ~6675-6762), and both kill-switched call sites already live *inside* its `cleanContext`-gated path
   (`check_component_drift` runs right alongside `check_attach_health` in the same
   `if (cleanContext && ...)` block, `mod.cpp:3838-3843`). **Correction to initial read**: re-checked the
   original crash's own comment closely — its stack already ran through the WndProc path when it happened,
   so `cleanContext` being clean does NOT by itself rule out a repeat; the crash's actual root cause is
   still unidentified, not confirmed to be nesting-related. The more specific new protection is that
   `do_game_tick_clean_ctx` (the WndProc handler's entry point) picked up an SEH wrap after that crash,
   matching the catch-and-log pattern already proven repeatedly elsewhere in this codebase — so a repeat of
   the same unhandled-C++-exception failure mode should now be caught and logged instead of taking the
   process down, though that's inference from a proven pattern, not a tested guarantee for this exact
   call. `t_processEventDepth` evidence (1852/1852 clean checks, 7-hour/45-relaunch session) still stands
   as confirmation this call isn't running from a provably-bad context, just isn't the whole safety story
   on its own. User explicitly authorized proceeding (twice) despite being unavailable to watch a live
   test, on the basis that a repeat failure is recoverable (relaunch only, no data loss — SQLite-backed
   persistence). Re-enabled `kEnableProactiveLeaderPoseRefresh` only (`mod.cpp`, still 20s throttle); the
   transform-snap unconditional variant was NOT part of this decision and stays off. Built clean, deployed
   to PC1 and PC2 while both were closed. **Next session: check debug.log first** for either a hang
   signature or an SEH-caught exception log line before assuming it's fine — the log itself is the honest
   verdict here, not this write-up's reasoning.

7. **`[RESOLVED]` `check_dump_delegate_trigger()` added and live-tested.** Answered "who's subscribed to
   `OnAttachmentsUpdated`": **`count=1`**, a real subscriber (unlike `OnEquipmentUpdated`'s `count=0`).
   The bound function is named `OnAttachmentsUpdated` (same as the delegate — the standard "Bind Event
   to..." auto-generated Custom Event signature), strongly suggesting real logic reacts to this
   broadcast. Full detail in `04_ida_investigation_log.md`'s "OnAttachmentsUpdated — the 'who's
   listening' question, answered live" entry. Both fix candidates (this broadcast, or
   `ValidateAttachedActor`'s transform-snap) are now equally well-grounded — item 5 above still applies
   for actually testing either live. Not yet resolved: which exact object/class owns that Custom Event
   (would need to walk `GUObjectArray` from the raw `ObjectIndex=175432`/`SerialNumber=66964` captured —
   not necessary to attempt the fix, just satisfies curiosity about self-bind vs. some other system).

## Resolved, no action needed

- `Client_Died`'s tail past the `Difficulty.Permadeath` CVar branch: on permadeath, calls
  `PrintString(Self, "Game Save Failed", ...)` then builds a struct (fields include a "A" string const,
  byte const 4, int64/float zeroes) — reads like save-file deletion/invalidation handling for permadeath
  mode. Unrelated to any bug this project is chasing; not traced further.
- `EquipActorToSocket`'s `IsSecondary`-branched socket-select comment: cross-checked against the actual
  decoded bytecode (`decoded_EquipActorToSocket.txt`) — the claimed `GetComponentByClass` →
  `ItemDataAsset` socket-select → `K2_AttachToComponent` shape is confirmed correct, not another instance
  of the "comment overclaims" pattern. No fix needed.
- Server-side (`gateway.js`) spot-check: `Equipment`/`WeaponAttachments`/`PawnAppearance` frames are
  deliberately relayed as opaque bytes with no server-side parsing (documented as intentional,
  client-authoritative cosmetic data) — confirms the `slotIndex` bounds-check gap (item 1) really does
  belong in the C++ deserializer, not the gateway. Host-auth secret comparison already uses
  `crypto.timingSafeEqual` with a correct length pre-check — no timing-attack issue found.

## New findings from the PC1 shotgun/pistol research session (2026-08-16)

8. **`[FIXED, DEPLOYED]` `find_and_claim_native_pickup` could claim a still-equipped item as a
   "dropped" entity.** `entity_manager.cpp:242` matched by class name + nearest-position-within-500-units
   only — no check that the candidate isn't currently attached to something. Equipped items are live
   instances of the same Pickup class as a real ground drop, and "within 500 units" is trivially true for
   anything on the player's own body. A later legitimate `EntityDespawn` for such a misclaimed entity would
   call `K2_DestroyActor()` on the player's real equipped weapon. Investigated for tonight's specific
   PC1 shotgun/pistol incident and **ruled out as the cause** (zero entity traffic for either item in either
   log) — but real and worth closing regardless. Fixed: skip any candidate whose `RootComponent->
   AttachParent` is non-null. Built clean, deployed to PC1 and PC2.

9. **`[IMPLEMENTED, DEPLOYED]` Recent-calls forensic watch widened to cover weapon/item actors.**
   `mod.cpp`'s `dump_recent_calls()` ring buffer only ever watched the clothing investigation's 5 objects —
   confirmed live that a real weapon DETACHED hit (PC1's shotgun, 21:42:04) produced a dump with nothing
   useful in it. Added a second, independently-refreshed watch region (`set_item_recent_calls_watch`,
   mirrors `check_attach_health("local", ...)`'s own children list every poll) so the next natural
   reproduction actually captures the causal ProcessEvent chain instead of nothing. Diagnostic-only, no
   gameplay behavior change. Built clean, deployed to PC1 and PC2.

## New finding, 2026-08-17: likely actual root cause identified via real UE5 engine research

10. **`[IMPLEMENTED, DEPLOYED, CVAR CONFIRMED APPLIED — AWAITING SOAK]` `a.ParallelAnimEvaluation 0` —
    candidate fix for the real root cause of tonight's whole render/animation-desync investigation.**
    First deploy failed silently: `FindFirstOf(L"KismetSystemLibrary")` never found a live instance (6+
    retries, 12+ real seconds of active play on both machines) — root cause, not just a timing fluke:
    `FindFirstOf` matches by class name, but a class-default-object needs to be looked up by its own actual
    object name, which UE always sets to `Default__<ClassName>` (`UClass::GetDefaultObjectName()`, a
    hardcoded engine-wide convention). Fixed via `UObjectGlobals::FindObject(nullptr, ANY_PACKAGE,
    L"Default__KismetSystemLibrary")` — the same exact-name lookup `resolve_class_by_name` already uses
    elsewhere in this codebase — with the original `FindFirstOf` kept as a fallback. **Confirmed live
    2026-08-17 on both machines**: `debug.log` shows `"parallel_anim_eval: executed
    'a.ParallelAnimEvaluation 0'"` on PC1's and PC2's first relaunch after this second fix. Live-reproduced
    yet again right before this fix went in (2026-08-17 morning): PC2 reported the local pawn's body mesh
    (torso/arms/legs/feet) animation frozen solid while still correctly attached — only backpack/helmet
    (independently-attached actors, not leader-pose followers) kept animating. `component_drift`'s own
    "giving up PERMANENTLY" fired for many components on both PC1 and PC2 simultaneously right as this was
    reported. Researched against real UE5 engine behavior (web search + Epic's own issue tracker) instead
    of guessing further
    (2026-08-17 morning): PC2 reported the local pawn's body mesh (torso/arms/legs/feet) animation frozen
    solid while still correctly attached — only backpack/helmet (independently-attached actors, not leader-
    pose followers) kept animating. `component_drift`'s own "giving up PERMANENTLY" fired for many
    components on both PC1 and PC2 simultaneously right as this was reported. Researched against real UE5
    engine behavior (web search + Epic's own issue tracker) instead of guessing further: **UE-191796**, a
    confirmed Epic bug affecting exactly engine 5.2 and 5.3 (this game is 5.3.2) — in 5.2 a skeletal mesh's
    bone-buffer swap moved from the sequential PrePhysics tick to the *parallel* end-of-frame render-data
    update, creating a race where a leader-pose follower's parallel task can run before its leader's buffer
    swap finishes, reading stale bone data. This plausibly explains not just tonight's animation freeze but
    the whole night's earlier "attached correctly but rendered in the wrong place" saga too — a socket's
    world transform is derived from bone data, so a stale-boned leader can make its followers' sockets read
    wrong even though every one of this project's own AttachParent/distance checks (which read that same
    transform) reports clean. A related Epic forum thread describes the identical symptom class (random
    skeletal-mesh detach + animation stop) specifically on packaged builds with networked/proxy characters —
    this project's exact setup. Epic's fix is source-level only, not in any shipped 5.3.2 patch. Implemented
    the one lever actually available from outside the engine: `ensure_parallel_anim_eval_disabled()`
    (`mod.cpp`, next to `ensure_hwnd_ticker_started`) fires `a.ParallelAnimEvaluation 0` once at startup via
    `UKismetSystemLibrary::ExecuteConsoleCommand` (a standard BlueprintCallable static, called through this
    project's own established `GetFunctionByNameInChain`+`ProcessEvent` pattern), forcing all animation
    evaluation back onto the game thread sequentially — eliminating the race by construction rather than
    detecting-and-repairing after the fact. One-shot at startup, not recurring — a fundamentally lower risk
    class than every other proactive-repair attempt tonight. Trade-off: costs some CPU (that's what parallel
    anim eval is for), likely immaterial for a 1-2 player session. **Next step: play a real session and
    watch for any recurrence of the frozen-animation or misplaced-item symptoms** — the CVar is confirmed
    active on both machines now (log-verified, not just deployed); if neither symptom recurs over a real
    session, this is very likely the actual fix, not just a mitigation.

## New feature, 2026-08-17: flashlight/NVG/weapon-light attachment sync

11. **`[FLASHLIGHT CONFIRMED WORKING LIVE — NVG/weapon-lights still untested]` Flashlight, night vision,
    and weapon-mounted laser/light toggle state now sync to proxies.** Three-part feature, requested
    directly (character flashlight, NVG, weapon lasers/flashlights all previously showed no on/off state
    on a proxy).
    - **Character flashlight**: `[CONFIRMED WORKING LIVE, 2026-08-17]`. First two attempts both failed
      live despite `ProcessEvent` genuinely reaching the target function every time with the right
      argument: (1) calling the real `FlashlightToggle(bool)` — no visible effect; (2) theorizing a
      `FlashlightEquipped?`@0x1DD0 gate — already true, not the blocker; (3) bypassing to a direct
      `SetVisibility` on the Flashlight component — still no effect. Root-caused by dumping
      `FlashlightToggle`'s own bytecode live (`bytecode_dump.flag` against the running process) instead of
      guessing a third time: the real mechanism is `ULightComponentBase::SetIntensity(float)` +
      `ULocalLightComponent::SetAttenuationRadius(float)` — UE spotlights are commonly toggled by zeroing
      intensity, not touching visibility, specifically to avoid recreating the render proxy every toggle.
      `AttenuationRadius` is a bytecode constant (16384.0 on / 0.0 off); `Intensity`'s on-value is
      per-equipped-item (a computed local in the bytecode, not a constant), so `PlayerLights`' wire format
      was extended with a `flashlightIntensity` float, read live off the sender's own component
      (`read_local_player_lights`, `mod.cpp`) rather than guessed, and applied via the same two real
      native functions on the proxy's Flashlight component (`sync_player_lights`, `proxy_manager.cpp`).
      New frame: `MsgType::PlayerLights` (`protocol.hpp`/`.cpp`). Read straight off the pawn
      (`FlashlightOn?`@0x13E5, `PlayerUsingNightVision?`@0x1401, plain bools, no ProcessEvent needed) in
      `send_player_lights()`, unthrottled/change-detected so it fires within one tick of an actual toggle.
      **A second, separate bug was also found and fixed along the way**: the server's own `gateway.js`/
      `protocol.js` had no knowledge of the new `MsgType.PlayerLights` value at all — the C++/mod side was
      completely correct from the start, but the live gateway process was silently dropping the frame
      server-side since its switch statement had no matching case (no default case either, so no error,
      just silence). Fixed in both files; required an actual gateway process restart to take effect
      (Node.js doesn't hot-reload) — both clients auto-reconnected cleanly via their existing backoff
      logic, no game relaunch needed.
    - **Night vision**: still calls the real `NightVisionOn(bool)` function directly, unverified live —
      given the flashlight lesson, this may have the exact same "ProcessEvent reaches it, does nothing
      visible" problem and need its own bytecode decode. Not yet tested.
    - **Weapon-mounted lasers/flashlights**: `WeaponAttachmentEntry` gained a `bool active` field (wire
      format extended, both ends updated together — no backward-compat concern, this frame isn't
      persisted anywhere). Populated in `read_local_weapon_attachments()` by walking each weapon's own
      AttachChildren one level deeper (mirrors `do_attach_health_scan`'s existing nested-scan pattern) to
      find the live `BP_<Item>LocalAttachment_C` actor instances (a completely different class hierarchy
      from the pickup/weapon actors — no `BP_JigPickupComponent`, itemId comes from
      `ABP_AMainLocalAttachment_C::Attachment Info`@0x02C0 instead) and reading their shared
      `ActivateState` FGameplayTag (@0x02C8) — one generic field covers all four toggleable attachment
      types (`TacticalLaserLightCombo`/`SmallTacticalLight`/`SmallTacticalLaserSight`/`TacticalLight`,
      confirmed siblings under the same base class). Applied via `Jig_ToggleAttachmentActiveState()`
      (parameterless) on the proxy's spawned attachment actor, comparing its own freshly-spawned
      `ActivateState` against the desired state and toggling at most once — deliberately NOT using
      `Jig_SetAttachmentActiveState(FGameplayTag)` despite it existing, since this project hasn't decoded
      which specific tag value(s) it expects for on vs off, only that some tag drives an `IsActive`-style
      bool via the subclass's own `OnActiveStateChanged()` override.
    - **Heuristic not yet live-confirmed**: "active" is read/compared as `ActivateState.ComparisonIndex !=
      0`, not a decoded specific tag value — matches this project's own "don't hardcode a GameplayTag CI"
      caution in spirit (no CI is hardcoded), but the *zero vs non-zero* semantics themselves are an
      assumption, not a confirmed decode. If lights don't visually toggle correctly on a proxy, this is
      the first thing to re-examine — likely by bytecode-dumping `Jig_SetAttachmentActiveState` fresh
      against a live session while manually toggling a light, to see the exact tag values involved.
    - Change-signature for `sync_weapon_attachments`'s existing respawn-on-change logic now folds in
      `active` too, so a pure toggle (itemId unchanged) is treated as a real change and triggers the
      respawn-and-reapply cycle — not the cheapest possible path (destroys and respawns the whole
      attachment actor just to flip a light) but reuses the already-correct, already-tested spawn
      pipeline instead of adding a second, narrower incremental-update path.
    - All 493 existing protocol round-trip test assertions still pass (`protocol_test.exe`) — confirms
      nothing existing broke, but no new test cases were added for `PlayerLights` or `WeaponAttachmentEntry
      .active` specifically; worth adding if this needs revisiting.
    - Built clean, deployed to PC1 and PC2 while both were closed. **Next step: relaunch both, toggle a
      flashlight/NVG/weapon light on one client, confirm it shows correctly on the other's proxy view** —
      watch `debug.log` for `send_player_lights`/`on_player_lights`/`sync_player_lights` and the
      `Jig_ToggleAttachmentActiveState` call path for confirmation either way.

## Still open, not yet investigated

- Item 10: live confirmation that `a.ParallelAnimEvaluation 0` actually prevents recurrence, and whether
  the earlier "attached but rendered wrong" bug (item 5) is also resolved by the same fix.
- If item 10 does NOT fully resolve things, the `MarkRenderTransformDirty`-notification-gap theory (item 5)
  and item 9's widened forensic capture are still the fallback investigation path.
- Item 6's decision: whether to re-enable the proactive leader-pose refresh and/or unconditional
  transform-snap now that the WndProc clean-trigger evidence substantially weakens the original
  hang/crash-driven justification for leaving them off.
- Item 11's live test, and the fishing-sync feature (requested, not yet investigated — likely a different
  mechanism, probably an animation/actor-state sync rather than a simple toggle).
