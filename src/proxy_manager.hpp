#pragma once
#include "state.hpp"
#include <string>

namespace RC::Unreal {
    class AActor;
    class UWorld;
    class UClass;
    class UObject;
}

namespace sdb {

// Looks up a live UJigsawItem_DataAsset_C* by its itemId (FName string),
// shared between equipment sync and world-entity spawning. Returns nullptr
// if no matching DataAsset is loaded. See proxy_manager.cpp for the cache.
void* resolve_item_asset(const std::string& itemId);

// Looks up a live UAnimMontage* by its short object name (e.g.
// "AM_Melee_Knife_1"), same name-keyed FindAllOf("AnimMontage") pattern as
// resolve_item_asset. Returns nullptr if no matching montage is loaded.
void* resolve_montage_asset(const std::string& montageName);

// Diagnostic: dumps every row name in the named UDataTable (e.g. L"DT_Clothing")
// plus every loaded clothing-capable item's raw ItemId, flagging ItemIds that
// don't exactly match a row name (research/04_ida_investigation_log.md
// Session 50 found "Makeshift"-prefixed crafted items never match their
// plain-named table row). Output goes to debug_log only; fixes nothing.
void dump_clothing_table(const wchar_t* tableName);

// Resolves a live UClass* by name — pass the class's full package path
// (e.g. "/Game/AI/Zombies/Roamer/BP_Zombie_Roamer.BP_Zombie_Roamer_C") for
// the primary, no-live-instance-needed lookup; falls back to resolving off
// a live instance of the class if that fails. See proxy_manager.cpp for the
// full rationale — live-verified, used successfully throughout this project
// (zombie archetypes, native vehicle/pickup resolution, and more).
RC::Unreal::UClass* resolve_class_by_name(const std::wstring& fullPathOrShortName);

// Spawns actorClass at the given world position/yaw via the same
// BeginDeferredActorSpawnFromClass/FinishSpawning native-call pattern used
// for proxy actors (UE4SS's own SpawnActor wrapper is broken on this build —
// see proxy_manager.cpp). Returns nullptr on failure.
RC::Unreal::AActor* spawn_actor_at(RC::Unreal::UWorld* world, RC::Unreal::UClass* actorClass,
                                    float x, float y, float z, float yaw);

// 2026-08-15: generic single-component mesh-apply, factored out of
// ProxyManager::sync_pawn_appearance's per-part logic so mod.cpp can reuse
// the exact same resolve-by-short-name + Set call for LOCAL pawn repair
// (HairMesh/BeardMesh/EyebrowsMesh/Mouth/Hands — none of which
// UpdateBodyParts covers). `component` is the target UObject* directly
// (caller already resolved it, e.g. via GetValuePtrByPropertyNameInChain);
// `isSkeletal` selects SetSkinnedAssetAndUpdate(bReinitPose=false) vs
// SetStaticMesh. Returns false if the named mesh can't be resolved or the
// component has neither setter. `leaderMesh` (only used when isSkeletal) —
// the character's own Mesh component — re-establishes the leader-pose
// bone mapping after the swap; see refresh_leader_pose's comment
// (proxy_manager.cpp) for why this is required, not optional, for any
// skeletal body-part component. Pass nullptr only for non-skeletal
// (StaticMesh) calls, where it's unused.
bool reapply_named_mesh(RC::Unreal::UObject* component, const std::string& meshShortName, bool isSkeletal,
                         RC::Unreal::UObject* leaderMesh = nullptr);

// Re-establishes a leader-pose follower's bone mapping to the character's
// main Mesh component — call immediately after any direct
// SetSkinnedAssetAndUpdate on a body-part/Clothing_X component. See its own
// comment (proxy_manager.cpp) for the full rationale.
void refresh_leader_pose(RC::Unreal::UObject* followerComp, RC::Unreal::UObject* leaderMesh);

// ProxyManager spawns and drives remote-player proxy actors in the UE5 world.
//
// Position tracking + actor teleport via K2_SetActorLocationAndRotation, and
// actor spawning via UWorld::SpawnActor. No dedicated proxy Blueprint exists
// in the game (research/04_ida_investigation_log.md Session 12) — spawns
// BP_PlayerCharacter_C itself, which gets the right mesh/animations for
// free; since nothing possesses it with a PlayerController, it doesn't
// process input and stays wherever teleport_proxy() puts it.
class ProxyManager {
public:
    ProxyManager() = default;
    ~ProxyManager() = default;

    void init();

    void on_player_connected(uint64_t playerId);
    void on_player_disconnected(uint64_t playerId);
    void on_movement(uint64_t playerId, const Movement& m);
    void on_equipment(uint64_t playerId, const Equipment& e);
    void on_weapon_attachments(uint64_t playerId, const WeaponAttachments& a);
    void on_pawn_appearance(uint64_t playerId, const PawnAppearance& a);
    void on_play_montage(uint64_t playerId, const std::string& montageName, float playRate);
    void on_player_lights(uint64_t playerId, const PlayerLights& l);
    void on_weapon_fired(uint64_t playerId);

    // Per-frame update – world and local_pawn may be null (proxies are skipped).
    // allowDirtyStateSync gates the equipment/weapon-attachment/appearance
    // sync block specifically (SetSkinnedAssetAndUpdate/SetLeaderPoseComponent/
    // clothing OnRep_*-class ProcessEvent calls — the ones confirmed present
    // in every live-captured GameThread freeze this session, see the
    // "Reliable GameThread Trigger via WndProc Subclass" plan). Teleport/
    // velocity/rotation always run regardless — those aren't part of any
    // captured freeze and don't need gating. False when called from the
    // existing on_actor_tick/on_process_event_pre cadence (which is
    // sometimes nested inside another ProcessEvent dispatch); true only from
    // the WndProc-triggered clean-context tick in mod.cpp.
    void tick(RC::Unreal::UWorld* world, RC::Unreal::AActor* local_pawn, bool allowDirtyStateSync);

    // 2026-08-15: forces player.appearance to be fully reapplied on the next
    // tick, regardless of whether player.appliedAppearanceKey already
    // matches — used when a proxy's hair/beard/eyebrows/mouth/body-part
    // mesh is detected cleared for a reason unrelated to a real appearance
    // change (see mod.cpp's component_drift repair path), so the existing,
    // already-correct sync_pawn_appearance logic re-runs instead of
    // silently no-op'ing on an unchanged key. No-op if proxyActor doesn't
    // match any known player.
    bool force_resync_appearance(RC::Unreal::AActor* proxyActor);

private:
    // 2026-08-16 audit: SEH trampoline for tick()'s whole per-proxy sync
    // body (teleport/rotation/velocity + equipment/weapon/appearance dirty
    // dispatch below) — all of it dereferences player.proxyActor, which can
    // go stale (world destroyed, actor pointer not yet reset) in the same
    // window that produced the confirmed do_game_tick crash. A `static`
    // member has unrestricted access to private members via the
    // ProxyManager* it's handed, while still being a plain void(*)(void*)
    // function pointer — required for MSVC's __try/__except (see
    // proxy_manager.cpp's seh_invoke comment for why a capturing
    // lambda/std::function can't be used here). See proxy_manager.cpp for
    // the context struct and the real per-player body.
    static void do_proxy_per_player_tick(void* ctxRaw);

    void teleport_proxy(RC::Unreal::AActor* actor,
                        float x, float y, float z, float yaw);

    // Returns new AActor* or nullptr on failure.
    RC::Unreal::AActor* spawn_proxy(RC::Unreal::UWorld* world,
                                    float x, float y, float z, float yaw);

    void destroy_proxy(RC::Unreal::AActor* actor);

    // Pushes player.equipment onto the proxy actor via
    // BP_JigHelperComp_C::SetEquippedInfoBySlot. See proxy_manager.cpp for
    // the verification gate (kEnableEquipmentWrite) — read
    // research/04_ida_investigation_log.md Session 43/44 before flipping it.
    void sync_equipment(RC::Unreal::AActor* actor, RemotePlayer& player);

    // Applies player.weaponAttachments onto whichever of the 4 weapon
    // visual actors (already spawned by sync_equipment) currently exist —
    // spawns/attaches the attachment's own Local_ActorClass actor at its
    // Local_AttachSocket on the *weapon's* own mesh (not the character's).
    void sync_weapon_attachments(RemotePlayer& player);

    // Applies player.flashlightOn/nightVisionOn onto the proxy's own
    // character-level toggles (BP_PlayerCharacter_C::FlashlightToggle/
    // NightVisionOn) — distinct from sync_weapon_attachments' per-attachment
    // active state (a weapon-mounted light/laser), this is the character's
    // own handheld flashlight and NVG goggles.
    void sync_player_lights(RC::Unreal::AActor* actor, RemotePlayer& player);

    // Applies player.appearance (gender/hair/beard) directly onto the proxy
    // actor's own HairMesh/BeardMesh components and color materials.
    void sync_pawn_appearance(RC::Unreal::AActor* actor, RemotePlayer& player);

    // Moves whichever weapon slot's visual actor matches
    // player.activeWeaponSlot onto its item's EquipSocket (in-hand), and
    // reverts the previously-active one back to its holster socket — the
    // actor-spawn/holster placement sync_equipment already does has no
    // concept of "currently drawn", only "currently equipped".
    void sync_active_weapon_hand(RC::Unreal::AActor* actor, RemotePlayer& player);

    bool initialized_ = false;
};

inline ProxyManager& g_proxy_manager() {
    static ProxyManager pm;
    return pm;
}

} // namespace sdb
