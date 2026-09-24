# UE5 Key Addresses

All offsets from image base `0x140000000`.

## UWorld

### Known Functions
| Address | Name | Notes |
|---------|------|-------|
| `0x14351D490` | `UWorld::InitializeActorsForPlay` | Confirmed via embedded string |

### Callers of InitializeActorsForPlay
| Address | Role |
|---------|------|
| `0x14323FC00` | Likely map load / world begin play path |
| `0x1434B6E00` | Likely seamless travel / level transition |
| `0x14352B6F0` | Likely PIE or world init variant |

### Data References
| Address | Content |
|---------|---------|
| `0x1473b6ed0` | Data xref to `InitializeActorsForPlay` — probable vtable or function table entry |

### UWorld Offsets Observed (from decompile of `InitializeActorsForPlay`)
```cpp
UWorld* world;  // a1 in decompile
world[6]   → some UObject (likely GameState class ptr, offset 48)
world[41]  → GameState actor                (offset 328)
world[42]  → GameMode actor                 (offset 336)
world[213..216] → cleared on begin play     (offset 1704–1736)
```

Vtable calls observed inside `InitializeActorsForPlay`:
- `(*vtable + 736)` → GameState method at vtable slot 92 (736 / 8)

## SpawnActor

### UFunction Registration
SpawnActor is registered as a UFunction for Blueprint exposure. The registration chain:

```
off_146D47270:
  +0x00: 0x1437bb1a0  ← UFunction constructor for SpawnActor
  +0x10: 0x145eaee8   ← "SpawnActor" FName string

off_145EAB430 (FFunctionParams):
  +0x00: 0x1437bb000  ← OuterFunc (UWorld UClass constructor)
  +0x08: 0x1453f6040  ← SuperFunc
  +0x10: 0x145eab420  ← NameUTF8
  +0x30: 0x0          ← NativeFunc = NULL (vtable dispatch, not UFunction)
```

**NativeFunc is NULL** — `SpawnActor` is called via the C++ vtable, not through UFunction dispatch.
Blueprint calls to SpawnActor go through the K2_SpawnActor Blueprint node, which calls the C++ implementation.

### FName Strings Related to SpawnActor
| Address | String |
|---------|--------|
| `0x145eaaee8` | `SpawnActor` |
| `0x145eaaed0` | (string before SpawnActor in data block) |
| `0x145c64860` | (property-related string) |

Parameter names embedded in FFunctionParams:
- `"DidNotSpawn"` at `+0x40`
- `"ClassToSpawn"` at `+0x50`

### Finding SpawnActor at Runtime
Since NativeFunc is NULL, SpawnActor must be called via vtable. To call from C++:
```cpp
// Via UE4SS RC::Unreal (recommended — no raw pointer needed)
auto* world = RC::Unreal::UObjectGlobals::GetWorld();
auto* actor = world->SpawnActor(InClass, &transform, params);

// Or via UE4SS Lua
UWorld:SpawnActor(class, location, rotation)
```

Do NOT try to call the raw `sub_XXXXXXXX` address — use UE4SS interfaces instead.

## ESpawnActor Enum Strings
| Address | String |
|---------|--------|
| `0x145a16528` | `ESpawnActorCollisionHandlingMethod::Undefined` |
| `0x145a16558` | `ESpawnActorCollisionHandlingMethod::AlwaysSpawn` |
| `0x145a165a0` | `ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn` |
| `0x145a165f0` | `ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding` |
| `0x145a16640` | `ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding` |

## UWorld::InitializeActorsForPlay Key Callees
| Address | Inferred Role |
|---------|--------------|
| `0x14352F1D0` | Called near start of InitializeActorsForPlay — likely BeginPlay dispatch |
| `0x142F7F710` | Called on each actor in `a1[46]` array (Actor's BeginPlay or register) |
| `0x1434AC720` | World type check / GetNetMode equivalent |
| `0x1434EAE20` | String compare (checks "load" for SeamlessTravel detection) |
| `0x142A20FA0` | Called on `a1 + 177` with a2 — likely loading screen or streaming |

## Networking
| Address | String | Notes |
|---------|--------|-------|
| `0x145405fe8` | `GameNetDriver` | Named net driver |
| `0x145405ff8` | `PendingNetDriver` | Pending connection driver |
| `0x145406038` | `DemoNetDriver` | Replay driver |
| `0x145ce8a70` | `NetDriverReplicationSystemConfig` | Iris replication |
| `0x145dc0c78` | `UWorld::InitializeActorsForPlay` | Source file string |
| `0x145e01260` | Source: `IpNetDriver.cpp` | Vanilla IP net driver is compiled in |
| `0x145e11198` | `/Script/OnlineSubsystemSteam` | Steam OSS registered |

## Large Functions (Potential Game Logic)
These are the 25 largest functions — good candidates for gameplay systems.

| Address | Size (bytes) | Notes |
|---------|-------------|-------|
| `0x140E87580` | 143,326 | Likely renderer or large gen'd code |
| `0x1444F61C0` | 112,419 | Unknown — deserves investigation |
| `0x1439DE300` | 106,250 | Unknown |
| `0x140D87020` | 90,144 | Unknown |
| `0x140FA32A0` | 84,716 | Unknown |
| `0x140D9D040` | 76,977 | Unknown |
| `0x14453A2C0` | 60,738 | Unknown |
| `0x144575A10` | 60,107 | Unknown |
| `0x14024BC40` | 58,928 | Pair with `0x140266C70` (same size) |
| `0x140266C70` | 58,928 | Pair — likely generated class pair |
| `0x140281CA0` | 56,477 | Pair with `0x14029C680` |
| `0x14029C680` | 56,477 | Pair — likely generated class pair |

**Pattern**: Paired functions of identical size suggest auto-generated UClass construction code for two similar game classes. The pairs starting at `0x14024BC40`/`0x140266C70` are worth examining — could be the two main character classes.
