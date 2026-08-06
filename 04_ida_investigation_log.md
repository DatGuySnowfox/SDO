# IDA Investigation Log

Session date: 2026-08-05  
Tool: IDA Pro 9.3 + Hex-Rays + IDA MCP server (http://192.168.4.54:8744/mcp)

## Methodology

- IDA MCP server on Docker container, accessed via Bash/curl from WSL
- Binary fully stripped — zero named functions in IDB initially
- Used string xrefs as anchors, then traced callers/callees

---

## String Anchors Found

### SurrounDead-specific
| Address | String |
|---------|--------|
| `0x145e83190` | `/Script/SurrounDead` |
| `0x14685d3f0` | `SurrounDead-Win64-Shipping.pdb` |
| `0x1461b3ec0` | `Z:\SurrounDead 5.3\Plugins\FSR3\...` (build path) |

### UWorld anchors
| Address | String |
|---------|--------|
| `0x145dc0c78` | `UWorld::InitializeActorsForPlay` |

### Network / Online
| Address | String |
|---------|--------|
| `0x145405fe8` | `GameNetDriver` |
| `0x145405ff8` | `PendingNetDriver` |
| `0x145df08b8` | `/Script/OnlineSubsystem` |
| `0x145df0e08` | `OnlineSubsystem` |
| `0x145dfb738` | `/Script/OnlineSubsystemUtils` |
| `0x145e11198` | `/Script/OnlineSubsystemSteam` |
| `0x145e01260` | `IpNetDriver.cpp` source path |

### SpawnActor
| Address | String |
|---------|--------|
| `0x145eaaee8` | `SpawnActor` (FName) |
| `0x145a16528` | `ESpawnActorCollisionHandlingMethod::Undefined` |
| `0x145a16558` | `ESpawnActorCollisionHandlingMethod::AlwaysSpawn` |

---

## Functions Identified

### `sub_14351D490` — `UWorld::InitializeActorsForPlay`
- **Size**: 1717 bytes  
- **Confidence**: High (embeds the `"UWorld::InitializeActorsForPlay"` string)
- **Signature**: `__int64 __fastcall(_QWORD *world, __int64 a2, unsigned __int8 a3, __int64 a4)`
- **Callers**: `sub_14323FC00`, `sub_1434B6E00`, `sub_14352B6F0`
- **Data xref**: `0x1473b6ed0` (probable vtable or function pointer table)
- **Key callees**:
  - `sub_14352F1D0` — BeginPlay dispatch (called on all actors)
  - `sub_142F7F710` — per-actor registration loop  
  - `sub_1434AC720` — world/net mode check

### `sub_1436F89E0` — SurrounDead Module init
- Called from module registration struct at `0x145e83168`
- Calls `sub_140E0CF50`, `sub_140E47D10`, `sub_140E47D40`
- Likely `FSurrounDeadModule::StartupModule()` or class registrar

### `sub_1437BB240` — SpawnActor UFunction constructor (singleton getter)
```c
__int64 sub_1437BB240() {
    if (!qword_147074670)
        sub_140E61240(&qword_147074670, &off_146D47270);
    return qword_147074670;
}
```
- `off_146D47270` → FFunctionParams for SpawnActor UFunction
- `qword_147074670` → cached `UFunction*` pointer (set on first call)

### `sub_1437BB1A0` — SpawnActor UClass constructor (UWorld class)
```c
__int64 sub_1437BB1A0() {
    if (!qword_1470745A8)
        sub_140E60CD0(&qword_1470745A8, &off_145EAB430);
    return qword_1470745A8;
}
```
- `off_145EAB430` → FFunctionParams with parameter names (`ClassToSpawn`, `DidNotSpawn`)
- `qword_1470745A8` → cached `UClass*` for UWorld

---

## Data Structures Found

### SurrounDead Module Registration (`0x145e83150`)
```
0x145e83150: ...
0x145e83158: 0x1000a000000012   (flags)
0x145e83160: 0x145e83018       (pointer)
0x145e83168: 0x1436f89e0       ← module init function
0x145e83170: 0x145e83190       ← "/Script/SurrounDead" string
0x145e83178: 0x0               (null)
0x145e83180: 0x1000000000      (size or count)
...
```

### SpawnActor FFunctionParams (`0x145eab430`)
```
+0x00: 0x1437bb000  OuterFunc (UWorld UClass constructor)
+0x08: 0x1453f6040  SuperFunc
+0x10: 0x145eab420  NameUTF8 pointer
+0x18: 0x145eab300  PropertyArray
+0x20: 0x145eab310  (second property array?)
+0x28: 0x145eab408
+0x30: 0x0          NativeFunc = NULL (vtable dispatch)
+0x38: 0x8800a400018032  packed flags
+0x40: "DidNotSpawn"   (embedded UTF-8 param name)
+0x50: "ClassToSpawn"  (embedded UTF-8 param name)
```

---

## What Was NOT Found

### Game-specific class strings
No strings like `BP_Character`, `BP_Item`, `BP_Pickup`, `BP_PlayerCharacter` etc.
**Reason**: All game blueprint class paths are stored in pak files, not the binary. The binary only contains native C++ class registrations.

### GUObjectArray
Not found as a named string — it's a static global variable without an associated string. Must be found via signature scan or UE4SS SDK dump.

### Direct SpawnActor implementation
NativeFunc is NULL in the UFunction registration. `UWorld::SpawnActor` is called directly via vtable or as a non-virtual C++ function. Neither the vtable offset nor the function address has been confirmed yet.

---

## Recommended Follow-up IDA Work

### Priority 1: Find Game Character Class (for proxy spawning)
**COMPLETED in Session 3**: The native base for `BP_PlayerCharacter_C` is `AArchVisCharacter`
(from `/Script/ArchVisCharacter`) which derives from standard `ACharacter` (`/Script/Engine`).
No custom C++ game character class exists. All game logic is in Blueprint (pak).

### Priority 2: Find Item Actor Class (for entity spawning)
1. Similar approach — look at other paired large functions
2. Search for property structs containing `"ItemId"`, `"Quantity"`, `"SlotIndex"` FNames

### Priority 3: Hook Health/Inventory System
1. Find functions that write to health/hunger/thirst fields
2. Look at callers of `ApplyDamage` (`0x145c1c808`) string xref
3. Trace from `"OnTakeAnyDamage"` string at `0x145afecc8`

### Priority 4: Confirm SpawnActor Address
Use `make_signature_for_function` after decompiling `0x14024BC40` to find:
- Calls into a SpawnActor-shaped function (~3000+ bytes, checks InClass != null)
- Candidate: search for functions with >20 callers in the 0x143xxxx range

---

## IDA MCP Tool Notes

The MCP server is running on `http://192.168.4.54:8744/mcp` (Docker container).

Key tools used:
- `find_regex` — string search (needs `pattern` param, not `regex`)
- `decompile` — Hex-Rays pseudocode (`addr` only, no `max_lines`)
- `func_query` / `list_funcs` / `entity_query` — all require `queries` wrapper
- `xref_query` — requires `queries` wrapper with `addr` singular
- `get_bytes` — requires `regions` array
- `py_eval` — direct Python in IDA context (most flexible)
- `analyze_function` — good comprehensive single-function report

Tool that failed: `survey_binary` — IDA plugin not running in the active IDA instance.

---

## Session 2: 2026-08-05 (continued) — Damage System + Large Function Audit

### Strings Found

| Address | String |
|---------|--------|
| `0x145b07510` | `ReceiveAnyDamage` |
| `0x145b0efd8` | `OnTakeAnyDamage` |
| `0x145c1c808` | `ApplyPointDamage` |
| `0x145c1c858` | `ApplyRadialDamage` |
| `0x145c1c870` | `ApplyRadialDamageWithFalloff` |

Not found as strings (blueprint-only — in pak): `Health`, `Hunger`, `Thirst`,
`CurrentHealth`, `DamageComponent`, `HungerThirst`, `CalculatePlayerDamage`,
`SendHealthToClient`, `Client_UpdateHealthUI`, `TakeDamage`

---

### Damage System Functions

#### `sub_142ABACD0` — `AActor::TakeDamage` (base implementation)
- **Size**: 1655 bytes
- **Code callers**: 1 (`sub_1431E3230`, the game's TakeDamage override calls this as super)
- **Data references**: 162 (vtable entries across all native actor classes — this is the default TakeDamage in all classes that don't override it)
- **Signature**: `__m128(AActor* target, float damage, __int64 damageEvent, __int64 instigatorController, __int64 damageCauser)`
- **Logic**:
  - Checks `damageEvent` type at vtable+16 against value `1` (AnyDamage) or `2` (PointDamage)
  - If AnyDamage: calls `target->vtable[1648/8]()` = `TakeAnyDamage` (slot 206)
  - If PointDamage: calls `target->vtable[1640/8]()` = `TakePointDamage` (slot 205)
  - Broadcasts `OnTakeAnyDamage` via `sub_142A9A300(actor+0x1D8, ...)`
  - Broadcasts `OnTakePointDamage` via `sub_142A9A500(actor+0x1DA, ...)`
  - Notifies instigator at `a4->vtable[2008/8]()` after damage applied

**AActor vtable offsets for damage** (slot = offset/8, vtable start = first vfptr):
| Offset | Slot | Role |
|--------|------|------|
| 1632 | 204 | `TakeDamage` (game override: `sub_1431E3230`; base: `sub_142ABACD0`) |
| 1640 | 205 | `TakePointDamage` |
| 1648 | 206 | `TakeAnyDamage` |
| 1912 | 239 | `CanBeDamaged()` bool check |

**Actor delegate fields** (relative to AActor base):
| Offset | Role |
|--------|------|
| `+0x1D8` (472) | `OnTakeAnyDamage` delegate |

#### `sub_1431E3230` — Game-specific `TakeDamage` override
- **Size**: 142 bytes
- **Data references**: 7 real vtable entries + 1 .pdata false positive (see below)
- **Vtable entries containing this function** (all at slot 204, offset 0x660 from vtable start):

| Vtable Entry | Vtable Start | Class Identified |
|---|---|---|
| `0x145b7a660` | `0x145b7a000` | **Game Character** (ACharacter-derived, C++ base of BP_PlayerCharacter_C) |
| `0x145bf97f8` | `0x145bf9198` | `ADefaultPawn` (sphere mesh root) |
| `0x145d101e8` | `0x145d0fb88` | `APawn` |
| `0x145d85198` | `0x145d84b38` | `ASpectatorPawn` (derived from ADefaultPawn) |
| `0x145ee1618` | `0x145ee0fb8` | **Game AI/Zombie Character** (derived from Game Character, NoCollision capsule) |
| `0x145f477d8` | `0x145f47178` | `AChaosWheeledVehicle` (SkeletalMesh root + ChaosWheeledVehicleMovementComponent) |
| `0x14635ce38` | `0x14635c7d8` | `AArchVisCharacter` (/Script/ArchVisCharacter plugin) |
| ~~`0x147384a70`~~ | ~~N/A~~ | **.pdata exception table** — IDA false positive, not a vtable entry |

- **Signature**: `void(AActor* self, float damage, __int64 damageEvent, __int64 instigator, __int64 causer)`
- **Logic**:
  - Calls `self->vtable[1912/8]()` — `CanBeDamaged()` check
  - Calls `sub_142ABACD0(self, damage, event, instigator, causer)`
  - If damage applied and instigator changed: stores instigator at `self[89]` = offset `0x2C8`
- **Actor offsets extracted**:
  - `actor + 0x2C8` = last damage instigator controller
  - `actor + 0x2D0` = previous instigator (comparison target)

#### `sub_142A9A300` — `OnTakeAnyDamage` delegate broadcaster
- **Size**: 511 bytes
- **Called from**: `sub_142ABACD0`
- **Signature**: `void(_BYTE* actorDelegateField, __int64 actor, float damage, __int64 instigator, __int64 instigatorController, __int64 causer)`
- Called as: `sub_142A9A300(actor + 472, actor, damage, ...)`
- Lazy-initializes FName "OnTakeAnyDamage" via `sub_140C827F0(&qword_14702DED8, "OnTakeAnyDamage", 1)`
- Looks up delegate at `actor - 472` (the actor base, then finds delegate by FName)
- Broadcasts via `sub_140E38610`

#### `sub_142A98E40` — `ReceiveAnyDamage` UFunction singleton getter
- **Pattern**: Same lazy-init pattern as SpawnActor UFunction getter
- **Cached UFunction\***: `qword_14702DCF8`
- **FFunctionParams**: `off_146CD4590`
- **UFunction name string**: `"ReceiveAnyDamage"` at `0x145b07510`
  - Embedded in FPropertyParams struct at `0x145b07500` with flags `0x0005_0000_1000_000a`, size 0x45
  - Next param at `0x145b080d0`

---

### Large Function Audit — CORRECTION

Prior hypothesis that `0x14024BC40`/`0x140266C70` (58,928 bytes each) were UClass
constructors was **wrong**. Decompile reveals they are **Chaos physics ISPC code**:
```
Runtime/Experimental/Chaos/Private/Chaos/PerParticlePBDCollisionConstraint.ispc
```
Strings: TaperedCylinder, Sphere geometry assertions. Callees: `puts`, `abort` only.
These are NOT game character classes.

---

### Hook Strategy for C++ DLL

For monitoring player damage in Phase 2:

**Option A — Hook `sub_142ABACD0` (ApplyDamage)**
- Hook at `image_base + 0x2ABACD0 - 0x140000000`
- Intercept ALL damage events for any actor
- Filter by checking if `a1` is the local player pawn

**Option B — Hook via vtable patching on the player pawn**
- After `StaticFindObject` returns the character class
- Patch vtable slot 204 (`TakeDamage`), 205 (`TakePointDamage`), or 206 (`TakeAnyDamage`)
- Only fires for that specific actor instance

**Option C — Use Lua RegisterHook (no IDA needed)**
- `RegisterHook(".../BP_PlayerCharacter_C:ReceiveAnyDamage", fn)`
- Already identified as working in the Lua probe scripts
- Recommended for Phase 2 — least invasive

**Recommended approach**: Option C (Lua) for health monitoring, Option A (C++ hook
at `sub_142ABACD0`) if sub-millisecond latency matters for death detection.

---

### Follow-up Work Still Needed

1. ~~Find the 8 actor classes~~ — **DONE in Session 3** (see below)

2. **Find UClass* for BP_PlayerCharacter at runtime** — already in plan via
   `StaticFindObject`. IDA search not needed; confirm with UE4SS runtime dump.

3. **Inventory / item pickup offsets** — no native strings found. All inventory
   logic is in pak (JigSaw system). Must hook via Blueprint (`RegisterHook` Lua)
   or via UE4SS C++ `Process Event` hook on the component class.

---

## Session 3: 2026-08-05 (continued) — Vtable Class Identification

### Overview

Identified all 7 real vtable entries for `sub_1431E3230` (TakeDamage override).
`0x147384a70` was a **.pdata exception table** entry — IDA data xref false positive.

All actor-derived classes in this binary have `TakeDamage` at vtable **slot 204**
(offset 0x660 = 1632 bytes from vtable start). Prior "slot 205" analysis was wrong
because the game character's vtable was misidentified as starting at `0x145b79ff8`
(an async movement struct vtable). The true start is `0x145b7a000`.

---

### Game Character Class (C++ base of BP_PlayerCharacter_C)

**Constructor**: `sub_142C58A20` (1295 bytes)
- Calls `sub_1431A9E60` (APawn constructor)
- Sets vtable to `0x145b7a000`, secondary vtable at `obj+0x298` (664) = `0x145b7a998`
- Creates `UCapsuleComponent` at `obj+0x330` (816) — set as RootComponent at `obj+0x1A0`
- Creates `UCharacterMovementComponent` at `obj+0x328` (808)
- Creates `USkeletalMeshComponent` at `obj+0x320` (800)
- Initializes FName `"Characters"` / SpriteCategory `"Characters"`

**Key field offsets** (relative to actor object base):
| Offset | Field |
|--------|-------|
| `0x000` | Primary vtable pointer (`0x145b7a000`) |
| `0x298` (664) | Secondary vtable pointer (`0x145b7a998`) |
| `0x1A0` (416) | `RootComponent` (= CapsuleComponent) |
| `0x320` (800) | `USkeletalMeshComponent* Mesh` |
| `0x328` (808) | `UCharacterMovementComponent* CharacterMovement` |
| `0x330` (816) | `UCapsuleComponent* CapsuleComponent` |
| `0x2C8` (712) | Last damage instigator controller (from TakeDamage override) |
| `0x2D0` (720) | Previous instigator (comparison target) |
| `0x1D8` (472) | `OnTakeAnyDamage` multicast delegate |

**Important**: The field offsets above are shared with all base classes up the hierarchy
(APawn → AActor → UObject), so `0x1D8` for `OnTakeAnyDamage` is an AActor field.

---

### Game AI/Zombie Character Class

**Constructor**: `sub_1437F8AF0`
- Calls `sub_142C58A20` (Game Character constructor) then overrides vtable to `0x145ee0fb8`
- Sets `"NoCollision"` on CapsuleComponent (typical for AI pawns)
- Initializes additional fields at offsets 1656–1808 (AI state data)
- This is the C++ base class for zombie/AI Blueprint characters

---

### APawn Hierarchy

| Class | Constructor | Vtable Start |
|-------|------------|-------------|
| `APawn` | `sub_1431A9E60` | `0x145d0fb88` |
| `ADefaultPawn` | `sub_142E09020` | `0x145bf9198` |
| `ASpectatorPawn` | `sub_143406630` | `0x145d84b38` |

`ASpectatorPawn` constructor calls `sub_142E09020` (DefaultPawn base) then overrides
vtable. FName `"Spectator"` initialized in SpectatorPawn constructor via
`sub_140C82960(&qword_147065CC0, L"Spectator", 1)`.

---

### Vehicle and Plugin Classes

| Class | Vtable Start | Notes |
|-------|-------------|-------|
| `AChaosWheeledVehicle` | `0x145f47178` | APawn-derived, USkeletalMeshComponent root + UChaosWheeledVehicleMovementComponent |
| `AArchVisCharacter` | `0x14635c7d8` | From `/Script/ArchVisCharacter` plugin |

---

### UClass Registrations Confirmed

| Class | Package | Size (bytes) | Vtable Start | UClass* global |
|-------|---------|-------------|-------------|----------------|
| `ACharacter` | `/Script/Engine` | 1664 (0x680) | `0x145b7a000` | `qword_1470329E0` |
| `AEQSTestingPawn` | `/Script/AIModule` | 1824 (0x720) | `0x145ee0fb8` | `qword_147075EE8` |
| `AArchVisCharacter` | `/Script/ArchVisCharacter` | 1760 (0x6E0) | `0x14635c7d8` | `qword_14710B408` |

Class hierarchy (UClass registrations show SuperFunc pointers):
```
APawn
├── ADefaultPawn → ASpectatorPawn
├── ACharacter (1664 bytes)
│   ├── AEQSTestingPawn (1824 bytes) — AI EQS testing pawn with NoCollision
│   └── AArchVisCharacter (1760 bytes) — archviz FPS character
│       └── BP_PlayerCharacter_C (Blueprint — in pak)
└── AWheeledVehiclePawn/AChaosVehicle (1824 bytes)
```

The game has **no custom C++ character class**. `BP_PlayerCharacter_C` derives from
`AArchVisCharacter` which derives from the standard UE5 `ACharacter`. All game logic
is in Blueprint (pak file).

### AArchVisCharacter Field Layout (extension above ACharacter base)

Fields at offsets 1656–1759 relative to object base (overlap zone at 1656-1663 is
last aligned-padding bytes of ACharacter; own fields start at 1664):

| Offset | Field | Initialized with |
|--------|-------|-----------------|
| 1664 (0x680) | FString `LookUpAxisName` | "LookUp" |
| 1680 (0x690) | FString `LookUpRateAxisName` | "LookUpRate" |
| 1696 (0x6A0) | FString `TurnAxisName` | "Turn" |
| 1712 (0x6B0) | FString `TurnRateAxisName` | "TurnRate" |
| 1728 (0x6C0) | FString `MoveForwardAxisName` | "MoveForward" |
| 1744 (0x6D0) | FString `MoveRightAxisName` | "MoveRight" |
| 1752 (0x6D8) | `float BaseTurnRate` | ≈ 0.02 (1020054733) |
| 1756 (0x6DC) | `float BaseLookUpRate` | ≈ 0.02 (1020054733) |

---

### Vtable Layout Clarification

The address `0x145b79ff8` (23 code xrefs) is NOT the game character vtable — it belongs
to a separate non-actor struct (FCharacterMovementComponentAsyncInput or similar)
that happens to be laid out immediately before the game character vtable in `.rdata`.
The true game character vtable starts at `0x145b7a000`.

Conclusion: ALL AActor-derived classes in this binary have TakeDamage at **slot 204**
(vtable byte offset 1632 = 0x660). ACharacter-derived classes do not add virtual
functions before TakeDamage relative to APawn.

---

## Session 4: 2026-08-05 (continued) — Online/Network System + Global Variables

### Overview

Investigated the online/networking layer. Key findings:
- The game uses the commercial **SteamCore** plugin (third-party UE5 plugin wrapping Steam SDK)
- Session management is via SteamCore's high-level async action nodes
- Lower-level Steam Matchmaking API is also exposed via Blueprint library
- Steam authentication (tickets) is used to verify joining players
- GEngine global address confirmed at `0x147068258`
- GUObjectArray struct region identified (starts ~`0x146EFDE40`)

---

### Steam API Interfaces

The game uses the following Steam SDK interface versions (confirmed via
`SteamInternal_FindOrCreateUserInterface` calls):

| Interface | Version String | Getter Function |
|-----------|---------------|----------------|
| ISteamMatchmaking | `SteamMatchMaking009` | `sub_1436569D0` at `0x1436569D0` |
| ISteamUser | `SteamUser021` | `sub_143656A90` at `0x143656A90` |
| ISteamFriends | `SteamFriends017` | `sub_143656AF0` at `0x143656AF0` |
| ISteamUtils | `SteamUtils010` | `sub_143656A00` at `0x143656A00` |
| ISteamNetworking | `SteamNetworking006` | (nearby in same table) |
| ISteamGameServer | `SteamGameServer014` | (via different path) |
| ISteamNetworkingSockets | `SteamNetworkingSockets012` | `0x14617ceb0` string |

All getters are stored in a function pointer table at `0x146d2de88` (±0x100 bytes).
The SteamMatchmaking getter is specifically at `0x146d2dee8` in that table.

---

### SteamCore Plugin — Session Management

The game uses SteamCore's `UAsyncAction`-based session nodes:

| UFunction Name | UClass* Getter | Registration Data |
|----------------|---------------|-------------------|
| `CreateSteamCoreSession` | `sub_143682BF0` | `off_145E3DA30` |
| `FindSteamCoreSessions` | `sub_143682C70` | (registration table) |
| `DestroySteamCoreSession` | `sub_143682C30` | (registration table) |
| `UpdateSteamCoreSession` | `sub_143682D70` | (registration table) |
| `JoinLobbyAsync` | `sub_143682CF0` | (registration table) |

Function registration table (getter fn ↔ name string pairs) at `0x146d35fd0`–`0x146d363xx`.

These are `UBlueprintAsyncActionBase`-derived classes — they wrap async Steam lobby calls
and fire delegate pins when complete.

---

### SteamCore Plugin — Direct Matchmaking API

The plugin also exposes direct ISteamMatchmaking functions as Blueprint library calls:

**UFunction exec thunk table** (`fn_getter → UFunction name`) at `0x145e3eb00`:

| Address | Function Name |
|---------|--------------|
| `0x143681E40` | `AddRequestLobbyListCompatibleMembersFilter` |
| `0x143681E70` | `AddRequestLobbyListDistanceFilter` |
| `0x143681EA0` | `AddRequestLobbyListFilterSlotsAvailable` |
| `0x143681ED0` | `AddRequestLobbyListNearValueFilter` |
| `0x143681F00` | `AddRequestLobbyListNumericalFilter` |
| `0x143681F30` | `AddRequestLobbyListResultCountFilter` |
| `0x143681F60` | `CreateLobby` |
| `0x143681F90` | `DeleteLobbyData` |
| `0x143682000` | `GetFavoriteGame` |
| `0x143682030` | `GetFavoriteGameCount` |
| `0x143682060` | `GetLobbyByIndex` |
| `0x143682090` | `GetLobbyChatEntry` |
| `0x1436820C0` | `GetLobbyData` |
| `0x1436820F0` | `GetLobbyDataByIndex` |
| `0x143682120` | `GetLobbyDataCount` |
| `0x143682150` | `GetLobbyGameServer` |
| `0x143682180` | `GetLobbyMemberByIndex` |
| `0x1436821B0` | `GetLobbyMemberData` |
| `0x1436821E0` | `GetLobbyMemberLimit` |
| `0x143682210` | `GetLobbyOwner` |
| `0x143682240` | `GetNumLobbyMembers` |
| `0x143682300` | `InviteUserToLobby` |
| `0x143682330` | `JoinLobby` |
| `0x143682390` | `LeaveLobby` |
| `0x1436823C0` | `RemoveFavoriteGame` |
| `0x1436823F0` | `RequestLobbyData` |
| `0x143682420` | `RequestLobbyList` |
| `0x143682450` | `SendLobbyChatMsg` |
| `0x143682480` | `SetLinkedLobby` |
| `0x1436824B0` | `SetLobbyData` |
| `0x143682510` | `SetLobbyGameServer` |

**Lobby callback structs** registered nearby (at `0x145e3ed40`+):
- `FavoritesListAccountsUpdated`, `LobbyChatMsg`, `LobbyChatUpdate`, `LobbyDataUpdate`,
  `LobbyEnter`, `LobbyGameCreated`, `LobbyKicked`

**ESteamLobbyType enum** (at `0x145e457e0`+):
- `Private`, `FriendsOnly`, `Public`, `Invisible`

---

### Session Struct Fields

Fields found near `SteamIDHost` in the session data structures:

| Field Name | Context |
|-----------|---------|
| `SteamIDLobby` | Lobby identifier (uint64, CSteamID format) |
| `SteamIDHost` | Host's Steam ID |
| `PlayerMin` | Minimum player count |
| `PlayerMax` | Maximum player count |
| `MaxTeamSize` | Team size limit |
| `ConnectionDetails` | Server connection info |
| `NumConnectionDetails` | Count of connection endpoints |

**Lobby search filter fields** (used in `FindSteamCoreSessions` / `RequestLobbyList`):
- `LobbyDistanceFilter`, `SlotsAvailable`, `KeyToMatch`, `ValueToBeCloseTo`,
  `ValueToMatch`, `ComparisonType`, `MaxResults`, `LobbyType`, `MaxMembers`

---

### Steam Authentication

The game uses Steam auth tickets for player verification:

| Function | Role |
|---------|------|
| `GetAuthSessionTicket` | Client generates ticket before joining |
| `BeginAuthSession` | Host validates incoming player's ticket |
| `EndAuthSession` | Host invalidates ticket on disconnect |
| `GetServerSteamID` | Get server's Steam ID for routing |
| `GetServerPublicIP` | Server's public IP address |
| `GetServerSteamID_Pure` | Pure (const) version |

These strings are at `0x145e2c658`–`0x145e2c768`.

**Server-side coordination functions** (at `0x145e2dbe8`+):
- `HostConfirmGameStart` — host signals game is starting
- `SearchForGameWithLobby` — finds game session via lobby ID
- `SetGameHostParams` — configures game host parameters

---

### GEngine Global

| Global | Address | Confirmed by |
|--------|---------|-------------|
| `GEngine` | `0x147068258` | Written at `0x1435ae590` in init fn `sub_1435AE370`; read at `0x1435ae6f7` before `GEngine->Init()` call (vtable offset `0x2D8`) |

The GEngine init function (`sub_1435AE370`) at `0x1435ae4dc` loads class string `"GameEngine"`,
creates the engine object via `sub_140E72AD0` (NewObject-equivalent), then writes to
`qword_147068258`.

---

### GUObjectArray Region

The `UObjectBaseInit` function (`sub_140E48940` at `0x140E48940`) initializes the
global `FUObjectArray`:

| Global | Address | Role |
|--------|---------|------|
| `dword_146EFDE48` | `0x146EFDE48` | `MaxObjectsNotConsideredByGC` |
| `qword_146EFDE50` | `0x146EFDE50` | Permanent object pool base |
| `qword_146EFDE58` | `0x146EFDE58` | Permanent object pool cursor |
| `qword_146EFDE60` | `0x146EFDE60` | Permanent object pool end |
| `dword_146EFE160` | `0x146EFE160` | `ObjFirstGCIndex` (= `ObjLastNonGCIndex`) |
| `dword_146EFE168` | `0x146EFE168` | `MaxObjectsNotConsideredByGC` mirror |
| `qword_146EFE170` | `0x146EFE170` | `FChunkedFixedUObjectArray::Objects` (chunk ptr array) |
| `qword_146EFE178` | `0x146EFE178` | Pre-allocated object pool |
| `dword_146EFE180` | `0x146EFE180` | `MaxElements` (= NumChunks × 65536) |
| `qword_146EFE184` | `0x146EFE184` | Packed: `[NumChunks, ...]` |
| `dword_146EFE18C` | `0x146EFE18C` | `NumChunks` |

The `FChunkedFixedUObjectArray` struct effectively begins at `0x146EFE160`.
`GUObjectArray` as a whole begins at approximately `0x146EFDE40`.

> **Note**: UE4SS locates GUObjectArray at runtime via signature scanning; the above
> static addresses are for reference only (IDB has all globals as `0xFFFFFFFFFFFFFFFF`
> since they are uninitialized in static analysis).

---

### Network Architecture Summary (for SD-Online)

```
SurrounDead multiplayer stack:
  UE5 Engine
    └── Online Subsystem Steam (/Script/OnlineSubsystemSteam)
          └── SteamCore plugin (/Script/SteamCore)
                ├── CreateSteamCoreSession → ISteamMatchmaking::CreateLobby
                ├── FindSteamCoreSessions  → ISteamMatchmaking::RequestLobbyList
                ├── JoinSteamCoreSession   → ISteamMatchmaking::JoinLobby
                └── Auth: GetAuthSessionTicket / BeginAuthSession / EndAuthSession
  Transport: IP/UDP (IpNetDriver, WS2_32 socket calls present in import table)
  P2P Relay: SteamNetworkingSockets012 available (optional relay path)
```

**SD-Online implication**: The game's session system is fully exposed via Blueprint
through the SteamCore plugin. SD-Online can hook the SteamCore session functions
via Lua `RegisterHook` on `CreateSteamCoreSession` / `FindSteamCoreSessions` to
intercept or extend the session management layer without touching native code.

Alternatively, the game can be made to join a custom session by calling
`JoinLobby` with a known SteamIDLobby — the SteamCore plugin will handle the
rest of the connection flow.

---

### SurrounDead Native C++ Classes (`/Script/SurrounDead`)

The native module exposes only **three utility classes** (all 40 bytes, CLASS_Native):

| Class | UClass* global | Constructor | Role |
|-------|---------------|-------------|------|
| `UCPlusPlusFunctionLibrary` | `qword_147072D70` | `sub_1436F8310` | Blueprint Function Library |
| `UDistanceSort` | `qword_147072DB0` | `sub_1436F8520` | Sort helper |
| `UGetStreamedLevel` | `qword_147072DD8` | `sub_1436F88A0` | Streaming level query |

**`UCPlusPlusFunctionLibrary`** Blueprint-callable functions (at `0x145e82c00`):
- `IsStreamingTextures` — checks if textures are streaming (getter: `sub_1436F8450`)
- `Distance_Sort(Array_To_Sort, From_Actor, Descending) → Sorted_Array` — sorts actors by distance
- `Actor_Dist` — calculates actor-to-actor distance

**`UGetStreamedLevel`** functions:
- `GetActorStreamingLevelName` / `GetStreamedLevel` — returns streaming level name for an actor

**Conclusion**: All game logic (health, inventory, damage, AI, crafting, etc.) is in
Blueprint (pak). The `/Script/SurrounDead` C++ module is a thin utility layer only.

---

### ECloudStorageDelegate Enum

The `CSD_*` strings are **not** SurrounDead-specific — they belong to `ECloudStorageDelegate`
from UE5's built-in cloud storage subsystem (`/Script/Engine`). The game uses UE5's
`UCloudStorageBase` for cloud saves (player progress, settings):

| Value | Meaning |
|-------|---------|
| `CSD_KeyValueReadComplete` | Cloud key-value read finished |
| `CSD_KeyValueWriteComplete` | Cloud key-value write finished |
| `CSD_ValueChanged` | Cloud value changed (sync notification) |
| `CSD_DocumentQueryComplete` | Document query finished |
| `CSD_DocumentReadComplete` | Document read finished |
| `CSD_DocumentWriteComplete` | Document write finished |
| `CSD_DocumentConflictDetected` | Conflict in document sync |

Field `LocalCloudFiles` at `0x1459f7e58` = local cache of cloud files.

---

### Priorities Updated

- **COMPLETED**: Full vtable class identification (Session 3)
- **COMPLETED**: Steam online subsystem architecture (Session 4)
- **COMPLETED**: GEngine global address confirmed: `0x147068258`
- **COMPLETED**: SurrounDead native C++ class inventory
- **NEXT**: Find player health/hunger/thirst field offsets in BP_PlayerCharacter_C
  (needs FModel pak export + UE4SS runtime dump)
- **NEXT**: Document GUObjectArray exact start for C++ DLL `ForEachUObject` equivalent
- **NEXT**: Confirm SpawnActor C++ address via callgraph from `UWorld::SpawnActor`
