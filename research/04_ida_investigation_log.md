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

