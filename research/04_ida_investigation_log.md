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

---

## Session 34: 2026-08-07 (continued) — GMalloc/FMemory::Free Traced for Safe FName::ToString Cleanup

**Goal**: close the remaining half of gap 3/8 (equipment send path) — read the local player's 21
equipment slots and resolve each `FRepItemInfo.ItemID` (a `UJigsawItem_DataAsset_C*`) to its `ItemId`
`FName` as a string, using `FName::ToString` (`0x140C9D940`, Session 9).

**Note on method**: this session is static analysis only — no debugger was attached to a running game
(`ida_dbg.is_debugger_on()` returned `False`, `idaapi.get_process_state()` returned `0`/no process). Every
address and offset below is IDA-derived and internally consistent, but **not yet confirmed against a
live process**. This is also the first fix in the codebase that calls a raw native function by address
rather than a UE4SS stub method (`K2_*`) or a plain pointer-offset read — flagged for live verification
before being trusted.

### The missing piece: `FName::ToString`'s output must be freed, or every call leaks

`FName::ToString(FName*, FString* out)` writes a `TArray<TCHAR>`-shaped `FString` (`{TCHAR* Data; int32
Num; int32 Max}`) into `out`, allocated via the engine's allocator — not the CRT heap. Decompiling the
allocation path confirms this and reveals the two addresses actually needed to use this safely from C++:

```
FName::ToString (0x140C9D940)
  -> sub_140B2C110 / sub_140B20500   (FString reserve / construct-from-buffer)
    -> sub_140B2D320                  (TArray::ResizeAllocation)
      -> sub_140B2C4D0                 (thin wrapper)
        -> sub_140B7D300                (FMemory::Realloc)
          -> qword_146E1DF70 vtable+0x38   ; GMalloc->Realloc(...)
```

- **`GMalloc`**: global `FMalloc*` at `0x146E1DF70`.
- **`FMemory::Realloc`**: `0x140B7D300` — `(*(*GMalloc)+0x38)(GMalloc, ptr, newSize, alignment)`.
- **`FMemory::Free`**: `0x140B27000` — one level of indirection, `(*(*GMalloc)+0x48)(GMalloc, ptr)`. This
  is the one that matters for the C++ DLL: call it on `FString.Data` after copying the chars out, or the
  buffer leaks on every equipment poll.

**Fix applied**: `native::fname_to_string()` in `mod.cpp` calls `FName::ToString`, converts the returned
UTF-16 buffer to UTF-8 via `WideCharToMultiByte`, then frees the buffer via `FMemory::Free`. Both
addresses are rebased at runtime against `GetModuleHandle(nullptr)` relative to this session's IDA
imagebase (`0x140000000`) rather than called as fixed addresses, since nothing guarantees the game
loads at that address at runtime.

**Live test (same session, later)**: deployed `main.dll` and ran it in-game with a temporary per-slot
diagnostic log. All three open questions above resolved cleanly:

- Resolved real item names correctly across ~19–20 occupied slots per poll: `AK15`, `BenelliM4`,
  `BattleReadyGlock`, `MilitaryTacticalHelmet`, `MilitaryPlateCarrier`, `LargeCoyoteBackpack`,
  `TacticalHatchet`, `HighPoweredFlashlight`, `MilitaryGradeBinoculars`, `GPS`, `Compass`,
  `FishingRod`, etc. Note these are plain names, not the `DA_`-prefixed form used in earlier
  hypothetical examples (gap 1/2's `"DA_AK74"`) — the actual `ItemId` FName has no `DA_` prefix.
- Watched slot 11 (`EquippedPrimary`) appear/disappear/reappear across consecutive polls while
  manually equipping and unequipping a primary weapon in-game — confirms both the slot-offset table
  and the "only emit occupied slots, full-replace on receive" design (gap 8) behave correctly.
- Ran continuously at the 2 s poll cadence for the test session with no crash and stable memory
  (checked via `tasklist` before/after) — confirms the `GMalloc` vtable-slot math for `Realloc`
  (`+0x38`) and `Free` (`+0x48`) is correct, not landing on adjacent vtable slots, and that
  `FString::Num` does include the null terminator on this build (the `--len` adjustment in
  `fname_to_string` is correct, not a source of off-by-one garbage).
- No crash dumps were produced after deploying, ruling out heap corruption from the `FMemory::Free`
  call.

**Gap 3/8 (equipment send path) is now closed** for the local-read direction. Still open: proxy-side
appearance rendering (blocked on `ProxyManager` Phase 2 / real proxy actor spawning), and switching
from 2 s polling to the `SetEquippedInfoBySlot` hook if push-based updates are ever wanted.

---

## Session 35: 2026-08-07 (continued) — Real Main Inventory Read, Live-Confirmed

**Goal**: read the player's actual backpack/container contents into `PlayerProgress.slots`, which had
been a comment ("Phase 2 – empty for now") since `send_profile_revision()` was written. Uses
`BP_JigMultiplayer_C.MainJigContainers` (Session 26/29) rather than reinventing a format.

### Correctness fix found first: `BP_JigHelperComp`/`BP_JigMultiplayer` should be read off the pawn, not `FindFirstOf`

`BP_PlayerCharacter_C` (`research/CXXHeaderDump/BP_PlayerCharacter.hpp`) has both components as named,
fixed-offset properties directly on the pawn: `BP_JigHelperComp` at `+0x700`, `BP_JigMultiplayer` at
`+0x818`. `read_local_equipment()` (Session 34) had been using
`FindFirstOf("BP_JigHelperComp_C")` instead, which only happened to return the right instance because
solo testing has exactly one player in the world — `BP_JigMultiplayer_C` in particular exists on
*every ground pickup as well as the player* (Session 26), so `FindFirstOf` on it would be a real bug in
actual multiplayer (could silently read some other player's or a ground item's component). Fixed
`read_local_equipment()` to use `pawn+0x700` and wrote `read_local_inventory()` to use `pawn+0x818`
from the start.

### `read_local_inventory()`: walk `MainJigContainers`, flatten into the existing flat-slot wire format

Implementation reads `TArray<FS_ReplicatedContainerInfo> MainJigContainers` (`jigMp+0xA8`), and for each
container reads `TArray<FContainerPickupsInfo> ContainerItems` (`container+0x40`), resolving each
item's `FRepItemInfo.ItemID` (`itemInfo+0x00`, a `UJigsawItem_DataAsset_C*`) via the same
`native::fname_to_string(itemDA+0x30)` from Session 34. Deliberately reuses the existing
`PlayerProgress.slots` flat-slotIndex format (no protocol/JS changes) rather than designing a new
container-aware wire format — full gap 11 closure is out of scope for this pass since it needs a
matching change in `server/src/lib/protocol.js`/`host-agent.js`, not just the C++ mod.

### Live test 1: revealed `MainJigContainers` reserves 21 placeholder entries before real containers

First live pass logged real data immediately (`Rags qty=2`) but also produced an `items=40` snapshot
whose first 20 entries were an exact duplicate of the equipped-gear list from the Session 34 equipment
poll (`AK15`, `MilitaryTacticalHelmet`, etc.). Root cause, confirmed by adding per-container
`Columns`/`Rows` logging: `MainJigContainers` index 0–20 are placeholder entries — `Columns`/`Rows` =
**-1** (not 0 or 1), `ContainerItems` empty or near-empty — one per equipment slot, sitting ahead of the
real storage containers in the array. (Original hypothesis before adding the diagnostic was "each
equipped item is its own 1×1 container" — wrong in the specifics, but the fix ended up the same: skip
any container with `Columns <= 1 && Rows <= 1`.)

**Fix**: skip containers where `Columns <= 1 && Rows <= 1` before reading their items. Confirmed this
threshold is correct (not merely "skip empty ones") because a later poll showed a placeholder entry with
`items=1` that was still correctly excluded by the dimension check alone.

### Live test 2: real multi-container inventory resolves correctly, and gap 11's cap is real

Second live pass (after picking up more items) showed containers 0–20 correctly skipped
(`cols=-1 rows=-1`, one now showing `items=1` — still skipped), and 17 real multi-cell containers
(`cols=2..6, rows=2..8`) starting at index 21+. All 40 resolved slots were genuine, varied inventory —
ammo of multiple calibers, `Money qty=2590`, `Level1Keycard`, `SmallMedkit`, `RadiationPills`,
`Painkillers`, etc. — with no crash and stable memory across both live passes.

**Confirms gap 11 is a real, not theoretical, limitation**: the actual inventory in this test exceeded
40 items across its real containers, so the flat `MAX_INV_SLOTS` cap silently truncated genuine
inventory. Closing this properly needs the container-aware wire format Session 29 already specified
(`FS_ReplicatedContainerInfo`'s `Columns`/`Rows` + a variable-length item list per container) plus a
matching decoder change in `server/src/lib/protocol.js` and `host-agent.js`'s `MAX_INV_SLOTS` handling
— left for a future session since it's a cross-language change, not just a C++ one.

---

## Session 36: 2026-08-07 (continued) — ProxyManager Phase 2: SpawnActor Implemented, `BP_PlayerCharacter_C` Rejected Live

**Goal**: implement real proxy-actor spawning (`proxy_manager.cpp:spawn_proxy()`, previously a stub
returning `nullptr` unconditionally) using Session 12's recommendation to spawn `BP_PlayerCharacter_C`
itself, since no dedicated proxy Blueprint exists in the game.

### Fixed a real linker bug in the vendored UE4SS stub: `UObject::GetClassPrivate()`

`vendor/ue4ss-stub/include/RC/Unreal/UObject.hpp` declares `GetClassPrivate()` as a member of `UObject`,
but its own comment shows the true DLL export is mangled against `UObjectBase`
(`?GetClassPrivate@UObjectBase@Unreal@RC@@...` vs. the `UObject`-mangled symbol the linker actually
looks for) — a real bug in the stub header, not a usage mistake; this was simply the first code in the
repo to call it. Fixed the same way `mod.cpp` already handles the two `RegisterXPreCallback` exports
that aren't in the stub at all: resolve by address via `GetProcAddress(GetModuleHandleW(L"UE4SS.dll"),
"?GetClassPrivate@UObjectBase@Unreal@RC@@QEAAAEAPEAVUClass@23@XZ")`. MSVC returns a reference
(`UClass*&`) as a pointer to the referenced storage in RAX, so the resolved function pointer type is
`UClass**(__fastcall*)(void*)`, not `UClass*(*)(void*)`.

### Live test: `BP_PlayerCharacter_C` spawn was rejected 100% of the time, no crash

Deployed and tested. `get_class_private()` resolved a non-null `UClass*` correctly (confirms the linker
fix works), but every single `world->SpawnActor(s_proxy_class, &loc, &rot)` call returned `nullptr` — no
crash, clean rejection every time.

**Real incident**: `ProxyManager::tick()` had no retry cooldown on a failed `spawn_proxy()`, so it
retried on every single call to `tick()` (many times per second via `do_game_tick()`). 848 consecutive
`SDB: proxy spawn failed` log lines and a severe in-game frame-rate drop confirmed this — the retry
storm itself was the acute problem, independent of why the spawn failed. **Fixed**: added
`RemotePlayer.lastSpawnAttemptUs` and a 5s cooldown between spawn attempts in `ProxyManager::tick()`.
This fix stands regardless of whether the underlying spawn ever succeeds — any future spawn strategy
needs this cooldown too.

**Initial hypothesis for the rejection** (falsified by the follow-up test below): Session 5's
already-documented `UWorld::SpawnActor` validation checklist includes `a2` (the class) having the
`CLASS_NotPlaceable` flag. Player-character Blueprint classes are commonly flagged this way
deliberately — meant to be spawned only through the game's own PlayerController/GameMode possession
flow, not placed directly by external code. This was never actually live-tested before this session;
Session 12's recommendation was a plausible hypothesis, not a confirmed one.

### Follow-up live test: `BP_Zombie_C` also rejected — rules out `CLASS_NotPlaceable`

Swapped the target class to `BP_Zombie_C` (found via `FindFirstOf` off a live zombie instance in the
world) to distinguish "the generic `SpawnActor` path itself doesn't work from this mod" from
"`BP_PlayerCharacter_C` specifically is blocked." Retry-storm fix from above confirmed effective at the
same time: spawn attempts were cleanly spaced ~5s apart in the log with no frame-rate impact, unlike the
first test.

**Result**: identical outcome — `get_class_private()` resolved a valid, non-null `UClass*` (confirms the
`FindFirstOf` + linker-fixed `GetClassPrivate` combination works correctly), but `world->SpawnActor(...)`
still returned `nullptr` on every call. Since the game's own zombie spawner places `BP_Zombie_C`
instances constantly, it cannot be `CLASS_NotPlaceable` — **this rules out the class-flag hypothesis
entirely**. The problem is not which class is targeted; it's something about how this mod's call into
the UE4SS `SpawnActor` wrapper itself behaves (or doesn't) — possibly an `FActorSpawnParameters`
default the 3-arg convenience wrapper sets up incorrectly for this UE4SS version, a threading/context
precondition not being met, or a bug in the wrapper itself. This needs IDA-level investigation of the
actual `UWorld::SpawnActor` export UE4SS.dll ships (address, decompile, compare against the confirmed
raw engine `sub_14300FE50` from Session 5) rather than more live trial-and-error with class names.

**Status**: `ProxyManager` remains functionally Phase 1 (no visible proxy actors). The retry-storm fix
is real, confirmed effective, and safe to leave deployed regardless of whether spawning ever works.
Left `spawn_proxy()` targeting `BP_PlayerCharacter_C` (the real desired proxy visual) since class choice
isn't the blocker. Next session should decompile UE4SS's `SpawnActor` export before trying another live
spawn — further blind live attempts are unlikely to reveal anything past this point.

---

## Session 37: 2026-08-07 (continued) — Gap 4/7 Closed: Extended PlayerController Stats, Live-Confirmed

**Goal**: wire the "at minimum" fields gap 4 asked for — `forename`/`surname`/kill-counts/`daysSurvived`
— into `PlayerProgress`, using the offsets Session 32 already confirmed off `ABP_PlayerController_C`.
`respawnLoc` and the 10 passive skills were deliberately left out of scope: `RespawnLoc` is a full
`FTransform` whose internal `FQuat`/`FVector` sub-offsets haven't been live-verified (only its total
size and starting offset are confirmed), and the passive skills are a much larger separate chunk of
data Session 32 itself flagged as a bigger follow-up.

### New read primitive: `native::read_fstring_field()` — reading an FString in place, not via `ToString`

`Forename`/`Surname` are already-live `FString` UPROPERTYs sitting directly on the PlayerController
(`ctrl+0x8C8`/`+0x8D8`), unlike `ItemId`, which is an `FName` requiring a call into
`FName::ToString` (Session 34) to materialize a string at all. Reading an in-place `FString` is
simpler and has no allocation to free — treat the address as the same `UnrealFString{data,num,max}`
layout already defined for `fname_to_string`'s output, read `Data`/`Num` directly, and convert with
`WideCharToMultiByte`. No engine call, nothing to free (the engine owns that buffer for the object's
lifetime).

### Wire format: appended a trailer rather than reworking the existing header

`PlayerProgress`'s wire format (`protocol.hpp`/`protocol.cpp`, mirrored in
`server/src/lib/protocol.js`) gained a new section appended *after* the existing slot list, rather than
inserting fields into the fixed 51-byte header. This matters because `db.saveProgress()`
(`server/src/db.js`) persists the *raw client-sent bytes* to SQLite and `db.getProgress()` replays them
verbatim on rejoin (`gateway.js`'s `JoinAccepted` handler) — any player with progress saved before this
change has an old-format blob with no trailer sitting in that database. Both `decode_player_progress`
(C++) and `decodePlayerProgress` (JS) tolerate a payload that ends right after the slots: the new fields
just stay at their `PlayerProgress` defaults rather than the decode throwing/returning `nullopt`. Also
updated `host-agent.js`'s `_applyProfileRevision` to store the decoded trailer onto `p.stats`, alongside
the existing `p.inventory` — `encodePlayerProgress` (JS) was extended too for consistency even though
nothing currently calls it (mirrors the existing gap-13 audit's concern about protocol drift between
what's defined and what's actually used).

### Live test: forename/surname resolved correctly, no crash

Deployed with a temporary diagnostic log. Result:
```
SDB: extended stats  forename=John surname=Doe zombieKills=0 daysSurvived=1 bossZombieKills=0
animalKills=0 humanKills=0 distanceTravelled=0.0 infestationsDestroyed=0
```
`forename`/`surname` resolved to sane values (not garbage or empty strings), confirming the
`UnrealFString` layout assumption holds for an in-place-read FString the same way it already did for
`FName::ToString`'s output. `daysSurvived=1` and all-zero kill counts are consistent with a freshly
started character. No crash; game remained stable through the test. Diagnostic logging removed after
confirmation — this closes gap 4/7's "at minimum" scope.

**Still open**: `respawnLoc` (needs the `FTransform` sub-layout confirmed, likely via a live
PropertyDumper pass rather than guessing) and the 10 passive skills (a separate, larger effort per
Session 32's own framing).

---

## Session 38: 2026-08-09 — SpawnActor Rejection: Full Call Chain Mapped, Root Cause Still Open

**Goal**: Session 36 found `ProxyManager::spawn_proxy()` cleanly rejected for both `BP_PlayerCharacter_C`
and `BP_Zombie_C` (no crash, clean `nullptr`), ruling out class-flag issues, and flagged this as needing
IDA-level investigation of the actual `SpawnActor` export before another live attempt was worthwhile.
This session does that investigation.

### Confirmed: the failure is universal, not specific to the C++ mod's call

Before touching IDA, isolated whether this was specific to the mod's own reflection-based invocation.
UE4SS's own Lua `SpawnActor` binding (`LuaUWorld.cpp`) calls the exact same underlying
`RC::Unreal::UWorld::SpawnActor` C++ method our mod calls — a live Lua test (`world:SpawnActor(cls,
{X=,Y=,Z=}, {Yaw=,Pitch=,Roll=})` against `BP_PlayerCharacter_C`) reproduced the identical clean
rejection. This rules out anything specific to our mod's parameter marshaling via `ProcessEvent` — both
call paths funnel into the same native code, and both fail identically.

### `UWorld::SpawnActor` (UE4SS) is not a raw engine call — it's itself reflection-based

`RC::Unreal::UWorld::SpawnActor` (`RE-UE4SS/deps/first/Unreal/src/World.cpp`) doesn't call a native
engine `SpawnActor` directly. It calls `UGameplayStatics::BeginDeferredActorSpawnFromClass` followed by
`FinishSpawningActor` — the same two-step deferred-spawn mechanism Blueprint's "Spawn Actor from Class"
node uses. Both are resolved and invoked via `StaticReflectedFunctionBase` (`ReflectedFunction.hpp`),
which does a one-time `StaticFindObject<UFunction*>` lookup by full path
(`/Script/Engine.GameplayStatics:BeginDeferredActorSpawnFromClass`) and calls it via `ProcessEvent` —
logging `"Was unable to locate '{}'"` if that lookup itself fails.

**No such message appears in `UE4SS.log` across a fresh test session** — meaning the UFunction resolves
fine; the rejection happens inside the actual native call, not in UE4SS's wiring to it.

### Full native call chain, traced via IDA/Hex-Rays

Found the real exec thunk for `BeginDeferredActorSpawnFromClass` at `0x142EB0540` (found via the
(name-string, code-ptr) pairs referencing the `"BeginDeferredActorSpawnFromClass"` string at
`0x145C1C920` — two of the three candidate code pointers were small `0x2f`-byte lazy
UFunction-object-getter trampolines, same dead-end pattern as Session 24's `K2_TeleportTo` chase; the
third, `0x24d` bytes, was the real one). It reads `WorldContextObject`, `ActorClass`, `SpawnTransform`
(with baked-in identity rotation/scale defaults matching `FTransform{FQuat(), Location, {1,1,1}}` from
the engine source), `CollisionHandlingOverride`, and `Owner` off the Blueprint stack, then calls:

```
sub_142E80E80(WorldContextObject, ActorClass, SpawnTransform, CollisionHandlingOverride, Owner, ...)
```

This is `UGameplayStatics::BeginDeferredActorSpawnFromClass`'s actual body:
1. `if (!ActorClass) return 0;` — not our issue, class resolves fine.
2. Resolves `WorldContextObject` to a `UWorld*` via `sub_1434ACF40`, confirmed to be
   `UEngine::GetWorldFromContextObject` (embedded strings `"No world was found for object (%s) passed in
   to UEngine::GetWorldFromContextObject()."` / `"A null object was passed as a world context object..."`).
   For `EGetWorldErrorMode::LogAndReturnNull` (mode `1`), this calls a **virtual function at vtable
   offset 392** (slot 49) on the passed object — almost certainly `UObject::GetWorld()` — and returns
   null (logging a warning first) if that returns null.
3. If world resolution succeeds, calls `sub_14300FE50` — the real native `UWorld::SpawnActor` — passing
   a `FActorSpawnParameters`-shaped struct built from the Owner/CollisionHandling/DeferConstruction
   fields set just before this call.

### `sub_14300FE50` (the real native SpawnActor) is confirmed stock UE5, no custom gate

Decompiled in full. It's the genuine, enormous `UWorld::SpawnActor` — actor-uniqueness-name generation,
NaN/invalid-transform checks on the spawn matrix, `CLASS_Deprecated` (`ClassFlags & 0x02000000`) and
`CLASS_Abstract` (`ClassFlags & 0x1`) rejection checks, collision-handling-mode branches, and the actual
construction/registration calls at the end. **No SurrounDead-specific validation, anti-cheat check, or
"reject non-internal spawns" gate found anywhere in this function.** `BP_PlayerCharacter_C`/`BP_Zombie_C`
are definitely not deprecated or abstract (they're the live, constantly-used gameplay classes), so these
checks should pass for both — consistent with Session 36 ruling out class-specific issues.

### Root cause still open — needs live value inspection, not just static analysis

The call chain is now fully mapped and confirmed to be entirely stock engine code, but **which specific
early-exit branch is actually firing can't be determined from static decompilation alone** — it requires
inspecting actual register/memory values at the moment of rejection (a live debugger breakpoint), which
this session didn't attempt. Compounding this: **Shipping builds strip `UE_LOG` output entirely**
(confirmed — `Saved/Logs/` exists but the game never writes to it, only `Saved/Crashes/` gets populated),
so even if `GetWorldFromContextObject`'s own warning fired for real, it would never be visible to us
through any log. Absence of that log message is not evidence either way.

**Leading candidate**: the virtual `GetWorld()` call at vtable+392 on the passed `UWorld*` itself. In
stock UE5, `UWorld::GetWorld()` trivially returns `this`, so this should never fail — but if this specific
engine build's vtable layout shifted (virtual function added/removed/reordered relative to what UE4SS's
hardcoded offset assumes for this UE version), the call could land on the wrong function entirely and
return garbage/null. Weighs against this: other Blueprint-library calls already work reliably in this
mod (`K2_GetActorLocation`, `K2_SetActorLocationAndRotation`, `GetAllActorsOfClass`), so the general
reflection/virtual-dispatch machinery isn't fundamentally broken — though those go through `AActor`'s
own `GetWorld()` override, a different vtable slot, so this doesn't fully rule it out either.

**Practical next step**: this needs either (a) a live IDA debugger session attached to the running game
with a breakpoint at `0x142EB0540` or `0x1434ACF40` to read actual parameter/return values, or (b) a
cheaper bypass — investigate whether the game's own internal spawners (used constantly for zombies and
loot) call a more specific, simpler native function we could hook/call directly instead of going through
the generic Blueprint reflection path at all.

### Session 38 addendum — live debugger attempt: real hazards found, no conclusive answer yet

Attempted (a) above: attached IDA's debugger to the running game via the GUI (Debugger → Attach to
process), jumped to `0x1434ACF40` and `0x142EB0540` post-attach, set breakpoints, resumed, then drove the
rest programmatically via IDA's Python execution (`ida_dbg` module) rather than the GUI.

**Real finding — address translation is not straightforward post-attach**: `idaapi.get_imagebase()`
returns the live runtime base once a debugger is attached (`0x7ff71b960000` here, vs. the static
analysis base `0x140000000`), but naively translating `static_addr - static_base + live_base` and
checking the result against `idc.get_func_name()`/segment info gave nonsense (an empty function name, a
segment named `debug10107`) — though this may simply be because `auto_analysis_ready` was `false` for
the whole attached session (confirmed via `server_health`), meaning symbol/function lookups are
unreliable while live regardless of whether the address itself is correct. Genuinely unresolved whether
the two breakpoints that were set (via jump-to-address *after* attaching) actually landed on the intended
functions at all.

**Real hazard — a live text search hung the whole MCP connection**: calling `ida_search.find_text()` to
empirically verify the address translation (searching for the `"BeginDeferredActorSpawnFromClass"`
string while attached) caused IDA to stop responding entirely — not just that call, but all subsequent
MCP calls including a bare health check — for an extended period, with the game process left suspended
at a breakpoint (frozen) for the duration. It recovered on its own after enough time passed. **Avoid
combining live search operations with an active attached debug session** until this is better understood;
prefer read-only register/state queries only while attached.

**Cleaned up**: removed both breakpoints and detached the debugger via `ida_dbg.detach_process()` once
IDA recovered, confirmed the game resumed normally.

**Lesson for the next attempt**: set breakpoints via jump-to-address *before* attaching (while IDA is
still in its normal static/idle view with well-defined addressing), then attach afterward — IDA is
designed to carry statically-set breakpoints through into a live session correctly, which should avoid
the post-attach addressing ambiguity hit here. This session's attempt did it in the opposite order
(attach first, then jump-to-address), which is the likely root cause of the confusion.

---

## Session 40: 2026-08-09 — SpawnActor Root Cause Found and Fixed

**Goal**: retry the live debugger approach from Session 38's addendum with corrected methodology
(breakpoints set before attach), to finally determine why `spawn_proxy()` is rejected.

### False start: the earlier "engine spawn succeeds" trace was watching the wrong call

Following the corrected order (breakpoints on `0x142EB0540`/`0x1434ACF40` set while static, attach
afterward), the fix worked exactly as expected: address translation across attach was clean, and a full
trace confirmed `GetWorldFromContextObject` resolves a valid `UWorld*`, the real native `SpawnActor`
(`sub_14300FE50`) is called and returns a non-null actor with a class pointer that exactly matches the
requested `UClass*`, and `BeginDeferredActorSpawnFromClass` itself returns that same non-null actor to
its caller. Every check along this path passes.

This looked like a full resolution — until re-arming the same breakpoints and confirming the mod's
`SDB.log` was actively logging `proxy spawn failed` every 5 seconds (the `ProxyManager::tick()` retry
cadence) at the same time. None of the breakpoints fired. Not even the well-established
`BeginDeferredActorSpawnFromClass` exec thunk, despite dozens of confirmed real retries happening in the
same window. The conclusion: the earlier "successful" trace was never watching this mod's call at all —
it was almost certainly a coincidental, unrelated spawn of a `BP_PlayerCharacter_C` instance (most likely
the local player's own pawn loading in), which happens to share the exact same `UClass*` pointer as the
proxy target and is indistinguishable from it by class/world checks alone. The lesson from Session 24's
`K2_TeleportTo` chase applies again here in a new form: confirming a breakpoint *can* fire on the
expected code path is not the same as confirming *this specific call* took that path. A register/pointer
match isn't enough in a live, busy game world — needed either a unique marker or, better, tracing from
the actual call site outward instead of trusting an assumed call chain.

### The real call site: UE4SS.dll's own SpawnActor, not the game's native reflection path

`ProxyManager::spawn_proxy()` calls `world->SpawnActor(...)` — the vendored UE4SS C++ SDK wrapper
(`RC::Unreal::UWorld::SpawnActor`), an exported function inside `UE4SS.dll`, a separate module from the
game binary entirely. Found its live entry point directly and reliably by parsing `UE4SS.dll`'s own PE
export table from live process memory (DOS header → NT header → export directory → name/ordinal/function
RVA arrays, walked manually via `ida_bytes` reads while attached) and resolving the RVA for
`?SpawnActor@UWorld@Unreal@RC@@QEAAPEAVAActor@23@PEAVUClass@23@PEBUFVector@23@PEBUFRotator@23@@Z` — far
more reliable than guessing at (name-string, code-pointer) heuristics in a binary IDA has no analysis
database for.

A breakpoint at that address fired immediately and reliably on every real `spawn_proxy()` retry, with
`RCX`/`RDX` matching the actual `UWorld*`/`UClass*` in play — this is unambiguously the correct call site.
Disassembling it manually (IDA's own disasm/decompile tools fail outright on `UE4SS.dll` since it isn't
loaded into this IDB — worked around by decoding raw bytes instruction-by-instruction via
`ida_ua.decode_insn`/`insn_t` fields directly, bypassing the database-dependent `generate_disasm_line`
which silently prints undefined bytes as `db XXh` for any address IDA hasn't marked as code) showed it
calls two further UE4SS-internal, already-symbolized functions in sequence:

```
RC::Unreal::UGameplayStatics::BeginDeferredActorSpawnFromClass(WorldContextObject, ActorClass, &Transform, 0, nullptr)
  -> if non-null: RC::Unreal::UGameplayStatics::FinishSpawningActor(result, &Transform)
```

A breakpoint immediately after the `BeginDeferredActorSpawnFromClass` call confirmed `RAX = 0` on every
real attempt — UE4SS's own implementation of this function is what returns null, not anything in the
game's native code.

### Root cause: a shared, unresolved internal cache lookup inside UE4SS.dll

Manually disassembling `BeginDeferredActorSpawnFromClass`'s real body (again via raw `decode_insn`, since
no analysis database exists for this module) found two early boolean validation calls — both pass
(`AL=1`) on every real attempt, ruling out a `WorldContext`/class-validity guard. Past those, the
UE5-major-version branch (`RC::Unreal::Version::Major/Minor`, correctly read as `5`/`3` — not a
misdetection) leads to a call into a shared internal helper (RVA `0x4C29C0` within `UE4SS.dll`) that
computes an **FNV-1a hash** (recognized by the classic
`0x100000001B3`/`0xCBF29CE484222325` constants) over repacked `FTransform` bytes and probes a small
hashtable via a handful of fixed global pointers. The result slot is zero-initialized just before this
call and never overwritten — this lookup is what's silently failing.

Decompiling `UE4SS.dll`'s own `FinishSpawningActor` export (`?FinishSpawningActor@UGameplayStatics@...`,
found the same way via PE export parsing) showed an **identical structure** — same two early checks, same
UE5-version branch, same call into the exact same helper address, same zero-initialized result slot never
overwritten. Both of UE4SS's `GameplayStatics` spawn-related wrappers bottom out in the same broken
internal helper. This is a single, shared bug in UE4SS's own reflection-dispatch/type-cache mechanism for
this specific engine build — not anything specific to this mod, to either function individually, to the
`BP_PlayerCharacter_C`/`BP_Zombie_C` class choice (Session 36), or to the `GetClassPrivate()` workaround
already in `proxy_manager.cpp`. Both Lua's and this mod's identical failure (Session 38) is now fully
explained: both funnel through this exact same broken UE4SS-internal path.

### Fix: bypass UE4SS's wrapper, call the game's own native functions directly

Since UE4SS's wrapper is broken but the game's own native reflection-adjacent call chain (traced in the
false-start section above, before its trigger was known to be miscredited) is 100% functional stock UE5
code, the fix bypasses UE4SS entirely for this call, following the same by-address-resolution pattern
already established for `get_class_private()`:

- `BeginDeferredActorSpawnFromClass`'s real native body (`sub_142E80E80`, confirmed via full decompile in
  Session 38: resolves `WorldContextObject` via `UEngine::GetWorldFromContextObject`, then calls the real
  native `UWorld::SpawnActor` with `bDeferConstruction=true`) is called directly by resolved RVA.
- The real native `AActor::FinishSpawning()` equivalent was identified by searching the second candidate
  cluster of (name-string, code-pointer) pairs for `"FinishSpawningActor"` — critically, the cluster
  physically adjacent to `BeginDeferredActorSpawnFromClass`'s own thunk address (`0x142EB0xxx`–
  `0x142EB4xxx`), not the first, distant cluster tried (`0x1437Bxxx`), which under decompile turned out to
  have `NewObject`-style construction semantics inconsistent with "finish an already-existing actor" and
  was discarded. The exec thunk at `0x142EB4320` reads an Actor pointer, an `FTransform`, and a byte off
  the Kismet frame and calls `sub_142AAAB90(Actor, &Transform, 0, 0, byte)` — matched with high confidence
  to real `AActor::FinishSpawning()` by its signature: a one-time guard flag at `this+92`
  (`bHasFinishedSpawning`-equivalent, checked-then-set exactly once) followed by a full parent-attachment-
  aware transform composition before applying it. The calling thunk returns the *original* Actor pointer
  read from the frame regardless of this call's own return value — matching real
  `UGameplayStatics::FinishSpawningActor`'s actual UE5 source (`Actor->FinishSpawning(...); return Actor;`,
  with `FinishSpawning` itself `void`).
- `proxy_manager.cpp` now builds the native 96-byte UE5 LWC `FTransform` layout by hand (three
  SIMD-aligned 32-byte blocks — Rotation quat XYZW, Translation XYZ+pad, Scale3D XYZ+pad, all doubles;
  confirmed via the constant-xmmword identity-transform pattern both exec thunks build for unconnected
  Blueprint pins) and calls both resolved functions directly instead of `world->SpawnActor(...)`.

**Live-tested end to end and confirmed working**: `spawn_proxy()` now returns a real, non-null actor;
`FinishSpawning`'s transform-composition and component-registration side effects run without exception in
either `SDB.log` or `UE4SS.log`; and the synthetic test proxy actually renders as a visible character
model in-game, 300 units from the local player, exactly as intended. No crash across the full change. This
closes the biggest remaining gap toward the mod being visually functional in multiplayer. The Session 36
TEMP test hook in `mod.cpp` (`kTestProxyId = 0xDEADBEEF`) has been removed now that spawning is confirmed
live — future testing should exercise this through a second real client connection instead.

**Unrelated infrastructure note**: this session's live testing was repeatedly blocked by the mod's join
ticket expiring/being replayed (`[tcp] authentication rejected` loops) — tickets are short-lived
(`scripts/play.ps1`, 2-minute TTL) and single-use server-side, so a fresh `play.ps1` run is needed
immediately before each in-game test, not just once per session. Also found and fixed a real bug in
`play.ps1` itself: it checked a `$resp.ok` field that the actual ticket-issuing endpoint never returns,
which would have caused it to always report failure even on a successful ticket fetch.

---

## Session 41: 2026-08-10 — Appearance Sync: Real Signature Found, Blocked on GameplayTag Discovery

**Goal**: now that `spawn_proxy()` produces a real, visible actor (Session 40), apply cached
`RemotePlayer.equipment` data to it so proxies actually look equipped instead of bare. Also closed gap 11
in this same session (see the gap analysis doc, `06_protocol_gap_analysis.md`) — the two are unrelated,
gap 11 is a pure wire-format change with no live dependency and is fully resolved separately.

### Real signature confirmed, but two live unknowns remain

`BP_JigHelperComp_C::SetEquippedInfoBySlot`'s actual signature, already captured in an earlier session's
`CXXHeaderDump/BP_JigHelperComp.hpp` (line 64) but never acted on:

```
void SetEquippedInfoBySlot(FGameplayTag Slot, FRepItemInfo Info, FGuid UID, bool SkipUID);
```

Applying equipment to a proxy needs two things this doesn't have yet: (1) the actual `FGameplayTag`
string value for each of the 21 equipment slots (the wire protocol's slot 0–20 order is a plain
positional convention over `S_ServerEquippedItems`'s named struct fields — `EquippedFacewear`,
`EquippedHeadwear`, etc., Session 16 — which is *not* the same thing as the `FGameplayTag` values this
Blueprint function actually switches on internally), and (2) the correct Kismet parameter-buffer layout
to call it via `ProcessEvent`, since it's Blueprint-authored and not in the vendored SDK's simpler
by-address wrapper methods.

### Static string search is a dead end for Blueprint tag/field names

Tried finding tag or field name strings directly in the binary via `find_regex` (after confirming the
strings cache needed `server_warmup(build_caches=true)` first — it silently returns zero matches
otherwise, with no indication the cache itself was the problem). Searched for `Facewear`, `Headwear`,
`Backpack`, `Equipped` — all zero matches, even the literal struct field names already known from
Session 16's PropertyDumper output. Confirms (again) that Blueprint-defined reflection metadata isn't
present as plain strings in a Shipping binary in a form IDA's string scanner recognizes — this needs live
runtime introspection, not static analysis, same conclusion as the project's established methodology for
all Blueprint-side gaps.

### UE4SS Lua's `GetPropertyValue` doesn't support `MapProperty` in this version

Wrote a diagnostic Lua script (`JigMPHookTest`, temporarily re-enabled in `mods.txt`) to dump
`EquipmentIDSlotConfig` and `EquipmentUIDs` (both `TMap<FGameplayTag, ...>` — see the field table in this
log's BP_JigHelperComp_C section) via `helper:GetPropertyValue(name)`. Both throw immediately (`pcall`
catches an opaque `function: 0x...` error object, not a string — UE4SS's Lua binding error objects
aren't descriptive strings in this build). `helper:GetPropertyValue("ActiveWeapon")` (a plain
`FGameplayTag` `StructProperty`, not a map) partially worked — returned a `userdata` whose `tostring()` is
`UScriptStruct: 0x...` — but this looks like the property's *type descriptor* object, not an instance
wrapper with usable data: both `:ToString()` and `:GetTagName()` calls on it failed the same opaque-error
way. `helper:GetFunction("SetEquippedInfoBySlot")` also failed identically — likely the wrong method name
for this UE4SS Lua API surface entirely, not confirmed correct and then failing for a different reason.

**None of these three failures were narrowed down further this session** — worth checking actual UE4SS
Lua API documentation/source directly next time rather than guessing method names from general
Unreal-modding conventions, since nothing tried here got far enough to even confirm which specific method
call format this UE4SS build's Lua bindings expect for struct/map property access.

### Raw `TMap` byte read attempted, not trusted enough to build on

With the game still running and the helper component's raw address known from the Lua diagnostic's own
`tostring()` output, attached IDA and read `EquipmentIDSlotConfig`'s raw 0x50-byte header
(`helper+0xAF8`) directly. Got plausible-looking pointers and small integers back, but manually parsing
UE5's `TSet`/`TSparseArray`/`FScriptMap` internal layout from raw bytes without any known-good reference
point to validate against is a real risk of silently building on a wrong interpretation — this is
`FMemory`-adjacent low-level container internals, not simple reflection metadata, and it's known to be
sensitive to exact engine version/build config in ways this project hasn't needed to touch before.
Deliberately stopped here rather than guessing further and shipping code built on an unverified struct
layout.

### Next steps identified, not yet attempted

- Resolve `ActiveWeapon`'s `FName` directly via the same technique `mod.cpp`'s `native::fname_to_string()`
  already uses for item IDs (`FName::ToString` at `0x140C9D940`, called on a raw pointer to the 8-byte
  `FName` inside the `FGameplayTag`) — `ActiveWeapon` is a single plain `FGameplayTag` at
  `helper+0xB98`, no `TMap` involved, so this should be low-risk and would give at least one *confirmed
  real* tag string to anchor a naming-convention guess for the other 20.
- Once at least one real tag string is known, try candidate tag strings for the other slots against the
  **read-only** `GetEquippedInfoBySlot` getter via Lua (`(FGameplayTag Slot, FRepItemInfo& Info, bool&
  Equipped)`) — side-effect-free, so wrong guesses cost nothing, and a slot known to be currently equipped
  in a live test session can validate a candidate tag by checking `Equipped` comes back `true`.
- Only after all 21 tags are confirmed real (not guessed) does building the `SetEquippedInfoBySlot`
  `ProcessEvent` call (marshaling `FGameplayTag`/`FRepItemInfo`/`FGuid`/`bool` into a Kismet parameter
  buffer) become worth attempting — this is itself unverified and carries real crash risk if the buffer
  layout is wrong, so it needs its own careful live verification pass, not a blind first attempt.

Paused here for the session — `JigMPHookTest` reverted to disabled in `mods.txt`, its diagnostic script
left in place (disposable, repeatedly repurposed across sessions per established convention) for whoever
picks this up next.

---

## Session 42: 2026-08-10 — Proxy Death Was Killing the Real Player; Fixed and Live-Verified With a Genuine Second Client

### Found: killing a spawned proxy triggered the real player's own death sequence

Live-tested with the Session 40 proxy spawn: attacking and killing a spawned proxy actor triggered the
local player's own death handling — their own loot crate spawned, their own death-location marker
appeared, and their own pawn froze in place (still standing, not ragdolled) — while the proxy itself was
what had actually taken the fatal damage. `find_local_pawn()` (`mod.cpp`) was checked and ruled out as the
cause: its primary path resolves the pawn via the local `PlayerController`'s own `AcknowledgedPawn`/`Pawn`
property, which is unaffected by a second, unpossessed `BP_PlayerCharacter_C` instance's health. No
`DeathRequest`/`RespawnRequest` activity appeared in `SDB.log` when this was reproduced, confirming it
wasn't this mod's own death-detection misfiring — the game's own native death-handling logic itself is
what's cross-wired.

Root cause (inferred, not fully traced): `BP_PlayerCharacter_C`'s own on-death Blueprint logic —
loot-crate spawn, death-location UI, input lock — appears to be keyed off *any* instance's health reaching
zero, not off which instance is actually possessed by the local `PlayerController`. The Blueprint was
never designed to have a second, locally-spawned instance of itself coexisting in the world at all, so
this is a design assumption baked into the class, not a native engine bug and not something fixable by
patching `find_local_pawn()` or similar mod-side logic.

### Fix: disable the proxy's collision entirely

The proxy is purely cosmetic — it never needs to take damage. `proxy_manager.cpp`'s `spawn_proxy()` now
calls `SetActorEnableCollision(false)` on the freshly spawned+finished actor, preventing any damage trace
from ever landing on it in the first place. This sidesteps the cross-wired death logic entirely rather
than trying to patch it, and is a one-line, low-risk, easily-reversible change.

### Live-verified with a genuine second client — real two-machine multiplayer test infrastructure now exists

Previous sessions' live testing all used a single client with a synthetic fake remote player
(`kTestProxyId`, removed in Session 40) or attempted (and failed) two-client tests via Sandboxie on one
PC. This session set up a **real second client on a separate Proxmox VM** and used it for genuine
two-machine verification — infrastructure worth keeping for future multiplayer-specific testing:

- SSH access configured to both the Windows VM guest and the Proxmox host itself (key-based, matching the
  existing `ironclaw` pattern — see `~/.ssh/config` entries `sdo-client2` and `sdo-proxmox`).
- UE4SS + the mod's `Mods/` folder copied from the working host installation to the VM's copy of the game
  (no game-side reinstall needed, UE4SS drops in as a normal file copy).
- Local dev server (`server/`) fetches tickets directly via its own `/v1/tickets` endpoint
  (no `adminToken` configured locally, so no auth needed) rather than going through `play.ps1`'s
  remote-only `/v1/join` directory-service assumption — `session.cfg` written directly with
  `SDB_GATEWAY_HOST` set to the *host's* LAN IP (not `127.0.0.1`) so the VM can actually reach it.
- Hit and fixed three real environment issues along the way, all now resolved for future sessions:
  1. **NVIDIA Code 43** on the passed-through GPU — the VM's `cpu:` line in
     `/etc/pve/qemu-server/102.conf` had no hypervisor-hiding flag, and NVIDIA's consumer driver actively
     disables itself when it detects it's running in a VM. Fixed by adding `hidden=1,flags=+pcid` to the
     `cpu:` line (Proxmox-native equivalent of the common `-cpu host,kvm=off` trick, without resorting to
     raw `args:` overrides).
  2. **GPU still Code 43 after that** — the config only passed through the GPU's VGA function
     (`0000:01:00.0`) and not its paired HD-audio function (`0000:01:00.1`, confirmed present and already
     bound to `vfio-pci` via `lspci -nnk` on the Proxmox host). Fixed by changing `hostpci0` to
     `0000:01:00,pcie=1` (dropping the explicit `.0` tells Proxmox to pass through *all* functions of that
     PCI device together as one multi-function unit).
  3. **"WGL: The driver does not appear to support OpenGL"** blocking the game from opening at all, after
     the GPU itself showed `Status: OK` in Device Manager — the NVIDIA driver had only partially
     initialized while stuck in the Code 43 state; a full silent reinstall
     (`<installer>.exe -s -clean -noreboot`) followed by an in-guest restart fixed it (confirmed via
     `Win32_VideoController.DriverVersion` going from blank to `32.0.16.1088`). Separately, disabled
     UE4SS's `GuiConsoleEnabled`/`GuiConsoleVisible` in `UE4SS-settings.ini` — its debug console window
     uses GLFW+OpenGL independently of the game's own DirectX rendering and was the specific thing
     throwing the WGL error; the plain (non-GUI) `ConsoleEnabled` console doesn't need this and was left on.

### Separate finding: two players creating characters concurrently causes one to fall through the world

While setting up the second client, a `New Game`-flow character consistently fell through the world
immediately on spawn — reproduced identically on both the earlier Sandboxie attempt and this VM, at
multiple different coordinates (including a coordinate directly copied from a different, healthy player's
own valid saved position, ruling out "bad coordinates" as the cause). Isolated by testing with only one
client's character-creation flow running at a time: **a brand-new character spawns and stabilizes
correctly when created in isolation**, but reliably falls through indefinitely when a *second* player's
character creation happens concurrently with (or close in time to) another. This points at a race
condition in shared, insufficiently-scoped server-side or mod-side state touched during character
creation/first spawn — not an environment issue (it reproduced identically across two completely
different environments: Sandboxie and a separate VM), and not related to the proxy/collision work above.
**Not investigated further this session** — worth a dedicated look at `host-agent.js`'s and `mod.cpp`'s
character-creation/first-spawn paths for anything keyed by a shared cursor, singleton, or un-keyed static
rather than being properly scoped per-player.

**Live-verified end to end**: with both a local-server-connected native client and the Proxmox VM client
in the same session, each saw a real, visible proxy for the other. Attacking the VM client's view of the
native client's proxy landed **zero damage** (collision fix confirmed working), and the native client
side showed **no effect whatsoever** — no health change, no hit reaction, nothing. The death-cascade bug
this session opened with is fully closed.

### Concurrent character-creation race — one real bug found and fixed, one likely cause identified but unfixed

Follow-up investigation (code review only, no live game access that session — see below): found a genuine,
concrete bug in the character-creation IPC mechanism. `mod.cpp`'s `CC_REQUEST_FLAG`/`CC_DONE_FILE` (the
flag/done files the C++ mod and the Lua-side `mods/SDOnline/Scripts/main.lua` character-creation UI use to
hand off to each other) were hardcoded to fixed paths — `C:\temp\SDB_cc_request.flag` /
`C:\temp\SDB_cc_done.json` — with no per-instance scoping at all. Any two mod instances on a machine
sharing that filesystem path (confirmed true for the two Sandboxie boxes used earlier this session, which
share `%APPDATA%` and, it turns out, presumably `C:\temp` too) would read and write the exact same files
during character creation, racing on both the request flag and the done-JSON payload/deletion.

**Fixed**: both files are now suffixed with the current process's PID, computed once in `mod.cpp`'s new
`init_cc_ipc_paths()` (called from `on_unreal_init()`) and published via the `SDB_CC_PID` environment
variable so `main.lua` — running in the same OS process — can independently build the matching suffixed
paths (env vars set in a process are visible to everything running inside it, C++ and embedded Lua alike).
Lua-side lookup is wrapped in `pcall` with a fallback to the old unsuffixed names and a warning print, in
case `os.getenv` isn't available in this UE4SS Lua build. **Not live-verified this session** — the local
game client was stuck at what appeared to be an animated menu/splash screen and neither synthetic keyboard
input (`SendKeys` and raw `SendInput` hardware-level events) nor synthetic mouse clicks got it to
progress, so this fix is implemented and builds cleanly but unconfirmed in practice.

This fix does **not** fully explain the original two-machine (VM) reproduction, though — the VM's
`C:\temp\` is a physically separate disk from the host's, so this specific file-race can't cross between
them. Leading remaining hypothesis for that case: **resource contention, not a logic bug**. The Proxmox VM
runs on the *same physical host* as the native client, sharing the same underlying CPU and — since the
VM's GPU is passed through from that same physical card — GPU cycles too. Running two demanding UE5
processes simultaneously on shared hardware is a very plausible way to induce exactly the kind of
level-streaming/collision-registration stall that causes a freshly-spawned character to fall through the
floor, without needing any shared mutable state at all. Not confirmed — would need reproducing with a
genuinely separate, unloaded physical machine as the second client to rule resource contention in or out.

---

## Session 43: 2026-08-10 — Appearance Sync: All 21 Equipment Slot GameplayTags Found Live

**Goal**: resume the paused appearance-sync investigation (Session 41) — specifically, discover the real
`FGameplayTag` values `BP_JigHelperComp_C::SetEquippedInfoBySlot`/`GetEquippedInfoBySlot` expect for each
of the 21 equipment slots, since these are distinct from the wire protocol's positional slot index and
from `S_ServerEquippedItems`'s plain struct field names (`EquippedFacewear` etc., Session 16).

### UE4SS Lua's `GetPropertyValue` still doesn't help with `MapProperty` — parsed the raw `TSet` manually instead

Confirmed again this session that `helper:GetPropertyValue("EquipmentIDSlotConfig")` (a
`TMap<FGameplayTag, FS_EquipmentIDInfo>`) throws immediately via this UE4SS Lua binding, same as Session
41. Rather than continue fighting the Lua API, parsed the live `TMap`'s raw memory directly via IDA, now
with a confirmed-valid `BP_JigHelperComp_C` instance to read from (found via the technique below).

`EquipmentIDSlotConfig` at `helper+0xAF8` (`TMap`, reported size `0x50` in the CXXHeaderDump) decodes as a
standard UE5 `TSet`-backed `TMap`:

```
+0x00  void*   Data            — pointer to the TSparseArray's element storage
+0x08  int32   ArrayNum        = 21
+0x0C  int32   ArrayMax        = 22
+0x10  uint32  AllocationFlags = 0x1FFFFF  (21 low bits set — inline TBitArray storage,
                                            indices 0–20 allocated, index 21 free)
+0x20  int32   FirstFreeIndex  = 0
+0x24  int32   NumFreeIndices  = 0
+0x28  ...     (hash-table fields — bucket count, sentinel 0xFFFFFFFF, etc.; not needed,
                 didn't bother decoding fully since walking the sparse array directly
                 is sufficient to enumerate every element)
+0x40  void*   Hash            — hash bucket array pointer (unused by this approach)
```

Each element is a `TPair<FGameplayTag, FS_EquipmentIDInfo>`, **28 bytes per stride**, laid out
contiguously in the `Data` array — no free-list overhead to account for on allocated slots. The key
(`FGameplayTag`, i.e. a plain 8-byte `FName{int32 ComparisonIndex; int32 Number}`) is the first 8 bytes of
each 28-byte element; found the stride empirically by reading a large raw dump and spotting the repeating
small-magnitude-FName pattern at regular intervals (all 21 `ComparisonIndex` values clustered within a
span of ~261, consistent with sequential registration from one config asset at startup).

### Found the local pawn without any new UE4SS API guesswork — caught an existing call in flight

Needed a confirmed-valid `helper` pointer to read `EquipmentIDSlotConfig` from. Rather than orchestrating
a fresh `FindFirstOf`-style call, resolved UE4SS.dll's exported
`?GetValuePtrByPropertyNameInChain@UObject@Unreal@RC@@QEAAPEAXPEB_W@Z` (same PE-export-table-parsing
technique as Session 40/42) and put a breakpoint on its entry — `mod.cpp`'s own `find_local_pawn()` already
calls this every tick (`ctrl->GetValuePtrByPropertyNameInChain(L"AcknowledgedPawn")`), so the very first
hit (`RCX` = the `PlayerController` instance, `RDX` → the wide string `"AcknowledgedPawn"`) was the
correct one. Breakpointed the return address (read off `[RSP]` at entry, same pattern as Session 40) and
dereferenced `RAX` once to get the actual pawn pointer. From there, `pawn+0x700` (established path,
`mod.cpp`'s own `read_local_equipment()`) gave the `helper` pointer directly.

### Resolved every tag's real string by live-calling the game's own `FName::ToString`

With 21 raw `ComparisonIndex` values in hand, needed their string form. Rather than guess at a naming
convention from just the local player's `ActiveWeapon` (Session 41's plan), called the game's own
`FName::ToString` (`0x140C9D940`, the same function `mod.cpp`'s `native::fname_to_string()` uses) directly
via the live debugger, once per key:

1. Caught the process mid-execution at an arbitrary breakpoint (any suspend point works, since the calling
   thread's own progress doesn't matter for this — the technique **temporarily hijacks** the current
   thread's register state to make an unrelated call, then discards whatever the interrupted call's
   original return value would have been).
2. Set up a synthetic call: `RCX` = key address (the `FName` is already sitting in memory at the right
   layout, no need to copy it anywhere), `RDX` = scratch output buffer (borrowed from well below the
   current `RSP`, safely out of the way of any real stack usage), pushed the *current* breakpoint address
   itself onto the scratch stack as the return address, then set `RIP` = `FName::ToString`'s live address
   and continued.
3. Hit the same breakpoint again (now acting as our controlled return point), read the output
   `UnrealFString{wchar_t* data; int32 num; int32 max}` (`mod.cpp`'s own struct, `native::fname_to_string`
   uses the identical layout) and decoded the UTF-16 string.
4. Repeated for all 21 keys reusing the same scratch region and return breakpoint (fully synchronous,
   request-response per key — no need to juggle multiple in-flight calls). Batched into groups of 5 to
   stay under the MCP bridge's request timeout (a known hazard from earlier sessions).
5. Restored the original `RIP`/`RSP`/`RCX`/`RDX` and continued normally — the one interrupted unrelated
   call (whatever code originally called `GetValuePtrByPropertyNameInChain` for `AcknowledgedPawn`, in an
   unidentified module outside both the game and UE4SS.dll) received a garbage return value in `RAX` as a
   side effect, but the game remained fully stable afterward with no crash and normal continued mod
   operation — this call site is evidently either extremely tolerant of an occasional bad result (a
   generic per-frame UI/HUD property poll, most likely) or coincidentally didn't matter this one time.
   **Worth using a dedicated, known-safe breakpoint (e.g. our own mod's code, or a location proven to
   tolerate corruption) instead of an arbitrary in-flight call if this technique is reused.**

### Result: all 21 real tags, a clean and entirely predictable naming convention

| Slot (wire protocol index) | Real `FGameplayTag` |
|---|---|
| 0 Facewear | `Jig.PlayerSlot.Facewear` |
| 1 Headwear | `Jig.PlayerSlot.Headwear` |
| 2 Eyewear | `Jig.PlayerSlot.Eyewear` |
| 3 Accessory | `Jig.PlayerSlot.Accessory` |
| 4 Torso | `Jig.PlayerSlot.Torso` |
| 5 Gloves | `Jig.PlayerSlot.Gloves` |
| 6 Legs | `Jig.PlayerSlot.Legs` |
| 7 Feet | `Jig.PlayerSlot.Feet` |
| 8 Container | `Jig.PlayerSlot.Container` |
| 9 BodyArmor | `Jig.PlayerSlot.BodyArmor` |
| 10 Backpack | `Jig.PlayerSlot.Backpack` |
| 11 Primary | `Jig.PlayerSlot.PrimaryWeapon` |
| 12 Secondary | `Jig.PlayerSlot.SecondaryWeapon` |
| 13 Sidearm | `Jig.PlayerSlot.SidearmWeapon` |
| 14 Melee | `Jig.PlayerSlot.MeleeWeapon` |
| 15 Throwable | `Jig.PlayerSlot.Throwable` |
| 16 Flashlight | `Jig.PlayerSlot.Flashlight` |
| 17 Binoculars | `Jig.PlayerSlot.Binoculars` |
| 18 GPS | `Jig.PlayerSlot.GPS` |
| 19 Compass | `Jig.PlayerSlot.Compass` |
| 20 FishingRod | `Jig.PlayerSlot.FishingRod` |

Every name matches the wire protocol's own slot naming exactly except the four weapon slots, which carry
an extra `Weapon` suffix (`Primary` → `PrimaryWeapon`, etc.).

**Independently cross-verified**: after this extraction, equipped a real weapon in-game and re-read
`ActiveWeapon` (`helper+0xB98`, a single plain `FGameplayTag`) — its `ComparisonIndex` (`1730659`) matched
`Jig.PlayerSlot.PrimaryWeapon` exactly, confirming the whole `TSet` decode and tag-resolution chain end to
end against an independent, live ground truth.

### Remaining work before `SetEquippedInfoBySlot` can actually be called

Knowing the real tag strings closes one of the two blockers from Session 41. Still needed before wiring up
`ProxyManager` to apply cached equipment to a spawned proxy:

- **Constructing an `FGameplayTag` from a known string at runtime** — `ComparisonIndex` values are
  name-pool indices for *this specific process instance*; almost certainly stable across relaunches of the
  same build (the pool fills deterministically from the same assets in the same order) but not something
  to hardcode and trust blindly. Need to find and resolve the native `FName` constructor (or an
  `FGameplayTag`-specific request/registration function) the same way `FName::ToString` was resolved here,
  so the proxy-equipping code can build the right tag from the plain string at runtime regardless of which
  process/session it's running in.
- **The `FRepItemInfo`/`FGuid`/`bool` parameter marshaling for the actual `SetEquippedInfoBySlot` call** —
  still unverified. This is a Blueprint-authored function (no native by-address equivalent to bypass to,
  unlike `SpawnActor`/`FinishSpawning` in Session 40), so it has to go through `ProcessEvent` with a
  correctly-laid-out Kismet parameter buffer. Getting this wrong risks a crash, unlike everything read-only
  done so far this session — this needs its own careful, incremental live-verification pass before being
  called for real, not a first attempt built entirely from documentation/analogy.

## Session 44 — `GetFunctionByNameInChain` crash via live register hijack; pivot to compiled C++

Attempted the first live call toward `SetEquippedInfoBySlot`: resolve `GetEquippedInfoBySlot`
(the read-only getter, as a safer first step) via `helper->GetFunctionByNameInChain(L"GetEquippedInfoBySlot")`,
using the same register-hijacking technique that made 21 successful `FName::ToString` calls earlier this
session. The call crashed instead of returning: an access violation (`0xc0000005`) reading memory at
address `0x80`, landing several frames deep inside `UE4SS.dll` (not at the intended trampoline), with `RSP`
having grown by `0x10D8` bytes from the hijack's starting point before faulting.

### Recovery

Diagnosed via `ida_dbg.get_debug_event()`: `eid=7` is `ida_idd.EXCEPTION` (`ida_idd.BREAKPOINT` is `5`,
confirmed empirically — these are the real `debug_event_t` codes, distinct from the `dbg_*` notification
constants on `ida_dbg` which happen to overlap in value and are easy to confuse). Recovered cleanly without
restarting the game: restored the hijacked thread's original `RIP`/`RSP` (saved before the hijack), removed
both the trampoline breakpoint and a stale leftover breakpoint from an earlier investigation, and resumed —
confirmed via `Get-Process -Id <pid> | Responding` that the game came back fully responsive. This recovery
procedure (save original `RIP`/`RSP` before every hijack, restore + clear breakpoints + continue on any
unexpected event) should be treated as mandatory going forward for this technique, not optional cleanup.

### Ruled out both easy explanations

1. **Stale `helper` pointer?** No — re-derived a fresh `BP_JigHelperComp_C` pointer via the established
   non-invasive technique (breakpoint the entry and return address of `GetValuePtrByPropertyNameInChain`,
   catch `find_local_pawn()`'s own `AcknowledgedPawn` read in flight) and got back the *exact same* pointer
   (`0x136769a00`), independently confirmed still valid (`ActiveWeapon` read back `ComparisonIndex=1730659`,
   correctly matching `PrimaryWeapon` — consistent with "weapon is out now").
2. **Wrong/guessed function name?** No — `research/CXXHeaderDump/BP_JigHelperComp.hpp:63` confirms
   `GetEquippedInfoBySlot(FGameplayTag Slot, FRepItemInfo& Info, bool& Equipped)` is a real, dumped member,
   not a guessed pairing with the setter.

Both of those were the leading hypotheses; ruling them out points at the manual call setup itself (shadow
space, stack alignment, or scratch-string placement colliding with the callee's own deep stack usage across
`0x10D8+` bytes) rather than anything wrong with the target object or name.

### Decision: move `GetFunctionByNameInChain`/`ProcessEvent` calls into compiled C++, not further live hijacking

`mod.cpp:847` already calls `menu->GetFunctionByNameInChain(...)` + `ProcessEvent` successfully from real
compiled code (the `try_open_world()` continue-button click), proving the exported function itself is fine
when called with a compiler-generated calling convention — it's the hand-rolled register/stack setup in the
live-hijack technique that's the suspect, not UE4SS's implementation. Given the remaining work is substantial
(FGameplayTag-from-string construction, `FRepItemInfo::ItemID` asset-lookup-by-name, and Kismet parameter
marshaling for the actual write call) and two live attempts have now frozen the game momentarily, further
iteration on `SetEquippedInfoBySlot`/`GetEquippedInfoBySlot` should happen as real mod code (correct calling
convention guaranteed by the compiler, testable via normal log output) rather than more ad-hoc IDA-side
native calls. This requires a mod rebuild + game restart to test, unlike the fully live IDA workflow used
so far this session.

### Retried anyway with a much larger stack safety margin — crashed again, differently, and found the real cause

Re-derived a fresh `helper` pointer (again `0x136769a00`, confirmed still valid — `ActiveWeapon` still reads
`ComparisonIndex=1730659`) via the same non-invasive entry/exit breakpoint technique. This time probed the
hijacked thread's actual mapped stack range first (`ida_bytes.get_bytes` reads succeeded at least 256 KB
below its `RSP`, vs. the ~0x8000-byte cliff seen on an earlier thread) and used generous separation: fake
call frame 64 KB below `RSP` (16-byte aligned per the real x64 `call` convention), scratch string buffer a
further 64 KB below that. Confirmed both writes with `ida_bytes.get_bytes` readback — note
**`ida_bytes.get_bytes` reads a stale cache after `ida_dbg.write_dbg_memory()`; must call
`ida_dbg.invalidate_dbgmem_contents(ea, size)` before reading back a fresh write**, a new gotcha for this
technique.

Also newly learned: `debug_event_t.eid` is inconsistently exposed by the SWIG binding — sometimes a plain
int attribute, sometimes a bound method depending on the event — so reading it must handle both
(`ev.eid() if callable(ev.eid) else ev.eid`). And confirmed thread IDs get recycled quickly (the original
`tid=30900` from earlier in the session had already exited by this point) — a "known-good" thread from
one moment cannot be assumed to still exist, or be the same physical thread, a few actions later.

The call crashed again — but this time only ~0x70 bytes into the call (not 0x10D8 bytes deep), a pure
null dereference (`referenced memory at 0x0`, not `0x80`). The shallow, different-address, different-offset
crash on the *same call with a much bigger safety margin* ruled out the stack-collision theory entirely.
Recovered the same way as before (restore saved `RIP`/`RSP`/`RCX`/`RDX`, clear breakpoints, continue) —
confirmed responsive again via `Get-Process -Id 90048 | Responding`.

**Root cause, found by manually disassembling the live bytes at `GetFunctionByNameInChain`'s real entry
(`0x7ff81aa6a720`, via `ida_ua.decode_insn` — no analysis database exists for `UE4SS.dll`, same approach as
Session 40's `BeginDeferredActorSpawnFromClass` trace):**

```asm
push rbx; sub rsp, 0x20; xor eax, eax; mov rbx, rcx        ; rbx = this
cmp byte ptr [rip+0xA245F7], al                             ; global "ready" flag
mov [rsp+0x40], rax                                         ; scratch slot defaults to 0
jz  0x7ff81aa6a746                                          ; flag==0 -> SKIP the init call
    xor r8d, r8d
    lea rcx, [rsp+0x40]
    call [rip+0xA245CA]                                     ; lazy-init, writes [rsp+0x40] as an out-param
mov rdx, [rsp+0x40]                                         ; rdx = whatever ended up there
mov rcx, rbx
call 0x7ff81aa6a760                                         ; main body: rcx=this, rdx=[rsp+0x40]
```

Inside the main body, that `rdx` is passed into a further helper (`call` to `0x7ff81aa69750`), and the
helper's return value in `RAX` is immediately dereferenced (`mov rcx, [rax]`) with no null check. If the
global "ready" flag reads as already-set on whatever thread/context we hijack (real state left over from
wherever the flag last got set during normal game execution — not necessarily meaningful for our
purposes), the lazy-init call is skipped, `rdx` stays `0`, the inner helper returns null for that input,
and `[rax]` faults reading address `0`. This is a **global, not per-thread, lazy-init gate**, so it isn't
about which physical thread does the hijacking at all — it depends on state the function's *normal callers*
set up beforehand that a manually-hijacked out-of-band call has no way to replicate correctly.

**Conclusion**: `GetFunctionByNameInChain` cannot be safely called via manual register-hijacking from an
arbitrary suspended point, regardless of thread choice or stack margin — it depends on caller-side state
that's only valid when reached through its real call path. Confirmed real compiled C++ (`mod.cpp:847`) goes
through that real path correctly and works. The pivot to writing this as real mod code (not further IDA-side
native calls) is no longer just "safer in general" but the actual fix for a real, now-understood blocker.

## Session 45 — Equipment sync implemented, read path verified live, write path hits a real wall

Implemented `get_equipped_info_by_slot()`/`set_equipped_info_by_slot()`/`set_active_weapon_slot()` in
`proxy_manager.cpp` as real compiled C++ (per Session 44's conclusion), wired into `ProxyManager::tick()`
via `sync_equipment()`. `RawFGameplayTag`/`RawFRepItemInfo`/`RawFGuid` mirror the CXXHeaderDump layouts;
Kismet param buffers use standard UHT sequential-alignment offsets, confirmed via `static_assert(offsetof(...))`.
Only `PrimaryWeapon` (slot 11, `ComparisonIndex=1730659`) has a real tag filled in — the other 20 are
placeholder `0`s (`slot_tag()` safely no-ops for those) since re-extracting them needs a live IDA session,
which wasn't available when this was written (game had closed, IDA session had restarted losing debugger
state).

### Found the mod's own log file: `SDB.log`, not `UE4SS.log`

Burned a long detour chasing "zero SDB: log output despite the mod visibly working (TCP connected, server
showed real joins)" — tried disabling `GuiConsoleEnabled` as a fix, which did nothing, because the actual
problem was checking the wrong file the entire time. The mod's own `Output::send` calls land in a **separate
`SDB.log`** file in the Win64 directory, not `UE4SS.log` (which only has UE4SS core + other mods' output).
Saved as a standing memory (`feedback_sdo_log_file_location`) since this had already happened once before
this session.

### Server-side gap: Equipment frames were never relayed between players

`host-agent.js`'s `MsgType.Equipment` handler was a stub (`// Equipment updates are informational for the
host (future: validate loadout). break;`) — meaning `on_equipment()` could never fire on any client, ever,
regardless of anything client-side. Fixed in `gateway.js`: `Equipment` now gets the same treatment as
`Movement` — broadcast directly to other clients via `_broadcast()`, plus a copy to host. Required a full
local dedicated-server restart to take effect (`worldId` is regenerated fresh each start, invalidating all
outstanding tickets — every client needs a fresh ticket after any server restart, not just a game restart).

### Read path: confirmed working end to end, live, no crashes

With the relay fix, `SDB: equip-getter slot=11 itemId=AK15 ok=1 equipped=0` appeared on client 2 for PC1's
real AK15 — `GetFunctionByNameInChain` + `ProcessEvent` succeeded via real compiled code, matching Session
44's prediction that this would work once routed through a normal call path instead of manual hijacking.
`equipped=0` is correct, not a bug: this queries the **proxy actor**, a blank never-written stand-in, so
reporting nothing equipped is exactly right before any write ever happens.

### Write path: calls succeed (`ok=1`) but never actually persist

Enabled `kEnableEquipmentWrite` for slot 11 only. `SetEquippedInfoBySlot` → `ok=1`. Added
`SetActiveWeaponSlot` (separate `ActiveWeapon` property, confirmed distinct from equipped-items data back in
Session 43) → `ok=1`. Added `OnRep_ActiveWeapon()` (manually invoking what real network replication would
normally trigger automatically on a client, since our proxy has no real replication happening) → `ok=1`.
None of it made the proxy visually hold the weapon. Read back via `get_equipped_info_by_slot()` immediately
after: still `equipped=0`, every time, across dozens of cycles. **`ok=1` only means `ProcessEvent` didn't
crash — it says nothing about whether the Blueprint's internal logic actually accepted the write.** The
item asset resolution itself was verified correct in the same test (`resolved_ptr=0x246f545a300,
cache_size=588` — a real, valid `UJigsawItem_DataAsset_C*`), ruling out a null-item-pointer explanation.

### Traced the real equip action — found it never calls anything reflectable at all

Added an `on_process_event_pre`-based trace (mod.cpp) to log every `ProcessEvent` call on the local player's
own `BP_JigHelperComp_C` instance, unthrottled. Had the user manually unequip and re-equip their AK15 through
the game's real UI — a confirmed real state transition (slot went from populated to empty and back). The
trace caught real activity (`OnInventoryOpenClose_Event_0`, `TraceToWorld`, `UpdatePrevFromPrim`,
`OnPawnControllerChangedDelegates_Event_0`) but **zero** calls resembling an equip/inventory write, on either
the unequip or the re-equip.

Broadened the trace further to log **every** `ProcessEvent` call from **any** object, gated behind a
short (5s) externally-triggered window (`check_trace_trigger()` polls an `%APPDATA%\SurrounDeadBridge\
trace_trigger.flag` file every `do_game_tick()`, same file-flag IPC pattern as character creation) to avoid
flooding the log the way an always-on unthrottled trace would (see the 106 MB `SDB.log` incident below).
Captured 100+ distinct functions across a real 5-second window including the equip action — background
ticks, AI, weather, UI bindings, animation graphs, everything — but still **nothing** containing "Equip" or
resembling an inventory write.

**Conclusion**: the real equip/unequip logic does not go through `UObject::ProcessEvent` at all — not on the
helper component, not on anything else in the entire game during that window. This means it's implemented in
native C++ (very plausibly inside the "JigSInventory" plugin's compiled backend, given the asset path
`/Game/JigSInventory/...`) that directly manipulates the underlying data, bypassing Blueprint/reflection
entirely for this specific operation. No `ProcessEvent`-hook-based technique — safe or otherwise — can ever
observe this call. `SetEquippedInfoBySlot`/`SetActiveWeaponSlot` are real, callable Blueprint functions that
exist for some other purpose (initial loadout, debug tooling, etc.), not what the interactive drag-and-drop
UI actually uses.

### Two live crashes from a hardware write-watchpoint — do not reuse this technique as tried

Attempted to find the real write by setting an IDA hardware write-watchpoint (`ida_dbg.add_bpt(addr, 8,
ida_idd.BPT_WRITE)`) on the exact live address of the Primary slot's `ItemID` field
(`helper + 0xF8 + kSlotOffsets[11]`), reasoning that a watchpoint is purely observational (unlike the
Session 44 register-hijacking) and would catch whatever native instruction performs the write regardless of
Blueprint visibility. Confirmed the target address and current value looked sane before arming
(read back `0x0` when the slot was genuinely empty in one attempt, a plausible-if-unverified pointer in
another).

Both attempts crashed the game outright — not a clean breakpoint stop: `RIP` came back as literal `0x0`
(`eid=7`/`EXCEPTION` on an unrelated thread), and the process was fully gone (not just frozen) moments later.
The **second** attempt crashed **immediately** upon resuming, before any user action at all, proving this
has nothing to do with the equip gesture itself and everything to do with the watchpoint mechanism being
unstable for this game/IDA combination — possibly a debug-register conflict, or something about how this
memory region gets accessed (JIT'd code, an SEH handler, or similar) that a plain write-watchpoint interacts
badly with. **Do not retry hardware watchpoints on this target without a fundamentally different setup**
(e.g., a soft page-guard technique instead of CPU debug registers, or watching a decoy/copy location instead
of the live field) — the failure was fast, clean, and 2-for-2 reproducible, not a fluke worth trying a third
time the same way.

### Known-good infrastructure fixes from this session (not mod-code, but load-bearing for testing)

- **Client 2 (VM) NVENC/software-encoding fix**: Parsec was falling back to software H.264 because Proxmox's
  default emulated console display (always "connected" from boot) kept Windows on it as primary, so Parsec's
  capture/encode pipeline never bound to the passthrough GPU at all — not a GeForce-VM-detection issue as
  first suspected (that theory's SMBIOS spoof + `vmgenid` disable made no difference). Real fix: `vga: none`
  in the Proxmox VM config, forcing Windows onto the GPU (or Parsec's own Virtual Display Adapter) as primary.
  Confirmed via `nvidia-smi`-style Parsec log lines: `encoder = nvidia`, `Using modern NVENC preset`.
- **Client 2 RAM bump**: 12 GB → 16 GB (`qm set 102 --memory 16384`, needs a full `qm stop`/`qm start`, not
  just a guest reboot — Proxmox stages it as `[PENDING]` otherwise). Also added a Windows Defender exclusion
  for the game's install directory (zero exclusions existed before), the more likely fix for reported
  in-game stutter than the RAM bump itself.
- **Launching the game on client 2 via SSH silently does nothing visible**: any `Start-Process "steam://..."`
  invoked over an SSH-opened PowerShell session lands in **Session 0**, not the real interactive desktop
  session (confirmed via `tasklist /V` showing spawned processes as `Services`/session 0 instead of
  `Console`/session 1). Fixed by launching through a Scheduled Task created with `/it` (interactive) and
  `/ru <user>`, triggered via `schtasks /run` — this lands correctly in the real session. `play_local.ps1`
  (the local dev-server ticket-fetch script) needed a `-NoLaunch` switch added so its own redundant
  session-0 launch attempt didn't keep interfering with the working scheduled-task launch.
- **Local dev server ticket flow is not `play.ps1`'s** (`game.ristl.org`/`/v1/join`, 2-minute TTL,
  `serverId`+`playerId`+`displayName` body) — the local server (`server/src/index.js`) exposes a different,
  simpler endpoint: `POST http://<host>:42201/v1/tickets` with just `{playerId, displayName}`, returns
  `{ticket, gatewayHost, gatewayPort, worldId}`, `ticketTtlMs=3600000` (1 hour, per `settings.json`).
  `gatewayHost` in the response is always `127.0.0.1` regardless of who asks (server-side logic, not
  client-aware) — a second machine (client 2) must override it to the host's real LAN IP manually.
- **Tickets are single-use** (`_usedTickets` replay guard in `gateway.js`) and tied to the server's current
  `worldId`, which is regenerated fresh on every server restart — reusing an old `session.cfg` after either
  a server restart or a prior successful connection silently fails auth (`ticket_replay` or a `worldId`
  mismatch), which manifests client-side as an infinite `[tcp] authentication rejected` retry loop that
  floods `SDB.log` to well over 100 MB if left running. Every fresh game launch needs a fresh ticket fetch,
  no exceptions.

### Correction: not native C++ after all — it's Blueprint, just via widget drag-drop, not BP_JigHelperComp directly

Static analysis reversed the native-code theory almost immediately: searching the executable's indexed
strings (94,226 total, via `mcp__ida__find`/`find_regex` after an explicit `server_warmup(build_caches=true)`
— the default search tools timed out repeatedly against this string count before warming the cache) for any
`Jig`-prefixed class name returned **zero matches anywhere in the binary**. There is no native C++ plugin
backing this system at all; `BP_JigHelperComp_C` sits directly on stock `UActorComponent`. The "native code"
conclusion from the trace was actually a **timing problem**: a one-time discrete UI event (a drag-drop) is
easy to miss in a narrow trace window when background ticks (which fire every frame, guaranteed to be
caught) dominate the noise.

Redid the broadened `on_process_event_pre` trace with tight synchronization (armed the trigger and had the
user act within the same message) and this time caught the real chain:

```
JSI_Slot_C:OnMouseButtonDown -> OnDragDetected -> DragWidget_C:Construct
  -> JSIContainer_C:OnDragEnter / OnDragLeave  (hover feedback while dragging)
  -> JSI_Slot_C:OnDrop / JSIContainer_C:OnDrop  (the actual drop)
  -> DragWidget_C:Destruct
  -> BP_JigMultiplayer_C:EventOnJigItemMouseButtonDown
```

Standard UMG drag-drop pattern: `JigSDragOperation_C` (a custom `DragDropOperation` subclass, confirmed via
FModel's structural JSON export of `JSI_Slot.uasset` — a local variable in `OnDrop`'s signature) carries the
dragged item's payload between widgets.

### Got FModel working (needed a `.usmap` mapping file) — but structural export doesn't include bytecode

FModel (`C:\Users\mccau\Downloads\FModel.exe`) initially failed to open any Blueprint asset:
`MappingException: Package has unversioned properties but mapping file is missing, can't serialize`. Fixed
by generating one live: UE4SS ships a USMAP dumper, bound by the stock Keybinds mod to **Ctrl+Numpad6**
(`DumpUSMAP` in `Mods/Keybinds/Scripts/main.lua`) — produces `Mappings.usmap` directly in the Win64
directory. Pointed FModel's Settings -> Mappings at it, and `JSI_Slot.uasset` exported cleanly afterward.

The JSON export (`Exports/SurrounDead/Content/JigSInventory/Jigsaw/Widgets/JSI_Slot.json`, 16.5k lines) only
contains structural data — class hierarchy, property/function **signatures**, CDO default values — no
`"Script"` field anywhere (confirmed via direct grep). FModel's structural export is the wrong tool for
seeing what a function's graph actually *does*; it's the right tool for confirming type/signature info
(which is what identified `JigSDragOperation_C`).

### Reading `UFunction::Script` directly from live memory — safe (pure read, no calls/watchpoints)

With the trace narrowing candidates to a short list of real `OnDrop`-family functions, went straight to each
one's raw compiled bytecode instead of more tracing or asset tooling. Reused the **already-proven-safe**
`GetFunctionByNameInChain` call path (real compiled code, per Session 44/45's established distinction between
"safe when called normally" vs. "crashes when hijacked") to get a live `UFunction*` for a given class+function
name pair, then just read raw struct bytes at increasing offsets — no `ProcessEvent`, no breakpoints, no
watchpoints, nothing that touches execution or debug registers. Added this as a reusable diagnostic
(`check_bytecode_dump_trigger()` in `mod.cpp`), driven by the same file-flag trigger pattern as the earlier
trace, with the target class/function names read from the flag file's own content so no rebuild is needed
per candidate.

**Found `UFunction::Script` (the `TArray<uint8>` bytecode array) at `UFunction + 0x60`** for this build —
`Data` pointer at `+0x60`, `Count` (int32) at `+0x68` low dword, `Max` (int32) at `+0x68` high dword/`+0x6C`.
Identified empirically (dumped a range of raw qwords looking for a plausible pointer-then-two-int32s
pattern) and confirmed conclusively: `JSI_Slot_C::OnDrop`'s bytecode was exactly 33 bytes and the 33rd byte
was `0x53` (`EX_EndOfScript`), the mandatory terminator for every compiled Kismet function — can't be a
coincidence at exactly the boundary implied by the `Count` field.

Checked three candidates by raw byte count (a strong, fast proxy for "does this function actually do
anything," without needing to hand-disassemble):
- `JSI_Slot_C::OnDrop` — **33 bytes**. Manually decoded: `0x14` (`EX_LetBool`) x2, then `0x04` (`EX_Return`),
  ending in `0x53`. A near-empty stub — just sets its `bool ReturnValue` and returns. Not where the logic is.
- `JigSDragOperation_C::Drop` — **0 bytes** (`Script.Data=0x0, Count=0, Max=0`). No override at all; this
  class doesn't implement the native `DragDropOperation::Drop` event in Blueprint. Needed the drag to be
  *actively in progress* (mouse held down) for `FindFirstOf` to find even the instance at all — a
  `DragDropOperation` object is transient, created in `OnDragDetected` and gone shortly after the drop, unlike
  a widget CDO which persists once its class is loaded.
- **`JSIContainer_C::OnDrop` — 9,137 bytes.** Two orders of magnitude larger than the other two — this is
  where the real logic lives. Saved the raw bytecode to
  `research/bytecode/JSIContainer_C_OnDrop.bin` (9,137 bytes, live-verified `EX_EndOfScript` would need
  checking at the tail same as the 33-byte case, not yet confirmed for this one specifically) for a future
  session with a proper Kismet bytecode disassembler — hand-decoding 9 KB of raw opcodes by eye isn't a
  reasonable use of a single session, but a real disassembler (opcode tables are fully public,
  `Engine/Source/Runtime/CoreUObject/Public/UObject/Script.h`'s `EExprToken` enum) should make quick work of
  it once one exists for this project.

### Wrote a working Kismet bytecode disassembler and fully decoded JSIContainer_C::OnDrop

Built `research/bytecode/kismet_disasm.py`, a recursive-descent Kismet bytecode disassembler, rather than
hand-reading the raw bytes. Bootstrapped the `EExprToken` opcode table by fetching the authoritative version
from CUE4Parse (`FabianFG/CUE4Parse`, the open-source library FModel itself uses — Epic's own `UnrealEngine`
repo is private) via WebFetch, after an initial hand-typed table (from memory) turned out to have several
wrong values, caught by cross-checking against `JSI_Slot_C::OnDrop`'s 33-byte stub as an empirical anchor
(known operand structure, known terminator position).

Iteratively fixed a series of operand-format bugs by decoding the 33-byte anchor first, then the real 9,137-byte
`JSIContainer_C::OnDrop`, re-verifying the anchor after every fix so a correction never silently broke what
already worked:

- `EX_Let` (plain, non-specialized) **does** serialize a leading `FProperty*` before its two nested
  expressions — unlike `EX_LetBool`/`EX_LetObj`/etc., which don't. An early version incorrectly assumed all
  `Let` variants shared the no-leading-pointer shape of `EX_LetBool` (the only one actually exercised by the
  33-byte anchor).
- **`FName` is 12 bytes in this build, not 8** (`ComparisonIndex`, an unidentified second `int32` — assumed
  `DisplayIndex` for case-preserving name support, a real UE5 feature — then `Number`), for every opcode that
  embeds one directly (`EX_NameConst`, `EX_VirtualFunction`, `EX_LocalVirtualFunction`, `EX_InstanceDelegate`,
  `EX_BindDelegate`). Found via a `KismetSystemLibrary::PrintString` call resolved live (see below) — its
  known, fixed signature made it obvious a trailing `Key=NAME_None` parameter was being misread. This also
  retroactively explained an earlier "Local variants have 4 mystery extra bytes" patch as the same underlying
  issue wearing a different hat — removed that special-casing once the real 12-byte width was understood.
- Several loop-termination bytes (`EX_EndFunctionParms`, `EX_EndStructConst`, `EX_EndArray`,
  `EX_EndArrayConst`) were hardcoded to their values from the *first*, wrong opcode table and never updated
  when the table was replaced with the authoritative one — silent off-by-one bugs since the corrected table
  shifted those values by one position.

Ultimately decoded the entire 9,137-byte function with **zero unhandled opcodes**, confirmed via two
independent structural checks: the function's very first instruction (`EX_PushExecutionFlow -> 0x23a6`) and
its second-to-last (`EX_PopExecutionFlow` immediately before `EX_Return` landing at exactly `0x23a6`) point at
the identical address, and the decode ends in the same `EX_Return`-of-`EX_LocalOutVariable`-then-
`EX_EndOfScript` pattern already validated against the 33-byte anchor.

### Resolved every function pointer and property name in the decoded output — found the real architecture

Extended `check_resolve_ptr_trigger()`/`check_resolve_fname_trigger()` (mod.cpp) to accept a whole batch (one
address/FName per line) instead of one at a time, then resolved all ~17 unique function pointers and ~32
unique `FName`s referenced anywhere in the decoded `OnDrop`. Saved to `research/bytecode/resolved_names.txt`
alongside the full annotated decode (`JSIContainer_C_OnDrop_disasm.txt`) and the disassembler script itself.

**This fully explains why `SetEquippedInfoBySlot` never persisted anything (Session 45's original mystery):
it simply isn't part of the real drag-drop path at all.** `OnDrop` never calls it. Instead it's a substantial
state-machine-style handler built almost entirely from generically-named internal helpers — `IsEquipped?`,
`IsEquipTo?`, `GetEquippedItemRef`, `CheckLimitedEquipToStack`, `CheckUnhandledSplit`, `CheckUnhandledStack`,
`CombineItemRequest`, `CompareItems`, `HandleContainerOnContainer`, `OnDropCheckStackability`,
`GetEmptySlotTryRotated_NonPure`, `DetectChange` — that check the drop's context (same slot? stackable?
swap-equip? container-on-container?) and ultimately dispatch to an internal function literally named
**`PerfromDrop`** (a genuine typo baked into the shipped Blueprint, not a transcription error here) to
actually execute the transfer. Confirmed the write mechanism itself is **not** a hardcoded function at all,
but `KismetSystemLibrary::SetObjectPropertyByName`/`SetIntPropertyByName` — generic by-name reflection
setters — building a small request object with `ContainerRec` (self-reference), `SlotRef`, and `ToSlot`
fields immediately before what's presumably the `PerfromDrop` dispatch later in the function.

Also resolved incidentally: `CancelHighlights` (confirms the earlier trace finding), and a UI-widget
creation/display sequence (`WidgetBlueprintLibrary:Create` -> three `SetObjectPropertyByName`/
`SetIntPropertyByName` calls -> `AddToViewport(ZOrder=99)`) — likely a drop-feedback popup, not equip logic.

### Traced the call chain one more layer down: PerfromDrop is UI-only, real chain lands in a shared Ubergraph

Exported `JSIContainer.uasset` via FModel (same `.usmap` setup as `JSI_Slot.uasset` earlier) to get
`PerfromDrop`'s real declared signature directly from reflection metadata instead of guessing from raw
local-variable pointers: `PerfromDrop(int32 SlotIndex, FVector2D SlotVector, JSI_Slot_C* SlotRef, bool
Rotated, out bool Moved)`. All five parameters are UI-layer (which slot *widget*, grid position, rotation) —
confirmed by then dumping and fully decoding `PerfromDrop` itself (2,265 bytes, zero errors): it's entirely
visual/layout bookkeeping (`ClearSlot`, `GetPaddingBySlotIndex`, `RotateSlot`, `SetHostedSlot`,
`SetHostingArray`, `SetVisibility`) with no property writes of its own. The actual authoritative write
happens elsewhere, reached via a **delegate broadcast** (`Drop_ItemOverItem`), not a direct call.

Found the delegate's bound handler in the existing `BP_JigMultiplayer.hpp` CXXHeaderDump (already on disk
from an earlier session, no new dump needed): `Drop_ItemOverItem_Event_0(...)` calls
`HandleItemOverItem(UJSIContainer_C* Container)`. Dumped and decoded `HandleItemOverItem` (36 bytes, zero
errors after one small fix) and found it's a **thin Ubergraph-dispatch trampoline** — the standard UE5
Blueprint compiler pattern where a named function just stashes its parameter on a "persistent frame" slot
and jumps into one shared per-class function (`ExecuteUbergraph_<ClassName>`) at a specific integer entry
point, rather than containing any real logic itself:

```
EX_LetValueOnPersistentFrame frame_slot=0xff967300 = LocalVariable(Container param)
EX_LocalFinalFunction func=ExecuteUbergraph_BP_JigMultiplayer(EntryPoint=9060)
Return Nothing
```

This surfaced one more genuine disassembler bug: `EX_LetValueOnPersistentFrame` was grouped with
`EX_LetBool`/`EX_LetObj`/etc. (no leading pointer) but actually has its own distinct shape — a direct 8-byte
frame-slot identifier followed by *only* an Assignment expression (no nested "Variable" expression at all,
unlike every other `Let` variant). Fixed and re-verified against the 33-byte anchor before trusting the
36-byte decode.

**Dumped the full `ExecuteUbergraph_BP_JigMultiplayer` (24,576 bytes) — decoding stalled partway through**,
saved as `research/bytecode/BP_JigMultiplayer_C_ExecuteUbergraph_partial_disasm.txt` (~311 of 24,576 bytes,
everything before it hand-verified correct byte-for-byte, including a working `EX_ObjToInterfaceCast` and a
correctly-decoded nested `EX_InterfaceContext`). This is a qualitatively different problem from the three
functions decoded before it: a Blueprint Ubergraph concatenates *every* event graph in the class into one
function with a jump-table dispatch at the top (an `EX_ComputedJump` off a precomputed offset — visible at
the very start of the file, before the byte range that stalled), so entry point 9060 (`HandleItemOverItem`'s
target) is a specific offset deep inside a 24 KB function whose *other* 23-odd KB is unrelated event-graph
code for entirely different Blueprint events. Reaching it means understanding the jump-table mechanism
first, not just fixing operand-format bugs one at a time the way the last three functions went — a
different, more open-ended task than what today's tooling was built for.

### Remaining work

- Fill in the other 20 slots' real `ComparisonIndex` values (safe, read-only, proven technique from Session
  43 — just needs a live IDA session with the game actually running).
- Understand `ExecuteUbergraph_BP_JigMultiplayer`'s entry-point jump-table mechanism (the `EX_ComputedJump`
  at the very start of the function) well enough to jump straight to entry point 9060's code instead of
  decoding sequentially from byte 0 — the real equip-write logic (the actual `ServerEquippedItems`/property
  mutation) is presumably right there, given everything upstream of it (`OnDrop`, `PerfromDrop`,
  `HandleItemOverItem`) is now conclusively ruled out as UI-only or a pure trampoline.
- Once the real write mechanism is identified, replicate it in `proxy_manager.cpp` (calling whatever
  specific internal helper it turns out to be via `GetFunctionByNameInChain`, or via
  `SetObjectPropertyByName`/`SetIntPropertyByName` the same way, now that both the technique and the
  toolchain — `kismet_disasm.py`, `check_bytecode_dump_trigger()`, the batch `resolve_ptr`/`resolve_fname`
  triggers — are all proven and reusable) instead of the abandoned `SetEquippedInfoBySlot` approach.
- Solve the "unequip doesn't clear the proxy" gap noted in `sync_equipment()`'s comments before enabling
  writes for real, once the underlying write mechanism is actually understood.

## Session 46 — Ubergraph fully decoded, real equip architecture found, visual-attach chain traced 5 layers deep, weapon actor confirmed never spawned

Picked up exactly where Session 45 left off: `ExecuteUbergraph_BP_JigMultiplayer`'s entry-point mechanism was
still unexplained, decode stalled at ~311 of 24,576 bytes.

### Confirmed entry point is a direct byte offset, not a jump-table index

Jumped straight to offset 9060 (`HandleItemOverItem`'s entry point) in the existing 24,576-byte dump and got
immediately coherent, correctly-structured output — no jump table lookup needed at all. `EntryPoint` passed to
`ExecuteUbergraph_<ClassName>(int32 EntryPoint)` is simply a direct byte offset into that same function's own
bytecode array.

### Two more disassembler bugs found and fixed, verified against the 33-byte anchor each time

- **`EX_Context`/`EX_ClassContext`/`EX_Context_FailSilent` wrapping an `EX_InterfaceContext` object_expr have
  no separate `skip`+`field` pair** — execution goes straight from the end of `object_expr` into `context_expr`.
  Found via manual byte forensics on two separate occurrences of the identical garbage-value pattern (once in
  `JSIContainer_C::OnDrop`, once in the Ubergraph itself). Fixed by peeking the not-yet-consumed opcode byte
  before parsing `object_expr`, and skipping the `skip`/`field` read when it's `0x51` (`EX_InterfaceContext`).
- **`EX_SkipOffsetConst`** (`0x5B`) was entirely unhandled — a plain `uint32` constant operand, no nested
  expression. Missing this stalled the decode again immediately after the `EX_InterfaceContext` fix, inside an
  `EX_StructConst` member list.

With both fixes, `ExecuteUbergraph_BP_JigMultiplayer` decodes **completely clean from entry point 9060 to
`EX_EndOfScript`, zero unhandled opcodes**, up from the ~311 bytes at the end of Session 45.

### Discovered live pointers/FNames are only valid within the exact process that produced the dump

First resolve-batch attempt against the existing (Session-45-era) `.bin` dump crashed the game hard —
`EXCEPTION_ACCESS_VIOLATION reading address 0xffffffffffffffff` inside `UObject::GetPathName()`'s Outer-chain
walk, called from `GetFullName()`. The struct/function pointers embedded in linked Kismet bytecode are literal
runtime addresses baked in at load time for *that specific process instance* — ASLR and heap layout differ on
every relaunch, so a dump captured in an earlier game session is guaranteed garbage once that process has
restarted, even for structurally identical bytecode. This also explains an earlier oddity from the *same*
dump: FName resolution wasn't crashing but was returning garbled, overlapping strings for many (not all)
indices — same root cause, not a decoder bug. Confirmed by re-dumping the identical function fresh in a live
session and getting 100% clean FName resolution with zero garbling.

**Fix, in two parts:**
- Added an SEH guard (`seh_invoke`/`__try`/`__except` in `mod.cpp`) around `check_resolve_ptr_trigger`'s
  `GetFullName()` call, split into a trampoline with no local C++ objects needing unwinding (MSVC `C2712`)
  plus a plain inner function that does the actual call — turns a hard game crash on a bad/stale pointer into
  a logged `<access violation, not a live UObject here>` line instead. Still a real crash risk for anything
  that overflows the stack rather than taking a clean access violation (see below), but closes the common case.
- **Established the actual working methodology going forward: always re-dump the target function fresh in the
  exact same live session as the resolve calls, never reuse a `.bin` from an earlier session or an earlier
  relaunch.** Re-verified this repeatedly for the rest of the session with zero further crashes from stale
  pointers.

One batch (11 func pointers from a later dump) was silently consumed with zero log output despite valid
content and a live, non-stale session — bisected down to a single suspect address (`0x10ba5eb00`, oddly
9-hex-digit-shaped) that seemed like the likely culprit for a stack-overflow-via-runaway-Outer-chain-recursion
(which SEH can technically catch via `STATUS_STACK_OVERFLOW`, but the stack is left in an unreliable state
afterward, plausibly explaining silent data loss rather than a clean crash or a clean success). Re-tested all
10 *other* addresses individually and in small batches afterward — every one resolved cleanly on its own,
making the specific cause inconclusive but the practical workaround (smaller batches, retry on silent
zero-output) trivial and reliable.

### Found the real equip data architecture — completely different from what `SetEquippedInfoBySlot` assumed

Resolved every FName/pointer in the freshly-decoded Ubergraph. Two calls stood out immediately:
`ServerFuncHandleEquipActor` and `HandleActorEquipped`, both invoked as `LocalVirtualFunction`s (same-class
calls) from inside the entry-point-9060 code path.

Dumped and fully decoded `ServerFuncHandleEquipActor` (3,458 bytes, zero errors). It:
1. Loops over an `InstanceVariable` array property (raw pointer, address confirmed process-specific/unstable
   as above), matching elements by GUID (`KismetGuidLibrary::EqualEqual_GuidGuid`) against a target container
   ID.
2. Builds a complete `FRepItemInfo` struct field-by-field in a local variable, sourced from a `TMap`'s
   `Map_Keys`/`Map_Values` (custom item data) plus the target actor's own instance variables (Count, ItemVec,
   Weight, Price) plus a freshly-`GenerateRandomStats` call, computing `Durability` via
   `KismetMathLibrary::MakeVector2D`, and explicitly resetting `Pending? = -1.0` (a "not pending" sentinel).
   Confirmed field-for-field against `research/CXXHeaderDump/RepItemInfo.hpp`'s real declared layout — every
   single field matched in order, including the `-1.0` landing exactly on the `Pending?` field.
3. Wraps that into a complete `FContainerPickupsInfo` the same way (`UniqueServerID` via `GetUniqueID`,
   `IsContainer`, `ContainerDimension` zeroed, the `FRepItemInfo` embedded as `ItemInfo`, `ContainerMotherID`
   zeroed, `SlotIndex=0`, `Rotated=false`, `PickupRef=`the target actor) — again confirmed field-for-field
   against `research/CXXHeaderDump/ContainerPickupsInfo.hpp`.
4. Calls `AddNewItemToSlot(FContainerPickupsInfo& ItemInfo, FGuid ToContainer, int32 ToIndex,
   TArray<FS_ReplicatedContainerInfo>& ContainerContent, bool SetUID?, bool& Added, FContainerPickupsInfo&
   AddedItemInfo)` — the real declared signature, already sitting in the existing `BP_JigMultiplayer.hpp`
   CXXHeaderDump, matching the decoded 7-parameter call exactly. `ContainerContent` is the *matched container
   element's own* `ContainerItems` sub-array (`TArray<FContainerPickupsInfo>`), obtained via
   `KismetArrayLibrary::Array_Length` on a `StructMemberContext` read off the GUID-matched element from step 1
   — not the top-level array itself.
5. On success, fires the `JigMP_OnPickupEquipped` interface notification and the `CLIENT_EquipActorSuccess`
   client RPC.

**`TArray<FS_ReplicatedContainerInfo>` appears exactly once in the whole class: `MainJigContainers`.**
Cross-referencing `research/CXXHeaderDump/S_ReplicatedContainerInfo.hpp` confirms the real architecture:
equipment is **not** a dedicated array or property at all. `MainJigContainers` holds *every* container this
component tracks (backpack, ground loot, vendor, and — per this whole call chain — an "equip slot" is modeled
as just another container entry, identified by a reserved GUID, holding 0 or 1 item in its own nested
`ContainerItems`). This fully explains Session 45's original mystery: `SetEquippedInfoBySlot` was never wrong
about crashing or erroring, it was writing to a property that was never the real source of truth to begin
with — the actual authoritative state lives inside this nested container structure, mutated only through the
full `AddNewItemToSlot` call chain with all its GUID-matching and notification side effects.

Also dumped and decoded `HandleActorEquipped` (2,689 bytes, zero errors) expecting to find the visual-attach
logic there — it turned out to be pure bookkeeping too (array remove/clear, owner/durability/stats updates via
generically-named helpers), no `AttachToComponent`, no mesh/socket calls anywhere in either function.

### Live two-client test: full write chain succeeds end to end, zero visual result — chased 5 layers deep

Since proxy actors are locally-`SpawnActor`'d visual stand-ins with no real replication or authoritative state
of their own, decided the existing `SetEquippedInfoBySlot`/`SetActiveWeaponSlot`/`OnRep_ActiveWeapon` chain
(already implemented and enabled from Session 45, `kEnableEquipmentWrite = true`) was still worth testing
directly on the proxy, rather than replicating the full `MainJigContainers` mechanism — a proxy has no
Tick-driven logic that would re-derive/overwrite a manually-set display property from authoritative data the
way a real player's own character apparently does (the likely real explanation for why the write "didn't
persist" when tested against a real character in Session 45: something else keeps refreshing it from
`MainJigContainers`, not a failed write).

Set up a genuine two-client live test (local machine as PC1, a second physical Windows VM as PC2, both against
the local dev server, PC2's `session.cfg` gateway host manually overridden to the host's LAN IP since the
ticket endpoint always returns `127.0.0.1`). PC1 equipped a real AK15 into the Primary slot; checked PC2's view
of PC1's proxy.

Every call in the existing chain reported `ok=1` (getter, setter, `SetActiveWeaponSlot`, `OnRep_ActiveWeapon`)
— but the proxy remained a bare, unequipped body. Decoded `OnRep_ActiveWeapon` itself (112 bytes) and found
why: it calls `Owner->HasAuthority()`, and returns immediately *without* broadcasting the
`OnActiveWeaponSlotChanged` multicast delegate if true — a check meant to distinguish server/client for a real
replicated `OnRep` (which normally only ever fires client-side regardless). A locally-`SpawnActor`'d proxy,
never part of any replication graph, has local authority by definition, so this branch always trips, and the
delegate that actually drives the visual never fires. Every prior "success" was real — the calls just never
reached the one broadcast that mattered.

Found the delegate's real bound handler in `research/CXXHeaderDump/BP_PlayerCharacter.hpp`:
`OnActiveWeaponSlotChanged_Event_0(FGameplayTag Slot)`. Added `call_on_active_weapon_slot_changed()` in
`proxy_manager.cpp`, calling it directly on the character actor (not the `+0x700` helper component) to skip
`OnRep_ActiveWeapon`'s gate entirely. Live-tested: `ok=1`, still no visual.

Dumped `Event_0` itself (36 bytes) — another thin persistent-frame trampoline into yet another Ubergraph, this
time `ExecuteUbergraph_BP_PlayerCharacter` (205,862 bytes — the biggest function decoded so far), entry point
164582. Decoded that section clean (9,723 lines, zero errors). Found `MC_AttachClothing` called as a bare
`EX_VirtualFunction` partway through — a strong general lead (`MC_` is this codebase's `NetMulticast` RPC
convention, and UnrealHeaderTool splits those into a dispatch wrapper plus a `_Implementation` twin containing
the real body; calling the wrapper via `ProcessEvent` on a non-networked actor very plausibly just enters
network-dispatch code that silently no-ops without a real connection) — but it's specifically clothing-related,
not the weapon. Checking `research/CXXHeaderDump/BP_PlayerCharacter.hpp` directly confirmed there is **no**
`MC_AttachWeapon`-equivalent function at all, and no dedicated weapon mesh component among the character's
`USkeletalMeshComponent*` body-part fields either — ruling out a simple "set mesh + toggle visibility"
mechanism.

Found a second, more promising property instead: `bool PrimaryWeaponEquipped?` (plain, `@0x1DC0` directly on
the character actor, not through the helper), with its own `OnRep_PrimaryWeaponEquipped?()`. Decoded that (913
bytes, zero errors after fixing a real bug below) — no `HasAuthority()` gate this time; it branches directly on
the property's *own* current value, and the true-branch does real setup work (interface casts, calls
referencing the already-known Primary-slot tag literal `1730659`). Since nothing had ever written this
property, added `set_primary_weapon_equipped()` (direct raw write to the `0x1DC0` bool, same raw-offset
pattern as the existing `+0x700` helper-pointer read) plus `call_on_rep_primary_weapon_equipped()`, gated to
slot 11 specifically since that's the only slot with a real tag mapped so far. Live-tested: `set=1 rep=1`,
still no visual.

Added a final read-only diagnostic, `get_current_active_weapon()` (`BP_PlayerCharacter_C::
GetCurrentActiveWeapon(AActor*& EquippedWeapon)`), and logged its result after the entire chain. **Result:
`activeWeaponPtr=0x0` — no weapon actor exists at all**, after every single property/notification call in the
whole 5-layer chain reported success. This is the real answer to why nothing renders: none of this chain ever
spawns the actual physical weapon actor that a mesh/socket-attach step would need. For a real player, that
spawn almost certainly happens somewhere else entirely — plausibly tied directly to the `MainJigContainers`/
`PickupRef` data itself rather than to the `ActiveWeapon`-slot-change chain this session traced — and hasn't
been located yet.

### Found and fixed a real, unrelated mod bug along the way: invalid filename characters silently dropped bytecode dumps

`check_bytecode_dump_trigger()` built its output filename directly from the requested function name with no
sanitization. `OnRep_PrimaryWeaponEquipped?` (a real, correctly-typed function name — Blueprint authors in this
codebase routinely suffix boolean-returning names with `?`) produced an invalid Windows filename; `std::ofstream`
silently failed to open, and the trigger's own success-log line was unconditional, so it reported "wrote 913
bytes" for a file that was never created at all. Fixed by sanitizing only the output filename (not the
`className`/`funcName` strings used for the actual lookups) and making the log line reflect whether the
`ofstream` genuinely opened and wrote successfully.

### Infrastructure: two-client live-test loop with both machines fully scripted from one side

Standardized the whole cycle for this session's many rebuild/redeploy/relaunch iterations: fetch a fresh
ticket from the local dev server for each client (`POST :42201/v1/tickets`, LAN IP override for client 2's
`gatewayHost` since the endpoint always returns `127.0.0.1`), write `session.cfg` directly (locally, or via
`scp` for client 2), then launch — locally via `steam://run/1645820//`, and on client 2 via the existing
`SDBLaunchGame` scheduled task (`schtasks /run`, working around the Session-0 SSH-launch isolation documented
in Session 45) — all driven from the host machine's shell without needing hands-on-keyboard on client 2 at all
except to read its screen.

### Pivoted to spawning the visual directly — real actor now spawns and resolves correctly, still not visible

Rather than keep chasing the real game's internal actor-spawn logic (open-ended — could live anywhere reacting
to `MainJigContainers`/`PickupRef` replication, never found this session), implemented
`spawn_and_attach_weapon_visual()` in `proxy_manager.cpp`: read the equipped item's own
`JigsawItem_DataAsset_C::PickupClass` (`TSubclassOf<AActor>` @`0x0128`) and `EquipSocket` (`FName` @`0x0280`,
both from `research/CXXHeaderDump/JigsawItem_DataAsset.hpp`), spawn that class directly via the already-proven
`call_begin_deferred_spawn`/`call_finish_spawning` native-call pair (the same one `spawn_proxy()` uses, per
Session 40), then attach it to the proxy via `K2_AttachToActor`. Added a `RemotePlayer::primaryWeaponVisualActor`
/`primaryWeaponVisualItemId` pair (`state.hpp`) so it only respawns when the equipped item actually changes.

Live-tested end to end, real two-client test again:
- `PickupClass` resolved to a fully sensible, correct real class:
  `/Game/Inventory/Items/Pickups/Weapons/Firearms/Rifles/BP_AK15Pickup.BP_AK15Pickup_C`.
- Spawn succeeded (non-null actor pointer, no crash) — confirms `call_begin_deferred_spawn`/
  `call_finish_spawning` works for arbitrary item classes, not just the proxy character class it was proven for.
- `EquipSocket` resolved to `"Weapon_r"` — a legitimate, sensibly-named right-hand weapon socket.
- `K2_AttachToActor` was found via reflection and called with no crash.
- **Still no visual result.**

Checked `research/CXXHeaderDump/BP_AK15Pickup.hpp` (empty subclass) up through
`research/CXXHeaderDump/BP_FirearmPickup.hpp` and `BP_SkeletalMeshPickup.hpp` for what the Pickup actor itself
needs. Found `JigMP_OnPickupEquipped(AActor* ActorRef, FName ToContainerName, FGuid UID, FGuid ToContainerUID,
FRepItemInfo Info, bool& Result, AActor*& OverrideActor)` directly on `ABP_SkeletalMeshPickup_C` — the concrete
implementation of the exact interface call already seen fired from the real `ServerFuncHandleEquipActor` chain
earlier this session (`ci=1846532`). Added a call to it (with zeroed `UID`/`ToContainerUID`/`ToContainerName`,
matching the established "no real UID for a cosmetic proxy" pattern, and an `Info` populated with just
`ItemID`+`Count`) immediately after spawn, before the manual `K2_AttachToActor` call, on the theory that this
is the real "you've just been equipped, transition your visual state" notification and might do a more correct
attach internally.

Live-tested: found, called, no crash — but **`Result=false`, `OverrideActor=0x0`**. Not a missing function or a
crash this time, a deliberate rejection by whatever validation logic lives inside it, most plausibly rejecting
the synthetic zeroed `UID`/`ToContainerUID`/`Info` fields as not matching something it expects to be real
(consistent with everything else found this session about the real pipeline requiring a genuine prior pickup
event). Stopped here rather than decoding a sixth Blueprint class's bytecode in one sitting.

### Remaining work

- Decode `JigMP_OnPickupEquipped` itself to find exactly what makes it reject a synthetic call — most likely
  candidate given tonight's pattern: something checking `UID`/`ToContainerUID` against real, non-zero
  identifiers, or checking `Info`'s mostly-zeroed fields (only `ItemID`/`Count` were populated) for a specific
  required value.
- If that doesn't converge quickly: try feeding it fabricated-but-plausible non-zero `UID`/`ToContainerUID`
  values instead of zero, or try calling `K2_AttachToActor` targeting the spawned pickup's specific mesh
  component (via `GetMainSceneComp` — also found on `ABP_SkeletalMeshPickup_C` this session) rather than its
  root component, in case `"Weapon_r"` is a bone socket that only exists on that mesh, not the actor's root.
- Independent of the above: confirmed real, durable wins from this whole line of investigation regardless of
  the final visual outcome — `spawn_and_attach_weapon_visual()`'s spawn mechanism itself is proven correct
  (right class resolved, real actor spawned, zero crashes) for *any* item's `PickupClass`, not just weapons.
- Re-extract the other 20 slots' real `ComparisonIndex` values (still only Primary/slot 11 is mapped) — the
  `Jig.PlayerSlot.PrimaryWeapon`/`SecondaryWeapon`/`SidearmWeapon` tag name strings turned up incidentally
  while resolving FNames this session (`ComparisonIndex` 1730576/1730591/1730607, tightly clustered — plausibly
  sequential tag-registration order), worth checking directly against the already-known slot 11 value
  (1730659) before doing another full live IDA TMap walk.
- Solve the "unequip doesn't clear the proxy" gap noted in `sync_equipment()`'s comments, once the underlying
  write mechanism is fully understood — now further complicated by needing to also `K2_DestroyActor()` the
  spawned `primaryWeaponVisualActor` on unequip, which the current code already does on *change* but not yet
  on a clean unequip-to-empty transition.
- Try calling `MC_AttachClothing_Implementation` (if it exists) directly, bypassing the RPC-wrapper theory
  entirely, as a fast general test of whether the `_Implementation`-twin approach is right — not yet tried this
  session, still an open, cheap thing to check next time.

## Session 47 — Corrupted install traced and fixed, all 21 equipment slot tags found via a real TMap walk

### A launcher crash saga turned out to be a corrupted install, not mod code

Picked up mid-session to find the game stuck on a blank/loading screen indefinitely after the previous session's
work. Chased this for a long stretch — tried auto-dismissing `PressAnyKeyWidget_C` (found via `UE4SS`'s own
`GenerateSDK()`, triggered live with a hardware-level `SendInput` `Ctrl+H` since the earlier `SendKeys`/
`AppActivate` approach silently did nothing against the fullscreen game window), tried clicking an orphaned
`Play`-named bound event on `MenuWidget_C` (a classic Unreal quirk: the button was renamed to `NewGame` after its
event-graph node was already generated, leaving the stale `Play` name baked into the compiled function), tried a
generic `LoadingScreenWidget_C` dismiss — none of it reliably got past the screen, and one attempt produced a real
engine crash: `LowLevelFatalError ... Pure virtual function being called`, with a call stack 100% inside
`SurrounDead_Win64_Shipping` itself (no `UE4SS`/`main.dll` frames at all) showing an alternating recursive pattern
consistent with a runaway UI state machine — almost certainly the `Play` click bypassing the game's normal menu
navigation state.

Root cause was more fundamental than any of that: the game install itself was corrupted, most likely from the
many forceful `taskkill /F` calls across this whole multi-session investigation interrupting a file write at some
point. Fixed by deleting the entire Steam install directory and the `.acf` manifest, then reinstalling fresh via
`steam://install/<appid>`. Confirmed `UE4SS.dll`/`dwmapi.dll` are actually shipped as part of the official game
depot (both restored with their original Feb 2024 timestamps) — not something that needs sourcing from a
different game's UE4SS release or a separate RE-specific build, an assumption from earlier in the investigation
that turned out to be wrong and cost an extra crash cycle (a `UE4SS.dll` pulled from an unrelated game's bundle,
built against a different binary, crashed with `EXCEPTION_ACCESS_VIOLATION` deep inside `UE4SS`'s own init —
signature-scanning built for the wrong executable). Only our own `Mods/SurrounDeadBridge/` needs manual
redeployment after a reinstall.

**Correction on the automated-menu-click diagnosis**: the `ContinueGame` auto-click (`try_open_world()`,
unchanged since Session 45) was never the problem — confirmed by testing it in isolation, works reliably. The
real problems were (a) the corrupted install itself, and (b) new-this-session automation (`PressAnyKeyWidget_C`
auto-dismiss, the `Play` click) that either had no effect or actively broke things. Both were removed. A real
keypress at the splash is required going forward; `try_open_world()` only handles the menu stage, unchanged
from its original working form. Added `check_call_trigger()` (generic "find this class, call this zero-arg
function on it" flag-file action) as a reusable replacement for one-off dedicated dismiss functions, and
`check_mem_dump_trigger()` (raw qword dump from a helper-relative or absolute address, SEH-guarded) for the
TMap work below.

**Remote automation extended to client 2 as well**: since client 2 is a VM with no one physically present to
press a key at the splash, replicated the same hardware-level `SendInput` technique there via a second
interactive scheduled task (`SDBPressKey`, same `/it`/`/ru` pattern as the existing `SDBLaunchGame`) — confirmed
working, both clients reachable fully hands-off from the host machine's shell.

### All 21 equipment slot ComparisonIndex values found via a real TMap walk

Returned to the long-standing gap: only slot 11 (Primary) had a real `ComparisonIndex` (`kSlotTagComparisonIndex`
elsewhere was all zeros, safely no-op via `slot_tag()`). Rejected a tempting shortcut — resolving a range of
FNames and matching by name against `Jig.PlayerSlot.*` tags found incidentally in Session 46 — because that tag
family's values don't match the already-established-working slot 11 value (1730659 vs. this family's
`PrimaryWeapon` at 1730576), meaning it's a different, unrelated tag namespace (very likely the one used for the
active-weapon-slot UI switching traced in Session 46, not equipment-slot identity). Matching by name-similarity
risked silently picking a plausible-but-wrong tag for the other 20 slots, the same class of trap that cost hours
across the last two sessions.

Went to the real authoritative source instead: `research/CXXHeaderDump/BP_JigHelperComp.hpp` declares
`TMap<class FGameplayTag, class FS_EquipmentIDInfo> EquipmentIDSlotConfig;  // 0x0AF8 (size: 0x50)` — the actual
config `SetEquippedInfoBySlot`/`GetEquippedInfoBySlot` presumably validate against, keyed by the *real* tag
identity rather than a numeric slot index. `research/CXXHeaderDump/S_EquipmentIDInfo.hpp` gives the value type:
`FS_EquipmentIDInfo { FName ContainerName; bool IsEquipment; bool IsPersistentActor?; bool IsSecondary?; bool
UseLeaderPose?; }` (0xC/12 bytes).

Walked the live TMap via `check_mem_dump_trigger()`, empirically (not by trusting an assumed struct layout,
per this whole project's established methodology):
1. Dumped `helper+0xAF8` directly: `[ptr, ArrayNum=21, ArrayMax=22, bitmask=0x1FFFFF (21 bits set), ...]` — the
   `21`/`22`/21-bits-set triple lining up three independent ways was strong confirmation this was the right map
   before touching a single actual entry.
2. Followed the element-storage pointer (a separate heap allocation — required extending
   `check_mem_dump_trigger()` with an `abs <address> <count>` mode, since the existing helper-relative-offset
   mode can't express an arbitrary heap pointer) and dumped 256 raw qwords.
3. The pair's own natural size (`FGameplayTag` key, 8 bytes + `FS_EquipmentIDInfo` value, 12 bytes = 20 bytes) did
   *not* match the real per-element spacing — confirmed by testing every candidate stride from 8 to 200 bytes
   against the modular distribution of every plausible "large CI + small Number" hit across the whole dump
   (written as a small Python script over the raw dumped bytes) until one, 28 bytes, explained the data with no
   stragglers: 21 clean, real-looking entries (indices 0–20) followed by exactly one all-zero record (index 21,
   the map's one free/unallocated slot — matching `ArrayMax − ArrayNum = 1` precisely), then genuine unrelated
   heap garbage beyond that. The extra 8 bytes over the pair's raw size is presumably TSparseArray/allocator
   padding, never pinned down further since it didn't matter once the stride was confirmed working.
4. Resolved all 21 keys via the existing batch `resolve_fname` trigger (a couple of batches silently produced
   zero output despite valid input and a live, non-stale session — same unexplained-but-known flake from Session
   46; the established workaround, smaller batches, worked immediately every time) and matched every one by name
   against `protocol.hpp`'s already-documented declaration order (`slotIndex` 0–20: Facewear, Headwear, Eyewear,
   Accessory, Torso, Gloves, Legs, Feet, Container, BodyArmor, Backpack, Primary, Secondary, Sidearm, Melee,
   Throwable, Flashlight, Binoculars, GPS, Compass, FishingRod) — every single name matched cleanly with no
   ambiguity.

Replaced the old single-slot `kSlotTagComparisonIndex` (including the stale 1730659 Primary value, which — per
the discrepancy noted above — was very likely from that same wrong `Jig.PlayerSlot.*` family all along, not a
real regression) with all 21 freshly-verified values, live-tested immediately in a real two-client session:
`ok=1` on every slot on both clients (BrownHeavyJeans/Legs, Boots1/Feet — including a confirmed
`equipped=1` real-state read-back, Knife/Melee through the full activate/onrep/notify chain,
Binoculars/GPS/Compass/FishingRod), a complete, clean result across the board.

### Weapon-visual attach refined (not yet re-tested)

Revisited `spawn_and_attach_weapon_visual()` from Session 46. Dropped the `JigMP_OnPickupEquipped` call entirely
— it explicitly rejected the synthetic call (`Result=false`) with no way to know what it validates, and
`K2_AttachToActor` alone already showed nothing visually even *before* that call was added, so it was never
actually the blocker. Real suspect: `K2_AttachToActor` attaches relative to the target *actor's* root component,
almost certainly a capsule with no `Weapon_r` bone socket — switched to a component-to-component attach instead
(`weaponActor`'s own root component, via reflection-called `GetRootComponent()`, attached with `K2_AttachTo`
directly onto the proxy's `Arms` `USkeletalMeshComponent`, read via the same raw-instance-offset pattern used
throughout this file). Builds clean; not yet live-tested this session.

### Remaining work

### Weapon-visual attach: every mechanical step now confirmed successful, still zero visual result

Live-tested the `Arms`-component attach in a real two-client session immediately after writing this session's
"Remaining work" above. Found and fixed one real bug along the way (`GetRootComponent` isn't found via
reflection — matches the `K2_`-prefix convention already established for every other Blueprint-exposed engine
function used tonight; the real name is `K2_GetRootComponent`), then iterated through a full chain of
live-verified checks, each confirming success and each followed by "still no visual" from a live two-client
check:

1. `armsComp` resolved via `GetFullName()` to a genuine, correctly-identified `SkeletalMeshComponent` on the
   right proxy instance (`...BP_PlayerCharacter_C_2147479186.Arms`) — not garbage from a wrong raw offset.
2. `K2_AttachTo` returned `true` (added a `ReturnValue` field to the params struct at the correct trailing offset
   to actually check this, rather than assuming success from "no crash" the way earlier sessions repeatedly got
   burned).
3. Added `weaponActor->SetActorHiddenInGame(false)` on the theory that pickup actors spawn hidden by default
   until the real pickup flow reveals them (`SetActorHiddenInGame` is a plain direct binding already in the
   vendored stub, no reflection needed) — no change.
4. Tried `GetSkeletalMeshComponent` instead of the root, on the theory that `ABP_SkeletalMeshPickup_C` inheriting
   from the native `ASkeletalMeshActor` might mean the Blueprint's *root* had been reparented to something else
   (a collision/interaction volume) above the real mesh — not found via reflection, but logging what
   `K2_GetRootComponent` actually returns settled the question anyway: it resolved to
   `...BP_AK15Pickup_C_2147477446.SkeletalMeshComponent0` — already the genuine mesh component, ruling that
   theory out directly.
5. Called `PickupBuildFromGround()` (declared on `ABP_SkeletalMeshPickup_C`, research/CXXHeaderDump/
   BP_SkeletalMeshPickup.hpp) on the theory that it's the real initialization routine that configures the mesh
   asset from item data when a pickup naturally spawns from a "dropped in world" event, which our direct
   `BeginDeferredActorSpawnFromClass` spawn never triggers — found and called successfully, still no visual
   change.

Six consecutive confirmed-successful mechanical steps with zero visual result is a strong, clear signal to stop
this specific line of live trial-and-error — the real blocker is something this session's diagnostic style
(flag-file-triggered reflection calls, checking return values) can't surface on its own. The next real lead would
need actually decompiling `ABP_SkeletalMeshPickup_C`'s/`ABP_FirearmPickup_C`'s compiled Blueprint logic (the same
`kismet_disasm.py` technique used all through Sessions 45–46) to find whatever gates the mesh's actual visibility
or asset assignment, rather than more guessing at engine-level function names.

### Unequip-doesn't-clear-the-proxy gap fixed and live-verified

With the weapon-visual thread paused, picked up the other open gap instead: `sync_equipment()` only ever
iterated slots present in the latest `Equipment` wire frame, which omits empty slots entirely
(`read_local_equipment()`) — a slot going from equipped to unequipped simply vanished from the next frame rather
than arriving with an empty `itemId`, so nothing ever detected or cleared it on the proxy. Fixed by adding
`RemotePlayer::appliedSlotsMask` (a `uint32_t` bitmask, one bit per slot — 21 slots fits comfortably), set at the
end of every `sync_equipment()` call to the current frame's slot set. On each subsequent call, any bit set in the
*previous* mask but missing from the *new* one means that slot was just unequipped — explicitly calls
`set_equipped_info_by_slot(actor, i, "")` (empty `itemId`, matching `resolve_item_asset()`'s existing "empty
means unequip" contract) for each one, and additionally tears down `primaryWeaponVisualActor` if slot 11 was the
one cleared. This only became worth doing now that all 21 slots have real tags (Session 47, above) — with only
Primary mapped, the gap only ever mattered for one slot.

Live-verified in the same two-client session: had PC1 unequip their melee weapon and a torso item, both
immediately produced `SDB: equip-clear slot=14 ok=1` / `SDB: equip-clear slot=4 ok=1` on client 2's view of PC1's
proxy — clean, immediate detection and clearing for both a weapon slot and a clothing slot.

### Remaining work

- Decompile `ABP_SkeletalMeshPickup_C`'s/`ABP_FirearmPickup_C`'s own Construct/BeginPlay-equivalent bytecode to
  find what actually assigns the mesh asset and/or gates its visibility — the real next step for the weapon
  visual, now that six live-verified mechanical fixes in a row have ruled out every guessable engine-level cause.
- The 21 slot values are confirmed to work for the getter/setter/activate/onrep/notify/clear *data* chain
  end to end, including now proxy-clear-on-unequip — worth a visual check specifically for non-weapon slots
  (clothing), which don't depend on the still-broken weapon-visual chain above and might already be working via
  whatever native visual system handles `MC_AttachClothing`.
- `MC_AttachClothing_Implementation` still untried, per Session 46's note.

## Session 48 — Clothing visual sync: real success (3/5 slots), unlike the weapon dead end

Followed up on the previous session's note that clothing slots might already render via `EquipClothingToMesh`
without needing the weapon path's actor-spawn dance. `BP_PlayerCharacter_C::EquipClothingToMesh(FName ItemId,
AActor* ActorRef, USkinnedMeshComponent* ClothingRef, FName BodyPart)` is a plain (non-RPC) function, unlike
`MC_AttachClothing`/`Svr_AttachClothing` — called directly via `ProcessEvent` on the proxy with one of the
character's own pre-existing `Clothing_Torso`/`Clothing_Legs`/`Clothing_Feet`/`Clothing_Gloves`/`Clothing_Armor`
`USkeletalMeshComponent*`s (raw offsets from `BP_PlayerCharacter.hpp`: Feet=0x0760, Legs=0x0768, Torso=0x0770,
Gloves=0x0780, Armor=0x07B8) as `ClothingRef`, and the item DataAsset's own `ItemId` FName (same `+0x30` field
used throughout `read_local_equipment()`) — no actor spawning needed at all.

First pass left `BodyPart` zeroed (None), since its real expected value was unknown. Live result across both
clients: **Torso rendered correctly both directions** (WinterCoat on PC1's proxy seen from PC2, BlueShirt on
PC2's proxy seen from PC1) — immediately better than the weapon path ever achieved. Feet also rendered one
direction (MilitaryBoots on PC1's proxy) but not the other (Boots1 on PC2's proxy). Legs, Gloves, and Armor
never rendered on either proxy despite every call logging `ok=1` (function found, component non-null,
`ProcessEvent` invoked) — the same "mechanically succeeds, silently does nothing" symptom as the weapon dead end.

### Decompiling `EquipClothingToMesh` instead of guessing further

Used `check_bytecode_dump_trigger()` (built in an earlier session, `bytecode_dump.flag` with class+function name
lines, writes raw `UFunction::Script` bytes to a `.bin`) plus `kismet_disasm.py` to actually read the function's
logic instead of more trial-and-error. `EquipClothingToMesh` (551 bytes) does, in order: (1) a 3-param lookup
call on a fixed manager object using a fixed DataAsset reference and the item's `ItemId`, writing into a local
result struct, with an early-return if it fails; (2) branches on the character's own `IsPlayerMale?`-style
instance bool; (3) calls `BodyPartVisibility(Parts, IsPlayerMale, BodyPart, UpdateAllBodyParts)` on `ClothingRef`
— our own `BodyPart` input param is forwarded here, confirming it's not a dead parameter; (4)
`SetSkinnedAssetAndUpdate` on the clothing mesh itself using a mesh reference from the lookup struct; (5)
`K2_AttachToComponent` of `ClothingRef` onto the character's base `Mesh` component; (6) `SetOwner(Self)`.

Decompiled `BodyPartVisibility` itself (2800 bytes) to find what `BodyPart` values it actually branches on,
rather than guessing strings — found exactly four `EX_NameConst` literals, resolved via the existing
`resolve_fname.flag` mechanism: **"Torso" (ci=1732801), "Gloves" (ci=1732805), "Legs" (ci=1732809), "Feet"
(ci=1732812)**. No literal for "Armor" anywhere in the function — Armor apparently doesn't need naked-mesh
hiding at all (plausibly because it layers over torso clothing rather than replacing a skin region).
`UpdateBodyParts(FName Name)` (a related but distinct function, decompiled first while chasing this) recognizes
only three of the same four values (Torso/Legs/Feet, not Gloves) and uses them to swap the *naked* body mesh
components' `SetSkinnedAssetAndUpdate`+`SetMaterial` — a different, adjacent system, not the direct answer, but
its bytecode is what led to finding the real literal pool in the first place.

### Wired in the real `BodyPart` values — Legs now works, Gloves still doesn't

Passed the live-resolved `ComparisonIndex` values directly as `RawFGameplayTag{ci, 0}` (exactly like
`kSlotTagComparisonIndex` already does for equipment tags — no string→FName conversion available or needed,
since these are already-interned engine FNames read straight from the running process) for Torso/Gloves/Legs/
Feet; Armor still passes 0/None since no literal exists. Rebuilt, redeployed to both clients, fetched fresh
join tickets (see gotcha below), retested live.

**Correction (Session 49):** initially misread a screenshot as showing tan/khaki pants on PC2's proxy after
this fix and logged it as confirmed here — the user later corrected this directly: legs never actually
rendered, in either direction, at any point. What was visible in that screenshot was almost certainly just the
character's base skin-tone mesh, not equipped pants. Session 49's `DT_Clothing` row dump independently confirms
this was never going to work regardless of the `BodyPart` fix: neither `BrownHeavyJeans` (PC2's real legs item)
nor `BlackMilitaryPants` (PC1's) has a matching row in the table at all (only differently-named rows like
`HeavyJeans`/`MilitaryPantsBlack` exist) — so Legs has *never* been validated with an item actually capable of
rendering. The `BodyPart="Legs"` literal itself may well still be correct; it just hasn't been tested against
an item that could possibly prove it either way (would need e.g. `HeavyJeans` or `MilitaryPantsBlack`
specifically equipped to test cleanly).

Actual confirmed result from this pass, per the user's direct correction: **Torso** works reliably (both
`WinterCoat` and `BlueShirt` have real `DT_Clothing` rows and both render, both directions). **Feet** only
works one-directionally — `MilitaryBoots` (PC1's item, has a row) renders on PC1's proxy as seen from PC2;
`Boots1` (PC2's item, no row) does not render on PC2's proxy as seen from PC1 — consistent with the per-item
DataTable-row theory, not a direction-dependent code bug. **Legs and Gloves have never been confirmed to render
for any item tested** — every real item tested in either slot (`BrownHeavyJeans`/`BlackMilitaryPants` for Legs,
`BlackFingerlessGloves` for Gloves) turned out to lack a matching `DT_Clothing` row, so neither slot's code path
nor its `BodyPart` literal has actually been exercised against an item that could succeed. **Armor** likewise
untested against a matching item (`MilitaryPlateCarrier` has no row either).

### Gotcha: stale join tickets after a `taskkill`-based dll swap

Swapping `main.dll` requires killing the running game (Windows won't let you overwrite a loaded dll) and
relaunching — but the local dev server's join tickets are single-use per `worldId`, and the *old* session.cfg
ticket the mod read at its previous startup was already exhausted/rejected, producing an infinite
`[tcp] authentication rejected` retry loop on relaunch that looks identical to a real auth/network bug. Fixed
by always fetching a fresh ticket (`POST http://127.0.0.1:42201/v1/tickets`) and rewriting
`%APPDATA%\SurrounDeadBridge\session.cfg` for *both* clients immediately before each relaunch, not just once per
session — client 2 additionally needs its `SDB_GATEWAY_HOST` overridden to the host's real LAN IP (the
endpoint always returns `127.0.0.1`, per Session 46's note) written via a small scp'd helper script (inline
`ssh ... powershell -Command` with nested quoting for a nested `$cfgFile`-style nested-variable write reliably
broke the PowerShell parser — a real file + `-File` was far more reliable than fighting quoting through two
layers of remote-shell escaping).

### Remaining work

- Find out why Gloves (with the correct `BodyPart` literal) and Armor still don't render — most likely a
  per-item or per-slot content/data-asset gap rather than a code issue, given Legs just proved the same code
  path works once given the right `BodyPart` value. Decompiling the lookup function at `EquipClothingToMesh`'s
  step (1) (fixed object `0x64a20790`/`0x6654d780`-style addresses are process-instance-specific, would need
  re-resolving) would confirm whether the lookup itself is failing for these items/slots.
- Facewear/Headwear/Eyewear/Accessory slots still have no identified component/mechanism at all.
- The weapon-visual dead end (Session 46/47) remains unresolved and deferred, per its own note above.

## Session 49 — Gloves/Armor traced to a real content gap; local-equipment-restore is a second dead end

### Clothing: Gloves/Armor failure traced to DT_Clothing DataTable row lookup, not a code bug

Followed up on Session 48's partial clothing win (Torso/Legs/Feet render, Gloves/Armor don't) by decompiling
`EquipClothingToMesh` fresh (live pointers go stale across relaunches — re-dumped via `bytecode_dump.flag`
before each new analysis pass). Its real logic: (1) `UDataTableFunctionLibrary::GetDataTableRowFromName`
against a fixed `DataTable /Game/PlayerModel/DT_Clothing` asset, keyed by the item's own `ItemId` FName, with
an early-return if the row isn't found; (2) branches on `IsPlayerMale?`; (3) calls `BodyPartVisibility(Parts,
IsPlayerMale, BodyPart, UpdateAllBodyParts)` — confirming `BodyPart` (Session 48 wired in real literals for
Torso/Gloves/Legs/Feet, found via decompiling `BodyPartVisibility` itself) is genuinely consumed; (4)
`SetSkinnedAssetAndUpdate` on the clothing mesh from the DataTable row's mesh field; (5) `K2_AttachToComponent`
+ `SetOwner(Self)`. Identified all of this via `resolve_ptr.flag` against the bytecode's `EX_ObjectConst`/
`EX_FinalFunction` operands (found `DataTableFunctionLibrary`, `GetDataTableRowFromName`, and the `DT_Clothing`
DataTable object by address).

Walked `DT_Clothing`'s `RowMap` (a `TMap<FName, uint8*>`) directly via the same raw-TMap technique from Session
47's `EquipmentIDSlotConfig` walk — found the UObject header layout (vtable/flags/ClassPrivate/NamePrivate/
OuterPrivate, 0x28 bytes) empirically from a raw dump, then the TMap header immediately after at `+0x30`
(pointer/Num/Max, Num=80 confirmed by dumping the header's own qword), then walked all 80 elements at 24 bytes
each (`{HashNextId/HashIndex int32 pair}{FName ComparisonIndex,Number}{uint8* RowData}` — the `TSetElement`
layout TMap is built on) and resolved every row name via `resolve_fname.flag`.

Result: DT_Clothing has real rows for `WinterCoat`, `MilitaryBoots`, `BlueShirt`, `HeavyJeans` (and many other
generic clothing items), but **no row at all for `BlackFingerlessGloves`, `BlackMilitaryPants`, or
`MilitaryPlateCarrier`** (PC1's actual real equipped items in gloves/legs/armor) — the table does have
`MilitaryGlovesBlack`, `MilitaryPantsBlack`, and body-armor rows like `WoodenBodyArmor`/`ScrapMetalBodyArmor`/
`RiotPoliceArmor`, just under different names than what these specific items are actually called. This is a
genuine base-game content/data gap (some items were never registered in the clothing render DataTable, or were
registered under a different, inconsistent name) — not fixable from the mod side at all; `EquipClothingToMesh`
correctly no-ops for any item with no matching row, by design — this fully explains why neither of PC1's Legs
item (`BlackMilitaryPants`) nor PC2's (`BrownHeavyJeans`) ever rendered: **neither one has a matching
`DT_Clothing` entry**, so Legs has never actually been tested against an item capable of succeeding (a claim in
this log that `BrownHeavyJeans` rendered after the Session 48 `BodyPart` fix was a misread screenshot, corrected
by the user directly — see the correction note under Session 48's clothing section above). Torso (`WinterCoat`/
`BlueShirt`, both real rows) confirmed working both directions; Feet confirmed one-directional
(`MilitaryBoots`, a real row, works; `Boots1`, no row, doesn't).

**Legs confirmed working** later the same session: PC1 equipped `ParamedicPants` (a real `DT_Clothing` row,
from this session's candidate list) — `equip-clothing slot=6 itemId=ParamedicPants ok=1` synced to client 2,
and the user confirmed it actually rendered on PC1's proxy. This validates both the `BodyPart="Legs"`
(ci=1732809) literal and the whole per-item-DataTable-row theory directly: the exact same code that failed for
`BlackMilitaryPants`/`BrownHeavyJeans` (no matching row) succeeds immediately once given an item that has one.

**Conclusion: the clothing sync code itself (Torso/Legs/Feet/Gloves wired with real `BodyPart` literals, Armor
with none) is correctly built.** Torso, Feet, and Legs are now all directly confirmed working when the
equipped item has a real `DT_Clothing` row. Gloves and Armor remain unvalidated only for lack of a test item —
neither has been tried yet with a real-row item (`MilitaryGlovesBlack`/`RiotPoliceGloves` for Gloves;
`WoodenBodyArmor`/`ScrapMetalBodyArmor`/`RiotPoliceArmor`/`SpecOpsPlateCarrier` for Armor) — but given Legs'
result there's no remaining reason to expect they'd behave differently once tested with a matching item.

### Menu auto-click now gated on an actual bridge ticket

`try_open_world()`'s auto-click of `ContinueGame` previously fired unconditionally once `MenuWidget_C` was
found, regardless of whether this launch was even configured to join the bridge. Added a guard: `if
(cfg_join_ticket.empty()) return false;` before doing anything — a plain solo/offline launch (no session.cfg,
or an empty ticket) now leaves the menu alone instead of being yanked into a game via our automation. Direct
user request.

### Dead end: "Create New Save Slot" wipes local equipment, and we have no way to restore it

User discovered that using the game's own "Create New Save Slot" menu option resets the local player's
equipment while a bridge session is still connected — a real, deliberate save-system reset (matches
`UBFL_SaveGames_C::Reset Player Stats` found in `BFL_SaveGames.hpp`, part of the game's third-party
"EasyMultiSave"-based save plugin), not a bridge bug, but disruptive since bridge progress is meant to be
server-authoritative rather than locally saved.

First attempt: a "cache last-known-good local equipment, restore on full wipe" watchdog in `send_equipment()`,
using `set_equipped_info_by_slot` (the same function proven to work for proxies) via a new `restore_local_equipment()`
exposed from `proxy_manager.hpp`. Live testing (via a temporary `wipe_local_equipment.flag`/
`seed_local_equipment.flag` diagnostic pair that simulated the wipe/reseed without touching the real save
system, to avoid repeatedly triggering the real menu action which correlated with the game process closing
unexpectedly at least twice this session) revealed the fatal flaw: **`SetEquippedInfoBySlot` reports `ok=1`
(item resolves to a real asset pointer, function found and called) but has literally zero effect on the LOCAL
player's real equipped state or visuals** — confirmed by directly dumping the raw memory `read_local_equipment()`
reads from (`BP_JigHelperComp_C::ServerEquippedItems` at `+0x0F8`, per `BP_JigHelperComp.hpp`) before and after
a `SetEquippedInfoBySlot` call: every slot stayed exactly as it was, and the user confirmed no visual change
either. This is the same "mechanically succeeds, silently does nothing" failure mode that blocked the
weapon-visual work (Session 46) — `SetEquippedInfoBySlot` evidently only really works for *proxies* (unpossessed
puppet actors with no real network ownership, where it's presumably the only/authoritative equip path), not for
the real, network-owned local player character, where the actual authoritative equipped-state write must go
through a different, real pipeline.

Decompiled `BP_JigHelperComp_C::OnPickupEquipped(AActor* ActorRef, FName ToContainerName, FGuid& UID, FGuid&
ToContainerUID, FRepItemInfo& Info, AActor*& OverrideActor)` as the next candidate (found via
`BP_JigHelperComp.hpp`, right next to `SetEquippedInfoBySlot`/`GetEquippedInfoBySlot`, and matching the
`JigMP_OnPickupEquipped`-style naming already seen in the weapon investigation). Its bytecode (1121 bytes, full
clean decode) does an `EX_ObjToInterfaceCast` on `ActorRef` early on and drives the rest of its logic entirely
through that interface (`EX_InterfaceContext` calls) — meaning it needs a *real* pickup actor already in the
world implementing a specific container/pickup interface, not just a resolved item DataAsset pointer. This is
the same class of problem as the still-unsolved weapon-visual dead end (spawn a real actor, get every
interface/component exactly right, verify visually) rather than a simple reflection call — explicitly stopped
here per direct instruction rather than open another multi-session investigation with no guaranteed payoff.

**Reverted all of it** (the non-functional watchdog in `send_equipment()`, `restore_local_equipment()` and
`debug_wipe_all_equipment_slots()` from `proxy_manager.hpp`/`.cpp`, and the two temporary diagnostic flag
triggers in `mod.cpp`) — dead code that logs warnings but never actually restores anything is worse than no
code. The ticket-gated menu-click fix (above) was kept; it's real and unrelated to this dead end.

### Remaining work

- **No known fix for local-equipment loss from "Create New Save Slot"** — the only current mitigation is not
  clicking it while connected. A real fix needs either (a) a working `OnPickupEquipped` call with a properly
  spawned+configured pickup actor (same effort class as the weapon-visual dead end), or (b) finding a UE4SS
  UFunction-hook mechanism (none found in this project's vendored stub — `vendor/ue4ss-stub/include/RC/Hook/`
  only exposes an `AActor::Tick` hook, nothing generic for intercepting arbitrary UFunction calls) to actually
  prevent the save-reset menu action from running in the first place, rather than trying to undo its effect
  after the fact.
- Facewear/Headwear/Eyewear/Accessory slots still have no identified component/mechanism at all (unchanged
  from Session 48).
- **Legs confirmed working** (`ParamedicPants`, live-tested). **Gloves still unvalidated only for lack of a
  test item** — try `MilitaryGlovesBlack` or `RiotPoliceGloves`; given Legs' result there's no reason to expect
  it won't just work.
- **Armor tested three times, all three real-world items name-mismatched the DataTable** — every armor item
  PC1 had access to turned out to have a crafted/prefixed internal `ItemId` that doesn't match `DT_Clothing`'s
  row name: `RiotPoliceBodyArmor` (table has `RiotPoliceArmor`), `MakeshiftWoodenArmor` (table has
  `WoodenBodyArmor`), `MakeshiftMetalArmor` (table has `ScrapMetalBodyArmor`) — none rendered, confirmed live
  each time. Crafted "Makeshift X Armor" items apparently always get a `Makeshift`-prefixed `ItemId` that never
  matches the table's un-prefixed row names, unlike Legs where a plain item (`ParamedicPants`) happened to
  match exactly. The one untried candidate, `SpecOpsPlateCarrier`, wasn't accessible this session. Still
  genuinely unknown whether Armor renders at all given a matching item — and separately, whether it needs a
  `BodyPart` literal, since none was found in `BodyPartVisibility`'s bytecode for Armor specifically (current
  code passes `BodyPart=None` for slot 9). Try `SpecOpsPlateCarrier` (or any other exact-DT_Clothing-row-name
  item, not a "Makeshift"-prefixed one) next time it's available.
- The weapon-visual dead end (Session 46/47) remains unresolved and deferred, per its own note above.

## Session 50 — Legs confirmed via real item, Armor still name-mismatched, IDA reconnected, weapon-visual attempt #8 and PickupBuildFromGround ruled out

### Legs confirmed working with a real matching item

Session 49's correction (Legs never actually validated) got resolved this session: PC1 equipped `ParamedicPants`
(a real `DT_Clothing` row, ci=588300, from the 80-row list captured in Session 49) — `equip-clothing slot=6
itemId=ParamedicPants ok=1` synced to client 2, and the user confirmed it rendered. This directly validates
both the `BodyPart="Legs"` literal and the whole per-item-DataTable-row theory: identical code, only the item
differs, success or failure tracks the DataTable row exactly.

### Armor: three real items tried, three name mismatches, still genuinely unvalidated

Tried three armor items PC1 had access to, all failed the same way — each one's actual `ItemId` is a
differently-named variant of a real `DT_Clothing` row, not an exact match:

| Equipped item (real `ItemId`) | Table row it should have matched | Rendered? |
|---|---|---|
| `RiotPoliceBodyArmor` | `RiotPoliceArmor` | No |
| `MakeshiftWoodenArmor` | `WoodenBodyArmor` | No |
| `MakeshiftMetalArmor` | `ScrapMetalBodyArmor` | No |

Pattern: every crafted "Makeshift X Armor" item gets a `Makeshift`-prefixed `ItemId` that never matches the
table's plain row name. `SpecOpsPlateCarrier` is the one remaining untried candidate (not accessible this
session). Armor is still genuinely unknown — not proven broken, just never tested against a real matching item,
and separately still unknown whether it needs a `BodyPart` literal at all (none found in `BodyPartVisibility`'s
bytecode for Armor specifically; current code passes `BodyPart=None` for slot 9).

### IDA MCP reconnected after a fresh re-analysis

The IDA MCP server (`ida` in `~/.mcp.json`, bridging to `http://192.168.4.54:8744/mcp`) wasn't connected to this
session even though its config exists and the bridge container was already running — MCP server connections
are established when a Claude Code session starts, not dynamically mid-conversation, so simply having the
bridge/IDA running doesn't make the tools appear without a session restart. Once reconnected, `server_health`
confirmed `auto_analysis_ready` and `hexrays_ready`. The underlying IDA Pro GUI instance itself had to be
launched fresh (`ida.exe` against the current `SurrounDead-Win64-Shipping.exe`) and given time to auto-analyze
from scratch — the prior game reinstall (Session 47's corrupted-install fix) meant the old `.i64` database
was gone. Verified the fresh re-analysis produced **identical addresses** to what earlier sessions documented
(`0x140C9D940` still decompiles to the exact same `FName::ToString`-shaped hash-table-lookup code) — confirms
all previously-documented native addresses in this log remain valid without needing rediscovery.

### Weapon-visual attempt #8: the real native `EquipActorToSocket`, still no visual

Decompiled `BP_PlayerCharacter_C::OnRep_FacewearEquipped?`'s bytecode while investigating the Facewear/Headwear/
Eyewear/Accessory slots (see below) and found it calls, in order: `helper.GetEquippedActorBySlot(slot, false,
out Actor, out ArrayIndex)` → `ObjToInterfaceCast(Actor, BP_MpInteractInterface_C)` → `Interface.GetItemInfo(...)`
→ **`helper.EquipActorToSocket(ActorRef, IsSecondary)`** (the function's real name genuinely has spaces —
`GetFunctionByNameInChain(L"EquipActorToSocket")` fails, `GetFunctionByNameInChain(L"Equip Actor to Socket")`
succeeds) → `SetActorHiddenInGame`. Added `equip_actor_to_socket()` to `proxy_manager.cpp` and called it as an
additional step at the end of `spawn_and_attach_weapon_visual()`, on the theory that this is the actual
game-native attach function rather than a guessed engine primitive.

Live-tested: `equip_actor_to_socket: called` logs successfully (function found, `ProcessEvent` completes) — but
the user confirmed **still no visible weapon**. This is the 8th consecutive mechanically-successful,
visually-null attempt for weapon visuals, now including the real function the game itself uses for this exact
purpose elsewhere. Strongly suggests the blocker isn't attachment at all (six different attach approaches
across two functions have now been tried) but something about the *spawned actor itself* never completing
render/mesh initialization — which the next investigation (below) partially addresses but doesn't resolve.

### `PickupBuildFromGround` ruled out — it's HUD/outline setup, not mesh assignment

Long-held theory (since Session 46) was that `PickupBuildFromGround` — called on every spawn attempt, always
found, always "succeeds" — was responsible for assigning the mesh asset from item data, and that our synthetic
spawn triggering it out of its normal context might be why it silently no-ops. Decompiled it for real this
session using a new address-based bytecode-dump mode (below) and found it's a 18-byte stub that just jumps into
`ExecuteUbergraph_BP_SkeletalMeshPickup` at offset 363. Decompiling *that* segment (784 bytes, partial — the
disassembler hit an unhandled `EX_SetMap` opcode, 0x3B, not yet added to `kismet_disasm.py`) showed calls to
`SetRenderCustomDepth`, `SetAttachmentsCustomDepth`, `GameFunctionLibrary_C.GetHUD`, `KismetSystemLibrary.IsValid`,
and `SetOption` — this is entirely about the world-interaction outline/highlight effect and HUD prompt, **not
mesh or visibility assignment at all**. The whole "PickupBuildFromGround assigns the mesh" theory was wrong from
the start; calling it was never going to help, mechanically succeeding or not. The real mesh almost certainly
comes from the Blueprint's own default `SkeletalMeshComponent` property (baked into `BP_AK15Pickup_C` itself,
not assigned at runtime) — meaning the actual blocker is elsewhere, still unidentified.

Attempted to pin down the exact `SkinnedAsset` property's native offset via IDA reflection-table archaeology
(found the `SetSkinnedAssetAndUpdate` and `SkinnedAsset` strings and their xrefs, read surrounding qwords
looking for a literal offset constant) — inconclusive; UE5's native property/function registration tables need
proper type info (a PDB or manually-modeled UE5 structs in IDA) to navigate reliably, which this IDB doesn't
have. Stopped here rather than continue uncertain byte-level guessing, per direct instruction.

### New tooling this session

- `bytecode_dump.flag` now supports an `abs <hex address>` first line (instead of a class name) to dump a
  UFunction's bytecode using a raw live `UObject*`/`AActor*` pointer directly (e.g. one already logged
  elsewhere, like `spawn_and_attach_weapon_visual`'s `spawnedPtr=`) — needed because `FindFirstOf` proved
  unreliable at locating a specific attached/spawned actor instance among possibly-many instances of the same
  class (repeatedly returned "instance/CDO not found" for a `BP_AK15Pickup_C` actor confirmed alive and attached
  via its own logged pointer).
- Confirmed `Equip Actor to Socket` — like `Reset Player Stats` (Session 49) — is a real UFUNCTION name that
  genuinely contains spaces; the header-dump tool's word-spacing is not a display artifact, at least not always.

### Facewear/Headwear/Eyewear/Accessory: same actor-spawn pattern as weapons, not clothing's simple mesh-swap

Decompiled `OnRep_FacewearEquipped?` (see above) specifically to scope this investigation: these 4 slots are
represented by **spawned actors** retrieved via `GetEquippedActorBySlot`, then interface-cast and attached via
`EquipActorToSocket` — the same actor-based category of problem as weapons, not the simple `Clothing_*`
component mesh-swap that made Torso/Legs/Feet/Gloves/Armor tractable. Not pursued further this session — same
risk/effort class as the still-unsolved weapon-visual dead end, explicitly deferred rather than opened as a
second parallel unsolved investigation.

### Remaining work

- Weapon visuals: 8 attempts across two sessions, all mechanically successful, zero visual result. Next real
  step (if resumed) needs proper UE5 type info in IDA to navigate native property offsets reliably, or finding
  a way to compare a genuinely server-spawned pickup's component state byte-for-byte against our synthetic one.
- Facewear/Headwear/Eyewear/Accessory: not started (same actor-spawn problem class as weapons, deferred).
- Armor: needs `SpecOpsPlateCarrier` or another exact-row-name (non-"Makeshift") item to actually test.
- Gloves: still untested (no accessible item this session) but expected to work given Legs' result.
- Add `EX_SetMap` (opcode 0x3B) support to `kismet_disasm.py` if the Ubergraph investigation resumes.

### Weapon-visual attempts 9-11: AttachParent confirmed genuinely NULL, three more theories ruled out

Continued past attempt #8 (`EquipActorToSocket`, already ruled out above) using IDA's declared type info
(`AActor_RE`/`USceneComponent_RE`/`USkinnedMeshComponent_RE`, added to IDA's local type library this session
from `Engine.hpp`'s confirmed offsets — `AttachParent`@0xB0, `RelativeLocation`@0x128, `bVisible`@0x188 on
`USceneComponent`; `SkinnedAsset`@0x5B8 on `USkinnedMeshComponent`; `bHidden`@0x58, `RootComponent`@0x1A0 on
`AActor`) plus direct live memory reads (`mem_dump.flag`, following pointer chains: actor → `RootComponent` →
`AttachParent`) to investigate why the weapon never renders despite every mechanical step reporting success.

**Confirmed `SkinnedAsset` is correctly assigned**: read the weapon's `SkeletalMeshComponent0+0x5B8` live and
resolved the pointer — `SkeletalMesh /Game/Meshes/Firearms/Rifle/AK15/AK15.AK15`, the exact correct mesh. Fully
rules out "mesh never assigned," the theory that motivated the `PickupBuildFromGround` investigation above.

**Confirmed `AttachParent`@0xB0 is the right offset, and it's genuinely NULL after every attach attempt**: read
the *Arms* component's own `AttachParent` (a component we know for certain is properly attached, since it's a
permanent part of the character) and got a real, resolvable pointer —
`SkeletalMeshComponent .../BP_PlayerCharacter_C.Torso` — Arms is attached to Torso, exactly as expected. This
proves the 0xB0 offset itself is correct. Reading the *weapon's* `AttachParent` at the identical offset,
immediately after `K2_AttachTo`/`K2_AttachToComponent` both report `ReturnValue=true`, comes back NULL every
single time. The reported success is not real — attachment genuinely never takes effect, regardless of which
attach function is used.

Systematically tested and ruled out three more explanations for the silent failure, each confirmed via the same
live-memory-read method (not just theory):
- **`EquipActorToSocket` interference** (Session 49's addition) — removed entirely; `AttachParent` still NULL
  with just `K2_AttachTo`. (This alone was still worth doing — see below.)
- **Deprecated `K2_AttachTo` vs. modern `K2_AttachToComponent`** — switched to the non-deprecated 6-param
  version (`EAttachmentRule` × 3 instead of the packed `EAttachLocation::Type` byte); `AttachParent` still NULL.
- **Interference from `call_on_active_weapon_slot_changed`** — this call runs on *every* `sync_equipment()`
  pass (not gated on item change, unlike the spawn itself), and invokes the real game's own native equip-visual
  delegate handler; theorized it might be detaching our actor on each subsequent cycle. Temporarily skipped
  entirely — `AttachParent` still NULL. Restored (removing it provided no benefit and risked side effects on
  data unrelated to the visual).
- **Component `Mobility` not `Movable`** (a well-known real UE gotcha — `AttachToComponent` silently no-ops for
  non-Movable components) — force-called `SetMobility(Movable)` (found, called successfully) immediately before
  the attach; `AttachParent` still NULL. Removed.

**Current honest state**: 11 attempts total across three sessions (46/47/50), all mechanically successful
(correct components, correct mesh, correct confirmed-real offsets, functions found and reporting success), zero
actual attachment, zero visual result, and every plausible interference/precondition theory tested and ruled
out via direct live memory verification rather than inference. The genuine remaining possibility is that the
*true* return value of the attach call is being misread as true when it's actually false (would need a live
debugger attached to the game process to verify with certainty, not just static IDA analysis — the `ida-pro-mcp`
bridge used this session is static-only, no live process access) — or that some deeper native-side gate (e.g. a
network-role check on the attach path itself) silently no-ops for an actor with no owning connection, the same
general shape of problem as every `MC_`/`Svr_`-prefixed RPC dead end found throughout this whole project. Both
would need either live debugging or proper UE5 type/PDB info in IDA to resolve conclusively — genuinely beyond
what static binary analysis without those tools can settle. Deferred again, now with a much more precisely
bounded remaining unknown than before.

### Digression: how much multiplayer infrastructure already exists in the base game

Explained to the user mid-session, worth recording: the base game has substantial, real client/server
architecture already built in, just dormant in the shipped single-player build — `OnRep_`-callback replicated
properties throughout (`OnRep_ActiveWeapon`, `OnRep_PrimaryWeaponEquipped?`, `OnRep_FacewearEquipped?`, etc.),
`MC_`/`Svr_`-prefixed NetMulticast/Server RPC variants of many gameplay systems (clothing, sound, stamina,
montages), an entire dedicated `BP_JigMultiplayer_C` class, and an equip/interact system built around
`BP_MpInteractInterface_C` ("Mp" almost certainly = Multiplayer) rather than plain local state. This explains
the recurring pattern of `HasAuthority()` gates and RPC no-ops found throughout this project's whole history —
they exist because the shipped game has no real second network connection for them to do anything over, not
because the underlying systems were never built. Likely a planned or prototyped co-op mode that was disabled or
left unfinished before release.

### New IDA local types this session

Declared in IDA's local type library (via `declare_type`) for future use — real offsets from `Engine.hpp`, not
guesses:
```c
struct AActor_RE { char pad_0000[0x58]; unsigned __int8 bHidden; char pad_0059[0x147]; void* RootComponent; };
struct USceneComponent_RE { char pad_0000[0xB0]; void* AttachParent; char pad_00B8[0x70]; double RelLocX; double RelLocY; double RelLocZ; char pad_0140[0x48]; unsigned __int8 bVisible; };
struct USkinnedMeshComponent_RE { char pad_0000[0x5B8]; void* SkinnedAsset; };
```

## Session 51 — A whole day chasing "PC2 is slow" turned out to be a mod bug, not the VM

### The setup

PC2 (a Proxmox VM, VMID 102, NVIDIA RTX 3080 Ti via VFIO passthrough, accessed through Parsec) had been sitting at
~10 FPS. A full day was spent on VM-level tuning, all individually real and verified, none of it the actual fix:

- `cores` was over-allocated (12 vCPUs on an 8-core/16-thread host) — reduced to 8.
- `cpu` type was a generic `x86-64-v2-AES` baseline instead of `host,hidden=1` (loses real instruction set
  exposure and doesn't hide the hypervisor from the NVIDIA driver).
- Memory ballooning was active on an already memory-constrained host — disabled.
- `hostpci0` was missing `x-vga=1`.
- Background bloat inside the guest (Microsoft Edge, Windows Defender real-time scanning the game folder,
  Xbox Game Bar) was competing for the same 8 cores — cleaned up, Defender exclusion added.
- The GPU was stuck in P8 (idle power state, ~495MHz) even under load, confirmed via `nvidia-smi` — force-locked
  to 1800-2100MHz via `nvidia-smi -lgc`.
- Windows was on the Balanced power plan with default TSC/platform-clock settings — switched to High
  Performance, `bcdedit /set useplatformclock false` + `disabledynamictick yes` (a fix sourced from an unrelated
  Unraid forum thread about the exact same "low GPU utilization despite fine hardware" symptom).
- CPU affinity pinning (`qm set --affinity 0-7`) to reserve hyperthread siblings for the host.

Cumulative result of all of the above: ~10 FPS → ~17 FPS. Real, but nowhere near where a 3080 Ti should be.

### The actual bottleneck

The user mentioned having successfully run Diablo 4 on the same physical host before (via Hyper-V + GPU-P, a
completely different virtualization stack — not directly comparable) and, more usefully, that **Stationeers hits
~200 FPS on this exact VM**. That's the finding that mattered: if a different game gets excellent performance on
the identical hardware/passthrough/VM config, the VM isn't the bottleneck at all — something specific to
SurrounDead (or our own mod) is.

Isolation test, done properly in stages:
1. Fully vanilla SurrounDead (UE4SS's `dwmapi.dll` proxy renamed aside, no injection at all): **~170 FPS**. Proves
   the game itself, GPU, and passthrough are all fine.
2. Official UE4SS (matched to PC1's exact build via MD5, since a nightly build from a different source crashed
   the game outright — ABI mismatch against our stub headers) with **zero mods loaded**: **~130 FPS**. Proves
   UE4SS's own hooking overhead is acceptable.
3. UE4SS + **only** `SurrounDeadBridge` enabled: **~6 FPS at the main menu** — before even loading into the
   world. This nailed it down to our own mod's code, not UE4SS, not any other leftover dev mod
   (`PropertyDumper`/`JigMPHookTest`/etc. were also found still enabled on PC2 from earlier sessions and
   disabled, but weren't the actual cause either — this exact test isolated our mod specifically).

### Root cause: `find_local_pawn()` called unthrottled inside `do_game_tick()`

`do_game_tick()` itself is only rate-limited to once per 5ms (200Hz) via `g_last_tick_us` — reasonable for the
tick body as a whole, but two call sites inside it called `find_local_pawn()` directly on every one of those
200 ticks/sec with no additional throttle: the lazy-connect check and step 2's own pawn lookup.

`find_local_pawn()` is a UE4SS reflection scan (`FindFirstOf`) — cheap once a pawn exists (found quickly), but
worst-case (no pawn at all — sitting at the main menu, a loading screen, or briefly on death) it has to exhaust
the whole search before concluding "not found". Running that worst case 200 times a second is exactly the same
shape of bug already fixed once this project for the drop/pickup hook resolution's own `find_local_pawn()` calls
(`s_drop_fn`/`s_pickup_fn` in `mod.cpp`, fixed for a different reason earlier this same day) — throttling
*those* calls to 1/sec was done, but `do_game_tick()`'s own separate calls were missed entirely.

PC1 never surfaced this because it auto-clicks through the main menu in a few seconds (`try_open_world()`), so
the "no pawn yet" window is always brief. Anything that sits in that state longer — a slower-loading fresh
install, or this session's explicit menu-FPS test — fully exposes it.

**Fix**: added `cached_find_local_pawn()`, wrapping `find_local_pawn()` with its own 100ms throttle (independent
of `do_game_tick()`'s 5ms body throttle), and switched both `do_game_tick()` call sites to use it. 100ms is
still more than responsive enough for detecting pawn/death/respawn transitions.

**Result, live-tested**: ~6 FPS → 60-100 FPS on PC2, with the exact same VM config, exact same hardware, exact
same UE4SS build. All the VM-level tuning above likely still helped marginally, but this was the actual
dominant bottleneck the whole time.

**Lesson for next time**: when a "slow VM" complaint comes with a mod running, isolate the mod (or the whole
UE4SS injection layer) *before* spending a day tuning virtualization config, not after. A vanilla-game FPS
check and a stock-UE4SS-no-mods FPS check are cheap, fast, and would have pointed here almost immediately.

---

## Session 51 (continued) — the weapon/equipment-visual AttachParent mystery finally resolved

After the FPS fix above, picked the long-standing weapon-visual investigation back up. Three real bugs were
found and fixed in sequence, each one uncovering the next; the mystery that survived 11+ attempts across
Sessions 46/47/49/50 turned out to be three separate, independent problems stacked on top of each other.

### Bug 1: wrong target component (Arms, not the character's real root Mesh)

Every prior attach attempt targeted Arms (BP_PlayerCharacter_C's per-body-part SkeletalMeshComponent,
actor+0x788) - a real component, but not the one carrying the gameplay socket set. Confirmed two independent
ways: live DoesSocketExist(Weapon_r) on Arms returned false, and the FModel export of
SK_Chr_ToplessMale_01_Skeleton.json (the skeleton actually driving ACharacter's native root Mesh component,
CharacterMesh0 - wired up completely separately from the modular per-body-part meshes) confirmed Weapon_r
(and all the other gameplay sockets) live there instead. AttachToComponent silently no-ops when the socket
name doesn't exist on the target component while still reporting ReturnValue=true - exactly the "succeeds but
AttachParent stays NULL" symptom chased for three sessions. Fixed by reading the actor's own Mesh UPROPERTY
directly (GetMesh() isn't a reflected UFunction on this build) instead of Arms.

### Bug 2: EquipActorToSocket needs a real ItemDataAsset on the pickup, which our synthetic spawn never set

With the component fixed, a manual K2_AttachToComponent call started actually attaching for the first time -
but the visual still never rendered correctly. Pulled BP_JigHelperComp_C's real "Equip Actor to Socket"
function's property list from the FModel export (research/Exports/.../BP_JigHelperComp.json) instead of
guessing further: its locals show it calls GetComponentByClass(BP_JigPickupComponent_C) on the equipped actor
and a custom GetMeshFromOwner() on the owner, then a Select(Name) node (keyed on IsSecondary) feeding a
K2_AttachToComponent call. This means EquipActorToSocket - the real, game-native function used for this exact
purpose everywhere else - reads the equipped actor's own BP_JigPickupComponent.ItemDataAsset
(research/CXXHeaderDump/BP_JigPickupComponent.hpp @0x0A8) to resolve which socket to use. Session 50's
conclusion that EquipActorToSocket was "actively harmful" (it un-set AttachParent after running) was recorded
*before* Bug 1 was found - the manual attach it was "undoing" was itself already broken by the wrong-component
bug, so that conclusion was likely a misread. Fixed by writing the item's own UJigsawItem_DataAsset_C* directly
into the spawned pickup's ItemDataAsset field before calling EquipActorToSocket (set_pickup_item_data()).
Live-verified afterward via AttachParent reading a real, non-null, stable pointer, RelativeLocation reading
exactly (0,0,0) (a clean SnapToTarget snap), and K2_GetComponentToWorld() returning a sane in-world position
right at the character - all confirmed via a live bytecode_dump.flag capture of the real function plus
mem_dump.flag/resolve_fname.flag resolving its FField* operands' NamePrivate (UE5's FField layout: NamePrivate
FName at +0x20) to real property/socket names, rather than inferring anything from symptoms alone.

### Bug 3: pickups need to be explicitly told "you're equipped now, stop acting like a loose world item"

Even with a proven-correct attach, the item still visibly detached and fell through the floor a moment after
spawning. Ruled out (with live evidence each time): physics simulation on the pickup's own root (disabled via
SetSimulatePhysics(false), no change), KeepRelative/KeepWorld vs SnapToTarget attachment rules (the real
EquipActorToSocket bytecode was decoded to confirm LocationRule=2/RotationRule=2/ScaleRule=1/
WeldSimulatedBodies=true - genuinely correct), wrong socket name (resolved via the FField::NamePrivate
technique above - PrimaryUnequipSocket = "Mask", a real, valid, existing socket), and redundant
SetEquippedInfoBySlot resends re-triggering some internal validation (gated out, no change). The real cause:
ABP_SkeletalMeshPickup_C::JigSetCanInteract(CanInteract, EnablePhysics, Result) is the actual Blueprint-level
"this is now equipped, not a loose pickup" signal - our own SetSimulatePhysics call is a raw engine-level
override that doesn't stop the pickup's own internal tick/timer logic (CheckDistanceFromActor and friends on
BP_JigPickupComponent) from re-asserting "world item" behavior against it. Calling
JigSetCanInteract(false, false) right after the attach fixed it - confirmed live, held steady across repeated
re-equips.

### Bonus fix 1: EquipActorToSocket doesn't know about backpack state - do that part ourselves

Once weapons and facewear/eyewear were rendering correctly, a backpack + two weapons test showed one weapon
landing on the backpack and the other landing on the bare back/spine - not a bug in the fix above, just an
incomplete one. Equip Actor to Socket's own socket-select only branches on IsSecondary (PrimaryUnequipSocket vs
SecondaryUnequipSocket) - it never checks whether a backpack is actually equipped, so it always resolves to the
no-backpack socket ("PrimaryWeapon"/"SecondaryWeapon" on the skeleton) even when a backpack is worn. The
skeleton has genuinely separate, dedicated sockets for the backpack-worn case (confirmed via the
SK_Chr_ToplessMale_01_Skeleton.json FModel export: "PrimaryWeaponBackpack", "SecondaryWeaponBackpack", both
real), and JigsawItem_DataAsset carries the matching FName in PrimaryUnequipSocketBackpack (@0x290) /
SecondaryUnequipSocketBackpack (@0x3F8). Fixed by doing a second, manual K2_AttachToComponent re-attach to the
correct backpack-aware socket ourselves, right after EquipActorToSocket runs, whenever the proxy currently has
a backpack equipped (tracked via player.backpackVisualItemId).

### Bonus fix 2: per-item EquippedTransform rotation was never applied

Once every slot attached mechanically correctly (RelativeLocation consistently (0,0,0)), several items -
Eyewear/Glasses, the Backpack, the melee axe - still rendered at a visibly wrong orientation. Neither
EquipActorToSocket nor the manual re-attach above ever apply JigsawItem_DataAsset_C::EquippedTransform
(FTransform @0x0220) - a per-item transform confirmed via the FModel export of DA_BlackFaceMask.json to carry
a real non-identity Rotation. The socket's own bone rotation alone had been rendering, which happened to look
right for Facewear/Primary but not for others. Fixed by raw-writing RelativeLocation/RelativeRotation directly
on the item's root component (USceneComponent @0x0128/@0x0140) from EquippedTransform's stored quaternion -
converted to Euler via the standard UE FQuat::Rotator() formula - rather than risking a ProcessEvent call to
K2_SetRelativeTransform, whose FHitResult mid-parameter is undocumented in this project (the vendored SDK pads
FHitResult to a generous 256 bytes specifically for this reason, per RC/Unreal/Core.hpp).

### Final state, live-confirmed by the user

Weapon (Primary), Secondary, Sidearm, Melee, Facewear, Headwear, Eyewear, and Backpack all render correctly and
stay attached - including correct backpack-aware socket switching for both weapon slots and correct per-item
orientation. Only Accessory remains unwired (same spawn_and_equip_item_visual pattern should apply directly).

### Weapon attachments (scopes/grips/mags/etc.) - full wire sync implemented

Attachments turned out to be architecturally simpler than equipment once the actual data was found: each
attachment DataAsset carries its own Local_ActorClass (TSubclassOf<ABP_AMainLocalAttachment_C> @0x03D8) and
Local_AttachSocket (FName @0x0398) - a socket on the *weapon's own mesh*, not the character's - so no
GetMeshFromOwner/EquipActorToSocket involvement is needed at all, just a direct spawn + K2_AttachToComponent
onto the weapon's own root. The local player's actually-installed attachments live in
BP_JigPickupComponent_C::RepAttachments (FS_RepWeaponAttachment @0x0110 - research/CXXHeaderDump/
S_RepWeaponAttachment.hpp + S_RepAttachmentInfo.hpp), read off the real equipped weapon actor (found via
BP_JigHelperComp_C::GetEquippedActorBySlot, the same call proxy_manager.cpp already used for proxies, called
here on the *local* player's own helper instead).

This is the first feature this session that needed actual wire-protocol work (everything else was purely
client-side visual code) - added MsgType::WeaponAttachments (43) end to end: protocol.hpp/.cpp (flat
[weaponSlotIndex][containerIndex][itemId] entry list, same style as Equipment), mod.cpp
(read_local_weapon_attachments() + periodic 2s send alongside send_equipment()), the Node.js gateway (identical
client-authoritative relay pattern already used for Equipment - the server treats the payload as opaque bytes),
and proxy_manager.cpp (ProxyManager::sync_weapon_attachments(), spawning attachment actors onto whichever
proxy weapon visual actor currently exists, gated by a cheap itemId-signature comparison so an unchanged resend
doesn't respawn anything). Not yet live-tested end to end as of this write-up - built and deployed, server
restarted, but the game clients were closed before a live pass could confirm it.

### Lesson for next time

The FModel-exported .json for a Blueprint class's own compiled Function entries - even though it's just a flat
property/local-variable list, not a full disassembly with control flow - names every CallFunc_X local after the
actual function it calls, and typed ObjectProperty locals name their PropertyClass. This alone was enough to
determine the real call sequence and parameter types of EquipActorToSocket (GetComponentByClass,
GetMeshFromOwner, a Select(Name), K2_AttachToComponent) without any live decompilation, and turned out to be
far more reliable than continuing to guess from trial-and-error live tests. Use it *before* spending another
multi-attempt guessing session on a native/Blueprint function whose real behavior is unknown - it's static,
free, and doesn't require a live game process at all. When it isn't enough (no control-flow/branch info), the
live bytecode_dump.flag + kismet_disasm.py pair fills the gap, and its raw FField*/UFunction* operands can be
resolved to real names via mem_dump.flag reading FField::NamePrivate (+0x20) into resolve_fname.flag, rather
than left as opaque pointers.

### Weapon attachments live-tested end to end; two follow-up bugs found and fixed

Live-tested with a real loadout (AK15 with 5 attachments, plus a secondary and sidearm) — scope, grip, magazine,
suppressor, and laser/light combo all rendered correctly on the other client. Finding the local player's actual
equipped-item actors needed a different approach than planned: `GetEquippedActorBySlot` returns null for every
slot even when genuinely equipped (confirmed live — a `FindAllOf(BP_AK15Pickup_C)` scan proved a real actor
exists and is genuinely `AttachParent`'d to the character's own Mesh, so the getter itself just isn't the right
tool). Switched to walking `USceneComponent::AttachChildren` (Engine.hpp `@0x00C0`) on the character's own Mesh
directly instead, matching each attached actor back to a slot via its own `BP_JigPickupComponent.ItemDataAsset`.

Two bugs found while extending this to non-weapon slots (helmets, night vision):
1. **Crash from an unverified offset guess**: tried generalizing `BP_JigPickupComponent` lookup (only reliable
   at `owner+0x320` for firearms) via a reflected `GetComponentByClass` call, sourcing the needed `UClass*` from
   a guessed, never-verified `ClassPrivate` offset (`+0x10`) on a known-good component instance. Wrong guess,
   crashed the live game (`EXCEPTION_ACCESS_VIOLATION` reading `0x9006`) — reverted immediately. Helmet/glasses/
   backpack attachment support is deferred until the real offset (or another verified way to get the class) is
   confirmed, not guessed.
2. **Stale-pointer crash during a concurrent native unequip**: removing an attachment from a helmet in-game
   crashed twice with the identical exception address, both times with the debug log cut off mid-iteration near
   the end of the `AttachChildren` array — a native unequip action destroying/detaching a component while this
   scan was concurrently reading through the same array. Fixed by running the whole `AttachChildren` walk under
   an SEH guard (same trampoline pattern as this file's own `seh_invoke`/`destroy_actor_safe`) — a crash there
   now just discards that cycle's read instead of taking down the game. Confirmed live: no crash on a repeat of
   the exact same action.

**Lesson**: any raw memory read walking a live engine collection (`TArray`, `AttachChildren`, etc.) that isn't
gated behind the game's own thread-safety guarantees should assume the data can mutate mid-read from native
game logic running the same frame, and be SEH-guarded accordingly — this isn't unique to `AttachChildren`, it
applies to any live collection read triggered by our own polling rather than an in-response-to-event hook.

### Pawn appearance sync (gender/hair/beard) — proxies stopped being generic clones

Extended the mod so a proxy actually shows the real player's gender, hairstyle, hair color, and beard instead of
always spawning as a generic default `BP_PlayerCharacter_C`. Fields all confirmed via real header dumps
(`research/CXXHeaderDump/BP_PlayerCharacter.hpp`): `IsPlayerMale?` (`@0x15A0`), `HairMesh`/`BeardMesh` (two plain
`UStaticMeshComponent`s, `@0x7C0`/`@0x7C8`, whose current `StaticMesh` `@+0x5B8` is the chosen style), and
`Hair Color`/`Beard Color` material instances (`@0x15C8`/`@0x15D0`). Skin color was deliberately left out of this
pass — unlike hair/beard, it applies across many separate body-part meshes via a mechanism not yet
reverse-engineered.

Since assets like hairstyles aren't `JigsawItem_DataAsset_C` items with a wire-friendly `ItemID`, they're synced
by their own short object name (e.g. `"Chr_MaleHair3"`, taken from the tail of `GetFullName()`) and resolved on
the receiving end via `UObjectGlobals::FindObject`. Two real bugs found and fixed live:

1. **`FindObject`'s `InOuter=nullptr` doesn't mean "search everywhere"** — every lookup returned null even
   though `SetStaticMesh`/`SetMaterial` were both confirmed present via reflection. Real UE5's actual "search
   every package" sentinel for `StaticFindObject`-family calls is `ANY_PACKAGE` (`(UObject*)-1`) — a
   **documented public API constant**, not a guess at internal layout (unlike the `ClassPrivate` incident
   above) — `nullptr` for `InOuter` means "no outer at all," a far narrower search that excludes nearly every
   real asset. Fixed by passing `(UObject*)-1` instead. Confirmed live: all four lookups (hair mesh, hair
   color, beard mesh, beard color) started resolving successfully.
2. **Beard visibility overcorrection**: the beard component didn't render at all even once the mesh assignment
   started working, so `SetVisibility(true)` was added unconditionally after `SetStaticMesh` — but beards are a
   male-only customization in this game, and a female character's `BeardMesh` component can still have some
   placeholder mesh assigned while normally staying hidden. Forcing visibility unconditionally incorrectly
   revealed a beard on female proxies too. Fixed by explicitly hiding (not touching) the beard component when
   the synced appearance's `isMale` is false, gated separately from the mesh/color assignment.

Extended the same session to add `SkinColor` (`@0x15A8`) — unlike hair/beard's one dedicated component each, it
applies uniformly (`SetMaterial(0, ...)`) across all nine naked-body `SkeletalMeshComponent`s (`Torso`/`Arms`/
`Legs`/`Feet`/`Hands`/`head`/`Biceps`/`LowerThighs`/`LowerLegs`, all confirmed offsets from the same header
dump). No new bugs — the `ANY_PACKAGE` fix and the object-resolution pipeline already worked for hair/beard, so
skin color worked on the first live test.

### Appearance sync, continued: body shape, and the real character-creator options

User immediately caught a real gap: PC2's own character is female, correctly read/sent as such
(`isMale=0`, `hairMesh=Chr_FemaleHair6`), but the proxy on PC1's screen still looked male. Root cause: flipping
the `IsPlayerMale?` bool alone doesn't retroactively change which body-*shape* mesh a proxy — spawned once at a
fixed default gender — is using. Fixed by syncing the actual assigned `SkeletalMesh` (`SkinnedAsset @+0x5B8`,
same offset convention as `HairMesh`) for all nine body-part components too, rather than computing a target
mesh name from `isMale` + a naming convention — the male variants aren't uniformly named (e.g. Biceps is
`SK_Chr_Underwear_Male_01_Biceps`, not `SK_Chr_Male_Biceps`, confirmed via `pak_all_files.txt`), so reading the
real assigned mesh from source sidesteps that entirely. Applied via `SetSkinnedAssetAndUpdate` (the same
UFUNCTION `equip_clothing_to_mesh` already uses), *before* the `SetMaterial` skin-color call so the mesh swap
doesn't reset the material override.

User then asked about "nose and 3 others" they recalled from character creation. Rather than guess at a
morph-target system, checked the real `CharacterCreatorMenu` Blueprint FModel export directly
(`research/Exports/.../CharacterCreatorMenu.json`) — its function list (`HairType`, `BeardType`,
`EyebrowsType`, `MouthType`, `AccessoryType1/2/3`, `SkinColor`, `Sex`, plus non-visual `EditableTextBox_Age`/
`Forename`/`Surname`) confirmed there's **no morph-target/facial-sculpting system in this game at all** — every
option is a dropdown selecting one of ~15-28 preset meshes via `CallFunc_SetStaticMesh`, the exact same
mechanism already used for hair/beard. No `SetMorphTarget` call appears anywhere in the whole Blueprint. Added
`Mouth`/`EyebrowsMesh` (`@0x0740`/`@0x0790`, mesh-only, no dedicated color property) and `Accessory1/2/3`
(`@0x0758`/`@0x0750`/`@0x0748`) using the same mesh-swap pattern — the "nose and 3 others" the user
remembered was almost certainly `AccessoryType1/2/3`.

**Third bug found and fixed**: PC2's proxy showed a scar accessory PC2's real character didn't have. The
mesh-only sync loop only *applied* a mesh when the source had one (`if (meshName.empty()) continue;`), so a
proxy that spawned with some non-empty default accessory already assigned never got it cleared when the real
player had nothing in that slot — same root cause as the earlier beard-visibility bug, just for a different set
of components. Fixed by explicitly hiding (`SetVisibility(false)`) the component when the synced name is empty,
instead of skipping entirely.

**Lesson (recurring this session)**: "only apply when there's data" is not the same as "match the source" —
any sync loop needs an explicit empty/cleared case, not just a happy-path case, or a proxy's spawn-time defaults
leak through permanently. This bit beard visibility and then accessories in the same session; check for it
proactively in any future appearance-sync field.

Final state, live-confirmed by the user across three rounds of testing: gender (including actual body shape,
not just the bool), hair (mesh + color), beard (mesh + color, correct male-only visibility), skin tone, mouth,
eyebrows, and all three accessory slots all sync correctly and clear correctly when unset.

### Three more small, real bugs found and fixed the same session

1. **Dropped item stacks always showed as 1**: `EntityManager::spawn_entity_actor()` (world-loot/ground-item
   spawning, separate code path from equipment) never populated the spawned pickup's own
   `BP_JigPickupComponent.ItemDataAsset` or called `SetCount` at all — despite the wire protocol
   (`EntityDescriptorData.quantity`) already carrying the real quantity correctly. Fixed using the exact same
   reflection-based `GetValuePtrByPropertyNameInChain(L"BP_JigPickupComponent")` lookup already proven
   layout-agnostic tonight (works whether the pickup extends `ABP_SkeletalMeshPickup_C` or
   `ABP_StaticMeshPickup_C`), calling `SetCount` with the real `entity.quantity` instead of a hardcoded 1.
   Live-confirmed immediately.

2. **Unequipping clothing (Torso/Gloves/Legs/Feet/BodyArmor) never removed it visually**: `equip_clothing_to_mesh`
   only ever pushes a mesh onto the `Clothing_*` component — nothing ever reset it, so the unequip-clear block
   (which already handled every actor-spawn slot: weapons, facewear, backpack, etc.) had no case for these five
   at all. Same "sync needs a clear case" lesson as the appearance-sync bugs above, just for a system built in an
   earlier session before that lesson was learned. Fixed by hiding (`SetVisibility(false)`) the `Clothing_*`
   component on unequip, and re-showing it (`SetVisibility(true)`) in `equip_clothing_to_mesh` itself so
   re-equipping the same slot after a prior unequip doesn't stay invisible.

3. **Disconnecting a player left all their spawned visuals behind**: `on_player_disconnected` only ever called
   `destroy_proxy()` on the main pawn actor. Every weapon/equipment visual and weapon attachment is a *separate*
   spawned actor merely attached to the proxy — UE5 does not cascade-destroy attached actors when their parent
   is destroyed (only attached components die with their owner) — so a backpack, weapons, attachments, etc. all
   leaked into the world permanently on disconnect. Fixed by destroying every tracked visual/attachment actor
   (`destroy_actor_safe`, already SEH-guarded) before destroying the proxy itself. Live-confirmed via an Alt+F4
   disconnect — everything was cleaned up correctly.

## Session 51 (continued) — "respawn treadmill" mitigation, redundant equip-resync, and an unresolved clothing pulse

A screenshot at the very end of the prior entry showed PC1 seeing duplicated/stacked weapons and backpacks around
PC2's proxy. Root-caused across three layers, in order:

1. **Duplicate visual-actor pile-up ("respawn treadmill")**: `SDB.log` showed `slot=14 itemId=Knife` respawning
   with a brand-new actor pointer dozens of times in a row despite the itemId never actually changing. Mitigated
   (not root-caused) with a per-slot cooldown — `RemotePlayer::lastVisualRespawnUs` + `respawn_cooldown_ok()` in
   `proxy_manager.cpp` — capping any one slot's visual actor to one real respawn per 2s regardless of what the
   itemId comparison says.

2. **The real cause turned out to be one level up**: `sync_equipment`'s per-slot loop called
   `set_equipped_info_by_slot` + (for weapon slots) `set_active_weapon_slot`/`call_on_rep_active_weapon`/
   `call_on_active_weapon_slot_changed`, and (for clothing slots) `equip_clothing_to_mesh`, **unconditionally on
   every `sync_equipment` pass** — the wire resends a full Equipment snapshot every ~2s even when nothing
   changed, and this whole write pipeline had no "did this slot's data actually change" gate at all, unlike the
   `*VisualItemId` checks that already guarded the separate actor-spawn logic. Every ~2s resend re-fired
   `OnRep_ActiveWeaponSlot`/etc, which visibly re-triggers the game's own draw/holster handling — this is what
   read as "pulsing" once the pile-up mitigation stopped the actor-count symptom. Fixed with a new
   `RemotePlayer::appliedEquipItemId` map, gating the entire write block on `slot.itemId` actually differing from
   what was last applied. Confirmed via direct log diffing (`equip-setter`/`equip-activate`/`equip-onrep`/
   `equip-notify`/`equip-clothing` all went from firing every ~2s pass to firing exactly once per real change).

3. **That surfaced a third layer**: even with the gate above, the *same* slot (Torso/BlueShirt) still refired
   periodically. Traced to a genuine `equip-clear slot=4` sandwiched between two identical `BlueShirt` reads —
   PC2's own `read_local_equipment()` (a raw, unsynchronized pointer read off `BP_JigHelperComp`'s Equipped
   array) intermittently reports a still-equipped slot as empty for exactly one ~2s frame, a sender-side read
   race, not a receiver bug. Mitigated with a 2-consecutive-miss debounce (`RemotePlayer::missingSlotStreak`)
   before the unequip-clear path actually fires — a single-frame miss no longer triggers a real clear+reapply
   cycle. (Root-causing the sender-side race itself is still open — this is a receiver-side debounce, not a fix
   for why PC2's own read occasionally comes back empty.)

Despite all three fixes confirmed via log evidence (the equip write-pipeline genuinely stopped re-firing), the
user still reported visible "pulsing" specifically on shirt+pants. A Medal clip (`C:\Medal\Clips\SurrounDead`)
let this be inspected directly for the first time this session — frames extracted with `cv2.VideoCapture` (no
`ffmpeg` on this machine; the already-installed `opencv-python` read the `.mp4` directly) at ~0.15s spacing
showed the pants rhythmically alternating between their real tan texture and a dark/black appearance on a
roughly ~0.9–1.2s period, localized to the legs, in a spot with a sharp environmental shadow line — consistent
with the character playing a walk-cycle animation (legs swinging) while its actual position stays put.

**Attempted fix**: proxies are spawned as real `BP_PlayerCharacter_C` instances (same class as the local player,
confirmed via `spawn_proxy`'s `FindFirstOf`), so feeding the sender's real velocity
(already read in `mod.cpp`'s `send_movement` off `ACharacter::CharacterMovement@+0x328` /
`UMovementComponent::Velocity@+0xB8`, already wired through the wire protocol, just never applied on the
receiving end) into the *proxy's own* CharacterMovementComponent should let its already-existing AnimBP drive a
real walk/idle blend instead of sitting in a static default pose. Added a write in `ProxyManager::tick()` doing
exactly that, plus (once the raw feed proved to be the actual mechanism behind the clip's swinging-legs pattern)
a small deadzone so a standing-still player's non-exactly-zero residual velocity reading doesn't make the proxy
"walk in place" and cross the shadow line every stride.

**This crashed the mod outright.** Live-tested 2026-08-12: `SDB.log` showed `SDB: ready` immediately followed by
`SDB: unloaded` (twice) with no further activity — the game process itself stayed alive and responsive
(`Get-Process` showed `Responding=True`, no new crash dump), consistent with UE4SS catching an exception from
this write and uninstalling the mod rather than a hard process crash. Reverted the velocity-write block entirely
(kept `RemotePlayer::velocityX/Y/Z` being populated from the wire — that part is inert and harmless, just
currently unused). **Lesson, same family as the ClassPrivate incident**: `moveComp` being a non-null pointer
read off `proxyActor+0x328` doesn't guarantee it's safe to write through immediately after `spawn_proxy()` —
non-null is necessary but not sufficient; this needs actual verification (does the proxy's
CharacterMovementComponent exist/finish-initializing synchronously within `BeginDeferredActorSpawnFromClass`/
`FinishSpawning`, or does it lag a tick or more?) before retrying, not another blind offset write.

**Also discovered along the way**: after redeploying, PC1 appeared to be stuck in the same "ready → unloaded ×2"
loop even *after* reverting the crash — this turned out to be a false alarm. The mod was actually fine; the game
itself was sitting on the "Press Any Key" splash screen (per the standing project note: this screen was
deliberately left to require a real keypress, auto-dismiss attempts were dropped in an earlier session), and the
`SDB.log` tail was just stale content from the *previous* two relaunch cycles' genuine `on_uninstall()` calls,
not a live symptom. `debug.log` (which was still actively growing with `on_process_event_pre` entries) was the
tell that the mod was alive and just waiting on `find_local_pawn()`. Sent a keypress via
`[System.Windows.Forms.SendKeys]::SendWait(" ")` after `AppActivate`-ing the game window, which unstuck it
immediately. **Takeaway for future relaunch cycles**: if `SDB.log` looks stuck right after a relaunch, check
`debug.log` before assuming a crash — a live-but-un-poked splash screen produces the exact same "no new SDB.log
lines" symptom as a genuinely dead mod.

**End state**: actor pile-up and redundant equip-resync are both fixed and confirmed live. The shirt/pants
pulsing is still unresolved — confirmed *not* caused by either of those two bugs, most likely tied to the
complete absence of real animation driving on proxies, but the specific velocity-feed approach tried tonight
crashes the mod and was reverted. Next attempt needs to verify the CharacterMovementComponent's readiness timing
on a freshly-spawned proxy before writing to it again.

## Session 52 — weapon-in-hand attach: a real GameplayTag family, a stale FModel-export string, and per-item hand sockets

Reported bug: every equipped weapon visual (spawned via `spawn_and_equip_item_visual`/`equip_actor_to_socket`)
always renders in its *holstered* position — `EquipActorToSocket`'s own internal socket-select only ever
branches Primary vs Secondary `UnequipSocket`, with no concept of "currently drawn". A weapon never actually
moves to the hand when it becomes the active slot.

**The missing piece**: `JigsawItem_DataAsset_C::EquipSocket` (`FName @0x0280`, distinct from
`Primary/SecondaryUnequipSocket`) is exactly the in-hand socket — confirmed via the FModel export of
`DA_AK15.json`'s class defaults (`Exports/.../JigsawItem_DataAsset.json`: `"EquipSocket": "Weapon_r"`,
`"PrimaryUnequipSocket": "PrimaryWeapon"`) — but nothing had ever read or attached to it. Also confirmed a melee
weapon (`DA_Knife.json`) uses a *different* EquipSocket (`"MeleeWeapon_r"`) — per-item, not a single hardcoded
name, so it has to be read from each item's own DataAsset rather than assumed constant.

**Determining "is this slot currently active" required a second GameplayTag family.** The wire protocol had
never transmitted which weapon slot is actually drawn at all. `BP_JigHelperComp_C::GetActiveWeaponSlot(FGameplayTag&)`
is the real getter (research/CXXHeaderDump/BP_JigHelperComp.hpp) — repurposed the wire's dead
`Movement.animationState` byte (populated but never read anywhere) to carry the resolved slot index (11-14, or
0xFF) instead of adding a new protocol field.

The first attempt hardcoded this tag family's `ComparisonIndex` values, captured live by cycling through all 4
weapon slots in one game session (1730633/48/64/19 for Primary/Secondary/Sidearm/Melee — confirmed self-consistent
by cycling back to Primary and getting the same number again). **This broke on the very next relaunch** — the
same held weapon returned a completely different, unmapped CI (1730553) in the new process. Unlike
`kSlotTagComparisonIndex` (proxy_manager.cpp's equipment-identity tags, which have held up across dozens of
restarts all of last session), this "Jig.PlayerSlot.*" tag family's ComparisonIndex evidently isn't stable
across process restarts — different registration path/order, never fully explained. Fixed by resolving the
tag's real string name via `FName::ToString` (the same native call itemId strings already go through) instead of
comparing a baked-in number — `RawFGameplayTag`/`FName` share the same `{ComparisonIndex,Number}` layout, already
relied on elsewhere in this file for the backpack-socket case, so the tag's own address can be passed straight
into `fname_to_string`.

**FModel's own export was subtly wrong for one case.** `Exports/.../BP_JigHelperComp.json` lists this tag family
as `Jig.PlayerSlot.Primary/Secondary/Pistol/Melee` — matched string substrings "Primary"/"Secondary"/"Pistol"/
"Melee" against that. Live-tested: `GetActiveWeaponSlot()` actually returns `"Jig.PlayerSlot.SidearmWeapon"` for
the real sidearm slot at runtime, not anything containing "Pistol" — the static export's enumerated tag list
apparently isn't the exact string this specific function call returns (possibly a different context/dropdown
than the live getter). Live values win over static export when they disagree; fixed by matching "Sidearm"
instead once the mismatch was caught from a real "pistol doesn't show at all" repro.

**Re-attaching also had to reapply the per-item orientation correction.** `apply_item_equipped_transform`
(`JigsawItem_DataAsset_C::EquippedTransform`, `FTransform @0x0220`) was already being applied once at spawn time
in `spawn_and_equip_item_visual`, but a `SnapToTarget` `K2_AttachToComponent` re-attach resets relative
transform to identity each time — so re-attaching to the hand socket (or back to the holster socket when a
different slot becomes active) needs the same correction reapplied afterward, every time, not just once at
spawn. Extracted into a shared helper so both the original spawn path and the new hand/holster re-attach path
call it identically. Live-tested: melee weapon appeared in-hand with correct orientation immediately once this
was added (previously showed up in-hand but rotated wrong).

**End state, live-confirmed**: Primary/Secondary/Melee/Sidearm all correctly move to the hand socket when active
and revert to the holster socket when a different slot becomes active, with correct per-item orientation in both
positions.

## Session 52 (continued) — the real root cause of last session's shirt/pants "pulsing": a fourth clear-case gap

Finally root-caused the clothing pulse left unresolved at the end of last session (the velocity-feed/animation
theory was reverted after crashing the mod, and z-fighting was suspected but never confirmed). The user's own
theory — "maybe the game removes the underwear when a shirt goes on" — turned out to be exactly right, confirmed
by directly watching `send_pawn_appearance`'s live output while removing a shirt on PC2: `bodyParts[0]` (Torso)
went from empty to `SK_Chr_Female_Torso` the instant the shirt came off, and back to empty the instant it went
back on. The real game genuinely clears a body-part slot's own mesh when clothing covers it.

`sync_pawn_appearance`'s body-part loop (`proxy_manager.cpp`) already received this correctly over the wire, but
`if (meshName.empty()) continue;` meant it only ever *applied* a non-empty mesh and silently skipped empty
ones — never hiding anything. A freshly-spawned proxy's own default per-part mesh (e.g. an underwear-style
torso, matching the existing `SK_Chr_Underwear_Male_01_Biceps` naming already noted in this file) stayed
permanently visible regardless of what the real player had equipped, z-fighting against the `Clothing_Torso`
mesh layered on top the instant a shirt was applied — this is what read as "pulsing". Same "sync needs a clear
case" gap as the beard/accessory/clothing-unequip bugs from last session, just never caught in the body-part
loop specifically because nothing had directly compared its behavior against the real game's own live behavior
until now. Fixed with the same pattern as those three: explicit `SetVisibility(false)` when the source mesh name
is empty, `SetVisibility(true)` when re-applying a real mesh (in case a previous cycle hid it). Live-confirmed
fixed by the user immediately after deploying.

## Session 52 (continued) — retrying proxy velocity-feed animation: SEH doesn't help when the crash is downstream

Retried the velocity-into-CharacterMovementComponent approach reverted at the end of last session, this time
with two changes: (1) first confirmed via FModel export (`Player_AnimBP.json`'s `GetSpeed&Direction` function —
`CallFunc_VSize_ReturnValue` + `CallFunc_CalculateDirection_ReturnValue`, both standard velocity-driven Kismet
nodes) that Velocity genuinely is the right signal driving the walk/idle blend, not a guess; (2) wrapped the
write itself in the same `seh_invoke` SEH trampoline `destroy_actor_safe` already uses, specifically to survive
a repeat of last session's crash.

**The SEH guard did not help.** Live-tested 2026-08-13: PC2's entire game process crashed for real this time —
confirmed via a fresh `SurrounDead-Win64-Shipping.exe.*.dmp` in `CrashDumps`, not just the mod silently
unloading like last session's incident. This proves the fault isn't a synchronous access violation inside the
write call itself (which SEH around that call would have caught) — it's downstream, in some later engine tick
(animation update or movement replication) reacting to the now-nonzero `Velocity` on an actor that was never
possessed by a `PlayerController`. Reverted the call site again (left the now-unused
`apply_proxy_velocity_safe`/`do_apply_proxy_velocity` helpers in place, commented as not-currently-called, so
the FModel-verification work and the "SEH doesn't cover this" finding aren't lost if revisited).

**Where this leaves animation sync**: the AnimBP side is no longer in question (confirmed via FModel that it
genuinely wants Velocity). The blocker is specifically that `CharacterMovementComponent` on an unpossessed proxy
pawn can't tolerate having its `Velocity` written, for a reason not yet identified — a real fix needs either (a)
finding what internal state a possessed pawn's movement component has that an unpossessed one lacks and safely
populating just that (not blindly possessing the proxy, which would fight with teleport_proxy's own positioning),
or (b) driving the AnimBP's `Speed`/`Direction` values directly rather than through Velocity at all, bypassing
`CharacterMovementComponent` entirely — not yet investigated whether that's exposed as settable state.

**Second crash, same session, on a read-only diagnostic.** Went with option (a) — first tried the cheapest
possible test: a *read-only* `GetController()` ProcessEvent call on the proxy (zero writes at all), to check
whether the proxy has any Controller (AI characters have an AIController and animate fine; the proxy has
nothing). Live-tested 2026-08-13: PC2 crashed again — UE5's own in-engine crash reporter popped up this time
(not caught by a Windows crash dump in `%LOCALAPPDATA%\CrashDumps`, and this game's `Saved/Crashes` directory
doesn't currently exist on PC2 at all, so no dump was recoverable either way). Reverted immediately out of
caution. Not confirmed whether `GetController()` itself was the actual cause (a plain reflected getter call
crashing is a much stranger failure mode than a raw memory write crashing) or something unrelated coincided —
but two crashes in a row while touching proxy-actor movement/controller state in the same session is enough to
stop taking further live risks against the active session today. Animation sync is paused here; next attempt
should be tried far more cautiously (isolated/offline repro before running against a live session) rather than
continuing to iterate live.

## Session 52 (continued, next day) — a real breakthrough, a real deadlock, and a strong new lead

Picked animation sync back up. Rather than continue guessing at `CharacterMovementComponent`, went looking for
what `Player_AnimBP_C`'s own locomotion blend actually reads. Live value-correlation on the local player's
`__AnimBlueprintMutables` (reached via `GetValuePtrByPropertyNameInChain(L"__AnimBlueprintMutables")` on the
`AnimInstance`, a real named UPROPERTY even though its 53 individual fields are compiler-anonymized
`__FloatProperty_N` in this Shipping build's FModel export) found several plausible "Speed" candidates across
idle/walk/sprint/crouch-walk tests, but the theory broke on aim-walking (ADS while moving) — every 4-byte-float
candidate either read 0 while genuinely moving or a nonzero value while genuinely idle at some point.

**Went to real bytecode instead of more guessing**, per direct instruction ("remember to use fmodel output to
not guess" / "decompile the code"), using this project's existing `bytecode_dump.flag` + `kismet_disasm.py`
tooling:
1. Dumped `Player_AnimBP_C::BlueprintThreadSafeUpdateAnimation` — small (191 bytes), showed a call sequence of
   6 `EX_LocalVirtualFunction`s. Resolved their raw ComparisonIndex operands via `resolve_fname.flag` to real
   names: `GetThreadSafeBooleans`, `GetSpeed&Direction`, `GetHeadRot`, `GetAimOffset`, `GetLean`,
   `GetLeftHandLoc` — confirming `GetSpeed&Direction` really is called every frame (gated behind one early-out
   boolean check, not per-branch on aim state as originally suspected).
2. Dumped `GetSpeed&Direction` itself (169 bytes) — showed exactly two `EX_Let` instance-property writes:
   `VSize(velocity)` into one property, `CalculateDirection(...)` (cast) into another. This is the ground truth:
   Speed and Direction are each a single, unconditionally-written instance property, not context-dependent.
3. The bytecode only gives an `FProperty*` for that property, not its byte offset. Live-dumped raw memory at
   that property's own address (`mem_dump.flag`'s `abs <addr> <count>` form) and found `FField::NamePrivate`
   at the SAME `+0x20` offset already established elsewhere in this project — resolving it via
   `resolve_fname.flag` returned literally `"Speed"`, confirming the whole chain end-to-end. Further into the
   same dump found `ArrayDim=1`/`ElementSize=8` (an 8-byte **double**, not one of the 4-byte floats the earlier
   live-correlation pass was reading — explaining why nothing lined up cleanly before) and a plausible
   `Offset_Internal` value of `23232`.
4. Verified `23232` directly and empirically: read a `double` at `(AnimInstance object base) + 23232` on the
   LOCAL player live across idle/walk/aim-walk — `0.000` / `400.000` / `250.000`. Clean, sensible, and critically
   correct on the exact case (aim-walking) that had broken the earlier guess. High confidence.

**Wired it in** (`apply_proxy_speed_safe` in `proxy_manager.cpp`): compute `sqrt(vx²+vy²+vz²)` from
`player.velocityX/Y/Z` (already on the wire, no protocol change needed) with the same 15 cm/s idle deadzone as
the reverted Velocity attempt, write the double directly at `AnimInstance_base + kAnimBPSpeedOffset` — no
`CharacterMovementComponent`, no `Controller`, SEH-guarded regardless given the day's history.

**First live test**: PC2 crashed immediately on load, right as it would have first spawned PC1's proxy — no
Windows crash dump, no event log entry, matching the `GetController()` crash's dump-less pattern from
yesterday. Reverted.

**Re-enabled for further diagnosis** (explicit instruction, to find the actual cause rather than stop
guessing). **Second live test produced a genuine, diagnosable deadlock instead of a crash** — PC2's process
stayed `Responding=True` at the Windows level, but both `SDB.log` and `debug.log` demonstrably stopped
advancing (confirmed stale across a re-check, not just slow). `debug.log`'s last line: `equip_clothing_to_mesh:
about to ProcessEvent SetSkinnedAssetAndUpdate clothingComp=...` for the Legs slot (`BlackMilitaryPants`) — no
matching "ProcessEvent returned" ever appeared. The `ProcessEvent` call itself never returned.

**This is a strong, concrete lead, not just another guess**: `equip_clothing_to_mesh` (an entirely different
function) calls `SetSkinnedAssetAndUpdate` (`bReinitPose=true`) on the *same* proxy actor's skeleton/animation
system that `apply_proxy_speed_safe` was concurrently writing raw `AnimInstance` memory into, both driven off
the same `tick()` — but UE5 commonly evaluates animation on a separate Parallel Anim Update worker thread, so
"same `tick()`" doesn't guarantee "same thread" relative to whatever internal lock `SetSkinnedAssetAndUpdate`'s
reinit path takes. An unsynchronized write contending with or corrupting that lock is a very plausible
mechanism for a genuine deadlock, and would also explain the first test's crash-with-no-dump (a livelock
captured mid-transaction can manifest as an unrecoverable, dump-less failure too).

**Reverted again**, call site commented out with the finding preserved. Where this leaves things: the *value*
(Speed, at `AnimInstance_base + 23232`) is now solidly verified correct — that work doesn't need to be redone.
The remaining problem is purely about *how* to write it without racing the engine's own animation/skeletal-mesh
update path. Next attempt should establish whether this game's `AnimInstance` actually runs Parallel Anim
Update (and if so, find the right synchronization or a write path that doesn't contend with it), rather than
retrying the same unsynchronized write a third time.

## Session 53 — gloves clothing pulse, aim-offset investigation, body-yaw fix, movement interpolation

**Gloves-flashing fix**: same "sync needs a clear case" bug class as the earlier Torso clothing pulse this
session, applied to the Gloves slot specifically. The bare "Hands" body-part mesh (`kBodyPartOffsets` index 8,
offset `0x7B0`) was never hidden when Gloves clothing (slot 5, `clothingOffset=0x0780`) was equipped, so it
z-fought against the Clothing_Gloves mesh — invisible while the proxy stood still, only became visually obvious
once animation sync started working (walk-cycle deformation made the static flicker much more noticeable). Fixed
with the same paired hide-on-equip/show-on-clear pattern already proven for Torso: hide the Hands body-part
`SetVisibility(false)` when Gloves applies successfully; re-show it in the existing `clearClothingOffset`
unequip block. Confirmed fixed live via an equip/unequip test cycle.

**Aim/look-direction sync — extensive investigation, real progress, still not visibly working.**

First attempt: wrote the sender's real camera pitch (`GetControlRotation()` via reflection on the local pawn,
quantized to a byte — same [0,255) scale UE's own `RemoteViewPitch` uses) into the proxy's own native
`ACharacter::RemoteViewPitch` property. No visible effect. Root-caused via full bytecode tracing (see below):
this game's AnimBP doesn't read `RemoteViewPitch` at all — that was an assumption from general UE knowledge, not
verified against this specific game, and the user correctly called this out ("did you check the fmod exports?")
before more time was sunk into it.

**Full `GetAimOffset` bytecode trace** (this game's `Player_AnimBP_C::GetAimOffset(DeltaTime)`), using the
existing `bytecode_dump.flag`/`kismet_disasm.py` toolchain plus two new diagnostics added this session:
- `resolve_fprop.flag`/`check_resolve_fprop_trigger` — resolves a raw `FProperty*` pointer (an
  `EX_InstanceVariable`/`EX_LocalVariable` bytecode operand) to its declared name by reading `FField::NamePrivate`
  at `prop+0x20` and calling `native::fname_to_string` directly. Needed because `resolve_ptr`'s `GetFullName()`
  approach only works on real `UObject`-derived pointers — UE5's Field system makes `FProperty` an `FField`, not a
  `UObject`, so calling a UObject vtable method on one access-violates. An earlier version of this trigger
  (`check_resolve_propname_trigger`, briefly added and replaced) tried comparing bytecode operand pointers against
  `GetValuePtrByPropertyNameInChain`'s return value — a category error: the bytecode operand is a pointer to the
  property *descriptor*, not the value's storage address, so they were never going to match regardless of name.
- A live-value watcher (`watch_aimoffset.flag`) reading the resolved properties' actual values off both the local
  player and a proxy once per second, to see what really varies with camera movement vs. what's fixed/garbage.

Found: `GetAimOffset` does
`SelectRotator(K2Node_PropertyAccess_9, K2Node_PropertyAccess_10, K2Node_PropertyAccess_8) -> NormalizedDeltaRotator(_, K2Node_PropertyAccess_11) -> MakeRotator -> RInterpTo -> BreakRotator -> write Pitch/Yaw`.
`RInterpTo`'s `InterpSpeed` is a hardcoded `0.0` literal — dead smoothing, per UE's own implementation this makes
it return `Target` unconditionally. Live sampling on the **local player** (`bSel8` always `1`/true) showed
`K2Node_PropertyAccess_9` tracking real, continuously varying camera rotation (`GetControlRotation()`-like) and
`_10` sitting close to actor/body rotation (the untaken branch). Live sampling on the **actual proxy**
(`bSel8=0`/false, confirming a `SelectRotator` condition that reads false for anything not locally controlled,
regardless of whether it has a `Controller` — this ruled out "give the proxy a Controller" as a fix, since
`IsLocallyControlled`-style checks are about *whose local machine* possesses the pawn, not merely having *any*
controller) showed `_10`/`_11` both **exactly 0.0**, deterministically, on every sample, no exceptions —
`GetAimOffset` hard-resets `Pitch` to `NormalizedDeltaRotator(0,0) = 0` every single frame for a
non-locally-controlled proxy in this game. (An earlier sample taken on a stale/older proxy connection showed
`_10` jumping wildly rather than sitting at a clean zero — inconsistent with a deterministic reset, most likely
an artifact of sampling a proxy mid-transition rather than genuine uninitialized memory; the fresh-proxy result
is the one to trust.)

Since `GetAimOffset` runs every render frame and unconditionally overwrites `Pitch`, a same-tick write (from
`ProxyManager::tick()`, throttled to ~5ms via `do_game_tick`) can never win that race — confirmed live, direct
writes had zero visible effect. **Found a real (non-patch) fix for the race**: verified live via `GetProcAddress`
against the actual on-disk `UE4SS.dll` that `RegisterProcessEventPostCallback` exists in this build (same mangled
name pattern as the already-used `RegisterProcessEventPreCallback`, with Pre substituted for Post) — an earlier
comment elsewhere in `mod.cpp` claiming "no post-callback resolved for this UE4SS build" was simply never
actually tested for this specific symbol. Registered `on_process_event_post` (`mod.cpp`), which does a cheap
`func == s_getAimOffsetFn` pointer-compare fast path (skips ~every ProcessEvent call in the game), then on a
match resolves the calling AnimInstance's owning actor (`GetOwningActor()`) and, if it matches a known proxy,
writes `Pitch` immediately after the real `GetAimOffset` call completes — structurally this should always win
cleanly (last writer for that frame) rather than fight every frame. Removed the now-redundant tick()-based
`apply_proxy_aim_pitch_safe` call and its helpers from `proxy_manager.cpp`.

**Deployed and tested live — still no visible effect** ("nothing"). The post-callback registered without error
and the mechanism is structurally sound (verified export exists, correct signature, correct fast-path filter),
but the end-to-end result wasn't visually confirmed working. Did not further diagnose before the session moved
on to body-yaw (a real, confirmed win) at the user's request — worth a proper write+readback check next time
(confirm the post-hook is actually firing for the proxy's `GetAimOffset` calls at all, and that the `Pitch`
write is actually landing and surviving to the next render) before assuming the post-hook itself is broken vs.
some other, still-undiagnosed piece (e.g. the AnimGraph's own `AimOffset` blend space node might read Pitch/Yaw
through a different, cached path than the raw instance property, similar to the `__AnimBlueprintMutables`
indirection seen for `Speed` earlier this session).

**Body rotation (yaw) — real, confirmed win.** Live sampling (`watch_rotation.flag`) comparing `player.yaw` (what
we send) against the proxy's actual `K2_GetActorRotation()` showed `actualYaw` pinned at exactly `0.00`
regardless of what `teleport_proxy`'s `K2_SetActorLocationAndRotation` call sent — rotation was silently a no-op
the whole time location worked fine. **Root cause confirmed via FModel export** (not guessed):
`BP_PlayerCharacter`'s `CharacterMovementComponent` CDO has `bOrientRotationToMovement: true` and
`bUseControllerRotationYaw: false` — the classic UE mechanism that resets the actor root's rotation from
`Velocity` every physics tick, silently overriding any external `SetActorRotation`/
`K2_SetActorLocationAndRotation` call. Since we also write real `Velocity` onto the proxy (for animation), this
was actively fighting our explicit rotation write every tick.

Fix: write yaw onto the proxy's own `Mesh` component's `RelativeRotation` instead of the actor root —
`bOrientRotationToMovement` only touches the actor root, not the mesh's separate relative transform. Same raw
`USceneComponent::RelativeRotation @ 0x0140` offset already proven safe for weapon-transform application
elsewhere in `proxy_manager.cpp`. Captured the mesh's own baked-in art-alignment offset (`RemotePlayer::
meshBaseline{Pitch,Yaw,Roll}`, typically ~-90° yaw for a UE mannequin-based character) once on first use and
added the desired body yaw on top of it, rather than overwriting it outright (which would have rendered the
proxy rotated 90° off). Confirmed working live.

**Movement interpolation.** With body rotation fixed, movement looked "teleporty" — `teleport_proxy` hard-snaps
position every `do_game_tick` (~5ms) to whatever `RemotePlayer::x/y/z` currently holds, which only changes once
per received network packet (~50ms, per `SDB_MOVE_INTERVAL_MS`), so between packets it kept re-snapping to an
unchanged value, then jumped instantly on the next one. Added `RemotePlayer::render{X,Y,Z,Yaw}`, exponentially
smoothed toward the raw `x/y/z/yaw` each tick (`update_proxy_render_smoothing`, time-constant `kTau=0.08s`,
tuned around the ~50ms packet interval) rather than a fixed-duration lerp, so it self-corrects regardless of
actual packet jitter. Yaw interpolation handles the ±180° wraparound explicitly (shortest-angle delta). A
distance-based snap (`kTeleportDistSq`, 500 units) bypasses smoothing entirely for a genuine teleport/respawn, so
those don't visibly slide across the map. Confirmed working live ("Thats a lot better!").

**Reusable lessons for next time**:
- `RegisterProcessEventPostCallback` **does exist** in this UE4SS.dll build — don't trust the old "no
  post-callback resolved" comment without re-testing; it was apparently never actually checked for this specific
  symbol, just assumed.
- When an explicit reflection-based property/rotation write has no visible effect, check whether a native
  Component (most commonly `CharacterMovementComponent`) is fighting it every physics tick before assuming the
  write itself is broken — `bOrientRotationToMovement`/`bUseControllerRotationYaw` are the two properties to
  check first for rotation specifically, findable in a Blueprint's CDO property dump in FModel's JSON export
  without needing any live testing at all.
- Writing onto a child **component's** own relative transform (Mesh's `RelativeRotation`, same offset already
  proven for weapon transforms) is a reliable way to route around an actor-root-level native override, so long
  as the component's own existing baseline offset is preserved rather than clobbered.
- Exponential/time-constant smoothing (`1 - exp(-dt/tau)`) toward a periodically-updated network target is a
  simple, jitter-tolerant fix for "teleporty" proxy movement — no need for precise two-point timestamp
  interpolation given this project's tolerance for a small amount of visual lag.

## Session 53 continued — crouch/ADS/falling sync, and the real reason ProcessEvent post-hooks kept failing

Found three more directly-settable, plain (non-bitpacked — `bIsNativeBool: true`, `FieldMask: 255`, each its own
dedicated byte) `BoolProperty` class members on `Player_AnimBP_C`, in the same property block as `Pitch`/`Yaw`:
`IsCrouching`, `IsADS`, `Falling`. Confirmed via FModel export before writing anything, same discipline as the
rotation fix. Repurposed the wire's `Movement.movementState` byte (confirmed dead the same way `animationState`/
`aimState` were earlier this session — never populated on send, never applied on receive) as a 3-bit flag byte
(`0x01`=crouching, `0x02`=ADS, `0x04`=falling), read on the sender via the same `Mesh->GetAnimInstance()`
reflection path already used for `Pitch`.

First attempt applied the three flags directly from `ProxyManager::tick()`, same as the very first (failed)
`Pitch` attempt. Live result: crouch **jittered** (won some frames, lost others — a genuine race, distinct from
`Pitch`'s clean deterministic loss), jump/ADS showed **no effect at all**.

**Extended the existing `Pitch` post-callback fix to cover these three too — still no effect, and this time a
proper diagnostic revealed why the whole post-callback approach was subtly broken from the start, not just
insufficiently targeted.** Rather than picking a single per-frame function to hook (as `Pitch`'s fix had hooked
`GetAimOffset` specifically), reasoned that hooking the *last* function in `BlueprintThreadSafeUpdateAnimation`'s
known per-frame call sequence (`GetThreadSafeBooleans -> GetSpeed&Direction -> GetHeadRot -> GetAimOffset ->
GetLean -> GetLeftHandLoc`, from earlier bytecode tracing) and reapplying every proxy override there would
guarantee last-writer-wins for the whole block regardless of which specific sub-function owns which property.
Implemented, deployed, tested — user reported "still all the same," no change at all.

**Added real diagnostics instead of guessing again** — logged (a) whether `s_lastUpdateFn` (the resolved
`GetLeftHandLoc` `UFunction*`) was ever non-null, and (b) whether the post-hook's `func == s_lastUpdateFn` match
ever fired, unconditionally, regardless of proxy match. Result: resolution succeeded, but the match **never fired
even once**, despite `GetLeftHandLoc`'s own effects clearly happening every frame (proven by `Speed` correctly
reflecting `Velocity` every frame all session). This is the real, previously-unverified gap flagged in this
log's own "Reusable lessons" section further up — the earlier `Pitch` post-hook fix was deployed and tested
("nothing") without ever confirming the hook itself fired for a matched proxy, so its failure was never actually
diagnosed at the time; it was silently the same bug as this one.

**Root cause**: `BlueprintThreadSafeUpdateAnimation` calls `GetThreadSafeBooleans`/`GetSpeed&Direction`/
`GetHeadRot`/`GetAimOffset`/`GetLean`/`GetLeftHandLoc` via `EX_LocalVirtualFunction` — a Kismet compiler
optimization for "call a function on `self`" that invokes directly within the *already-executing* `ProcessEvent`
call rather than triggering a separate one. **No per-sub-function hook, pre or post, can ever intercept an
`EX_LocalVirtualFunction`/`EX_LocalFinalFunction` call individually** — only the true *outer* function, the one
the engine's own native anim-update system actually calls via a real `ProcessEvent` dispatch
(`BlueprintThreadSafeUpdateAnimation` itself), is hookable this way.

Fixed by re-pointing `s_lastUpdateFn`'s resolution at `BlueprintThreadSafeUpdateAnimation` instead of
`GetLeftHandLoc`, and confirmed live with the same diagnostics: `owner=` now matches the known proxy address
every time, and a write+readback check showed `movState=0x01` (crouch bit) -> `readback(crouch=1)` and
`movState=0x04` (falling bit) -> `readback(fall=1)`, both landing and persisting correctly at write-time (the
same technique that caught the `Speed`-scratch-var dead end back in the crash-investigation saga). Diagnostics
then stripped back out, session paused before a full visual confirmation pass.

**Reusable lesson, supersedes/refines the aim-pitch post-callback note above**: when hooking a `ProcessEvent`
post-callback to override a value an AnimBP recomputes every frame, hook the **outermost** Blueprint-callable
function actually invoked by the engine's native call path (usually the anim update entry point itself, e.g.
`BlueprintThreadSafeUpdateAnimation`/`NativeUpdateAnimation`), not any function it calls *internally* — internal
calls compiled as `EX_LocalVirtualFunction`/`EX_LocalFinalFunction` never trigger their own distinct
`ProcessEvent` dispatch, so no hook on them (pre or post) will ever fire, regardless of how correctly the rest of
the mechanism is built. **Always verify a new `ProcessEvent` hook actually fires for the intended target with a
throwaway diagnostic log before trusting a "no visible effect" result as evidence the underlying idea failed** —
this cost real time twice in a row this session (`Pitch`, then `IsCrouching`/`IsADS`/`Falling`) because the first
attempt's failure was accepted at face value instead of checked.

## Session 53 continued — one-handed weapon grip: five reflection dead ends, then a live-debug trail

**Visual confirmation first.** Live screenshots (both machines) confirmed the actual symptom precisely: a proxy
holding a shotgun shows a relaxed, **lowered**, one-handed carry pose (right hand on the grip, left hand hanging
free, barrel down) while the real local player shows a raised, **two-handed**, aimed pose (both hands engaged,
weapon up) for the identical weapon. A reference shot of a genuinely one-handed weapon (pistol) confirmed that
one-handed idle carry is the *correct* rendering for that weapon class — so the bug is specifically "shotgun
renders as if one-handed," not a generic aiming/stance gate.

Five FModel/reflection-based hypotheses were checked and ruled out, each with real live evidence, not just
"seemed to work":
1. `BP_PlayerCharacter_C::GetCurrentActiveWeapon()` — a real, callable `UFUNCTION` (confirmed via FModel export)
   returning an `Actor*` out-param — read `0x0` (null) on **both** local and proxy, even with the local player
   confirmed actively holding the shotgun correctly. Rules this out entirely; it isn't what any grip logic reads.
2. `LeftHandWeaponLocation`/`K2Node_PropertyAccess_13` (`GetLeftHandLoc`'s only output, a 30-byte function — see
   above) — byte-identical between local and proxy live samples.
3 & 4. Both `AnimGraphNode_Fabrik_6` and `_7` (the `AnimNode_Fabrik` IK solver node instances, 864 bytes each,
   dumped raw via a new SEH-guarded diagnostic after an *unguarded* first attempt crashed `do_game_tick` silently
   with zero log output — this project's "SEH-guard every new proxy/AnimInstance touch" rule paid for itself
   again) — **byte-for-byte identical**, all 864 bytes, both nodes, local vs. proxy. Conclusively rules out the
   Fabrik node's own stored state (including whatever `Alpha`/weight field it has) as the differentiator.
5. `WeaponType` (a plain `ByteProperty` enum `Enum_Firearms` on `BP_PlayerCharacter_C`, confirmed via FModel,
   Pistol=0/SMG=1/AR=2/Sniper=3/**Shotgun=4**/...) — read `0` (Pistol) on **both** local and proxy despite both
   confirmed holding the shotgun. Rules this out too — it's evidently a loadout/primary-slot classification set
   once, not a live "currently held" indicator, since local's grip renders correctly despite this reading wrong.

**Conclusion from the five dead ends**: whatever actually decides one-handed-vs-two-handed grip pose is not
reachable through the Blueprint reflection surface at all — it must be native C++ logic (most likely gating the
`AnimNode_Fabrik`'s blend weight from outside the node's own stored struct, e.g. a native
`IsTwoHandedWeapon(WeaponType)`-style check feeding into the anim graph's link/blend logic) that the
`GetValuePtrByPropertyNameInChain`/bytecode-dump toolchain simply cannot see, since it isn't a `UFUNCTION` or
reflected property at all.

**Live IDA remote-debug session, take 2** — reused Session 52's remote-debug methodology (`win64_remote.exe` on
the target, `idc.set_remote_debugger` + `idc.load_debugger("win32", 1)`) but hit **repeated IDA plugin hangs**
this time, each requiring a full IDA restart to recover, with the actual cause different each time — real
findings worth keeping for next time this happens:
- Attaching (local *or* remote) to a live process with default debugger options breaks at **every single DLL
  load event**, which looks identical to a genuine hang from the MCP client's side (request just times out) — no
  visible dialog, no crash, just a debugger sitting suspended waiting for a manual Continue. Fixed for the rest
  of the session with `ida_dbg.set_debugger_options(...)`, clearing `DOPT_LIB_BPT` (0x80) and `DOPT_THREAD_BPT`
  (0x8) while keeping the `*_MSGS` flags (0x40/0x4/0x100/0x1/0x10) for output-panel visibility — i.e. log module
  loads, don't break on them.
- Separately, `ida_dbg.retrieve_exceptions()` / mutate `.flags` (clear `ida_idd.EXC_BREAK`) / `ida_dbg.
  store_exceptions()` (no arguments — different signature than expected, operates on the vector already
  retrieved) disables the exception-notification dialogs from Session 52, same as before.
- **New finding this session**: any text/string search (`find`, or a manual `ida_search.find_text` loop) reliably
  hung the plugin *while a debugger was attached*, even with the above options already set and the process
  confirmed stable/running — requiring a full IDA restart every time. Root cause not fully identified (possibly
  the live-process variant of string search reading over the remote debug protocol is just extremely slow; a
  custom `ida_search.find_text` loop with placeholder `y`/`x` params of `0, 0` may also have been a genuinely
  broken/infinite loop rather than a hang, since the same *shape* of failure occurred both via the tested `find`
  tool and the hand-written loop). **Reliable workaround found**: do all string/text searching while the
  debugger is *not yet attached* (fast, reliable, confirmed working — found `AnimNode_Fabrik`'s string address in
  under a second on a fresh, un-attached IDB), then attach only afterward for the breakpoint/inspection phase
  that actually needs a live process.
- `server_health` (a trivial, non-mutating call) reliably timing out is a good signal the plugin's single request
  thread is genuinely blocked on a prior call, not that the connection itself is dead — retrying `server_health`
  a few times over ~10-30s often recovers on its own; if it doesn't, the IDA process itself needs restarting
  (check `Get-Process -Name "ida*" | Select Responding` — "Responding: True" at the OS level does NOT mean the
  MCP plugin's request thread is unstuck, these are independent).

**Static analysis trail found before running out of time** (all via the fast un-attached `find`/`decompile`
tools, no debugger needed): `AnimNode_Fabrik`'s string is referenced once, from a `UScriptStruct` registration
table at `0x7ff770273308` containing — among other fields — two lazy-singleton getter functions.
`sub_7FF76BB753E0` is `FAnimNode_Fabrik::StaticStruct()` (confirmed by its body: lazy-init a
`qword_7FF7705F1DC8`, calling a registration function with the struct's construct-ops pointer and its name
string). That construct-ops pointer (`sub_7FF76BBF06C0`) is itself another lazy singleton wrapping
`sub_7FF76A4416F0(&cache, off_7FF7702732F0)` — and `off_7FF7702732F0` is a *second*, near-identical registration
table containing `sub_7FF76BB796C0`, one level further in, which is yet another lazy singleton wrapping
`off_7FF7702739B8` — that turned out to be a single-entry table pointing back to the same shared package-getter
function (`sub_7FF76BB779A0`), a dead end for finding field offsets specifically (it's just package metadata,
not `SuperStruct()` as guessed).

**Continued and found the actual property list.** `sub_7FF76BB75900` (referenced twice in the outer registration
table) turned out to be the `ICppStructOps` builder — allocates a 16-byte descriptor with a vtable pointer
(`off_7FF76EEE6B10`) and the struct's real size (496 bytes) and alignment (16). More importantly,
`off_7FF76EEC7FA0` is a **flat array of property-name-pointers** — exactly analogous to what FModel's Blueprint
export shows for Blueprint-added properties, just for this native C++ struct's own fields. Read in order, `AnimNode_Fabrik`'s
own 8 declared fields are: `EffectorTransform`, `EffectorTarget`, `TipBone`, `RootBone`, `Precision`,
`MaxIterations`, `EffectorTransformSpace`, `EffectorRotationSource` — no `Alpha` among them, confirming it's
inherited from the base class, not redeclared by Fabrik itself.

Searched for `"AnimNode_SkeletalControlBase"` directly (same fast un-attached `find`) and found its own
registration table the same way: `ComponentPose`, `LODThreshold`, **`ActualAlpha`**, then enum metadata
(`UnderlyingType`/`AlphaInputType`). **`ActualAlpha`, not `Alpha`, is almost certainly the real per-frame-resolved
blend weight** — matching the standard UE5 `FAnimNode_SkeletalControlBase` pattern where `Alpha`/`AlphaScaleBias`/
`AlphaCurveName` are just the *configured input*, and a separate `ActualAlpha` field holds what the engine
actually resolves and applies each frame from whatever `AlphaInputType` selects. If this reads 0 on the proxy and
non-zero on local, that would fully explain the visual symptom without needing anything from `AnimNode_Fabrik`'s
own (already-proven-identical) struct data at all.

Attempted to read the raw property-descriptor bytes at `ActualAlpha`'s entry directly (`get_bytes`, 64 bytes from
the name-pointer) to extract its `Offset` field without a live debugger, but the UE5 UHT-generated
`FPropertyParamsBase`-family struct layout for property offset/flags isn't confidently known for this exact
engine build — decoding the raw bytes further risks exactly the kind of guessed-offset mistake this project's own
rules exist to prevent (a previous unverified offset guess crashed the live game). **Deliberately stopped before
guessing** rather than push an unverified interpretation.

**Self-correction, same session — `ActualAlpha` is actually already ruled out too, no live read needed.**
`FAnimNode_SkeletalControlBase` is `AnimNode_Fabrik`'s *base class* — in C++, a base-class subobject is embedded
at the start of the derived struct's own memory, meaning `ActualAlpha` (declared on the base) physically lives
*inside* the same 864-byte `AnimNode_Fabrik` memory region already raw-dumped and diffed earlier this session
(dead end #3/#4 above), which was **byte-for-byte identical, all 864 bytes**, between local and proxy. That diff
necessarily already covered `ActualAlpha`'s bytes too, at whatever offset it turns out to be — so it's already
proven identical without needing to find its exact offset at all. This is a sixth dead end, arrived at by
re-deriving a lead already disproven by earlier evidence — worth remembering *why* before chasing a named field
by excitement alone: check whether a "new" candidate is actually already covered by prior evidence before
spending time re-confirming it.

**Where this actually leaves the investigation**: every property stored *inside* the `AnimNode_Fabrik` struct
(including its inherited base-class portion) is now proven identical between local and proxy. Whatever causes
the visual difference must be **outside this struct entirely** — leading candidates, in rough order of
likelihood, none yet checked:
1. A different, upstream AnimGraph node (e.g. a "Layered blend per bone" or state-machine blend node) deciding
   how much of the whole arm-IK *chain*, not just this one Fabrik node, to blend into the final pose — would have
   its own separate alpha/weight, not part of `AnimNode_Fabrik`'s struct at all.
2. Bone *resolution* at runtime — `TipBone`/`RootBone`/`EffectorTarget` store bone *names* (proven identical), but
   the actual runtime bone *index* lookup against the skeleton could still fail differently, if e.g. the proxy's
   mesh/skeleton setup differs subtly from a real equipped-weapon setup in a way that doesn't show up in the
   static name data.
3. The weapon mesh's own attachment socket — our weapon-visual is attached via a separate manual socket-attach
   path (`equip_actor_to_socket`/`spawn_and_equip_item_visual`), not the game's real equip flow. If
   `EffectorTarget` is a bone-*socket* reference that expects a socket living on the currently-equipped weapon
   actor specifically (not just the character skeleton), our differently-attached weapon actor might not expose
   that socket the same way the game's own equip flow would.

**For next time**: don't re-chase anything inside `AnimNode_Fabrik`'s own struct (proven identical, closed).
Start instead from candidate 3 (most actionable, no live debugger needed — just inspect how
`equip_actor_to_socket`/the weapon-visual actor's own socket setup compares to what the real equip flow would
produce), or candidate 1 if that's a dead end (would need live `find`-while-unattached tracing of whatever
upstream blend node exists, same safe methodology as this session).

## Session 54: 2026-08-13 — Candidate 3 closed via existing raw dumps; new lead found; session.cfg staleness bug root-caused

**Candidate 3 (weapon's own attach socket) is closed — structurally impossible, not just unconfirmed.** Reused
the four raw struct dumps already sitting in `%APPDATA%\SurrounDeadBridge\` from a prior session
(`fabrik_{local,proxy}_AnimGraphNode_Fabrik_{6,7}.bin`, 864 bytes each) rather than re-capturing live. First
verified the dump's byte 0 aligns with `FAnimNode_Fabrik`'s own struct base (not some AnimInstance-relative
offset) by scanning for the `Precision`/`MaxIterations` pair from `research/CXXHeaderDump/AnimGraphRuntime.hpp`'s
declared offsets (`0x1E0`/`0x1E4`) — found `Precision=0.5, MaxIterations=10` at exactly `0x1E0` in all four
files, confirming the offset table lines up. Using that same table (`EffectorTarget` `FBoneSocketTarget` @
`0x130`, composed of `bUseSocket` bool @ +0x00, `BoneReference.BoneName` FName @ +0x04, `SocketReference.
SocketName` FName @ +0x60 within the `FSocketReference` sub-struct — full chain worked out from `Engine.hpp`'s
`FBoneSocketTarget`/`FBoneReference`/`FSocketReference` declarations), read `bUseSocket` directly: **`0` (false)
in all four dumps** (`_6` and `_7`, local and proxy alike). `EffectorTarget` is configured as a plain **bone**
reference, never a socket reference — it can never resolve through any socket exposed by a weapon actor,
manually-attached or otherwise, because the socket path is switched off entirely. This closes candidate 3 as
literally framed: the weapon-visual's own attach socket cannot be the differentiator for this specific IK
node, regardless of how it compares to the real equip flow's socket setup.

Raw values recovered (identical across all four files, `Number=0` for every FName): `BoneReference.BoneName`
CI=`1743560`, `TipBone` CI=`1743556`, `RootBone` CI=`1788648`. Not yet resolved to strings — `all_ci.txt` in the
same folder is a leftover from the unrelated GameplayTag CI-resolution work (Session 43) and doesn't cover
these. Resolving them needs a live `FName::ToString` call (`0x140C9D940`, rebased — see Session 9/34) the same
register-hijack way Session 43 did, at a frequently-hit, tolerant breakpoint; deliberately not attempted this
session to avoid destabilizing a live 2-client setup that had just been fixed (see below) — do this first thing
next live session, before anything else risks the connection.

**New, not-yet-checked lead**: `BP_PlayerCharacter_C::IsAiming?()` (confirmed via `research/CXXHeaderDump/
BP_PlayerCharacter.hpp`) — none of the five hypotheses ruled out in the previous entry touched this. Worth a
live read (same safe `ProcessEvent`-based getter pattern already used throughout `proxy_manager.cpp`, not the
riskier register-hijack technique — this is a plain callable `UFUNCTION`) on both local and proxy the next time
a shotgun is equipped and reproduced.

**Live reproduction reconfirmed today**: two-client setup (this machine as PC1, `win11-test` VM as PC2), PC2
wielding a shotgun — PC1's rendering of PC2's proxy still shows the one-handed carry pose. Bug is still live,
not something that self-resolved from any change since the previous entry.

**Root-caused a real, previously-undocumented gotcha: `session.cfg` silently overrides fresh tickets/env vars,
with no expiry-based self-correction.** `mod.cpp`'s `load_session_config()` reads `%APPDATA%\SurrounDeadBridge\
session.cfg` **first** and only falls back to environment variables for keys *absent* from the file — so a
`session.cfg` left over from a previous local-server instance (a different `worldId`, since `config.js`
generates a fresh random `worldId` every time the dev server restarts unless one is pinned via `settings.json`/
`SDB_WORLD_ID`) silently wins over any freshly-fetched ticket passed via `setx`/process env, with **no error
until the moment auth is actually attempted** (`wrong_world` server-side, surfaced client-side only as a
generic `[tcp] authentication rejected`) — cost most of a session's setup time before being traced to the file
rather than the ticket-issuing flow. Recovery: overwrite `session.cfg` directly with a ticket from the
currently-running server instance (matching `worldId`), not just env vars/`setx` — the game process must also
be fully killed and relaunched, since `load_session_config()` only runs once at mod init, not per reconnect
attempt. Separately (self-inflicted, not a project bug): a `cmd.exe echo TEXT > file` with a stray space before
the redirection operator got recorded as a literal trailing space in `SDB_GATEWAY_HOST`, which — being read
without trimming — broke `connect()` outright (`[tcp] connect failed`, distinct from an auth-level rejection);
write `session.cfg` via a real templating approach (PowerShell here-string / `scp`'d file), never
`cmd echo >>` line-by-line.

**Bone names resolved live — Fabrik IK chain identified as left-hand-follows-right-hand, refocusing the
investigation onto whatever picks the right arm's base pose.** With both clients back up, attached IDA
(elevated — the process crashed once mid-session on a bad first attempt, see incident note below, but the
technique itself worked cleanly once redone correctly) and reused the exact live `Player_AnimBP_C` instance
already sitting in `RCX` at a `GetValuePtrByPropertyNameInChain(Yaw)` breakpoint hit (this build's `watch_
aimoffset` diagnostic from Session 53, still active) — confirmed it was genuinely the AnimInstance by reading
`Precision=0.5, MaxIterations=10` at `+0x810+0x1E0` (`AnimGraphNode_Fabrik_6`), matching the struct exactly.
Read the **current** (this-process-valid) `ComparisonIndex` values directly from that live instance rather than
trusting the stale ones from the offline dumps (`1743643`/`1743639`/`1788730` this run, vs. `1743560`/
`1743556`/`1788648` in the old dump — a small but real per-process shift, confirming [[feedback-sdo-
gameplaytag-ci-unstable]] applies to plain bone `FName`s too, not just `FGameplayTag`s). Resolved all three via
a live `FName::ToString` register-hijack call (sanity-checked first against `CI=0` → `"None"` to confirm the
`FString` layout assumption before trusting real output): **`EffectorTarget.BoneReference.BoneName = "hand_r"`,
`TipBone = "hand_l"`, `RootBone = "clavicle_l"`** (`AnimGraphNode_Fabrik_6`; `bUseSocket=0` confirmed again on
this fresh read, doubly closing candidate 3).

**This fully reframes the bug.** `AnimGraphNode_Fabrik_6` is a left-arm IK chain (`clavicle_l` → `hand_l`)
whose effector target is the character's own **`hand_r` bone** — i.e. this node makes the left hand IK-reach
toward wherever the right hand currently is, the standard "off-hand follows the grip hand" rig for two-handed
weapons. The left hand's placement is therefore not an independent decision at all — it's a pure consequence of
wherever `hand_r` ends up, which is driven entirely by whatever upstream logic picks the *right* arm's own base
pose/animation. This makes **candidate 1** (an upstream AnimGraph blend/state-machine node, not
`AnimNode_Fabrik` itself) the clearly correct direction, now for a concrete, specific reason rather than "it's
not candidate 2/3/the struct itself, so try candidate 1 next": **next session should trace what actually
selects the right arm/hand's base pose** (state machine transition, layered-blend-per-bone alpha, or similar)
and check what it reads on local vs. proxy — this is now a much narrower, better-targeted search than "find
some upstream node" was before this session.

**Incident: one crash during this live-debug work, root-caused and not repeated.** The first attempt at this
resolution (using the stale `1743560`/`1743556`/`1788648` values from the old dump, before realizing they were
per-process-invalid) produced garbled `FString` output (`Num` in the hundreds, content that looked like
unrelated `DragonIK` plugin default-object names) and the game crashed shortly after resuming from that call.
Root cause: those `ComparisonIndex` values were valid in *some earlier* process instance's `FName` pool but not
in the one being debugged — calling `FName::ToString` with an out-of-range/reassigned index for the live
process's pool is unsafe (not just "returns wrong data," genuinely crashed the game this time), on top of the
technique's already-known return-value-corruption caveat for whatever real call gets hijacked. **Lesson**:
never reuse `FName`/bone `ComparisonIndex` values captured in a previous process instance for a live call in a
*different* instance — always re-read the current value fresh from a live object in the same session before
resolving it, exactly as this session eventually did successfully. Also worth keeping: sanity-check the
technique against `FName{0,0}` (`"None"`, universally stable) before trusting output for any real value, to
separate "the hijack mechanism itself is broken" from "this specific input is invalid" — this is what caught
the problem cleanly on the second attempt instead of repeating the same crash blind.

**Candidate upstream-node cluster identified (static, no live debugger) for "what picks the right arm's base
pose."** `research/CXXHeaderDump/Player_AnimBP.hpp` declaration order (`AnimGraphNode_StateMachine_2` @
`0x3388` → `AnimGraphNode_SaveCachedPose_1` → `AnimGraphNode_LayeredBoneBlend` @ `0x34D0` →
`AnimGraphNode_Slot_1` @ `0x35C0` → `AnimGraphNode_BlendListByBool_2` @ `0x3608`) is the classic UE5
"upper-body slot layered over full-body locomotion" pattern: a `Slot` node (montage/procedural pose injection
point) feeding a `LayeredBoneBlend` (blends that upper-body layer onto specific bones only — arms, plausibly),
gated by a `BlendListByBool`. Two more `StateMachine`+`BlendListByBool` pairs exist elsewhere in the same class
(`_1` @ `0x3770`/`0x3948`, unsuffixed @ `0x3A88`/`0x3B50`) — not yet distinguished which (if any of the three)
actually governs weapon-holding arm pose specifically. `FAnimNode_BlendListByBool`/`FAnimNode_BlendListBase`'s
own reflected fields in `AnimGraphRuntime.hpp` only show `BlendPose` (a `TArray<FPoseLink>`) — the real
per-frame-resolved `ActiveChildIndex`/weight fields exist in the true engine struct but aren't UPROPERTYs in
this build, so they're invisible to this reflection-based header dump; no `EvaluateGraphExposedInputs_...`
function exists for any `BlendListByBool` instance either (checked, none found), meaning its bool input is
likely a direct "Fast Path" property-copy rather than interpreted Kismet bytecode, or driven by a state-machine
transition rather than a plain bound bool.

**Deliberately stopped here rather than guess.** Two ways to actually resolve this, both requiring either live
access or substantial dedicated time — neither attempted this session:
1. **Live comparison** (fastest, needs a real 2-client session): dump raw bytes across the
   `StateMachine_1/_2/(unsuffixed)` + `BlendListByBool_1/_2/(unsuffixed)` region for the same character
   unarmed vs. holding the shotgun (same empirical byte-diff methodology already used successfully for
   `AnimNode_Fabrik` — no offset guessing needed, just diff two live captures and see what actually changes).
2. **Static bytecode decode**: find and manually decode (same `EX_*` opcode-parsing approach used throughout
   this project since Session 40) whatever state-machine transition-rule functions or Fast-Path copy records
   actually drive these three `StateMachine`/`BlendListByBool` pairs — a much larger, dedicated-session-sized
   task, not attempted here.

**Empirical byte-diff of the StateMachine/LayeredBoneBlend/BlendListByBool region attempted — inconclusive,
too noisy to isolate cleanly.** Captured the same `0x3388`-`0x3FF8` region (`armpose_*.bin` files in
`%APPDATA%\SurrounDeadBridge\`) three ways: PC1 local holding the shotgun (correct 2h render), PC1 local
holding a pistol (correct 1h render — found the PC2-proxy comparison's ~166 differing bytes were dominated by
per-instance heap-pointer noise, so switched to a same-object local-only comparison instead, which cleanly
isolates real state from object-identity noise), and PC2's proxy holding the shotgun (buggy 1h render, found
by re-arming the same `GetValuePtrByPropertyNameInChain` breakpoint repeatedly and catching a hit whose `RCX`
differed from the known local address — proxies run their own real `Player_AnimBP_C` tick same as local, so
this breakpoint fires for both). Local pistol vs. local shotgun diffed to just 12 bytes across 3 small regions
(`0x33B0` 4 bytes, `0x3430`/`0x3438` 2 bytes each, mirrored identically at `0x34C0`/`0x34C8`) — promising at
first given how much cleaner it was than the noisy proxy comparison. **But cross-checking against the proxy
capture killed this lead**: at every one of those 5 offsets, the proxy-shotgun value matched *neither* the
pistol pattern *nor* the local-shotgun pattern — a third, unrelated value each time. A real binary pose-state
indicator would put the (buggy, 1h-looking) proxy's bytes on the *pistol* side of the diff; getting a third
value instead means these particular bytes are far more likely continuously-varying internal state (elapsed
per-state timers, blend-interpolation progress, or similar) that just happens to differ between any two
non-simultaneous captures, not a stable behavioral flag. **Conclusion: raw byte-diffing this region, across
different capture moments and/or object instances, doesn't cleanly surface the real differentiator** — the
non-reflected (private, non-UPROPERTY) portions of `FAnimNode_StateMachine`/`FAnimNode_BlendListBase` that
this reflection-based header dump can't see are exactly where the real "who's driving this pose" signal likely
lives. Next attempt at this specific thread needs either genuine structural knowledge of those private fields
(from actual bytecode/transition-rule decompilation, not guessing) or a much larger set of same-state repeat
captures to first establish a noise floor before diffing across states — not attempted further this session.

**Montage hypothesis checked live and ruled out.** Resolved two more `UE4SS.dll` exports the same PE-export-
parsing way as `GetValuePtrByPropertyNameInChain` — `?GetFunctionByNameInChain@UObject@Unreal@RC@@...`
(`0x7ffe87daa720`, wide-string overload) and `?ProcessEvent@UObject@Unreal@RC@@...` (`0x7ffe87db29c0`) — and
used them for a genuine two-call live invocation (not just a memory read) of the local player's own
`AnimInstance::GetCurrentActiveMontage()`, on the theory that the two-handed hold pose might come from an
externally-triggered `UAnimMontage` playing through `AnimGraphNode_Slot_1`/`Slot` rather than being AnimGraph-
internal state. **Result: `NULL`** — no montage is active even while the local player correctly renders the
shotgun two-handed. This rules the montage/slot-injection theory out entirely; whatever drives the pose is
either baseline AnimGraph blend logic (the state-machine/`BlendListByBool` cluster already investigated and
found inconclusive above) or the separate `AnimGraphNode_ControlRig` node (`0x3B98`, size `0x460`) also present
on this class — not yet investigated, and the next concrete thing to try.

**Infrastructure note: one MCP hang + one more crash this session, both recovered.** A `continue_process()`/
`wait_for_next_event()` call issued while polling for a *specific* AnimInstance address (cycling through many
non-matching breakpoint hits) left the MCP plugin's request thread stuck hard enough that even `server_health`
kept timing out — this time it did **not** self-recover within a normal retry window (contrast with the
lesser hangs in Session 53's addendum) and needed a full IDA restart; the game crashed as a side effect of that
restart/forced-detach. No data was lost (everything up to that point was already written to disk). Successfully
recovered by relaunching IDA elevated, reattaching, and switching to a stricter one-hit-at-a-time polling style
(single `continue_process`+`wait_for_next_event` per call, checked and abandoned individually rather than
looped) for the rest of the session, which stayed stable. **Lesson**: prefer catching whatever the *next*
breakpoint hit is and adapting, over repeatedly polling for one specific expected address across many hits in
a row — the latter seems to be what actually triggers the known MCP-hang class of problem, not merely "many
calls" in the abstract.

**`AnimGraphNode_ControlRig` checked live — active on both, ruled out as a simple on/off gate.**
`FAnimNode_ControlRig`'s own reflected fields (`ControlRig.hpp`) give real, verified (not guessed) offsets:
`ControlRigClass`/`ControlRig` (live instance) pointers at struct offset `0x230`/`0x238`, `Alpha` at `0x240` —
absolute from AnimInstance base: `+0x3DC8`/`+0x3DD0`/`+0x3DD8` (struct itself at `+0x3B98`). Read live on local
(correct 2h shotgun render): `ControlRig` instance non-null, `Alpha=1.0`. Read live on PC2's proxy (buggy 1h
render, found by re-arming the `GetValuePtrByPropertyNameInChain` breakpoint and batching through a burst of
unrelated `BP_JigPickup` lookups until a `Pitch`/`Yaw` hit landed on a different, valid AnimInstance address)
— **also non-null, also `Alpha=1.0`**. ControlRig is equally fully-active on both; whatever's wrong isn't
"the rig doesn't run for proxies." Checked `InputMapping` (the `TMap<FName,FName>` that would show which
Blueprint AnimBP variables feed named rig inputs, same raw `TMap` layout convention as
[[sdo-mod-project]]'s `EquipmentIDSlotConfig` finding) — **empty (`ArrayNum=0`) on local too**, meaning
whatever the rig depends on isn't injected via a Blueprint-exposed variable at all; it must read skeleton bone
transforms directly inside its own RigVM graph, the same self-contained way the Fabrik node does. This is a
genuine dead end for tonight without actual RigVM bytecode analysis (a different, more complex bytecode format
than Kismet, not attempted this session) — noting it here rather than guessing further.

**Where this leaves the investigation, end of session.** Ruled out with real evidence, not just elimination by
exhaustion: candidate 3 as originally framed (weapon socket — `EffectorTarget` is bone-based, not socket-based,
confirmed twice live); the montage/slot-injection system (`GetCurrentActiveMontage()` returns `NULL` even on a
correctly-rendering local character); raw struct-field state in the `StateMachine`/`BlendListByBool`/
`LayeredBoneBlend` region (proven to be 100% time-noise via a same-state double-capture, not a real signal);
`ControlRig` simply not running for proxies (it runs identically, `Alpha=1.0`, on both). What's left: either
the `ControlRig`'s own internal RigVM logic (would need real bytecode-level analysis of the rig graph itself,
a substantially larger undertaking than anything attempted tonight), or the state-machine transition-rule
functions gating `BlendListByBool`/`StateMachine` (same caveat — needs actual decompilation, not more
memory-diffing, which this session already showed doesn't work for this specific data). Both are legitimate
next targets, but sized more like their own dedicated session than a continuation of tonight's.

**Candidate fix implemented for the mesh-detachment bug (code review, not live-verified — the bug has no
reliable on-demand repro).** Traced `spawn_and_equip_item_visual`'s existing `JigSetCanInteract(false, false)`
call (the fix for a related, already-documented physics-reassertion issue — see that function's own long
comment trail, Session 51) and found it's a **one-shot call, only at initial spawn**. Every slot that skips
reprocessing once already applied — the four early-`continue` clothing/accessory slots
(Facewear/Headwear/Eyewear/Backpack) *and*, on closer reading, the weapon slots (11-14) too, since their whole
write+visual-spawn path only runs inside `if (kEnableEquipmentWrite && equipItemChanged)` — never re-touches
its visual actor again after that first spawn, for the rest of the session. If the game's own internal
tick/timer logic can re-assert physics/interactability a second time later (unproven, but exactly the
mechanism the original comment already theorized, and consistent with the reported symptom being intermittent/
delayed rather than immediate), nothing would catch or correct that. Factored the existing call into a small
`reassert_no_interact(AActor*)` helper (`proxy_manager.cpp`) and call it every `sync_equipment()` pass — on the
already-applied path for the four clothing/accessory slots, and via a small switch on slot index for the four
weapon slots — cheap enough (single `ProcessEvent`, no spawn/attach work) to not need its own change-gate.
Builds clean, deployed to both machines' `Mods/SurrounDeadBridge/dlls/main.dll`. **Not yet live-verified**: the
detachment bug's own report gave no reliable trigger to test against on demand, so this is a well-evidenced
hypothesis fix, not a confirmed one — next session should watch for whether it recurs over a longer play
session with this build active, same as any other "did the fix work" check in this log.

**2026-08-14: disproven.** Confirmed `reassert_no_interact()` is still present and wired into every
`sync_equipment()` pass in the current build (verified via grep before this session's ADS-fix redeploys, which
rebuilt this same file) — it has been continuously running on both machines all night, including through
tonight's live 2-client testing. User reports the detachment still happened. This rules out "physics/
interactability gets silently re-asserted by the game later" as the mechanism, or at minimum shows
`JigSetCanInteract(false,false)` reasserted every tick isn't sufficient to prevent it. **Root cause is still
open** — falls back to the two real candidates already identified and explicitly sized as their own session:
the `ControlRig`'s internal RigVM bytecode, or the state-machine transition-rule functions gating
`BlendListByBool`/`StateMachine`. Next occurrence should be logged with: which item, roughly how far into the
session, and what the player was doing right before (moving/reconnecting/swapping gear) — no repro trigger has
ever been captured.

**Note distinguishing this from an existing, already-fixed glove/hand issue.** `proxy_manager.cpp`'s clothing
sync (`sync_equipment`, slot 5/Gloves) already documents a *different*, already-fixed "hands" bug: a
Gloves-vs-bare-hands **z-fight/flicker** (two overlapping meshes rendering against each other, fixed by hiding
the bare-hands body-part mesh once gloves are genuinely equipped). That's a visibility/render-order bug, not
physical detachment, and was already resolved before this session. Tonight's report (shirt/knife/shotgun lying
separately in the world, "occasionally the hands too") describes actual spatial separation, not a flicker — a
different symptom, most likely the same one-shot-`JigSetCanInteract` mechanism addressed above if "hands"
there means the Gloves *item actor* specifically (plausible, since Gloves go through the exact same
`spawn_and_equip_item_visual`/one-shot-physics-fix path as every other clothing slot) rather than the base
body-part hand mesh itself (which isn't a spawned/attached actor at all and couldn't "fall away" the same way).
Worth confirming which one the player meant next time it's reproduced live.

**Full manual scan of `BP_PlayerCharacter_C`'s reflected property list (`research/CXXHeaderDump/
BP_PlayerCharacter.hpp`), not just keyword search — no new candidate found.** Read every declared property
looking for anything explicitly encoding weapon handedness/grip type. Nothing found beyond what's already
ruled out: `WeaponType` (`Enum_Firearms`, dead end #5 above), `LeftHandLoc`/`FVector LeftHandLoc` @ `0x1D98`
(almost certainly the same underlying data as the already-checked `GetLeftHandLoc()` function, dead end #2).
Nearby candidates considered and dismissed as unlikely on their names alone (`HipFire?`, `InMeleeStance?`,
`CurrentFiringWeapon` @ `0x0930` — a direct `ABP_FirearmPickup_C*`, worth a live null-check next session since
it's a raw property rather than the already-ruled-out `GetCurrentActiveWeapon()` function, but not confirmed
either way this session — needs the *character* actor pointer, not the AnimInstance this session was reading
from; get there via the `find_local_pawn()` breakpoint hit, arg `AcknowledgedPawn`, same
`GetValuePtrByPropertyNameInChain` breakpoint already used all session — one more hijacked call on that hit to
complete the lookup and dereference the result, not attempted this session for lack of time). Confirms the earlier conclusion: this is genuinely not exposed as a simple reflected
property — real progress needs either the `ControlRig`'s own RigVM bytecode or the state-machine transition
rules, both bigger, dedicated-session-sized tasks.

## Session 55: 2026-08-13 continued — mod-side bytecode-dump tooling reused; `CActiveSlot` ruled out; ubergraph handed to background workers

**Rediscovered and reused this project's own mod-side diagnostic API instead of live IDA debugging** —
much safer, no debugger/crash risk at all: `bytecode_dump.flag` (`%APPDATA%\SurrounDeadBridge\`, two lines
class+function name, dumps `UFunction::Script` to a `.bin`), `resolve_fprop.flag`/`resolve_fname.flag` (batch
pointer/CI→name resolution), all processed by the already-running mod on its own tick, zero debugger attach
needed. Also recovered `kismet_disasm.py` (a complete, empirically-validated Kismet bytecode disassembler from
an earlier session, copied into this session's scratchpad) — every opcode's shape cross-checked against real
anchors from past decodes, not guessed.

Dumped and decoded three small, descriptively-named functions on `Player_AnimBP_C` (found via a full manual
read of its function list, not just hash-suffixed ones): `CombatState(int32 BlendSpace)` turned out to be a
thin stub that stores its param then jumps into the shared `ExecuteUbergraph_Player_AnimBP` at entry point
2085 — the real logic isn't in this small function at all. `GetThreadSafeBooleans()` copies ~8 booleans from
non-thread-safe character properties into thread-safe AnimBP-local copies (resolved names confirm this:
`Falling`, `IsCrouching`, `IsADS`, `InMeleeStance`, `LadderClimb`, `Swimming`, `Swimming_UnderWater`,
`InVehicle?` — matches the Parallel Anim Update theory from earlier, and cross-confirms the crouch/ADS/falling
sync properties already wired in Session 53). `GetAnimationInfoFromCharacter()` interface-casts the owning
pawn and calls an interface function (`GetAnimationInfo`, FName ci=1941812) returning 6 values, the first
(an FName) stored into an instance variable resolved to **`CActiveSlot`** — a promising-sounding new candidate
not covered by any prior hypothesis.

**`CActiveSlot` checked live and ruled out.** Added a small new mod diagnostic
(`check_watch_activeslot_trigger`/`log_activeslot_values` in `mod.cpp`, `watch_activeslot.flag`) reading it by
name (not a raw pointer, so immune to the cross-process staleness issue below) on local and the first proxy
once a second. Result: **`"None"` on both, consistently, including after confirming the local player's weapon
was genuinely drawn and active** (a real user-caught snag mid-session: the shotgun didn't actually draw the
first equip attempt — holstering and re-drawing was needed to get it genuinely active, worth remembering as a
general gotcha for any future live weapon-state test). Since the read is live and by-name, this is a real
negative result, not stale data — `CActiveSlot` doesn't track weapon-hold type at all, most likely an unrelated
UI/hotbar-selection concept that happens to share the `GetAnimationInfo` interface call.

**Cross-process bytecode pointer staleness reconfirmed** — attempted to resolve the interface class pointer
(`0x12ac9ac00`) found in the pre-relaunch `GetAnimationInfoFromCharacter.bin` dump against the *post-relaunch*
process (rebuilt+redeployed for the `CActiveSlot` watcher in between) and got `<access violation, not a live
UObject here>`, exactly the documented [[feedback-sdo-gameplaytag-ci-unstable]] class of gotcha extended to
raw bytecode operand pointers, not just `FName` CIs. Any bytecode `.bin` dump's embedded pointers are only
valid for resolution within the *same* process instance that produced them — re-dump fresh after any
relaunch before resolving pointers from it, don't reuse an old dump's addresses.

**Handed the large remaining work to background workers per direct instruction**, since the live-game/flag-
file interaction (a single shared resource) doesn't parallelize safely but static decode/interpretation of
already-dumped bytecode does: dumped `ExecuteUbergraph_Player_AnimBP` fresh from the current process
(entry point 2085 — `CombatState`'s target — is somewhere in here) and handed it, along with `kismet_disasm.py`
and this session's full context, to parallel agents for offline decode/interpretation.

**Worker 1 result — `CombatState`'s real logic decoded, entry point confirmed.** The ubergraph's dispatch
(offset 0) is a single `EX_ComputedJump` off the function's own `EntryPoint` int32 parameter — no jump table,
the entry-point value *is* the literal byte offset to jump to. `2085` decimal = `0x825`; the entire payload
there is two statements: `EX_Let` (`BlendSpaceInt = BlendSpace`) then `EX_PopExecutionFlow` straight to the
function's shared `Return`. That's the complete body — no branch, no weapon-type/grip check at all. Confirmed
`BlendSpaceInt` (resolved via `resolve_fprop.flag`) is a plain, directly-readable instance property, same
class as `Pitch`/`IsCrouching`/`CActiveSlot`. Whole 6609-byte ubergraph decoded cleanly, zero unhandled
opcodes. Full decode saved to the worker's scratchpad (`ubergraph_full.txt`) if ever needed again.

**Worker 2 result — RigVM roadmap, and real evidence against `ControlRig` being the mechanism.** Found the
real `ERigVMOpCode` enum and `URigVM`/`FRigVMByteCode` struct shapes already sitting in this repo's own
`research/CXXHeaderDump/RigVM.hpp`/`RigVM_enums.hpp` (nobody had looked before) — real offsets:
`URigVMHost::VM` at `+0x58`, `URigVM::ByteCodeStorage` at `+0x60`, pre-decoded `Instructions` at `+0x108`,
`FunctionNamesStorage` at `+0x120` — flagged as needing live verification before trusting, same discipline as
everything else this project does, not yet live-checked. More importantly: the game's *only* Control Rig
asset (`ControlRig_Player`, already exported to `Exports/SurrounDead/Content/Animations/
ControlRig_Player.json` from earlier FModel work) decodes to just 8 nodes implementing a **head/spine
look-offset** (`OffsetTransformForItem` targeting `spine_01`/`head`) — no IK, no constraint, nothing
weapon-related at all. Real evidence, not just absence of a hit, that `ControlRig` was never the mechanism —
deprioritize it if this ever needs revisiting.

**Root cause found and FIX CONFIRMED LIVE.** With the real `CombatState` logic known, added a live watcher
(`watch_activeslot.flag` extended to also read `BlendSpaceInt` by name — same safe pattern as everything else)
and did a real A/B comparison on the actual local (always-correct) player switching weapons: **`BlendSpaceInt`
genuinely tracks weapon grip category** — `0` = default/unarmed, `1` = confirmed for `BenelliM4`/shotgun
(Secondary slot, two-handed), `2` = confirmed for `BattleReadyGlock`/pistol (Sidearm slot, one-handed). The
proxy, holding the identical shotgun, stayed pinned at `0` throughout — because nothing ever calls
`CombatState()` for a proxy at all; its equip path is this mod's own manual `spawn_and_equip_item_visual`/
`equip_actor_to_socket` route, never the real game equip flow that apparently triggers `CombatState` as a
side effect for actual local input.

**Fix**: `call_combat_state()` + `combat_state_blendspace_for_slot()` (`proxy_manager.cpp`), called from
`sync_active_weapon_hand()` whenever the proxy's active weapon slot changes — mirrors what real equipping
apparently does automatically. Slot→BlendSpace mapping: Primary/Secondary (11/12) → `1` (two-handed, Secondary
confirmed), Sidearm (13) → `2` (one-handed, confirmed), Melee (14) → `1` (**unconfirmed guess**, not
live-tested). Built, deployed to both machines, **live-verified working** — the proxy now visibly holds the
shotgun two-handed, correctly. This is the real fix for the entire weapon-grip-pose investigation that ran
across Sessions 53-55.

**Melee mapping corrected and confirmed live, same day.** The initial guess (Melee -> BlendSpace `1`, same
family as Primary/Secondary) was wrong — live-tested via the same `watch_activeslot.flag` technique
(`InMeleeStance=1` on a genuinely-correct local render -> `BlendSpaceInt=3`, not `1`) and produced a visibly
wrong "holding it like ADS with a long gun" pose on the proxy until corrected. Fixed in
`combat_state_blendspace_for_slot` (slot 14 -> `3`) and confirmed live: proxy grip now looks correct.
`kEnableEquipmentWrite`-style caveat still applies — every other weapon *type* within Primary/Secondary/
Sidearm hasn't been individually tested, only one representative of each so far (BenelliM4, BattleReadyGlock,
TacticalHatchet).

**Separate, still-open thread from the same testing pass: syncing the actual melee *swing* animation, not
just the static grip pose.** `BP_PlayerCharacter_C::PlayMontage`/`MC_Montage`/`Svr_Montage` were all hooked
(resolved successfully, confirmed via log) but **none of the three fire during a real melee swing** — ruling
out the "one generic montage-player function" theory this session started with. The actual swing-montage
selection (`NormalMeleeAttackMontages`/`PowerMeleeAttackMontages` on `BP_WeaponsPickupComponent_C`, confirmed
present via `research/CXXHeaderDump/BP_WeaponsPickupComponent.hpp`) is very likely invoked directly via the
engine's own `UAnimInstance::Montage_Play` from within the *weapon component's* own Blueprint graph, not
through any character-level wrapper — next step is hooking `Montage_Play` itself (a standard engine
UFUNCTION every AnimInstance has, so expect it to be noisy/fire for other animations too — will need a
filter, e.g. only relay if the played montage's name matches one of the two confirmed melee-attack-montage
array properties) rather than continuing to chase player-character-level RPCs.

**Protocol/relay infrastructure for this already built and confirmed working end-to-end for the mechanism
itself** (just needs the right sender-side hook once found): `MsgType::PlayMontage` (=45), full encode/
decode round-trip tested (`tests/protocol_roundtrip.cpp`, 482/482 passing), `gateway.js` relay wired
(client-authoritative, same pattern as Equipment/WeaponAttachments/PawnAppearance), receiver-side
`ProxyManager::on_play_montage()` resolves a montage by name (`resolve_montage_asset`, new cache mirroring
`resolve_item_asset`'s pattern for `AnimMontage` instead of `JigsawItem_DataAsset_C`) and calls the proxy's
own `PlayMontage` — this receiver path is untested live since nothing has successfully triggered it yet
(sender-side hook is the missing piece, not the relay/receiver plumbing).

**Follow-up attempt: hooking the standard engine `UAnimInstance::Montage_Play` directly (public, documented
UE5 API, not a guess) instead of continuing to chase custom character-level RPCs — also inconclusive, a
genuine resolution mystery, not attempted further this session.** Added step-by-step diagnostic logging
(`on_process_event_pre`) around the exact same `Mesh->GetAnimInstance()` chain `on_process_event_post`'s
`s_lastUpdateFn` resolution already uses successfully elsewhere in this file. Result: consistently logs
`find_local_pawn() null` on every throttled attempt, even minutes into a live session with a confirmed
active pawn (other equip actions logging successfully in the same window) — genuinely puzzling, since the
*other* hooks in this same function (`s_drop_fn`/`s_pickup_fn`/`s_playMontage_fn`/`s_mcMontage_fn`/
`s_svrMontage_fn`) all resolved fine via the identical `find_local_pawn()` call pattern. Swapping to
`cached_find_local_pawn()` wouldn't help (it's a 100ms cache wrapping the exact same underlying call, not a
different lookup mechanism) so wasn't attempted live. **Left as-is, diagnostic-only, harmless** (the
resolution block just keeps failing silently, no crash risk) — worth a fresh look next session, possibly by
checking whether `find_local_pawn()`'s own implementation has some fragility (a class-name/component check
that could intermittently miss) rather than assuming the calling pattern itself is at fault.

**Push/shove — scoped via FModel exports (per direct instruction to check them), not yet attempted live.**
`Exports/SurrounDead/Content/Animations/Player/Anims/Actions/Push/` has four plain `Anim_*` assets
(`Anim_LargeMeleePush`, `Anim_SmallMeleePush`, `Anim_PistolPush`, `Anim_RiflePush`) — bare `AnimSequence`
assets, **not** `_Montage`-suffixed like every confirmed melee-attack asset (`1HMeleeAttack1-4_Montage`,
`Crouch_MeleeAttack_Montage`). This is a real, structural difference, not just a naming quirk: a plain
`AnimSequence` can't be played via `Montage_Play` (which expects a pre-built `UAnimMontage`) — push almost
certainly goes through the engine's `PlaySlotAnimationAsDynamicMontage` instead, a function with several more
parameters (`SlotNodeName`, blend in/out times, loop count, etc.) than `Montage_Play`'s two, meaning more
alignment/offset guessing risk to safely hook. Also confirmed via export: CUE4Parse's default
`BlueprintGeneratedClass`/`Function` export only lists function metadata (params, flags) — no decoded Kismet
bytecode/script content — so exports can narrow *which* engine API a system likely uses (as they did here)
but can't reveal the actual call site/trigger the way a live `bytecode_dump.flag` capture of the *right*
function can. Not attempted live this session — a genuinely separate, not-yet-started investigation from
melee's, given the different underlying mechanism.

## Session 56 continued — Montage_Play resolution fixed; MeleeTrace decoded, ruled out; still unresolved

**Fixed the `find_local_pawn()` resolution mystery**: swapping `find_local_pawn()` for
`cached_find_local_pawn()` (a 100ms-cache wrapper around the exact same underlying call, defined in
`mod.cpp`) in the `Montage_Play` resolution block made it resolve successfully, after 10 straight failed
attempts with the direct call. Root cause not understood (both calls are the same function underneath), but
the fix is real and cheap — worth defaulting to `cached_find_local_pawn()` over a bare `find_local_pawn()`
call in any *new* hook added to `on_process_event_pre` from now on, given this is the second time a bare
call has behaved unreliably in this exact function.

**`Montage_Play` still doesn't fire during a real swing even once correctly resolved and hooked** — six
candidate functions now ruled out with real evidence (`PlayMontage`, `MC_Montage`, `Svr_Montage`,
`Montage_Play`, and now `BP_MeleePickup_C::MeleeTrace` itself, decoded via `bytecode_dump.flag` +
`kismet_disasm.py`, confirmed to call only `SphereTraceMulti`/`GetPlayerCharacter`/`Array_Length`/
`Array_Get`/`Add_IntInt`/`Less_IntInt` — resolved via `resolve_ptr.flag`, **not** `resolve_fprop.flag`
(a real mixup this session: `resolve_fprop` is for `FProperty*`/`FField` pointers via the `NamePrivate`
offset, `resolve_ptr` is for `UObject*`/`UFunction*` via `GetFullName()` — using the wrong one on a
`func=0x...` operand silently produces `<access violation>` for every single entry, which looks identical
to a real crash-on-resolve until you notice it's *always* every single one). `MeleeTrace` is confirmed pure
hit-detection (a sphere trace + iterating the hit array) — genuinely not where the animation gets triggered
from, ruling out this specific function rather than just failing to find something in it.

**Also hit a real flag-file race worth remembering**: a `resolve_ptr.flag` write was consumed (file deleted)
but produced zero log output at all — not even a parse-failure message — most likely the mod's poll read the
file mid-write and saw it empty. Retrying the identical write a few seconds later worked cleanly. If a flag
file disappears without any corresponding debug.log output at all (not even an error), don't assume the
content was invalid — just retry.

**Where this leaves melee swing sync**: the actual trigger is neither `MeleeTrace` nor any of the five
character/engine-level montage-playing functions tried. Likely candidates not yet checked: whatever function
is bound to the actual attack *input action* (`InpActEvt_IA_...`-style, per `BP_PlayerCharacter.hpp`) which
probably calls `MeleeTrace` *and* separately triggers the animation — dump and decode that input handler
next, rather than continuing to guess at montage-playing functions directly. This sub-investigation has now
consumed a large amount of session time across many rebuild/redeploy/live-test cycles for six ruled-out
candidates; worth treating as its own dedicated session rather than a quick continuation next time.

**New bug spotted live, not yet investigated: weapon base mesh invisible while its attachments still
render.** Screenshot evidence: a proxy's AK15 — its own base rifle mesh is missing/invisible entirely,
while its attached items (scope, and at least one other attachment) render correctly and stay positioned
as if still attached to the (invisible) weapon. Distinct from both the now-fixed grip-pose bug and the
open mesh-detachment bug above — this isn't about pose or about an item falling away, the base mesh itself
just isn't drawing while its children still are. Worth checking `spawn_and_equip_item_visual`'s
`SetActorHiddenInGame(false)` call and whatever governs the base weapon actor's own mesh visibility
specifically (as opposed to the attachment sub-actors', which appear to have their own separate visibility
state that's unaffected) next time reproduced.

**New data point on the mesh-detachment bug (still open, separate from the grip-pose fix above)**: this
session's live testing also caught a character's **head** detaching/floating separately from the body — same
bug class as the shirt/knife/shotgun detachment already logged, now confirmed to also affect head/headwear,
not just weapon and clothing slots. Doesn't change the existing hypothesis (`JigSetCanInteract` fix already
deployed) since heads aren't a `JigPickup`-style spawned actor at all — worth a fresh look at whether headwear
specifically (a real spawned/attached item, unlike the base head mesh) is the actual thing detaching, next
time it's reproduced.

**New finding, same session: aim-offset (pitch/yaw) sync partially works now, contradicting the earlier
"no visible effect" note above.** Live-reported while testing other things: pitch (looking up/down) now
visibly works on the proxy, just needs smoothing/interpolation (likely the same class of "teleporty" snap
issue position/yaw sync already solved via exponential smoothing — see `RemotePlayer::renderX/Y/Z/Yaw` in
`state.hpp`, not yet applied to pitch). Yaw (looking left/right) still doesn't work at all. Not yet
investigated further this session (mid-flight on the melee-swing-animation thread when reported) — worth a
dedicated look next: check whether the existing `Pitch` write (`on_process_event_post`, aim-offset section)
has a yaw counterpart at all, or whether left/right look is actually meant to come from the existing body-yaw
smoothing (`RemotePlayer::renderYaw`) rather than a separate `AimYaw`-style AnimBP property no one's added yet.

**Confirmed 2026-08-13 (same session, later): it's headwear specifically, not the base head mesh.** Live
screenshot: a gas mask/helmet (a real spawned/attached item actor, same `spawn_and_equip_item_visual` path as
weapons/clothing) visibly detached and floated away from the proxy's head — exactly the hypothesis above. This
is the same one-shot-`JigSetCanInteract` mechanism as the other detached slots, just not yet observed for
headwear specifically before now — corroborating evidence the already-deployed fix (`335a5b1`) has the right
scope, not a sign of a new gap.

**New bug reported live, not yet investigated: proxy meshes intermittently detach entirely (not a pose issue —
the mesh visibly separates from the character).** Screenshot evidence from this session: PC2's proxy (rendered
on PC1) had its equipped shirt, knife, and shotgun all lying/standing separately in the world, no longer
attached to the character. Per the player: **also occasionally happens to the hands** (i.e. not limited to
spawned/attached item actors — a body-part mesh too), which argues against this being purely the same class of
bug as the physics-reassertion issue already fought and partially fixed in `spawn_and_equip_item_visual`
(`proxy_manager.cpp` ~L908-1010: the game was found to silently re-enable physics/interactability on an
equipped item's actor sometime after `SetSimulatePhysics(false)` was applied, worked around by also calling the
item's own `JigSetCanInteract(CanInteract=false, EnablePhysics=false)`). That existing fix only covers spawned
item actors going through this function — it can't explain hands (part of the character's own skeletal mesh,
not a separately spawned/attached actor) detaching by the same mechanism. Two live testing sessions now
independently report full detachment (this one; the equipped-item pulsing/flicker bugs fixed in Sessions 52/53
were a *different*, already-closed symptom — visibility flicker, not physical separation). Worth its own
dedicated investigation next session: reproduce deliberately, and check (a) whether the existing
`JigSetCanInteract` re-assertion is actually still firing/effective over a longer play session, (b) whether
hands detaching correlates with any specific trigger (backpack swap, clothing re-equip, proxy re-sync tick) or
looks purely time-based/random, (c) whether it's specific to the proxy path at all or can be reproduced on a
real local player's own hands too (would rule out anything proxy-specific).

## Session 57: 2026-08-13 continued — melee/push/vault montage sync solved via generic polling; four stacked live-connectivity bugs found and fixed

**The melee swing trigger hunt (six prior ruled-out hooks: `PlayMontage`, `MC_Montage`, `Svr_Montage`,
`Montage_Play`, `BP_MeleePickup_C::MeleeTrace`) ends here, but not by finding the trigger — by making the
trigger irrelevant.** Picked back up by dumping `BP_PlayerCharacter_C`'s `InpActEvt_IA_PrimaryAction_*`
variants (Enhanced Input generates one stub per trigger phase — Started/Triggered/Completed — per binding;
found 8 total: `_4, _6, _13, _23, _24, _42, _64, _65`). Each is a 90-byte stub that just calls
`ExecuteUbergraph_BP_PlayerCharacter(EntryPoint)` with a different `EntryPoint` int constant — the actual
per-phase logic lives at that offset inside one enormous (205,862-byte) flat ubergraph function, not in the
stub. Disassembled from `_65`'s entry point (59833) forward with `kismet_disasm.py`
(`C:\Users\mccau\AppData\Local\Temp\claude\...\scratchpad\kismet_disasm.py` this session, same tool as prior
sessions) — decoded 146KB linearly with zero unhandled-opcode failures, confirming it's genuinely one
connected control-flow region (only one top-level `EX_Return` in the whole slice, at the very end).

Found 29 calls to `PlayMontageCallbackProxy::CreateProxyObjectForPlayMontage` (the *async* "Play Montage" K2
node — resolved via `resolve_ptr.flag`) in that region, but **every single one** reads its montage argument
from only two `BP_WeaponsPickupComponent_C` fields (`EquipMontage`/`UnequipMontage`, confirmed via
`resolve_fprop.flag`) or two hardcoded `EX_ObjectConst` literals — `A_Equip_Item_Montage` /
`A_Unequip_Item_Montage` (resolved via `resolve_ptr.flag`, confirmed `AnimMontage` assets). All 29 are the
generic weapon-draw/holster flow, not melee. Traced *why* this is architecturally correct: `BP_MeleePickup_C`'s
own header (`research/CXXHeaderDump/BP_MeleePickup.hpp`) has `MeleeTrace`/`MeleeTracePower`/damage helpers but
*no* montage-playing function or montage property at all — melee hit detection lives there, but swing
animation doesn't. `BP_WeaponsPickupComponent_C`'s header, by contrast, owns
`NormalMeleeAttackMontages`/`PowerMeleeAttackMontages` (both `TArray<UAnimMontage*>`),
`CrouchedMeleeAttackMontage`, `ShoveMontage`, `ReloadMontage`, `ChamberFirearmMontage` — i.e. **every**
current and future montage-driven action's asset lives on this one component, picked essentially at
random from an array per-swing (hence four different swing montages —
`1HMeleeAttack1_Montage`...`1HMeleeAttack4_Montage`, confirmed live) — but its own `ExecuteUbergraph` turned
out to be a tiny 325-byte BeginPlay-style init block with no montage logic in it either. The actual
selection+play call is somewhere else entirely (never pinned down exactly where — possibly a native/opaque
path, given `UBP_WeaponsPickupComponent_C` exposes no other Blueprint function that could hold it).

**Rather than keep chasing the exact call site, made the whole hunt moot.** `ACharacter::GetCurrentMontage()`
(confirmed live via `resolve_ptr.flag`: `Function /Script/Engine.Character:GetCurrentMontage`) is a plain,
zero-arg, always-resolvable native UFUNCTION — a pure getter reading whatever the local player's AnimInstance
currently has active, regardless of *how* it got there (sync `Montage_Play`, the async
`CreateProxyObjectForPlayMontage` proxy — which plays the montage via a raw C++ call inside its own
`Activate()`, invisible to any ProcessEvent hook no matter which UFUNCTION is targeted — or any future
mechanism). Added `check_local_montage_change(AActor* pawn)` to `mod.cpp`, called once per movement-tick
(same cadence as `send_movement`): calls `GetFunctionByNameInChain(L"GetCurrentMontage")` +
`ProcessEvent`, and broadcasts a new `PlayMontage` frame whenever the returned pointer changes to a new,
non-null value. One mechanism, confirmed live to catch: all four melee swing variants, `Roll_Montage`,
`Crouch_MeleeAttack_Montage`, `Anim_SmallMeleePush_Montage` (the push/shove action — a separate
`InpActEvt_IA_Shove_K2Node_EnhancedInputActionEvent_5` binding, not investigated further since polling already
covers it), `MQ_Vault_RM1_Montage` (vault — likewise not separately investigated, same reason),
`MQ_GettingUp_RM1_Montage`, `Montage_RifleEquip`/`Montage_RifleUnequip`, and the pre-existing
`A_Equip_Item_Montage`/`A_Unequip_Item_Montage`. PlayRate is defaulted to 1.0 (the getter doesn't expose it;
none of these are precision-timed enough for that to visibly matter). This approach is a strict superset of
every one of the six previously-hooked functions and needs zero future per-action bytecode archaeology for
whatever montage-driven action gets added next.

**Live 2-client testing of the above then surfaced four independent, stacked connectivity/stability bugs —
each looked like "the game/network is just broken" until root-caused individually:**

1. **Reconnect had no backoff on auth/join rejection, only on raw TCP connect() failure**
   (`tcp_client.cpp::thread_func`). `reconnectMs` reset to the 250ms floor on every *successful* TCP-level
   connect, before authentication was even attempted — so a rejected (single-use, already-spent) ticket
   caused an unbounded tight reconnect loop with **zero delay** between attempts (connect+auth+reject+close
   on a LAN completes in single-digit milliseconds). One stuck client produced **~24,000 leaked `TIME_WAIT`
   sockets** on port 42200 in one session and fully exhausted the machine's local ephemeral port range,
   breaking *all* outbound connections until they drained — this is what made PC1 "disappear" the first two
   times tonight, not a crash. Fixed: backoff now applies uniformly to every disconnect reason; only a
   session that actually reaches `ConnState::Active` resets the floor back to 250ms.

2. **`config.js`'s `str()` settings-file reader silently treated a literal JSON `false` as "unset."**
   `if (f && ...)` is falsy for boolean `false`, so `ticketReplayProtection: false` in `settings.json` (added
   as the intended fix for #1's underlying "tickets are single-use, client can't get a new one" gap — a
   dev/LAN-only escape hatch until the launcher owns fetching a fresh ticket per reconnect) was never actually
   applied — replay protection stayed on all night regardless. This is why the bug in #1 kept recurring even
   *after* the settings change looked correct and the server was restarted with it. Fixed the helper
   (`f !== undefined && f !== null` instead of bare truthiness) — a general footgun for any future
   boolean-via-settings.json config, not just this one field.

3. **Gateway's per-client rate limit (120 frames/s) was tuned before montage sync existed.** Movement alone
   is ~20/s at the default `SDB_MOVE_INTERVAL_MS`; a real melee combo now fires several `PlayMontage` sends
   within well under a second on top of that, plus whatever periodic equipment/appearance/attachment resyncs
   land in the same window — easily exceeding 120/s in a burst during actual combat. Both clients got
   server-side `rate limited` and dropped mid-fight — the improved logging added this session (see below) is
   what finally made this *visible* instead of looking like an unexplained crash. Made `CLIENT_RATE_LIMIT` a
   real config value (`cfg.clientRateLimit`, was a hardcoded constant) and raised it for this dev server; a
   production deployment should retune deliberately rather than inherit either number blindly.

4. **The actual "pawns jittering" cause: a live network echo loop, not a sync-quality problem.**
   Two *pre-existing* `ProcessEvent` hooks — `s_playMontage_fn` (watching `PlayMontage`) and
   `s_montagePlayEngine_fn` (watching `UAnimInstance::Montage_Play`) — each resolved their target as a bare
   `UFunction*` off the local pawn once and compared every future `ProcessEvent` call's `func` against it.
   `UFunction*` is shared per-*class* in UE, not per-instance — proxies are the same `BP_PlayerCharacter_C`
   class, so the moment `on_play_montage` (`proxy_manager.cpp`, this session's new receiver) legitimately
   called `PlayMontage` on a *proxy* to apply a montage it had just received over the network, these two
   hooks fired again, had no way to tell it wasn't a fresh local action, and re-broadcast it — bouncing
   between both machines' proxies and amplifying with whatever each side had recently played. Live symptom:
   `debug.log` showed the exact same ~9-montage sequence repeating verbatim every ~30ms. Removed both hooks
   entirely (`mod.cpp` — `handle_play_montage_hook`/`handle_montage_play_engine_hook` are now dead code,
   confirmed via compiler warnings) since `check_local_montage_change` already supersedes them and is
   correctly scoped — it's called on a *known* local-pawn pointer, never matched against every `ProcessEvent`
   call in the process by bare function pointer.

**Also added this session, prompted directly by how much of the above took line-number-proximity guesswork
across three separate log files to diagnose:** real `HH:MM:SS.mmm` timestamps on both `debug.log`
(`src/debug_log.hpp`) and every server console line (`server/src/index.js`, wraps `console.log/warn/error`
globally at startup) so future cross-log correlation doesn't require re-deriving a shared timeline by hand.
`SDB.log` (UE4SS's own `Output::send` channel) deliberately left untimestamped — too many existing call sites
across `mod.cpp`/`tcp_client.cpp` to retrofit for the marginal value tonight. Also substantially expanded
server-side connection-lifecycle logging (`gateway.js`): remote address + connection age + role/state on every
close (previously silent for anything that never reached `'joined'`), and an explicit log line for the
stale-connection-eviction path (same-pid double-auth) that was invisible before — the exact case that would
otherwise look identical to an ordinary rejected ticket from the evicted side.

Confirmed live after all four fixes: both clients hold a stable connection through active combat, and the
montage-spam is gone (`debug.log` timestamps show the last old-pattern `send_play_montage:` line landing
*before* the fixed build's relaunch, zero since). User confirmed "everything seems to be working" after
retesting melee swings against the fixed build.

**Not yet separately confirmed live, though the generic poll should cover both for free:** push/shove
(`Anim_SmallMeleePush_Montage`, already observed being sent correctly) and vault (`MQ_Vault_RM1_Montage`,
likewise) — worth a deliberate visual check next session that the *receiving* side actually plays them
correctly on a proxy, not just that the sending side detects and broadcasts them (which is confirmed).

## Session 57 continued, same night — aim-yaw polish: turn-in-place attempted and reverted; real ADS
## body-rotation mechanism found via live bytecode trace, not yet wired up

**Aim-yaw shipped this session (see above) is body-relative and correctly signed, but clamps hard at
the AnimBP blendspace's own ±90-degree range** (confirmed via the FModel export,
`Exports/SurrounDead/Content/Animations/Anims/AimOffsets/RifleIronsightsAimOffset.json`'s `AxisX`/`AxisY`
`Min`/`Max`). Two live bugs found and fixed in the sign/smoothing itself before landing on the
turn-in-place question at all:

1. **Sign was inverted.** Live-confirmed: turning the camera left visibly swung the whole upper-body/arm
   pose right. This game's AnimBP `Yaw` blendspace convention is the opposite of the standard UE
   `ControlYaw − BodyYaw` sign. Fixed by negating `player.renderAimYaw` at the write site
   (`mod.cpp::on_process_event_post`), not earlier in the pipeline — negating the sender's raw absolute
   control yaw instead would NOT be equivalent, since the body-yaw term in the subtraction isn't negated
   too (`-(a) - b != -(a - b)`).

**Turn-in-place — attempted, then fully reverted; do not re-attempt without a real fix for the underlying
mesh-transform-refresh problem found below.** The goal: once the aim-offset exceeds the blendspace's ±90
range, physically rotate the proxy's body to "catch up," matching how survival/third-person games usually
handle this, instead of the pose just clamping at the hard edge.

- **First attempt** nudged `RemotePlayer::renderYaw` directly as a second force alongside the existing
  body-yaw smoothing (which pulls `renderYaw` toward the raw synced `yaw` every tick regardless). The two
  fought to a stable equilibrium instead of the offset ever actually closing — live-confirmed via a densified
  (~20/sec, was 1/sec) diagnostic added specifically to catch this: `targetYaw` plateaued at ~112 degrees for
  a full 5-second window instead of decaying toward the 70-degree trigger threshold, despite the per-tick
  math suggesting it should close in well under a second.
- **Second attempt** fixed that: added a persistent `RemotePlayer::turnInPlaceYawOffset` (NOT reset by
  incoming packets, unlike every other raw field) and folded it into the body-yaw smoothing's *target*
  (`yaw + turnInPlaceYawOffset`) instead of being a second competing force. This actually worked in the sense
  that the accumulator grew/shrank correctly — but then:
- **Grow/decay used the wrong reference and became self-defeating.** The decay condition checked the
  *post-correction* offset (`aimYaw − renderYaw`), which shrinks toward zero *by design* the moment the
  correction starts working — but that same smallness was also the decay trigger, so success immediately
  started undoing itself. Live-reported: hold a steady ADS look-left angle, stop moving the mouse, and the
  body visibly snapped back toward center even though the player was still actively aiming left. Fixed by
  basing grow/decay on the *raw* gap (`aimYaw − yaw`, ignoring the accumulated offset entirely) instead —
  this only actually shrinks once the camera itself moves back toward the real synced body facing, not
  whenever the correction happens to be working.
- **Even after both fixes, the body visibly never rotated at all — a completely different, deeper problem.**
  Densified diagnostic showed `renderYaw` correctly reaching and holding the exact target value
  (`renderYaw == rawAimYaw` to 2 decimal places, sustained for 5+ seconds) — the *data* was completely
  correct — but the live player-facing report was unambiguous: **"the full model never rotates... when you
  stop moving [the mouse] it snaps back to the original position."** Root cause: `renderYaw` is applied to
  the proxy's *mesh* via a raw memory write to `RelativeRotation` (`proxy_manager.cpp`'s
  `apply_proxy_body_yaw_safe`/`do_apply_proxy_body_yaw`, offset `USceneComponent+0x140` — the same
  established workaround for `bOrientRotationToMovement` silently overriding any actor-root rotation call).
  That raw write appears to only get visually picked up when something *else* recomputes the mesh's cached
  world transform, which normally happens during movement — while genuinely stationary (exactly the case
  turn-in-place matters most for: holding ADS and looking around without moving your feet), nothing ever
  triggers that recompute, so the write never becomes visible even though the underlying value is exactly
  right. Confirmed this isn't just a mesh-vs-actor distinction, either: `ProxyManager::teleport_proxy`
  *already* calls the actor-level `K2_SetActorLocationAndRotation` with this same `renderYaw` value every
  single tick, unconditionally, and the model *still* didn't visibly turn in that same test — so both
  available direct-write mechanisms (mesh-relative, actor-level-with-bTeleport) were effectively exercised
  together and both came back negative.
- **Decision: reverted entirely.** Removed the whole `turnInPlaceYawOffset` growth/decay block and the
  now-unused `RemotePlayer::turnInPlaceYawOffset` field's write path (field itself left in `state.hpp`,
  currently dead — harmless to remove later, not urgent). Replaced with a simple hard clamp of the final
  `renderAimYaw` to ±90 (`proxy_manager.cpp`, right before the `on_process_event_post` write) — the exact
  same AnimBP-property-only mechanism Pitch already uses reliably, no mesh/actor transform involved at all.
  User explicitly agreed to defer real body-rotation to a later, more-finished-state session rather than
  keep fighting the transform-refresh problem live.

**Found via live bytecode trace what the real game actually does for ADS body rotation — a genuinely new,
promising lead for that later session, not yet attempted against a proxy.** Traced from
`BP_PlayerCharacter_C::MC_ADS` (an 18-byte stub, same "jump into the flat ubergraph at an `EntryPoint` int
constant" pattern as every other event on this class — jumps to absolute ubergraph offset 173204). Extracted
and resolved (via `resolve_ptr.flag`) all 77 unique function addresses reachable from that entry point;
`Function /Script/Engine.Actor:K2_SetActorRotation` was among them (two call sites, offsets `0x6c14`/checked
the first in detail). The exact sequence at the first call site:

```
LerpResult      = KismetMathLibrary::Lerp(A, B, Alpha)          ; func=0x3f1419e0, 3 local-var params
YawOnlyRotator  = KismetMathLibrary::MakeRotator(0.0, 0.0, LerpResult)  ; func=0x3f1420e0 — Roll=0, Pitch=0, Yaw=LerpResult
                  Actor->K2_SetActorRotation(YawOnlyRotator, bSweep=false)  ; func=0x232f6ec0
```

I.e. the real game **does not use a mesh-relative-rotation trick at all** — it lerps toward a target yaw
every tick and calls the plain `K2_SetActorRotation(rotator, bSweep)` directly on the actor, with Pitch/Roll
explicitly zeroed (Yaw-only). `A`/`B`/`Alpha` (the Lerp's three inputs) weren't traced further this session —
worth doing next time, almost certainly "current body yaw", "target/aim yaw", and a time-based or
speed-based alpha constant respectively, given the Lerp-toward-target-every-tick shape.

**Why this is a promising lead despite the actor-level approach already having failed once in this
session's proxy testing:** the proxy's failed test used `K2_SetActorLocationAndRotation` (a *different*,
more complex function — takes a location AND rotation together, plus an explicit `bTeleport` flag, which was
passed `true`) via `ProxyManager::teleport_proxy`, not the plain `K2_SetActorRotation(rotator, bSweep)` the
real game actually uses. `bTeleport=true` specifically may interact with `bOrientRotationToMovement` or the
movement component's rotation-blending very differently than a bare rotation-only call with no teleport
semantics at all — this was never isolated as its own variable. **No native binding for
`K2_SetActorRotation` exists in the vendored UE4SS SDK** (`vendor/ue4ss-stub/include/RC/Unreal/AActor.hpp`
only has `K2_SetActorLocationAndRotation`) — trying this would need a raw `ProcessEvent` reflection call
(param struct: `FRotator NewRotation` + `bool bSweep`, no output params — simpler than `SetRelativeRotation`'s
4-param signature, which was the other alternative considered and rejected as too risky to attempt blind).
**Next step, when picking this back up:** call `K2_SetActorRotation` directly (via reflection) on the proxy
actor instead of/alongside `teleport_proxy`'s combined call, with `bSweep=false` matching the real game
exactly, and see whether *that* specific call — isolated from `bTeleport`/location-setting — actually
sticks while the proxy is stationary. This needs live 2-client testing to verify either way.

**Pistol/melee aim-yaw gap — narrowed down, not solved: confirmed `GetAimOffset` itself is weapon-type
agnostic, so the gap is in the AnimGraph's node wiring, not Kismet bytecode.** `Player_AnimBP.hpp` has
**six** separate `FAnimNode_RotationOffsetBlendSpace` instances (`AnimGraphNode_RotationOffsetBlendSpace`
through `_5`) but only **one** shared `Pitch`/`Yaw` property pair (0x5AF0/0x5AF8) — strongly suggesting each
of the six feeds from a different `AimOffset` blendspace asset (matching the multiple exported assets,
`RifleIronsightsAimOffset`/`PistolCrouchIronsightsAimOffset`/etc.) but all nominally reading the same
`Pitch`/`Yaw` scratch properties, gated by whichever one the `BlendSpaceInt`-driven state machine currently
has selected. Dumped and fully decoded `GetAimOffset`'s bytecode (747 bytes, clean disassembly, no unhandled
opcodes) expecting to find weapon-type branching — there is none. It's a straight-line computation, structurally
identical to the already-known Pitch-tracking logic (`SelectRotator` → `NormalizedDeltaRotator` → `RInterpTo`
→ `BreakRotator`, matching the RInterpTo finding from earlier sessions), run twice into two instance-variable
outputs, with zero `EX_Jump`/conditional instructions gating any of it by weapon type or `BlendSpaceInt`. This
rules out `GetAimOffset` itself as the place a pistol/melee-specific fix would go — **the real gap is almost
certainly in which of the six `RotationOffsetBlendSpace` AnimGraph nodes is actually wired into the active
pose for a given `BlendSpaceInt`, and whether all six genuinely read the shared `Pitch`/`Yaw` pair or only
some of them do.** That's AnimGraph node-linkage information, not Kismet function bytecode — not reachable
with the tools used this session (`kismet_disasm.py` decodes `UFunction::Script` byte arrays; compiled
AnimGraph node wiring is a separate, baked data structure on the `AnimBlueprintGeneratedClass` that this
tool has no visibility into). **Solved — this is a content/asset limitation, not a sync bug, no fix needed on the mod side.** Read each of
the six `RotationOffsetBlendSpace` nodes' `BlendSpace` member directly off the live `Player_AnimBP_C`
instance via `mem_dump.flag`'s `abs <addr> <count>` form (struct base offsets from `Player_AnimBP.hpp`, `+0x68`
for `FAnimNode_BlendSpacePlayer::BlendSpace` per `AnimGraphRuntime.hpp`'s struct layout — that field lives on
the base class, not on `FAnimNode_RotationOffsetBlendSpace` itself), then resolved each resulting pointer via
`resolve_ptr.flag`:

| Node | `BlendSpace` asset | Type |
|---|---|---|
| `AnimGraphNode_RotationOffsetBlendSpace` (no suffix) | `RifleIronsightsAimOffset` | `AimOffsetBlendSpace` (2D) |
| `_1` | `HG_Aim` | `AimOffsetBlendSpace1D` |
| `_2` | `RifleIronsightsAimOffset` (same as no-suffix) | `AimOffsetBlendSpace` (2D) |
| `_3` | `PistolCrouchIronsightsAimOffset` | `AimOffsetBlendSpace1D` |
| `_4` | `RifleIronsightsAimOffset` (same again) | `AimOffsetBlendSpace` (2D) |
| `_5` | `PistolCrouchIronsightsAimOffset` (same as `_3`) | `AimOffsetBlendSpace1D` |

Three unique assets across six nodes (rifle's asset reused 3×, presumably one per standing/crouched/other
substate that all still reference the same blendspace; pistol's two assets — standing vs crouched — each
reused once more). **No dedicated melee blendspace exists anywhere in this set at all** — melee simply has
no `RotationOffsetBlendSpace` node in the graph, full stop, matching the live "yaw doesn't work for melee"
report exactly and explaining it completely: there's nothing there to feed. **Both pistol-family assets are
`AimOffsetBlendSpace1D`, not the 2D `AimOffsetBlendSpace` rifle uses** — a `1D` blendspace has a single blend
axis by construction, so it can only ever be driven by one of Pitch/Yaw, not both, regardless of what the
sync code writes to the other. This matches the live "yaw doesn't work for pistols" report as a real,
inherent asset limitation rather than a wiring bug — nothing on the sync side can add a second axis to an
asset authored with only one. (Not confirmed this session which single property, if either, the 1D nodes
actually read — `Pitch` is the more likely candidate for a handgun since vertical aim-offset while roughly
facing the target is the more common use case, but this wasn't traced further.) **Conclusion: rifle-class
weapons (`BlendSpaceInt=1`, i.e. two-handed) are the only case where full Pitch+Yaw aim-offset sync is even
meaningful given this game's own authored content — pistol is inherently partial by asset design, and melee
has no aim-offset content at all.** No further mod-side work needed here; this was a content-boundary
question, not a bug to fix.

## Session 58 (overnight, autonomous research per user directive, 2026-08-13)

User went to bed with instructions to work down the remaining-work backlog as far as possible via
research/RE, without live 2-client deployment (PC1's splash screen needs a manual keypress the user
isn't present to give, and PC2 wasn't running). This session is documentation-and-build-checked-only;
nothing below has been live-verified on a running game yet.

### Phase D item: Hunger/Thirst/Stamina/Radiation sync — found a real, scoped gap and fixed it

Backlog framed this as "needs a native hook to wire it into the sync loop," implying nothing existed
yet. On inspection, most of it already did, from an earlier undocumented session:

- **Read side** (`read_local_progress()`, `mod.cpp` ~line 323) already reads health, hunger, thirst,
  stamina, and radiation from the correct live components every tick:
  `HungerThirstComponent` at `pawn+0x7F8` (hunger `+0xC8`, thirst `+0xD8`), `StaminaComponent` at
  `pawn+0x800` (`+0xC8`), `RadiationComponent` at `pawn+0x7F0` (`+0xC8`), `MedicalComponent` at
  `pawn+0x7D0` (health `+0xD0`).
- **Send side** (`send_profile_revision()`, ~line 1051) already packs all of these into the
  `PlayerProgress` payload and sends it every 30s as a `ProfileRevision` frame.
- **Persistence** (`gateway.js`) already saves that payload verbatim
  (`db.saveProgress(playerId, revision, f.payload)`) — no field-by-field extraction, so nothing was
  lossy here either.
- **The actual gap**: on rejoin, `gateway.js` replays the saved payload back as a
  `PlayerProgressRestore` frame (confirmed by reading `gateway.js` directly — this is the *only*
  message sent carrying restored state; no separate `PlayerDamage` or vitals frame follows it). The
  mod's handler for that case (`mod.cpp`, `MsgType::PlayerProgressRestore`) decoded the frame,
  applied `posX/posY/posZ/yaw` to `pendingTeleport`, and even logged `prog->health`/`prog->level` —
  but never wrote health, hunger, thirst, stamina, or radiation back into any live component. A
  player who disconnects and rejoins would have their saved position restored but their vitals reset
  to whatever the base game assigns a freshly-loaded pawn, silently discarding everything this mod
  had been dutifully persisting.

This matches (and answers) the user's own question before going to bed — "does it matter if the pawn
dies? it'll starve or die of thirst" — yes: without this fix, hunger/thirst/stamina/radiation state
picked up at 30s intervals was being thrown away on every reconnect regardless of what it actually
was.

**Fix applied** (`mod.cpp`, `PlayerProgressRestore` case): added a direct write-back block using the
exact same offsets `read_local_progress()` already reads, in the same one-shot raw-pointer-write style
the adjacent `PlayerDamage` handler already uses for `MedicalComponent.Health` (`pawn+0x7D0+0xD0`).
Health is now restored the same way (current only — `PlayerProgress` has no max-health field, so
maximum is left untouched, matching what data is actually available). `xmake build` succeeds cleanly
(only pre-existing unrelated warnings). **Not live-tested** — needs a real disconnect/reconnect pass
on the 2-client setup to confirm the writes land and are visible in-game (health/hunger/thirst/
stamina/radiation bars should reflect the pre-disconnect values immediately after rejoin, not reset).
Flagging this explicitly since every other native-offset write in this project has needed at least one
live verification pass before being trusted (see project memory on this).

**On whether this conflicts with EasyMultiSave's own local restore**: SDO isn't real multiplayer —
each client runs its own local single-player game instance, and the mod only relays/persists state
across sessions server-side. EMS's `OnPlayerLoaded` fires once, locally, whenever that client's own
game finishes loading a save (i.e. at game launch / save-slot selection), which happens before the
bridge's TCP join handshake completes and `PlayerProgressRestore` gets sent — so this write isn't
racing EMS, it's deliberately overwriting whatever EMS just loaded locally with the mod's own
last-synced server-side truth, the same way position-restore already does. That's the correct
behavior for this architecture (keeps the shared/persisted state authoritative even if the local save
diverged — different local save slot, crash-and-relaunch, etc.), not a bug to reconcile.

### Phase D item: EasyMultiSave hooks — re-confirmed blocked on live testing, not IDA

Revisited backlog item 19 ("EasyMultiSave hooks, pending IDA investigation"). Sessions 22 and 23
already did the actual IDA work here and reached a firm conclusion, re-verified directly against the
live IDB in Session 23 rather than just trusted from the writeup: `LoadPlayerActorsCustom` and the
`EmsLoadPlayerComplete__DelegateSignature` broadcast site both have zero code cross-references — the
delegate broadcast is resolved generically by the Blueprint VM at runtime, not a fixed call site static
disassembly can pin down. Session 23's conclusion stands: "hooking via live Lua (binding to the
delegate, or hooking the BFL node that wraps it) remains the only viable path." There is no further
static/IDA work available on this item — the backlog description ("pending IDA investigation") is
stale; it should read "pending live hook experiment" instead. Since this requires a running game to
attempt (register a Lua delegate binding or hook `BFL_SaveGames_C`'s wrapping node and observe whether
it fires), it's blocked on the same live-testing constraint as everything else tonight and is being
left for a live session rather than attempted blind.

Worth noting the original motivating use case (Session 22: hook `OnPlayerLoaded` as the trigger to
send an early, accurate `ProfileRevision` right when local save data finishes loading, instead of
waiting for the periodic 30s tick) is a **different** concern from tonight's restore-write-back fix
above — that one's about the SEND-side timing of the mod's own state being fresh soon after launch,
not about applying server state to local components. Both remain valid, independent reasons to revisit
this hook in a live session.

### Phase D item: Building-piece/container entity spawning — re-checked, found a bigger gap than expected

Backlog item 17 framed this as "may be much closer to done than `03`'s stale TODO list suggests" —
worth re-checking against the native-actor-spawn technique proven for player proxies. Traced the
current state fully (pure repo code-reading, no IDA needed for this part):

**Server side** (`host-agent.js`, `InteractionRequest`/`BUILD` case, ~line 399): already sends
`EntitySpawn`/`EntityState` frames with `kind: EntityKind.PlacedStructure` when a build interaction
comes in, with an explicit comment acknowledging the client can't render it yet. `req.pieceTypeId` (a
plain number, presumably an index into whatever the local building-menu enumerates) is stashed as a
numeric string in the wire format's `itemId` field — there's no `classPath` field sent, and no
resolution table anywhere mapping a `pieceTypeId` to an actual Blueprint class.

**Client side** (`mod.cpp`): grepped for `InteractionRequest`/`pieceTypeId` — **zero matches**.
`InteractionRequest` exists only as an enum value in `protocol.hpp` (`= 26`); there is no encode call,
no send site, and therefore **no hook on the local player's own build/place action at all**. This is a
bigger gap than "spawning doesn't render yet" — nothing captures a local build action or transmits it
in the first place. Building-piece sync is 100% unimplemented on the sending side, not just the
rendering side.

**Client-side receive path** (`entity_manager.cpp`): `tick()` calls `spawn_entity_actor()` for every
entity kind uniformly with no branch on `entity.kind` at all (confirmed by reading the loop directly —
`entity.actor = spawn_entity_actor(world, entity);` unconditionally). `spawn_entity_actor()` itself
only implements the `GroundItem` path: it requires `entity.itemId` to resolve via
`resolve_item_asset()` to a real `UJigsawItem_DataAsset_C*` (see line ~204,
`if (!world || entity.itemId.empty()) return nullptr;`). A `PlacedStructure` entity's `itemId` field
holds a bare numeric string (the `pieceTypeId` placeholder above), which isn't a real Jigsaw item ID,
so `resolve_item_asset()` would fail and the function bails — meaning even if the server-side send
path existed, nothing would spawn client-side today. Also worth noting as a minor inefficiency (not a
crash risk): `tick()`'s retry gate (`lastActorAttemptUs`, 2s cooldown) means a `PlacedStructure` entity
that can never spawn just retries silently forever rather than giving up — harmless but wasteful.

**Good news found**: an earlier, superseded design doc (`research/03_modding_framework_plan.md`,
predates the current `item-asset-cache`-based `spawn_entity_actor` implementation) already scoped a
plausible approach and even identified real class paths — `Buildable_MASTER_C`
(`/Game/Blueprints/BuildingSystem/Actors/Buildable_MASTER.Buildable_MASTER_C`) as a **generic "any
building piece" master class**, and `BuildableMaster_Container_C`
(`.../Containers/BuildableMaster_Container.BuildableMaster_Container_C`) as the equivalent for
containers — the same "spawn one generic class, then configure the specific variant post-spawn" shape
already proven working for `GroundItem` pickups today (`spawn_entity_actor` spawns a generic pickup
class, then calls `PickupBuildFromGround`/sets `BP_JigPickupComponent`'s `ItemDataAsset` to configure
the specific item afterward). If `Buildable_MASTER_C` follows the same shape, the native-actor-spawn
technique proven for proxies should apply directly to it too — spawning isn't the open question.

**What's actually still unknown and blocks implementation**: what identifies a *specific* piece within
the generic master class (e.g. wood wall vs. stone foundation vs. which container variant), and what
Blueprint-callable configure function (if any, mirroring `PickupBuildFromGround`) needs to be called
post-spawn to apply it. That requires either (a) IDA investigation of `Buildable_MASTER_C`'s
construction/configuration flow, or (b) capturing whatever piece-identifying data the local player's
own in-game build action already has at the moment of building (mirroring how item pickups pass a
resolvable `itemId` today) instead of the currently-meaningless numeric `pieceTypeId` placeholder.
Both of those need either live IDA work or a live 2-client capture session. IDA's MCP plugin hung
mid-session on an overly broad `search_text(code_only=false)` call (matches this project's known
instability pattern for string searches) before this specific class's construction flow could be
traced — it recovered on its own after ~20-25 minutes rather than needing a manual restart (see
updated [[sdo-ida-debug-stability]]), so this was picked back up once it did; see below for the
result. There's still no live game session available to capture the local build action, so only the
static half of this question could be pursued tonight.

**Net revision to backlog item 17**: this is genuinely bigger than "re-check something that might
already work" — it's three sequential gaps (no client-side send hook → no receive-side
kind-branching → no piece-identity resolution), not one.

**Update, same session, after IDA recovered**: the piece-identity gap above is now **fully resolved**
— turned out to need neither IDA nor a live session, just reading headers already sitting in
`research/CXXHeaderDump/`. The "generic master class + post-spawn configure" theory (from the
superseded `03_modding_framework_plan.md`) was wrong: `pak_all_files.txt` shows every individual piece
(`Buildable_WoodenWall`, `Buildable_Bed`, `Buildable_AmmoCrate`, 100+ others) is its own concrete
Blueprint class, not a shared master configured after spawn. The real mechanism, traced end to end:

- `BP_PlayerCharacter.hpp`: `BuildingComponent` (`UBuildingComponent_C*`) sits at pawn `+0x7E0`.
- `BuildingComponent.hpp`: `DARef` (`UJigsawItem_DataAsset_C*`) at `+0x298` — **the same DataAsset
  class already used for inventory items**, holding whichever piece is currently selected in build
  mode. `Event_LaunchBuildMode(UJigsawItem_DataAsset_C* DA, ...)` sets it when the player opens the
  build menu and picks a piece; `SpawnBuild(FTransform)`/`Svr_SpawnBuild(FTransform)` are the actual
  placement calls.
- `JigsawItem_DataAsset.hpp`: alongside the already-used `ItemId` (`+0x30`) and `PickupClass`
  (`+0x128`, used for dropped-item rendering), there's a **`TSubclassOf<AActor> BuildActorClass` at
  `+0x4E8`** — the exact concrete class to spawn for that piece when built, sitting on the exact same
  DataAsset object `resolve_item_asset()` already fetches by itemId today.

So the fix is a straight extension of the already-proven `GroundItem` pattern, not a new mechanism:
capture `BuildingComponent->DARef->ItemId` at the moment of a real build (`SpawnBuild`/
`Svr_SpawnBuild` firing is the natural hook point, mirroring how montage-change polling works
elsewhere in this project) and send that real itemId instead of today's meaningless numeric
`pieceTypeId`; server-side, forward it verbatim instead of re-deriving anything; client receive-side,
`spawn_entity_actor()` needs only a kind-branch for `PlacedStructure` that reads `BuildActorClass`
instead of `PickupClass` off the same resolved DataAsset — everything else (itemId resolution,
`spawn_actor_at`'s `BeginDeferredActorSpawnFromClass`/`FinishSpawning` technique) is already written
and working. Remaining unknowns are now purely mechanical (confirm `Svr_SpawnBuild` vs `SpawnBuild` is
the right hook point, confirm containers follow the identical pattern via `BuildableMaster_Container_C`
subclasses) — no more research needed here, this is ready to implement and live-test next session.

### Phase D items: Zombie AI + Vehicle health/fuel — found complete offsets with zero IDA needed, plus a scope correction

IDA being down turned out not to matter for this one. Backlog items 15/16 assumed native IDA work was
required because Session 15's live `PropertyDumper` reflection scan against `BP_Zombie_C` and
`Vehicle_PickupTruck_C` directly found nothing (`UberGraphFrame` only / zero properties). The mistake:
those are **intermediate/leaf Blueprint classes in the inheritance chain**, and the real runtime actors
use subclasses further down that were never checked. `research/CXXHeaderDump/` (FModel static exports,
already sitting in the repo from a past session, not live reflection) has the full chain:

**Zombies**: `ABP_Zombie_C : ABP_MainEnemy_C : ABP_Human_C : ABP_AI_C : ACharacter` — every one of those
intermediate classes really is empty except `ABP_Human_C` (`IsBaseActorDead?` @ `+0x68A`) and `ABP_AI_C`
(`Dead` bool @ `+0x688`) — so Session 15's specific finding on `BP_Zombie_C` itself wasn't wrong, it was
just the wrong class to check. The class actually placed in the world is one level further down:
**`ABP_MasterZombie_C : ABP_Zombie_C`** (confirmed `BP_ZombieBoss_C` also derives from it, same offset —
this is the shared base for all zombie variants), which carries a dedicated
`class UDamageComponent_C* DamageComponent` at `+0x698`, plus `IsDead?` (`+0x718`), `DamageToDo`
(`+0x710`, the zombie's own attack damage), `AttackSocketName`/`AttackMontage`/`AttackSound`/
`Start_Attacking` (attack animation/trigger data, `+0x6E0`–`+0x700`), `HealthDeviation` (`+0x808`), and
Blueprint functions `AttackTrace()`/`Death(AActor*, bool Headshot)`/`AttackPlayer()`/
`ReceiveAnyDamage(...)`/`Damage_Object(...)` plus a bindable `DeathEvent(bool Headshot)` delegate.
`UDamageComponent_C` itself (`research/CXXHeaderDump/DamageComponent.hpp`) is fully self-contained and
game-generic (also used by non-zombie damageable actors per its name): `CurrentHealth` (`+0xB8`),
`MaxHealth` (`+0xC0`), `StandardHealthValue` (`+0xB0`), plus Blueprint-callable `SetHealth(double,
bool)`, `GetValues(double&,double&,double&)`, `IsAlive?(bool&)` — i.e. reading (or even writing, via
the real `SetHealth` UFunction instead of a raw memory write) zombie health needs exactly the same
`actor+0x698` → `+0xB8`/`+0xC0` two-hop pattern already used for the player's own
`MedicalComponent` (`pawn+0x7D0` → `+0xD0`), just with different offsets. No IDA required — this was
sitting in already-exported headers the whole time.

**Vehicles**: same shape. `ABP_VehicleMaster_C : AWheeledVehiclePawn` (the real shared base — matches
Session 12's "17 vehicle types all inherit from `BP_VehicleMaster_C`") carries
`class UVehicleHealthComponent_C* VehicleHealthComponent` at `+0x390` and
`class UVehicleFuelComponent_C* FuelComponent` at `+0x3D0`. `VehicleHealthComponent`
(`research/CXXHeaderDump/VehicleHealthComponent.hpp`): `CurrentHealth`/`MaxHealth` at `+0xC0`/`+0xC8`.
`VehicleFuelComponent` (`VehicleFuelComponent.hpp`): `CurrentFuel`/`MaxFuel` at `+0xC8`/`+0xD0`, plus an
`Empty` bool at `+0xE8`. Also visible directly on `ABP_VehicleMaster_C`: `EngineOn?` (`+0x4D8`),
`VehicleColor` (`+0x4F0`). No dedicated "occupants" array was found on this class — seating/driver
state for `AWheeledVehiclePawn` is a native engine concept (seat possession), not a custom Blueprint
field, so occupant sync would read possession/seat state generically rather than a game-specific
offset — not investigated further tonight, lower priority than health/fuel.

**Important scope correction to the backlog** (this affects how items 15/16 should be read going
forward): the backlog says "position sync already works (server-authoritative via
`EntitySpawn`/`EntityDespawn`)" for both zombies and vehicles, implying only health/fuel/behavior is
missing. Grepped the entire repo for actual usages of `EntityKind::Zombie` / `EntityKind::Vehicle`
outside the protocol layer itself: **the only real hits are in `tests/protocol_roundtrip.cpp`** (unit
test fixtures). Neither `host-agent.js`/`gateway.js` (server) nor `mod.cpp` (client) ever actually
construct a `Zombie` or `Vehicle` entity descriptor/state anywhere — `mod.cpp` only ever *receives*
`EntitySpawn`/`EntityState` frames (confirmed by grep — no send call exists), and the server-side only
does this for `PlacedStructure` (via `InteractionRequest`/BUILD, itself unreachable from the client
tonight per the building-piece section above) and `GroundItem` (via pickup/drop). **Zombie and vehicle
sync — position included, not just health — is 100% unimplemented end-to-end today**, not "position
done, health missing." The wire protocol and `EntityKind` enum values are ready (including a `health`
field already on `EntityState`), and position sync would reuse the exact same generic
`K2_GetActorLocation`/`K2_GetActorRotation` no-Blueprint-reflection-needed technique already
proven for `GroundItem`/proxy actors (Session 15b) — so this isn't a hard problem, just a genuinely
unstarted one. Tonight's offset findings above remove what would have been the biggest unknown (native
health/fuel access) for whenever this gets picked up.

## Session 59 (2026-08-14) — Server-authoritative rewrite: real multiplayer game server, zombie simulation live-verified

Following up on the redesign approved this session (unified entity-based world model, server as
genuine authority for shared world state): built and live-tested the first real slice end to end —
GroundItem/PlacedStructure migrated onto a new structured `entities` table (replacing the old
opaque-blob replay model in `db.js`), and a full server-authoritative zombie simulation
(`server/src/world/zombie-simulation.js`) built from the real 913-zone/7-archetype data extracted
from `Exports/` (`server/scripts/extract-zombie-data.js` — spot-checked against every number
manually verified earlier this session: Infected=200hp, Boss=4000hp, Nightmare difficulty
multipliers, spawn zone counts 831/19/7/56 all matched exactly).

**Found and fixed a real protocol bug via testing, not live debugging**: `gateway.js`'s shared
request-routing block normalized `entityId` to the sender's own entity for every message type in
that group — correct for `DeathRequest`/`InteractionRequest`/etc. (entityId = "my own entity"), but
wrong for `ZombieAttackRequest` (entityId = "the zombie being targeted"), silently clobbering the
attack target with the attacker's own id. This is the exact same bug class already documented for
`ItemPickupRequest` on 2026-08-12 — `ZombieAttackRequest` needed the same carve-out. Caught by the
new integration test going straight to a timeout with zero server-side error log at all, matching
that original bug's signature exactly. Fixed by moving `ZombieAttackRequest` into the same
already-existing exception case as `ItemPickupRequest`.

**Live 2-client deploy** (both machines, full pipeline: server restarted with all new code, DLL
rebuilt and deployed to both, fresh tickets, both joined): `suppress_zombie_spawners()` — the
riskiest untested piece, a brand-new native call (`SetIsSpawningStopped`/`KillSpawnedActors` on
every live `ABP_AISpawner_Master_C`-derived instance) across the whole level — worked flawlessly on
the first live attempt: **857/857 spawner instances found and stopped** (831 zombie + 19 hound + 7
boss, matching the extracted data exactly), zero crashes. `resolve_class_by_name`'s primary method
(`FindObject` with the class's full package path, no live instance required) also resolved
`BP_Zombie_Roamer_C` correctly on the first attempt.

**Real crash, found and fixed live**: after 4 successful zombie proxy spawns, both connected clients
hard-crashed simultaneously (`EXCEPTION_ACCESS_VIOLATION reading address 0xFFFFFFFFFFFFFFFF` — the
exact bit pattern of this file's own `ANY_PACKAGE` sentinel, `(UObject*)-1`) inside a defensive
`GetController`/`UnPossess` check that had been added specifically because auto-possession behavior
was unconfirmed. The crash *confirmed* the thing it was checking for — `BeginDeferredActorSpawnFromClass`
really does auto-possess these Character-derived classes with a real AI Controller by default,
unlike the Pawn-based player proxy — but the raw single-`UObject*`-field params struct used to read
`GetController()`'s return value has a wrong ABI assumption somewhere, and both clients crashed on
the 5th spawn (both had received the same broadcast, since zombie EntityState currently goes to
every connected client rather than being filtered per-recipient by relevance — a known gap, not new
tonight). Fixed by removing the check entirely rather than guessing again blind — zombie proxies may
now act semi-independently (their own AI Controller still possesses them) rather than being pure
puppets, a real but far lesser problem than a hard crash. Redeployed; zombie spawning continued past
the exact point that crashed before (including the identical `eid=3676812055053758644` that crashed
it originally) with sustained activity and zero further crashes on either client.

**Confirmed working live, end to end**: spawner suppression (857/857), server-side spawn-zone
relevance scoping + spawning + roam movement (multiple zombies spawned and persisted across both
clients), class resolution (`FindObject` full-path method), proxy actor spawning and rendering on
both clients simultaneously. **Not yet exercised live**: the damage/death path (`ZombieAttackRequest`
send-side — Phase 4's melee hit-detection hook, `BP_MeleePickup_C::MeleeTrace`, is still unbuilt, so
nothing in-game currently triggers an attack request).

### Crash root-cause found and fixed (same session, continued debugging)

The `GetController`/`UnPossess` removal (above) didn't actually fix anything — the very next live
test crashed **again**, both clients simultaneously: PC1 hit the identical
`0xFFFFFFFFFFFFFFFF` address as the original crash (proving that reflection call was never the real
cause), PC2 hit a different, genuine heap address (`0x00000001e31a0b88`). Re-examined what actually
runs on *every* tick for an already-spawned zombie, not just once at spawn time:
`EntityManager::on_entity_state`'s teleport call (`K2_SetActorLocationAndRotation` on
`entity.actor`, added this session to apply the server's roam-movement broadcasts to an
already-spawned proxy). Working theory: with the zombie's real AI Controller left possessing it
(confirmed auto-possession happens, per the original crash), its native AI/behavior tree is
genuinely running and can destroy/replace the actor through completely ordinary gameplay logic with
no involvement from this mod at all — leaving `entity.actor` dangling, and the next roam-movement
`EntityState` update calls `K2_SetActorLocationAndRotation` on freed memory. A real heap address
(PC2) and a reused region that still happened to contain the `-1` sentinel pattern from an earlier
`resolve_class_by_name` call (PC1) are both consistent with one dangling-pointer bug, not two
unrelated ones.

**Fix**: `actor->SetActorTickEnabled(false)` right after spawn — a real, already-linked C++ export
(`vendor/ue4ss-stub/include/RC/Unreal/AActor.hpp`), same safety category as
`SetActorHiddenInGame`/`K2_SetActorLocationAndRotation` (both already proven safe elsewhere in this
file), not a raw-reflection guess like the `GetController` attempt. Stops the actor's own
tick-driven logic without touching the controller at all. **Live-verified**: redeployed to both
clients, multiple zombies spawned continuously and sustained well past every previous crash point
(the exact same recurring entity IDs from prior crashed sessions — `eid=15612827715167089370`,
`eid=15153992375002062009`, etc. — all spawned cleanly this time), both game processes remained
running, zero disconnects, zero further crashes.

**Lesson for future native-hook work in this project**: a Blueprint-native actor that's a puppet in
name only (still fully possessed and ticking) can affect its own lifetime through completely
ordinary gameplay systems this mod never touches — any code holding a raw actor pointer across
multiple ticks needs to either stop that actor's own agency (`SetActorTickEnabled(false)`, proven
here) or treat the pointer as untrusted between ticks. Two guessed fixes in a row (both plausible,
both wrong) is a good reminder that "this specific reflection call looks risky" and "this is
actually the risky part" aren't always the same code — the real cause was three call-sites away
from the code that first crashed.

### Update: the "fixed" conclusion above was wrong — full crash saga, disabled pending real debugging

The `SetActorTickEnabled` fix above was declared confirmed-stable after one good test window. It
wasn't. Zombie proxy spawning crashed **five times total** across **three different guessed
fixes** this session, and the final state is **disabled**, not fixed. Full sequence, so a future
session doesn't have to re-derive it:

1. `GetController`/`UnPossess` (raw UFUNCTION reflection guess) — crashed both clients,
   `0xFFFFFFFFFFFFFFFF` (this file's own `ANY_PACKAGE` sentinel bit pattern).
2. Removed that block entirely — crashed again immediately: PC1 hit the *identical*
   `0xFFFFFFFFFFFFFFFF` address (proving step 1 was never the real cause), PC2 hit a different, real
   heap address, `0x00000001e31a0b88`, consistently, across multiple separate crash incidents and
   fresh process launches.
3. Added `SetActorTickEnabled(false)` on the pawn + read `Controller` via
   `GetValuePtrByPropertyNameInChain` and disabled its tick too — attached a live IDA debugger
   (`ida_dbg.attach_process`, `set_debugger_options` clearing `DOPT_LIB_BPT`/`DOPT_THREAD_BPT` per
   this project's established precaution) to catch the *next* crash with real diagnostics instead of
   guessing again. It worked: caught a real fault, repeated identically —
   **`EXCEPTION_ACCESS_VIOLATION` attempting to *execute* code at `0x900000003`**, not just read bad
   memory. That's the signature of a virtual call through a pointer that isn't a real object of the
   assumed type — strong evidence `GetValuePtrByPropertyNameInChain(L"Controller")` doesn't resolve
   the way `find_local_pawn()`'s `Controller`→`Pawn` read does for this class, and the garbage
   result's `SetActorTickEnabled()` call is what actually froze the game (the debugger kept re-
   hitting the same instruction in a loop rather than terminating, matching the observed "froze,
   not crashed" symptom). **IDA's own MCP plugin then hung** while trying to suspend the frozen
   process for further inspection — required force-killing both `ida.exe` and the game process
   directly (`Stop-Process -Force`; killing IDA released the OS debug port, at which point the game
   terminated normally too).
4. Removed the `Controller` property code entirely, kept only pawn-level `SetActorTickEnabled(false)`
   (the smallest version tested) — **changed nothing**: both clients crashed again at their own
   exact same per-machine addresses as step 2 (PC1 `0xFFFFFFFFFFFFFFFF`, PC2
   `0x00000001e31a0b88`). This is the single most important finding of the whole saga: it
   conclusively rules out *every* piece of controller-handling code across all three fix attempts —
   whatever's actually wrong was present and unchanged in every version tested.

**Current leading theory, not yet tested**: `resolve_class_by_name`'s primary resolution method
(`UObjectGlobals::FindObject(nullptr, kAnyPackage, fullPath)`) passes `Class=nullptr`, meaning no
type filtering at all — it matches *any* object anywhere by name, not specifically a `UClass`, and
the result is cast to `UClass*` unconditionally. If the full path string ever matches a same-named
object that isn't really the class (e.g. a `UBlueprint` editor asset sharing a name/path fragment
with its generated class), every subsequent spawn using that cached, wrong-typed pointer would be
working with garbage class metadata — plausibly explaining crashes that vary in exactly which spawn
count triggers them (4, 5, 8+, not a fixed N) rather than a clean, deterministic per-spawn fault.
The `on_entity_state` position-teleport call added this session (`K2_SetActorLocationAndRotation`
on `entity.actor` for already-spawned zombies) is the other remaining untested suspect. Neither has
been isolated or tested yet.

**Current state**: `spawn_zombie_actor` returns `nullptr` unconditionally (`src/entity_manager.cpp`)
— zombies are simulated server-side and spawner-suppressed client-side exactly as designed, but
never rendered as proxies. This is deliberate and should stay this way until a real, dedicated
debugging session — not another live guess-and-redeploy cycle — actually isolates the cause.
Suggested first steps for that session: (a) type-check `resolve_class_by_name`'s `FindObject`
result before casting (compare its own reported class/type against an expected sentinel, if the
reflection API exposes one) rather than trusting any non-null hit; (b) SEH-wrap the
`on_entity_state` teleport call as a cheap, low-effort mitigation regardless of root cause; (c) if
attaching IDA again, do it *before* relaunching the game and *before* the crash point, with
`DOPT_LIB_BPT`/`DOPT_THREAD_BPT` cleared from the start, not scrambled together mid-session — the
plugin hang this session happened while trying to interact with an already-frozen target, which may
itself be avoidable with a cleaner setup.

**What stayed rock-solid across all five crashes, zero exceptions**: `suppress_zombie_spawners()`
(857/857 spawner instances found and stopped on every single attempt), the entire server-side
`ZombieSimulation` (spawning, relevance scoping, roam movement, damage/death — all still fully unit-
and integration-tested per the earlier section of this session), and the `entities` table/GroundItem/
PlacedStructure migration work. The bug is narrowly scoped to zombie *proxy rendering* specifically.

**Post-crash hardening prep (build-checked only, not deployed/live-tested)**, done after the user
stepped away:
- `resolve_class_by_name` (`proxy_manager.cpp`): swapped the two resolution methods' priority.
  `FindFirstOf(shortName)`+`get_class_private()` (spawn_proxy's own live-proven technique) is now
  primary; the untyped `FindObject(nullptr, ANY_PACKAGE, fullPath)` call is now the fallback. On
  review, that method's original justification — "same call shape as the live-tested
  `find_object_by_short_name()`" — was only actually validated for *short* names like
  `"Chr_MaleHair3"`; a full path with an embedded class-name suffix was never actually confirmed to
  match the same way, and this is the current leading suspect for the whole crash saga (untyped
  match → wrong object type → corruption on later use).
- `on_entity_state`'s zombie position-teleport call is now SEH-wrapped (local `seh_invoke` copy,
  same shape as `mod.cpp`'s) — if `entity.actor` ever goes dangling between spawn and a later
  position update, this now drops the update instead of crashing.
- Zombie proxy spawning itself is still fully disabled regardless of the above — these are
  defense-in-depth improvements for whenever it's re-enabled with real live testing, not a claim
  that the root cause is fixed.

### Vehicle sync — built same session, deliberately avoiding every risky technique above

Unlike zombies, vehicles don't need spawning, class resolution, or any AI-controller handling at
all — every client already has its own native instance of all 56 vehicles from level load (fixed
spawn points, `world-data.json`), so "syncing" a vehicle just means finding the client's own
already-existing actor and (eventually) applying canonical health/fuel to it, not creating anything.

**Server** (`host-agent.js`): new `_broadcastVehicles()`, called once when the host authenticates
(not on a repeating timer — nothing about a stationary vehicle changes on its own the way a zombie's
position does). Assigns each of the 56 known spawn points a stable `entityId`
(`stableNumericId(\`vehicle-spawn:${zone.name}\`)`, same helper player entityIds already use) and
sends `EntitySpawn`/`EntityState` (kind=Vehicle, health defaulted to the real confirmed base value,
100 — not a guess) through the exact same wire messages every other entity kind uses. Since this
fires before any real client has joined, nobody catches it live — verified via a new integration
test that a joining client receives it through the existing late-joiner replay path
(`gateway.js`'s `_replayTo`, reconstructed from the unified `entities` table) instead, the same
mechanism every other entity kind already relies on. 70 integration tests now (was 65), all passing
except the pre-existing unrelated ticket-replay failure.

**Client** (`entity_manager.cpp`): new `find_native_vehicle_near()` — checks all 17 confirmed
vehicle Blueprint classes (`pak_all_files.txt`, cross-referenced against Session 12's "17 vehicle
types" finding) via `FindAllOf`, nearest-position match against the server's known spawn point,
exact same technique as the already-proven `find_and_claim_native_pickup` (GroundItem's own-drop
claiming) — no `BeginDeferredActorSpawnFromClass`, no class resolution, no possession/tick concerns
whatsoever. `spawn_entity_actor`'s dispatch needed a real fix while wiring this in: the function's
very first line rejected any entity with an empty `itemId`, which every vehicle has (it isn't a
JigsawItem, an archetype name, or anything else this mod resolves a class from) — moved the Vehicle
check ahead of that gate, mirroring how Zombie already needed to bypass it.

**Deliberately NOT done yet, even though it compiles fine**: applying the server's health/fuel
value to the found native actor's `VehicleHealthComponent`/`VehicleFuelComponent`
(`actor+0x390`/`+0x3D0` → `CurrentHealth`/`CurrentFuel`, offsets from two nights ago's FModel
research, never live-verified as read *or* write targets). After tonight, writing to a new raw
memory offset without live verification available isn't a risk worth taking blind — this stays as
"find and adopt the actor" only until a live session can verify the read/write offsets are correct
before trusting them.

### Two real bugs found and fixed once live testing resumed

**Bug 1 — unbounded zombie entity tracking.** Deploying the disabled-zombie-rendering build and
letting it run for a real session (not a quick crash-test cycle) surfaced a genuinely new issue:
`EntityManager::tick()`'s per-entity retry throttle (2s) bounds *frequency* but not *count* — every
zombie the server's ongoing simulation ever broadcasts stays in `g_state().entities` forever (a
disabled spawn attempt never succeeds and nothing ever removes the entry), so the retry set grows
unboundedly over a session. Found live: ~7900 accumulated retry attempts, plausibly contributing to
a symptom that looked like a full freeze (needed a force-close) but was actually a render-thread
hang — `debug.log` kept showing fresh activity throughout, meaning the game/logic thread was still
ticking; only the render side was stuck, consistent with UE5's separate game/render threads under
a growing per-tick workload. Fixed at the ingestion point, not just the retry point: both
`on_entity_descriptor` and `on_entity_state` now skip Zombie-kind entities entirely while rendering
is disabled, so the map never grows for them in the first place.

**Bug 2 — vehicle `FindAllOf` cost multiplied by entity count, not class count.** The first live
deploy of the vehicle sync work reproduced a freeze almost immediately after join, on both
machines — a very different timing signature from bug 1 (which needed a long session to build up).
Root cause: `find_native_vehicle_near()` called `FindAllOf` (already documented elsewhere in this
codebase as expensive enough alone to cause 1-2 FPS) once per vehicle class (17) **for every
individual vehicle entity**, every 2s, until each of the ~56 vehicles resolved — up to ~950 full
world scans every 2 seconds, all 56 running concurrently right after the replay-delivered batch of
vehicle entities arrived at once. Same cost class as the already-documented 1-2 FPS case, just
~56x larger, and it lines up exactly with an immediate-post-join freeze instead of a slow one.
Fixed by scanning each class exactly once per client session (module-level cache, same pattern as
`resolve_zombie_archetype_class`'s own caching) instead of once per entity — vehicles are static
from level load, so the candidate list never needs to change during a session. **Live-confirmed
fixed**: redeployed to both machines, the one-time scan logged exactly once
(`find_native_vehicle_near: one-time scan found 0 native vehicle instances` — 0 because neither
player was near a loaded vehicle at the time, not an error), both clients joined and stayed stable
with ongoing activity, no freeze.

Both fixes are live-verified working as of this session's end. Zombie proxy rendering itself
remains disabled (unrelated to either bug above — a third, still-unsolved issue, see the crash
saga earlier in this section).

## Same session, later — ADS turn-in-place actually landed; new strong evidence on the still-open mesh-fragmentation bug

**ADS turn-in-place, finally working.** Picked back up the lead from Session 57 (real game's
`BP_PlayerCharacter_C::MC_ADS` calls the plain `K2_SetActorRotation(FRotator, bSweep=false)`
directly on the actor, Pitch/Roll zeroed — never tried against a proxy before). Added it via the
same `GetFunctionByNameInChain`/`ProcessEvent` reflection pattern already proven throughout this
file (e.g. `read_local_aim_pitch`'s `GetControlRotation` call — same `FRotator`-typed param
struct), reinstated `RemotePlayer::turnInPlaceYawOffset` with the corrected grow/decay logic
Session 57 had already worked out (based on the *raw* aim/body gap, not the post-correction gap,
which was self-defeating), and gated the whole thing on `movState & 0x02` (`IsADS`) so it doesn't
run during ordinary hip-fire look-around — the real game's equivalent lives specifically in
`MC_ADS`, not general movement.

First deploy had it running *alongside* the old mesh-relative `apply_proxy_body_yaw_safe` write
instead of replacing it — both fired every tick, and since actor rotation was previously always
silently overridden by `bOrientRotationToMovement` (which is *why* the mesh workaround existed in
the first place), the mesh's own baked ~-90 degree art-alignment baseline was now being added on
top of an actor that was *also* genuinely rotating for the first time. Live-reported: ADS body
turned ~90 degrees further clockwise than it should, and ordinary movement made the character
appear to run backward (the actor's facing no longer tracked its own velocity the way
`CharacterMovementComponent`'s directional blend expects). Removed the now-redundant mesh-relative
write entirely — `apply_proxy_actor_rotation_safe` alone is sufficient once actor rotation actually
works. Redeployed; user confirmed **working correctly** on both symptoms afterward.

**Mesh-fragmentation bug — strong new evidence, still not fixed.** Live-reported and screenshotted
mid-session: not a single item detaching, but **two separate proxies simultaneously showing their
entire mesh broken into disconnected floating pieces** — head, torso, hands, and legs all
separated from each other rather than posed as one skeleton, on both proxies at once, on both
machines. This is a stronger, clearer version of Session 54's "proxy meshes intermittently detach
entirely (not a pose issue)" report. Two data points worth carrying into the next dedicated
session on this:

- The `reassert_no_interact()` fix from earlier tonight (continuously re-asserting
  `JigSetCanInteract(false,false)` every `sync_equipment()` pass, confirmed still present and
  active in the build running when this was observed) did **not** prevent it — disproves "physics
  gets silently re-enabled later" as a sufficient explanation on its own, or at least shows this
  specific countermeasure doesn't stop it.
- **Both proxies broke at the same time**, on both viewers — not an isolated one-off render glitch
  on a single client. Whatever's happening is either synchronized to some shared trigger (both
  players' game state doing the same thing at once — reconnect, entity-replay burst, a
  world/day-night event) or is a rendering-thread-level issue independent of any specific player's
  own state. Worth checking client-side logs for what else was happening at that exact timestamp
  next time this is caught.

No live fix attempted — this matches the already-logged conclusion that the remaining candidates
(`ControlRig`'s internal RigVM bytecode, or the state-machine transition-rule functions gating
`BlendListByBool`/`StateMachine`) need real decompilation work, not another live guess-and-redeploy
cycle.

**Critical reframe, same live report, more detail extracted.** Session 55 already found real
evidence `ControlRig` isn't the mechanism (the game's only ControlRig asset is an 8-node head/spine
look-offset only — no weapon/body relevance at all), so that lead should be deprioritized, not
re-tried. More importantly: **the missing meshes are not the same on both machines, and at least
one is on the LOCAL, non-proxy character**: PC1 saw their *own* respirator (a local equipped item,
not a proxy) gone; PC2 saw most of their own meshes gone; PC2 separately saw PC1's night-vision
goggles floating/detached (that one *is* a proxy-rendered item, PC1's NVG as seen on PC2's screen).

This is a real problem for the entire "proxy-rendering bug" framing this has been investigated
under since Session 54 — **local equipment rendering is pure native game logic; nothing this mod's
`sync_equipment`/`reassert_no_interact`/proxy code touches runs against a player's own local
character at all.** If a local player's own gear genuinely detaches with no mod code anywhere near
that code path, either (a) this is a base-game bug unrelated to the mod, exposed/coincidental with
multiplayer sessions, or (b) some other still-unidentified mod code path (network receive handling,
a hook installed broadly rather than proxy-scoped, e.g. `on_process_event_post`) is reaching further
than intended. **Next step before any further ControlRig/state-machine work: check whether this
reproduces in vanilla singleplayer with the mod not loaded at all** — if it does, this is a base-game
bug outside this project's ability to fix, and investigation should stop; if it doesn't, the search
needs to shift from "proxy rendering" to "what does this mod's code touch that could affect the
local player's own character," which is a much shorter list (network receive handlers, any hook
installed without a proxy-only guard) than anything explored so far.

**2026-08-14: confirmed never happens in vanilla singleplayer — this is mod-caused, and a real
candidate found.** Ruled out `sync_equipment`/`reassert_no_interact`/every `JigSetCanInteract`/
`SetSimulatePhysics` call site directly — all live in `proxy_manager.cpp`, all only ever called
against `player.proxyActor` or its tracked visual-actor children, never the local pawn. Also
audited every `RemotePlayer::*VisualActor` pointer's lifecycle for the stale-pointer-reuse theory
(destroy a proxy's item, forget to null the field, UE's allocator reuses that address for something
unrelated, a later call hits the wrong object) — `destroy_actor_safe()` correctly nulls every
pointer on every destroy path, including the SEH-caught-crash path, and the one place that was
provably leaking actors (disconnect, before the already-documented Session-? fix: "destroying the
proxy itself does not cascade-destroy attached *actors* in UE5, only attached *components*") now
cleans up every visual actor before erasing player state. Both theories checked out clean —
neither explains gear disappearing on the *local* player.

Found a real candidate instead: `PlayerProgressRestore`'s handler (`mod.cpp` ~line 2732) calls
`pawn->K2_SetActorLocationAndRotation(newLoc, newRot, false, hit, /*bTeleport=*/true)` on the local
player's own pawn, unconditionally, on every single join/reconnect — this is the *same call shape*
(`K2_SetActorLocationAndRotation` with `bTeleport=true`) Session 57 already flagged as suspect for
not reliably preserving attached *actors'* relative transforms (as opposed to attached
*components'*, which do move correctly) — and every equipped item is exactly that: a separately
attached actor, per the same UE distinction the disconnect-cleanup fix above already had to work
around. This call had never been examined for local-player side effects before, only ever discussed
in the context of proxy rendering. Tonight had an unusually high number of reconnects (every
redeploy cycle triggers one), which lines up with why this was the session this finally got caught.

**Mitigation applied (not a confirmed fix — the underlying UE behavior, if this really is the
cause, isn't something this project can change): gated the teleport on actual distance.** It was
firing unconditionally even when the saved position was already right where the local game's own
save had the player, which is pure risk for zero benefit. Added a 500-unit distance-squared
threshold (same value already used for the proxy teleport-vs-smooth cutover elsewhere) — skips the
call entirely when it wouldn't move the player meaningfully, keeps it for a genuine cross-session
rejoin. Builds clean. **Not yet live-verified** — same problem as the mesh-fragmentation bug itself,
no on-demand repro exists, so this needs to be watched over further sessions rather than confirmed
immediately. If items keep detaching even with teleports mostly skipped now, this theory is wrong
and the search should move to auditing every other hook this mod installs without a proxy-only
guard (starting with `on_process_event_post`, which fires for every character's `AnimInstance`, not
just proxies') for anything that could reach a local player's own equipped-item actors.

**Split-call fix deployed and both machines joined clean.** Redeployed with
`K2_SetActorLocation`+`K2_SetActorRotation` split (see above) — both PC1 and PC2 joined without the
arms/mesh-detachment bug recurring on this join, and no reported position/rotation issues from the
split call itself. Given this bug has no reliable on-demand repro, this is "held up on this one
join," not a confirmed fix — worth continued watching over further play rather than closing this
out. If it recurs despite this, the next candidate is auditing every hook this mod installs without
a proxy-only guard (`on_process_event_post` first) for anything that could reach a local player's
own equipped-item actors.

**Same session, right after the clean-join check above: proxy-side detachment recurred.** PC1's
eyewear (slot 2, `Glasses`) fell off as rendered on PC2's screen — this is specifically the
`sync_equipment`/proxy-rendering path, not the local-player teleport path just fixed above (that fix
doesn't apply here at all: PC2 is rendering PC1's proxy, nothing to do with PC2's own pawn's
position). Confirms **two separate mechanisms are both live**, not one bug with one cause:

1. Local player's own gear disappearing on join (candidate fix: split
   `K2_SetActorLocation`/`K2_SetActorRotation` call, held up on the one join tested so far).
2. Proxy-rendered gear detaching independent of joins (this occurrence) — already disproven earlier
   tonight that `reassert_no_interact()` (continuously re-asserting `JigSetCanInteract`) is
   sufficient to prevent it, despite being active and correctly proxy-scoped.

No further live fix attempted for (2) this session — same reasoning as before: the remaining real
candidates (`ControlRig`'s RigVM bytecode — though already weakly deprioritized by Session 55's
finding that the game's only ControlRig asset has no body-part relevance, so may not even apply
here since that finding was about grip *pose*, not physical detachment; or the state-machine
transition-rule functions) need actual decompilation work, not another live guess-and-redeploy.
Next dedicated session on this should start by re-confirming whether ControlRig's dismissal
(Session 55) truly rules it out for *this* symptom specifically (detachment) or only for the
grip-pose symptom it was checked against — that distinction was never explicitly re-tested.

**Local detachment recurred again, this time with no join/reconnect involved.** PC2's own hair
came off, locally, mid-session (no relaunch reported before this). This weakens the
`PlayerProgressRestore`-teleport theory as the sole cause of local-side detachment — that path only
ever fires once per join, and no join happened here. Either the split-call fix only ever addressed
part of the local-detachment cases (join-triggered ones) while a separate, still-unidentified
mechanism causes it mid-session too, or the teleport theory was never really it and the one clean
join earlier was coincidence, not confirmation.

**Session conclusion on the whole mesh/item-detachment bug family**: three distinct occurrences
across one evening (PC1 respirator + "most of PC2's meshes" pre-fix, PC1 eyewear-on-proxy
post-fix, PC2 hair-locally-mid-session post-fix) across at least two mechanisms (local and proxy),
neither fully explained nor fixed. Stop attempting further live guesses this session — every
candidate tried (`reassert_no_interact` continuous reassertion, distance-gated teleport, split
location/rotation calls) has been disproven or left unconfirmed. This needs a genuine dedicated
session: systematic reproduction attempts (does frequency correlate with session length? equip
changes? any specific action?) plus real decompilation (RigVM bytecode or state-machine transition
rules) rather than more reactive live patching.

**Debugging infrastructure added for next time it happens: `attach_health` monitor
(`mod.cpp`).** Runs unconditionally (no flag trigger needed) every ~2s against the local pawn's
Mesh and any connected proxy's Mesh, snapshotting `AttachChildren` (same offsets/SEH-guard pattern
already proven by `read_local_weapon_attachments`) and diffing against the previous snapshot. Any
child present last check but gone this check gets logged immediately —
`attach_health: <local|proxyN> DETACHED ptr=0x... name=<GetFullName()> (prevCount=X curCount=Y)` —
giving a real timestamp and identity the moment something detaches, instead of only a several-
seconds-later visual report with no diagnostic trail. Builds clean, deployed to both machines,
running now. Next occurrence should be immediately followed by a `debug.log` check on both machines
around the reported time for these lines — this is the first time this bug will have any actual
data trail to investigate from.

**`attach_health` monitor: zero hits on PC2's hair loss — useful negative result.** Checked PC2's
full `debug.log` (290,972 lines, confirmed live/current via matching recent timestamps) for
`attach_health` — no matches at all. The monitor only watches `Mesh->AttachChildren` (the mechanism
equipped items use), so hair not showing up there means **hair isn't a separately spawned+attached
actor at all** — consistent with `send_pawn_appearance`'s own log line treating it as a named
property (`hairMesh=Chr_MaleHair3`), which points at a dedicated body-part component (e.g.
`HairMeshComponent`) whose mesh asset is getting cleared/swapped rather than physically detaching.
The current monitor can't see that class of change. **Next concrete step, not attempted yet**:
extend `attach_health` (or add a sibling check) to read known body-part component properties
directly (need to find their actual reflected names first — `HairMeshComponent` is a guess based on
the `hairMesh` wire-field name, not confirmed) and snapshot/diff their `SkeletalMesh` asset pointer
the same way, rather than only watching the generic attachment list. This also means the equipped-
item detachment reports (glasses, respirator) and the body-part reports (hair, "most of PC2's
meshes") may turn out to be two genuinely different bugs sharing only a visual symptom, not one
mechanism — worth keeping that distinction sharp going forward rather than assuming they're the
same root cause.

**Debugging infrastructure extended: component-drift monitor + weapon-attachment-level coverage.**
Two additions to the `attach_health` monitor, same session:
1. `component_drift` — covers body parts that are named direct properties, not attached actors
   (`HairMesh`/`BeardMesh`/`head`/`Torso`/`Arms`/`Hands`/`Legs`/`Feet`, real offsets from
   `BP_PlayerCharacter.hpp`), snapshotting `RelativeLocation` (proven `+0x128` offset, already used
   throughout `proxy_manager.cpp`) and logging a `component_drift: DRIFTED` line if any moves more
   than 30 units between 2s checks. Directly targets the negative result above (hair loss produced
   zero `attach_health` hits since it isn't an attach/detach event at all).
2. `attach_health` extended one level deeper: since UE attaches component-to-component (not
   actor-to-actor), a child returned by walking `Mesh`'s `AttachChildren` is itself a full
   USceneComponent with its own `AttachChildren` — the same scan now also recurses into each
   equipped item's own attachments (weapon scopes/mags/suppressors), reusing 100% of the existing
   scan/diff code, just called one level deeper per item. User's own suggestion, prompted by the
   monitor working for one class of item.

Both build clean, deployed to both machines, running now. Between the two, this covers: equipped
item detachment (existing), weapon-attachment detachment (new), and body-part-component drift
(new) — should catch nearly every variant of this bug family reported tonight except any mechanism
that neither attaches/detaches nor moves the component's own transform (e.g. purely toggling
visibility or clearing a mesh asset while leaving the transform untouched — not yet covered, would
need a further extension if a future report doesn't show up in either log).

**Debugging infrastructure, third extension same session: mesh-asset-clear detection.** PC2's own
hands vanishing locally produced zero hits on both existing monitors — meaning the component
neither detached (attach_health) nor moved (component_drift), so "gone" apparently means invisible/
cleared, not displaced. Extended `do_component_drift_scan` to also read each tracked component's
`SkeletalMesh`/`SkeletalMeshAsset` property (reflection name lookup, not a raw offset — tries both
since the exact property name for this engine version wasn't independently confirmed, a wrong guess
just returns null harmlessly) and log `component_drift: MESH ASSET CLEARED` if it transitions from
set to null between checks. Builds clean, deployed both machines, running now.

Between all three extensions, the monitor now covers every mechanism that's actually been reported
tonight: physical detachment (attach_health, both top-level items and one level into their own
attachments), transform drift (component_drift positional), and asset-clear/invisibility
(component_drift mesh-asset). If the next occurrence still produces zero hits across all of these,
that would be a strong signal the mechanism is something structurally different again (e.g. render
state without any of these three signatures) rather than needing a fourth reactive extension.

**Session conclusion, this bug family: likely a rendering glitch, not a state bug.** Four more live
occurrences checked against the fully-verified-working monitor (heartbeat confirmed alive,
correctly scoped, `localMeshChildren=5`/`proxiesScanned=1`, and it DID catch real events earlier —
the Torso/Legs/Feet mesh-asset-clear at 10:02:50): PC1's AK15 suppressor (weapon-attachment level,
the specific gap the "one level deeper" extension was built for), PC2's forearms (a directly-tracked
`component_drift` name), plus the backpack and NVG reports from just before. **Zero hits across all
of them, on top of confirmed-alive instrumentation.** This is a meaningful negative result, not a
coverage gap: attachment state, RelativeLocation, and mesh-asset pointers all provably did not
change on the affected components when these were reported.

**Working theory going forward: this is a transient rendering glitch (bad skinning/pose
interpolation, LOD pop, a briefly-wrong frame), not game-state corruption.** None of tonight's
reports described an item staying detached — always "fell off," checked moments later, story moves
on — consistent with something that self-corrects visually without ever actually changing the
underlying component data reflection can see. If true, this is not fixable via this mod's code at
all (nothing to instrument further — reflection sees state, never the rendered frame) and is likely
an inherent engine/performance quirk exposed by having more skeletal meshes actively animating at
once in multiplayer, not a bug in this project's code. Revisit this conclusion only if a future
report describes something *staying* broken rather than a momentary visual glitch — that would
argue for real state corruption after all and justify picking the state-bug investigation back up.

**Debugging infrastructure, fourth extension: item-level mesh-asset-clear (still-attached case).**
"PC1's AK is gone but attachments are there" — the parent (weapon) mesh going invisible/cleared
while its still-attached child attachments (scope/mag/suppressor) remain visible, distinct from
every prior check: not a top-level detach (never left AttachChildren), not a fixed body-part
component (weapons aren't in component_drift's named list). Extended `do_attach_health_scan` to
also track each *currently attached* item's own `SkeletalMesh`/`SkeletalMeshAsset`/`StaticMesh`
asset pointer (tries all three names since items can be either mesh type), keyed by a persistent
map so it survives across ticks even as the attached-children list itself is unchanged. Logs
`attach_health: <label> item ptr=... MESH ASSET CLEARED (was set, now null, still attached)`.
Builds clean, deployed both machines, running now.

This walks back part of the earlier "likely just a rendering glitch" conclusion — "gun gone, scope
still floating there" is a real, reportable, presumably-persisting state (not obviously a
single-frame render hiccup) and now has direct instrumentation for the first time. If this new
check also comes back empty on the next occurrence, that would be much stronger evidence for the
rendering-glitch theory than anything checked so far; if it fires, that's the first real lead this
session into an actual root cause.

**False-positive found and fixed in the monitor itself.** The 10:18:21 cascade (local Torso/Legs/
Feet + two item mesh-clears, all in the same millisecond, right after two `localPawn=NULL`
heartbeats) was a **monitoring artifact, not a real bug**: PC2 had respawned/reconnected, and the
brand-new pawn's components hadn't finished initializing yet (legitimately null for a moment) —
but the snapshot state was keyed by the fixed label `"local"`, which doesn't change across a
respawn even though the underlying `AActor*` does, so the first check against the new pawn compared
against the *old* pawn's fully-loaded baseline and false-flagged a "CLEARED." Also worth noting:
this cascade was entirely on PC2's own local pawn, not PC1's proxy — meaning the actually-reported
bug this round (PC1's pants/vest, seen on PC2's screen) still produced zero real hits.

Fixed via `reset_label_snapshots_if_actor_changed()`: tracks which `AActor*` each label
("local"/"proxyN") last referred to, and wipes every snapshot entry under that label the moment the
underlying actor identity changes (respawn, reconnect, or a proxy despawning/respawning), so the
first post-change check is treated as a fresh baseline instead of a comparison. Logs
`attach_health: <label> actor changed (respawn/reconnect), snapshot baseline reset` when this
happens, so a future respawn is visible in the log as a labeled event rather than silently
resetting or (as before the fix) producing false detections. Builds clean, deployed both machines,
running now.

**Updated read on this whole bug family**: three real reports since the monitor went live
(backpack, NVG, suppressor, forearms, AK, pants/vest — six distinct reports total) have now
produced exactly one non-false-positive-suspect hit (the original 10:02:50 Torso/Legs/Feet clear,
which happened mid-session with no adjacent respawn in the log — worth re-examining once more data
exists, since the same false-positive class could theoretically apply if a respawn happened before
logging started that session, though the surrounding heartbeats around 10:02:50 don't show a NULL
gap the way 10:18:21's did). Net effect: still no confirmed real state-level cause found tonight,
but the diagnostic pipeline itself is now meaningfully more trustworthy, and any future hit is much
more likely to be a genuine signal rather than snapshot-continuity noise.

**Second false-positive gap found and fixed: `s_itemHadMesh` wasn't reset on actor change.**
Same session, immediately after the first respawn-reset fix shipped — PC1's own AK15 Acog produced
the identical cascade shape (heartbeat NULL, then a burst of "MESH ASSET CLEARED (still attached)"
the instant the pawn came back), proving the per-label reset alone wasn't sufficient.
`s_itemHadMesh` is keyed by raw item pointer (not by label, since an item's identity isn't tied to
which character owns it), so it couldn't be scoped/cleared by label the way the other two maps
were — a freshly-spawned item after a respawn can get the exact same memory address a
just-destroyed item held, and the stale `true` entry at that address made the new (still-
initializing) item look like a real clear. Fixed with a full `s_itemHadMesh.clear()` whenever
either label's actor-change reset fires — simplest correct option, cheap to rebuild over the next
tick or two, not worth scoping further given how rarely this actually triggers (only on
respawn/reconnect, at most a couple times per session). Builds clean, deployed both machines,
running now.

At this point every hit this monitor has ever produced across the whole session has turned out to
be a respawn-timing false positive once traced back — PC2's "pants gone again" report (same session,
right after this fix shipped) should be the first real test of whether the diagnostic pipeline is
now actually trustworthy, or whether a third gap exists.

**BREAKTHROUGH: first confirmed real, persistent occurrence — not a false positive, not a
transient respawn artifact.** Live screenshot from PC1 (their own local character) shows base
shirt/pants, bald head, only a knife — jacket, backpack, helmet, gloves all gone. Cross-referenced
against `debug.log`: the `11:34:05.944` cascade (attach_health item clears + component_drift
Torso/Arms/Legs/Feet, same shape as every prior "false positive") — but this time, every heartbeat
since (`11:34:09` through at least `11:38:10`, all `localPawn=found localMeshChildren=14`) shows
**no recovery at all**. This state has now persisted for 4+ minutes and is still visible live. User
confirmed explicitly: not an intentional gear-rack strip, the items actually fell off.

**No "actor changed" reset ever fired around this event** — confirmed via grep, zero matches —
meaning this is the *same* character actor throughout, not a new pawn from a respawn. So the
mechanism isn't "died and respawned"; it's something that makes `find_local_pawn()`'s reflection
lookup fail to resolve for ~27 seconds (heartbeats at `11:33:38` and earlier show `NULL`) while the
underlying actor never actually changed, and the moment resolution succeeds again, its equipment
reads as cleared and — critically, unlike every prior occurrence — never gets reapplied.

**New working theory: level streaming (entering/exiting a building), not death/respawn.** The
screenshot shows an interior room. A sub-level load/unload transition is a plausible explanation for
a *temporary* reflection-lookup failure on the same actor (components briefly unregistered/
re-registered during streaming) without the character having actually died — and would explain why
no equipment-reapply logic runs afterward, since the game has no reason to treat a level-streaming
boundary as an event that needs re-equipping. Not yet confirmed; needs deliberate testing (walk in
and out of a building repeatedly while watching `debug.log`) in a focused future session — this
session found the correlation but is far too deep already to safely chase a live fix on the local
player's own core equipment logic right now.

**This is the most concrete, actionable lead of the entire night** — first real repro trigger
candidate (building entry/exit) for a bug that had zero reliable trigger all session up to this
point, on top of a now-verified-trustworthy diagnostic pipeline (both false-positive classes found
and fixed earlier this same session). Next session should start here: confirm the level-streaming
correlation with a deliberate building-entry/exit test, then look at what actually differs between
a normal respawn's equipment-reapply path (which does work, per every earlier false-positive
"recovery" implied by heartbeats going back to normal counts) and whatever this transition invokes
instead (likely: nothing invokes it at all, which is the actual bug).

**Root cause of the 11:33:36 UE4SS reinit and the later 11:52:25 crash: likely a KVM switch.**
UE4SS.log confirmed a full restart (member-offset dump, "Starting mods", "Event loop start") at
11:33:36 — not explained by the Keybinds mod (only Ctrl+key dev-tool bindings, nothing close to a
reload) or `EnableHotReloadSystem` (confirmed `0`, disabled), and no crash dump exists at that exact
timestamp. User identified the likely real cause after the fact: swapping their KVM switch mid-
session — a plausible trigger for both this reinit and the actual crash logged at 11:52:25
(`UECC-Windows-CEDC60E0...`). This is an environment/hardware interaction, not a mod or base-game
bug — closes out tonight's mesh-detachment investigation on the actual root cause rather than an
unexplained mystery.

**Does this change the mesh/equipment-clearing conclusion?** Only partially. The KVM-triggered
reinit explains *why* `find_local_pawn()` failed to resolve for ~30s and why equipment read as
cleared right after (the mod's own state, including whatever normally reapplies equipment,
got wiped by the reinit — a real, understood mechanism now, not a mystery). It does **not**
explain every other report from tonight (backpack/NVG/suppressor/forearms/AK/acog/pants — none
of which coincided with a logged `on_unreal_init` reentry in the same way). Those may be a
different, still-unexplained mechanism, or could turn out to share a cause once checked the same
way (grep each future report's timestamp against `on_unreal_init: entered` before assuming a new
bug). **Next session: when a similar report comes in, check for an `on_unreal_init` reentry first
before treating it as a fresh state-corruption investigation** — it's now a known, real, checkable
candidate cause, not console-log noise.

**Retest after KVM-avoidance: mixed result, one real gap found in the diagnostic itself.** User
restarted both clients deliberately without the KVM switch. PC1's log shows the relaunch clearly
(`on_unreal_init: entered` at `11:59:18.150`), followed by a `local` cascade at `12:00:01.999` — but
this one **self-corrected** (heartbeat 20s later already back to normal `localMeshChildren=14`),
consistent with a normal "hasn't finished spawning in yet" transient, not a bug — first evidence the
diagnostic can now tell a real problem (the earlier persistent KVM-linked one) apart from benign
join transients.

The actually-reported bug this round — PC2's hair, rendered on PC1's screen (proxy-level) — produced
**zero** `component_drift` hits despite `proxiesScanned=1` confirmed active in the heartbeat the
whole time. Since `component_drift` needs to observe a component as set at least once before it can
ever detect a later clear, zero hits ever (not even a false one) strongly suggests the `HairMesh`
property lookup is silently failing specifically on proxy actors — never establishing a baseline at
all, not "nothing changed." This is a genuine, previously-unknown gap in the diagnostic itself
(distinct from either false-positive class already fixed), not investigated further this session —
next step: add explicit logging when `GetValuePtrByPropertyNameInChain` for these named body-part
components returns null on a *proxy* specifically, to confirm whether resolution fails entirely for
proxies or only sometimes.

**Correction to the "self-corrected" read above: recovery was only partial.** User later reported
weapon attachments, eyebrows, and mouth all missing locally on PC1 — traced back to the *same*
`12:00:01.999` cascade from the `11:59:18` relaunch, not a new event. The heartbeat showing a
normal count (`localMeshChildren=14`) 20 seconds later only confirms the top-level attachment
*list* came back, not that each item's own mesh asset was actually reapplied — the three item
pointers that cleared under `local>1cb3c0100` (the weapon's own attachment sub-tree) apparently
never recovered even though the list-level structure did. Eyebrows/Mouth aren't in
`component_drift`'s tracked name list at all (only `HairMesh`/`BeardMesh`/`head`/`Torso`/`Arms`/
`Hands`/`Legs`/`Feet`), so their loss can't be independently confirmed by this session's
instrumentation, but fits the same broad-clear-at-join pattern.

**Updated overall read for the night: most reports likely trace back to join/relaunch events, with
incomplete self-recovery, not random mid-session corruption.** This reframes nearly everything
reported across the whole session — many, maybe most, of tonight's "X fell off" reports happened
within a few minutes of some relaunch/redeploy (this session did an unusually large number of them).
The mechanism is now much better scoped for a future session: **on join, some equipment-reapply
step runs for body/base components but appears to skip or fail for weapon-attachment sub-items and
at least two untracked face components (Eyebrows, Mouth)** — a real, targeted lead (find what runs
on join for equipment restoration and why its coverage is incomplete) rather than a vague
"something sometimes breaks" investigation. Next session should extend `component_drift`'s tracked
list to include `Mouth`/`EyebrowsMesh` (real properties, `BP_PlayerCharacter.hpp` @0x0740/@0x0790)
and specifically trace what equipment-restoration logic runs at join to find the actual gap.

**Diagnostic coverage extended: `Mouth`/`EyebrowsMesh` added to `component_drift`'s tracked list**
after the weapon-attachments/eyebrows/mouth report. Builds clean, deployed both machines (user
relaunching manually via desktop shortcuts from here on, not via script). No further live code
changes attempted this session beyond this — a real fix needs bytecode-level tracing of whatever
native function handles equipment-restoration at join, which is dedicated-session work, not
something to guess at after this many hours live.

**Pattern noticed across the whole session: proxy-level reports always produce zero hits, local-
pawn cascades always get caught.** Every single "X fell off, seen on someone else's screen" report
tonight (glasses, NVG, hair, AK, helmet, suppressor) — zero matching log lines under any `proxyN`
label, while `local` cascades tied to `on_unreal_init` reentries kept getting caught reliably. Too
consistent across this many independent reports to be "proxies just don't have this problem" —
added `proxy0Children` to the 30s heartbeat (previously only reported local's count) to check
whether the proxy-side scan is finding a sane `AttachChildren` count at all, or silently finding
~0 every time, which would mean `Mesh` resolution or the whole scan is quietly failing specifically
for proxy actors. Builds clean, deployed both machines. Next heartbeat check (or next proxy-level
report) should make this immediately visible either way.

**Proxy-scan-integrity question resolved: the scan works, hypothesis disproven.** New heartbeat
field confirmed `proxy0Children=5` — a real, non-zero count — meaning the proxy-side
`attach_health`/`component_drift` scan genuinely finds real attached items on a proxy's Mesh, not
silently reading empty every time. This rules out "the scan is broken for proxies" as the
explanation for the all-session pattern of zero hits on every proxy-level report (glasses, NVG,
hair, AK, helmet, suppressor).

**Remaining candidates for that pattern, not investigated further this session**: (1) proxy
connections may cycle (reconnect/redespawn) more often than local, repeatedly resetting the
snapshot baseline (via `reset_label_snapshots_if_actor_changed`) right before a real change would
otherwise be caught — plausible given how many redeploys/relaunches happened tonight, each of which
briefly drops and respawns every proxy; (2) proxy-rendered items may detach via a genuinely
different, still-uninstrumented mechanism than local items do. Good stopping point for tonight —
the proxy-scan-integrity question that was the last open thread is now answered either way, and
further work here needs either a longer, redeploy-free play session (to test candidate 1 cleanly)
or fresh instrumentation ideas (for candidate 2), both better suited to a fresh session than
continuing to extend tonight's diagnostic incrementally.

**Real crash: PC2 hit EXCEPTION_ACCESS_VIOLATION writing 0x4ec.** Last log line before the crash
was `on_process_event_post`'s own `aim_write` diagnostic — this function's Pitch/Yaw/IsCrouching/
IsADS/Falling write block has run unguarded (no SEH) since Session 53, writing through a
per-frame-resolved `AnimInstance` pointer that can go stale if the owning proxy is mid-destroy/
respawn — the exact stale-pointer-during-proxy-lifecycle race this project has SEH-hardened
everywhere else, just never here. The near-null write address (`0x4ec`) is consistent with that
theory but not confirmed via live debugger attach (none was active). Split the write logic into a
trampoline (`do_aim_write`/`AimWriteCtx`) and wrapped it in `seh_invoke`, matching the established
pattern throughout this codebase — a safe, low-risk hardening regardless of whether this exact
theory is the confirmed cause. Builds clean, deployed both machines. If it crashes again despite
this, the SEH catch will at least produce a `caught via SEH` log line, giving a real diagnostic
signal instead of a silent hard crash — meaningful progress either way.

**FINAL, well-supported conclusion for the night: this is 100% reproducible, tied specifically to
the join-time teleport in `PlayerProgressRestore`.** Fourth-plus occurrence of the identical cascade
shape tonight, every single time immediately following an `on_unreal_init` relaunch — no exceptions
observed once this was actually tracked. This is no longer "no reliable repro"; it now reads as
"happens on literally every join."

**Real mechanism, most likely candidate**: `PlayerProgressRestore`'s teleport (this session's own
code, `mod.cpp` ~line 2750) fires on every fresh join, since a genuinely fresh join's saved position
is always far enough from wherever the engine's default spawn point put the pawn to clear the
500-unit distance gate added earlier tonight. Splitting the call into `K2_SetActorLocation`+
`K2_SetActorRotation` (also this session, to fix the ADS/movement-backward symptoms) fixed those
specific problems, but likely never addressed the deeper issue: teleporting the actor root — by
*any* method — may not reliably carry attached actors' (weapons, clothing, accessories, and
possibly body-part components too) relative transforms with it. This matches Session 57's original
finding almost exactly, just now confirmed to apply to the *local* player's own join-teleport, not
only proxy rendering.

**This is the single most concrete, actionable lead of the entire session.** Next session should
test directly: does the equipment cascade still happen on a join where the distance gate skips the
teleport entirely (i.e., rejoin near the same saved position)? If yes, the teleport theory is wrong.
If no cascade occurs, that's near-certain confirmation, and the fix becomes "either skip the
join-teleport when the engine's own default spawn is close enough to not bother, or find a way to
force attached actors to recompute their transforms immediately after any teleport that must
happen" — both concrete, testable engineering tasks, not open-ended investigation.

**Likely real root cause found: unprotected raw-memory vitals write-back on the SAME join event as
the teleport, timed too early.** User asked to re-audit the whole attach chain for native-vs-custom
usage — that search led to `PlayerProgressRestore`'s vitals write-back (`mod.cpp`, added in an
earlier session, self-documented as "NOT live-verified") instead: it writes `double`s directly
through four raw pointer chains (`pawn+0x7D0/0x7F8/0x800/0x7F0` for Medical/HungerThirst/Stamina/
Radiation components) the *instant* `find_local_pawn()` first succeeds after a join — **zero SEH
protection, zero validity check beyond non-null, and fires on the exact same event** this session
already showed leaves the pawn mid-initialization (`preChildrenCount=4` vs. a normal ~14, logged
just a few lines earlier in the same join sequence via `join_teleport`'s bracketing logs). A raw
write through a pointer fetched that early — even if non-null — has real potential to hit an
object that isn't what the offset assumes yet, plausibly explaining collateral damage to unrelated
components (equipment, body parts) without ever touching them directly. Also a strong candidate for
the real cause of tonight's earlier confirmed crash (`EXCEPTION_ACCESS_VIOLATION` writing `0x4ec`) —
`aim_write` was logged as the last line before that crash only because it logs continuously
(~20/sec) regardless of what's actually happening, while this vitals write only fires once per join,
a much narrower and more circumstantially-suspicious window.

**Fix**: deferred the write the same way `pendingTeleport` already is (`state.hpp`'s new
`pendingVitalsRestore` fields) instead of writing inline in the network-receive handler — applied
~2s later from `do_game_tick`, after the pawn has had time to actually finish initializing, and
wrapped in `seh_invoke` as defense in depth regardless. Builds clean, deployed both machines. This
is the most concrete, well-evidenced fix attempt of the entire session — testable directly on the
next join.

**Even deferred, the vitals write still correlates tightly with the cascade — under 1 second.**
`vitals_restore: applied deferred vitals write` at `12:32:18.326`, cascade at `12:32:19.252` — much
tighter than the teleport's own ~3s gap. Points at the write itself (landing on the wrong memory
even 2s post-join, not just being too early) rather than pure timing.

**Direct test deployed**: `kEnableVitalsWrite = false` — write is now skipped entirely, logging kept
(`vitals_restore: SKIPPED (kEnableVitalsWrite=false, testing cascade correlation)`) so the
correlation stays visible either way. If joins stop producing the cascade with the write disabled,
that's strong confirmation of root cause. Also patched PC2's server-saved vitals (`players.db`,
`player_progress` table) back to full — first attempt wrongly used a 0.0-1.0 scale (based on an
unverified protocol.js comment), corrected to 0-100 after noticing a real saved hunger value
(16.7) that only makes sense on a percentage scale, matching the in-game HUD.

## Same session, IDA reconnected — real root cause found via bytecode tracing

**Both leading theories (join-teleport, vitals write-back) were cleanly disproven by direct live
tests** (see above): disabling the vitals write entirely still produced the identical cascade
under a second after the SKIPPED log line. This ruled out everything the mod itself does at join,
pointing conclusively at the base game's own native load sequence — which IDA/bytecode tracing
could actually investigate properly, unlike guessing from timing correlation alone.

**Traced the real native equipment-restore chain, entry to root cause:**

1. `BP_PlayerCharacter_C::Event_LoadPlayer`/`ActorPreLoad`/`ActorLoaded` — all tiny (18-byte) stubs,
   dead ends.
2. `BP_PlayerCharacter_C::OnLoadSavedDataRequested` (50 bytes) — thin wrapper: calls
   `BP_JigHelperComp.OnLoadDataRequested()` via `EX_Context`+`EX_LocalVirtualFunction`, then sets
   its own `Result` bool to true and returns. The real logic lives on a different class entirely.
3. **`BP_JigHelperComp_C::OnLoadDataRequested`** (1454 bytes) — the real entry point. Two loops:
   - **Loop 1** (`0x0154`–`0x02e9`-ish): iterates a slot collection, calls `K2_DestroyActor`
     (resolved via `resolve_fname`, ci=93336) on each entry — destroys existing/placeholder
     equipped item actors.
   - **Loop 2** (`0x037d`–`0x055c`): iterates **`RepActorsData`** (resolved via `resolve_fprop` on
     the instance-variable property at the loop's read site, `0x13d94ce80`) and calls
     `SetEquippedInfoBySlot` (resolved via `resolve_fname`, ci=1846069) per entry with a large
     struct (transform + container arrays — real per-item saved equip data) to restore each item.

**Root cause theory, strongly supported but not yet 100% proven**: `RepActorsData`'s `Rep` prefix
is UE's standard naming convention for a *replicated* property — meaning its real contents arrive
from the server over the network, not instantaneously on possession. If `OnLoadDataRequested` runs
before this replication has actually landed (very plausible right at join — a classic UE
networking race between "possess pawn" and "replicated property delivery"), Loop 2 iterates an
incomplete or still-default/empty array, restoring fewer items than Loop 1 just destroyed — and
nothing in this function retries once the real data does arrive moments later. This explains every
observed symptom simultaneously: always tied to join (this function only runs then), per-slot
inconsistent (whichever entries of `RepActorsData` happened to have arrived by read time), usually
self-heals if something else independently re-triggers a restore shortly after (matching most of
tonight's "recovers within seconds" cases), and sometimes doesn't if nothing re-triggers it
(matching the confirmed-persistent screenshot case from earlier tonight).

**Not yet confirmed**: whether `RepActorsData` genuinely arrives late relative to this function's
execution (would need a live breakpoint/watch on both the replication callback and this function's
entry, comparing timestamps) — that's the next concrete verification step, not attempted yet this
session (IDA was reconnected but a live debugger attach + breakpoint session wasn't started before
this session needed to wrap for the night). If confirmed, the real fix is almost certainly either:
(a) delaying `OnLoadDataRequested`'s call until `RepActorsData` is confirmed non-empty/replicated
(a native engine-side or Blueprint-side timing fix, likely outside what this mod can easily patch
from outside), or (b) finding whatever native retry/OnRep mechanism *should* exist for exactly this
race and confirming why it isn't firing.

Decoded `.bin` files and the resolved names are all in `%APPDATA%\SurrounDeadBridge\` from this
session — `BP_JigHelperComp_C_OnLoadDataRequested.bin` (full decode saved separately as
`decoded_OnLoadDataRequested.txt` in the same directory) is the one to start from next time.

**Continued digging, same session: found the missing link, hit the wall a live debugger would clear.**
`RepActorsData` does have a real `OnRep_RepActorsData()` callback (confirmed: `TArray<FS_RepActorData>
RepActorsData` at `BP_JigHelperComp` `+0xAE0`, matching header dump) — but its bytecode (22 bytes,
decoded) does **not** call `SetEquippedInfoBySlot` or re-trigger `OnLoadDataRequested` directly. It
only does one thing: `EX_CallMulticastDelegate` on an instance variable resolved to
**`OnEquipmentUpdated`**. So whether replication landing late actually triggers a restore depends
entirely on who's subscribed to that delegate and what they do — and that's not visible in any
static header dump (dynamically-bound delegate handlers don't show up as named class functions
the way `OnActiveWeaponSlotChanged_Event_0`-style auto-bound events did elsewhere in this project).
Checked both `BP_JigHelperComp.hpp` and `BP_PlayerCharacter.hpp` for a plausible bound-handler name
— nothing.

**This is the real wall for tonight**: finding out who's actually subscribed to `OnEquipmentUpdated`
(if anyone) needs either (a) a live debugger reading the multicast delegate's actual bound-function
array off a running instance (the property itself, `BP_JigHelperComp+0xC30`, is a real
`FMulticastScriptDelegate` with its own internal invocation list — readable live, not statically),
or (b) a broader bytecode search across every class for `EX_LocalVirtualFunction`/`EX_FinalFunction`
calls binding to this specific delegate name. Neither attempted this session — genuinely needs a
live IDA debug session (breakpoint on `OnRep_RepActorsData`, inspect the delegate's bound list) or
a systematic multi-class bytecode grep, both real next-session tasks, not something to guess at
further tonight.

**Summary of the complete, still-open chain for next session**:
`OnLoadDataRequested` (destroy old items, immediately try to restore from `RepActorsData`) →
if `RepActorsData` hasn't replicated yet, restore is incomplete → `OnRep_RepActorsData` fires later
when it does arrive, but only broadcasts `OnEquipmentUpdated` → **unknown whether anything actually
listens to that broadcast and re-runs the restore**. If nothing does, that's the exact bug: a
one-shot restore attempt with no verified retry path when replication is late. Confirming that
missing link is the single most valuable next step.

## ROOT CAUSE CONFIRMED — live read of the delegate's actual bound-function list

Attached IDA's debugger read-only to PC1's already-running game (no new join, no redeploy, no
2-client testing — safe to do without the user present). Following this project's own established
stability precautions: set `DOPT_*_MSGS` flags only (no `DOPT_LIB_BPT`/`DOPT_THREAD_BPT`), attached,
confirmed process suspended (safe for a static read, no race risk), read memory directly, detached
immediately after — clean, no crash, no hang, no effect on the running game.

Read `OnEquipmentUpdated`'s `FMulticastScriptDelegate` (`BP_JigHelperComp_C` instance `+0xC30`) —
its `InvocationList` TArray: **`data_ptr=0x0 count=0`. Zero bound listeners.** Sanity-checked the
object pointer was still genuinely valid (not stale/reused) by reading `RepActorsData` at `+0xAE0`
in the same call — 11 real entries, a sane count for a fully-equipped character, confirming this is
the correct live object, not garbage.

**Root cause, confirmed as far as tonight's tools allow**: `BP_JigHelperComp_C::OnLoadDataRequested`
destroys existing equipped items and attempts to restore them from `RepActorsData` in one shot, at
join. If that replicated property hasn't fully arrived yet at that exact moment (a real, plausible
network race — nothing here proves the timing directly, but everything else lines up), the restore
is incomplete. `OnRep_RepActorsData` fires later when replication does land, but its only action is
broadcasting `OnEquipmentUpdated` — and **nothing is subscribed to that delegate**, confirmed via a
live read. There is no retry path. This is a genuine base-game logic gap (a delegate that's declared
and broadcast but has no listener wired up for the one case that would need it), not something
introduced by this mod, and not something fixable by changing mod-side timing/deferral — the mod
doesn't own this code path at all.

**What a real fix would look like** (not attempted — this is native game logic, well outside what
reflection-based writes should touch): the mod could itself subscribe a callback to
`OnEquipmentUpdated` (the delegate machinery is already right there, `AddDynamic`-equivalent via
reflection is a known pattern) and, on that callback, re-invoke whatever `OnLoadDataRequested`-
equivalent restore step is needed for any slot that's still empty. This is a real, scoped
implementation task for a future session — bind the delegate, re-run `SetEquippedInfoBySlot` for
any slot present in `RepActorsData` but not currently reflected in the live equipped-actor list.
Needs careful design (avoid double-applying already-successful slots, avoid interfering with normal
gameplay equip/unequip which also presumably fires this same delegate) before attempting live.

**Session over for tonight.** This is a complete, well-evidenced root-cause chain from symptom to
confirmed mechanism — a strong foundation for a future implementation session, reached via safe,
non-destructive tooling throughout (bytecode dumps, name/property resolution, one careful read-only
live debugger attach), with zero live deployment risk taken while unsupervised.

**Fix rebuilt as a surgical, periodic self-healing check instead of a blunt one-shot retry.**
Original design (re-invoke `OnLoadDataRequested` wholesale, once, 5s after join) would have
flickered every slot on every single join, not just broken ones. Rebuilt after live IDA
confirmation of the exact failure signature — walks `RepActorsData` directly every 3s, checks only
`Actor->RootComponent->AttachParent == null` per entry (the real actors are already correctly
replicated; only the local attachment is missing), and calls the same native
`"Equip Actor to Socket"` function this project already uses successfully for proxies, with the
existing actor reference straight from `RepActorsData` — no spawning, no DataAsset lookup needed.
Silent when healthy (only logs when it actually finds and fixes something), cheap enough to run
continuously rather than gate to a one-shot join window — this makes it self-heal any future
occurrence of the same failure shape, not only the join-time replication race. Builds clean, not
yet deployed for live testing.

## Same session, continued — traced and fixed the SEPARATE base-body-mesh mechanism

The equipped-item fix (`RepActorsData`/`AttachParent`) doesn't cover base body components at all —
confirmed live when a fresh report (floating shotgun/headwear, bald-looking character) showed the
same Torso/Legs/Feet cascade happening despite the equip-restore fix being deployed and working.
This is a genuinely separate mechanism, traced the same way:

- **`BP_PlayerCharacter_C::OnRep_ClothingLegsEquipped?`** (759 bytes, real logic) — turned out to
  govern the `Clothing_Legs` overlay component, not the bare `Legs` body mesh `component_drift`
  actually tracks. A real, working replication callback (branches on a bool, calls
  `SetSkinnedAssetAndUpdate` with the correct Male/Female mesh) — not the broken link, just the
  wrong component.
- **`BP_PlayerCharacter_C::UpdateBodyParts(FName Name)`** (1500 bytes) — the real one. Dispatches by
  name (resolved via `resolve_fname`: ci=1732710→`"Torso"`, ci=1732718→`"Legs"`,
  ci=1732721→`"Feet"`) and calls `SetSkinnedAssetAndUpdate` (ci=100173, same call
  `OnRep_ClothingLegsEquipped` uses) on the matching component — the genuine native mesh-(re)apply
  function for base body parts.

**Fix**: extended `component_drift`'s existing mesh-asset-clear detection (already tracking
Torso/Legs/Feet's `SkeletalMesh`/`SkeletalMeshAsset` pointer) to actively repair, not just log —
when a tracked component reads null, call `UpdateBodyParts` via reflection with the matching FName
(passed as a raw `{ComparisonIndex, Number}` struct, matching this project's own established
pattern for FName/GameplayTag params — no FName-from-string constructor exists in the vendored
SDK). Comparison-index values hardcoded from this session's live `resolve_fname` reads — flagged as
stable (compile-time string literals baked into the shipped build's Kismet bytecode, not
runtime-registered like the already-documented-unstable GameplayTag CIs). Self-limiting: a
successful repair makes the mesh read non-null on the very next check, naturally stopping further
calls; silent when healthy. Applies to both local pawn and any tracked proxy, since
`check_component_drift` already runs for both.

Only 3 of the 8 tracked component names (`Torso`/`Legs`/`Feet`) are covered by `UpdateBodyParts` per
the decoded bytecode — `Hands`/`Arms`/`head`/`HairMesh`/`BeardMesh`/`EyebrowsMesh`/`Mouth` remain
log-only for now; worth checking whether `UpdateBodyParts` has more branches further in the
function (only fully traced the first ~90 of 1500 bytes) or whether those use a different
mechanism entirely, in a future session. Builds clean, deploying for live test now.

**Two live-tested refinements to the body-part repair fix, same session:**

1. **Retry cap (5 attempts)**: `UpdateBodyParts` fired every single 2s check indefinitely on a
   proxy without ever succeeding — likely client-authority gating this native function has that
   `EquipActorToSocket` doesn't (that one's already proven working cross-network for proxies
   elsewhere in this project). Capped retries per component so a structurally-unfixable case (proxy)
   stops spamming `ProcessEvent` forever instead of retrying eternally; local player repairs
   (confirmed working) are unaffected since they succeed well within 5 attempts.

2. **Skin-showing-through-pants fix**: `UpdateBodyParts` alone reapplies only the *bare* body mesh —
   live-reported: after a successful repair, skin visibly showed through gaps in worn pants, since
   `UpdateBodyParts` has no awareness a clothing overlay should be covering that part. Traced
   `BodyPartVisibility` (2800 bytes) hoping it was the fix — resolved its repeated call
   (`func=0x79861900`) to **`SkeletalMeshComponent::SetSkeletalMeshAsset`**, meaning despite the
   name it's not a visibility toggle at all, it's a bulk per-part mesh-asset setter driven by a
   caller-supplied `FBodyPartSettings` struct (real Male/Female mesh references per part) — too
   complex/risky to construct blind this session. Used the already-fully-decoded, parameterless
   `OnRep_ClothingTorsoEquipped?`/`OnRep_ClothingLegsEquipped?`/`OnRep_ClothingFeetEquipped?`
   instead (confirmed real properties on `BP_PlayerCharacter_C`) — called immediately after
   `UpdateBodyParts` on repair, reusing the exact real per-slot logic that already knows whether
   that slot is actually clothed and re-covers the bare mesh correctly if so.

Both changes build clean, deploying now for live test.

**New, worse failure variant found live: `RepActorsData` can be stuck permanently empty, not just
late.** PC2's log this session shows `equip_restore_retry: RepActorsData empty or implausible
count=0` on every single 3s check, indefinitely — never once transitioning to a real
`checked=N fixed=N` line the way PC1's session did (which recovered once RepActorsData populated).
Glasses/helmet on PC2 stayed unrepaired for this reason — not a bug in the fix itself (it correctly
declines to act on empty/implausible data rather than risk operating on garbage), but the
underlying replicated property genuinely never arrived at all this session, a more severe case than
the "arrives a few seconds late" scenario the fix was designed around. Also separately observed:
the local player's own body-part retry cap (5 attempts) triggered and gave up on Torso/Legs/Feet in
this same session — meaning `UpdateBodyParts` isn't guaranteed to succeed even locally in every
case, contrary to this session's earlier assumption that it always works locally and only fails on
proxies.

**Open question for a future session**: why would `RepActorsData` fail to replicate at all for an
entire session, rather than just arriving late? Worth checking whether this correlates with
anything specific about that join (network conditions, connection order, a missed initial-bunch
delivery) — this is a genuinely different, more severe failure mode than anything characterized so
far tonight, and neither of tonight's two fixes can address a permanently-absent data source, only
a delayed one.

**Pivoted from static bytecode archaeology to a live hook — user's suggestion, much better fit.**
Static-decoding `LoadPlayerInventory`/`ActorPreLoad`/`ActorLoaded` hit a real methodological trap:
resolving their embedded func/property pointers came back as garbage, because those `.bin` dumps
were captured in an earlier process instance — a relaunch invalidates every raw pointer embedded in
a dump (ASLR/heap layout differs per process), a caveat this project already knew but tonight
tripped over live. Rather than keep re-dumping and re-resolving reactively, added a direct,
always-on hook instead: `check_load_data_requested_hook()` in `on_process_event_pre`, same cached-
UFunction-pointer fast-path pattern as `on_process_event_post`'s `s_lastUpdateFn`. Resolved once off
any live `BP_JigHelperComp_C` instance (UFunction* is shared per-class, so this covers every
instance's calls — local and proxy alike, confirmed live with real replicated data tonight,
[[feedback_sdo_ufunction_shared_per_class]]). Logs `RepActorsData`'s real count immediately before
every single `OnLoadDataRequested` call, giving direct empirical proof — is it ever called more than
once per session, and is it genuinely empty at call time — instead of inferring from bytecode.
Builds clean, deployed both machines, running now. Next join's log is the real test.

**Live hook result: `OnLoadDataRequested` proven NOT the cause of the body-part cascade.** Zero
`load_data_requested` log lines during an actual, real, live occurrence of the Torso/Legs/Feet
clear (confirmed: `component_drift`'s repair loop fired and gave up right next to it, with nothing
from the hook anywhere nearby). This correction matters: `OnLoadDataRequested`'s whole chain
(destroy+restore from `RepActorsData`, no retry on late replication) only explains the **equipped-
item** symptom (weapons/clothing-as-actors going missing) — it has nothing to do with the base
body-mesh clearing, which is a genuinely separate, still-unidentified mechanism. The cascade did
still correlate with a join (`on_unreal_init` fired shortly before), just via some other function.

Added a second live hook watching **`SetSexMesh`** (real `BP_PlayerCharacter_C` function, name
alone strongly suggests it sets the base body mesh set by gender — exactly Torso/Legs/Feet/Arms).
Same cached-UFunction-pointer pattern. Deployed, next join/occurrence is the test.

**Real diagnostic bug found and fixed: HairMesh/BeardMesh/EyebrowsMesh/Mouth were never actually
checkable at all.** `SetSexMesh` hook also caught zero calls during a live occurrence — second
exonerated candidate. While investigating why `HairMesh`'s own "cleared" state never once logged
despite repeated hair-loss reports all session, found the real bug: `do_component_drift_scan`'s
mesh-asset lookup only ever tried `"SkeletalMesh"`/`"SkeletalMeshAsset"` — correct for
Torso/Legs/Feet/Arms/Hands/head, but `HairMesh`/`BeardMesh`/`EyebrowsMesh`/`Mouth` are all
`UStaticMeshComponent` (confirmed, `BP_PlayerCharacter.hpp`), needing the property name
`"StaticMesh"` instead. Every hair/beard/eyebrows/mouth report tonight went completely undetected
by this diagnostic — not because those components were fine, but because the property-name lookup
silently never matched, so the whole detection block never ran for them. Added `"StaticMesh"` as a
third candidate. Builds clean, deployed both machines. This should finally make hair loss visible
in the log — real progress on scoping the investigation correctly, independent of whatever the
actual root trigger turns out to be.

**Final occurrence logged, end of session.** Screenshot showing both characters simultaneously
fragmented (floating helmet, disconnected legs/hands with no torso on one; floating head with no
torso connection on the other) — same ongoing issue, no new mechanism identified beyond what's
already documented above. `EyebrowsMesh`'s "cleared" transition still not caught despite the
StaticMesh property fix, most likely the same first-observation blind spot every tracker in this
session has (no baseline to compare against if the component was already null the first time it's
checked after a respawn/reset). Session ending here — see the "Next session should" priorities
throughout this log (RigVM/state-machine work was never reached, still needed for whatever residual
mechanism isn't explained by RepActorsData/UpdateBodyParts) and the summary at the top of this
session's entries for the full picture.

**New session, resumed: recent-calls ring buffer built after two named-function guesses both came
back clean.** Rather than guess a third candidate function name blind, added a 65536-entry ring
buffer recording every single `ProcessEvent` call's `(func, obj, timestamp)` unconditionally (cheap
— array write + index increment, no string work on the hot path). When `component_drift` detects a
mesh-asset-clear transition, it dumps the buffer (consecutive-repeat runs collapsed, since per-frame
anim/tick calls would otherwise drown out genuinely interesting one-off calls) — giving a real trace
of what actually happened right before the clear, instead of another named-candidate guess.

Also tightened `component_drift`'s (and `attach_health`'s/`equip_restore_retry`'s, since they share
the same polling timer) interval from 2s to 300ms, specifically so the gap between the real clear
event and this check noticing it stays small enough for the ring buffer to still contain the actual
causal calls rather than several seconds of unrelated activity that happened after. Builds clean,
deployed both machines, running now with the tighter interval — next real occurrence should finally
produce a genuine trace instead of another blind guess.

## Root cause found: the fall-off cascade is a join-time race, not a random runtime bug

After deploying the recent-calls ring buffer + tightened 300ms polling, PC1's debug.log captured
7 full occurrences of the Torso/Arms/Legs/Feet body-part cascade over ~1 hour of play. Every
single one shows the identical sequence, within ~1.5s:

```
join_teleport: about to call K2_SetActorLocation/Rotation, preChildrenCount=4
join_teleport: K2_SetActorLocation/Rotation done, postChildrenCount=4
equip_restore_retry: RepActorsData empty or implausible count=0
  ... (~1.5s later)
attach_health: local item ptr=... MESH ASSET CLEARED (was set, now null, still attached)   x3-4
component_drift: local:Torso MESH ASSET CLEARED (was set, now null)
component_drift: local:Arms MESH ASSET CLEARED (was set, now null)
component_drift: local:Legs MESH ASSET CLEARED (was set, now null)
component_drift: local:Feet MESH ASSET CLEARED (was set, now null)
```

**This is not a mid-session drift bug.** It only fires around join/rejoin. `join_teleport` (the
existing join-position restore logic) runs before `RepActorsData` has replicated back in — at that
moment the array reads `count=0`. Our own drift monitors, sampling shortly after, see the
still-loading state (meshes not yet (re)applied post-join) as "was set, now null" and fire repair.
Existing repair (`UpdateBodyParts` + clothing `OnRep_`, `equip_restore_retry`'s orphaned-slot
re-attach) genuinely fixes most of it within a few seconds — confirmed by
`equip_restore_retry: checked=11 fixed=1` and no further clears until the next join event in every
one of the 7 cases. But repair is capped at 5 attempts per component and only covers what
`component_drift`/`equip_restore_retry` explicitly track — weapon attachments held in hand
(the axe) and anything not in the tracked component-name table can still lose out permanently if
the real data doesn't finish repopulating before the cap or the retry loop gives up.

This reframes the "two exonerated theories" (`OnLoadDataRequested`, `SetSexMesh`) finding from
before: they were exonerated because they're genuinely not the trigger — the trigger isn't a
function call at all, it's a **load-order gap at join**: `join_teleport` fires before the
save-data-driven equip/appearance restore has actually landed.

Also confirmed: reports of PC1 items missing "on PC2's screen" (proxy-observed) can't be diagnosed
from PC1's own debug.log at all — `attach_health`/`component_drift` only ever watch the locally-
owned pawn, never proxies. A `PC1's axe fell off` report with no matching local clear in PC1's log
around that time is exactly this case. Proxy-side monitoring doesn't exist yet.

### Next fix direction
Don't chase more mystery trigger functions. Instead: gate/delay `join_teleport` (or the whole
join sequence) until `RepActorsData` has a plausible non-zero count, or explicitly re-run the full
equip+appearance restore once it does land, instead of relying on the retry-capped drift/attach
monitors to paper over a load-order gap after the fact.

## Root cause session, continued: repair lands but gets fought; ring-buffer dump on the repair path was itself a bug

After the join-load-order gate (equipDataReady) and retry-pacing fix (2s grace + 1/s throttle)
shipped, a live occurrence on both PC1 and PC2 showed the retry loop calling `UpdateBodyParts`
correctly-paced (once/second) but still never converging — "giving up after 20 failed repair
attempts" on both machines' LOCAL pawn (not just proxies, overturning this project's prior
assumption that UpdateBodyParts reliably works on the local player).

Added a before/after mesh-pointer read directly in `do_body_part_repair`. Result, live-confirmed
on PC1: **the repair call always lands** — `meshImmediatelyAfter` is a valid non-null pointer
(the same hardcoded default-mesh constant each time, e.g. `0x4433081472` for Torso, consistent
with `decoded_UpdateBodyParts.txt` showing UpdateBodyParts sets a compile-time `EX_ObjectConst`
default mesh per Torso/Legs/Feet/gender, not the player's actual saved appearance — it's a "reset
to bare default skin" function, not a real restore). But `meshBefore` on the NEXT repair attempt,
~1 second later, reads null again every time. Something else is re-clearing the mesh in that
~1-second window, undoing our fix as fast as we apply it. That something is still unidentified.

Attempted to catch it by calling `dump_recent_calls()` (the 65536-entry ring buffer) from inside
`do_body_part_repair`, once per repair cycle. **This was itself a real bug, live-confirmed and
reverted the same session**: writing all 65536 lines via `debug_log` (one call per line) took
~7 real seconds of wall-clock time on the same thread, confirmed by counting log lines between
the dump's start and the next unrelated log line (55,809 lines, all dump output). This likely
stalled the game itself every time repair fired, and directly correlates with the repair call
starting to read back `meshImmediatelyAfter=0x0` (a genuine failure, unlike every prior clean
read) immediately after this was added — plausible that the multi-second stall let the hardcoded
default mesh asset (kept alive only by being newly referenced) get unloaded/GC'd before the next
repair cycle, or that the stall itself desynced something. Removed immediately.

Since each individual repair call is proven cheap and harmless (no crash, lands correctly), and
capping at a fixed attempt count only guarantees the character stays permanently broken once
exhausted, removed the attempt cap entirely for the "local" repair path (proxies were never
covered by this path in the first place — see check_attach_health_trigger — so no proxy-forever-
retry risk). Retry now continues indefinitely at 1/s. This doesn't fix the underlying fight, but
it means the character self-corrects within ~1s of any clear, indefinitely, instead of eventually
staying broken once the old cap (5, then 20) was exhausted.

### Still open
What re-clears Torso/Legs/Feet's mesh ~1s after every successful repair. Next attempt at
answering this should NOT dump the full 65536-entry ring buffer synchronously via debug_log from
a hot path — either sample only the most recent ~50-100 entries, or build the string in memory and
flush once, or move the dump off the calling thread entirely.

## 2026-08-14 — batch decode of undumped equip/appearance/inventory-load .bin files

Static-decoded and reviewed 13 previously-undumped `.bin` captures from
`C:\Users\mccau\AppData\Roaming\SurrounDeadBridge\`, prioritizing appearance/equip/clothing/
inventory-load candidates per the open investigation. Each now has a matching
`decoded_<FunctionName>.txt` next to its `.bin`. Cross-referenced every FName `ComparisonIndex`
found against the known body-part CIs (Torso=1732710, Legs=1732718, Feet=1732721) and searched
each decode for calls to `SetSkinnedAssetAndUpdate` (ci=100173, the function `UpdateBodyParts`
and `OnRep_ClothingLegsEquipped` both use to actually (re)apply a body-part mesh).

**One genuinely new, relevant finding: `BP_PlayerCharacter_C::EquipClothingToMesh`**
(`decoded_EquipClothingToMesh.txt`, 551 bytes) directly calls `SetSkinnedAssetAndUpdate`
(ci=100173) **twice** — once down a `True`-branch and once down a matching `False`-branch that are
otherwise structurally identical (almost certainly the Male/Female mesh split, same shape as
`OnRep_ClothingLegsEquipped`'s confirmed Male/Female branch). Each branch first calls a local
virtual function (ci=1937166) passing two `StructMemberContext` reads off what's structurally a
`FBodyPartSettings`-shaped local struct (matches the `Svr_AttachClothing`/`MC_AttachClothing`
signature's `FBodyPartSettings Parts` parameter from `BP_PlayerCharacter.hpp`), then applies the
result via `SetSkinnedAssetAndUpdate` on a component reference, then does interface-context calls
(materials/overlay refresh, ci=93673). **This is very likely the actual mesh-application body of
`Svr_AttachClothing`/`MC_AttachClothing`, or a shared helper both of those RPCs call into** — it is
the only one of these 13 functions that touches `SetSkinnedAssetAndUpdate` at all, and its
parameter shape lines up with `FBodyPartSettings`. No direct call-site/caller info was available
statically to confirm which RPC calls it, but this narrows the "still-undumped
`Svr_AttachClothing`/`MC_AttachClothing`" lead considerably — a future live capture of those two
should be compared against this decode to check whether `EquipClothingToMesh` is literally what
they call.

**Everything else decoded is NOT related to body-part meshes** — none of the following reference
ci=100173 (`SetSkinnedAssetAndUpdate`) or any Torso/Legs/Feet-range FName CI:
- `OnRep_FacewearEquipped` / `OnRep_PrimaryWeaponEquipped` (609/913 bytes) — real, working
  replication callbacks, but for held/worn *item actors* (interface-based attach/detach + material
  refresh calls), not the base body skeletal meshes. Their embedded FName CIs (1730464, 1730659)
  are item-slot names, not body-part names — outside the known Torso/Legs/Feet range.
- `OnPickupEquipped` (`BP_JigHelperComp_C`, 1121 bytes) — attaches/detaches a picked-up item actor
  via interface calls (`ObjToInterfaceCast` + `LocalVirtualFunction`), no skeletal-mesh-component
  writes at all.
- `OnRep_ActiveWeapon` (`BP_JigHelperComp_C`, 112 bytes) — small, just broadcasts a multicast
  delegate (`OnEquipmentUpdated`-style) if the actor changed; no mesh writes.
- `HandleActorEquipped` / `ServerFuncHandleEquipActor` (`BP_JigMultiplayer_C`, 636/805 bytes) —
  large actor-equip bookkeeping/RPC-relay functions; grepped for both `ci=100173` and any
  `173271x`-range FName and found neither. Not decoded line-by-line in full given the negative
  grep result and time budget — flagging as "probably not it" rather than "definitely not it" for
  a future session that wants full certainty.
- `LoadPlayerInventory` (26 lines), `OnLoadSavedDataRequested` (16 lines), `HandleItemOverItem`
  (11 lines) — all trivially short: each is just 1-2 calls into a larger virtual/interface function
  (name unresolvable further without live pointers) plus a bool return. No inline body-mesh logic;
  if a load-order gap exists here it's inside whatever they call, not in these stubs themselves.
- `ActorLoaded`, `ActorPreLoad`, `Event_LoadPlayer` (18 bytes each) — all three are just a single
  `EX_LocalFinalFunction` call to the *same* `func=0x11f0ec720` with a different int-literal
  argument (202664 / 202657 / 199722 respectively) — almost certainly a Blueprint instrumentation/
  trace-event stub (K2Node ID or similar), not meaningful game logic. Confirms these three save/
  load lifecycle events are otherwise empty Blueprint graphs; if anything happens on
  load/actor-preload it's native-side, not in these BP event graphs.

**Net assessment**: nothing decoded tonight overturns or extends the existing root-cause chain
(join-time `RepActorsData`/appearance-restore race, `UpdateBodyParts` applying only a hardcoded
default mesh, and the still-unexplained ~1s re-clear after every successful repair). The one solid
new lead is `EquipClothingToMesh` as a plausible match for what `Svr_AttachClothing`/
`MC_AttachClothing` actually call to touch the mesh — worth comparing directly once those two are
captured live.

**Still explicitly open**: `Svr_AttachClothing` and `MC_AttachClothing` themselves remain
undumped — no `.bin` exists for either. A live capture attempt this session stalled (the
flag-file mechanism the diagnostic system uses to request a dump stopped being consumed by the
running game, for an unknown reason — not something fixable without the live process attached).
These two RPCs are still the most likely holders of the real "apply the player's actual saved
appearance" logic (as opposed to `UpdateBodyParts`'s bare-default fallback), and are the
single highest-priority next capture target — ideally diffed against `EquipClothingToMesh` above
once captured.

## Svr_AttachClothing decoded: it's a native stub, not Blueprint logic

Captured live (2026-08-15): `BP_PlayerCharacter_C::Svr_AttachClothing` is 126 bytes, decoded to
`decoded_Svr_AttachClothing.txt`. Its entire body is: store all 6 parameters (`Clothing`, `Mesh`,
`Parts`, `IsPlayerMale`, `BodyPart`, `UpdateAllBodyParts`) into persistent-frame slots via
`EX_LetValueOnPersistentFrame`, then a single `EX_LocalFinalFunction` call to another function
(`func=0x124f13dc0`, an in-process-only pointer, not yet resolved — `resolve_ptr.flag` attempts
this session didn't get picked up, same intermittent flag-consumption issue noted below) with one
`EX_IntConst 195814` argument.

This is a real finding: **the actual clothing/body-mesh-attach logic is native C++, not Blueprint
bytecode.** `Svr_AttachClothing`'s Blueprint graph is just a parameter-marshaling stub for a native
thunk. This confirms last session's theory from the filtered `ProcessEvent` ring-buffer trace
(nothing Blueprint-dispatched touches the mesh between a repair and the next re-clear) — the real
mechanism genuinely cannot be seen by any `ProcessEvent` hook, Blueprint-side instrumentation has
hit its ceiling here. Going further requires native disassembly (IDA) of whatever
`func=0x124f13dc0` resolves to, in a fresh capture (the pointer is only valid within the process
instance that produced it).

### Static catalog correction: the mesh properties themselves are NOT replicated

`research/bp_catalog_player_core.md` (built from the FModel export, no live game needed) confirms
`BP_PlayerCharacter_C`'s actual mesh-reference properties (`Clothing_Torso`/`Legs`/`Feet`/`Gloves`/
`Armor`, `HairMesh`, `BeardMesh`, `EyebrowsMesh`, `SkinColor`) carry **no** `Net` flag at all — only
`InstancedReference | NonTransactional`/`DisableEditOnInstance`. Only the ~14 paired
`*Equipped?` booleans (e.g. `ClothingTorsoEquipped?`) are `Net | RepNotify`.

This **corrects** the "property replication silently reasserting a broken saved value" theory from
earlier — there's no continuously-replicated mesh property to reassert. Appearance sync is driven
entirely by the explicit `Svr_AttachClothing`/`MC_AttachClothing` RPCs firing at the moment of
equip, plus the `*Equipped?` bools' `OnRep_` callbacks. Revised leading theory: something is
re-triggering an `OnRep_*Equipped?` callback (or the underlying bool is flapping) with the wrong
internal state each time, OR the RPC pair itself is being invoked repeatedly with bad/empty
`FBodyPartSettings` data. Either of those WOULD be Blueprint-dispatched (OnRep callbacks are real
UFunctions) — worth re-checking the filtered `ProcessEvent` ring-buffer trace specifically for
`OnRep_ClothingTorsoEquipped?`/`OnRep_ClothingLegsEquipped?`/`OnRep_ClothingFeetEquipped?` next,
since the earlier trace wasn't filtered to include those by name explicitly, only the pawn +
Torso/Legs/Feet component objects as receivers — an OnRep call's receiver IS the pawn, so it
should have been caught already, meaning either it didn't fire, or it fired and the existing
decoded logic (`decoded_OnRep_ClothingLegsEquipped.txt`) doesn't behave as understood. Re-read that
decode with fresh eyes next session.

### Recurring issue: bytecode_dump.flag / resolve_ptr.flag intermittently not consumed

Noticed again this session (also seen last session): sometimes a flag file sits unconsumed for
10+ seconds despite the game actively ticking (other diagnostics logging normally in the same
window). Not yet root-caused. Not a per-mechanism bug (both `bytecode_dump.flag` and
`resolve_ptr.flag` showed it) — possibly something about `GetFileAttributesW` caching, antivirus
real-time-protection transiently locking newly-created files, or the Bash tool's file write not
being immediately visible to the native process. Worth a real look next time it blocks something
important — for now, the workaround is just to retry the write.

## Static BP catalogs built (2026-08-15) — three parallel agents, FModel export, no live game needed

Wrote `research/bp_catalog_player_core.md`, `research/bp_catalog_ai_vehicles.md`,
`research/bp_catalog_inventory_jigsaw.md` — function/property reference tables for every
gameplay-relevant Blueprint class, built from the static `Exports/SurrounDead` FModel export
(structure only, no bytecode — that still needs live capture per-function).

### New top lead for the appearance/equip investigation: RepPrimitiveActorsData

`bp_catalog_inventory_jigsaw.md` found `BP_JigHelperComp_C` holds **two parallel replicated
arrays**, not one: the already-known `RepActorsData` AND a second one, `RepPrimitiveActorsData`,
each with its own `OnRep_` handler — plus explicit manual reconciliation functions
`ForceRepPrimitiveActorSpawns` and `UpdatePrevFromPrim`. A two-array design that needs an explicit
"force re-replicate" escape hatch is a strong structural match for intermittent detach: if the two
arrays fall out of sync, slot/actor-reference data (`RepActorsData`, what `equip_restore_retry`
already reads) can be entirely correct while whatever `RepPrimitiveActorsData` actually drives
(likely the spawned mesh/primitive component itself) silently isn't. This could be the real
explanation for repairs that land then revert with no visible `ProcessEvent` call in between —
if `RepPrimitiveActorsData`'s own OnRep is what's re-asserting a stale/empty primitive state, that
callback firing is a normal Blueprint call our filtered ring-buffer trace should be able to catch,
just under a name we weren't watching for yet.

Also found: `BP_JigMultiplayer_C::WaitFullReplicationOfUIDs` (an explicit replication-wait helper)
and `BP_JigPickupComponent_C::CheckMismatch` (an existing consistency-check function) — both
suggest the original developers already anticipated and partially handled this exact failure
class, which is worth reading before building any more custom repair logic on our side.

**Next live-capture priority, in order** (per the catalog's own recommendation): `BP_JigHelperComp_C
::ForceRepPrimitiveActorSpawns`, `::UpdatePrevFromPrim`, both `OnRep_` handlers (RepActorsData's
already have one decoded; RepPrimitiveActorsData's does not), `BP_JigMultiplayer_C::
ServerFuncHandleEquipActor`/`HandleActorEquipped`/`WaitFullReplicationOfUIDs`,
`BP_JigPickupComponent_C::CheckMismatch`. This supersedes the `Svr_AttachClothing`/
`MC_AttachClothing`-first plan from earlier tonight — those turned out to be native stubs (dead
end for Blueprint-level tracing); `RepPrimitiveActorsData`'s OnRep is a real Blueprint function and
a more promising target.

### Other confirmed findings from the catalogs (not directly the bug, but load-bearing reference)

- AI/vehicle health & damage is component-driven (`DamageComponent_C`, `VehicleFuelComponent_C`,
  `VehicleHealthComponent_C`), not ad-hoc per-class RPCs — `BP_VehicleMaster_C` has real unreliable
  server RPCs `Svr_UpdateEngine`/`Svr_UpdateFuel` as the actual authoritative driving-input path.
  Useful reference for the still-pending vehicle health/fuel sync work.
- `BP_MasterZombie`/`BP_ZombieBoss` both expose `OnRep_IsDead?` — death state is replicated-bool-
  driven, confirms the existing zombie-simulation design's assumption.
- Leaf item/attachment Blueprints (sampled ~30) carry no own logic — everything funnels through the
  handful of base classes already cataloged; full per-item enumeration (~1,170 files) isn't worth
  doing.

## Static BP catalogs, second batch (2026-08-15) — BuildingSystem, SmartAI, misc systems

Three more parallel agents covering the rest of the gameplay-relevant Blueprint surface:
`research/bp_catalog_building.md`, `bp_catalog_smartai.md`, `bp_catalog_misc_systems.md`.

- **`Content/SmartAI/`** turned out to be the base AI plugin framework (likely a purchased "Smart
  AI System" asset) that `Content/AI/`'s actual zombie/bandit/trader archetypes build on — not a
  competing or overlapping system. `BP_SmartAIComponent_C` (~180 properties, 100+ functions) is the
  generic combat/flee/melee/interact "brain" every archetype layers on top of. Also contains a
  leftover marketplace tutorial character (`BP_ExampleCharacter`) that isn't shipped gameplay.
- **`Content/Blueprints/BuildingSystem/`**: nearly everything funnels through one base,
  `Buildable_MASTER_C` (66 functions); most of the ~53 leaf pieces (walls/foundations/etc.) are
  empty 0-function subclasses. **`Health` is a plain `SaveGame`-only `DoubleProperty` — NOT
  replicated** (same unreplicated-mesh-property pattern already found on the player character).
  Only 3 real replicated properties exist in the whole 191-file scope (`PoweredOn?`, generator
  `CurrentFuel`/`TurnedOn?`, decon-shower `CooldownRunning?`) — no repair/upgrade/ownership/snap
  networking exists at the Blueprint layer at all. `Buildable_MASTER_C::Svr_Spawn` is the closest
  existing analog to the mod's own `Svr_SpawnBuild` hook, worth diffing signatures against before
  extending `PlacedStructure`.
- **Misc systems** (`HordeSystem`/`Infestation`/`Quests`/`POI`/`Prefabs`/`Laboratory`/`Other`, 403
  files): `HordeSystem` is nearly empty (2 real classes). `Prefabs` confirmed pure static dressing.
  `Laboratory`/`Other` hold real reusable RPC patterns: a consistent `Svr_*`→`MC_*` door-open pair
  across 7 door classes (`Other/Doors/`), and `Svr_PlaySound`/`MC_PlaySound`/`Svr_FallOverEffect`/
  `MC_FallOverEffect` hit-feedback on harvestable Tree/Rocks/ScrapMetal — both clean, reusable
  Server-RPC-in/Multicast-out reference patterns for any future mod feature needing the same shape.
  `OnRep_Locked`/`OnRep_Unlocked`/`OnRep_Warning`/`OnRep_LightOn?`/`OnRep_On?`/`OnRep_Off?` confirm
  replicated-bool-driven state on Laboratory doors/switches, consistent with the zombie
  `OnRep_IsDead?` pattern found in the first batch.

**Cross-cutting pattern now confirmed across THREE independent domains** (player clothing meshes,
building `Health`, and — by the export's own limitation — every other domain): visual/state
properties are very often plain `SaveGame`-only, NOT replicated, with actual sync handled entirely
by explicit RPC pairs and `OnRep_` bools. This is the base game's standard architecture, not
something mod-specific — worth keeping in mind for ANY future entity-sync work, not just the body-
mesh bug: never assume a visual property is replicated without checking, always look for the
RPC/RepNotify pair driving it instead.

This completes static cataloging of every gameplay-relevant Blueprint folder in the FModel export.
Remaining work on the appearance-bug investigation is live-capture (see the `RepPrimitiveActorsData`
lead from the first batch) — static analysis of the export is exhausted for that specific question.

## MAJOR FIND: FBodyPartSettings located, and a concrete repro lead (2026-08-15)

`research/bp_catalog_playermodel.md` (from cataloging `Content/PlayerModel/`, 489 files, never
examined before) found the actual `FBodyPartSettings` struct: `Content/PlayerModel/Other/
BodyPartSettings.json`, a Blueprint `UserDefinedStruct` (not native C++) with 16 optional
`SkeletalMesh` reference fields, one per sex × body segment (Torso/Arms/Biceps/Hands/Legs/
LowerThighs/LowerLegs/Feet). It's wrapped by `ClothingSettings` (top-level `MaleMesh`/`FemaleMesh`
garment + `UpdateAllBodyParts?` bool + nested `BodyPartSettings`), the row type for `DT_Clothing`
(80 rows, one per clothing item in the game).

**Concrete repro lead**: a full sweep of all 80 `DT_Clothing` rows found only 1 (`Robe`) sets
`UpdateAllBodyParts?=true`, but **20 of 80 rows** (shorts, plaid/short-sleeve/Hawaiian shirts,
fingerless gloves, swim trunks, slippers, robe, oil-rig jacket) populate the nested per-segment
`BodyPartSettings` override fields directly while leaving that bool **false** — a materially more
complex code path than the other 60 single-mesh items, whose exact gating semantics aren't fully
clear from static data alone. **Next live test session: specifically try to repro the fall-off bug
while wearing one of these 20 items** (vs. one of the 60 simple ones) — if the bug correlates with
this item list, that's the smoking gun tying it directly to this data path rather than a generic
timing race.

Confirms (third independent domain now) the "visual state is plain data, not replicated" pattern:
`DT_Clothing`/`ClothingSettings`/`BodyPartSettings` carry zero replication metadata themselves —
they're static asset data. The actual replication gap is entirely on `BP_PlayerCharacter_C`'s side
(unreplicated mesh properties, only `*Equipped?` bools are `Net|RepNotify`), with appearance state
traveling as one-shot RPC payloads (`Svr_AttachClothing`/`MC_AttachClothing`) resolved against this
table at the moment of equip.

Also cataloged: `BP_CharacterCreator_C` (client-only, non-networked preview mannequin used during
character creation) and `Enum_Occupation` (13-value cosmetic enum, not gameplay-relevant). The
remaining ~484 `PlayerModel` files are confirmed pure art assets.

## Full Content/ tree sweep complete (2026-08-15)

Last batch: `research/bp_catalog_playermodel.md` (see MAJOR FIND above),
`bp_catalog_anim_input_wip.md`, `bp_catalog_sky_terrain_levels.md`.

- **Animations**: 8 real logic classes — 4 Player AnimNotifyStates (`MeleeHitDetect` x2, `Shove`,
  `Stomp`) and 3 Zombie/ZombieBoss attack AnimNotifyStates, all local-only (no `FUNC_Net*`, no
  replicated properties). **Directly relevant to this project's still-pending Phase 4 melee
  hit-detection work** (see [[sdo-remaining-work-backlog]]) — these are the real notify hooks to
  read/reuse rather than reinventing hit-window timing.
- **Input**: pure Enhanced Input data, zero `Function` entries anywhere. **WIP**: confirmed entirely
  dead/unused art content (wandering-trader NPC, ghillie set, MP5/USP meshes, zombie-boss skin
  materials) — zero Blueprint classes, nothing live.
- **UltraDynamicSky**: unmodified marketplace plugin. Weather already has real server RPCs
  (`Change Weather` etc.) — no sync work needed if the mod ever touches weather. **Time-of-day has
  NO net-flagged function or replicated property anywhere** — `Ultra_Dynamic_Sky_C`'s `Get
  TimeCode`/`Set Time with Time Code` pair is the clean hook point if a future session wants
  server-authoritative day/night sync, but it doesn't exist yet.
- **Terrain**: no real logic beyond two trivial water-collision Blueprints. **Levels** (6 maps):
  each map's Level Blueprint is nearly empty (~2 functions, no RPCs) — most of each Level JSON is
  actually a full per-map actor dump, not gameplay logic. **Sequences**: pure cinematic data.

Manually spot-checked the last three folders NOT worth a dedicated agent (only 1 function-bearing
file each, confirmed via grep): `Meshes/Vehicles/TestChargerVehicle` (leftover test asset, just
AnimGraph/Ubergraph stubs), `Audio/BP_AmbientSoundController` (weather-reactive ambient audio,
zero gameplay/network relevance), `EditorOnly/DEBUG_Marker` (trivial debug placement marker). None
warrant cataloging.

**This completes static cataloging of the entire `Content/` export.** Remaining top-level folders
(`PolygonFiles`, `ButtonIcons`, `UI`, bulk of `Meshes`/`Audio`) confirmed to have zero or
near-zero `Function` entries (checked via grep) and were excluded per the original scope decision
(pure art/UI, not gameplay logic) — see the reference-table files (`research/bp_catalog_*.md`, 9
files total) for the full per-class breakdown. Nine catalog files now cover every gameplay-relevant
Blueprint class in the game: player_core, ai_vehicles, inventory_jigsaw, building, smartai,
misc_systems, playermodel, anim_input_wip, sky_terrain_levels.

**Standing highest-priority next actions for the appearance-bug investigation** (unchanged targets,
now with a concrete repro path added):
1. Live-test the fall-off bug specifically while wearing one of the 20 flagged `DT_Clothing` items
   (shorts/plaid-shirt/short-sleeve-shirt/Hawaiian-shirt/fingerless-gloves/swim-trunks/slippers/
   robe/oil-rig-jacket) vs. a simple single-mesh item — see the MAJOR FIND entry above.
2. Live-capture `BP_JigHelperComp_C::ForceRepPrimitiveActorSpawns`, `::UpdatePrevFromPrim`, and
   `RepPrimitiveActorsData`'s `OnRep_` handler.

## CRITICAL FIND: RepPrimitiveActorsData's real struct shape, from native header dump (2026-08-15)

`research/CXXHeaderDump/BP_JigHelperComp.hpp` (a real native member-offset dump, not the FModel
structure-only export) gives exact offsets and confirms the function name guessed at earlier:

```
TArray<FS_RepNonActorData> RepPrimitiveActorsData;   // 0x0AD0 (size: 0x10)
TArray<FS_RepActorData>    RepActorsData;             // 0x0AE0 (size: 0x10)
...
void OnRep_RepPrimitiveActorsData();
void UpdatePrevFromPrim();
void ForceRepPrimitiveActorSpawns();
```

And critically, `research/CXXHeaderDump/S_RepNonActorData.hpp` (element type) vs.
`S_RepActorData.hpp` (the already-known, already-used-by-`equip_restore_retry` element type):

```
FS_RepActorData     { FGameplayTag Slot; AActor* Actor; }                       // 0x10, 2 fields
FS_RepNonActorData  { FGameplayTag Slot; UJigsawItem_DataAsset_C* DA; AActor* Primitive; }  // 0x18, 3 fields
```

`RepPrimitiveActorsData`'s entries carry a **third field, `Primitive` (AActor*), that `RepActorsData`
doesn't have at all.** This is very likely the missing link: `RepActorsData`/`equip_restore_retry`
tracks whether the equipped ITEM actor (weapon, backpack, etc.) is correctly attached — which
`equip_restore_retry`'s logs show as consistently fine (`checked=11 fixed=1`, stable slot count).
`RepPrimitiveActorsData`'s separate `Primitive` actor per slot is a plausible candidate for what
actually drives the VISIBLE body/clothing mesh, independent of whether the logical item is
equipped — exactly matching the observed symptom (equip state correct, visible mesh wrong).

**This is now the single highest-priority next live-capture target**, and — unlike last night —
we now have the exact function name (`OnRep_RepPrimitiveActorsData`, not a guess) and the exact
offset (`0x0AD0`) needed to read it directly via raw pointer arithmetic (same pattern
`equip_restore_retry` already uses on `RepActorsData` at `0x0AE0`), without even needing a fresh
bytecode dump first. A future session could:
1. Add a raw diagnostic read of `RepPrimitiveActorsData` (offset `0x0AD0`, `FS_RepNonActorData`
   entries: Slot@0x00, DA@0x08, Primitive@0x10, 0x18 bytes/entry) alongside the existing
   `RepActorsData` read in `equip_restore_retry`, logging both side-by-side during a live cascade —
   if `Primitive` reads null/stale while the matching `RepActorsData` entry's `Actor` is fine, that
   confirms this exact struct is the root cause.
2. Live-hook `OnRep_RepPrimitiveActorsData` (same cheap `GetFunctionByNameInChain`/pointer-equality
   pattern as the existing `OnLoadDataRequested`/`SetSexMesh` hooks) to see exactly when it fires
   relative to a visible clear.
3. Bytecode-dump `UpdatePrevFromPrim`/`ForceRepPrimitiveActorSpawns`/`OnRep_RepPrimitiveActorsData`
   itself for the actual repair logic, now that exact names are known.

## Catalog enrichment pass: real native offsets added (2026-08-15, continued overnight)

Two agents cross-referenced `research/bp_catalog_*.md` against `research/CXXHeaderDump/` (2407 real
native member-offset dumps) and added real byte offsets in place. Two directly actionable results:

**Vehicle health/fuel sync — offsets now known, unblocks the pending feature** (see session59
memory: "adopt-only for now... next piece to verify live"):
- `VehicleFuelComponent_C::CurrentFuel` @ `0x00C8` (double), `MaxFuel` @ `0x00D0`.
- `VehicleHealthComponent_C::CurrentHealth` @ `0x00C0` (double), `MaxHealth` @ `0x00C8`.
- `BP_VehicleMaster_C`: `FuelComponent` pointer @ `0x03D0`, `VehicleHealthComponent` pointer @
  `0x0390`. Same read/write pattern already proven elsewhere in this project (dereference actor →
  follow component pointer → raw double read/write at offset) — no new technique needed, just
  these specific numbers.
- `Buildable_MASTER_C::Health` confirmed @ `0x0380` (double) — matches the earlier "unreplicated"
  finding, now with a real offset if a future local-write approach is wanted.
- `DamageComponent_C::CurrentHealth`/`MaxHealth` @ `0x00B8`/`0x00C0` (used by most AI classes).

**Further confirms the RepPrimitiveActorsData theory**: `FS_RepNonActorData` (element type) is
0x18 bytes vs `FS_RepActorData`'s 0x10 — the extra 8 bytes is `AActor* Primitive` @ offset `0x10`,
a field `RepActorsData`'s struct simply doesn't have. This is real, offset-confirmed evidence (not
inference) that `RepPrimitiveActorsData` owns a live spawned-actor pointer per slot that
`RepActorsData` has no equivalent for.

**Also found and ruled out as a red herring**: `FRepItemInfo` (the `ServerEquippedItems` element
type, `BP_PlayerCharacter_C`) is pure inventory bookkeeping (ItemID/Count/Weight/Price/Durability/
Stats) — no actor/primitive/mesh reference. Rich structure, useful reference, but not relevant to
the visual-mesh-clearing bug specifically.

`bp_catalog_inventory_jigsaw.md`, `bp_catalog_player_core.md`, `bp_catalog_ai_vehicles.md`,
`bp_catalog_building.md` now carry real offsets for their highest-value classes (full list of what
was/wasn't covered is in each file). `bp_catalog_smartai.md`, `bp_catalog_misc_systems.md`,
`bp_catalog_playermodel.md`, `bp_catalog_anim_input_wip.md`, `bp_catalog_sky_terrain_levels.md`
still only have structure (no offsets) — lower priority, not yet done.

## Offset-enrichment pass complete — all 9 catalogs now have real offsets where headers exist

Final agent covered the remaining five catalogs. Notable results: `BP_SmartAIComponent_C` (the
~180-property AI "brain") got offsets for its ~40 highest-value fields (Health, Dead, AttackTarget,
combat/ammo/ragdoll/follow/climb state). `BP_LaboratorySlidingDoor_C`'s `Locked`/`Unlocked`/
`Warning` bools are now offset-confirmed as the clearest verified-replicated-door example in the
whole catalog set. `FBodyPartSettings`/`FClothingSettings` (the appearance structs from the earlier
MAJOR FIND) now have every field's real offset. `BP_BuildableTurretAIComponent` confirmed
byte-identical in layout to `BP_TurretAIComponent` — safe to template one off the other.

`bp_catalog_misc_systems.md`/`bp_catalog_sky_terrain_levels.md` had little to annotate (those
catalogs describe functions more than named properties) despite large matching headers existing
(`Ultra_Dynamic_Sky.hpp` 762 fields, `Ultra_Dynamic_Weather.hpp` 548 fields) — flagged as available
for a future pass if that plugin's internals are ever needed in depth. `bp_catalog_anim_input_wip.md`
confirmed to have nothing offset-worthy (AnimNotifyStates declare no member properties).

## Session summary: full night's static RE work, 2026-08-15

For a future session picking this up cold: tonight (after the live debugging session) produced
**9 Blueprint catalog files covering the entire gameplay-relevant Blueprint surface of the game**,
now enriched with real native memory offsets wherever a matching header dump exists. This is a
durable reference — check these files before doing fresh IDA/bytecode work on ANY class. Combined
with the live-debugging findings earlier in the session (join-time race, retry-pacing fix,
`Svr_AttachClothing` proven to be a native stub, `RepPrimitiveActorsData`'s `Primitive` field
found and offset-confirmed as the leading root-cause theory), the appearance-bug investigation now
has concrete, numbered next actions (see the MAJOR FIND and CRITICAL FIND entries above) rather
than open-ended guessing. Nothing live was touched during the overnight cataloging portion — all
of it is static file analysis, safe to have run unsupervised.

## Performance bug found and fixed: uncapped retry + per-cycle ring-buffer dump = live freezing (2026-08-15)

After removing the repair attempt cap (yesterday) and confirming a character can get stuck fighting
indefinitely (repair lands, reverts every ~1.2s, for MULTIPLE MINUTES straight in a live test
today), **PC1 started freezing**. Root cause: `do_body_part_repair` still called
`dump_recent_calls()` on every single repair attempt (a leftover from earlier live-tracing work,
believed "safe" after being rewritten to a single string-flush instead of one debug_log call per
line). With repair now uncapped and firing every ~1-2s indefinitely during a stuck fight, this meant
walking the full 65536-entry ring buffer and formatting up to 1000 function names via `GetFullName()`
+ `WideCharToMultiByte`, continuously, for as long as the fight lasted — a real per-tick cost, not
a one-time diagnostic. Removed the call from this hot path entirely (`do_body_part_repair` no
longer dumps at all). `dump_recent_calls()` itself is unchanged and still safe for ad-hoc/rare use.

**Lesson for future diagnostic additions**: anything added to a retry/repair loop needs to be
re-evaluated for cost every time that loop's cap or frequency changes — a diagnostic that was cheap
"once per stuck detection" became expensive once the loop became "forever, every second."

## Live findings from today's join-race test, before the freeze was traced

- Confirmed via the new initial-post-join-state logging: `local:Torso/Arms/Legs/Feet` were ALL
  already MISSING at the very first sample after join (not a later transition) — hard confirmation
  of the join-time race theory. `Hands`/`HairMesh`/`BeardMesh`/`head`/`EyebrowsMesh`/`Mouth` were
  all fine from the start the same join.
- Repair kicked in after the 2s grace and DID set valid meshes (Torso/Legs/Feet — Arms has no
  repair path, `bodyPartCi=0`), but reverted and re-fought every ~1.2s continuously for several
  minutes straight in this occurrence — the uuncapped retry worked as designed (never permanently
  stuck) but never "won" either.
- **`RepPrimitiveActorsData` cross-check came back clean every single pass (`no mismatches`)
  throughout this entire multi-minute fight** — a real negative result. The leading theory from last
  night (Primitive actor pointer going stale while RepActorsData looks fine) did NOT hold up for
  this occurrence. Either the mismatch window is narrower than our 3s sampling can catch, or
  `RepPrimitiveActorsData` isn't the actual mechanism after all — don't keep leaning on this theory
  without more evidence.
- Also caught, same join: two attached items on PC2's PROXY (as rendered on PC1's screen) missing
  from frame one (`proxy0>2056d20e0` children at `0x5750689904`/`0x4937343008`), while proxy0's own
  Torso/Legs/Feet/etc. were all fine. Confirms the join-race affects proxy-rendered equipment too,
  a code path we've never directly repaired (proxy `UpdateBodyParts` calls are already known to
  fail — see earlier "on a PROXY this call fired every check indefinitely without succeeding").

## Decision: next step is a live IDA session, not another guess-and-patch

User's explicit call after the RepPrimitiveActorsData negative result: stop guessing at Blueprint-
level theories and attach IDA live to watch the actual native clear happen in real time. Not yet
started as of this log entry — see next entry for the session itself once it happens.

## Live IDA attach attempt today: caused a real hang, aborted (2026-08-15)

Attempted the live IDA session per user's explicit direction (after the RepPrimitiveActorsData
negative result). `ida_dbg.attach_process`/`suspend_process`/`wait_for_next_event(WFNE_SUSP)`
worked as expected (process genuinely suspended, confirmed via `get_process_state()`), but the
pawn pointer derived from a `recent_calls` log line (`obj=0x<N>`, which is DECIMAL digits despite
the misleading `0x` prefix — a known quirk of this project's `debug_log("...0x" +
std::to_string(ptr))` pattern) was read incorrectly as hex, producing a wrong address and garbage
memory reads (`ida_bytes.get_qword` at the wrong address, high-entropy non-pointer-looking values).

Detached cleanly (`detach_process()` returned `True`, `is_debugger_on()` eventually went `False`
after polling) — but **the game process was left permanently hung** (`Get-Process ...
Responding=False`, CPU time still climbing but zero new log lines, no recovery after two detach
attempts and ~15s of waiting). Had to force-kill the process. This matches a documented prior-
session risk exactly (see `[[sdo-ida-debug-stability]]` memory: "IDA's own MCP plugin then hung
trying to inspect it — had to force-kill both ida.exe and the game process").

**Root cause of the hang itself not diagnosed** — could be the brief suspend triggering an anti-
cheat/protection watchdog, a deadlock from resuming mid-critical-section, or something else
entirely. Not confirmed whether the WRONG pawn address read (garbage memory access) contributed,
or whether ANY suspend at this game's current protection/build would hang regardless of what's
read. **Do not attempt another live IDA attach on this game without addressing the pointer-format
gotcha first** (project's own decimal-labeled-as-hex logging convention) and treating the hang risk
as a near-certainty, not an edge case — this is now 2-for-2 documented hangs from IDA attach on
this specific game.

**No progress made on the actual root-cause question** (what natively re-clears the mesh) — this
attempt ended in cleanup, not data. Next attempt, if tried again, needs: (1) get the pawn pointer
from a reliable source with correct hex/decimal handling (e.g. add a temporary raw hex-formatted
log line to mod.cpp rather than reusing the existing decimal one), (2) treat the hang as expected
and have a plan to recover before starting, not react to it after, (3) consider whether the risk
is acceptable at all given it's now 2-for-2 — the user may prefer to stay on Blueprint/native-static
analysis and accept the mitigated-but-unsolved flicker rather than risk further hangs.

## Live IDA attach, attempt 2: got real data this time, but hung again (3-for-3) — likely anti-debug

Second attempt, this time with a correctly-verified pawn pointer (added a proper %llx-formatted
`pawn_ptr_hex:` log line to mod.cpp specifically to avoid the decimal/hex mixup from attempt 1).
Sanity-checked the pointer (vtable read resolved to a plausible module address) before trusting it.

**Got real, decisive live data before the hang**: while the local pawn was actively mid-cascade
(repair landing, reverting every ~1.2s, confirmed via log timestamps matching the read window),
read `RepActorsData` (11 entries) and `RepPrimitiveActorsData` (12 entries) directly via
`ida_bytes.get_qword` while the process was suspended. **Every single `RepPrimitiveActorsData`
entry had a live, non-null `Primitive` pointer — fully healthy.** Then read
`Torso.SkeletalMeshAsset` directly (`USkeletalMeshComponent+0x8F0`, confirmed real offset from
`research/CXXHeaderDump/Engine.hpp`) and got `0x0` — genuinely null, at the exact same instant
`RepPrimitiveActorsData` looked completely fine.

**This is a real, live-confirmed result, not a 3-second-sampling inference**: `RepPrimitiveActorsData`
is NOT the mechanism. The mesh is null while every piece of replicated slot/primitive-actor data
looks correct. Whatever's clearing `Torso.SkeletalMeshAsset` is doing so without leaving any trace
in either replicated array — points toward a genuinely independent native code path (streaming
state, an async load that never resolves, or an unrelated system entirely), not a replication-sync
issue at all. **Drop the RepPrimitiveActorsData theory. Do not chase it further without new
evidence.**

**The process hung again after a clean `detach_process()` — same as attempt 1.** This is now
**3-for-3** documented hangs from attaching IDA to this specific running game (this session's two
attempts + the prior session's crash-cycle hang noted in `[[sdo-ida-debug-stability]]`). Given the
consistency (happens whether or not a breakpoint is ever set, happens on a clean read-only
suspend+read+detach with no memory writes), the leading explanation is **this game has some form
of anti-debug/anti-cheat protection that reacts to a debugger attaching**, not a bug in our own
attach sequence. If true, live IDA debugging of the running game process is not a reliably usable
technique for this project going forward — every attempt costs a full game relaunch and risks
losing whatever live state was being investigated.

**Recommendation for future sessions**: treat live IDA attach as high-cost/unreliable. Prefer
static analysis (bytecode dumps via `bytecode_dump.flag`, the FModel export, `CXXHeaderDump`) and
the mod's own live logging (which doesn't trigger this problem — it's not a debugger) over live
IDA attach. If live IDA is attempted again, expect a hang and plan the relaunch cost in up front;
don't treat a hang as a surprise requiring re-diagnosis each time.

### Where this leaves the appearance-bug investigation

Both `RepActorsData` (item-equip tracking) and `RepPrimitiveActorsData` (this session's leading
theory) are now confirmed clean during live cascades. The actual mechanism remains unidentified.
Given the confirmed-native, ProcessEvent-invisible nature of the real cause (from `Svr_AttachClothing`
being a stub) and now this negative replication-data result, the most likely remaining explanation
is a native streaming/async-load issue specific to how this character's mesh assets get
(re)requested after a fresh join — not a networking/replication bug at all. This would explain why
gating on `RepActorsData`'s readiness (this project's `equipDataReady` fix) didn't fully solve it:
that gate watches the wrong data. A future session investigating this should look for whatever
governs asset streaming/loading state on the character or its mesh components, not equip-related
replicated arrays.

## PC1 freeze traced to the uncapped repair loop, cap reintroduced (2026-08-15)

User reported PC1 "hanging whenever the pawn stops moving," reproduced via a deliberate test — this
happened WITHOUT any IDA attach involved (separate from the 3-for-3 IDA hangs above). Checked
`debug.log`: the last line before the freeze was a `component_drift` repair attempt at
`09:44:29.925`; the freeze landed ~1.2s later, almost exactly matching the repair loop's 1/s
cadence — `debug.log` itself stopped growing entirely (a real engine-thread freeze, not the
render-only hangs seen elsewhere in this project's history where the log kept advancing).

Strong correlation, not proven root cause: the uncapped retry (introduced earlier the same day)
means once a character gets stuck, up to 3 components × 2 `ProcessEvent` calls each
(`UpdateBodyParts` + clothing `OnRep_`) fire once/second **forever** — never exercised for more
than a few minutes continuous before today. Reverted to a bounded cap (60 attempts, ~60s — well
past every genuine convergence time seen live, but not infinite) rather than keep running unbounded
on an unconfirmed theory. Deployed to both machines.

**Open question for next session**: was the freeze actually caused by resource accumulation from
the repeated reflection calls, or something else entirely (worth testing: does the freeze recur
with the 60-cap in place — if it still happens even within a bounded ~60s window, the repair loop
isn't the real cause and something else needs investigating, e.g. `aim_write`'s own continuous
per-tick writes, which run unconditionally regardless of repair state).

## New, distinct bug found and fixed: clothing overlay can fail independent of base mesh (2026-08-15)

Live report: PC2 spawned in with skin visible through her clothes. Checked the new initial-post-
join-state logging — `local:Torso/Legs/Feet` were ALL `SET` from the very first sample (no base-
mesh clear at all this occurrence). This is a genuinely different failure than everything
investigated so far tonight: the CLOTHING OVERLAY components (`Clothing_Torso`/`Clothing_Legs`/
`Clothing_Feet`, `BP_PlayerCharacter.hpp` @ `0x0770`/`0x0768`/`0x0760`) failed on their own, and
`component_drift`'s `kNames` table never tracked them directly — the clothing `OnRep_` callbacks
only ever got called as a side effect of a base-mesh repair, so a clothing-only failure with a
healthy base mesh had **no detection and no repair path at all** until now.

Fixed: added `Clothing_Torso`/`Clothing_Legs`/`Clothing_Feet` as their own tracked entries
(`ci=0`, since `UpdateBodyParts` only dispatches on `"Torso"`/`"Legs"`/`"Feet"` by name and doesn't
know these component names at all — confirmed via its decoded bytecode). Relaxed
`check_component_drift`'s repair-trigger gate (previously required `bodyPartCi != 0`) to also allow
`ci==0` entries that have a `clothingOnRepName` set, and `do_body_part_repair` now skips the
`UpdateBodyParts` call entirely for these (straight to the clothing `OnRep_`, since there'd be
nothing to call it with). Same 2s grace / 1/s throttle / 60-attempt cap applies.

Not yet live-verified — deployed to both machines, next occurrence of this specific symptom should
now self-heal within a few seconds instead of having no repair path at all.

## Mod-disable gotcha: mods.txt alone doesn't disable a C++ DLL mod (2026-08-15)

User set `SurrounDeadBridge : 0` in `mods.txt` to fully disable the mod on PC1 (after repeated
freezes, wanting a clean baseline), but the mod kept loading anyway. Root cause: UE4SS has TWO
independent mod-loading mechanisms, confirmed via `UE4SS.log`:
```
Starting mods (from mods.txt load order)...
Mod 'ActorDumperMod' disabled in mods.txt.
...
Starting mods (from enabled.txt, no defined load order)...
```
`mods.txt`'s `Name : 0/1` only governs the first list. C++ DLL mods (including ours) load via the
SECOND mechanism — the mere presence of an `enabled.txt` marker file inside the mod's own folder
(`Mods/SurrounDeadBridge/enabled.txt`), completely independent of what `mods.txt` says.
`scripts/deploy.ps1` always creates this file (`New-Item -ItemType File -Force -Path $enabledTxt`)
and nothing ever removed it — so `mods.txt`'s toggle was silently a no-op for this mod the whole
project.

**To actually disable**: delete `Mods/SurrounDeadBridge/enabled.txt`, not (only) edit `mods.txt`.
Takes effect on next launch (UE4SS only reads mod-enable state at startup). Deleted on PC1 this
session. If `deploy.ps1` or any future redeploy script runs again on PC1 while the mod is meant to
stay off, it will silently re-create `enabled.txt` and re-enable it — worth fixing the deploy
script to not stomp an intentional disable, or at least remembering this gotcha next time
something needs to be temporarily off.

## LIKELY ROOT CAUSE OF THE FREEZES FOUND: wrong bReinitPose parameter (2026-08-15)

User's push to dig into "how equipping actually works, maybe we're calling the wrong method" paid
off immediately. Isolation test first confirmed the local `component_drift` repair loop was NOT
the cause (froze even with all repair `ProcessEvent` calls disabled). Log at the moment of that
freeze showed the last line mid-way through `proxy_manager.cpp::sync_pawn_appearance`, stuck at
the `ProcessEvent` call to `SetSkinnedAssetAndUpdate` for `bodyPart[0]=SK_Chr_Female_Torso` — two
earlier runs of the exact same sequence (same function, same call shape) had completed cleanly
seconds apart, so this wasn't a deterministic per-call failure, just an intermittent one.

Already had `decoded_EquipClothingToMesh.txt` (`BP_PlayerCharacter_C::EquipClothingToMesh`,
captured/decoded in an earlier session but never closely read) — this is the REAL game's own
native equip logic. Both its male and female branches call `SetSkinnedAssetAndUpdate` (ci=100173)
with **`param[1] = EX_False`** (`bReinitPose=false`). Checked our own code:
`proxy_manager.cpp`'s `equip_clothing_to_mesh` (line ~1381) and `sync_pawn_appearance` (line
~2338) both had **`bReinitPose = true`** — the wrong value, at both call sites, matching neither
the real game's own proven-safe usage.

`bReinitPose=true` triggers a full skeleton pose reinitialization (rebuilds the bone tree) — a much
heavier native operation than a normal mesh swap. This is a textbook UE threading hazard: mutating
a `SkeletalMeshComponent`'s mesh with a full pose reinit while that same component's own
`AnimInstance` is concurrently ticking (a proxy's `AnimBP` runs completely independent of our mod's
own tick) can contend on render/anim-thread state — plausible root cause for an intermittent,
timing-dependent full engine-thread freeze exactly matching everything observed today (froze
regardless of which call site fired it — local repair path, proxy appearance sync — because ALL of
them share this same wrong parameter; "freezes whenever something falls off" was really "freezes
whenever ANY of these code paths happens to fire at a bad moment," which naturally correlates with
fall-off symptoms since that's when repair/sync logic runs most).

**Fixed both call sites to `bReinitPose = false`, matching the real game's own call shape exactly.**
Re-enabled the local repair loop (temporarily kill-switched during the isolation test) since it was
proven not to be the direct cause either way. Deployed to both machines — not yet live-verified.

If this doesn't stop the freezing, the next thing to check is whether `SetSkinnedAssetAndUpdate`
itself is simply unsafe to call from our tick context at all regardless of parameters (i.e. it
needs to run through the proper `Svr_AttachClothing`/`EquipClothingToMesh` RPC chain, not a direct
call) — but this specific parameter mismatch is a strong, concrete, previously-unnoticed bug worth
ruling out first before escalating to a bigger architectural change.

## New gap found and partially fixed: equipped-item POSITIONAL drift (not just detach/mesh-clear)

Live report: PC1's helmet visibly floating away from his head, but `read_local_weapon_attachments`
showed it correctly attached (`slot=1 itemId=MilitaryTacticalHelmet arrayCount=2`, unchanged) the
whole time, and no `DETACHED`/`MESH ASSET CLEARED` fired. Root cause: none of tonight's (or any
prior session's) diagnostics ever checked an equipped ITEM ACTOR's relative-transform drift — only
the character's own named body components (`component_drift`'s `DRIFTED` check) had that logic.
An item can be perfectly attached (valid `AttachParent`) and have a valid mesh, and still visually
sit in the wrong place.

Extended `do_equip_restore_retry` (which already walks every equipped actor via `RepActorsData`
every 3s) to also track each actor's `RootComponent` `RelativeLocation` (offset `0x128`, the same
proven offset `component_drift` uses) tick-to-tick, and re-call `Equip Actor to Socket` (the same
repair already used for the orphaned-`AttachParent` case) if it jumps more than 30 units between
checks. **Untested whether this native call actually corrects position, not just attachment** —
it's the only native re-snap entry point this project has found; logged clearly
(`re-snapped DRIFTED slot entry[N]`) so a future check can confirm live whether it worked.

**Important scope limit, confirmed same session via a PC2 screenshot of PC1's proxy (full-body
fragmentation — head/beanie/pants/shirt/hands all separated)**: `equip_restore_retry` (and this new
drift check) only ever runs for the LOCAL pawn (`EquipRestoreRetryCtx ctx{ pawn }` in
`check_equip_restore_retry_trigger`, local-only) — **proxies are not covered at all**. What one
player sees on another's screen goes through `proxy_manager.cpp` entirely, a separate code path
this fix does nothing for. This remains a real, unaddressed gap — the single biggest fragmentation
symptom (full proxy body scattering) is still unmonitored and unrepaired.

## Attachment-system audit: found and fixed a real ScaleRule mismatch (2026-08-15)

Per user's request to audit whether the mod is using the right native calls for the whole
attachment system (not just the bReinitPose bug already found). First: decoded `Equip Actor to
Socket` itself for the first time (`BP_JigHelperComp_C`, the exact function `equip_restore_retry`
calls everywhere) — confirmed our usage (`ActorRef` + `IsSecondary`, nothing else) matches its own
signature exactly, and its internal native attach uses `K2_AttachToComponent` with
`LocationRule=SnapToTarget(2), RotationRule=SnapToTarget(2), ScaleRule=KeepWorld(1),
WeldSimulatedBodies=true` — this is the reference shape.

Audited all three of this project's own direct `K2_AttachToComponent` call sites in
`proxy_manager.cpp` against that reference:
- `spawn_and_equip_item_visual`'s backpack re-attach (line ~1141): `(2,2,1,true)` — matches.
- `reattach_weapon_visual_to_socket` (line ~1963): `(2,2,1,true)` — matches.
- `spawn_and_attach_weapon_attachment` (line ~1300, attaches scopes/mags/suppressors/etc. onto a
  weapon): **`ScaleRule` was `2` (SnapToTarget), not `1` (KeepWorld) — a real mismatch, the only
  one of the three.** Fixed to match.

Also confirmed both `SetSkinnedAssetAndUpdate` call sites (already fixed for `bReinitPose` earlier
today) are the only two in the codebase — nothing else calls it. Deployed to both machines, not
yet live-verified. This specific call site (`spawn_and_attach_weapon_attachment`) is exercised
every time ANY player's weapon attachments get rendered on another client's screen — plausible
contributor to attachment-specific visual glitches (wrong scale on a scope/suppressor/etc.), though
not yet tied to a specific live-reported symptom the way bReinitPose was.

## Systematic bytecode audit (mod.cpp done): found and fixed a 3rd real bug — missing ReturnValue field

Per user's request, spawned agents to systematically audit EVERY `ProcessEvent` call site against
real native signatures (not just attach/equip calls). `mod.cpp` audit (20 call sites) found:
**`K2_SetActorRotation`'s params struct (`join_teleport`, ~line 3717) was missing the trailing
`bool ReturnValue` field** that `bool K2_SetActorRotation(FRotator, bool)` requires
(`Engine.hpp:8550`) — its sibling call three lines above (`K2_SetActorLocation`) has this field
correctly, making the omission clearly accidental, not a deliberate choice. Missing this field
means the Kismet-packed return slot doesn't exist in the struct, causing a 1-byte stack overwrite
adjacent to the params struct on **every single join-time teleport of the local player's own
pawn** — this exact call was already flagged in its own code comment as "HIGHER RISK than any
other change tonight" and "not yet live-verified" from when it was first written. Fixed by adding
the missing field, matching the sibling call's shape. Deployed to both machines.

15 other call sites in `mod.cpp` verified correct against `CXXHeaderDump`/decoded bytecode
references; 3 unverifiable (a runtime-target debug command, a raw bytecode-dump utility, and
incoming-hook parameter readers — different category, not outgoing calls, flagged for a possible
follow-up pass). `proxy_manager.cpp`'s audit still running as of this entry.

## proxy_manager.cpp audit complete: 2nd instance of the same ReturnValue bug found and fixed

`do_apply_proxy_actor_rotation`'s `ActorRotationParams` had the identical missing-`ReturnValue`
bug as `mod.cpp`'s `K2_SetActorRotation` call (same function, different call site) — this one
happened not to corrupt anything live (`FRotator`'s own struct padding left enough room), but
relying on that is fragile, not a real fix. Also fixed a misleading field name (`bSweep` → the
real param is `bTeleportPhysics`, no sweep param exists on this function). Fixed and deployed.

32 call sites audited total in this file; only these + the two already-fixed bugs (bReinitPose,
ScaleRule) found anything wrong — 30 verified correct. One unverifiable: `GetSkeletalMeshComponent`
isn't a real UFUNCTION name on this build (not found anywhere in the 2407-file header dump), but
it's always used with a verified `K2_GetRootComponent` fallback already, so it's a harmless
always-no-op lookup, not a functional risk.

**Systematic ProcessEvent audit now complete for both `mod.cpp` and `proxy_manager.cpp`** — 3 real
bugs found total today (bReinitPose ×2 call sites, ScaleRule ×1, missing ReturnValue ×2 call
sites), all fixed. See `research/bytecode_audit_modcpp.md`/`bytecode_audit_proxymanager.md` for
full per-call-site detail.

## Full content mapping complete: gameplay + non-gameplay, everything accounted for (2026-08-15)

Final two catalogs close out full coverage of the entire `Exports/SurrounDead/Content/` tree
(gameplay-relevant catalogs from earlier today + these two):

- **`bp_catalog_ui.md`**: 392 UI files surveyed, 98 with real logic (294 are pure widget-tree
  assets). Confirmed via full-text scan: **zero UI widgets contain any RPC or Net/Replicated
  property** — UI is entirely a passive client-side display layer, consistent with the
  actor/component-driven replication pattern already found everywhere else in the game.
  Gameplay-adjacent notes worth remembering: `UpdateHealth`/`UpdateFuel` push functions on
  health/fuel bar widgets mirror the vehicle component pattern already documented;
  `LockpickUI_C`'s actual consume logic lives server-side in the Jigsaw inventory component, not
  the widget itself; `DebugMenu.json` is a real developer/cheat panel (damage player, reduce
  hunger/thirst, reset skills, spawn items, change weather) worth knowing exists for testing.
- **`bp_catalog_nongameplay_assets.md`**: confirmed Audio/Meshes/ButtonIcons/PolygonFiles/
  EditorOnly (3,743 files total) are genuinely asset-only — only 3 files have any Blueprint logic
  at all (an ambient-sound weather state machine, one AnimGraph stub, one debug marker), none
  networked.

**This completes full-content bytecode/structure mapping** — every Blueprint-bearing folder in the
game, gameplay-relevant or not, now has a corresponding `research/bp_catalog_*.md` reference
(11 files total: the original 9 + ui + nongameplay_assets). Combined with
`research/bytecode_decode_status.md` (which class/function actually has real decoded bytecode vs.
just structural cataloging) and the two `bytecode_audit_*.md` parameter-correctness audits, this is
now a complete, current picture of the codebase's relationship to the real game's native/Blueprint
surface.

## Live-captured and decoded the 6 remaining priority functions (2026-08-15)

Game was up and cooperative — captured and decoded all 6 functions flagged as priorities across
tonight's sessions: `MC_AttachClothing`, `WaitFullReplicationOfUIDs`, `CheckMismatch`,
`OnRep_RepPrimitiveActorsData`, `ForceRepPrimitiveActorSpawns`, `UpdatePrevFromPrim`.

- **`MC_AttachClothing`**: confirmed to be a native stub, identical shape to the already-decoded
  `Svr_AttachClothing` — marshals 6 params, calls one native function. Both halves of the RPC pair
  are now confirmed native/opaque to Blueprint-level tracing, as expected.
- **`OnRep_RepPrimitiveActorsData`**: trivially short (17 bytes) — just calls one other function
  (`ci=1859803`, name not yet resolved) with no parameters. A thin dispatcher, not itself
  interesting.
- **`ForceRepPrimitiveActorSpawns`**: sets a ONE-SHOT timer (`SetTimerByFunctionName`-shaped call:
  `Self`, `"UpdatePrevFromPrim"`, `0.05s`, `bLooping=False`) then broadcasts a multicast delegate.
  So calling this function doesn't reconcile anything itself — it just schedules
  `UpdatePrevFromPrim` to run 0.05s later.
- **`UpdatePrevFromPrim`** (1572 bytes, real logic): **a two-array diff/reconciliation loop.**
  Structurally, it walks a "Prev" primitive-actor snapshot array against what's presumably the
  current `RepPrimitiveActorsData`, and for each index where they differ, calls an interface
  function (`ci=1860625`) with either the full entry (Slot/Actor/DataAsset fields) to add/update a
  visible primitive, or a "cleared" call shape (`NoObject`, empty `FName`, `False`) for entries
  present in the old snapshot but missing from the new one — an add/update/remove reconciliation
  against a previous-state cache, not a direct read-and-apply.

**New, concrete leading theory this creates**: the actual application of `RepPrimitiveActorsData`
changes to visible primitives does NOT happen automatically when the property replicates — it only
happens when `UpdatePrevFromPrim` actually RUNS, which only happens if something calls
`ForceRepPrimitiveActorSpawns` (or otherwise sets that timer) to schedule it. If `OnRep_
RepPrimitiveActorsData`'s target function (`ci=1859803`, still unresolved) does NOT itself trigger
this reconciliation path, then a pure network replication update landing outside the normal
in-session equip/unequip flow (i.e. exactly the join-time catch-up case this whole investigation
has been chasing) could update the underlying data with NO corresponding call to reconcile it into
a visible primitive — matching every symptom seen tonight: `RepPrimitiveActorsData` reads
healthy/correct, yet the visible mesh never updates.

**Next concrete step**: resolve `ci=1859803` (`OnRep_RepPrimitiveActorsData`'s single call target)
by name — if it's NOT `ForceRepPrimitiveActorSpawns` or `UpdatePrevFromPrim` itself, that's very
strong evidence this is the actual gap: replication landing without ever triggering reconciliation.
If it turns out this resolves to exactly that call, the fix becomes concrete: have the mod itself
call `ForceRepPrimitiveActorSpawns` (or `UpdatePrevFromPrim` directly) after confirming
`RepActorsData`/`RepPrimitiveActorsData` are populated post-join, the same pattern
`equip_restore_retry` already uses for the orphaned-`AttachParent` case.

## Resolved: the reconciliation chain IS correctly wired (theory above disproven)

Resolved the two remaining unknown names via `resolve_fname.flag`: `ci=1859803` =
`ForceRepPrimitiveActorSpawns`, `ci=1860625` = `Preview Set Equipped Primitive By Slot`.

**This means `OnRep_RepPrimitiveActorsData` directly calls `ForceRepPrimitiveActorSpawns`** — the
full chain is: property replicates → `OnRep_RepPrimitiveActorsData` fires automatically (standard
UE RepNotify behavior) → calls `ForceRepPrimitiveActorSpawns` → schedules `UpdatePrevFromPrim` via
a 0.05s one-shot timer → `UpdatePrevFromPrim` diffs old-vs-new and calls `Preview Set Equipped
Primitive By Slot` per changed entry to actually apply/clear the visible primitive. **This
disproves the "replication lands without triggering reconciliation" theory from the previous log
entry** — the wiring is self-contained and automatic, not something that needs an external trigger.

Given this, if the visible mesh still doesn't update despite this whole chain being correctly
wired, the remaining candidates narrow to: (1) `OnRep_RepPrimitiveActorsData` simply not firing at
all in the failure case (a RepNotify only fires on an actual detected value change from the
client's perspective — if the array's initial replicated state at actor-spawn time already
"matches" some default, a later logical change might not register as a wire-level delta the client
notices), or (2) `Preview Set Equipped Primitive By Slot` itself (not yet decoded — it's called
through an interface, so its concrete implementing class isn't yet identified from this trace
alone) failing or no-op'ing under some condition, e.g. an asset reference that hasn't finished
streaming in yet at the moment it runs, 0.05s after replication — back to an asset-streaming-timing
explanation, consistent with the leading theory from earlier tonight's live IDA session.

**Next step, if continued**: identify which concrete class implements `Preview Set Equipped
Primitive By Slot` (search `research/CXXHeaderDump/` and the FModel export for that exact function
name across classes) and decode it — that's the actual terminal function that would explain a
timing-dependent failure to apply.

## FINAL CORRECTION: RepPrimitiveActorsData is for the character-CREATOR PREVIEW, not live gameplay

`Preview Set Equipped Primitive By Slot` (the terminal function `UpdatePrevFromPrim` calls) lives
on `research/CXXHeaderDump/BPI_PreviewChar.hpp` — `IBPI_PreviewChar_C`, an interface whose entire
function set is unambiguously the character-creator preview mannequin: `SetPreviewRenderTarget`,
`SetPreviewZoomInOut`, `SetPreviewMeshRotation`, `SetCaptureActivate`, `PreviewOnWeaponEquipped`,
etc. — a render-target-captured preview camera for the character customization screen, not the
live in-game character.

**This closes out the entire `RepPrimitiveActorsData`/`ForceRepPrimitiveActorSpawns`/
`UpdatePrevFromPrim` investigation thread as a dead end for tonight's actual bug.** It's not just
"ruled out by one live data point" as recorded earlier — it's structurally a completely different
subsystem (character creation preview), never involved in live in-game appearance sync at all. The
live IDA read earlier tonight that found it "healthy" while the real mesh was null makes total
sense now: it's healthy because it's tracking the preview mannequin's own state, unrelated to
whatever's wrong with the live character.

**Do not investigate `RepPrimitiveActorsData` further for this bug family.** The real live-gameplay
appearance-application path is still `Svr_AttachClothing`/`MC_AttachClothing` (confirmed native
stubs, dead end for Blueprint tracing) and whatever native code they call — genuinely still
unresolved, and likely requires native disassembly (IDA) to go further, with the caveat that IDA
attach has hung this game 3-for-3 today and should be treated as high-cost if attempted again.

## Remaining captured-but-undecoded bytecode cleared out (2026-08-15)

Two agents decoded all 27 remaining `.bin` files that were already captured but never
disassembled — `research/bytecode_decoded_batch1.md` (BP_PlayerCharacter_C core functions +
Jigsaw/item-drop path, 11 files) and `bytecode_decoded_batch2_animbp.md` (the full Player_AnimBP_C
animation system + a minor UI widget, 16 files). Notable, not directly tied to tonight's main
investigation:

- `MeleeTrace` is a clean, standard sphere/multi-trace — nothing suspicious, good reference for
  the still-pending Phase 4 melee hit-detection work.
- `JSIContainer_C_PerfromDrop` (real item-drop logic) ends in a `CallMulticastDelegate` broadcast —
  worth checking if any future equip/sync investigation reopens, since a missed delegate broadcast
  is exactly the kind of gap already found elsewhere tonight (`OnEquipmentUpdated`'s zero listeners).
- `BlueprintThreadSafeUpdateAnimation` is a thin dispatcher into ~6 `Get*` functions (all now
  decoded); `GetThreadSafeBooleans` is a flat gameplay→anim-thread bool-mirror copy, explaining the
  "thread safe" naming; `GetHeadRot`/`GetAimOffset` do clamped+interpolated look-at IK math with
  explicit degree constants.
- `MC_ADS`/`OnActiveWeaponSlotChanged_Event_0`/`ItemDropRequest_Event_0` are trampoline stubs
  calling a native function with a single `EX_IntConst` literal — same shape as the already-
  confirmed-native `Svr_AttachClothing`/`MC_AttachClothing` stubs (very likely a Blueprint node-ID
  passed to an engine dispatch mechanism, not a hand-constructed parameter this project could get
  wrong — these are the game's own compiled trampolines, not our own `ProcessEvent` calls, so NOT
  in scope for the earlier parameter-mismatch audit).

**Every `.bin` file ever captured by this project is now decoded.** Combined with the earlier
9+2 structural catalogs and the 2 parameter-correctness audits, this is the fullest picture of the
codebase-vs-real-game relationship this project has had. Remaining unresolved `FName ci=` values
across all these functions are listed in their respective batch docs for a future
`resolve_fname.flag` pass if any of them become relevant to a specific question later.

## Extended equip_restore_retry (orphan-attach + drift repair) to cover proxies (2026-08-15)

Live-confirmed the actual gap behind today's remaining gun/glasses/knife reports: `DETACHED` had
fired ZERO times all session, `re-snapped DRIFTED` zero times, despite repeated visual reports —
because `equip_restore_retry` (both the original orphaned-`AttachParent` repair and today's new
drift-repair) only ever ran against the LOCAL pawn. A screenshot showing PC1's AK15+pistol AND
PC2's knife floating simultaneously confirmed this is proxy-side fragmentation, a code path with
zero monitoring or repair this entire session.

Extended `do_equip_restore_retry` to take a `label` (for clear per-actor logging) and run once per
proxy actor too, every 3s, alongside local — same `Equip Actor to Socket` call this project has
already proven safe for proxies elsewhere (`proxy_manager.cpp`'s own item-spawn/attach code), just
never previously applied to this specific repair loop. `equipDataReady` (the join-race gate) stays
local-only, since that's specifically what it's for. Deployed to both machines — not yet
live-verified against a real proxy fall-off.

Still NOT covered: `attach_health`'s "mesh asset cleared while still attached" detection (item
stays in `AttachChildren`, mesh goes null) has no repair for local OR proxy — still log-only. If
proxy gear loss persists after this fix, check whether it's landing in that category instead
(`attach_health: proxyN ... MESH ASSET CLEARED ... still attached` in the log) rather than the
orphan/drift categories this fix addresses.

## Major addition: weapon-attachment drift repair + hair/beard/eyebrows/mouth/hands repair (2026-08-15)

Per user's request to close out every remaining known repair gap. Two substantial additions:

**1. Weapon-attachment (scope/mag/suppressor/etc.) positional drift detection + repair.** Extended
`attach_health`'s existing scan (which already walks both top-level equipped items AND one-level-
deep weapon attachments, for both local and proxy) to also track each visited child's
`RelativeLocation` tick-to-tick, and re-snap via `K2_AttachToComponent` on a >30-unit jump — same
threshold as every other drift check in this project. Repair uses the child's OWN currently-
recorded socket (native `GetAttachSocketName()`, confirmed present in `Engine.hpp`) rather than
looking up which item-data field applies, so it works uniformly for any attached child regardless
of type. This covers what `equip_restore_retry` (RepActorsData-only) never could: attachments
aren't tracked in that array at all, only in each item's own `RepAttachments`. Also fully explains
the "guns fall off" reports that had ZERO log hits all session — nothing was watching this layer.

**2. Hair/Beard/EyebrowsMesh/Mouth/Hands repair** (previously log-only — `component_drift` could
see them go missing but had no fix, since `UpdateBodyParts` only knows `"Torso"/"Legs"/"Feet"` by
name). Two different mechanisms depending on local vs proxy:
- **Proxy**: new `ProxyManager::force_resync_appearance(AActor*)` — finds the matching
  `RemotePlayer`, clears `appliedAppearanceKey` and sets `appearanceDirty=true`, so the existing
  (and now `bReinitPose`-fixed) `sync_pawn_appearance` fully re-runs on the next tick, reapplying
  hair/beard/eyebrows/mouth/all body parts together in one shot — no new apply logic needed, just
  forcing the existing correct logic to fire again.
- **Local**: no equivalent "resync" exists for the local pawn, so this needed new pieces: a
  per-field "last known good" appearance cache (`state.hpp`'s `lastGoodLocalAppearance`, merged
  in `send_pawn_appearance` — only overwrites a field when a read comes back non-empty, so a
  partial clear doesn't erase still-good cached fields) feeding a new exported function,
  `sdb::reapply_named_mesh(component, meshShortName, isSkeletal)` (factored out of
  `sync_pawn_appearance`'s own per-part logic in `proxy_manager.cpp`, reusing its
  `find_object_by_short_name` resolver and the correct `SetStaticMesh`/`SetSkinnedAssetAndUpdate
  (bReinitPose=false)` call shape). `component_drift`'s repair dispatch now branches on a new
  `appearanceField` tag per tracked component ("hair"/"beard"/"eyebrows"/"mouth"/"hands") to call
  this with the right cached name.

Both go through the same existing grace/throttle/cap machinery (2s grace, 1/s throttle, 60-attempt
+ 5-minute hard ceiling) — no new timing risk introduced.

Deployed to both machines, not yet live-verified. Given today's freeze history, watch closely on
first relaunch — this is the largest single change of the night in terms of new code paths
touched, even though each individual piece reuses already-proven call shapes.

## Freeze recurred yet again after the drift/appearance repair deploy — isolated it off too

PC1 froze again shortly after the weapon-attachment-drift + appearance-repair deploy. Log showed
NO evidence either new code path had fired yet before the freeze (last activity was pre-existing
`component_drift` Torso/Legs/Feet repair, `read_local_weapon_attachments` — nothing new). Combined
with the earlier isolation test (ALL repair disabled, froze anyway), this freeze's actual cause is
still unconfirmed and increasingly doesn't look like it's any specific repair-loop content.

Kill-switched both of tonight's newest additions anyway (`kEnableItemDriftCheck=false`,
`kEnableAppearanceRepair=false`) as a further isolation step, specifically targeting the new
DETECTION read added tonight (RelativeLocation read for every attached child, every 300ms — new
load even when no repair ever fires) separately from repair itself. Deployed to both machines.

**Status: freeze cause still not confirmed after multiple isolation attempts spanning most of
tonight's changes.** If it recurs even with these newest additions fully inert too, that would
point toward something pre-existing (present before tonight's session even started) or genuinely
unrelated to this mod's own logic — worth considering seriously if the next occurrence also shows
no correlation.

## Re-enabling isolated features one at a time (2026-08-15)

Game stable for a stretch with both new features off. User chose to re-enable
hair/beard/eyebrows/mouth/hands repair (`kEnableAppearanceRepair=true`) first;
weapon-attachment drift check/repair (`kEnableItemDriftCheck`) stays off for now. If stable,
that isolates appearance-repair as safe and narrows any future freeze toward the drift-check
code specifically; if it freezes again, appearance-repair becomes the next suspect.

## Both new features now fully re-enabled (2026-08-15)

Appearance repair confirmed stable on its own. Re-enabled weapon-attachment drift check/repair
(`kEnableItemDriftCheck=true`) as well — both of tonight's newest additions are now active
together. If stable from here, the freeze is confirmed unrelated to either; if it recurs, drift-
check is the remaining suspect (appearance-repair already cleared on its own).

## Root cause of severe full-body proxy fragmentation found: pure positional, not mesh-clearing

Two screenshots: PC1's own gear (gun/axe) floating nearby but attached, and much more severe —
PC2's ENTIRE body scattered (head/hands/boots/torso/shirt all flung apart) as seen on PC1's
screen. Checked the log: **every one of `component_drift`'s mesh-asset checks for proxy0's body
components read `SET` continuously — Torso/Legs/Feet/Hands/HairMesh never cleared.** The meshes
were never missing; they were positionally wrong from the very first sample. `component_drift`'s
existing `DRIFTED` check (tick-to-tick `RelativeLocation` delta) never fired either — same
blind spot as every other transition-only detector tonight: if a component is ALREADY scattered
the moment tracking starts (right after proxy spawn), there's no "before" to diff against.

**Fixed with an absolute check instead of a relative one.** These are permanent, non-socketed body
components skinned onto the character's main skeleton — a healthy one always sits at `(0,0,0)`
relative to its parent `Mesh`, never anywhere else. Added: if a tracked component's
`RelativeLocation` is ever more than 50 units from origin, snap it back via
`K2_SetRelativeLocation(0,0,0)` directly — no history needed, no baseline-comparison gap possible,
since it's a standing invariant check ("is this where it should always be"), not a change-detection
one. Runs on the SAME throttle as the existing mesh-asset repair (2s grace, 1/s), for both local
and every proxy (this check already ran for both labels, only the repair action is new).

This is a different, more fundamental fix than the RepPrimitiveActorsData/replication-timing
theories chased earlier — those explain why a mesh reference might go null; this explains why a
component might sit somewhere visually wrong (0.0.0.0 vs the character in your screenshot's case,
however whatever pushed it there stays theoretically unresolved) with its mesh intact the whole
time. **`Clothing_Torso/Legs/Feet` were ALSO stuck `MISSING` (mesh-asset, not position) in the same
proxy at the same time** — that's a separate, already-covered issue (appearance-repair, force-
resync), so both mechanisms were needed together for this one proxy.

Deployed to both machines, not yet live-verified. This is the third of tonight's biggest structural
additions (after bReinitPose/ScaleRule/ReturnValue and the drift/appearance repair pair) — watch
closely given the freeze history, though this repair (unlike drift/appearance) hadn't been isolated
as a freeze suspect since it didn't exist until just now.

## Confirmed absolute-position repair is live-working; added missing Clothing_Gloves/Armor tracking

New off-origin repair confirmed firing live: `component_drift: proxy0:Mouth re-centered
OFF-ORIGIN component from (0.0,157.0,0.6) to (0,0,0)` and the same for `local:Mouth` moments
later — the fix works exactly as designed, catching and correcting a real off-origin component
with no prior baseline needed.

But a follow-up screenshot (PC1's own hands/boots floating) showed no matching `Hands`
DRIFTED/re-centered hit despite `Hands` mesh reading `SET` throughout. Root cause: `Clothing_Gloves`
(`BP_PlayerCharacter.hpp` @`0x0780`) was never added to the tracked list at all — every OTHER
clothing slot (Torso/Legs/Feet) got added over the course of tonight except gloves (and armor,
@`0x07B8`, also missing). The floating "hands" were very likely the bare-hand mesh showing through
because the glove overlay fell off, same "skin visible through gaps" class of bug already fixed
for Torso/Legs/Feet, just never extended to these two. Added both with a best-guess
`OnRep_ClothingGlovesEquipped?`/`OnRep_ClothingArmorEquipped?` name (following the exact
established convention, safe no-op fallback if the guess is wrong — same as every other
speculative function-name lookup in this file). Deployed to both machines.

## LIKELY REAL FREEZE ROOT CAUSE FOUND: a known-load-bearing grace period was disabled and forgotten

User's most specific freeze report yet — "PC1 froze when PC2 loaded in" — led directly to it.
`ProxyManager::tick()` had a 2-second grace period before hitting a freshly-spawned proxy with its
full sync burst (`sync_equipment`/`sync_active_weapon_hand`/`sync_weapon_attachments`/
`sync_pawn_appearance`), with an EXISTING comment from a much earlier session (Session 52)
documenting exactly why: **this same sync burst, run immediately after a fresh proxy spawn, had
already caused two separate live crashes/deadlocks** at different call sites each time, because
the proxy's own components aren't fully ready the instant it's spawned. The grace period was
**disabled "temporarily, for testing" on 2026-08-13 and never re-enabled** — sat forgotten for two
days, including this entire session's worth of freeze-chasing. Its own comment even said: "the one
most likely to actually be load-bearing — re-enable first if anything crashes right as a new proxy
spawns in." Nobody had, until now.

This is a much stronger candidate than anything investigated tonight (bReinitPose, ScaleRule,
ReturnValue, the repair-loop isolation tests, the new drift/appearance code) — it's a PRE-EXISTING,
already-diagnosed, already-fixed-once issue that got silently regressed, not a new bug. Every
freeze tonight happened during active two-player testing, which necessarily means proxy spawns
were happening throughout (redeploys/relaunches constantly re-trigger this exact path) — fully
consistent with every occurrence, including the ones that showed no correlation to any of tonight's
actual new code (this explains those: the real cause was never in the new code being tested, it
was in a much older path that regressed silently).

Re-enabled the grace period exactly as it originally existed. Deployed to both machines. This
should be tested in isolation from any further new changes — if freezing stops, tonight's entire
freeze-chasing thread resolves to "a real fix existed, was accidentally turned off, forgot to turn
back on" rather than any of the theories entertained along the way.

---

**⚠️ DATA LOSS NOTICE (2026-08-16, ~15:44)**: this file was accidentally overwritten with a single
placeholder line (used `Write` instead of `Edit`) and had to be restored via `git restore` to the last
commit (`3e51ef0`, 2026-08-15 13:11:49) — everything written between that commit and the overwrite was
uncommitted and is genuinely gone except for what could be reconstructed from this conversation's own
visible history (things this session itself had just read or written). What follows below this notice
is that reconstruction: verbatim where this session had the exact text in hand, otherwise a clearly
labeled gap. Anything from earlier sessions/segments that isn't reproduced here (the full "Session 60"
death-sync writeup, the original DeathState investigation entries, the CurrentActor-theory correction
entry, the OnExecuteInteract-real-body discovery entry, and whatever preceded the partial section
recovered just below) is lost for real and would need to be re-derived from scratch if needed again —
none of the underlying CODE was affected, only this narrative log.

**Partial recovery** (verbatim, from this session's own earlier `Read` of the pre-overwrite file — the
paragraph starts mid-list because that's exactly where this session's own read window began; whatever
came before "re-attach/re-parent validation" in the original is NOT recovered):

> re-attach/re-parent validation, but the specific class/function pointers involved are unresolved
> (would need a fresh live capture + batch resolve to name them). Doesn't touch offsets 0xE0/0xE8
> directly as far as the decode shows.
> - **`SetCanInteract(bool CanInteract, bool EnablePhysics, bool& Result)`** — 576 bytes. Confirmed:
>   writes both bool params straight into two instance variables (near-certainly `CanBePicked`/
>   `EnablePhysics`, the two properties with matching `OnRep_` callbacks per the header), then manually
>   invokes each one's own OnRep callback right after — since this component isn't really being
>   replicated to anyone, nothing would fire those OnReps on its own, so the Blueprint calls them
>   itself. This is almost certainly what `JigSetCanInteract` (called on `BP_SkeletalMeshPickup_C`,
>   which this project already calls via `reassert_no_interact`) ultimately calls down into. Behaves
>   as expected — not itself a bug source, though it does confirm `EnablePhysics`/`CanBePicked` really
>   are the two properties this project has been fighting to keep pinned false.
> - **`CheckMismatch()`** — 1601 bytes, a validation/diagnostic loop over a container array, comparing
>   configs and logging `"Container config mismatch for ... in pickup: ... Check data asset container
>   dimensions and Pickup container settings!"` on mismatch. Reads like a data-integrity check, not a
>   physics/attach trigger. Not the culprit as far as the decode shows.
> - **`OnRep_CanBePicked?()`** — 310 bytes, **`OnRep_EnablePhysics?()`** — literally 3 bytes (an
>   effectively-empty stub, likely just `return;`). Neither dump has been traced in detail yet.
> - **`CanInteract?()`** — 22 bytes, trivially small (a bare getter, presumably `return CanBePicked;`
>   or similar) — not decoded further, too small to matter.
>
> **Honest status**: none of these six confirms the actual trigger for physics/attach reset. The most
> promising unexplored lead is `OnExecuteInteract`'s real body at Ubergraph entry 2365 — worth pursuing
> next by dumping the full `ExecuteUbergraph_BP_JigPickupComponent` and disassembling from that offset,
> the same technique already used successfully for `Svr_RequestRespawnSuicide`/`Client_SUICIDE` earlier
> tonight. Also untried: `BP_WeaponsPickupComponent_C` (a distinct class from `BP_JigPickupComponent_C`,
> already dumped once as `ExecuteUbergraph_BP_WeaponsPickupComponent.bin` — weapons may go through a
> specialized subclass with its own logic not covered by anything decoded above).

### Session continues: live hang, then live crash, from two "proactive unconditional repair" changes — both reverted/kill-switched

Same night, later: chased the "pants/shirt/mag/laser/shotgun freeze" reports (items/clothing visibly
stuck or detached-with-physics on screen, while every existing DETACHED/DRIFTED/component_drift
diagnostic showed nothing wrong) to two real, decoded mechanisms:
1. Body/clothing meshes are leader-pose FOLLOWERS (`bUseBoundsFromLeaderPoseComponent: true` per the
   FModel export) whose bone-mapping can go stale independent of the mesh asset itself staying valid —
   confirmed live: `local:Clothing_Legs` got exactly one "initial state" log line and nothing else for
   20+ minutes, while `local:Legs` (reactive-path-eligible since clothing keeps its own asset check
   reading null) got refreshed ~63 times in the same window.
2. Weapons/attachments are rigid socket-attached actors (confirmed no leader-pose property on any
   pickup class header) where `AttachChildren`'s count never changed (heartbeat steady at 16) and the
   socket-distance check read clean, yet the item visibly rendered elsewhere — consistent with a
   render-thread transform-refresh notification not firing for some re-attach path, invisible to any
   check that (like ours) only reads the game-thread-side transform.

Both fixes shared the same shape: make a previously-reactive-only repair call (`SetLeaderPoseComponent`,
`K2_AttachToComponent`) fire **unconditionally on a timer** instead of only when a discrepancy was
detected, since a stale-but-self-consistent cache produces no detectable discrepancy in the first place.
**Both caused real live failures on the very next test, in different ways**: at 1/s/component (up to
~20 components per character), both PC1 and PC2 hung simultaneously (debug.log stalled 90-120s+ on
both machines, matching this project's known GameThread-parked-in-`WaitUntilTasksComplete` deadlock
signature). Widened the throttle 1s→20s as a first response; the very next test instead **crashed**
(`0xe06d7363` — unhandled C++ exception, not an access violation — stack running entirely through this
mod's own module, reached via `user32`→the game's own message dispatch, i.e. through the new
WndProc-subclass tick path, while the player was idle; no minidump was generated for it). Two different
failure modes from the same addition across two consecutive live tests is treated as strong enough
evidence that the *mechanism* — not just its frequency — is unsafe in some circumstance not yet
identified, most likely still this project's own documented `SetSkinnedAssetAndUpdate`/attach-vs-
Parallel-Anim-Update-worker-thread lock-contention hazard, just triggered far more often by an
unconditional timer than by the original occasional reactive path.

**Current state**: both proactive additions are kill-switched back to `false` (reactive-only behavior,
the same shape that was stable all session before tonight). The `Clothing_Legs`-never-revisited gap and
the weapon-render-desync gap are both real, known, and currently unfixed again — deliberately, until
this can be root-caused against an actual symbolized dump rather than live-guessed a third time. Also
added a defensive `seh_invoke` wrap around the WndProc path's `do_game_tick(true)` call itself (it was
the only clean-context trigger site calling it unguarded) as cheap belt-and-suspenders, independent of
whether it would have caught this specific exception.

### `ExecuteUbergraph_BP_JigPickupComponent` — full fresh decode + name resolution

User asked to "start byte decoding EVERYTHING" while away for a few hours. Picked up the exact thread
left open above. Game was still live on PC1, so did this properly: re-dumped
`BP_JigPickupComponent_C::ExecuteUbergraph_BP_JigPickupComponent` fresh (`bytecode_dump.flag`) rather
than reusing the old `.bin` sitting in `%APPDATA%\SurrounDeadBridge\` from earlier tonight — **confirmed
live why this matters**: disassembling the OLD dump and resolving its `ci=` values against the CURRENT
process produced a mix of clean names and garbled CJK-looking noise with recognizable English fragments
bleeding across different CIs' output (e.g. `ci=1841846` and `ci=1841857` returned overlapping tails of
the same giant run-on string). First suspected a disassembler desync bug; ruled that out by re-dumping
fresh and diffing against the old decode — the two dumps are byte-identical in length (3406 bytes) but
the embedded `ci=` values differ throughout (e.g. the very first `EX_LocalVirtualFunction`'s target went
from `ci=1842750` to `ci=1845901` for what's structurally the same call) — **directly confirms FName
ComparisonIndex values are assigned dynamically per-process at runtime, not baked into cooked bytecode
as stable constants**, even in a Shipping build. Resolving the fresh dump's CIs against the same live
process it was captured from resolved all 22 previously-unresolved names cleanly, no garbage. Lesson
banked for all future decode work this session: always re-dump immediately before resolving, never trust
an old `.bin`'s CIs against a differently-aged process.

Full resolved name table for this Ubergraph's `EX_VirtualFunction`/`EX_LocalVirtualFunction`/`EX_NameConst`
targets (fresh dump, this process instance only — re-resolve after any relaunch):
`92404`=`SetSimulatePhysics`, `93336`=`K2_DestroyActor`, `1729580`=`Jig.InteractOptions.Option1`,
`1790698`=`Jig_SetAttachmentActiveState`, `1797933`=`GetCapacity`, `1844012`=`AddJigWidgetToContent`,
`1844651`=`CanLootPickup?`, `1844671`=`CheckMismatch`, `1844793`=`ExecuteUbergraph_BP_JigPickupComponent`
(self-reentry), `1844888`=`FindLocalAttachmentByUID`, `1844949`=`GetMainSceneComp`,
`1845044`=`IsInventoryVisible?`, `1845055`=`IsValidPickup`, `1845494`=`OnInteractActorOverDistance`,
`1845509`=`OnPickupInteractExecuted`, `1845650`=`ProcessStartingItems`, `1845678`=`RepCapacity`,
`1845711`=`RequestServerData`, `1845721`=`SERVER_UpdateAttachmentState`, `1845768`=`SetCount`,
`1845773`=`SetInteractDistance`, `1845796`=`SetupInventoryWidget`, `1845868`=`UpdateAttachments`,
`1845878`=`UpdateAttachmentStateByUID`, `1845901`=`ValidateAttachedActor`.

**Genuinely new and directly relevant to tonight's open bug**: `OnInteractActorOverDistance`,
`SERVER_UpdateAttachmentState`, `UpdateAttachments`, `UpdateAttachmentStateByUID`,
`Jig_SetAttachmentActiveState`, `FindLocalAttachmentByUID` — none of these were previously known to
exist on this component at all. `OnInteractActorOverDistance` in particular is a strong candidate for
what actually runs when `CheckDistanceFromActor`'s 300-unit auto-release check trips (previously only
traced as far as "starts a 0.1s repeating timer calling `CheckDistanceFromActor`" — never found what
that timer's own over-distance branch actually calls).

`OnInteractActorOverDistance` turned out NOT to be a standalone `UFunction` (`GetFunctionByNameInChain`
returned "not found" — it's an internal graph-local label only reachable via `EX_LocalVirtualFunction`
dispatch inside the Ubergraph, not separately reflectable). Deferred; came back to the two names that
WERE standalone functions and turned out to matter far more.

### `UpdateAttachments()` + `ProcessAttachments()` — the real native attachment pipeline, fully decoded

**`UpdateAttachments()`** (486 bytes, self-contained, not a trampoline) — clean, fully understood:
validates via `IsValidPickup` + two `BooleanAND`s against two instance-var gates, and if all pass:
calls `GetListOfAttachments` to populate a local struct/array, assembles it into an instance-var cache
(`RepAttachments`-backing struct), calls `MarkPropertyDirtyFromRepIndex(Self, 5, "RepAttachments")`
(`/Script/Engine.NetPushModelHelpers`, confirms `RepAttachments` is push-model replicated at rep index
5), **manually invokes its own `OnRep_RepAttachments()`** (same "not really replicated to self, so call
the OnRep by hand" pattern this project has seen elsewhere on this exact component), then calls
`ProcessAttachments()`.

**`ProcessAttachments()`** (4305 bytes) — the real spawn/attach/cleanup logic. High-confidence structure
(exact branch wiring around the reverse-scan section not fully pinned down byte-for-byte, but the
overall shape and every call target is confirmed via live `resolve_ptr`/`resolve_fname`):
- Loops over the `RepAttachments`-backed array (`InstanceVariable 0x1244b4280`'s array member). For each
  entry, calls `self.FindLocalAttachmentByUID(entry.UID, &outActor)` to check whether an attachment
  actor already exists for that entry.
  - **Not found (new attachment)**: spawns via `GameplayStatics::BeginDeferredActorSpawnFromClass` +
    `FinishSpawningActor` (the identical two-call pattern this project's own `spawn_proxy` already
    uses), computes a transform via `GetSocketTransform` (`ci=44895`) + `KismetMathLibrary::
    MakeTransform`, calls `Actor::K2_AttachToComponent` (`SnapToTarget`/`SnapToTarget`/`KeepWorld`,
    matching this project's own re-snap parameter choice — confirms that part was never the issue),
    `SetActorScale3D`, then casts the new actor through an interface and calls
    `Jig_GetAttachmentInfo`/`Jig_GetPrimitiveComponent`/`Jig_SetAttachmentInfo` on it (component-level
    bookkeeping, not yet decoded further), and adds it to a tracking array
    (`InstanceVariable 0x1244b4380`) via `Array_AddUnique`.
  - **Found (already exists)**: a separate branch (~offset `0x0c46`) that does NOT re-spawn — instead
    re-resolves the socket/primitive component and calls the same `Jig_SetAttachmentInfo`-family
    interface calls again, i.e. an "update existing attachment's info without respawning" path.
  - A second, reverse pass over the tracking array (`InstanceVariable 0x1244b4380`) checks each tracked
    actor against `Array_Contains` on a current-UID list; entries no longer present get
    `K2_DestroyActor`'d and `Array_Remove`'d from the tracking array — the stale-attachment cleanup path.
  - **`CallMulticastDelegate` on `InstanceVariable 0x1244b6200` (`OnAttachmentsUpdated`) fires as an
    integral part of this same flow** (`func=0x132675fe0` resolved to
    `BP_JigPickupComponent_C:OnAttachmentsUpdated__DelegateSignature`), not bolted on afterward by some
    other system.

**Why this matters for tonight's mag/laser/shotgun render-desync bug**: this project's own repair code
(`do_attach_health_scan`'s OFF-SOCKET check) reacts to a detected problem by calling `K2_AttachToComponent`
directly on the drifted child — which matches ProcessAttachments' OWN attach call parameters exactly, but
skips everything around it: no `FindLocalAttachmentByUID` re-check, no `Jig_SetAttachmentInfo`-family
interface bookkeeping, and critically **no `OnAttachmentsUpdated` broadcast**. If anything else in the
game (UI, or whatever the render-thread-transform-refresh path actually depends on) is wired to that
delegate specifically — plausible, since it's a real, named, purpose-built multicast delegate on this
exact component — rather than reacting generically to any `K2_AttachToComponent` call, a bare direct
re-attach would never trigger it, fully explaining why our own re-snap reads clean but the screen stays
wrong. **Not yet tested** (the unconditional-re-snap experiment was kill-switched off after the hang/
crash before this was found) — the next, better-grounded experiment is calling `UpdateAttachments()`
itself (or at minimum broadcasting `OnAttachmentsUpdated`) instead of a bare `K2_AttachToComponent`, once
this can be tested without repeating tonight's live-crash cycle. `UpdateAttachmentStateByUID`,
`SERVER_UpdateAttachmentState`, and `Jig_SetAttachmentActiveState` (found but not yet dumped) are likely
the actual RPC/authority-aware entry points a real fix should route through rather than calling
`UpdateAttachments` directly (component replication/authority not yet confirmed either way).

### `OnAttachmentsUpdated` — the "who's listening" question, answered live

Added a new read-only diagnostic (`check_dump_delegate_trigger` in `mod.cpp`, flag file
`dump_delegate.flag`, two lines: class name + delegate property name) after establishing this project
had already answered the exact same question for a DIFFERENT delegate once before
(`OnEquipmentUpdated`, found `count=0` via a live IDA-debugger read earlier this session) but that
capability isn't reachable through what's connected right now (checked: the connected `ida` MCP server
only exposes static IDB resources with no UE types modeled; its HTTP bridge on :13337 only implements a
`ping` RPC; IDAPython is in-process-only per Hex-Rays' own docs). Reused
`GetValuePtrByPropertyNameInChain` (already used everywhere in this codebase) instead of a raw offset —
it returns a pointer straight to the delegate's own storage, which per the `OnEquipmentUpdated`
precedent IS the `FMulticastScriptDelegate`'s `TArray<FScriptDelegate>` directly (data ptr/count/max,
standard TArray layout).

**Result, live, on `BP_JigPickupComponent_C`**: `InvocationList data_ptr=0x12a3c1e80 count=1 max=4` —
**one real subscriber**, unlike `OnEquipmentUpdated`'s zero. Read the actual `FScriptDelegate` entry via
`mem_dump.flag` (`abs 12a3c1e80 2`): first qword is the bound object's `TWeakObjectPtr`
(ObjectIndex=175432, SerialNumber=66964 — a `GUObjectArray` index, not directly resolvable to a pointer
without walking that array, which isn't set up yet), second qword is the bound function's `FName`
(ComparisonIndex=1823023). Resolved that CI live: **`"OnAttachmentsUpdated"`** — the bound function has
the SAME NAME as the delegate itself, the standard UE/Blueprint signature of "Bind Event to
OnAttachmentsUpdated" auto-generating a matching Custom Event node. Strongly suggests a self-contained
handler (very plausibly on this same component, or its owning actor) reacting to its own broadcast —
real logic beyond attach bookkeeping, not a dead/unbound delegate.

**Verdict on the render-desync fix candidates**: this makes the `OnAttachmentsUpdated`-broadcast path a
real, live-confirmed lead, not just a theory — there IS something on the other end that would run if
this project's repair code broadcast it instead of (or alongside) the bare `K2_AttachToComponent` it
calls today. Doesn't resolve which of the two candidates (this, or `ValidateAttachedActor`'s
transform-snap) actually fixes the visual bug — that still needs a live test, deliberately not attempted
yet given tonight's hang/crash history — but both are now equally well-grounded rather than one being
speculative. Next research step if pursued further: resolve the `GUObjectArray` index to find out
exactly what object/class owns that Custom Event (would confirm self-bind vs. some other system
entirely), though the practical fix experiment doesn't strictly need that answer first.

**Follow-up, same night**: tried finding the bind statically instead of walking `GUObjectArray` — the
disassembler already recognizes `EX_BindDelegate`/`EX_AddMulticastDelegate`/`EX_InstanceDelegate`
(opcodes `0x61`/`0x5C`/`0x4B`), so searched for those directly. `BP_JigPickupComponent_C`'s own
Ubergraph (fully decoded): zero matches — it doesn't bind anything to its own delegate.
`BP_JigMultiplayer_C`'s Ubergraph (24576 bytes, fully decoded): 6 binds, resolved every one —
`Drop_ItemOverItem_Event_0`, `EventOnJigItemMouseButtonDown`, `ItemDropRequest_Event_0`,
`EventOnInventoryAction`, `OnItemSplitRequest_Event`, `OnItemStackRequest_Event` — all real inventory
UI/drag-drop events, none attachment-related. `BP_PlayerCharacter_C`'s Ubergraph (205862 bytes — by far
the largest decode tonight, still disassembled in well under a second): hundreds of `EX_BindDelegate`
calls with suspiciously regular CI spacing (mostly exactly +23 or +24 between consecutive ones) —
consistent with `bp_catalog_player_core.md`'s own already-documented finding that this class has ~230
auto-generated per-node stubs (montage/timeline notify callbacks, `OnBlendOut_<hash>`-style), not
hand-authored bindings. Did not exhaustively resolve all of them — the volume makes that impractical for
a question that isn't blocking the actual fix experiment. Parked here; if picked back up, the efficient
next step would be filtering for BindDelegate calls whose nearby `AddMulticastDelegate` context resolves
to a `BP_JigPickupComponent_C`-typed object reference, rather than resolving CIs one at a time.

### `ValidateAttachedActor` fix implemented — built, not yet live-tested

Replaced the OFF-SOCKET repair action in `do_attach_health_scan` (`mod.cpp`'s absolute-check block) —
previously a bare `K2_AttachToComponent` re-parent — with the real game's own pattern: `GetOwner()` on
the drifted child, `GetSocketTransform(SocketName, RTS_World)` on `ctx->mesh` (the confirmed parent —
skipped `ValidateAttachedActor`'s own `GetAttachParentActor`/`Character`-cast re-derivation, since this
scan already knows both from its own calling context), then `K2_SetActorTransform(transform, bSweep=
false, hit, bTeleport=true)` on the item's owning actor. Verified every parameter shape against the
actual decoded bytecode directly (`decoded_ValidateAttachedActor.txt`) rather than the earlier summary,
byte for byte — matches exactly.

Kept deliberately **reactive only** — still gated on `offSocket` at the same throttle as before, no
unconditional/timer-driven variant this time, per the lesson from tonight's hang and crash (both came
from making a previously-occasional repair fire unconditionally, not from the repair action itself).
Builds clean, hash `b83ac44e...`. Not yet deployed or live-tested — next real step whenever picked back
up.

(Full per-function detail for the attachment subsystem now lives in `research/bp_catalog_jigpickup_bytecode.md`
— a flat lookup table, kept in sync with this log's narrative going forward rather than duplicated here.)

### `Client_SUICIDE` / `Svr_RequestRespawnSuicide` — finally decoded, the deferred Suicide follow-up

Both are 18-byte trampolines into `BP_PlayerController_C::ExecuteUbergraph_BP_PlayerController` (9978
bytes, fresh-dumped and disassembled — decoder hit an unhandled opcode `0xb0` at byte 8628, past both
target entry points, so both are fully readable regardless).

**`Client_SUICIDE`** → Ubergraph offset `0x1e64` → `0x1dec`: calls
`GameFunctionLibrary.GetLevellingComponent(Self)` then `.XPDeath()` on the result (client-side XP
penalty for suicide), then `WidgetLayoutLibrary.RemoveAllWidgets(Self)` (clears UI), then continues into
shared tail logic at `0x1b48` (not yet traced — likely the same death-screen/UI path `Client_Died` also
uses, not decoded this pass).

**`Svr_RequestRespawnSuicide`** → Ubergraph offset `0x18b7` → `0x17b7`: calls
`GameFunctionLibrary.GetGameInstance(Self).Survival_SuicideRespawn()` — a DEDICATED suicide-specific
respawn entry point on the GameInstance, distinct from the `Survival_Respawn(bool)` also visible nearby
in the same Ubergraph region (called with `false`/`true` in neighboring blocks, likely the normal-death
and some other respawn-reason variants sharing this same tail code region via Kismet's common
compiled-tail-sharing). **This is exactly where the earlier-documented "0.3-0.4s death→respawn cycle
with a brand-new pawn instance, causing an equipment-strip race" timing issue lives** — inside
`Survival_SuicideRespawn()` itself, which is on the GameInstance-derived class (name not yet confirmed;
`GetGameInstance` resolves through `GameFunctionLibrary`'s CDO, the actual runtime class wasn't
captured this pass), not on `BP_PlayerController_C` — a different class than dumped so far, needs its
own `FindFirstOf`/CDO lookup to reach. Not pursued further this pass; logged as the concrete next step
whenever the Suicide bug gets picked back up (still deferred, not fixed).

### `SD_GameInstance_C::Survival_SuicideRespawn()` — fully decoded, root cause of the equipment-strip race found

Found `SD_GameInstance_C` via `research/CXXHeaderDump/SD_GameInstance.hpp` (declares both
`Survival_SuicideRespawn()` and `Survival_Respawn(bool Random?)`). Dumped and disassembled cleanly
(1772 bytes, no unhandled opcodes). Real sequence, in order:

1. `GameFunctionLibrary.GetGameHUD(Self)` → `.Re-Initialise()` — resets the HUD.
2. `GetAllActorsOfClassWithTag(Self, <class>, "Survival", &array)` → `Array_Random` picks one → reads its
   `GetTransform()` (spawn transform source — the "Survival"-tagged actor here, not yet identified
   further; likely a single always-present settings/anchor actor, not a per-spawn-point marker despite
   the name proximity to `RespawnMarker` used later).
3. **`BeginDeferredActorSpawnFromClass(Self, <PawnClass>, transform, ...)`** — spawns a genuinely NEW
   pawn instance (confirms the earlier live-timing finding: this is not a respawn-in-place, it's a fresh
   actor).
4. **Before `FinishSpawningActor` is called**: `SetBoolPropertyByName(NewPawn, "ShowStartingQuests?", true)`
   and **`SetBoolPropertyByName(NewPawn, "SpawnStartingItems?", true)`** — the new pawn is explicitly
   flagged to spawn its own DEFAULT starting inventory.
5. `FinishSpawningActor(NewPawn, transform, ...)` — this is where the new pawn's construction/`BeginPlay`
   actually runs, almost certainly acting on the `SpawnStartingItems?` flag just set and giving the new
   pawn a fresh default loadout.
6. `PlayerController = GetPlayerController(0)` → `.Possess(NewPawn)`.
7. **`CallMulticastDelegate(NewPawn.KeepInventoryOnDeath)`** — broadcasts a delegate ON THE NEW PAWN,
   named exactly for restoring/keeping the previous inventory, fired AFTER possession, i.e. AFTER step 5
   already gave the pawn default starting items.
8. `CallMulticastDelegate(Self.PlayerRespawned)` — a general GameInstance-level respawn notification.
9. Three follow-up loops over `GetAllActorsOfClass` results calling `RespawnMarker`/`RespawnMarker`/
   `AddMarker` per-actor (map/UI marker bookkeeping, not inventory-related) — not decoded in full detail,
   not relevant to this bug.
10. `CastToController(...)` → `.Death_PlayerStats()` — death/respawn stat tracking.

**This is the equipment-strip race, found**: the new pawn gets a DEFAULT starting-item loadout
(`SpawnStartingItems?=true`, applied during step 5's `FinishSpawningActor`) and is SEPARATELY told to
restore/keep the OLD pawn's inventory via a delegate broadcast (`KeepInventoryOnDeath`, step 7) — two
independent inventory-populating mechanisms running back-to-back within the same ~0.3-0.4s window
already clocked live earlier tonight, on the same fresh pawn, with no visible ordering guarantee or
mutual-exclusion between them in this function itself. Whatever's bound to `KeepInventoryOnDeath`
(unknown — not this project's own code, needs a live scan of the delegate's bound-function array to
find) determines whether it fully overwrites the default items or races/partially-conflicts with them;
either way, this function's own logic is sufficient to explain a real, reproducible equipment-strip
window without needing any further hypothesis. Not yet fixed — this project doesn't own this code path
(it's the base game's own GameInstance logic, not something this mod calls or controls), so a real fix
would likely mean either intercepting/hooking around this window client-side, or determining whether
`KeepInventoryOnDeath`'s listener can be made to run BEFORE `FinishSpawningActor` rather than after
`Possess` (would need decompiling whatever's bound to that delegate, still unknown).

### `Client_Died` — brief note

Same trampoline pattern, Ubergraph offset `0x1e6e` → `0x18bc`: calls the cached levelling-component
instance var's `.XPDeath()`, `WidgetLayoutLibrary.RemoveAllWidgets`, then branches on
`ConsoleUtils.GetBoolCVar("Difficulty.Permadeath")` before continuing into further death-handling logic
not traced this pass. Not related to the equipment-strip race (that's `Survival_SuicideRespawn`, above).

### `Player_AnimBP_C::DeathState` — finally traced, resolves an old open question: audio-only, no mesh/attach effect

`DeathState`'s own trampoline (36 bytes) dispatches into `ExecuteUbergraph_Player_AnimBP` (6609 bytes,
fresh-dumped, disassembled cleanly) at entry offset `6092` decimal (`0x17cc`) — the exact marker this
project already knew about ("dispatches into the shared per-character Ubergraph at entry point 6092,
whose actual effect was never traced," from an earlier session). Now traced, start to `EX_EndOfScript`:

1. Stores the incoming bool param into an instance variable (the actual "state" write).
2. `TryGetPawnOwner()` (`ci=102667`), `IsValid` check, `DynamicCast<BP_PlayerCharacter_C>`, caches the
   typed result into another instance var.
3. `TryGetPawnOwner()` again → `K2_GetActorLocation()` → `GameplayStatics::PlaySoundAtLocation(Self,
   SoundCue'Swimming_Cue', location, rotation=(0,0,0), volume=1.0, pitch=1.0, startTime=0.0,
   AttenuationSettings='FootSteps', ...)`.
4. `TryGetPawnOwner()` a third time → `K2_GetActorLocation()` → same `PlaySoundAtLocation` call again,
   this time with `SoundCue'HeavySwimming3_Cue'` (otherwise identical params, same attenuation asset).
5. `EX_EndOfScript` — that's the entire effect. No mesh, pose, skeletal, or attachment call anywhere in
   this entry point.

**Resolves the earlier open question directly**: `DeathState` does NOT touch rendering/attachment/pose
in any way — its only real effect is playing two audio cues (oddly swim-themed — `Swimming_Cue` /
`HeavySwimming3_Cue` — rather than anything explicitly death-named; not investigated further why, but
irrelevant to this project's own bugs either way) at the pawn's location. This is exactly consistent
with the earlier live A/B test that found an identical item-detach pattern whether `DeathState` was
called or not — now explained structurally rather than just empirically: there was never a mesh/attach
code path in `DeathState` for that A/B test to have been isolating in the first place. Fully closes this
thread; `DeathState` can be ruled out from any future mesh/attachment/render-desync investigation.

### `LevellingComponent_C::XPDeath` — straightforward, confirmed harmless

Self-contained (469 bytes, no trampoline). `Greater_IntInt(CurrentLevel_instancevar, 1)` gate — only
applies if level > 1. If so: resets two XP-progress instance vars to `0.0`, decrements the level
instance var by 1 (`Subtract_IntInt(level, 1)`), recomputes an XP-threshold value
(`Round(Conv_IntToDouble(Multiply(Divide(prevThreshold, 2.65), 2.0)))`, stored back to an instance var),
then `PrintString("XP DEATH", color=(0, 0.66, 1, 1), duration=2.0)` — a debug-only on-screen message.
Exactly what the name implies: an XP/level penalty on death, gated to not go below level 1. Not related
to any bug this project is chasing.

### Session status, decode phase — where this leaves things

Summary of tonight's "decode everything" pass, for whoever picks this back up:

- **Attachment/render-desync bug** (mag/laser/shotgun/shirt visibly stuck or floating while game-thread
  state reads clean): root mechanism now understood in detail (`ProcessAttachments`'s
  `OnAttachmentsUpdated` delegate broadcast, skipped by this project's own bare `K2_AttachToComponent`
  repair call). Concrete next step identified (call `UpdateAttachments()` or broadcast the delegate
  instead) but **not yet implemented or tested** — the unconditional-re-snap experiment that would have
  tested a related theory caused a live hang then a live crash and was kill-switched off before this
  finding was made. Full detail: this log's `ProcessAttachments`/`UpdateAttachments` section above, plus
  `research/bp_catalog_jigpickup_bytecode.md`.
- **Suicide equipment-strip race**: root cause fully found (`Survival_SuicideRespawn`'s
  `SpawnStartingItems?=true` + later `KeepInventoryOnDeath` broadcast racing on the same new pawn). Not
  this project's own code (base game GameInstance logic) — no fix implemented, needs a decision on
  whether/how to intervene from the mod side.
- **`DeathState`**: fully resolved, ruled out as a contributor to anything (audio-only effect).
- Not reached this pass: `Jig_SetAttachmentActiveState`, `FindLocalAttachmentByUID`,
  `GetListOfAttachments`, `OnInteractActorOverDistance`'s body (Ubergraph-internal, needs a different
  discovery technique), `SERVER_UpdateAttachmentState`'s full downstream RPC-authority behavior,
  `Client_Died`'s tail past the permadeath-CVar branch, who (if anyone) is bound to
  `OnAttachmentsUpdated`, and the `InpActEvt_*`/other lower-priority `.bin` files already sitting in
  `%APPDATA%\SurrounDeadBridge\` from earlier sessions.
- **Process note for next time**: always re-dump (`bytecode_dump.flag`) immediately before resolving
  CIs/pointers against a live process — reusing an old `.bin`'s embedded values against a different
  process instance silently returns garbage (see the `ExecuteUbergraph_BP_JigPickupComponent` section
  above for the concrete example and why). Also: this log had a real, first-time-this-session incident —
  see the "DATA LOSS NOTICE" earlier in this file — `Write` was used instead of `Edit` on this exact file
  and destroyed most of it, recovered via `git restore` + reconstruction from conversation history. Some
  narrative from earlier sessions (pre-tonight) is permanently gone. No source code was affected.

### Follow-up round (same night, after a several-hour gap while away)

Game crashed and was closed at some point after the above (debug.log's last line before the gap:
16:04:34; process relaunched 20:15:31, confirmed via `Get-Process`). No crash dump was generated for
whatever caused that one either — consistent with this project's own repeated finding that unhandled
C++ exceptions from this mod don't reliably trigger a WER dump the way access violations do. Not
investigated further (not asked to).

Picked back up against the fresh process: **`FindLocalAttachmentByUID`** (679 bytes) confirmed as a
plain linear search — loops `InstanceVariable 0x133482980` (the tracking array) via
`Array_Length`/`Array_Get`, calls `Jig_GetAttachmentInfo` per entry, compares UID via
`EqualEqual_GuidGuid`. Nothing unexpected, closes that item.

**`Jig_SetAttachmentActiveState`/`GetListOfAttachments`**: confirmed these are interface-dispatched
(`EX_InterfaceContext`/`EX_ObjToInterfaceCast` in `ProcessAttachments`, re-resolved fresh this round:
casts to `BP_MpInteractInterface_C` and `BP_WeaponAttachments_C`), not direct methods on
`BP_JigPickupComponent_C` — explains why `GetFunctionByNameInChain` always returned not-found for both.
Tried `FindFirstOf` against `BP_WeaponAttachments_C`, `BP_MpInteractInterface_C`, `BP_762MagPickup_C`,
and `BP_TacticalLaserLightComboPickup_C` (all currently-equipped-per-`send_weapon_attachments`
candidates) — none had a live/CDO instance `FindFirstOf` could find, even though a 762 mag and laser
combo are confirmed equipped right now. Likely means container-held attachment items (as opposed to
top-level equipped items) aren't spawned as separately-`FindFirstOf`-able actors, or the concrete class
name doesn't match the `itemId` string directly. Would need a genuine live pointer (e.g. via
`bytecode_dump.flag`'s `abs <hex>` form, sourced from some other live capture of an actual spawned
attachment actor) rather than a name-based guess. Deprioritized — the core pipeline
(`ProcessAttachments`/`UpdateAttachments`/`OnAttachmentsUpdated`) is already understood well enough to
act on; these two are secondary bookkeeping calls within the "attachment already exists" branch, not
the mechanism itself.

Also confirmed (cheaply, via `pbfg.bin`) that the various old `armpose_*`/`fabrik_*`/`noisefloor_*`/
`pbfg`/`ubergraph` `.bin` files sitting in `%APPDATA%\SurrounDeadBridge\` ARE genuine Kismet bytecode
dumps (not some other format) — but they're from Aug 11-13, days before this session, investigating the
arm/weapon-grip-pose bug that's already fixed per git history (`1f77fb9 Fix weapon grip pose bug`,
`0eb0f88 Fix melee grip pose`). Not pursued further — reopening an already-closed, already-fixed
investigation isn't a good use of time right now.

### `CheckDistanceFromActor` — fully decoded, and it CORRECTS an earlier assumption: this whole mechanism is unrelated to tonight's attachment bug

Re-dumped fresh (492 bytes) and it's a genuine standalone function, not a trampoline — disassembled and
resolved completely, no unknowns left:

1. `IsValid(CurrentActor)` — if invalid, `KismetSystemLibrary::K2_ClearTimer(Self,
   "CheckDistanceFromActor")` and return (stops the repeating 0.1s timer once there's nothing to track).
2. Otherwise: `Delta = Subtract_VectorVector(InteractingActorLoc, CurrentActor.K2_GetActorLocation())`,
   `Distance = VSize(Delta)`, `IsOverDistance = Greater_DoubleDouble(Distance, 300.0)`.
3. If NOT over distance: return — nothing happens, timer just keeps ticking.
4. **If over distance**: `K2_ClearTimer(Self, "CheckDistanceFromActor")`, then
   `CastToInterface<BP_JigCharacterInterface>(CurrentActor)` — casts `CurrentActor` (the *character*
   that was interacting, per this component's own established field meaning) to a CHARACTER interface,
   not an item interface. If the cast succeeds: `Owner = Self.GetOwner()` (`ActorComponent::GetOwner` —
   this component's own owning actor, i.e. the pickup item itself) → calls
   **`CastedCharacter.OnInteractActorOverDistance(Owner, bool)`** — confirmed via fresh `resolve_fname`,
   `ci=1845499` (this process) resolves exactly to `"OnInteractActorOverDistance"`, matching the earlier
   Ubergraph CI-table entry from before the relaunch. Either way (cast succeeds or not), this component's
   own `CurrentActor` gets set to `NoObject` at the end.

**This overturns the working assumption this whole `CheckDistanceFromActor`/`CurrentActor`/
`OnInteractActorOverDistance` thread was built on** (dating from earlier in this session, before the
attachment-pipeline decode above): `OnInteractActorOverDistance` is called ON THE CHARACTER (via an
interface the character implements), not on the item, and nothing in this function's own logic touches
attachment, socket, or render state at all — only its own `CurrentActor` interaction-tracking variable.
`CurrentActor`/`InteractingActorLoc` (comp+0xE0/+0xE8) track "which character is currently
looking-at/interacting-with this pickup, and from where" — almost certainly the loot-prompt lifecycle
(walk away from an item you were about to interact with → the prompt cancels), not anything about an
already-equipped, already-attached item's ongoing physical state. **This whole mechanism is very likely
unrelated to tonight's actual attachment/render-desync bug** — the real mechanism for that is
`ProcessAttachments`/`OnAttachmentsUpdated`, decoded earlier in this session. Worth remembering next
time this component's `CurrentActor` field comes up in an equipped-item context: it's probably not the
right lead, despite the name's surface-level plausibility — this is now settled with real decoded
ground truth, not just a hunch.

### `ValidateAttachedActor` — fully decoded, likely the single most actionable finding tonight

Re-dumped fresh (488 bytes, standalone, not a trampoline). Complete, no unresolved operands:

1. `Owner = Self.GetOwner()` (this component's own owning actor — the item itself), `Parent =
   Owner.GetAttachParentActor()` (`Actor::GetAttachParentActor`) — whatever the item is CURRENTLY
   attached to. `IsValid(Parent)` check.
2. If valid: re-fetches `Owner`/`Parent` (Blueprint-graph redundancy, same calls again), then
   `DynamicCast<Character>(Parent)` — checks specifically whether the attach parent is a `Character`.
3. **If the parent is a Character**: `SocketName = Owner.GetAttachParentSocketName()` (the socket this
   item is CURRENTLY attached to), then reads that socket's live world transform via
   `ParentCharacter.Mesh.GetSocketTransform(SocketName, Space=0)` (same `GetSocketTransform`, `ci=44895`,
   already seen in `ProcessAttachments`), and **directly calls
   `Owner.K2_SetActorTransform(socketTransform, bSweep=false, HitResult, bTeleport=true)`** on the item
   actor itself.

**This is a real, already-existing, game-authored repair mechanism, and it's structurally different
from both approaches tried tonight**: not a `K2_AttachToComponent` re-parent (this project's own
kill-switched attempt), not a `ProcessAttachments()`/`OnAttachmentsUpdated` full re-run (the
theory from earlier tonight) — it directly reads the current correct socket transform and
`K2_SetActorTransform`s the item straight onto it, with `bTeleport=true` forcing an immediate,
non-interpolated snap. `SetActorTransform` is UE's general-purpose "move this actor" API (the same kind
of call used for teleporting any actor in the game), which internally drives the standard
`UpdateComponentToWorld`/render-transform-dirty path every actor move goes through — a plausibly more
reliable way to force a correct render refresh than either alternative already considered, since it
isn't specific to the attachment-reparenting code path where the missing notification was theorized to
live.

**Not yet tested live** — found via static decode only, this session's live-testing budget already used
up on two failed proactive-fix attempts (hang, then crash) earlier tonight. This is the strongest
concrete next experiment for the render-desync bug: call this exact sequence (get attach parent's
socket transform, `K2_SetActorTransform` with `bTeleport=true`) on a detected-drifted item instead of
either previous approach, at a conservative/low frequency given tonight's two live failures. Should be
tried fresh, in isolation, next time this is picked back up.

### `OnRep_CanBePicked?` — confirms the `CheckDistanceFromActor` correction

Re-dumped fresh (310 bytes), fully resolved. `if (Not_PreBool(CanBePicked) && IsValid(CurrentActor))`:
runs the EXACT same sequence as `CheckDistanceFromActor`'s over-distance branch — `K2_ClearTimer(Self,
"CheckDistanceFromActor")`, cast `CurrentActor` to `BP_JigCharacterInterface`,
`.OnInteractActorOverDistance(Self.GetOwner(), bool)`, then `CurrentActor = null`. Makes sense: if an
item becomes un-pickable (someone else grabbed it, etc.) while a character is mid-interaction with it,
the same interaction-prompt-cancel fires immediately instead of waiting for the distance timer. Further
confirms this whole system is interaction-prompt lifecycle management, unrelated to attachment/render
state — no new information for the actual bug, but a clean independent confirmation of the correction
above.

### Finishing the table: the last three blocked functions, unblocked

User asked to finish everything remaining in `bp_catalog_jigpickup_bytecode.md`. Two rows had been stuck
on "interface-dispatched, no live instance found" — unblocked both, for different reasons than assumed:

**`Jig_SetAttachmentActiveState`**: the earlier assumption (interface-dispatched via
`BP_MpInteractInterface_C`/`BP_WeaponAttachments_C`, found through `ProcessAttachments`' cast chain) was
half-right about needing a different class, wrong about needing an interface lookup. Found a live pointer
directly from the running `attach_health` diagnostic's own log output (`attach_health: local>... child
ptr=0x...`, already logging one-level-deep attachment children every throttle cycle) and `resolve_ptr`'d
it to `BP_MilitarySuppressorLocalAttachment_C` — revealing the REAL naming convention for spawned
attachment actors is `BP_<Item>LocalAttachment_C`, not `BP_<Item>Pickup_C` (which is presumably the
world-pickup/ground-item class, a different actor entirely from the equipped-attachment actor).
`GetFunctionByNameInChain` found it immediately once the right class was used. Trampolines into a shared
base class, `BP_AMainLocalAttachment_C::ExecuteUbergraph_...` (129 bytes, tiny, fully decoded): sets an
`IsActive`-style instance var to the incoming bool, calls `OnActiveStateChanged()` (an overridable hook —
e.g. toggling a laser's beam on/off). Purely functional state, not positional — confirmed unrelated to
the render-desync bug.

**`GetListOfAttachments`**: same wrong assumption (interface-dispatched) — the real answer was simpler:
re-read `UpdateAttachments`' own bytecode closely and noticed both `IsValidPickup` and
`GetListOfAttachments` are called through `EX_Context{ object_expr: InstanceVariable 0x1244b4500 }`, a
direct property READ (not a cast, not a function result) — meaning it's a plain object-reference field on
`BP_JigPickupComponent_C` itself, almost certainly the already-known `OwnerMPComp` field
(`BP_JigPickupComponent.hpp`, confirmed present but "never populated by this project's own equip path"
per much earlier session notes — apparently it IS populated by the game's own native equip flow, just
never by this mod's). Tried `BP_JigMultiplayer_C` (a class this session already knew has related
functions like `HandleActorEquipped`) and both `IsValidPickup` and `GetListOfAttachments` were found
there immediately. `IsValidPickup` (95 bytes): validity-checks a struct member of a cached
pickup/UID-reference instance var, returns it. `GetListOfAttachments` (1213 bytes): calls
`GetAllContainerByItemUID(itemUID, &containers)` then loops the result filtering for attachment-type
entries into the output array — pure data-gathering, confirms/extends `ProcessAttachments`'
understanding without changing it.

**Catalog status**: every row is now ✅ except two genuinely-minor 🟡s
(`ExecuteUbergraph_BP_JigPickupComponent` — fully disassembled as raw bytes, just not every one of its
many entry points individually traced; `ProcessAttachments` — every call target confirmed, just the
exact reverse-scan branch wiring not pinned to the byte). The real remaining bottleneck for the
attachment/render-desync bug is no longer decoding — it's actually live-testing the two candidate fixes
(`ValidateAttachedActor`'s transform-snap pattern, or the `OnAttachmentsUpdated` broadcast) that this
session's decode work already found and fully understands.

## PC1 live incident: shotgun + pistol rendering on the ground while still equipped — third reproduction of the render-desync bug, plus a real (if unrelated) EntityManager risk found and fixed

**The report**: PC1's `BenelliM4Pickup` (shotgun, slot 12) and `BattleReadyGlockPickup` (pistol, slot 13)
appeared physically resting on the ground/against a couch in a screenshot, in natural settled poses (not a
freeze-frame — genuinely different, sensible resting positions relative to nearby furniture). User
confirmed live: **both items were still pickup-able and the game still treated them as the player's own**
("I can pick them up, they are still attached technically. but the model isnt") — i.e. game-thread/data
state was correct, only the rendered position was wrong. This is the third reproduction this session of
the exact bug class already theorized in `do_attach_health_scan`'s own 2026-08-16 comment (mag/laser/
shotgun floating on PC1 earlier, then PC2's helmet), now with the clearest evidence yet.

**What the existing diagnostics showed, live**:
- `attach_health`'s socket-dist check read clean (0.0–3.0 units, `offSocket=0`) for both items both before
  and after the screenshot — confirms (again) that this check's own `K2_GetComponentLocation` read agrees
  with the socket's expected transform even while the rendered pixels disagree. The transform-snap fix
  deployed earlier tonight can never fire for this bug class, because its gate never trips.
- `resolve_ptr` on the live "local child" pointer list (via `resolve_ptr.flag`) confirmed the attached
  actors genuinely are `BP_BenelliM4Pickup_C`/`BP_BattleReadyGlockPickup_C` instances — the *same* Pickup
  Blueprint class used for a real ground-dropped item, just currently `AttachParent`'d to a socket instead
  of sitting free. This class-sharing turned out to matter (see below).
- The background monitor caught a genuine, real **DETACHED** hit for the shotgun at 21:42:04.559
  (`do_attach_health_scan`'s prev/current `AttachChildren` diff, not just the distance check) — it briefly,
  actually left `AttachChildren` and came back within ~3 seconds (`localMeshChildren` count never dropped
  below 16 across the whole window). `read_local_weapon_attachments`' slot mapping (`[12]=BenelliM4`,
  `[13]=BattleReadyGlock`) never lost either item at any point through this — confirms nothing was lost
  from the mod's own equip bookkeeping, only a rendering artifact plus one brief real detach/reattach
  cycle. `send_weapon_attachments` (the frame actually sent to the server) likewise never dropped either
  slot's accessory data throughout.
- The DETACHED handler's own `dump_recent_calls()` — meant to capture the exact ProcessEvent call that
  caused it — produced **nothing usable**: `record_recent_call`'s watch-list (`s_watchObjs[8]`, populated
  by `set_recent_calls_watch`) only ever contained the *clothing* investigation's five objects
  (pawn/torso/legs/feet/helper), never weapon/item actors. The forensic capture existed but was scoped to
  a different bug. This is why the causal chain for tonight's specific detach is still not pinned down —
  fixed for next time, see below.

**Entity-manager "claimed native pickup" theory, investigated and ruled out for this specific incident**:
`spawn_entity_actor` (`entity_manager.cpp:518`) logged `claimed native pickup itemId=AK15` in the same
window. `find_and_claim_native_pickup` (`entity_manager.cpp:242`) resolves a self-dropped `WorldEntity` to
its native pickup actor by **class name + nearest position within 500 units**, with no check that the
candidate isn't currently attached to something. Since equipped items are live instances of the exact same
Pickup class as a genuine ground drop, and "within 500 units of the drop" is trivially true for anything
attached to the player, this function could in principle silently claim the player's own currently-worn
item as a "dropped" `WorldEntity` — and a later legitimate `EntityDespawn` for that (bogus) entity would
call `K2_DestroyActor()` directly on the player's real equipped weapon (`destroy_entity_actor`,
`entity_manager.cpp:627`). Checked both logs for any `EntityState`/`EntityDescriptor`/claim traffic
mentioning Benelli/Glock specifically: **none** — this mechanism did not cause tonight's incident. It's a
real, separate latent risk regardless, so fixed anyway (cheap, read-only, no behavior change for the
real-drop case): `find_and_claim_native_pickup` now skips any candidate whose `RootComponent->AttachParent`
(`AActor+0x1A0` → `+0xB0`, both offsets already proven elsewhere in this codebase, e.g. `mod.cpp`'s
`equip_restore_retry` mismatch check) is non-null — a live-attached actor can never legitimately be an
unowned ground pickup. Built clean, not yet deployed (see below).

**Diagnostics extended (safe, observation-only, no gameplay behavior change)**: widened the recent-calls
ring buffer's watch-list mechanism (`mod.cpp`, near `dump_recent_calls`) with a second, independently-
refreshed region — `s_watchObjs[21]`, `[0,5)` still the clothing watch unchanged, `[5,21)` now the local
player's current top-level item children, refreshed every `check_attach_health("local", ...)` poll via the
new `set_item_recent_calls_watch()` (mirrors the exact children list that scan already computes for its own
detach-diffing — no extra scanning cost). Next time a weapon/item DETACHED hit fires, `dump_recent_calls()`
should actually show the real ProcessEvent call chain around it instead of nothing, closing the gap found
above. This does not change any gameplay behavior — pure logging capture, same risk class as every other
diagnostic addition tonight, not the "make a repair fire unconditionally" risk class that crashed/hung
twice earlier.

**Status**: both changes built clean and deployed to PC1 and PC2 (both game processes had closed on their
own by the time the deploy was attempted — not force-closed — so no lock issue in the end). Root cause of
the actual render-stuck-after-reattach mechanism itself is still open; the leading theory remains the
`MarkRenderTransformDirty`-notification-gap one already recorded in `do_attach_health_scan`'s comment, and
actually pinning it down needs either (a) the widened forensic capture above catching a real ProcessEvent
chain on the next natural reproduction, or (b) the WndProc-based clean GameThread trigger — see the next
entry, which found that mechanism is not just planned but already fully built, deployed, and running.

## The WndProc clean-trigger plan (standing plan, "Reliable GameThread Trigger via WndProc Subclass") is already implemented and empirically holding up — this changes the risk calculus for the proactive-repair kill-switches

While following up on the render-desync investigation above, checked whether the standing plan (background
thread + `PostMessage`/`WM_APP` + subclassed `WndProc`, meant to give this mod a way to run risky
ProcessEvent-heavy repair work from a point in the frame that's provably NOT nested inside another
`ProcessEvent` dispatch) was still just a plan or had been started. **It's fully implemented** —
`ensure_hwnd_ticker_started()`, `sdb_wnd_proc`, `kCleanTickMessage` (`mod.cpp` ~6675-6762) — and wired all
the way through: `do_game_tick(bool cleanContext)` takes the flag, `check_attach_health_trigger`'s actual
scan/repair invocation (`mod.cpp:3838`, `if (cleanContext && !targets.empty() && ...)`) and
`ProxyManager::tick()`'s own gate are both conditioned on it, and `check_component_drift` (which contains
`do_body_part_repair`, home of the `kEnableProactiveLeaderPoseRefresh` kill-switch) is called from
*inside* that same `cleanContext`-gated block (`mod.cpp:3843`, right next to `check_attach_health`) — so
anything reachable from either of tonight's two kill-switched proactive-refresh attempts already runs
inside the new clean-trigger path whenever it fires, not the old nested `on_process_event_pre` fallback.

**The code marks this "NOT YET LIVE-VERIFIED," but the log says otherwise.** `t_processEventDepth` (a
`thread_local` nesting counter, incremented/decremented around every `ProcessEvent` call) is checked every
single time `cleanContext` is true, logging `"cleanContext=true, t_processEventDepth=0 confirmed"` when the
assumption holds and a (never-yet-seen) warning line if it doesn't. Checked the full session's
`debug.log` (`%APPDATA%\SurrounDeadBridge\debug.log`, one continuous file spanning **14:54:37 through
21:56:26 today — about 7 hours, 45 process relaunches**, covering everything from this whole marathon
session's death/respawn cycles to the shotgun/pistol incident investigated above):
- **1852 occurrences of `cleanContext=true, t_processEventDepth=0 confirmed`. Zero occurrences of the
  violation-warning line.** The core assumption behind the entire plan — that UE5's Windows message pump
  dispatches from a point outside any `ProcessEvent` chain, so a `WM_APP`-triggered `WndProc` handler is a
  genuinely clean entry point — has held 100% of the time it's been checked, across a real multi-hour
  session with heavy, varied gameplay activity, not just a short synthetic test.
- Zero occurrences of `0xe06d7363`/`EXCEPTION_ACCESS_VIOLATION`/any hang-adjacent signal anywhere in
  today's log. (The two known failures from *this* mechanism specifically — the leader-pose-refresh hang
  and the transform-snap crash — both happened earlier in this marathon session, most likely before
  whatever point today's `debug.log` starts from or before the SEH-wrap noted at `mod.cpp:6683` was added;
  can't fully reconstruct exact timing against a single day's file, but the practical takeaway is the same
  either way — no such failure in the entire window this file actually covers.)

**Why this matters for the two currently-OFF kill-switches** (`kEnableProactiveLeaderPoseRefresh`,
`mod.cpp:3616`, and the transform-snap block's own unconditional variant, both reverted after a hang and a
crash respectively — see this file's own history above): both failures were originally attributed to firing
ProcessEvent-heavy repair calls from an unverified, possibly-nested execution context.

**Correction, same night, after re-reading the crash's own comment more carefully**: the 0xe06d7363 crash's
stack already ran through the WndProc path when it happened — `cleanContext` was already true at the time,
not the unverified nested path. So the 1852/1852 depth=0 evidence, while real, does NOT by itself prove this
specific crash can't recur — it confirms the nesting theory is false for whatever *did* cause it, nothing
more; the actual root cause of that crash is still unidentified. The more specific new protection is that
`do_game_tick_clean_ctx` (the WndProc handler's own entry point) was given an SEH wrap after that crash —
`mod.cpp:6683`'s own comment confirms this — matching the catch-and-log pattern this codebase has used
successfully many times elsewhere tonight. That means a repeat of the same unhandled-C++-exception failure
mode should now be caught and logged rather than crash the process — a reasonable inference from a proven
pattern, not a tested guarantee for this exact call site.

**Re-enabled leader-pose refresh only, transform-snap unconditional variant left off.** User explicitly
authorized proceeding despite being unavailable to watch a live test (asked twice, in fact, including after
this correction was already in progress), on the grounds that a repeat failure just costs a relaunch — no
data loss, since progress persists to SQLite independently of the live process. Built clean, deployed to
PC1 and PC2 while both were closed (`kEnableProactiveLeaderPoseRefresh = true`, `mod.cpp`, still the 20s
throttle). The transform-snap unconditional variant was never part of this decision and stays off — no
comparable "here's the new protection" argument was made for it tonight. **First thing next session: check
`debug.log` for a hang signature or an SEH-caught-exception line before assuming this went fine** — the log
is the actual verdict, this write-up is just the reasoning that led here.
