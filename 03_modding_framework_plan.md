# Modding Framework — Implementation Plan

## Architecture

```
Game (UE5.3)
  └─ UE4SS (loaded via UE4SS-RE)
       ├─ SurrounDeadOnline/    ← Lua mod (UE4SS Lua)
       │    main.lua            ← orchestrator, file/native bridge
       │    world_event_probe   ← world state capture
       │    targeted_world_probe← entity/item probing
       │    visual_capability   ← appearance capture
       │    fixed_day.lua       ← time-of-day lock
       │
       └─ SurrounDeadOnlineCpp/ ← C++ UE4SS mod (DLL)
            entity_manager      ← world entity sync (Phase 1 ✓, Phase 2 TODO)
            proxy_manager       ← remote player proxies (Phase 1 ✓, Phase 2 TODO)
            udp_bridge          ← UDP ↔ runtime relay
            tcp_client          ← direct gateway (fallback)
            protocol            ← binary frame codec (v3 SDO3)
            state               ← BridgeState singleton

Node.js Runtime (local, client-side)
  └─ UDP (port 42100/42101) ↔ TCP (gateway:42200)

Node.js Gateway Server
  └─ SQLite (players.db), auth, session, host-agent

C# WPF Launcher (SDBLauncher.exe)
  └─ Starts game + runtime, manages lifecycle
```

## Protocol Summary

Binary protocol v3, magic `SDO3` (`0x53444F33`).

### Message Types
| Type | Value | Direction | Description |
|------|-------|-----------|-------------|
| `HostAuthenticate` | 1 | host→srv | Host session auth |
| `ClientAuthenticate` | 2 | client→srv | Client join auth |
| `AuthenticationOk` | 3 | srv→client | Auth success |
| `AuthenticationFail` | 4 | srv→client | Auth failure |
| `HostHeartbeat` | 5 | host→srv | Keep-alive |
| `ClientHeartbeat` | 6 | client→srv | Keep-alive |
| `JoinRequest` | 10 | client→srv | Request to join world |
| `JoinAccepted` | 11 | srv→client | Session assigned |
| `JoinRejected` | 12 | srv→client | Rejection reason |
| `PlayerConnected` | 13 | srv→all | New player joined |
| `PlayerDisconnected` | 14 | srv→all | Player left |
| `Movement` | 20 | client→srv→all | Position/rotation/state |
| `Equipment` | 21 | — | Equipment sync |
| `Death` | 22 | srv→all | Player died |
| `Respawn` | 23 | srv→all | Player respawned |
| `ProfileRevision` | 24 | client→srv | Inventory/vitals save |
| `SaveAck` | 25 | srv→client | Save acknowledged |
| `InteractionRequest` | 26 | client→srv | BUILD/LOOT/USE |
| `InteractionResult` | 27 | srv→client | Interaction outcome |
| `DeathRequest` | 28 | client→srv | Client reports death |
| `RespawnRequest` | 29 | client→srv | Client requests respawn |
| `WorldState` | 30 | srv→all | Weather/time-of-day |
| `EntitySpawn` | 31 | srv→all | Loot/building/container spawned |
| `EntityState` | 32 | srv→all | Entity update |
| `EntityDespawn` | 33 | srv→all | Entity removed |
| `ItemDropRequest` | 34 | client→srv | Drop item from slot |
| `ItemPickupRequest` | 35 | client→srv | Pick up entity |
| `ItemPickupResult` | 36 | srv→client | Pickup outcome |
| `ZombieAttackRequest` | 37 | client→srv | Client hit a zombie |
| `ZombieDamageResult` | 38 | srv→client | Zombie damage result |
| `ItemDropResult` | 39 | srv→client | Drop outcome |
| `PlayerDamage` | 40 | srv→client | Health/damage sync |
| `PlayerProgressRestore` | 42 | srv→client | Full inventory/vitals restore |
| `Error` | 255 | any | Error frame |

### Frame Header (88 bytes)
```
magic        uint32  0x53444F33 "SDO3"
version      uint16  3
header_size  uint16  88
type         uint16  MsgType
flags        uint16
connection_id uint64
session_id   [16]byte
world_id     [16]byte
player_id    uint64
entity_id    uint64
sequence     uint32
tick         uint32
timestamp_us uint64
payload_len  uint32
```

## Phase 2 TODO: Actor Spawning

### Problem
`ProxyManager::spawn_proxy()` and `EntityManager::spawn_entity_actor()` are stubs.
They need a `UClass*` to pass to `UWorld::SpawnActor`.

### Solution Path

**Step 1: Class paths (confirmed from pak asset registry)**

These paths are confirmed from `SurrounDead-Windows.pak` directory index + `AssetRegistry.json`:

| Role | Class path |
|------|-----------|
| **Player character** (proxy spawning) | `/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C` |
| Player character child variant | `/Game/Blueprints/BP_PlayerCharacter_Child.BP_PlayerCharacter_Child_C` |
| Master player base | `/Game/Blueprints/Other/More/BP_MasterPlayer.BP_MasterPlayer_C` |
| **Loot item drop** (LOOT_ITEM) | `/Game/Inventory/Items/BP_StaticMeshPickup.BP_StaticMeshPickup_C` |
| Skeletal loot drop (wearables) | `/Game/Inventory/Items/BP_SkeletalMeshPickup.BP_SkeletalMeshPickup_C` |
| **Building piece master** (BUILDING_PIECE) | `/Game/Blueprints/BuildingSystem/Actors/Buildable_MASTER.Buildable_MASTER_C` |
| Modular building master | `/Game/Blueprints/BuildingSystem/Actors/Modular/Buildable_ModularMaster.Buildable_ModularMaster_C` |
| **Container master** (CONTAINER) | `/Game/Blueprints/BuildingSystem/Actors/Containers/BuildableMaster_Container.BuildableMaster_Container_C` |
| Storage crate | `/Game/Blueprints/BuildingSystem/Actors/Containers/Buildable_StorageCrate.Buildable_StorageCrate_C` |

Key components on `BP_PlayerCharacter`:
| Component | Path |
|-----------|------|
| Damage/health | `/Game/Blueprints/Components/DamageComponent.DamageComponent_C` |
| Hunger + thirst | `/Game/Blueprints/Components/HungerThirstComponent.HungerThirstComponent_C` |
| Stamina | `/Game/Blueprints/Components/StaminaComponent.StaminaComponent_C` |
| Building | `/Game/Blueprints/Components/BuildingComponent.BuildingComponent_C` |
| World object | `/Game/Blueprints/Components/WorldObjectComponent.WorldObjectComponent_C` |

**Step 2: Load class by path in C++**

```cpp
// In proxy_manager.cpp
#include <Unreal/UObjectGlobals.hpp>

static RC::Unreal::UClass* s_CharacterClass = nullptr;

void ProxyManager::init() {
    s_CharacterClass = RC::Unreal::UObjectGlobals::StaticFindObject<RC::Unreal::UClass*>(
        nullptr, nullptr, L"/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C"
    );
    initialized_ = true;
}
```

**Step 3: Implement spawn_proxy**

```cpp
RC::Unreal::AActor* ProxyManager::spawn_proxy(
    RC::Unreal::UWorld* world, float x, float y, float z, float yaw)
{
    if (!s_CharacterClass || !world) return nullptr;

    RC::Unreal::FVector loc{x, y, z};
    RC::Unreal::FRotator rot{0.f, yaw, 0.f};
    RC::Unreal::FActorSpawnParameters params{};
    params.SpawnCollisionHandlingOverride =
        RC::Unreal::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    return world->SpawnActor(s_CharacterClass, &loc, &rot, params);
}
```

**Step 4: Implement spawn_entity_actor**

```cpp
// In entity_manager.cpp
static RC::Unreal::UClass* s_StaticPickupClass  = nullptr;
static RC::Unreal::UClass* s_BuildableClass     = nullptr;
static RC::Unreal::UClass* s_ContainerClass     = nullptr;

void EntityManager::init() {
    s_StaticPickupClass = RC::Unreal::UObjectGlobals::StaticFindObject<RC::Unreal::UClass*>(
        nullptr, nullptr, L"/Game/Inventory/Items/BP_StaticMeshPickup.BP_StaticMeshPickup_C");
    s_BuildableClass = RC::Unreal::UObjectGlobals::StaticFindObject<RC::Unreal::UClass*>(
        nullptr, nullptr, L"/Game/Blueprints/BuildingSystem/Actors/Buildable_MASTER.Buildable_MASTER_C");
    s_ContainerClass = RC::Unreal::UObjectGlobals::StaticFindObject<RC::Unreal::UClass*>(
        nullptr, nullptr, L"/Game/Blueprints/BuildingSystem/Actors/Containers/BuildableMaster_Container.BuildableMaster_Container_C");
    initialized_ = true;
}

RC::Unreal::AActor* EntityManager::spawn_entity_actor(
    RC::Unreal::UWorld* world, const WorldEntity& entity)
{
    RC::Unreal::UClass* cls = nullptr;
    switch (entity.entityType) {
        case EntityType::LOOT_ITEM:     cls = s_StaticPickupClass; break;
        case EntityType::BUILDING_PIECE: cls = s_BuildableClass;   break;
        case EntityType::CONTAINER:     cls = s_ContainerClass;    break;
    }
    if (!cls || !world) return nullptr;

    RC::Unreal::FVector loc{entity.x, entity.y, entity.z};
    RC::Unreal::FRotator rot{0.f, entity.yaw, 0.f};
    RC::Unreal::FActorSpawnParameters params{};
    params.SpawnCollisionHandlingOverride =
        RC::Unreal::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    return world->SpawnActor(cls, &loc, &rot, params);
}
```

## Key Hook Points Needed

### Health / Vitals
The `PlayerDamage` (msg 40) and `PlayerProgressRestore` (msg 42) messages already carry health/hunger/thirst. The Lua mod currently captures these via file IPC. The C++ DLL needs to:
1. Hook the native health-change event on the local pawn
2. Send `DeathRequest` (28) when health hits 0
3. Apply `PlayerProgressRestore` when received from server

Search IDA for strings: `"Health"`, `"Hunger"`, `"Thirst"`, `"Stamina"` in the SurrounDead game class range.

### Inventory
`ProfileRevision` (24) syncs the local player's inventory to the server.
`PlayerProgressRestore` (42) restores it from the server.

Need to hook the game's inventory component to intercept slot changes. Find via UE4SS object dump at runtime — look for component classes named `InventoryComponent`, `ItemStorage`, etc.

### World / Time of Day
Already handled: `WorldState` (30) carries `timeOfDay`, `rain`, `snow`, `fog`, `cloudCover`, `wind`, `thunder`. The Lua `fixed_day.lua` script overrides the local time.

## Bridge Modes

| Mode | IPC mechanism | Status |
|------|--------------|--------|
| `legacy-file` | TSV files polled by Lua | Working, slow (~100ms latency) |
| native | C++ DLL direct UDP | Working for movement/entities |

The `bridgeMode` env var in `main.lua` controls which path is used.

## Recommended Next Steps

1. **Run UE4SS object dump** → find actual blueprint class paths for character + item actors
2. **Implement `spawn_proxy`** using found class path
3. **Implement `spawn_entity_actor`** for LOOT_ITEM, BUILDING_PIECE, CONTAINER
4. **Hook health system** → replace file-based vitals polling with C++ hook
5. **Hook inventory** → replace file-based inventory sync with C++ hook
6. **Test proxy teleportation** (Phase 1 already works — verify before Phase 2)
