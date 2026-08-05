# SurrounDead Binary Overview

## Binary Info
| Field | Value |
|-------|-------|
| File | `SurrounDead-Win64-Shipping.exe` |
| Engine | Unreal Engine 5.3 |
| Image base | `0x140000000` |
| IDB | `SurrounDead-Win64-Shipping.exe.i64` |
| Symbols | None (full strip, all functions are `sub_XXXXXXXX`) |
| Hex-Rays | Available |
| Link type | Fully statically linked (single EXE, no UE DLLs) |
| String count | ~94,226 |

## Segments
- `.text` — code, functions starting at `0x140001000`
- `.rdata` — read-only data (strings, vtables, UFunction registration structs)
- `.data` — global mutable state
- Main data range: `0x140000000 – 0x148000000` (valid pointer check)

## Build Path Evidence
From embedded strings:
```
Z:\SurrounDead 5.3\Plugins\FSR3\...
D:\build\++UE5\Sync\Engine\Source\...
```
Confirms UE 5.3, AMD FSR3 plugin present.

## Dependencies (Import Table)
| Module | What it provides |
|--------|-----------------|
| `WS2_32.dll` | TCP/UDP sockets (WSAPoll, WSACreateEvent, send/recv etc.) |
| `KERNEL32.dll` | Thread, file, memory, sync primitives |
| `XINPUT1_3.dll` | Gamepad input |
| `DSOUND.dll` | DirectSound audio |
| `dxgi.dll` | DXGI factory (D3D12 renderer) |
| `WINMM.dll` | Timer resolution |

All UE5 engine code is statically compiled in — no UnrealGame DLLs.

## Online Subsystem
```
/Script/OnlineSubsystemSteam   ← primary online backend
/Script/OnlineSubsystem
/Script/OnlineSubsystemUtils
```
Steam is the transport. The game uses native WS2_32 sockets alongside Steam networking (for the custom online bridge).

## Game Module
```
/Script/SurrounDead  →  0x145e83190
```
Module registration struct: `0x145e83150`
Module init function: `0x1436f89e0`

All game-specific class logic is in blueprint pak files. Very few game-specific strings exist in the binary itself.
