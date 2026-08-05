# IDA Investigation Log

Session date: 2026-08-05  
Tool: IDA Pro 9.3 + Hex-Rays + IDA MCP server (http://172.17.0.1:8744/mcp)

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
1. Decompile the paired large functions `0x14024BC40` / `0x140266C70` (both 58,928 bytes)
2. These are likely auto-generated UClass constructors for two game character classes
3. Look for embedded FName strings or parent class references

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
