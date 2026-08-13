#pragma once
#include "state.hpp"
#include <string>

namespace RC::Unreal {
    class AActor;
    class UWorld;
    class UClass;
}

namespace sdb {

// Looks up a live UJigsawItem_DataAsset_C* by its itemId (FName string),
// shared between equipment sync and world-entity spawning. Returns nullptr
// if no matching DataAsset is loaded. See proxy_manager.cpp for the cache.
void* resolve_item_asset(const std::string& itemId);

// Diagnostic: dumps every row name in the named UDataTable (e.g. L"DT_Clothing")
// plus every loaded clothing-capable item's raw ItemId, flagging ItemIds that
// don't exactly match a row name (research/04_ida_investigation_log.md
// Session 50 found "Makeshift"-prefixed crafted items never match their
// plain-named table row). Output goes to debug_log only; fixes nothing.
void dump_clothing_table(const wchar_t* tableName);

// Spawns actorClass at the given world position/yaw via the same
// BeginDeferredActorSpawnFromClass/FinishSpawning native-call pattern used
// for proxy actors (UE4SS's own SpawnActor wrapper is broken on this build —
// see proxy_manager.cpp). Returns nullptr on failure.
RC::Unreal::AActor* spawn_actor_at(RC::Unreal::UWorld* world, RC::Unreal::UClass* actorClass,
                                    float x, float y, float z, float yaw);

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

    // Per-frame update – world and local_pawn may be null (proxies are skipped).
    void tick(RC::Unreal::UWorld* world, RC::Unreal::AActor* local_pawn);

private:
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
