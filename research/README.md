# research/

Reverse-engineering notes and generated artifacts for SurrounDead. This is the substance of the
project — the mod source is mostly a consequence of what is written down here.

The directory is split by **engine version**, because the game moved from UE 5.3 to UE 5.6.1
(`++UE5+Release-5.6-CL-44394996`) and that invalidated every hardcoded address and struct offset
derived from the old build.

## Current (UE 5.6.1)

| Path | What it is | How to regenerate |
|---|---|---|
| `04_ida_investigation_log.md` | The ongoing investigation log, ~725 KB, append-only. Cited by name from roughly 30 source comments. Start here. | By hand, as work happens. |
| `CXXHeaderDump/` | 2,619 generated headers — every reflected class, its properties and their offsets. The source of truth for name-based lookups. | UE4SS `Keybinds` mod, **Ctrl+H** (`GenerateSDK()`), in-game. |
| `Mappings.usmap` | Type mappings FModel needs to read the paks. Gitignored (2.7 MB, regenerable). | UE4SS `Keybinds` mod, **Ctrl+Num6** (`DumpUSMAP()`), in-game. |
| `Exports/` | FModel `.json` property dumps of the game's assets. Gitignored (619 MB, regenerable). | FModel: right-click a folder, **Export Folder Packages (.json)**. Double-clicking only previews — it does not write to disk. |
| `bytecode/` | Kismet bytecode disassembler and batch resolver, plus captured dumps. The tooling is version-independent; the dumps are not. | `kismet_disasm.py` / `batch_resolve_local.py` — see `project_sdo_bytecode_decode_pipeline`. |
| `audit_todo.md` | Running list of confirmed issues in the mod's own C++. Not version-specific. | By hand. |

## Archived (UE 5.3)

`archive-ue5.3/` — 35 files of pre-port material: binary overviews, the hardcoded-address table,
Blueprint catalogs, the 5.3 asset registry and pak listing, and decoded bytecode. Kept for the
reasoning, not the numbers. See its own README before trusting anything in it.

## Regeneration status

`Exports/` was re-exported from the 5.6 pak on 2026-09-24: 4,753 of 14,097 `.json` files are
current, the rest are 5.3 leftovers. **`Exports/SurrounDead/Content/Levels/LongdownValley.json` is
not among the updated ones** — FModel fails to serialize `LongdownValley.umap` on 5.6, dying at
byte 28. Until that is solved, `server/scripts/extract-zombie-data.js` cannot regenerate
`server/src/data/world-data.json`, and zombie/vehicle spawn placement stays on 5.3 coordinates.
