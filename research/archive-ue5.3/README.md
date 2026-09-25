# Archive - Unreal Engine 5.3 era

Everything in this directory was produced against the **UE 5.3** build of SurrounDead, before the
game shipped its 5.6.1 (`++UE5+Release-5.6-CL-44394996`) update. It is kept because the *reasoning*
is still the best record of how these systems were worked out, but the concrete numbers in it - 
addresses, struct offsets, pak contents, asset lists - describe a binary that no longer exists.

**Treat every offset, address and asset path in here as historical.** The current material lives one
directory up:

| Superseded by | |
|---|---|
| `../CXXHeaderDump/` | Regenerated per-version via UE4SS `GenerateSDK()` (Ctrl+H). The source of truth for property names and layout. |
| `../Mappings.usmap` | Regenerated per-version via UE4SS `DumpUSMAP()` (Ctrl+Num6). Required by FModel to read the paks at all. |
| `../Exports/` | Regenerated per-version from FModel. |
| `../04_ida_investigation_log.md` | The ongoing log; still current, not archived. |

## Contents

### Binary / engine analysis
- `01_binary_overview.md` - first pass over the 5.3 shipping executable.
- `02_ue5_key_addresses.md` - hardcoded RVAs for `ProcessEvent`, `GObjects`, `FName::ToString` and
  friends. **Entirely invalid on 5.6**: the exe went 122 MB to 165 MB and every address moved. These
  are exactly the sort of constants the port replaced with name-based reflection.
- `03_modding_framework_plan.md` - the original plan for standing the mod up on UE4SS.
- `05_lua_script_analysis.md` - analysis of the older Lua-based mod's scripts.
- `06_protocol_gap_analysis.md` - where the game's own replication stops and the bridge has to
  take over. Largely conceptual, so it aged better than the rest, but it was written against 5.3.

### Asset / Blueprint catalogs
Generated from the 5.3 FModel exports; names/flags/properties only, no implementation.

- `bp_catalog_player_core.md`, `bp_catalog_inventory_jigsaw.md`, `bp_catalog_building.md`,
  `bp_catalog_ai_vehicles.md`, `bp_catalog_smartai.md`, `bp_catalog_playermodel.md`,
  `bp_catalog_misc_systems.md`, `bp_catalog_sky_terrain_levels.md`, `bp_catalog_ui.md`,
  `bp_catalog_nongameplay_assets.md`, `bp_catalog_anim_input_wip.md`,
  `bp_catalog_jigpickup_bytecode.md`
- `bp_*_props.txt`, `bp_final_sweep.txt` - raw property dumps the catalogs were built from.
- `AssetRegistry.json` (16 MB) and `pak_all_files.txt` (2.2 MB) - the 5.3 pak's full asset
  registry and file listing.
- `ground_item_research.txt` - ground-item spawning notes.

### Bytecode
- `bytecode_decode_status.md`, `bytecode_decoded_batch1.md`, `bytecode_decoded_batch2_animbp.md`,
  `bytecode_audit_modcpp.md`, `bytecode_audit_proxymanager.md`

The decoded output is version-specific, but the **pipeline that produced it is not** and stays live
at `../bytecode/` (`kismet_disasm.py`, `batch_resolve_local.py`). Re-running it against 5.6 is how
these documents get replaced rather than repaired.

## Note on cross-references

`../04_ida_investigation_log.md` and `../audit_todo.md` refer to these files by bare filename in
prose (roughly 45 places). Those references were deliberately left alone - the log is an append-only
record of what was true at the time. If a filename in the log has no match at `research/`, look here.
