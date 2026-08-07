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
- **COMPLETED**: UWorld::SpawnActor C++ address confirmed (Session 5)
- **NEXT**: Find player health/hunger/thirst field offsets in BP_PlayerCharacter_C
  (needs FModel pak export + UE4SS runtime dump)
- **NEXT**: Find UGameplayStatics::GetPlayerController / GetPlayerPawn addresses
- **NEXT**: Document GUObjectArray exact start for C++ DLL `ForEachUObject` equivalent

---

## Session 5: 2026-08-05 (continued) — SpawnActor Confirmation + Actor Placement Functions

### UWorld::SpawnActor — CONFIRMED

**Address**: `sub_14300FE50` = `0x14300FE50` (RVA: `0x300FE50`)

**Confirmation method**: Decompile revealed embedded string:
```
L"Cannot generate unique name for '%s' in level '%s'."
```
This string is unique to `UWorld::SpawnActor` in UE5 source.

**Signature**: `__int64 __fastcall(UWorld* world, UClass* a2, FTransform* a3, FActorSpawnParameters* a4)`
- `a1` = UWorld pointer
- `a2` = UClass to spawn (checked for RF_Abstract flag at a2+212 bit 25)
- `a3` = FTransform pointer (NULL = default transform)
- `a4` = FActorSpawnParameters struct

**Size**: 701 decompiled lines — very large function (~4KB+ of machine code)

**Call chain confirmed**:
```
sub_142EB0790  (BeginSpawningActorFromBlueprint exec thunk)
  → sub_142E80E80  (UGameplayStatics::BeginSpawningActorFromBlueprint)
      → sub_1434ACF40(GEngine, world_context, 1)  ← GetWorldFromContextObject
      → sub_14300FE50(world, class, transform, params)  ← UWorld::SpawnActor
```

**Key callees within SpawnActor**:

| Callee | Address | Role |
|--------|---------|------|
| `sub_142AAFB10` | `0x142AAFB10` | CDO/class validation (get default object) |
| `sub_142AAEA90` | `0x142AAEA90` | Probably `UClass::GetDefaultObject` |
| `sub_1430077E0` | `0x1430077E0` | Name generation (`MakeUniqueObjectName`) |
| `sub_140E661C0` | `0x140E661C0` | Level notification (pre-spawn) |
| `sub_140DDB430` | `0x140DDB430` | Actor class validity check |
| `sub_142FFF900` | `0x142FFF900` | `StaticConstructObject_Internal` (allocation) |
| `sub_140E6FAC0` | `0x140E6FAC0` | Get allocated object pointer |
| `sub_142D2F650` | `0x142D2F650` | Actor initialization (pre-component) |
| `sub_142AB4AB0` | `0x142AB4AB0` | `PostSpawnInitialize` (transform, owner, instigator) |
| `sub_140DDB470` | `0x140DDB470` | `InitializeComponents` |
| `sub_140B39480(a1+920, v143)` | — | `OnActorSpawned` delegate broadcast |
| `sub_14350DC80(a1, v143)` | `0x14350DC80` | Final world notification |

**FActorSpawnParameters layout** (a4 offsets, inferred from decompile):
| Offset | Field | Notes |
|--------|-------|-------|
| `+0x00` | Reserved / Name | default = None |
| `+0x08` | Template | Actor to use as template |
| `+0x10` | Owner | Owning actor |
| `+0x18` | Instigator | Pawn instigator |
| `+0x20` | OverrideLevel | Force spawn into specific level |
| `+0x30` | bNoFail | bool |
| `+0x31` | SpawnCollisionHandlingOverride | ESpawnActorCollisionHandlingMethod |
| `+0x32` | TransformScaleMethod | ETeleportType |
| `+0x33` | PackedFlags | bAllowDuringConstructionScript (bit 0), bDeferConstruction (bit 1), bTemporaryEditorActor (bit 2), bHideFromSceneOutliner (bit 3) |

**Validation checks** (reasons SpawnActor returns NULL):
1. `a2 == NULL` (no class provided)
2. `a2` has `RF_Abstract` flag (offset+212, bit 25)
3. `a2` has `CLASS_NotPlaceable` flag (offset+212, bit 0)
4. Class CDO validation fails
5. World is `None` type or sealed
6. Unique name generation fails
7. `StaticConstructObject_Internal` returns NULL

---

### GetWorldFromContextObject — `sub_1434ACF40`

**Address**: `0x1434ACF40`  
**Signature**: `UWorld* __fastcall(UEngine* engine, UObject* worldContextObject, int WorldType)`
- Called as `sub_1434ACF40(qword_147068258, a1, 1)`
- `a1` here is the WorldContext object (e.g. player controller or game instance)
- Returns the UWorld pointer for the given context

This is the standard `GEngine->GetWorldFromContextObject(...)` which walks the outer chain to find the owning UWorld.

---

### StaticConstructObject_Internal — `sub_142FFF900`

**Address**: `0x142FFF900`  
Called from SpawnActor to allocate the new actor object. This is `StaticConstructObject_Internal` — the lowest-level UObject allocation primitive.

---

### UGameplayStatics::GetPlayerController — `sub_142E92450`

**Address**: `0x142E92450`  
**Exec thunk**: `sub_142EB6E90` at `0x142EB6E90` (in UGameplayStatics Blueprint library table at `0x145c359e8`)  
**Signature**: `APlayerController* __fastcall(UObject* worldContextObject, int playerIndex)`

**Algorithm**:
1. `sub_1434ACF40(GEngine, worldContextObject, 1)` → UWorld
2. `UWorld + 440` = `AGameStateBase*` GameState
3. Walk `GameState + 56` array (TArray<APlayerState*>::Data) with count at `GameState + 64`
4. Each `PlayerState + 0x140` = `APlayerController*` owning controller (confirmed Session 7/8)
5. Return the controller at index == `playerIndex`

**Key field confirmed**: `UWorld + 440` = `AGameStateBase* GameState`  
**Correction**: Earlier notes stated "PlayerState + 48 = owning controller" — the correct offset is **+0x140 (320)**, confirmed by decompile of `sub_1432819C0`.

---

### K2_SetActorLocationAndRotation — exec thunk `sub_142AC5500`

**Address**: `0x142AC5500` (exec thunk in AActor Blueprint library table at `0x145b142a8`)

**Algorithm**:
1. Reads Blueprint params: NewLocation (FVector3d), bSweep (bool), HitResult (FHitResult), bTeleport (bool)  
2. `actor + 416` = RootComponent (USceneComponent*)
3. Reads current world-space location from `RootComponent + 608` and rotation from `RootComponent + 576`–`+607` (FQuat4d)
4. Dispatches via vtable: `(*RootComponent->vtable[1312/8])(RootComponent, delta, newRot, bSweep, hitResult, 0, !bTeleport)`

**USceneComponent field offsets** (all double-precision in UE5):
| Offset | Field | Size |
|--------|-------|------|
| `+0x240` (576) | `ComponentToWorld.Rotation` (FQuat4d: X,Y,Z,W) | 32 bytes |
| `+0x260` (608) | `ComponentToWorld.Translation.X` | 8 bytes |
| `+0x268` (616) | `ComponentToWorld.Translation.Y` | 8 bytes |
| `+0x270` (624) | `ComponentToWorld.Translation.Z` | 8 bytes |
| `+0x278` (632) | `ComponentToWorld.Scale3D.X` | 8 bytes |

**USceneComponent vtable offsets** (byte offsets from vtable base):
| Byte Offset | Function |
|-------------|---------|
| `1200` (0x4B0) | `GetComponentLocation()` → reads Translation directly |
| `1312` (0x520) | `MoveComponentImpl(delta, newRot, bSweep, hitResult, flags, teleportType)` |

**Fast C++ DLL position read** (no vtable overhead):
```cpp
// actor = AActor*
USceneComponent* root = *(USceneComponent**)(actor + 416);  // RootComponent
double X = *(double*)(root + 608);
double Y = *(double*)(root + 616);
double Z = *(double*)(root + 624);
```

---

### K2_GetActorLocation — exec thunk `sub_142AC5100`

**Address**: `0x142AC5100` (exec thunk, AActor table at `0x145b14258`)

**Algorithm**:
1. Reads 3 boolean/flag params from Blueprint stack
2. `actor + 416` = RootComponent
3. Calls `(*RootComponent->vtable[1200/8])(RootComponent, outputBuffer)` → FVector3d

---

### Confirmed Field Offsets Summary

**AActor (applies to all actor-derived classes)**:
| Offset | Field |
|--------|-------|
| `0x000` | Primary vtable pointer |
| `0x1A0` (416) | `RootComponent` (USceneComponent*) |
| `0x1D8` (472) | `OnTakeAnyDamage` multicast delegate |
| `0x2C8` (712) | Last damage instigator controller |
| `0x2D0` (720) | Previous instigator |

**ACharacter (extends AActor)**:
| Offset | Field |
|--------|-------|
| `0x298` (664) | Secondary vtable pointer |
| `0x320` (800) | `USkeletalMeshComponent* Mesh` |
| `0x328` (808) | `UCharacterMovementComponent* CharacterMovement` |
| `0x330` (816) | `UCapsuleComponent* CapsuleComponent` |

**AArchVisCharacter (C++ base of BP_PlayerCharacter_C, extends ACharacter)**:
| Offset | Field |
|--------|-------|
| `0x680` (1664) | `FString LookUpAxisName` ("LookUp") |
| `0x690` (1680) | `FString LookUpRateAxisName` ("LookUpRate") |
| `0x6A0` (1696) | `FString TurnAxisName` ("Turn") |
| `0x6B0` (1712) | `FString TurnRateAxisName` ("TurnRate") |
| `0x6C0` (1728) | `FString MoveForwardAxisName` ("MoveForward") |
| `0x6D0` (1744) | `FString MoveRightAxisName` ("MoveRight") |
| `0x6D8` (1752) | `float BaseTurnRate` |
| `0x6DC` (1756) | `float BaseLookUpRate` |
| `0x6E0` (1760) | **BP_PlayerCharacter_C Blueprint fields begin here** |

**USceneComponent** (used via RootComponent):
| Offset | Field |
|--------|-------|
| `0x240` (576) | `ComponentToWorld.Rotation` (FQuat4d, 32 bytes) |
| `0x260` (608) | `ComponentToWorld.Translation` (FVector3d: X,Y,Z as doubles) |
| `0x278` (632) | `ComponentToWorld.Scale3D` (FVector3d) |

**APlayerState**:
| Offset | Field | Type |
|--------|-------|------|
| `+0x140` (320) | `OwningController` | APlayerController* |
| `+0x298` (664) | `Score` | float |
| `+0x29C` (668) | `PlayerId` | int32 |
| `+0x2A0` (672) | `CompressedPing` | uint8 |
| `+0x2A4` (676) | `StartTime` | float |
| `+0x2A2` (674) | Name override flag | uint8 (MSB set → vtable GetPlayerName) |
| `+0x2A8` (680) | `EngineMessageClass` | TSubclassOf (8 bytes) |
| `+0x2B0` (688) | `ExactPing` | float (raw C++ field, not UPROPERTY) |
| `+0x2B8` (696) | `SavedNetworkAddress` | FString |
| `+0x330` (816) | `PlayerNamePrivate.Data` | TCHAR* |
| `+0x338` (824) | `PlayerNamePrivate.ArrayNum` | int32 |
| `+0x33C` (828) | `PlayerNamePrivate.ArrayMax` | int32 |

**APlayerController**:
| Offset | Field | Type |
|--------|-------|------|
| `+0x2A0` (672) | `PlayerState` | APlayerState* |
| `+0x2D8` (728) | `Pawn` | APawn* |
| `+0x520` (1312) | `NetConnection` | UNetConnection* |

**AGameStateBase**:
| Offset | Field | Type |
|--------|-------|------|
| `+56` (0x38) | `PlayerArray.Data` | APlayerState** |
| `+64` (0x40) | `PlayerArray.Num` | int32 |

**UObject** (base of all UE objects):
| Offset | Field | Type |
|--------|-------|------|
| `+0x08` | `ObjectFlags` | EObjectFlags |
| `+0x0C` | `InternalIndex` | int32 (GUObjectArray index) |
| `+0x10` | `ClassPrivate` | UClass* |
| `+0x18` | `NamePrivate` | FName (8 bytes: ComparisonIdx u32 + Number u32) |
| `+0x20` | `OuterPrivate` | UObject* |

**UWorld** (extends UObject):
| Offset | Field | Type |
|--------|-------|------|
| `+0x1B8` (440) | `GameState` | AGameStateBase* |
| `+0x158` (344) | Probable `GameInstance` | UGameInstance* |
| `+0x6A8` (1704) | `TimeSeconds` | double |
| `+0x6B0` (1712) | `UnpausedTimeSeconds` | double |
| `+0x6B8` (1720) | `RealTimeSeconds` | double |

**AGameStateBase** (extends AInfo → AActor):
| Offset | Field | Type |
|--------|-------|------|
| `+0x38` (56) | `PlayerArray.Data` | APlayerState** |
| `+0x40` (64) | `PlayerArray.Num` | int32 |
| `+0x218` (536) | `ReplicatedWorldTimeSeconds` | float |

---

## Session 6: 2026-08-05 (continued) — Plugin Discovery: EasyMultiSave + TechTree

### Overview

Discovered two commercial Marketplace plugins providing critical game systems:
- **EasyMultiSave** (EMS) — `/Script/EasyMultiSave` — full save game system
- **TechTree** — `/Script/TechTree` — technology research system

These were found via targeted string search in the `0x145f1xxxx` binary region.

---

### EasyMultiSave Plugin

**Package**: `/Script/EasyMultiSave` at `0x145f216f0`  
**Source path**: `D:\build\U5M-Marketplace\...` (Marketplace build)

EasyMultiSave is a UE Marketplace plugin providing multi-slot save game management. The game uses it for **all player state persistence**.

**EMS save data structure** (what gets saved per player):

| Field | Meaning |
|-------|---------|
| `SavedPawn` | Player's pawn (character) actor |
| `SavedController` | Player controller |
| `SavedPlayerState` | APlayerState data |
| `SavedGameMode` | AGameMode state |
| `SavedGameState` | AGameState data |
| `PlayerStackData` | Player inventory/item stack |
| `PlayerPositionArchive` | Player world position |
| `PlayerStackArchive` | Full inventory serialization |
| `ActorList` / `ActorMap` | All actors in level |
| `LevelArchiveList` | Per-level save data |
| `MultiLevelStreamData` | Streaming level data |
| `DestroyedActors` | Actors that were destroyed |
| `RawObjectData` | Custom raw save objects |
| `WorldPartitionActors` | World partition actor data |

**EMS Functions exposed to Blueprint**:

| Function | Purpose |
|---------|---------|
| `AsyncLoadActors` | Async actor restore |
| `AsyncSaveActors` | Async actor save |
| `AutoLoadLevelActors` | Auto-restore level actors |
| `AutoSaveLevelActors` | Auto-save level actors |
| `LoadPlayerActorsCustom` | Load player-specific actors |
| `SavePlayerActorsCustom` | Save player-specific actors |
| `LoadRawObject` / `SaveRawObject` | Raw data save/load |
| `SaveCustom` / `GetCustomSave` | Custom save data |
| `SetCurrentSaveGameName` | Select save slot |
| `SetCurrentSaveUserName` | Set player user name |
| `SetActorSaveProperties` | Per-actor save config |
| `DeleteAllSaveDataForSlot` | Wipe a save slot |
| `DeleteCustomPlayerFile` | Delete player-specific file |
| `DoesSaveSlotExist` | Check if slot has data |
| `GetAllSaveUsers` | Get all player user names |
| `GetNamedSlotInfo` / `GetSlotInfoSaveGame` | Slot metadata |
| `GetSortedSaveSlots` | Sorted slot list |
| `IsSavingOrLoading` | Save operation in progress |
| `AsyncWaitForOperation` | Async wait helper |
| `ClearMultiLevelSave` | Clear multi-level save |
| `ExportSaveThumbnail` / `ImportSaveThumbnail` | Thumbnail management |

**EMS Enums**:

| Enum | Values |
|------|--------|
| `EDataLoadType` | `DATA_Level`, `DATA_Player`, `DATA_Object` |
| `EActorType` | `AT_Runtime`, `AT_Placed`, `AT_LevelScript`, `AT_PlayerActor` |
| `EUpdateActorResult` | `RES_Success`, `RES_Skip`, `RES_ShouldSpawnNewActor` |
| `ENextStepType` | `SaveLevel`, `FinishSave` |
| `ESaveGameMode` | `MODE_Player`, `MODE_Level`, `MODE_All` |

**EMS Configuration Properties**:

| Property | Type | Purpose |
|----------|------|---------|
| `bPersistentPlayer` | bool | Player data persists between sessions |
| `bPersistentGameMode` | bool | Game mode persists |
| `bAutoSaveStructs` | bool | Auto-save struct data |
| `bAutoDestroyActors` | bool | Destroy actors on load |
| `bAdvancedSpawnCheck` | bool | Advanced spawn handling |
| `bSkipSave` | bool | Exclude actor from save |
| `bSkipTransform` | bool | Skip actor transform in save |
| `bPersistent` | bool | Actor persists |
| `bMultiThreadSaving` | bool | Multi-thread save operations |
| `FileSaveMethod` / `LoadMethod` | enum | Serialization format |
| `DeferredLoadStackSize` | int | Deferred load batch size |
| `SaveGameVersion` | string | Version string for migration |
| `DefaultSaveGameName` | FString | Default save slot name |
| `SlotInfoSaveGameClass` | UClass | Save class for slot info |

**EMS Delegates** (hook points for SD-Online):

| Delegate | Fires When |
|----------|-----------|
| `OnPlayerLoaded` | Player save data loaded → SD-Online sync point |
| `OnPartitionLoaded` | World partition loaded |
| `AsyncLoadOutputPin` | Async load succeeded |
| `AsyncLoadFailedPin` | Async load failed |
| `AsyncSaveOutputPin` | Async save succeeded |
| `AsyncSaveFailedPin` | Async save failed |
| `EmsLoadPlayerComplete` | Player load complete |
| `EmsLoadLevelComplete` | Level load complete |
| `EmsLoadPartitionComplete` | Partition load complete |

**SD-Online Implications**:
1. `OnPlayerLoaded` fires after EMS restores player save data — **this is when SD-Online should receive the player's server-synced position**
2. Proxy actors can be configured with `bSkipSave=true` to prevent EMS from saving them
3. `bSkipTransform=true` on proxy actors prevents EMS from overwriting their network-synced positions
4. `SetActorSaveProperties` allows dynamic configuration of which actors are saved
5. `EMSObject` singleton is the main handle for all EMS operations

---

### TechTree Plugin

**Package**: `/Script/TechTree` at `0x145f50c20`  
**Source path**: `D:\build\U5M-Marketplace\...\Plugins\TechTree\Source\...`

A Marketplace plugin providing a technology research/progression tree.

**Key classes/functions**:

| Name | Purpose |
|------|---------|
| `AssignTechTree` | Assign a tech tree to a player |
| `GetAssignedTechTree` | Get current tech tree |
| `GetSavegameTechObject` | Get tech tree save data |
| `GetTechTreeWidget` | Get the UI widget |
| `SelectTechTree` | Select active tech tree |
| `TechTreeTemplate` | Template asset |
| `AssignedTechTrees` (TMap) | Player → tech tree assignments |

**Delegates**:
- `OnTechTreeAssigned` — fires when tech tree assigned to player
- `OnTechTreeRecovered` — fires when tech tree loaded from save

**`ESaveGameMode` enum** (for TechTree save integration):
- `MODE_Player` — save player-specific tech progress
- `MODE_Level` — save per-level data
- `MODE_All` — save everything

The TechTree integrates with EasyMultiSave via `GetSavegameTechObject`.

**SD-Online implication**: Tech tree progression is player-specific save data. SD-Online does not need to sync this between players — each player has their own progression.

---

### AGameMode — Blueprint Lifecycle Hooks

**Exec thunk table** at `0x145c21d50` (UFunction singleton getters):

| Function | Singleton Getter | Role |
|---------|----------------|------|
| `FindPlayerStart` | `sub_142E71700` | Find spawn point |
| `GetDefaultPawnClassForController` | `sub_142E717C0` | Get pawn class |
| `GetNumPlayers` | `sub_142E718B0` | Count players |
| `GetNumSpectators` | `sub_142E719C0` | Count spectators |
| `HandleStartingNewPlayer` | `sub_142E71AE0` | New player setup |
| `HasMatchEnded` | `sub_142E71B70` | Match over check |
| `HasMatchStarted` | `sub_142E71C00` | Match started check |
| `InitializeHUDForPlayer` | `sub_142E71C90` | Init HUD |
| `K2_FindPlayerStart` | `sub_142E71D50` | BP: find spawn |
| `K2_OnChangeName` | `sub_142E71F50` | BP: player renamed |
| `K2_OnLogout` | `sub_142E72010` | BP: player left |
| `K2_OnRestartPlayer` | `sub_142E72070` | BP: player respawn |
| `K2_OnSwapPlayerControllers` | `sub_142E720F0` | BP: controller swap |
| `K2_PostLogin` | `sub_142E72120` | BP: player joined |
| `MustSpectate` | `sub_142E721B0` | Force spectator |
| `PlayerCanRestart` | `sub_142E721E0` | Check can respawn |
| `RestartPlayer` | `sub_142E722A0` | Restart player |
| `RestartPlayerAtTransform` | `sub_142E72360` | Restart at position |
| `SpawnDefaultPawnAtTransform` | `sub_142E724C0` | Spawn pawn at pos |
| `SpawnDefaultPawnFor` | `sub_142E724F0` | Spawn default pawn |
| `StartPlay` | `sub_142E72520` | Match start |

**Key hooks for SD-Online**:
- `K2_PostLogin` — player controller pointer available here
- `K2_OnLogout` — player has left, clean up proxies
- `K2_OnRestartPlayer` — player respawned (position reset)
- `SpawnDefaultPawnFor` — game is spawning a new pawn for a player

These are all hookable via UE4SS `RegisterHook` on the game's `BP_GameMode_C` Blueprint class.

---

### Plugins Summary

| Plugin | Package | Role | SD-Online Impact |
|--------|---------|------|----------------|
| SteamCore | `/Script/SteamCore` | Steam sessions | Hooks for session join/create |
| EasyMultiSave | `/Script/EasyMultiSave` | Save/load all state | `OnPlayerLoaded` = sync point |
| TechTree | `/Script/TechTree` | Research progression | Per-player, no sync needed |
| ArchVisCharacter | `/Script/ArchVisCharacter` | BP_PlayerCharacter base | C++ character class |

---

## Session 7: 2026-08-05 (continued) — APlayerState Field Offsets

### Overview

Resolved the GetPlayerName / GetPlayerId table ambiguity. Confirmed `PlayerNamePrivate`
FString offset directly from decompilation of the implementation function `sub_14325DBB0`.

---

### Exec Thunk Table Format Clarification

The exec thunk table entries are **(name_ptr, fn_ptr)** pairs — name first, function second.
This was previously misread as (fn_ptr, name_ptr).

Corrected table at `0x145d35a30` (APlayerState functions):

| Table Address | name_ptr | fn_ptr | Function |
|---------------|----------|--------|---------|
| `0x145d35a30` | `0x145d2ac58` ("GetPlayerId") | `0x14253e610` | GetPlayerId exec thunk |
| `0x145d35a40` | `0x1459d0158` ("GetPlayerName") | `0x143281a30` | GetPlayerName exec thunk |
| `0x145d35a50` | `0x145d2ac68` ("GetScore") | *(next entry)* | GetScore |

---

### APlayerState — GetPlayerName Implementation

**Implementation**: `sub_14325DBB0` at `0x14325DBB0`  
**Exec thunk**: `sub_143281A30` at `0x143281A30`

Confirmed from disassembly of `sub_14325DBB0`:
```asm
cmp [rcx+2A2h], al      ; flag byte at APlayerState+0x2A2
jge short .direct_read  ; if >= 0: read PlayerNamePrivate directly
call [rax+768h]         ; else: vtable slot 237 (subclass override)

.direct_read:
mov rsi, [rcx+330h]     ; PlayerNamePrivate.Data  (TCHAR*)
movsxd rdi, [rcx+338h]  ; PlayerNamePrivate.ArrayNum
```

---

### APlayerState — GetPlayerId Implementation

**Exec thunk**: `sub_14253E610` at `0x14253E610`

```c
__int64 __fastcall sub_14253E610(__int64 a1, __int64 a2, _DWORD *a3)
{
    *(_QWORD *)(a2 + 32) += *(_QWORD *)(a2 + 32) != 0;  // advance bytecode ptr
    result = *(unsigned int *)(a1 + 0x29C);  // PlayerId int32
    *a3 = result;
    return result;
}
```

`PlayerId` (int32) is at `APlayerState + 0x29C` (668 decimal).

---

### APlayerState — Confirmed Field Offsets

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x29C` (668) | `PlayerId` | int32 | Unique player session ID |
| `+0x2A2` (674) | Override flag byte | uint8 | If MSB set → vtable call for GetPlayerName |
| `+0x330` (816) | `PlayerNamePrivate.Data` | TCHAR* | FString data pointer |
| `+0x338` (824) | `PlayerNamePrivate.ArrayNum` | int32 | Character count (including null) |
| `+0x33C` (828) | `PlayerNamePrivate.ArrayMax` | int32 | Allocated capacity |

**`PlayerNamePrivate` FString starts at `APlayerState + 0x330`.**

**Vtable override**: `GetPlayerName()` uses vtable slot 237 (byte offset 0x768 = 1896)
when the flag at `+0x2A2` has its MSB set. Otherwise reads `PlayerNamePrivate` directly.

---

### Fast C++ DLL Player Name Read

```cpp
// playerState = APlayerState* (from APlayerController->PlayerState)
const wchar_t* nameData = *(const wchar_t**)(playerState + 0x330);
int32_t nameLen = *(int32_t*)(playerState + 0x338);
// nameData is a null-terminated wchar_t string of length nameLen-1
std::wstring playerName(nameData, nameLen > 0 ? nameLen - 1 : 0);
```

---

### APlayerController — PlayerState Pointer

From the `GetPlayerController` algorithm documented in Session 5:
- `GameState + 56` = `PlayerArray.Data` (TArray of APlayerState*)
- `PlayerState + 48` = `APlayerController*` owning controller

This means: `APlayerState + 0x30` (48) = `APlayerController* OwningController`

And to get PlayerState from PlayerController, we have the reverse confirmed:

**APlayerController + 0x2A0** (672) = `APlayerState* PlayerState`  
**APlayerController + 0x2D8** (728) = `APawn* Pawn`

Confirmed from `FObjectPropertyParams` metadata:
- `0x145bdb6e0` → PlayerState property, encoded offset `0x02A0`
- `0x145bdb7e0` → Pawn property, encoded offset `0x02D8` (cross-validated by K2_GetPawn exec thunk reading `a1 + 728`)

For APlayerState → APlayerController (reverse): the exec thunk `sub_1432819C0` reads
`*(QWORD*)(a1 + 320)` where a1 is an APlayerState. This is likely `OwningController`
at `APlayerState + 0x140` (320). (**Note**: Session 5 incorrectly stated "+48 = owning
controller" — the correct value is +320 from the decompile.)

---

### Confirmed APlayerController Field Offsets

| Offset | Field | Type |
|--------|-------|------|
| `+0x2A0` (672) | `PlayerState` | APlayerState* |
| `+0x2D8` (728) | `Pawn` | APawn* |

**Key addresses**:
- K2_GetPawn exec thunk: `sub_1429C6760` at `0x1429C6760`
- APlayerController exec thunk table: `0x145be5de0`–`0x145be5f18`
- PlayerState property metadata: `0x145bdb6e0`
- Pawn property metadata: `0x145bdb7e0`

---

### Pending After Session 7

- **Player health/hunger/thirst** — Blueprint fields in pak, not findable in IDA
- **AIOptimizer plugin** — investigated in Session 8 (see below)
- **AGameState/UWorld map name offset** — still pending
- **GUObjectArray** exact struct start — documented at ~`0x146EFDE40` (Session 4)

---

## Session 8: 2026-08-06 — AIOptimizer Plugin (Critical Multiplayer System)

### Overview

AIOptimizer is a **custom SurrounDead-specific** proximity-based AI actor lifecycle
subsystem. **All zombies are managed by this system.** This is the single most
impactful system for SD-Online that was previously unknown.

**Package**: `/Script/AIOptimizer` at `0x145efedd0`  
**Package descriptor**: `0x145efedb0`  
**Package singleton getter**: `0x1438635F0`

---

### Registered Classes (6 total)

| Class | Role | Size |
|-------|------|------|
| `AIOptimizerSubsystem` | Central world subsystem (singleton) | 208 bytes |
| `AIOSubjectComponent` | Attached to zombie actors | — |
| `AIOInvokerComponent` | Attached to player actors | — |
| `AIOSpawnPoint` | Spawn point actor/component | — |
| `DebugAIOptimizer` (x2) | Debug widget overlay, two variants | — |
| `AIOptimizationLayer` | Per-distance-band configuration | — |

---

### AIOptimizerSubsystem — Native Functions (19 total)

`StaticRegisterNativesUAIOptimizerSubsystem` at `0x143869F50`  
Native function table at `0x145eff0e0`

| Address | Function |
|---------|---------|
| `0x14386B340` | `DebugAIOptimizer` |
| `0x14386B510` | `GetCategorizedDebugSubjects` |
| `0x14386B680` | `GetClosestInvokerLocation` |
| `0x14386B750` | `GetDebugSubjects` |
| `0x14386B900` | `GetDistanceToClosestInvokerSquared` |
| `0x14386B9A0` | `GetInvokerIndex` |
| `0x14386BCD0` | `GetSubjectIndex` |
| `0x14386BEC0` | `K2_DespawnSubject` |
| `0x14386C320` | `K2_DespawnSubjectByHandle` |
| `0x14386C4A0` | `K2_SpawnSubjectByHandle` |
| `0x14386C580` | `LoopPendingSubjects` |
| `0x14386C5C0` | `LoopSubjects` |
| `0x14386C5E0` | `RegisterInvoker` |
| `0x14386C720` | `RegisterSubject` |
| `0x14386C990` | `RemoveDespawnedSubjectByHandle` |
| `0x14386CED0` | `SetIsSystemEnabled` |
| `0x14386D1E0` | `ShrinkArrays` |
| `0x14386D260` | `UnregisterInvoker` |
| `0x14386D400` | `UnregisterSubject` |

---

### AIOSubjectComponent — Key Methods

Native table at `0x145efa908`:
- `AddUniqueHandle` / `FindHandle` / `RemoveHandle` / `IsHandleValid`
- `GetInvokerTag` / `GetSubjectTag`
- `SetAILogicEnabled` / `SetCharacterMovementEnabled` / `SetCharacterFeatures`
- `SetSpawner` / `SetCanBeUpdatedBySubsystem`
- `CanBeUpdatedBySubsystem` / `ShouldBeDespawned` / `IsDespawning` / `IsSeenByAnyInvoker`
- `GetCurrentOptimizationLayer` / `GetOptimizationLayerForCurrentDistance`
- `GetDistanceToClosestInvoker` / `GetDistanceToClosestInvokerSquared`
- `GetSpawnRadiusSquared` / `GetDespawnRadiusSquared`
- `ReinitializeOptimizationLayers`
- **Delegates**: `OnOptimizationUpdate`, `OnPreDespawn`, `OnPostSpawned`

---

### Enumerations

| Enum | Values |
|------|--------|
| `EAIOStartSpawningMethod` | `None`, `SpawnOnGameStart`, `SpawnOnRadius`, `SpawnOnRegion` |
| `EAIOSelectSpawnPointsMethod` | `UseRandomPoints`, `UseSpecifiedSpawnPoints` |
| `EAIORespawnMethod` | `Undefined`, `AllAtOnce`, `EachIndividually` |
| `EAIOFeaturesFlags` (bitmask) | `AIBrain`(1), `MovementComponent`(2), `Visibility`(4), `Collision`(8), `Animations`(16), `ActorTick`(32), `Shadows`(64) |
| `EAIODebugGroup` | `Undefined`, `Spawned`, `Despawned`, `PendingSpawn`, `PendingDespawn`, `SpawnedClose`, `SpawnedMedium`, `SpawnedFar`, `NotUpdated` |
| `EDespawnMethod` | `UseQueue`, `Immediately` |

---

### AIOptimizerSubsystem — Configuration Fields

| Field | Purpose |
|-------|---------|
| `bIsSubsystemEnabled` | Master enable gate |
| `bDisplayDebugInfo` | Debug display |
| `OptimizationUpdateInterval` | Timer period for update loop |
| `HandleSpawnDespawnMethod` | EDespawnMethod (queued vs immediate) |
| `SpawnCapacityPerUpdate` | Throttle: max spawns per timer tick |
| `SpawnInterval` / `DespawnRadius` | Distance thresholds |
| `PeripheralVisionHalfAngleDegrees` | Cone angle for IsSeenByAnyInvoker |
| `SpawnedSubjects` / `DespawnedSubjects` | Live subject arrays (stride 144 bytes) |
| `Invokers` | All registered player invoker components |
| `PendingDespawnSubjectsHeap` / `PendingSpawnSubjectsHeap` | Queued operations |

---

### Key Function Behaviors

#### `SetIsSystemEnabled` (`0x14386CED0`)
- Writes to global `byte_146D4ECC0`
- **Enable**: starts 3 recurring timers (spawn, despawn, update)
- **Disable**: cancels all timers, then **immediately hard-despawns all registered zombies** by looping `SpawnedSubjects` (stride 144) and disabling all EAIOFeaturesFlags on each
- Fires `OnSubsystemEnabledChanged` delegate

#### Spawn timer callback (`0x143868AA0`)
1. Calls `0x1438693A0` — tick/update subjects (update positions, distances, layer indices)
2. Calls `0x14386AC10` — process spawn queue
3. Iterates `PendingSpawnSubjectsHeap` backwards
4. Per entry: if `IsForcedToSpawn` OR `distance < spawnRadius²`: spawn actor
5. Uses `sub_14300FE50` (UWorld::SpawnActor — confirmed address) internally

#### `K2_DespawnSubject` (`0x14386BEC0`)
Blueprint params: `SubjectHandle` (int), `bDespawnImmediately` (bool), `SubjectRef`, `OverrideRadius` (float), `bAllowRespawnOnlyByHandle` (bool)
- Immediate → `sub_143866A30`: calls `SetAILogicEnabled(false)`, `SetCharacterMovementEnabled(false)`, applies EAIOFeaturesFlags to disable components
- Queued → adds to `DespawnedSubjects` heap

#### Visibility management (in spawn core `0x143869C00`)
Uses `_InterlockedCompareExchange` on EObjectFlags:
- `0x40000000` or `0x20000000` flag path controlled by `byte_146EFB965`
- This is how actors transition between "visible" and "hidden" states when spawned/despawned

---

### Global Addresses

| Symbol | Address |
|--------|---------|
| `AIOptimizerSubsystem::StaticClass` | `0x143863700` |
| `SetIsSystemEnabled` native | `0x14386CED0` |
| `RegisterInvoker` native | `0x14386C5E0` |
| `RegisterSubject` native | `0x14386C720` |
| `UnregisterInvoker` native | `0x14386D260` |
| `UnregisterSubject` native | `0x14386D400` |
| `K2_DespawnSubject` native | `0x14386BEC0` |
| Spawn timer callback | `0x143868AA0` |
| Spawn/feature-apply core | `0x143869C00` |
| `bIsSubsystemEnabled` global | `0x146d4ecc0` |
| `EAIOFeaturesFlags` metadata | `0x145efd6f0` |
| `/Script/AIOptimizer` string | `0x145efedd0` |

---

### SD-Online Implications — CRITICAL

1. **All zombies use AIOptimizer.** They are not persistent actors — they are spawned/
   despawned based on distance from registered "Invoker" actors. Without registering
   invoker components for each connected player, zombies will only spawn near the host.

2. **RegisterInvoker must be called for each remote player's pawn.** The subsystem
   uses the closest invoker for distance checks and feature-flag decisions. Register
   a new invoker when `K2_PostLogin` fires, unregister on `K2_OnLogout`.

3. **`SetIsSystemEnabled(false)` destroys all active zombies instantly** — any game
   logic that calls this (menus, loading screens, cinematics) will wipe the AI world.
   Monitor `byte_146d4ecc0` if zombie consistency matters across scene transitions.

4. **Feature flags are distance-banded.** A zombie near Player A (close band = full AI)
   and far from Player B (far band = AI off) uses the **closest invoker** distance.
   This means zombie behavior is set by the nearest registered player — correct for
   server-authoritative multiplayer.

5. **SpawnCapacityPerUpdate throttles spawning.** With multiple players each adding
   invokers, the pending spawn heap may grow large. May need to tune
   `SpawnCapacityPerUpdate` or call `ShrinkArrays` periodically on the server.

6. **Delegate hooks for server tracking:**
   - `OnSubjectSpawnedByOptimizerSubsystem` — zombie spawned
   - `OnSubjectDespawnedByOptimizerSubsystem` — zombie despawned  
   - Bind these on the server for authoritative entity tracking.

7. **Boss/scripted zombies** use `bCanBeRespawnedOnlyByHandle = true`. Server must
   track handle IDs (`K2_SpawnSubjectByHandle` / `K2_DespawnSubjectByHandle`) for these.

8. **`bIsBeyondLastLayer` = completely frozen** — beyond the outermost optimization
   layer radius: AI off, tick off, invisible. Still in `SpawnedSubjects` array but
   effectively suspended. In singleplayer this is fine; in multiplayer with players
   spread apart, ensure the outermost layer radius covers the largest player spread.

---

---

## Session 10: 2026-08-06 — PlayerArray Verification + Dedicated Server Audit

### AGameStateBase::PlayerArray — CONFIRMED

Property descriptor at `0x145c1d340` (`PlayerArray` in `AGameStateBase`):
- DWORD at `+0x38` encodes Offset_Internal = **0x0038 = 56 decimal**

**Confirmed**: `PlayerArray` (`TArray<APlayerState*>`) starts at `AGameStateBase + 0x38`.

TArray layout (16 bytes total):
| Offset | Field | Type |
|--------|-------|------|
| `+0x00` | Data | T** |
| `+0x08` | Num | int32 |
| `+0x0C` | Max | int32 |

Therefore:
- `PlayerArray.Data` at `AGameStateBase + 0x38`
- `PlayerArray.Num` at `AGameStateBase + 0x40`
- `PlayerArray.Max` at `AGameStateBase + 0x44`

**Both `+0x38` and `+0x40` claims from Session 5 are correct.**

Also confirmed: `GameState` property in `AGameModeBase` encodes Offset_Internal = `0x2F0`
(752) — consistent with `AGameModeBase` total size 824 bytes (0x338).

---

### Dedicated Server Support — CLIENT-ONLY CONFIRMED

**Finding**: `SurrounDead-Win64-Shipping.exe` is a **client-only binary**. There is no
dedicated server execution mode, no `*Server-Win64-Shipping.exe` variant.

**Evidence**:
1. Binary name follows client convention (`Win64-Shipping` not `Server-Win64-Shipping`)
2. Static imports include `XINPUT1_3` (gamepad), `dxgi` (Direct3D rendering), `DSOUND` (audio) — all absent in UE5 dedicated servers
3. No `IsRunningDedicatedServer`, `NM_Dedicated`, `bIsRunningAsDedicatedServer`, or UE5 dedicated server compile-flag strings found anywhere in the binary
4. Full rendering pipeline present in binary — servers skip all of this

**"Dedicated server" strings that ARE present** are:
- UProperty behavioral flags (`bSimulateSkeletalMeshOnDedicatedServer`, etc.) — exist in all UE5 builds
- `IsDedicatedServer` — a Steam API wrapper checking lobby type, not UE5 headless mode
- `SetDedicatedServer` at `0x145e2b448` — part of `ISteamGameServer` interface (for advertising a Steam listen server), confirmed by surrounding API: `LogOn`, `LogOnAnonymous`, `SetAdvertiseServerActive`, `SetBotPlayerCount`, `GetAuthSessionTicket`, `GetServerPublicIP`, `GetServerSteamID`

**SD-Online implication**: The game runs as a **Steam listen server** on the host's machine.
SD-Online must run entirely within one player's game instance (UE4SS + Lua + C++ mod).
There is no headless server binary to target. The host player's game is the authoritative server.

**Steam Game Server table** (`0x145e2c700`):
- Confirms `ISteamGameServer` API for listen-server advertising (lobbies, auth, bots)
- SD-Online's server-side bridge runs as a sidecar process that communicates with the host's game instance via IPC (the established file IPC or protocol-v3 socket protocol)

---

## Session 9: 2026-08-06 — StaticFindObject + Net Fields + FName::ToString + UWorld Timing

### StaticFindObject

**Address**: `0x140E71E70`  
**Thunk alias**: `0x140E720C0` (5-byte call, same target)  
**StaticFindObjectFastInternal** (hash table core): `0x140E72200`  
**Signature**: `UObject* __fastcall(UClass* Class, UObject* Outer, const TCHAR* Name, bool ExactClass)`

Usage to find a Blueprint class at runtime:
```cpp
typedef UObject*(*StaticFindObject_t)(UObject* Class, UObject* Outer, const wchar_t* Name, bool ExactClass);
StaticFindObject_t StaticFindObject = (StaticFindObject_t)0x140E71E70;

// ANY_PACKAGE = (UObject*)-1 — searches all packages
UClass* playerCharClass = (UClass*)StaticFindObject(
    nullptr,
    (UObject*)-1,
    L"/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C",
    false
);
```

**Confirmed by**: embedded error string "Illegal call to StaticFindObjectFast() while serializing object data or garbage collecting!" (UObjectGlobals.cpp line 350).

---

### GetAllActorsOfClass

- **Exec thunk** (Blueprint VM entry): `0x142EB4D20`
- **Implementation**: `0x142E904C0`

---

### APlayerController — Additional Fields

| Offset | Field | Type |
|--------|-------|------|
| `+0x2A0` (672) | `PlayerState` | APlayerState* |
| `+0x2D8` (728) | `Pawn` | APawn* |
| `+0x520` (1312) | `NetConnection` | UNetConnection* |

`NetConnection` confirmed from FObjectPropertyParams at `0x145D28450`.

---

### APlayerState — Extended Field Layout

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x140` (320) | `OwningController` | APlayerController* | |
| `+0x298` (664) | `Score` | float | |
| `+0x29C` (668) | `PlayerId` | int32 | |
| `+0x2A0` (672) | `CompressedPing` | uint8 | `ExactPing / 4` |
| `+0x2A4` (676) | `StartTime` | float | |
| `+0x2A8` (680) | `EngineMessageClass` | TSubclassOf<> (8 bytes) | |
| `+0x2B0` (688) | `ExactPing` | float | Raw C++ field, NOT a UPROPERTY |
| `+0x2B8` (696) | `SavedNetworkAddress` | FString | |
| `+0x330` (816) | `PlayerNamePrivate.Data` | TCHAR* | |
| `+0x338` (824) | `PlayerNamePrivate.ArrayNum` | int32 | |
| `+0x33C` (828) | `PlayerNamePrivate.ArrayMax` | int32 | |

**ExactPing note**: `ExactPing` is NOT a UPROPERTY. `GetPingInMilliseconds()` at `0x143281900` returns `ExactPing` if > 0, otherwise `CompressedPing * 4.0f`. For the C++ DLL, read directly: `*(float*)(playerState + 0x2B0)`.

---

### FName::ToString

**Address**: `0x140C9D940`  
**Signature**: `void FName::ToString(const FName* this, FString* result)`  
- `rcx` = FName* (8 bytes: ComparisonIndex uint32 at +0, Number uint32 at +4)  
- `rdx` = FString* (output, caller-allocated)

**Confirmed by**:
- `execGetObjectName` at `0x142FB76B0`: reads `*(QWORD*)(uobj + 0x18)` = NamePrivate, calls `0x140C9D940`
- `execGetDisplayName` at `0x142FB57E0`: same pattern

**UE5 FNamePool internals** (from disassembly):
```asm
mov ebx, [rcx]           ; ComparisonIndex (32-bit)
movzx eax, bx            ; chunk offset = idx & 0xFFFF
shr edx, 10h             ; chunk index = idx >> 16
lea r8, [0x146E57DC0]    ; GNamePool (FNamePool global)
add rcx, [r8+rdx*8+10h]  ; entry = GNamePool.Blocks[chunk] + offset*2
```

**GNamePool** global: `0x146E57DC0`

**UObject::NamePrivate** (FName) is at **UObject + 0x18** (24 decimal) — confirmed for all objects including UWorld.

**Fast C++ DLL world name read**:
```cpp
// world = UWorld* (from GetWorldFromContextObject)
uint32_t fname_idx = *(uint32_t*)(world + 0x18);   // ComparisonIndex
uint32_t fname_num = *(uint32_t*)(world + 0x1C);   // Number

FString nameStr = {};
typedef void(*FNameToString_t)(const void* fname, void* outStr);
FNameToString_t FName_ToString = (FNameToString_t)0x140C9D940;
FName_ToString((void*)(world + 0x18), &nameStr);
// nameStr.Data = *(wchar_t**)&nameStr  (points to world's map name)
```

---

### UWorld Timing Fields

All confirmed from disassembly of `execGetGameTimeInSeconds` (`0x142EB8070`):
```asm
call 0x1434ACF40          ; GetWorldFromContextObject → rax = UWorld*
movsd xmm0, [rax+6A8h]   ; read TimeSeconds as double
movsd [rsi], xmm0         ; write to output
```

| Field | Offset | Type | Notes |
|-------|--------|------|-------|
| `TimeSeconds` | `+0x6A8` (1704) | double | Authoritative server clock, updated every tick |
| `UnpausedTimeSeconds` | `+0x6B0` (1712) | double | Never paused |
| `RealTimeSeconds` | `+0x6B8` (1720) | double | Wall clock time |

**In UE5.3, all world time fields are double precision** (not float).

Also found on AGameStateBase:
| Field | Offset | Type | Notes |
|-------|--------|------|-------|
| `ReplicatedWorldTimeSeconds` | `+0x218` (536) | float | Network-replicated copy |

Exec thunks:
- `execGetGameTimeInSeconds`: `0x142EB8070`  
- `execGetServerWorldTimeSeconds` (AGameStateBase): `0x142AA3C00`

---

### Confirmed UObject Layout

| Offset | Field | Size |
|--------|-------|------|
| `+0x00` | vtable | 8 |
| `+0x08` | `ObjectFlags` | 4 |
| `+0x0C` | `InternalIndex` | 4 |
| `+0x10` | `ClassPrivate` (UClass*) | 8 |
| `+0x18` | `NamePrivate` (FName: ComparisonIndex + Number) | 8 |
| `+0x20` | `OuterPrivate` (UObject*) | 8 |

FName at `+0x18` confirmed by `execGetObjectName` decompile.

### FName::ToString

**Address**: `0x140C9D940`  
**Signature**: `void FName::ToString(const FName* this, FString* result)`  
- `rcx` = FName* (8 bytes: ComparisonIndex uint32 at +0, Number uint32 at +4)  
- `rdx` = FString* (output, caller-allocated)

**Confirmed by**:
- `execGetObjectName` at `0x142FB76B0`: reads `*(QWORD*)(uobj + 0x18)` = NamePrivate, calls `0x140C9D940`
- `execGetDisplayName` at `0x142FB57E0`: same pattern

**UE5 FNamePool internals** (from disassembly):
```asm
mov ebx, [rcx]           ; ComparisonIndex (32-bit)
movzx eax, bx            ; chunk offset = idx & 0xFFFF
shr edx, 10h             ; chunk index = idx >> 16
lea r8, [0x146E57DC0]    ; GNamePool (FNamePool global)
add rcx, [r8+rdx*8+10h]  ; entry = GNamePool.Blocks[chunk] + offset*2
```

**GNamePool** global: `0x146E57DC0`

**UObject::NamePrivate** (FName) is at **UObject + 0x18** (24 decimal) — confirmed for all objects including UWorld.

**Fast C++ DLL world name read**:
```cpp
// world = UWorld* (from GetWorldFromContextObject)
uint32_t fname_idx = *(uint32_t*)(world + 0x18);   // ComparisonIndex
uint32_t fname_num = *(uint32_t*)(world + 0x1C);   // Number

FString nameStr = {};
typedef void(*FNameToString_t)(const void* fname, void* outStr);
FNameToString_t FName_ToString = (FNameToString_t)0x140C9D940;
FName_ToString((void*)(world + 0x18), &nameStr);
// nameStr.Data = *(wchar_t**)&nameStr  (points to world's map name)
```

---

### UWorld Timing Fields

All confirmed from disassembly of `execGetGameTimeInSeconds` (`0x142EB8070`):
```asm
call 0x1434ACF40          ; GetWorldFromContextObject → rax = UWorld*
movsd xmm0, [rax+6A8h]   ; read TimeSeconds as double
movsd [rsi], xmm0         ; write to output
```

| Field | Offset | Type | Notes |
|-------|--------|------|-------|
| `TimeSeconds` | `+0x6A8` (1704) | double | Authoritative server clock, updated every tick |
| `UnpausedTimeSeconds` | `+0x6B0` (1712) | double | Never paused |
| `RealTimeSeconds` | `+0x6B8` (1720) | double | Wall clock time |

**In UE5.3, all world time fields are double precision** (not float).

Also found on AGameStateBase:
| Field | Offset | Type | Notes |
|-------|--------|------|-------|
| `ReplicatedWorldTimeSeconds` | `+0x218` (536) | float | Network-replicated copy |

Exec thunks:
- `execGetGameTimeInSeconds`: `0x142EB8070`  
- `execGetServerWorldTimeSeconds` (AGameStateBase): `0x142AA3C00`

---

### Confirmed UObject Layout

| Offset | Field | Size |
|--------|-------|------|
| `+0x00` | vtable | 8 |
| `+0x08` | `ObjectFlags` | 4 |
| `+0x0C` | `InternalIndex` | 4 |
| `+0x10` | `ClassPrivate` (UClass*) | 8 |
| `+0x18` | `NamePrivate` (FName: ComparisonIndex + Number) | 8 |
| `+0x20` | `OuterPrivate` (UObject*) | 8 |

FName at `+0x18` confirmed by `execGetObjectName` decompile.

---

### Updated Plugins Summary

| Plugin | Package | Role | SD-Online Impact |
|--------|---------|------|----------------|
| SteamCore | `/Script/SteamCore` | Steam sessions | Hooks for session join/create |
| EasyMultiSave | `/Script/EasyMultiSave` | Save/load all state | `OnPlayerLoaded` = sync point |
| TechTree | `/Script/TechTree` | Research progression | Per-player, no sync needed |
| ArchVisCharacter | `/Script/ArchVisCharacter` | BP_PlayerCharacter base | C++ character class |
| **AIOptimizer** | `/Script/AIOptimizer` | Proximity AI lifecycle | **Must register invoker per player** |

---

## Session 11: 2026-08-06 — Vital Stats via UE4SS Runtime Dump

### Method

UE4SS Lua mod dumped `ForEachProperty` on `BP_PlayerCharacter_C` and its
component classes at runtime. Source files committed to repo:
- `bp_playercharacter_props.txt` — 225 properties on the character class
- `bp_components_props.txt` — MedicalComponent, HungerThirstComponent,
  StaminaComponent, RadiationComponent

All vital stat fields use **double** (64-bit float), consistent with UE5.3.

---

### BP_PlayerCharacter_C — Component Pointer Fields

Blueprint component pointers on the character (ObjectProperty):

| Offset | Field | Notes |
|--------|-------|-------|
| `+0x6E8` (1768) | `RadioComponent` | |
| `+0x708` (1800) | `MinimapSystemComponent` | |
| `+0x720` (1824) | `SwimmingComponent` | |
| `+0x730` (1840) | `LockPickingComponent` | |
| `+0x7D0` (2000) | `MedicalComponent` | Health, bleed, broken bone |
| `+0x7D8` (2008) | `VehicleDrivingComponent` | |
| `+0x7E0` (2016) | `BuildingComponent` | |
| `+0x7F0` (2032) | `RadiationComponent` | Radiation level |
| `+0x7F8` (2040) | `HungerThirstComponent` | Hunger and thirst |
| `+0x800` (2048) | `StaminaComponent` | Stamina, sprint state |
| `+0x808` (2056) | `PhotoModeComponent` | |

**Blueprint fields start at `+0x6E8`** (= `+0x6E0` C++ class end + 8 bytes for `RadioComponent`).
This confirms the IDA Session 3 finding that AArchVisCharacter occupies 0x6E0 bytes.

---

### MedicalComponent Field Layout

Accessed via: `*(UObject**)(character + 0x7D0)`

| Offset | Field | Type |
|--------|-------|------|
| `+0xC0` (192) | `Bleed` | bool |
| `+0xC1` (193) | `HeavyBleed` | bool |
| `+0xC2` (194) | `BrokenBone` | bool |
| `+0xC8` (200) | `Character` (back-ref) | ObjectProperty |
| `+0xD0` (208) | **`Health`** | **double** |
| `+0xD8` (216) | **`MaxHealth`** | **double** |
| `+0x108` (264) | `RadiationSickness` | bool |

---

### HungerThirstComponent Field Layout

Accessed via: `*(UObject**)(character + 0x7F8)`

| Offset | Field | Type |
|--------|-------|------|
| `+0xC0` (192) | `MaxHunger` | double |
| `+0xC8` (200) | **`CurrentHunger`** | **double** |
| `+0xD0` (208) | `MaxThirst` | double |
| `+0xD8` (216) | **`CurrentThirst`** | **double** |
| `+0xE8` (232) | `ReduceHungerAmount` | double |
| `+0xF0` (240) | `ReduceThirstAmount` | double |
| `+0x110` (272) | `HungerMultiplier` | double |
| `+0x118` (280) | `ThirstMultiplier` | double |
| `+0x120` (288) | `HungerDebuff` | bool |
| `+0x121` (289) | `ThirstDebuff` | bool |

---

### StaminaComponent Field Layout

Accessed via: `*(UObject**)(character + 0x800)`

| Offset | Field | Type |
|--------|-------|------|
| `+0xC0` (192) | `MaxStamina` | double |
| `+0xC8` (200) | **`CurrentStamina`** | **double** |
| `+0xD0` (208) | `CurrentlySprinting` | bool |
| `+0xD8` (216) | `SprintSpeed` | double |
| `+0xE0` (224) | `WalkSpeed` | double |
| `+0xF0` (240) | `StaminaDrain` | double |
| `+0x100` (256) | `StaminaRecoverAmount` | double |

---

### RadiationComponent Field Layout

Accessed via: `*(UObject**)(character + 0x7F0)`

| Offset | Field | Type |
|--------|-------|------|
| `+0xC0` (192) | `MaxRadiation` | double |
| `+0xC8` (200) | **`CurrentRadiation`** | **double** |
| `+0xD8` (216) | `ReduceRadiationAmount` | double |
| `+0xE8` (232) | `RadDeduction` | double |
| `+0xF0` (240) | `InRadArea` | bool |

---

### BP_PlayerCharacter_C — Death State

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x13E1` (5089) | `PlayerDead` (flag) | bool | Set when player dies |
| `+0x15B0` (5552) | `KeepInventoryOnDeath` | MulticastDelegate | Fires on death |
| `+0x1658` (5720) | `PlayerDead` (event) | MulticastDelegate | Death broadcast |

---

### Fast C++ DLL Vital Read Pattern

```cpp
// character = BP_PlayerCharacter_C* (AActor-derived)

// Health
UObject* med = *(UObject**)(character + 0x7D0);
double health    = *(double*)(med + 0xD0);
double maxHealth = *(double*)(med + 0xD8);

// Hunger / Thirst
UObject* ht = *(UObject**)(character + 0x7F8);
double hunger = *(double*)(ht + 0xC8);
double thirst = *(double*)(ht + 0xD8);

// Stamina
UObject* stam = *(UObject**)(character + 0x800);
double stamina = *(double*)(stam + 0xC8);

// Radiation
UObject* rad = *(UObject**)(character + 0x7F0);
double radiation = *(double*)(rad + 0xC8);

// Death flag (no component hop needed)
bool isDead = *(bool*)(character + 0x13E1);
```

---

## Session 12: 2026-08-05 — AssetRegistry Blueprint Class Path Mining

### Method

Parsed `AssetRegistry.json` (exported from pak alongside `pak_all_files.txt`) using
Python to extract `BlueprintGeneratedClass` entries. These are the paths used for
`SpawnActor` in UE4SS C++ mods and for `FindFirstOf` / `StaticLoadObject`.

UE class path format for spawning: ObjectPath from AssetRegistry (already ends in `_C`).

---

### Key BlueprintGeneratedClass Paths

| Class | Full Object Path |
|-------|-----------------|
| `BP_PlayerCharacter_C` | `/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C` |
| `BP_PlayerCharacter_Child_C` | `/Game/Blueprints/BP_PlayerCharacter_Child.BP_PlayerCharacter_Child_C` |
| `BP_PlayerController_C` | `/Game/Blueprints/BP_PlayerController.BP_PlayerController_C` |
| `BP_SurroundeadGameMode_C` | `/Game/Blueprints/BP_SurroundeadGameMode.BP_SurroundeadGameMode_C` |
| `BP_SurroundeadGameState_C` | `/Game/Blueprints/BP_SurroundeadGameState.BP_SurroundeadGameState_C` |
| `BP_StaticMeshPickup_C` | `/Game/Inventory/Items/BP_StaticMeshPickup.BP_StaticMeshPickup_C` |
| `BP_SkeletalMeshPickup_C` | `/Game/Inventory/Items/BP_SkeletalMeshPickup.BP_SkeletalMeshPickup_C` |
| `BP_LootContainer_C` | `/Game/Inventory/Containers/BP_LootContainer.BP_LootContainer_C` |
| `Buildable_MASTER_C` | `/Game/Blueprints/BuildingSystem/Actors/Buildable_MASTER.Buildable_MASTER_C` |
| `BP_VehicleMaster_C` | `/Game/Blueprints/Vehicles/BP_VehicleMaster.BP_VehicleMaster_C` |
| `BP_Zombie_C` | `/Game/AI/Zombies/BP_Zombie.BP_Zombie_C` |
| `BP_ZombieBoss_C` | `/Game/AI/Zombies/Boss/BP_ZombieBoss.BP_ZombieBoss_C` |

**No `BP_GameMode_C` or `BP_PlayerState_C`** — those are named `BP_SurroundeadGameMode_C`
and the PlayerState (if it exists as a Blueprint) would be similarly prefixed. The
`FindFirstOf("BP_PlayerState_C")` call in PropertyDumper v2 may return nothing; the
actual class is likely a pure C++ APlayerState or named differently.

---

### Spawnable Vehicle Types (17 total)

All inherit from `BP_VehicleMaster_C`. Specific types:

| Short Name | Object Path |
|------------|-------------|
| `Vehicle_4x4Jeep_C` | `/Game/Blueprints/Vehicles/Types/4x4Jeep/Vehicle_4x4Jeep.Vehicle_4x4Jeep_C`* |
| `Vehicle_Ambulance_C` | `/Game/Blueprints/Vehicles/Types/Ambulance/Vehicle_Ambulance.Vehicle_Ambulance_C`* |
| `Vehicle_BigRig_C` | `/Game/Blueprints/Vehicles/Types/BigRig/Vehicle_BigRig.Vehicle_BigRig_C`* |
| `Vehicle_Buggy_C` | `/Game/Blueprints/Vehicles/Types/Buggy/Vehicle_Buggy.Vehicle_Buggy_C`* |
| `Vehicle_Charger_C` | `/Game/Blueprints/Vehicles/Types/Charger/Vehicle_Charger.Vehicle_Charger_C`* |
| `Vehicle_PickupTruck_C` | `/Game/Blueprints/Vehicles/Types/PickupTruck/Vehicle_PickupTruck.Vehicle_PickupTruck_C`* |
| `Vehicle_Humvee_C` | `/Game/Blueprints/Vehicles/Types/Humvee/Vehicle_Humvee.Vehicle_Humvee_C`* |
| `Vehicle_RV_C` | `/Game/Blueprints/Vehicles/Types/RV/Vehicle_RV.Vehicle_RV_C`* |
| `Vehicle_SUV_C` | `/Game/Blueprints/Vehicles/Types/SUV/Vehicle_SUV.Vehicle_SUV_C`* |

*`_C` suffix appended — AssetRegistry only shows the Blueprint asset; append `_C` for the generated class.

---

### Item Pickup Class Path Pattern

591 pickup blueprints found. All inherit from `BP_StaticMeshPickup_C` (static mesh)
or `BP_SkeletalMeshPickup_C` (weapons/clothing with skeletal mesh).

Pattern: `/Game/Inventory/Items/Pickups/<Category>/<Name>.<Name>_C`

Examples:
- `/Game/Inventory/Items/Pickups/Weapons/Firearms/Rifles/BP_AK74Pickup.BP_AK74Pickup_C`
- `/Game/Inventory/Items/Pickups/Consumables/Medical/BP_BandagesPickup.BP_BandagesPickup_C`
- `/Game/Inventory/Items/Pickups/Attachments/Ammo/BP_9mmAmmoPickup.BP_9mmAmmoPickup_C`

For entity_manager::spawn_entity_actor(), spawning the **base** `BP_StaticMeshPickup_C`
is sufficient — the server tells the client which item via `itemId` string, and the
existing pickup actor reads its mesh from a data table lookup. Alternatively, spawn
the specific subclass for correct mesh without data table dependency.

---

### Proxy Actor Strategy — Using BP_PlayerCharacter_C

The existing `proxy_manager.cpp` looks for `"BP_RemoteProxy_C"` which does not exist
in the pak. The correct approach is to spawn `BP_PlayerCharacter_C` directly as the
proxy — it's the actual player class with correct animations and mesh.

**Resolved class path for `spawn_proxy()`:**
```
/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C
```

This eliminates the need for a custom proxy Blueprint entirely.

**Concern**: `BP_PlayerCharacter_C` has full game logic (AI optimizer registration,
component ticks, inventory, etc.). For a remote proxy we want it passive. Options:
1. Spawn it and immediately call `DisableInput` + detach from controller (no PC assigned = inert).
2. Use `BP_Zombie_C` as proxy (has skeleton/animations but simpler logic). Risk: wrong skeleton rig.
3. Create a minimal proxy Blueprint (requires dev tools we don't have in cooked build).

**Recommendation**: Option 1 — spawn `BP_PlayerCharacter_C` with no controller assigned.
UE4 actors with no PlayerController assigned do not process player input and their
blueprint Tick will still run, but the character will be static without a controller
sending movement commands. The mod controls position via `K2_SetActorLocationAndRotation`.

---

### Building System

`Buildable_MASTER_C` is the base for all player-built structures. Subclass pattern:
`/Game/Blueprints/BuildingSystem/Actors/<Category>/Buildable_<Name>.Buildable_<Name>_C`

For `WorldEntityKind::PlacedStructure`, the server would need to track which specific
Buildable subclass was placed. The entity descriptor's `classPath` field already
supports this.

---

## Session 12b: 2026-08-06 — AssetRegistry Deep Dive: Item Database + Replication Structs

### Item Database — JigsawItem_DataAsset_C (589 items)

Every item in the game is a `JigsawItem_DataAsset_C` instance, named `DA_<ItemName>`.

**Path pattern**: `/Game/Inventory/Items/DataAssets/<Category>/<Name>.<Name>`

Examples:
- `/Game/Inventory/Items/DataAssets/Weapons/Firearms/Rifles/DA_AK74.DA_AK74`
- `/Game/Inventory/Items/DataAssets/Consumables/Medical/DA_Bandages.DA_Bandages`
- `/Game/Inventory/Items/DataAssets/Attachments/Ammo/DA_556Ammo.DA_556Ammo`

**SD-Online implication**: The `itemId` field in `WorldEntityDescriptor` and `LocalSlot`
should reference the `DA_` asset name (e.g. `"DA_AK74"`) not the pickup Blueprint class.
This is the canonical item identifier used throughout the Jigsaw system.

The mapping between DataAsset and pickup Blueprint is 1:1 by naming convention:
strip `DA_` prefix → add `BP_` prefix + `Pickup_C` suffix = `BP_AK74Pickup_C`.

---

### Jigsaw Inventory — Replicated Structs

The Jigsaw inventory plugin was designed with network replication in mind.

| Struct | Path | Purpose |
|--------|------|---------|
| `RepItemInfo` | `.../Jigsaw/Data/RepItemInfo` | Per-slot replicated item data |
| `S_JigPayload` | `.../Jigsaw/Data/S_JigPayload` | Inventory slot payload |
| `S_RepActorData` | `.../Jigsaw/Components/Data/S_RepActorData` | Replicated actor inventory |
| `S_RepNonActorData` | `.../Jigsaw/Components/Data/S_RepNonActorData` | Replicated non-actor inventory |
| `S_ReplicatedContainerInfo` | `.../Jigsaw/Components/Data/S_ReplicatedContainerInfo` | Container sync info |
| `S_ServerEquippedItems` | `.../Jigsaw/Data/S_ServerEquippedItems` | Server-side equipped items |
| `S_RepAttachmentInfo` | `.../Jigsaw/Data/S_RepAttachmentInfo` | Weapon attachment replication |
| `S_RepWeaponAttachment` | `.../Jigsaw/Data/S_RepWeaponAttachment` | Weapon attachment details |
| `S_EquipmentIDInfo` | `.../Jigsaw/Components/Data/S_EquipmentIDInfo` | Equipment slot IDs |
| `DefaultItemInfo` | `.../Jigsaw/Data/DefaultItemInfo` | Base item info struct |

These structs indicate the Jigsaw plugin had a planned server-authoritative replication
path that shipped inactive. For SD-Online, `RepItemInfo` is the key struct defining
the minimum per-slot sync payload. Its fields need runtime dumping via PropertyDumper.

---

### Save System

- `BFL_SaveGames` — Blueprint Function Library wrapping EasyMultiSave. All save/load
  calls go through this BFL.
- `Struct_KeepInventory` — the struct at `PlayerController + 0x890`.
  Defines which inventory slots survive death.

---

### AIOptimizer Structs

- `S_AIOptimization` — Optimization layer config (used by `AIOSubjectComponent.OptimizationLayers`)
- `S_AIRespawn` — Zombie respawn settings per spawner
- `S_AISpawner` — Spawner config per zone

---

## Session 14: 2026-08-06 — Runtime Dump: Levelling, Skills, Narrative, Zombie, Vehicle

### Method

PropertyDumper v7 Lua mod. `FindFirstOf` on each component class name.
Output: `bp_components2_props.txt`.

---

### LevellingComponent_C — Field Layout

Accessed via: `*(UObject**)(controller + 0x868)`

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0xC0` (192) | `CurrentLevel` | IntProperty | Player level |
| `+0xC4` (196) | `LevelCap` | IntProperty | Max level |
| `+0xC8` (200) | `CurrentXP` | DoubleProperty | |
| `+0xD0` (208) | `CurrentMaxXP` | DoubleProperty | XP needed for next level |
| `+0xD8` (216) | `CurrentPercentage` | DoubleProperty | 0.0–1.0 progress |
| `+0xE0` (224) | `BufferXP` | DoubleProperty | Pending XP to apply |
| `+0xE8` (232) | `RemainingXP` | DoubleProperty | |
| `+0xF0` (240) | `MultiplyValue` | DoubleProperty | XP multiplier |
| `+0xF8` (248) | `Difficulty_MultiplierValue` | DoubleProperty | Difficulty XP scale |

Minimal sync needed for SD-Online: `CurrentLevel` + `CurrentXP`.

---

### TechTreeComponent_C — Field Layout

Accessed via: `*(UObject**)(controller + 0x870)`

| Offset | Field | Type |
|--------|-------|------|
| `+0x148` (328) | `SkillPoints` | IntProperty |
| `+0x14C` (332) | `CurrentPoints` | IntProperty |

---

### PassiveSkillsComponent_C — Field Layout

Accessed via: `*(UObject**)(controller + 0x878)` (also on `StaminaComponent` back-ref)

9 skill tracks: Fitness, Strength, Toughness, Sneaking, FirstAid, Marksmanship,
Reloading, Thief, Fishing, Scavenging. Each track follows the pattern:

```
Current<Skill>XP   (double)
Max<Skill>XP       (double)
Current<Skill>Level (double)
Max<Skill>Level    (double)
```

Starting at `+0xC0`, packed sequentially. Multipliers and UI delegates follow.

For SD-Online: only the `Current<Skill>Level` fields need syncing (not XP or multipliers —
those are derived). 10 doubles = 80 bytes total for the sync payload.

---

### NarrativeComponent — Field Layout

C++ native component (Narrative Marketplace plugin). All-delegate layout with key data:

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x180` (384) | **`OnJoinedParty`** | MulticastInlineDelegateProperty | **Party system hook** |
| `+0x190` (400) | **`OnLeaveParty`** | MulticastInlineDelegateProperty | **Party system hook** |
| `+0x230` (560) | `QuestList` | ArrayProperty | Active/completed quests |
| `+0x248` (584) | `MasterTaskList` | MapProperty | Task progress map |
| `+0x2A0` (672) | `PartyComponent` | ObjectProperty | |
| `+0x2A8` (680) | `OwnerPC` | ObjectProperty | Back-ref to PlayerController |

**Critical discovery**: the Narrative plugin already has `OnJoinedParty` / `OnLeaveParty`
party hooks. This means quest state already has multiplayer-awareness stubs at the
plugin level. SD-Online can broadcast these delegates when a remote player connects/
disconnects to keep quest NPCs consistent across the session.

For SD-Online Phase 1: quests are **host-authoritative** — guest players inherit the
host's `QuestList`. No per-player quest sync needed initially.

---

### BP_Zombie_C — Result

`UberGraphFrame` at `+0x8B0` is the only Blueprint property — all zombie state
(health, AI behavior, attack) is in native C++ parent classes invisible to
`ForEachProperty`. The C++ zombie class is at least `0x8B0` bytes.

**Implication**: reading zombie health requires IDA investigation of the C++ parent.
For Phase 2 position sync this is not blocking — zombie positions are managed
server-side via AIOptimizer and broadcast via `EntitySpawn`/`EntityDespawn` frames.

---

### Vehicle_PickupTruck_C — Result

Zero Blueprint properties. All vehicle state (speed, health, fuel, occupants) is
in C++ native classes. Same situation as zombies — IDA required to read state.

For Phase 2: vehicle position sync uses the same `Movement` frame type as players
(x/y/z + yaw). Entity type `WorldEntityKind::Vehicle` is already defined in the
protocol.

**Confirmed (Session 15b)**: `Vehicle_PickupTruck_C` and `Vehicle_Charger_C` both
return 0 Blueprint properties. Position/rotation sync uses `K2_GetActorLocation` /
`K2_GetActorRotation` — available on any AActor, no Blueprint property access needed.
Vehicle health, fuel, and occupant data require IDA investigation of C++ parent classes
(not blocking for Phase 2 position sync).

---

## Session 15: 2026-08-06 — Jigsaw Inventory + Multiplayer Component Deep Dump

### Method

PropertyDumper v9. Found `BP_JigHelperComp_C`, `BP_JigMultiplayer_C`, and full
`JigsawItem_DataAsset_C` field layout. Output: `bp_jigsaw_props.txt`.

---

### BP_JigHelperComp_C — Field Layout (Player Character Component)

The primary Jigsaw helper component attached to the player character.

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0xA8` (168) | `EquipmentUIDs` | MapProperty | Slot index → item UID map |
| `+0xF8` (248) | `ServerEquippedItems` | StructProperty | Full equipped loadout (`S_ServerEquippedItems`) |
| `+0xAD0` (2768) | `RepPrimitiveActorsData` | ArrayProperty | Replicated primitive actor (static pickup) data |
| `+0xAE0` (2784) | `RepActorsData` | ArrayProperty | Replicated actor inventory data |
| `+0xAF0` (2800) | `TraceToActors` | BoolProperty | |
| `+0xAF8` (2808) | `EquipmentIDSlotConfig` | MapProperty | Equipment slot definitions |
| `+0xB98` (2968) | `ActiveWeapon` | StructProperty | Currently equipped weapon info |
| `+0xBA0` (2976) | `OnActiveWeaponSlotChanged` | MulticastDelegate | Fires on weapon switch |
| `+0xBB0` (2992) | `PreviewChar` | ObjectProperty | Character preview reference |
| `+0xBB8` (3000) | `CurrentTracActor` | ObjectProperty | Actor being looked at |
| `+0xBC0` (3008) | `CurrentInteractOptions` | MapProperty | Interaction options map |
| `+0xC30` (3120) | `OnEquipmentUpdated` | MulticastDelegate | **Fires on any gear change** |

**SD-Online usage**:
- Hook `OnEquipmentUpdated` to detect clothing/equipment changes → send to server for remote player appearance updates
- Read `ServerEquippedItems` at +0xF8 to get full loadout for `PlayerProgress` sync
- Read `ActiveWeapon` at +0xB98 to update animation state on remote proxies

---

### BP_JigMultiplayer_C — Field Layout (Multiplayer Component)

Found on loot containers and the player character. The Jigsaw plugin's built-in
server-replication component — designed for multiplayer but shipped inactive.

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0xA8` (168) | `MainJigContainers` | ArrayProperty | All inventory containers |
| `+0xB8` (184) | `LocalJSIContainers` | ArrayProperty | Local-only containers |
| `+0xC8` (200) | `MPComponentType` | StructProperty | Role: Player/Container/Vendor |
| `+0xD0` (208) | `PendingRequests` | ArrayProperty | **Built-in server request queue** |
| `+0xE0` (224) | `MainContainersIDs` | ArrayProperty | Container UID list |
| `+0xF0` (240) | `ContainersSettings` | ArrayProperty | Per-container settings |
| `+0x100` (256) | `RefillContainerTimerInSeconds` | DoubleProperty | Loot respawn timer |
| `+0x108` (264) | `InventoryWidgetClass` | ClassProperty | UI widget class |
| `+0x12A` (298) | `AllowDroppingItems` | BoolProperty | Enable/disable item dropping |
| `+0x130` (304) | `PickupInfo` | StructProperty | Pickup actor info (216 bytes) |
| `+0x208` (520) | `VendorCurrentCurrencyAmount` | DoubleProperty | Trader cash amount |
| `+0x220` (544) | `VendorAcceptedCurrencyID` | ObjectProperty | Accepted currency DataAsset |
| `+0x230` (560) | `ItemsToCraft` | ArrayProperty | Available crafting recipes |
| `+0x250` (592) | `MonitorContainerUID` | StructProperty | Watched container UID |
| `+0x280` (640) | `InventoryWeight` | ArrayProperty | Per-container weight data |
| `+0x2A1` (673) | `Looted` | BoolProperty | Container has been looted |
| `+0x2D8` (728) | `OnWeightUpdated` | MulticastDelegate | Fires on weight change |
| `+0x2E8` (744) | `OnInventoryOpenClose` | MulticastDelegate | Fires on inventory open/close |
| `+0x2F8` (760) | `ClientSaveDataReceived` | MulticastDelegate | **Fires when server sends save data** |
| `+0x308` (776) | `OnRefillContainer` | MulticastDelegate | Fires when container refills |

**SD-Online usage**:
- Hook `ClientSaveDataReceived` at +0x2F8 → inject remote player initial inventory state
- `PendingRequests` at +0xD0 → the MP component already has a request queue we can populate
- `AllowDroppingItems` at +0x12A → set to false on remote player proxies (no phantom drops)
- `Looted` flag at +0x2A1 → sync container looted state across players

---

### JigsawItem_DataAsset_C — Field Layout (Item Template)

The canonical item data asset. Every item type has one `DA_` instance.

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x30` (48) | **`ItemId`** | NameProperty | **The canonical item ID (FName: `"DA_AK74"` etc.)** |
| `+0x38` (56) | `Name` | TextProperty | Display name |
| `+0x50` (80) | `Description` | TextProperty | |
| `+0x68` (104) | `SlotDimensions` | StructProperty | Grid size (1×1, 2×1, etc.) |
| `+0x78` (120) | `Type` | StructProperty | Item type enum |
| `+0x80` (128) | `Rarity` | StructProperty | Rarity tier |
| `+0x88` (136) | `Count` | IntProperty | Stack count |
| `+0x8C` (140) | `CanStack` | BoolProperty | |
| `+0x90` (144) | `MaxStack` | IntProperty | |
| `+0x94` (148) | `UniqueServerID` | StructProperty | **Per-instance unique ID (20 bytes)** |
| `+0xA8` (168) | `Weight` | DoubleProperty | |
| `+0xC0` (192) | `Durability` | DoubleProperty | Current durability |
| `+0xC8` (200) | `MaxDurability` | DoubleProperty | |
| `+0x128` (296) | `PickupClass` | ClassProperty | Pickup actor Blueprint class |
| `+0x140` (320) | `IsContainer` | BoolProperty | |
| `+0x4E8` (1256) | `BuildActorClass` | ClassProperty | Buildable actor class |

**`ItemId` at +0x30** is the FName used as the canonical item identifier throughout
the protocol. The `LocalSlot.itemId` and `WorldEntityDescriptor.itemId` strings in
`protocol.hpp` should match these FName values (e.g. `"DA_AK74"`, `"DA_Bandages"`).

**`UniqueServerID` at +0x94** is a 20-byte struct (likely `FGuid` padded, or a custom
UUID struct). This is the per-instance identifier for items with unique state
(durability, attachments). For ground item entity sync, this is the UID to track.

**`PickupClass` at +0x128** — the DataAsset stores its own pickup Blueprint class
reference. This means when spawning a ground item, we can read `PickupClass` from
the DataAsset rather than deriving it from the `DA_` → `BP_Pickup_C` naming convention.

---

---

## Session 16: 2026-08-06 — Inner Struct Layouts: UniqueServerID, ServerEquippedItems, PickupInfo

### Method

PropertyDumper v11 — `prop:GetStruct()` successfully walked into nested StructProperty
fields. Output: `bp_structs_props.txt`.

---

### UniqueServerID / MonitorContainerUID — FGuid (16 bytes)

```
A  IntProperty  +0x0
B  IntProperty  +0x4
C  IntProperty  +0x8
D  IntProperty  +0xC
```

**Confirmed FGuid** — standard UE4 `FGuid` (4 × uint32). All Jigsaw container and
item instance UIDs are FGuids. Total struct size: 16 bytes (+ 4 bytes padding to
next field on DataAsset). For protocol encoding: encode as 16-byte binary or UUID
string `XXXXXXXX-XXXXXXXX-XXXXXXXX-XXXXXXXX`.

---

### ActiveWeapon / Type / Rarity / MPComponentType — FGameplayTag

All single-field structs with just `TagName` (NameProperty). These are `FGameplayTag`
wrappers — the tag name is an FName like `"Item.Type.Weapon.Rifle"` or `"Rarity.Legendary"`.

For `ActiveWeapon`: the currently held weapon slot is identified by its tag name, not
an index. For remote proxy animation state, read `TagName` to determine which animation
stance to play.

---

### S_ServerEquippedItems — 21 Equipment Slots × 120 bytes each

The full player loadout struct at `BP_JigHelperComp_C + 0xF8`. Total size: 2520 bytes.
Each slot is a StructProperty of 120 bytes (likely a `RepItemInfo` or `DefaultItemInfo`).

| Index | Slot Name | Offset within S_ServerEquippedItems |
|-------|-----------|-------------------------------------|
| 0 | `EquippedFacewear` | `+0x000` |
| 1 | `EquippedHeadwear` | `+0x078` |
| 2 | `EquippedEyewear` | `+0xF0` |
| 3 | `EquippedAccessory` | `+0x168` |
| 4 | `EquippedTorso` | `+0x1E0` |
| 5 | `EquippedGloves` | `+0x258` |
| 6 | `EquippedLegs` | `+0x2D0` |
| 7 | `EquippedFeet` | `+0x348` |
| 8 | `EquippedContainer` | `+0x3C0` |
| 9 | `EquippedBodyArmor` | `+0x438` |
| 10 | `EquippedBackpack` | `+0x4B0` |
| 11 | `EquippedPrimary` | `+0x528` |
| 12 | `EquippedSecondary` | `+0x5A0` |
| 13 | `EquippedSidearm` | `+0x618` |
| 14 | `EquippedMelee` | `+0x690` |
| 15 | `EquippedThrowable` | `+0x708` |
| 16 | `EquippedFlashlight` | `+0x780` |
| 17 | `EquippedBinoculars` | `+0x7F8` |
| 18 | `EquippedGPS` | `+0x870` |
| 19 | `EquippedCompass` | `+0x8E8` |
| 20 | `EquippedFishingRod` | `+0x960` |

Full read path:
```cpp
uint8_t* helperComp = (uint8_t*)(uintptr_t)FindFirstOf("BP_JigHelperComp_C");
uint8_t* equipped   = helperComp + 0xF8;  // S_ServerEquippedItems base
// Slot 11 (Primary weapon) inner struct starts at:
uint8_t* primary    = equipped + 0x528;
```

For SD-Online: minimum visual sync needs slots 1 (Headwear), 4 (Torso), 11 (Primary),
10 (Backpack). Each 120-byte slot struct contains a `RepItemInfo` — needs one more
PropertyDumper pass to reveal its inner layout.

---

### BP_JigMultiplayer_C.PickupInfo — Full Layout

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x00` | `UniqueServerID` | StructProperty (FGuid) | Item instance UID |
| `+0x10` | `IsContainer` | BoolProperty | |
| `+0x18` | `ContainerDimension` | StructProperty | Grid size |
| `+0x28` | `ItemInfo` | StructProperty | 120-byte item data (same as equipment slot) |
| `+0xA0` | `ContainerMotherID` | StructProperty (FGuid) | Parent container UID |
| `+0xB0` | `SlotIndex` | IntProperty | Slot in parent container |
| `+0xB4` | `Rotated` | BoolProperty | Item rotated in grid |
| `+0xB8` | `InContainerIndex` | IntProperty | Index within container |
| `+0xC0` | `PickupRef` | ObjectProperty | The actual pickup actor |
| `+0xC8` | `SubContainers` | ArrayProperty | Nested containers |

**`PickupRef` at +0xC0** is the direct actor reference to the ground pickup —
`BP_JigMultiplayer_C` already holds a pointer to its own pickup actor.

---

---

## Session 17: 2026-08-06 — The Universal Slot Struct (120 bytes)

### Result

Both `S_ServerEquippedItems` slot structs and `PickupInfo.ItemInfo` use the **exact
same 120-byte struct**. This is the canonical per-item-instance representation in Jigsaw.

| Offset | Logical Name | Type | Size | Notes |
|--------|-------------|------|------|-------|
| `+0x00` | **`ItemID`** | ObjectProperty | 8 | Pointer to `JigsawItem_DataAsset_C` (the `DA_` asset) |
| `+0x08` | **`Count`** | IntProperty | 4 | Stack count |
| `+0x0C` | *(padding)* | | 4 | |
| `+0x10` | `ItemVec` | StructProperty | 16 | Unknown — likely FGuid (UniqueServerID for this instance) |
| `+0x20` | `Weight` | DoubleProperty | 8 | |
| `+0x28` | `Price` | DoubleProperty | 8 | |
| `+0x30` | `Durability` | StructProperty | 16 | Likely {Current:double, Max:double} |
| `+0x40` | `Stats` | ArrayProperty | 16 | Random stats TArray |
| `+0x50` | `Pending` | DoubleProperty | 8 | Pending durability/stat change |
| `+0x58` | `CustomDataKey` | ArrayProperty | 16 | Extended custom data keys |
| `+0x68` | `CustomDataValue` | ArrayProperty | 16 | Extended custom data values |

Total: `0x68 + 16 = 0x78 = 120 bytes` ✓

**`ItemID` at +0x0** is a UObject pointer to the `JigsawItem_DataAsset_C` instance
for that item type. Reading it gives the DataAsset, from which `ItemId` (FName at
+0x30) gives the canonical string identifier (`"DA_AK74"` etc.).

**`ItemVec` at +0x10** — confirmed `{X: double, Y: double}` = inventory grid position
(column, row). NOT a UID — no per-instance FGuid exists in the slot struct.

**`Durability` at +0x30** — confirmed `{X: double current, Y: double max}`.

### Complete Slot Struct Layout (Session 18 confirmed)

| Offset | Field | Type | Detail |
|--------|-------|------|--------|
| `+0x00` | `ItemID` | ObjectProperty (8b) | → `JigsawItem_DataAsset_C*` → `ItemId` FName at DA+0x30 |
| `+0x08` | `Count` | int32 (4b) | Stack size |
| `+0x0C` | *(padding)* | (4b) | |
| `+0x10` | `ItemVec` | `{X:double, Y:double}` (16b) | Inventory grid position (col, row) |
| `+0x20` | `Weight` | double (8b) | |
| `+0x28` | `Price` | double (8b) | |
| `+0x30` | `Durability` | `{X:double current, Y:double max}` (16b) | |
| `+0x40` | `Stats` | TArray (16b) | Random stat modifiers |
| `+0x50` | `Pending` | double (8b) | Pending change buffer |
| `+0x58` | `CustomDataKey` | TArray (16b) | Arbitrary key extensions |
| `+0x68` | `CustomDataValue` | TArray (16b) | Arbitrary value extensions |

**No per-instance FGuid in the slot struct.** Item-instance uniqueness is managed
server-side. SD-Online assigns its own UUIDs and maps them to (`ItemID` FName, `Count`,
`Durability.X`) tuples. The `JigsawItem_DataAsset_C.UniqueServerID` FGuid (at DA+0x94)
is the DataAsset-level UID, not per slot instance.

### Minimal SD-Online Slot Sync Payload

```
ItemID FName string  (variable, ~8–24 bytes: "DA_AK74")
Count  int32         (4 bytes)
DurabilityX double   (8 bytes, current durability)
```
Full loadout sync: 21 slots × ~24 bytes = ~504 bytes per player update.

### Read Pattern for Active Weapon Name

```cpp
// Get equipped primary weapon's item name from the slot struct
uint8_t* helperComp = ...; // BP_JigHelperComp_C instance
uint8_t* equipped   = helperComp + 0xF8;        // S_ServerEquippedItems
uint8_t* primary    = equipped + 0x528;          // EquippedPrimary slot
UObject* itemDA     = *(UObject**)(primary + 0x0); // ItemID ObjectProperty
// itemDA is now the JigsawItem_DataAsset_C*
// FName at +0x30 = ItemId (e.g. "DA_AK74")
FName itemId = *(FName*)(itemDA + 0x30);
```

---

## Session 19: 2026-08-06 — Final In-Game Sweep

### BP_JigMultiplayer_C — Player Character Check

`FindFirstOf("BP_JigMultiplayer_C")` outer = `Container_CompoundCrate_C`.
**`BP_JigMultiplayer_C` is NOT on the player character — only on world containers.**

Player inventory sync path: `BP_JigHelperComp_C` exclusively.
No `BP_JigMultiplayer_C` hook needed for player state replication.

---

### GameState.AllUIDs — Element Type: int32

`AllUIDs` at `BP_SurroundeadGameState_C + 0x338` is `TArray<int32>`.

World item instance UIDs are **simple 32-bit integers** — not FGuids, not FStrings.
Each spawned pickup/container in the world has an integer UID tracked in this array.

**SD-Online implication**: The server's world item UID registry can use int32 keys.
The existing `WorldEntityDescriptor.entityId` (uint64) is a superset — use the lower
32 bits to match the game's native UID when needed.

---

### Struct_KeepInventory — Inner Fields

Located at `BP_PlayerController_C + 0x890`.

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x00` | `MainJigContainers` | ArrayProperty | Container refs preserved on death |
| `+0x10` | `MainUIDs` | ArrayProperty | Item UIDs preserved on death |
| `+0x20` | `Weights` | ArrayProperty | Weight data per preserved container |

This struct defines which inventory containers and their UIDs survive player death.
For SD-Online: not needed for Phase 2 — death/respawn can use the existing
`Death`/`Respawn` frame types without inventory preservation sync.

---

### BP_TraderMaster_C

Found as `BP_ShakesConesTrader_C`. Blueprint fields:
- `Audio` (ObjectProperty) at +0x358
- `SkeletalMesh2/1` (ObjectProperty) at +0x360/+0x368

All trader logic (inventory, currency, buy/sell) is native C++. Traders are
server-authoritative world actors — for Phase 2, treat them as static world
entities; no sync needed beyond their world position.

---

### BFL_SaveGames_C / Jigsaw Interfaces

Not found as world objects — function libraries and interfaces are type definitions
only, not instantiable. EasyMultiSave hooks require IDA investigation (pending).

---

### Revised Item Identity Strategy for SD-Online

| Use Case | Identifier |
|----------|-----------|
| Item type ID in protocol | `JigsawItem_DataAsset_C.ItemId` FName (`"DA_AK74"`) |
| Per-instance unique ID | `JigsawItem_DataAsset_C.UniqueServerID` (20-byte struct) |
| Spawn pickup actor class | `JigsawItem_DataAsset_C.PickupClass` ClassProperty |
| Server item ID in `LocalSlot` | `ItemId` FName string |

---

## Session 13: 2026-08-06 — Runtime Dump: PlayerController, GameMode/State, AIOptimizer Components

### Method

PropertyDumper v3 Lua mod ran successfully in-game. Output: `bp_extended_props.txt`.
Committed to repo.

---

### BP_PlayerState_C — NOT FOUND

`FindFirstOf("BP_PlayerState_C")` returned nothing. The game either uses a pure C++
`APlayerState` (no Blueprint) or a class named differently. The PlayerState functionality
is **folded into BP_PlayerController_C** (see below).

---

### BP_PlayerController_C — Field Layout

C++ `APlayerController` base ends at ~`0x858` (UberGraphFrame offset). Blueprint fields:

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x860` (2144) | `NarrativeComponent` | ObjectProperty | Quest/story system |
| `+0x868` (2152) | `LevellingComponent` | ObjectProperty | XP and level |
| `+0x870` (2160) | `TechTreeComponent` | ObjectProperty | Research progression |
| `+0x878` (2168) | `PassiveSkillsComponent` | ObjectProperty | Passive perks |
| `+0x880` (2176) | `Widget` | ObjectProperty | HUD widget reference |
| `+0x888` (2184) | `AutoSaveEnabled` | BoolProperty | |
| `+0x890` (2192) | `KeepInventory` | StructProperty | Death inventory retention setting |
| `+0x8C0` (2240) | `Level` | NameProperty | **Save slot / level name** |
| `+0x8C8` (2248) | `Forename` | StrProperty | Character first name |
| `+0x8D8` (2264) | `Surname` | StrProperty | Character last name |
| `+0x8E8` (2280) | `Sex` | StrProperty | |
| `+0x8F8` (2296) | `Age` | StrProperty | |
| `+0x908` (2312) | `Occupation` | ByteProperty | Enum — class/occupation type |
| `+0x90C` (2316) | `ZombieKills` | IntProperty | |
| `+0x910` (2320) | `BossZombieKills` | IntProperty | |
| `+0x914` (2324) | `AnimalKills` | IntProperty | |
| `+0x918` (2328) | `HumanKills` | IntProperty | |
| `+0x91C` (2332) | `DaysSurvived` | IntProperty | |
| `+0x920` (2336) | `DistanceTravelled` | DoubleProperty | |
| `+0x928` (2344) | `InfestationsDestroyed` | IntProperty | |
| `+0x92C` (2348) | `RespawnPointEnabled` | BoolProperty | |
| `+0x930` (2352) | `RespawnLoc` | StructProperty | FVector respawn location |

**Key SD-Online implication**: All player progression and identity lives in the
**PlayerController**, not a PlayerState. `BP_PlayerController_C` is local-only in a
single-player game — the mod must read and transmit this data manually. The
`PlayerProgress` struct in `protocol.hpp` is the right vessel for syncing it.

**`Forename`/`Surname`** at `+0x8C8`/`+0x8D8` can serve as a display name for remote
players without needing Steam API calls.

**`RespawnLoc`** at `+0x930` is a StructProperty (FVector, 24 bytes). Reading it:
```cpp
// RespawnLoc is an FVector3d (3 × double = 24 bytes)
double* respawnLoc = (double*)(controller + 0x930);
float rx = (float)respawnLoc[0];
float ry = (float)respawnLoc[1];
float rz = (float)respawnLoc[2];
```

---

### BP_SurroundeadGameMode_C — Field Layout

Extremely thin — game logic is almost entirely in C++.

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x338` (824) | `UberGraphFrame` | StructProperty | BP base starts at 0x340 |
| `+0x340` (832) | `DefaultSceneRoot` | ObjectProperty | |
| `+0x348` (840) | `Autosave` | BoolProperty | |
| `+0x350` (848) | `DiscordRpc` | ObjectProperty | Discord rich presence |

---

### BP_SurroundeadGameState_C — Field Layout

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0x2F0` (752) | `UberGraphFrame` | StructProperty | BP base starts at 0x2F8 |
| `+0x2F8` (760) | `DefaultSceneRoot` | ObjectProperty | |
| `+0x300` (768) | `SnapActorRef` | ObjectProperty | FModel/snap system ref |
| `+0x308` (776) | `FirstCaptureDone` | BoolProperty | |
| `+0x310` (784) | `ItemsQueue` | ArrayProperty | Queued item operations |
| `+0x320` (800) | `SnapDelay` | DoubleProperty | |
| `+0x328` (808) | `AllInspectedIDs` | ArrayProperty | IDs of already-inspected items |
| `+0x338` (824) | `AllUIDs` | ArrayProperty | **All world item UIDs** |

**`AllUIDs` at `+0x338`** is likely the authoritative list of all spawned item unique
IDs in the world. This is the deduplication registry the server needs — reading this
TArray gives the set of items that already exist and should not be re-spawned.

Reading it:
```cpp
// TArray layout: Data*(8) + Num(4) + Max(4)
UObject* gs = FindFirstOf("BP_SurroundeadGameState_C");
uint8_t* gsPtr = (uint8_t*)(uintptr_t)gs;
void**  uidData = *(void***)(gsPtr + 0x338);  // data pointer
int32_t uidNum  = *(int32_t*)(gsPtr + 0x340); // count
```

---

### AIOSubjectComponent — Field Layout

Attached to every zombie. C++ base ends before `+0xC8`.

| Offset | Field | Type | Notes |
|--------|-------|------|-------|
| `+0xC8` (200) | `Handle` | StructProperty | AIOptimizer registration handle |
| `+0xD0` (208) | `OnOptimizationUpdate` | MulticastInlineDelegateProperty | Fires on LOD tier change |
| `+0xE0` (224) | `OnPreDespawn` | MulticastDelegate | **Hook here to intercept despawn** |
| `+0xF0` (240) | `OnPostSpawned` | MulticastDelegate | Fires after AIOptimizer spawns |
| `+0x100` (256) | `Spawner` | ObjectProperty | Back-ref to owning spawner actor |
| `+0x108` (264) | `bAllowSubsystemToAutoDespawn` | BoolProperty | **Set false = prevent auto-despawn** |
| `+0x108` (264) | `bCanBeUpdatedBySubsystem` | BoolProperty | Bit-packed in same byte as above |
| `+0x10C` (268) | `OverrideSubsystemDespawnRadius` | FloatProperty | Custom despawn radius |
| `+0x110` (272) | `Priority` | ByteProperty | Optimization priority tier |
| `+0x118` (280) | `DataClass` | ClassProperty | AI data class reference |
| `+0x120` (288) | `OptimizationLayers` | ArrayProperty | LOD layer definitions |
| `+0x130` (304) | `bShouldCalculateIsSeen` | BoolProperty | Visibility check toggle |

**`bAllowSubsystemToAutoDespawn` at `+0x108`** — setting this to `false` on a zombie
prevents AIOptimizer from despawning it when players move away. This is how we keep
server-authoritative zombie positions locked when the host's invoker radius shrinks
but a remote player's invoker is still within range.

---

### AIOInvokerComponent — Field Layout

Only one Blueprint-visible field:

| Offset | Field | Type |
|--------|-------|------|
| `+0xA0` (160) | `DebugWidget` | ObjectProperty |

**All invoker logic (RegisterInvoker, UnregisterInvoker, radius, tick) is in C++.**
No Blueprint properties for radius or registration state. Access to registration is
via the C++ AIOptimizer plugin API — `UObjectGlobals::FindObject` + virtual dispatch,
or via the Lua `RegisterInvoker()`/`UnregisterInvoker()` wrappers if they exist.

The sparse property table confirms we cannot manipulate invoker registration purely
through property reads — we must call C++ methods.

---

### BP_StaticMeshPickup_C / BP_SkeletalMeshPickup_C — Zero Blueprint Properties

**Confirmed**: both pickup base classes have zero UE-reflected Blueprint properties.

Items dropped during dump:
- Static mesh: `BP_WaterCanteenPickup_C` (leaf of `BP_StaticMeshPickup_C`) — 0 properties
- Skeletal mesh: `BP_BattleReadyGlockPickup_C` (leaf of `BP_SkeletalMeshPickup_C`) — 0 properties

`GetSuperClass()` throws at the C++ class boundary, preventing reflection of
native UPROPERTY fields on parent classes. Any per-instance data (quantity, durability)
is in native C++ structs invisible to UE4SS `ForEachProperty`.

**Key implication for SD-Online**: item identity is **entirely encoded in the class path**.
There is no `UID` or `Quantity` UPROPERTY to read from the actor. The protocol's
`WorldEntityDescriptor.classPath` field is sufficient to identify a ground item type.
For quantity/stack count, the client sending an `ItemDropResult` message already
includes the item data — the server does not need to read it from the pickup actor.

**Item spawning strategy** (for remote clients):
- Spawn `BP_StaticMeshPickup_C` for non-weapon/clothing items, OR
- Spawn the exact subclass (e.g. `BP_AK74Pickup_C`) for correct mesh without data table
- Position via `K2_SetActorLocationAndRotation`
- Type is communicated via `classPath` in `EntitySpawn` frame

---

## Session 22 — AIOptimizer RegisterInvoker + EasyMultiSave OnPlayerLoaded

**Goal**: Find the C++ implementation of `AIOInvokerComponent::RegisterInvoker` and locate the `EasyMultiSave::OnPlayerLoaded` delegate broadcast callsite for potential hooking.

---

### AIOptimizer — RegisterInvoker / UnregisterInvoker

**UAIOWorldSubsystem** (the AI Optimizer subsystem) hosts a TArray of registered invokers.

| Symbol | Address | Notes |
|--------|---------|-------|
| `EXEC_RegisterInvoker` | `0x14386c5e0` | Native thunk, reads `AIOInvokerComponent*` from stack, appends to TArray at `[subsystem+0x80]`, deduplicates, calls sort helper |
| `EXEC_UnregisterInvoker` | `0x14386D260` | Mirror of RegisterInvoker — removes from TArray |
| `EXEC_LoopSubjects` | `0x14386c5c0` | Reads subject count from `[rdx+0x20]`, jumps to `sub_143868AA0` |
| `Z_Construct_UClass_UAIOWorldSubsystem` | `0x143863700` | Class lazy getter |
| Sort/update after register | `sub_143865DA0` | Called after appending to invoker TArray |
| AIO EXEC table (data) | `0x145efd030` | Interleaved (code ptr, FName ptr) pairs |
| AIO UFunction descriptors | `0x146d4f3d0` area | 21 registered UFunctions |

**Key offset confirmed**: `UAIOWorldSubsystem.RegisteredInvokers` TArray = **`subsystem+0x80`**.
This matches `AIOInvokerComponent.Handle` at `+0xC8` (Session 13) and confirms the registration flow is pure C++, not Blueprint-callable state we need to track for the mod.

---

### EasyMultiSave — UEMSObject Delegate Layout

All three async-completion delegates live sequentially on `UEMSObject`:

| Delegate | Offset | Inner UFunction getter |
|----------|--------|----------------------|
| `OnPlayerLoaded` | **`+0x50`** | `0x1438a2760` |
| `OnPartitionLoaded` | **`+0x60`** | `0x1438a2700` |
| (third delegate, probably `OnSaveComplete`) | **`+0x70`** | `0x1438a2730` |

**Source**: Property descriptor at `0x145f1dc90`; offset confirmed at descriptor+0x30 = `0x0000000000500001` (0x50 in bytes [2:3]).

**EMS delegate signature strings** (all in data section, referenced from UFunction descriptors):
- `EmsLoadPlayerComplete__DelegateSignature` → `0x145f1d628`
- `EmsLoadLevelComplete__DelegateSignature` → `0x145f1d7e8`
- `EmsLoadPartitionComplete__DelegateSignature` → `0x145f1dbc8`

**EMS EXEC functions found** (all on `UEMSObject`):

| Function | Address |
|----------|---------|
| `ImportSaveThumbnail` | `0x1438cb6b0` |
| `IsLevelStreamingActive` | `0x1438cb870` |
| `IsSavingOrLoading` | `0x1438cb900` |
| `IsWorldPartition` | `0x1438cb9e0` |
| `LoadPlayerActorsCustom` | `0x1438cbb00` |
| `LoadRawObject` | `0x1438cbfd0` |
| `SaveCustom` | `0x1438cc830` |
| `SavePlayerActorsCustom` | `0x1438cd050` |
| `SaveRawObject` | `0x1438cd480` |
| `SetActorSaveProperties` | `0x1438cd9f0` |
| `SetCurrentSaveGameName` | `0x1438cdd80` |
| `SetCurrentSaveUserName` | `0x1438cdf10` |

**UEMSAsyncLoadGame** (the async action that wraps save load operations):
- Parent class: `UBlueprintAsyncActionBase`
- Vtable: `0x145F21C98` (only destructor overridden — no additional vfuncs)
- CDO constructor: `sub_1438C6320`
- Class init: `sub_1438A68E0`
- Class size: 136 bytes (0x88)

**Broadcast callsite**: The `OnPlayerLoaded.Broadcast()` call is a native C++ function that directly iterates the TMulticastScriptDelegate's InvocationList at `[UEMSObject+0x50]`. No named export or string anchor was found for the broadcast function itself — it is inlined or called from a private completion handler. The delegate descriptor-table xrefs all point to data only (UClass/UFunction registration tables), not to any code that calls Broadcast.

**Hooking strategy** (UE4SS Lua):
```lua
-- Bind to OnPlayerLoaded to detect save restore completion
local ems = FindFirstOf("UEMSObject")
if ems and ems:IsValid() then
    -- Bind dynamic delegate via Blueprint callable
    -- Or use RegisterHook on the EXEC_OnPlayerLoaded wrapper if it exists
    -- Simplest: hook EMS's async action node called from SurrounDead's
    -- BP_PlayerController_C (the node will call the EMS BFL which fires the delegate)
end
```

**Practical conclusion for SDO**: The `OnPlayerLoaded` delegate fires when EasyMultiSave finishes restoring the player's save data into game objects. Hooking it (or binding to it via Lua) would be the correct time to:
1. Read final vitals from `MedicalComponent`, `HungerThirstComponent`, etc.
2. Send a `ProfileRevision` frame with accurate state
3. Teleport the pawn to the server-authoritative respawn position

The gap analysis already identifies `send_profile_revision()` reading stale `BridgeState` (Gap 2). Adding an EMS hook here would fix that correctly without polling.

---

## Session 23: 2026-08-07 — Blueprint Boundary Confirmation + RegisterInvoker Native Algorithm

**Goal**: Resolve the ground-item UID candidates from `ground_item_research.txt` (all guesses came back
empty), and locate the EasyMultiSave `OnPlayerLoaded` broadcast callsite left open in Session 22.

---

### Ground Item Classes — Resolved via Static Pak Analysis, Not IDA

The Session-22-era candidate list (`BP_GroundItem_C`, `BP_ItemPickup_C`, etc.) was blind guessing and
found nothing in-world. `pak_all_files.txt` already answers it: **580 pickup Blueprints** live under
`SurrounDead/Content/Inventory/Items/Pickups/...` (Ammo, Buildable, Consumables, CraftingMaterials,
Currency, Equipment, etc.), all descending from the already-confirmed `BP_StaticMeshPickup_C` /
`BP_SkeletalMeshPickup_C` (zero reflected Blueprint properties, per Session 21). No further scan needed.

No `BP_RemotePlayer_C` / `BP_ProxyPlayer_C` / ghost-actor equivalent exists anywhere in the pak listing
either. Confirms remote players must be represented by real `BP_PlayerCharacter_C` instances driven by
the bridge — there is no premade stand-in actor to spawn instead.

---

### `AllUIDs` and Vitals Components Are Pure Blueprint — Confirmed Dead End for IDA

Two static checks against the shipping binary, run directly against the live IDB:

- `find_regex("AllUIDs")` across all strings: **zero matches**.
- Function name search for `Pickup`, `Jig*`, `Medical`, `Hunger`, `Thirst`, `Stamina`, `Radiation`:
  **zero native symbols**.

This confirms `BP_SurroundeadGameState_C.AllUIDs` (+0x338) and every vitals/pickup component are
interpreted Blueprint bytecode over cooked reflection data loaded from the `.pak` at runtime — none of
it is compiled into the executable. There is no native "append to AllUIDs" function to reverse; it
doesn't exist as disassemblable code. Same conclusion for `MedicalComponent`/`HungerThirstComponent`
(Gap 2) — the offsets already documented from live UE4SS runtime dumps are the only way to reach these,
not IDA.

**Implication**: IDA is only useful in this binary for native engine/plugin internals (AIOptimizer,
EasyMultiSave, core UE5 functions). Everything gameplay-specific in SurrounDead — items, vitals,
pickups — is Blueprint and invisible to static analysis. Future sessions chasing game-logic offsets
should go straight to live UE4SS/Lua probing instead of searching the binary.

---

### EasyMultiSave `OnPlayerLoaded` Broadcast Callsite — Confirmed Unreachable (Re-verified)

Re-checked Session 22's conclusion directly against the live IDB rather than trusting the writeup:

- `LoadPlayerActorsCustom` (`0x1438CBB00`) has **zero code cross-references** — its only xrefs are data
  (UFunction descriptor table entries). It is only ever invoked through the Blueprint VM's UFunction
  dispatch, never called directly from other native code.
- The delegate signature string `EmsLoadPlayerComplete__DelegateSignature` (`0x145F1D628`) has exactly
  one xref: its own UFunction descriptor. No code path references it.

Confirms Session 22's finding rather than overturning it: the actual `Broadcast()` callsite is inlined
or resolved generically at runtime and is not something static disassembly can pin down. Hooking via
live Lua (binding to the delegate, or hooking the BFL node that wraps it) remains the only viable path.

---

### AIOptimizer `RegisterInvoker` — Full Native Algorithm (not just the offset)

Session 22 located the offset (`UAIOWorldSubsystem.RegisteredInvokers` TArray at `subsystem+0x80`) but
not the actual append logic. Decompiling `EXEC_RegisterInvoker` (`0x14386C5E0`) directly:

```cpp
// Simplified from decompilation. a1 = subsystem, v8 = invoker component (read off the
// Blueprint stack by the FFrame-stepping preamble at the top of the function).
uint8_t* data = *(uint8_t**)(a1 + 128);   // array data ptr
uint32_t num  = *(uint32_t*)(a1 + 136);   // array count
// dedup scan: 16 bytes per entry, skip if v8 already present
// ... if not found:
*(uint32_t*)(a1 + 136) = num + 1;
if (num + 1 > *(uint32_t*)(a1 + 140))     // array max — grow if needed
    sub_1436F9060(a1 + 128, num);
data = *(uint8_t**)(a1 + 128);
*(uint64_t*)(data + 16*num)     = (uint64_t)v8;              // component pointer
*(uint64_t*)(data + 16*num + 8) = *(uint64_t*)((uint8_t*)v8 + 144); // cached field, see below
sub_143865DA0(a1);
```

Each entry is **16 bytes**: `{ComponentPtr, *(ComponentPtr+0x90)}`. The second field is read directly
off the invoker component at registration time — **`AIOInvokerComponent+0x90` is a previously
undocumented native (non-reflected) pointer field**, one qword before the only known Blueprint-visible
field (`DebugWidget` at `+0xA0`, Session 13). Likely a cached owner/transform reference used by the
subsystem's tick to avoid repeated `GetOwner()` resolution — not yet identified precisely.

**Correction to Session 22**: `sub_143865DA0(a1)` was assumed to be a "sort/update helper." Decompiling
it shows it's actually gated behind a global debug flag (`byte_146D4ECC0`) and, when set, schedules three
deferred closures bound to `sub_143868AA0`/`sub_143868A70`/`sub_143869BA0` into task slots at
`subsystem+176/184/192`. This reads as AIOptimizer's debug-overlay/telemetry update path, not a sort —
it's a no-op in a shipping build with the debug flag off. Doesn't change correctness of registration,
but the earlier characterization was wrong and should not be relied on.

**Practical implication for SD-Online**: because the second array field is read from a live pointer
offset on the component (not a value the caller supplies), a fake/hand-crafted array entry would need
that offset to hold a valid pointer or risk a crash on the next subsystem tick that reads it. The safe
implementation path for keeping zombies active near a remote player is to spawn a lightweight actor with
a **real** `AIOInvokerComponent` at the remote player's position and register it normally (either by
routing through the existing Blueprint-callable `RegisterInvoker` UFunction via reflection, or by
replicating the append algorithm above verbatim against a genuine component pointer) — not to synthesize
an entry directly.

---

## Session 24: 2026-08-07 (continued) — AIOInvokerComponent+0x90 Identified

**Goal**: identify the previously-unknown cached field at `AIOInvokerComponent+0x90` found in Session 23.

Traced the read side by decompiling the subject-culling tick (`sub_143868AA0`, previously named
`EXEC_LoopSubjects`'s jump target) and the distance helper it calls, `sub_143867E10`:

```cpp
// sub_143867E10 — nearest-invoker distance for a subject at *a2 (FVector3d)
for each entry in RegisteredInvokers (subsystem+0x80, stride 16):
    cachedPtr = entry.field1;               // == *(ComponentPtr + 0x90) at registration time
    if (cachedPtr) {
        rootComp = *(void**)(cachedPtr + 0x1A0);   // AActor::RootComponent
        loc      = *(FVector3d*)(rootComp + 0x260); // USceneComponent world location, if rootComp valid
    } else {
        loc = <hardcoded sentinel vector>;   // fallback when the cached actor is gone
    }
    dist = min(dist, |loc - subjectPos|^2);
```

`AActor+0x1A0 = RootComponent` and `RootComponent+0x260 (608) = world location` are both already
confirmed elsewhere in this doc (Session 5/9 area) — this is the same pattern, not a new offset.

**Conclusion**: `AIOInvokerComponent+0x90` caches the **owning `AActor*`**, captured once at
`RegisterInvoker` time so the per-tick culling loop can read the invoker's current world position via
`Actor->RootComponent->WorldLocation` without repeated `GetOwner()` resolution. If the cached actor
pointer is null (e.g. destroyed), the tick falls back to a hardcoded sentinel vector instead of crashing
— so a stale/invalid pointer here fails safe rather than corrupting the tick, but a genuinely garbage
(non-null, non-actor) pointer would not, since it's dereferenced unconditionally at `+0x1A0`.

**Practical implication for SD-Online**: confirms Session 23's conclusion — there is no safe shortcut to
registering a "virtual" invoker without a real actor. The minimum viable fake invoker is: spawn any
actor with a valid `RootComponent` (a bare `USceneComponent` is enough, no mesh needed) at the remote
player's position, attach a real `AIOInvokerComponent`, and register it through the normal path. Moving
that actor's root component each frame (via the existing native root-component move calls already
documented in this file) is sufficient to keep the cached location fresh — no need to re-register.

---

## Session 25: 2026-08-07 (continued) — UE4SS Built-In SDK Dump: Crash Finding + Vitals Validation

**Goal**: stop reconstructing class layouts one property at a time. UE4SS ships a built-in CXX Header
Generator (`Ctrl+H`, `[CXXHeaderGenerator]` in `UE4SS-settings.ini`) that dumps every currently-loaded
class — native and Blueprint — as a `.hpp` with property names and byte offsets in one pass
(`DumpOffsetsAndSizes = 1` is the default). This had never been run for this game before this session.

### Negative finding: `LoadAllAssetsBefore*` reliably crashes this game

Both `[ObjectDumper] LoadAllAssetsBeforeDumpingObjects` and
`[CXXHeaderGenerator] LoadAllAssetsBeforeGeneratingCXXHeaders` force-load every asset in the game before
dumping, intended to produce a fully exhaustive dump regardless of what's currently loaded. **Enabling
either one crashes the game outright**, even on a 64GB-RAM machine — this is not a memory-capacity
problem. `UE4SS.log` stops cold immediately after `Loading all assets...` with no further output and no
crash dump written (`crash_*.dmp` stays 0 bytes). Something in this game's asset set (likely dev-only,
platform-specific, or otherwise-unreachable content) does not tolerate being force-loaded outside its
normal load order.

**Conclusion**: do not set either `LoadAllAssetsBefore*` flag to `1` for this game. Both are confirmed
back at their default of `0`. Get a rich a set of classes loaded organically instead (be in-world, open
inventory, get near a zombie, get in a vehicle) before dumping.

### Successful dump (defaults, no force-load)

With both flags at `0`, `Ctrl+H` generated 2,407 header files in `CXXHeaderDump/` in 2.6 seconds, no
crash. This pass happened to catch the native vitals components (they're always loaded once a character
exists) but not yet `BP_SurroundeadGameState_C`, `BP_JigHelperComp_C`, pickup classes, or the AIOptimizer
plugin classes — those need a pass while genuinely in-world with inventory/zombies/vehicles interacted
with. Re-run `Ctrl+H` (and `Ctrl+J` for a live object dump, and `Ctrl+Numpad7` for actors) during an
actual play session to fill those in.

### Vitals offsets — fully confirmed against Gap 2/4, with types now known

Every offset guessed from live `ForEachProperty` dumps in earlier sessions checks out exactly against
the authoritative dump, and the dump adds the missing type information:

| Class | Field | Offset | Type |
|-------|-------|--------|------|
| `UMedicalComponent_C` | `Health` | `+0xD0` | `double` |
| `UMedicalComponent_C` | `MaxHealth` | `+0xD8` | `double` |
| `UHungerThirstComponent_C` | `CurrentHunger` | `+0xC8` | `double` |
| `UHungerThirstComponent_C` | `CurrentThirst` | `+0xD8` | `double` |
| `UStaminaComponent_C` | `CurrentStamina` | `+0xC8` | `double` |
| `URadiationComponent_C` | `CurrentRadiation` | `+0xC8` | `double` |
| `ULevellingComponent_C` | `CurrentLevel` | `+0xC0` | **`int32`** (previously unspecified) |
| `ULevellingComponent_C` | `CurrentXP` | `+0xC8` | `double` |
| `ULevellingComponent_C` | `LevelCap` | `+0xC4` | `int32` (new) |

All five inherit from `UBaseComponent_C` (`Player`/`Controller` pointers at `+0xA8`/`+0xB0`, own
`UberGraphFrame` at `+0xB8`, size `0xB8`), which itself extends native `UActorComponent` (confirmed size
`0xA0` in this build). This is why every vitals component's own fields start at `+0xC0`/`+0xC8` — it's
the same Blueprint base class pattern, not a coincidence.

**Practical implication for SD-Online**: gap 2's `read_local_progress()` can now be implemented with
fully confirmed offsets and types for health/hunger/thirst/stamina/radiation/level/XP — no further
research needed on these fields specifically.

---

## Session 26: 2026-08-07 (continued) — BP_JigMultiplayer_C: An Already-Built Replication Layer

**Correction to method, first**: UE4SS filenames drop the `_C` Blueprint suffix (`BP_SurroundeadGameState.hpp`,
not `BP_SurroundeadGameState_C.hpp`). The first pass over the dump missed this and wrongly looked like
`BP_SurroundeadGameState`/`BP_JigHelperComp`/pickup classes hadn't loaded. They had. Full re-check:

### Correction to Session 21: pickups are not propertyless

"`BP_StaticMeshPickup_C` / `BP_SkeletalMeshPickup_C` — Zero Blueprint Properties" was wrong. Confirmed
layout (`ABP_StaticMeshPickup_C : public AStaticMeshActor`, size `0x310`):

| Offset | Field |
|--------|-------|
| `+0x2B0` | `UBP_JigPickupComponent_C* BP_JigPickupComponent` |
| `+0x2B8` | `UBP_JigMultiplayer_C* BP_JigMultiplayer` |
| `+0x2C0` | `TMap<FGameplayTag, FText> InteractOptions` |

Every ground pickup carries its own `BP_JigMultiplayer_C` component instance. This was invisible to the
earlier `ForEachProperty` pass (likely queried the wrong class or an instance where the component
hadn't initialized) and completely invisible to IDA (confirmed Session 23) since it's 100% Blueprint —
but is fully visible via the UE4SS CXX dump, which reads live reflection data rather than static binary
content. **Methodological note for future sessions**: when IDA and raw-property dumps both come up
empty for a Blueprint class, try the CXX Header Generator before concluding it's unreachable — it
appears to succeed where targeted single-class property walks failed.

### `BP_JigMultiplayer_C` — the real inventory replication system

`UBP_JigMultiplayer_C : public UActorComponent`, size `0x318`, ~300 member functions. The naming
convention (`SERVER_RequestDropItem`, `CLIENT_NewItemAdded`, `MC_UpdateCount`, etc.) exactly matches
Unreal's own Server/Client/Multicast RPC pattern. This is not a thin wrapper — it's the actual mechanism
the base game uses to move items between containers, drop/pickup, equip, craft, reload magazines, and
sync vendor state. Every one of these functions addresses items and containers by **`FGuid`**, not the
`int32` UIDs in `GameState.AllUIDs` and not `classPath` strings:

- Item identity: `FGuid ItemUID` (per-instance)
- Container identity: `FGuid ContainerUID` / `MainContainersIDs` (`TArray<FGuid>`)
- Item *type* (as opposed to instance) is still `UJigsawItem_DataAsset_C*` (the `DA_*` asset — confirms
  earlier `classPath`/DA-name findings are about item type, not instance identity)

Full field layout and the complete function list are in `CXXHeaderDump/BP_JigMultiplayer.hpp` (committed
alongside this session) — not reproduced here given its size.

Key fields:

| Offset | Field |
|--------|-------|
| `+0xA8` | `TArray<FS_ReplicatedContainerInfo> MainJigContainers` |
| `+0xE0` | `TArray<FGuid> MainContainersIDs` |
| `+0x130` | `FContainerPickupsInfo PickupInfo` (0xD8 bytes) |
| `+0x250` | `FGuid MonitorContainerUID` |

**Practical implication for SD-Online**: this changes the shape of the right answer for items/inventory
sync (gaps 1, 11, 12, 13, 14 in the protocol gap analysis). Rather than the bridge inventing its own
wire format for drop/pickup/move and hoping it lines up with game state, hooking these existing
`SERVER_*`/`CLIENT_*`/`MC_*` functions directly would ride on replication logic the game already
implements and tests — and `FGuid` (16 bytes, not `int32` or a variable-length string) is the correct
wire type for per-instance item/container identity. Whether this replication path is actually wired up
to real networking in a live build (vs. dormant/local-only scaffolding) is not yet confirmed and is the
natural next thing to check — e.g. by hooking one of these functions in Lua and seeing if it fires
during normal single-player play.

### `BP_JigHelperComp_C` — equipment layout fully confirmed

`UBP_JigHelperComp_C : public UActorComponent`, size `0xC40`. Confirms the Session 15 finding exactly:

| Offset | Field |
|--------|-------|
| `+0xA8` | `TMap<FGameplayTag, FGuid> EquipmentUIDs` |
| `+0xF8` | `FS_ServerEquippedItems ServerEquippedItems` (0x9D8 bytes) |
| `+0xC30` | `FBP_JigHelperComp_COnEquipmentUpdated OnEquipmentUpdated` (dynamic multicast delegate) — matches
  gap 3/8's hook point exactly, now with the real delegate type name for binding |
| `+0xB98` | `FGameplayTag ActiveWeapon` |

### `BP_SurroundeadGameState_C` — fully confirmed, matches every prior live-dump finding

`ABP_SurroundeadGameState_C : public AGameStateBase`, size `0x348`. `AllUIDs` at `+0x338` is
`TArray<int32>`, exactly as found in Session 19 — this is a separate, coarser tracking array from
`BP_JigMultiplayer_C`'s per-item `FGuid` system, not a duplicate of it. What specifically populates
`AllUIDs` (vs. the FGuid system) is still an open question, but is now a much narrower one.

---

## Session 27: 2026-08-07 (continued) — BP_JigMultiplayer_C Replication Path Is Live, Not Dormant

**Goal**: Session 26 left open whether `BP_JigMultiplayer_C`'s `SERVER_*`/`CLIENT_*`/`MC_*` functions are
actually used during normal play or dormant scaffolding. Tested directly with a live Lua hook (UE4SS
`RegisterHook`) rather than more static analysis.

**Method**: a disposable diagnostic mod (`Mods/JigMPHookTest`) hooked 9 candidate functions across
`BP_JigMultiplayer_C` and `BP_JigHelperComp_C`, each just logging a marker on invocation. First attempt
registered at mod-init time and **all 9 failed** — Blueprint classes don't exist in memory until
actually loaded/spawned, so hooking by path at main-menu time can't resolve the UFunction. Fixed by
retrying registration every 3 seconds until each succeeds (standard UE4SS pattern for hooking Blueprint
functions, worth remembering for future hook-based sessions — don't register once at load, poll).

**Result**, after dropping an item, picking one up, and equipping something in a normal single-player
session:

| Function | Fired? |
|----------|--------|
| `SERVER_RequestDropItem` | **Yes**, 3× |
| `SERVER_RequestEquipActorToContainer` | **Yes**, 3× |
| `CLIENT_EquipActorSuccess` | **Yes**, 3× |
| `CLIENT_NewItemAdded` | No |
| `MC_NewItemAdded` | No |
| `SERVER_RequestAddActorToContainer` | No |
| `CLIENT_AddActorToContainerSUCCESS` | No |
| `JigHelperComp.OnItemAdded` | No |
| `JigHelperComp.OnEquipmentUpdated` | Inconclusive (hook never confirmed registered) |

**Conclusion**: the replication scaffolding is confirmed **live**, not dormant — `SERVER_*`/`CLIENT_*`
functions genuinely fire during ordinary single-player play. In a listen-server-shaped single-player
session the "server" and "client" roles collapse onto the same local call, which is exactly why these
fire with no real networking involved. This directly de-risks Session 26's proposal of hooking these
functions from the mod instead of reinventing wire formats.

**Also learned**: ground-pickup does **not** go through `SERVER_RequestAddActorToContainer` /
`CLIENT_AddActorToContainerSUCCESS` as guessed — those are for some other add-to-container path (likely
crafting/loot-container transfer, given the naming). `BP_JigHelperComp_C.TryPickup(AActor* PickupRef,
UJSIContainer_C* TargetContainer, bool& Result)` (seen in the Session 26 header dump) is the more likely
entry point for a normal ground pickup and is the natural next thing to hook.

**Housekeeping**: `Mods/JigMPHookTest` is a throwaway diagnostic mod, not part of the SD-Online bridge —
safe to disable/remove once no longer needed for follow-up hook testing.

---

## Session 28: 2026-08-07 (continued) — Ground Pickup Entry Point Confirmed: `TryPickup`

**Method note**: UE4SS's hot-reload (`Ctrl+R` by default, `[General].HotReloadKey` in
`UE4SS-settings.ini`, needs `EnableHotReloadSystem = 1` which is already the default) reloads all Lua
mods from disk without restarting the game. Used this to iterate on the hook list from Session 27
in-session instead of relaunching each time — much faster for this kind of live-hook testing.

**Fix carried over from Session 27**: hooking a dynamic multicast delegate by its bare name (e.g.
`OnEquipmentUpdated`) fails to register. Delegates hook via their `__DelegateSignature` wrapper instead
(`OnEquipmentUpdated__DelegateSignature`) — this registered successfully where the bare name had not.

**Test**: extended the diagnostic mod with new candidates for the actual ground-pickup path
(`TryPickup`, `JigMP_OnPickupAdded`, `PickupBuildFromGround`, `JigTryAddItemSomewhere`,
`AddNewItemSomewhere`), all 8 hooks registered cleanly this time. After dropping and picking up an item:

| Function | Fired? |
|----------|--------|
| `SERVER_RequestDropItem` | Yes, 3× (reconfirms Session 27) |
| `JigHelperComp.TryPickup` | **Yes, 2×** |
| `StaticMeshPickup.JigMP_OnPickupAdded` | No |
| `StaticMeshPickup.PickupBuildFromGround` | No |
| `JigTryAddItemSomewhere` | No |
| `AddNewItemSomewhere` | No |
| `SERVER_RequestEquipActorToContainer` / `OnEquipmentUpdated` | Not exercised this pass (no equip action taken) |

**Conclusion**: `BP_JigHelperComp_C.TryPickup(AActor* PickupRef, UJSIContainer_C* TargetContainer, bool&
Result)` is the confirmed real entry point for a normal ground pickup — not any of the
`BP_JigMultiplayer_C` add-to-container functions guessed earlier. `PickupRef` is the ground actor being
picked up and `TargetContainer` is the destination; this is directly usable as the mod's hook point for
detecting pickups and their target container without needing to reverse further.

**Still open**: confirm `OnEquipmentUpdated__DelegateSignature` actually fires on a real equip action
(registered successfully this session but wasn't exercised) — straightforward follow-up with the same
mod, just needs an equip action during the test pass.

---

## Session 29: 2026-08-07 (continued) — Inventory Data Model Fully Resolved

**Goal**: move from equipment/pickup events to the inventory *data model* itself — what actually backs
a container's contents, and what closes gap 11 (`MAX_INV_SLOTS` arbitrary) and gap 1 (`itemId` type).

### `UJSIContainer_C` / `UJSI_Slot_C` are UI widgets, not the data model

Both extend `UUserWidget` (sizes `0x783` and `0x5F0`). They're the visual grid presentation — instantiated
when an inventory panel is actually open — not where authoritative state lives. That's
`BP_JigMultiplayer_C` (Session 26), specifically its `MainJigContainers: TArray<FS_ReplicatedContainerInfo>`
at `+0xA8`. This matters for the mod: read/sync from the actor component, not the transient widgets,
which likely don't even exist server-side or while the panel is closed.

### `FS_ReplicatedContainerInfo` — closes gap 11 completely

```
FGuid  ReplicationUID     // 0x00, this container's own UID
FGuid  InContainerUID     // 0x10, UID of the item that owns this (sub-)container, if nested
int32  Columns            // 0x20
int32  Rows               // 0x24
int32  ContainerIndex     // 0x28, index within MainJigContainers
TArray<FS_ContainerSlots>      ContainerSlots  // 0x30, occupancy grid
TArray<FContainerPickupsInfo>  ContainerItems  // 0x40, actual items
```
Size `0x50`. **There is no fixed slot count anywhere in this system.** Every container carries its own
runtime `Columns`/`Rows`, resizable live via `BP_JigMultiplayer_C.ServerFuncExpandContainer` /
`UJSIContainer_C.ExpandContainer`, backed by a plain dynamic `TArray` of actual items — not a fixed-size
slot array. `protocol.hpp`'s `MAX_INV_SLOTS = 40` should be removed outright, not replaced with a better
constant — the wire format needs a variable-length item list per container plus that container's current
`Columns`/`Rows`, mirroring this struct.

### `FContainerPickupsInfo` — per-item placement wrapper

```
FGuid   UniqueServerID     // 0x00 — the per-instance item ID (this is what "itemId" should really be
                           //        keyed on for identity, distinct from item *type* below)
bool    IsContainer        // 0x10 — true if this item is itself a container (nested backpacks etc.)
FVector2D ContainerDimension // 0x18
FRepItemInfo ItemInfo      // 0x28 (0x78 bytes, see below)
FGuid   ContainerMotherID  // 0xA0
int32   SlotIndex          // 0xB0
bool    Rotated            // 0xB4
int32   InContainerIndex   // 0xB8
AActor* PickupRef          // 0xC0 — set when this item exists as a ground actor
TArray<FS_SubContainerInfo> SubContainers // 0xC8 — recursive nesting (container-within-container)
```
Size `0xD8`.

### `FRepItemInfo` — closes gap 1, and reveals more than expected

```
UJigsawItem_DataAsset_C* ItemID   // 0x00 — raw pointer to the item-type DataAsset (the DA_* asset)
int32     Count                   // 0x08
FVector2D ItemVec                 // 0x10 (grid footprint)
double    Weight                  // 0x20
double    Price                   // 0x28
FVector2D Durability               // 0x30 (current, max)
TArray<FS_ItemStat> Stats         // 0x40
double    Pending?                // 0x50
TArray<FString> CustomDataKey     // 0x58
TArray<FString> CustomDataValue   // 0x68 (parallel array to CustomDataKey)
```
Size `0x78`.

**Gap 1 resolution**: `ItemID` is a native pointer to the `DA_*` DataAsset, not a string or an int at the
engine level — but a raw pointer can't cross the network, and since every client has the same static
content installed, the DataAsset's own name/path (`"DA_AK74"` etc.) is exactly the right wire
representation, resolvable back to the local pointer on each end via `StaticFindObject`. This confirms
gap 1's original recommendation (`itemId` → `std::string`) was correct, not merely a guess.

**Beyond gap 1**: the actual per-item model the game maintains is far richer than a bare `itemId` +
`quantity` — weight, price, durability (current/max), an arbitrary stat array, and free-form
`CustomData` key/value pairs (used for e.g. weapon attachment state, per the `BP_JigMultiplayer_C`
function names referencing custom data). None of this is in the current protocol; whether it needs to
be synced is a design question, not a research one, but it's now fully visible if/when it does.

---

## Session 30: 2026-08-07 (continued) — Full Equipment Slot Layout

**Goal**: finish gap 3/4/8 (equipment sync) — get the complete, named slot layout for
`FS_ServerEquippedItems` rather than the four slots previously spot-checked.

`FS_ServerEquippedItems` (`BP_JigHelperComp_C + 0xF8`, size `0x9D8` — matches the Session 16 total of
2520 bytes exactly). All 21 slots, each a full `FRepItemInfo` (`0x78` bytes — not just an item ID; every
slot carries its own weight/price/durability/stats/customdata per Session 29):

| Offset | Slot |
|--------|------|
| `+0x000` | `EquippedFacewear` |
| `+0x078` | `EquippedHeadwear` |
| `+0x0F0` | `EquippedEyewear` |
| `+0x168` | `EquippedAccessory` |
| `+0x1E0` | `EquippedTorso` |
| `+0x258` | `EquippedGloves` |
| `+0x2D0` | `EquippedLegs` |
| `+0x348` | `EquippedFeet` |
| `+0x3C0` | `EquippedContainer` |
| `+0x438` | `EquippedBodyArmor` |
| `+0x4B0` | `EquippedBackpack` |
| `+0x528` | `EquippedPrimary` (confirms the existing gap-analysis offset exactly) |
| `+0x5A0` | `EquippedSecondary` |
| `+0x618` | `EquippedSidearm` |
| `+0x690` | `EquippedMelee` |
| `+0x708` | `EquippedThrowable` |
| `+0x780` | `EquippedFlashlight` |
| `+0x7F8` | `EquippedBinoculars` |
| `+0x870` | `EquippedGPS` |
| `+0x8E8` | `EquippedCompass` |
| `+0x960` | `EquippedFishingRod` |

**Practical implication**: gap 3's `RemotePlayer` equipment fields can now be built with exact offsets
for every slot, not just weapon/torso/headwear as originally scoped. Each slot's `FRepItemInfo.ItemID`
(the DA_ name) is sufficient for visual appearance sync (gap 3's original goal); the rest of each slot's
`FRepItemInfo` (durability etc.) is available if ever needed but isn't required for appearance alone.

---

## Session 31: 2026-08-07 (continued) — Real Equipment Hook Point Found; Delegate Hooking Limitation

**Goal**: close gap 3/8's hook point for real — confirm something actually catches every equipment
change, since `OnEquipmentUpdated` (gap 3/8's originally proposed hook) never fired across three
different equip actions in prior sessions despite registering without error.

### Negative finding: `RegisterHook` on a delegate's `__DelegateSignature` doesn't reliably fire

Both `OnEquipmentUpdated__DelegateSignature` and `OnActiveWeaponSlotChanged__DelegateSignature`
registered successfully (no error) but never fired, even when the function that should broadcast them
(`SetActiveWeaponSlot`) was confirmed firing via its own direct hook in the same test. **Lesson for
future hook-based sessions**: hooking a `UPROPERTY` dynamic multicast delegate by its
`__DelegateSignature` name registers cleanly but is not a reliable way to observe `Broadcast()` calls in
this UE4SS setup — hook the plain UFunction that performs the actual mutation instead, not the
notification delegate.

### `SetActiveWeaponSlot` is "switch active held weapon," not an equipment change

Confirmed by direct observation: fires when cycling between already-equipped Primary / Second-Primary /
Secondary / Melee via hotkey — i.e. which already-equipped weapon is currently held, not a change to
equipment slot contents. Distinct from actually equipping a new item.

### `SetEquippedInfoBySlot` — the real, reliable equipment hook point

`BP_JigHelperComp_C.SetEquippedInfoBySlot(FGameplayTag Slot, FRepItemInfo Info, FGuid UID, bool
SkipUID)` fired reliably on every equip action tested (both ground-equip and hotkey-driven). This is a
plain UFunction, not a delegate, and is the actual mutator that writes into `FS_ServerEquippedItems`
(Session 30). **This, not `OnEquipmentUpdated`, is the correct hook point for gap 3/8** — hook this
function, read `Slot` (which `Equipped*` field) and `Info.ItemID` (the DA_ name) directly from its
arguments, no need to poll `FS_ServerEquippedItems` afterward.

### Housekeeping: UE4SS hot-reload (`Ctrl+R`) is unsafe for this game

Observed directly: hot-reloading Lua after editing a hook script uninstalled the mods and caused a crash
on the next movement input. **Always fully relaunch the game after changing a Lua mod on this project**
rather than using hot-reload, despite it being convenient in Session 28. Diagnostic mod
`Mods/JigMPHookTest` has served its purpose across Sessions 27–31 and can be disabled in `mods.txt` now.

---

## Session 32: 2026-08-07 (continued) — PlayerController + Passive Skills: Gaps 4 and 9 Closed

**Goal**: close out gap 4 (`PlayerProgress` missing fields) and gap 9 (`PlayerDamage` dispatch needs
`HUD.Widget` offset) with the CXX dump rather than the live-property-dump-derived offsets currently in
the gap analysis.

### `ABP_PlayerController_C` — every previously-documented offset confirmed, one corrected

`ABP_PlayerController_C : public ABP_MasterPlayerController_C`, size `0x990`. All of gap 4's offsets
check out exactly: `LevellingComponent+0x868`, `PassiveSkillsComponent+0x878`, `Forename+0x8C8`,
`Surname+0x8D8`, `ZombieKills+0x90C`, `DaysSurvived+0x91C`.

**Correction**: `RespawnLoc` at `+0x930` is a full **`FTransform`** (`0x60` bytes — rotation quat +
translation + scale), not a bare `FVector3d` as gap 4 assumed. Respawn restores facing direction too,
not just position.

**Resolves gap 9**: `Widget` (`UWidgetComponent*`) is at `+0x880`, exactly where gap 9 expected `HUD.Widget`.

**New fields beyond gap 4's scope** — a richer stats model than previously assumed:

| Offset | Field |
|--------|-------|
| `+0x8E8` | `FString Sex` |
| `+0x8F8` | `FString Age` |
| `+0x908` | `TEnumAsByte<Enum_Occupation::Type> Occupation` |
| `+0x910` | `int32 BossZombieKills` |
| `+0x914` | `int32 AnimalKills` |
| `+0x918` | `int32 HumanKills` |
| `+0x920` | `double DistanceTravelled` |
| `+0x928` | `int32 InfestationsDestroyed` |
| `+0x92C` | `bool RespawnPointEnabled` |

### `UPassiveSkillsComponent_C` — all 10 skills confirmed, full layout

Ten skills (Fitness, Strength, Toughness, Sneaking, FirstAid, Marksmanship, Reloading, Thief, Fishing,
Scavenging), each following the same pattern: `CurrentXP`/`MaxXP`/`CurrentLevel`/`MaxLevel` (4 doubles,
32 bytes), starting at `+0xC0` and running sequentially (Thief and Fishing land later at `+0x220`/`+0x250`
respectively, everything else contiguous from `+0xC0`–`+0x198`). Also present but not previously
documented: per-skill `Percentage` and `Multiplier` fields, and bool `*MultiplierUsed?` flags. Full
offset table in `CXXHeaderDump/PassiveSkillsComponent.hpp` (committed) rather than reproduced here.

**Practical implication**: gap 4's `read_local_progress()` can now include the full character-stats and
passive-skill picture with confirmed offsets throughout, not just the four vitals from Session 25.

---

## Session 33: 2026-08-07 (continued) — Gap 12 Closed: `AllUIDs` Is Not a World-Item Registry

**Goal**: find what populates `BP_SurroundeadGameState_C.AllUIDs` (`TArray<int32>`, `+0x338`), left open
since Session 19 first found it and gap 12 assumed it was "the authoritative list of all spawned item
unique IDs in the world" needing a `uint64 entityId` mapping.

**Method**: live test rather than more static analysis. A diagnostic mod polled `AllUIDs`'s length every
2 seconds and also logged it on every confirmed drop (`SERVER_RequestDropItem`) and pickup (`TryPickup`)
event.

**Result**: `AllUIDs` stayed at exactly **0** throughout — across 3 confirmed drops and 3 confirmed
pickups, it never changed. Normal item drop/pickup gameplay does not touch this array at all.

**Reframing, not just a narrower gap**: looking at `BP_SurroundeadGameState_C`'s full field/function list
(Session 26) with this result in hand, `AllUIDs` sits alongside `FirstCaptureDone`, `ItemsQueue`,
`SnapDelay`, `AllInspectedIDs`, and functions named `SpawnSnapshotCaptor`, `UpdateSnapCustom`,
`AddItemToQueue`, `HandleQueue`, `HandleSnapTaken`, `OnSnapTaken` — this entire region of the class is the
**item icon/thumbnail snapshot-capture system** (rendering a 3D preview image for inventory icons), not a
world-item spawn registry. `AllUIDs` is almost certainly internal bookkeeping for that system (e.g.
which numeric IDs already have a captured snapshot cached) — unrelated to tracking spawned world items
for multiplayer purposes. The original assumption (an early, pre-deep-dive session) that this was "the
deduplication registry the server needs" was a plausible-sounding guess from the name alone that this
result overturns.

**Practical implication for SD-Online**: gap 12 (map `int32` `AllUIDs` entries to `uint64 entityId`) can
be **dropped entirely** — there's nothing there to map. The correct per-instance world-item identity,
per Session 29, is `FContainerPickupsInfo.UniqueServerID` (`FGuid`, 16 bytes) — that's the real thing to
track for loot dedup/sync, not `AllUIDs`.

