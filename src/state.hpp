#pragma once
#include "protocol.hpp"
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace sdb {

// Per-remote-player state.
struct RemotePlayer {
    uint64_t playerId   = 0;
    float    x = 0, y = 0, z = 0;
    float    yaw        = 0.0f;
    float    aimYaw     = 0.0f;
    // Smoothed position/yaw actually applied to the proxy each tick,
    // separate from the raw x/y/z/yaw above (which jump directly to whatever
    // the last-received network packet said, every ~50ms per
    // SDB_MOVE_INTERVAL_MS). teleport_proxy calling K2_SetActorLocationAnd-
    // Rotation with bTeleport=true every do_game_tick (~5ms) against the raw
    // fields just re-snaps to an unchanged value between packets, then jumps
    // instantly on the next one — visually "teleporty" (live-tested
    // 2026-08-13; body-rotation fix landed clean, this is the separate,
    // remaining issue). ProxyManager::tick() exponentially smooths render*
    // toward x/y/z/yaw each tick instead, snapping directly only on a large
    // jump (a real teleport/respawn, not just normal movement).
    bool     renderInitialized = false;
    float    renderX = 0, renderY = 0, renderZ = 0, renderYaw = 0;
    uint64_t lastRenderTickUs = 0;
    float    health     = 100.0f;
    bool     dead       = false;
    // Tracks whether the AnimBP's own DeathState(bool) has been called with
    // the CURRENT value of `dead` — edge-triggered the same way handAttachedSlot
    // tracks activeWeaponSlot, so the ProcessEvent call only fires on an
    // actual death/respawn transition, not every tick. See proxy_manager.cpp's
    // call_death_state for what this actually drives.
    bool     deathStateApplied = false;
    uint8_t  movState   = 0;
    // Repurposed from the wire's unused Movement.animationState byte
    // (2026-08-13) — carries the sender's real active/drawn weapon slot
    // (11-14) from BP_JigHelperComp_C::GetActiveWeaponSlot(), or 0xFF for
    // none. Applied in ProxyManager::sync_active_weapon_hand to move the
    // right weapon visual onto the proxy's hand socket instead of leaving
    // every weapon sitting in its holstered/unequipped socket regardless of
    // which one is actually being held.
    uint8_t  activeWeaponSlot = 0xFF;
    // Which slot (if any) is currently re-attached to the hand socket —
    // compared against activeWeaponSlot each tick so a change reverts the
    // previous weapon to its holster socket before attaching the new one.
    uint8_t  handAttachedSlot = 0xFF;
    // Real velocity read straight off the sending player's own
    // CharacterMovementComponent (mod.cpp's send_movement) — applied to the
    // proxy's own CharacterMovementComponent::Velocity in ProxyManager::tick()
    // so the proxy's AnimBP (same Blueprint class as the real player, so the
    // same Speed/MovementState blend logic already exists) drives real
    // walk/run animation instead of a static default pose. Position itself
    // is still hard-teleported each tick; only velocity feeds the animation.
    float    velocityX = 0, velocityY = 0, velocityZ = 0;
    // Repurposed from the wire's unused Movement.aimState byte (2026-08-13)
    // — a quantized look-pitch, read via GetControlRotation() on the
    // sender's pawn. NOT applied via RemoteViewPitch (this game's AnimBP
    // doesn't read it) — applied by mod.cpp's on_process_event_post
    // straight into the AnimBP's own "Pitch" instance property immediately
    // after GetAimOffset's own per-frame recompute, since a same-tick write
    // here always loses that race. See on_process_event_post's own comment.
    uint8_t  aimPitchByte = 0;
    // Smoothed aim pitch/yaw (degrees, [-180,180]) actually written to the
    // AnimBP each frame — same reasoning as renderX/Y/Z/Yaw above:
    // aimPitchByte/aimYaw only change once per network packet (~50ms), and
    // on_process_event_post writing the raw value straight into Pitch/Yaw
    // every frame in between produced a visible step/stutter rather than
    // continuous motion (2026-08-13, most noticeable while ADS since the
    // camera is zoomed in). Smoothed toward the raw values in
    // update_proxy_render_smoothing using the same exponential/kTau approach
    // already proven for position.
    bool     aimRenderInitialized = false;
    float    renderAimPitch = 0, renderAimYaw = 0;
    // Turn-in-place accumulator (2026-08-13) — persistent, NOT reset by each
    // incoming Movement packet (unlike yaw/x/y/z above). First attempt had
    // turn-in-place nudge renderYaw directly, in the same tick as (and right
    // before) the ordinary body-yaw smoothing step that pulls renderYaw
    // toward the raw synced `yaw` — since that smoothing reapplies a
    // proportional restoring force toward `yaw` every single tick regardless,
    // it fought the turn-in-place push to a stable equilibrium instead of
    // ever letting it close the gap (live-confirmed: offset plateaued around
    // 112 degrees for a full 5-second window instead of decaying toward the
    // 70-degree threshold). Fixed by adding this offset to the body-yaw
    // smoothing's *target* (yaw + turnInPlaceYawOffset) instead of nudging
    // renderYaw as a separate, competing force.
    float    turnInPlaceYawOffset = 0;
    // Mesh component's own RelativeRotation the first time it's read after
    // spawn (2026-08-13) — captured once and reused as the baseline every
    // subsequent tick's body-yaw write. Needed because BP_PlayerCharacter's
    // CharacterMovementComponent has bOrientRotationToMovement=true and
    // bUseControllerRotationYaw=false (confirmed via FModel export), which
    // resets the actor root's own rotation from Velocity every physics
    // tick — any K2_SetActorLocationAndRotation call on the actor root gets
    // silently overridden (confirmed live: actualYaw stayed pinned at 0.00
    // regardless of what was sent). Writing yaw onto the Mesh component's
    // RelativeRotation instead sidesteps that mechanism entirely, but the
    // mesh has its own baked-in alignment offset (typically ~-90 degrees
    // for a UE mannequin-based character) that must be preserved, not
    // overwritten, or the proxy would render rotated 90 degrees off.
    bool     meshBaselineCaptured = false;
    double   meshBaselinePitch = 0, meshBaselineYaw = 0, meshBaselineRoll = 0;
    void*    proxyActor = nullptr;
    uint64_t updatedUs  = 0;
    uint64_t lastSpawnAttemptUs = 0; // throttles retry after a failed spawn_proxy()
    // Set the moment spawn_proxy() succeeds. Live-tested 2026-08-13: a
    // freshly-spawned proxy hit by sync_equipment/sync_weapon_attachments/
    // sync_pawn_appearance's full burst of ProcessEvent calls immediately
    // crashed/deadlocked PC2 twice, at two different specific call sites
    // each time — consistent with the proxy's own components (mesh,
    // skeleton) not being fully ready the instant FinishSpawning() returns,
    // not one specific bad call. ProxyManager::tick() holds off on all the
    // heavy sync calls until now_micros() - proxySpawnedAtUs clears a short
    // grace period.
    uint64_t proxySpawnedAtUs = 0;
    std::vector<EquipmentSlot> equipment; // last Equipment frame received, for proxy appearance sync
    bool equipmentDirty = false; // set by on_equipment(), cleared once ProxyManager::tick() applies it
    void* primaryWeaponVisualActor = nullptr; // spawned PickupClass actor attached to the proxy's EquipSocket
    std::string primaryWeaponVisualItemId;    // itemId the actor above was spawned for — respawn only on change
    void* facewearVisualActor = nullptr; // spawned PickupClass actor equipped via the real EquipActorToSocket path
    std::string facewearVisualItemId;    // itemId the actor above was spawned for — respawn only on change
    void* headwearVisualActor = nullptr; // spawned PickupClass actor equipped via the real EquipActorToSocket path
    std::string headwearVisualItemId;    // itemId the actor above was spawned for — respawn only on change
    void* eyewearVisualActor = nullptr; // spawned PickupClass actor equipped via the real EquipActorToSocket path
    std::string eyewearVisualItemId;    // itemId the actor above was spawned for — respawn only on change
    void* backpackVisualActor = nullptr; // spawned PickupClass actor equipped via the real EquipActorToSocket path
    std::string backpackVisualItemId;    // itemId the actor above was spawned for — respawn only on change
    void* secondaryWeaponVisualActor = nullptr; // spawned PickupClass actor equipped via the real EquipActorToSocket path
    std::string secondaryWeaponVisualItemId;    // itemId the actor above was spawned for — respawn only on change
    void* sidearmVisualActor = nullptr; // spawned PickupClass actor equipped via the real EquipActorToSocket path
    std::string sidearmVisualItemId;    // itemId the actor above was spawned for — respawn only on change
    void* meleeVisualActor = nullptr; // spawned PickupClass actor equipped via the real EquipActorToSocket path
    std::string meleeVisualItemId;    // itemId the actor above was spawned for — respawn only on change

    // "Respawn treadmill" mitigation (long-documented, never root-caused —
    // live-tested 2026-08-12: slot 14/Knife respawned dozens of times with a
    // new actor pointer every time despite the itemId genuinely never
    // changing, piling up orphaned duplicate visuals). The itemId-change
    // check alone isn't reliable enough to prevent this — cap how often any
    // one equipment slot is allowed to actually respawn its visual actor,
    // regardless of what the comparison says, as a safety net independent of
    // whatever the real underlying cause turns out to be.
    std::unordered_map<uint8_t, uint64_t> lastVisualRespawnUs;

    // Tracks, per equipment slot, the itemId last actually pushed through the
    // core equip-flag pipeline (SetEquippedInfoBySlot + ActiveWeaponSlot
    // activate/onRep/notify for weapon slots, equip_clothing_to_mesh for
    // clothing slots) — separate from the *VisualItemId fields above, which
    // only track the spawned-actor side. Equipment frames resend the full
    // snapshot every ~2s even when nothing changed (live-tested 2026-08-12:
    // this pipeline was re-running unconditionally on every resend, and for
    // weapon slots the onRep/notify calls visibly re-trigger the game's own
    // draw/holster handling each time — the "pulsing" a proxy showed even
    // with no real equipment change). Gate the write-side calls on this
    // instead of re-running them for byte-identical data every pass.
    std::unordered_map<uint8_t, std::string> appliedEquipItemId;

    // Consecutive-missing counter per slot for the unequip-clear check below.
    // Live-tested 2026-08-12: PC2's own read_local_equipment() occasionally
    // reports a genuinely-still-equipped slot as absent for exactly one
    // ~2s frame (e.g. slot 4/BlueShirt vanishing then reappearing on the very
    // next frame) — a one-frame read glitch on the sender, not a real
    // unequip. Clearing immediately on a single missing frame turned that
    // glitch into a real clear+reapply cycle on the proxy (the residual
    // "flashing" after the change-gate above). Require a slot to be missing
    // for 2 consecutive frames before treating it as a real unequip.
    std::unordered_map<uint8_t, int> missingSlotStreak;

    // Last WeaponAttachments frame received (flat list across all 4 weapon
    // slots), applied in ProxyManager::sync_equipment once the owning
    // weapon's own visual actor exists. weaponAttachmentsAppliedKey[slot] is
    // a cheap signature (concatenated itemIds) of what's currently spawned
    // for that slot, so a resend with no real change doesn't respawn
    // anything — same "don't reprocess unchanged data" lesson as the
    // redundant-resend gate above for Facewear/Eyewear/Backpack.
    std::vector<WeaponAttachmentEntry> weaponAttachments;
    bool weaponAttachmentsDirty = false;
    std::unordered_map<uint8_t, std::string> weaponAttachmentsAppliedKey;
    std::unordered_map<uint8_t, std::vector<void*>> weaponAttachmentActors;

    // Last PawnAppearance frame received, applied in
    // ProxyManager::sync_pawn_appearance. appliedAppearanceKey is a cheap
    // signature of what's currently applied, so a resend with no real change
    // is a no-op.
    PawnAppearance appearance;
    bool appearanceDirty = false;
    std::string appliedAppearanceKey;
    // 2026-08-15: sync_pawn_appearance used to apply hair/beard/mouth/
    // eyebrows/accessories/9 body parts/skin color all in one call — up to
    // ~40 ProcessEvent calls in a single nested pass. Since do_game_tick()
    // can only run from inside UE4SS's ProcessEvent pre-hook (see
    // do_game_tick's own comment, mod.cpp), every one of those is itself
    // nested inside the engine's own outer dispatch — a live-captured hang
    // dump traced a freeze to exactly this function (see the bodyPart loop's
    // own comment below). Splits the work into stages (0=hair/beard,
    // 1=mouth/eyebrows/accessories, 2-10=one body part each, 11=skin color),
    // one stage applied per ProxyManager::tick() call — reset to 0 whenever
    // a fresh PawnAppearance frame arrives (on_pawn_appearance), since a
    // stage cursor mid-sync against stale target data would apply a mix of
    // old and new values.
    int appearanceSyncStage = 0;

    // Bit i set = slot i was actually written to the proxy on a previous
    // sync_equipment() pass. The wire Equipment frame omits empty slots
    // entirely (see read_local_equipment()), so a slot that becomes
    // unequipped simply vanishes from the next frame instead of arriving
    // with an empty itemId — without this, nothing ever detects that and
    // the proxy keeps showing stale equipped state forever. Compared against
    // the new frame's slots each sync to find newly-missing (i.e. just
    // unequipped) slots to explicitly clear, then updated to match.
    uint32_t appliedSlotsMask = 0;

    // Last PlayerLights frame received (flashlight/NVG toggle sync,
    // 2026-08-17), applied in ProxyManager::sync_player_lights.
    // appliedFlashlightOn/appliedNightVisionOn track what's currently
    // applied to the proxy so a resend with the same values is a no-op —
    // same "don't reprocess unchanged data" pattern as appliedAppearanceKey/
    // weaponAttachmentsAppliedKey above.
    bool  flashlightOn         = false;
    bool  nightVisionOn        = false;
    // Ground-truth from FlashlightToggle's own bytecode (2026-08-17): the
    // real per-item "on" brightness, read live off the sender's own
    // Flashlight component — see mod.cpp's read_local_player_lights.
    float flashlightIntensity  = 0.0f;
    bool  lightsDirty          = false;
    bool  appliedFlashlightOn  = false;
    bool  appliedNightVisionOn = false;
};

// Bridge session context.
struct Session {
    bool     ready       = false;
    Uuid16   sessionId   {};
    Uuid16   worldId     {};
    uint64_t connectionId = 0;
    uint64_t playerId     = 0;
    uint64_t entityId     = 0;
};

// A world entity (ground item, zombie, vehicle, building piece, …).
// Descriptor and state arrive in separate frames; actor spawning is deferred
// until hasPosition is set by the first EntityState frame.
struct WorldEntity {
    uint64_t    entityId      = 0;
    EntityKind  kind          = EntityKind::Unknown;
    uint32_t    revision      = 0;
    uint16_t    quantity      = 0;
    uint64_t    ownerPlayerId = 0;
    std::string classPath;
    std::string itemId;          // FName string, e.g. "DA_AK74"
    bool        hasPosition   = false;
    float       x = 0, y = 0, z = 0, yaw = 0;
    float       health        = 0.0f;
    uint8_t     state         = 0;
    void*       actor         = nullptr;
    // Throttles retrying actor resolution in EntityManager::tick() — without
    // this, a failed attempt (e.g. an owned entity whose native pickup can't
    // be found) gets retried every single frame forever, and the owned-entity
    // path does a full FindAllOf UObject scan (2026-08-12: caused 1-2 FPS).
    uint64_t    lastActorAttemptUs = 0;
};

struct BridgeState {
    // Frame counters – incremented atomically by senders.
    std::atomic<uint32_t> seq  {0};
    std::atomic<uint32_t> tick {0};

    // Session context – written once on JoinAccepted, read by all senders.
    std::mutex  sessionMtx;
    Session     session;

    // Latest world-state pushed by the host.
    std::mutex  worldMtx;
    WorldState  worldState;
    bool        worldStateValid = false;

    // Local player lifecycle.
    std::atomic<bool>     hasPawn         {false};
    std::atomic<bool>     sentDeath       {false};
    std::atomic<uint64_t> noPlayerSinceUs {0};
    // 2026-08-16: independent death signal — a live zombie-kill retest
    // proved cached_find_local_pawn() never actually returns null on a real
    // death in this game (the pawn persists, frozen in place), so hasPawn/
    // sentDeath/noPlayerSinceUs above never fire for genuine combat deaths,
    // only for an actual pawn-instance swap (level reload, new character).
    // Tracks the last MedicalComponent.Health<=0 state pushed via
    // DeathRequest/RespawnRequest, kept deliberately separate from the
    // pawn-nullness state machine above rather than merged into it.
    std::atomic<bool>     sentDeathByHealth {false};
    // Timestamp of the moment the local pawn most recently transitioned
    // from invalid to valid (fresh join, respawn, or a level/save reload
    // handing back a new pawn instance). Every nested-ProcessEvent freeze
    // reproduced 2026-08-15/16 happened shortly after exactly this
    // transition, doing heavy component/mesh repair work on a character
    // whose skeletal mesh/material assets may still be mid-stream via the
    // engine's own async loading system — a real hazard already documented
    // (do_game_tick's own comment: a live-captured hang traced a
    // SetSkinnedAssetAndUpdate call blocking on a critical section shared
    // with FAsyncLoadingThread). equipDataReady already gates the local
    // component-drift/attach-health scan on network-replication readiness,
    // but that's a different kind of "ready" than asset-streaming
    // readiness — this timestamp backs an ADDITIONAL grace period for the
    // latter, mirroring RemotePlayer::proxySpawnedAtUs's existing 2s gate
    // for the exact same class of risk on proxies.
    std::atomic<uint64_t> pawnValidSinceUs {0};

    // Remote player map (keyed by playerId).
    std::mutex                               playersMtx;
    std::unordered_map<uint64_t, RemotePlayer> players;

    // World entity map (keyed by entityId).
    std::mutex                               entityMtx;
    std::unordered_map<uint64_t, WorldEntity> entities;

    // Local player progress-revision counter.
    std::mutex inventoryMtx;
    uint32_t   progressRevision = 0;

    // Local player vitals read from game components (not from server).
    LocalVitals localVitals{};

    // 2026-08-15: last-known-GOOD local appearance, cached per-field (only
    // overwritten when a read comes back non-empty, never blanked out by a
    // transient/already-broken read) — used to repair HairMesh/BeardMesh/
    // EyebrowsMesh/Mouth/Hands on the LOCAL pawn, none of which
    // UpdateBodyParts covers. read_local_pawn_appearance() reads the
    // CURRENTLY assigned mesh, which is useless as a repair source once
    // it's already cleared — this cache exists specifically to have
    // something to restore FROM by the time a repair is needed.
    PawnAppearance lastGoodLocalAppearance{};

    // Pending teleport from PlayerProgressRestore; applied on next game tick.
    std::atomic<bool> pendingTeleport{false};
    float teleportX = 0, teleportY = 0, teleportZ = 0, teleportYaw = 0;

    // Pending vitals restore from PlayerProgressRestore (2026-08-14) —
    // deferred the same way pendingTeleport already is, instead of writing
    // raw memory inline in the network-receive handler the instant
    // find_local_pawn() first succeeds after a join. That inline write used
    // find_local_pawn()'s very first successful resolution as its only
    // gate — but this session's own diagnostic logging showed the pawn's
    // own component/attachment count is still mid-initialization at that
    // exact moment (`preChildrenCount=4` vs. a normal ~14), not necessarily
    // fully constructed. Writing raw doubles through component pointers
    // fetched that early, with zero SEH protection, is a plausible
    // contributor to (or outright cause of) the equipment-clearing cascade
    // that reliably follows every join this session — deferred a couple
    // seconds and SEH-wrapped as a safer replacement.
    std::atomic<bool> pendingVitalsRestore{false};
    float vitalsHealth = 0, vitalsHunger = 0, vitalsThirst = 0, vitalsStamina = 0, vitalsRadiation = 0;
    uint64_t vitalsRestoreReadyAtUs = 0;

    // Equip-restore retry (2026-08-14) — root cause confirmed via live IDA
    // reads: BP_JigHelperComp_C::OnLoadDataRequested does a one-shot equip
    // restore from the replicated RepActorsData property at join; if that
    // hasn't finished replicating yet, restore is incomplete, and the
    // OnRep_RepActorsData callback that fires when it does land only
    // broadcasts a delegate (OnEquipmentUpdated) with zero bound listeners
    // (confirmed live — InvocationList count=0) — no retry path exists
    // natively. Also confirmed live the exact failure shape: RepActorsData
    // holds a real Actor* for every slot (replication itself is fine), the
    // break is specifically that actor's RootComponent->AttachParent being
    // null instead of matching every other slot's shared parent.
    //
    // Runs periodically (not just once at join) — see check_equip_restore_
    // retry_trigger — so it self-heals ANY future occurrence of this same
    // failure shape, not only the join-time race. Cheap when healthy (a
    // handful of pointer reads, no-op if every slot's already attached), so
    // an ongoing periodic check costs nothing extra over a one-shot timer.
    uint64_t lastEquipRestoreRetryUs = 0;

    // Join-load-order gate (2026-08-14, root cause session) — confirmed live:
    // every body-part/attachment fall-off cascade this project has ever
    // logged is preceded, within ~1.5s, by join_teleport firing while
    // RepActorsData still reads count=0. component_drift/attach_health
    // sampling during that window see the character's still-loading state
    // (meshes legitimately not yet (re)applied) as "was set, now null" and
    // burn their limited repair attempts fighting a load that was already
    // in progress. False false-positive, not a false-positive-of-baseline —
    // the mesh really is transiently null here, but that's expected, not a
    // bug. Starts false on every fresh pawn sighting (see hasPawn transition
    // in mod.cpp) and flips true the first time do_equip_restore_retry
    // observes a plausible non-zero RepActorsData count; component_drift/
    // attach_health's "local" scan is skipped entirely while false, so their
    // baselines simply aren't sampled during the risky window instead of
    // being sampled and misread.
    std::atomic<bool> equipDataReady{false};

    // New-player detection: time when session was latched.
    std::atomic<uint64_t> sessionLatchUs{0};
    // Set when the server sends PlayerProgressRestore (i.e. returning player).
    std::atomic<bool> receivedProgressRestore{false};
    // Set when char creation request has been written to the flag file.
    std::atomic<bool> ccRequestWritten{false};
    // Set when char creation is complete (done JSON was found and consumed).
    std::atomic<bool> ccDone{false};
    // Character data from creation (protected by sessionMtx).
    std::string ccForename;
    std::string ccSurname;
    std::string ccSex;
    std::string ccAge;
    int         ccOccupation{0};

    // Set (network thread) when the server's MsgType::FirstJoin arrives —
    // authoritative "this playerId has never saved before" signal
    // (gateway.js sends it exactly when db.getProgress() comes back empty).
    // Replaces the old sessionLatchUs/receivedProgressRestore timeout
    // heuristic above (2026-08-17) — that approach, and the SDOnline Lua
    // script it was built to interoperate with, are both retired now that
    // the server can just say so directly. Cleared (game thread) once
    // do_open_barber_menu() has been kicked off for it.
    std::atomic<bool> pendingFirstJoin{false};
    // Game-thread-only (do_game_tick and its callees) — true from the
    // moment a FirstJoin-triggered barber-menu open starts until
    // check_barber_widget_removed_poller sees it close, so that closure
    // triggers a random spawn-point teleport instead of just the normal
    // camera/input restore a manually-triggered/organic barber visit gets.
    bool inFirstJoinFlow = false;
};

inline BridgeState& g_state() {
    static BridgeState s;
    return s;
}

} // namespace sdb
