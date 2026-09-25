# research/

Reverse-engineering notes and generated artifacts for SurrounDead. This is the substance of the
project - the mod source is mostly a consequence of what is written down here.

The directory is split by **engine version**, because the game moved from UE 5.3 to UE 5.6.1
(`++UE5+Release-5.6-CL-44394996`) and that invalidated every hardcoded address and struct offset
derived from the old build.

## Current (UE 5.6.1)

| Path | What it is | How to regenerate |
|---|---|---|
| `04_ida_investigation_log.md` | The ongoing investigation log, ~725 KB, append-only. Cited by name from roughly 30 source comments. Start here. | By hand, as work happens. |
| `CXXHeaderDump/` | 2,619 generated headers - every reflected class, its properties and their offsets. The source of truth for name-based lookups. | UE4SS `Keybinds` mod, **Ctrl+H** (`GenerateSDK()`), in-game. |
| `Mappings.usmap` | Type mappings FModel needs to read the paks. Gitignored (2.7 MB, regenerable). | UE4SS `Keybinds` mod, **Ctrl+Num6** (`DumpUSMAP()`), in-game. |
| `Exports/` | FModel `.json` property dumps of the game's assets. Gitignored (619 MB, regenerable). | FModel: right-click a folder, **Export Folder Packages (.json)**. Double-clicking only previews - it does not write to disk. |
| `bytecode/` | Kismet bytecode disassembler and batch resolver, plus captured dumps. The tooling is version-independent; the dumps are not. | `kismet_disasm.py` / `batch_resolve_local.py` - see `project_sdo_bytecode_decode_pipeline`. |
| `audit_todo.md` | Running list of confirmed issues in the mod's own C++. Not version-specific. | By hand. |

## Archived (UE 5.3)

`archive-ue5.3/` - 35 files of pre-port material: binary overviews, the hardcoded-address table,
Blueprint catalogs, the 5.3 asset registry and pak listing, and decoded bytecode. Kept for the
reasoning, not the numbers. See its own README before trusting anything in it.

## Regeneration status

`Exports/` was re-exported from the .8 pak on 2026-09-24, including the level, and
`server/src/data/world-data.json` was regenerated from it: 982 spawn zones, up from 913.

The level is extracted with `tools/asset-export` rather than FModel. FModel's engine version is a
remembered per-game setting, and extracting a UE 5.6 pak while it still says 5.3 fails at byte 28
with `VersionException: Read size is smaller than zero` - which looks like a corrupt asset rather
than a misconfiguration. The CLI takes the version as code instead. See that tool's README.
