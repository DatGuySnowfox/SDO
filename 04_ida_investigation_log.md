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
