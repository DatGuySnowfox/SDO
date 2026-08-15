# Sky, Weather, Terrain, Levels & Sequences Blueprint Catalog (FModel export analysis)

Source: `Exports/SurrounDead/Content/UltraDynamicSky/**`, `Terrain/**`, `Levels/**`, `Sequences/**`.
Export is flags-only (no bytecode) — function bodies are unknown, only name/flags/params exist.

## Top-line findings

- **UltraDynamicSky is the marketplace "Ultra Dynamic Sky/Weather" plugin, largely unmodified.** Of 470 files under `UltraDynamicSky/`, almost all are weather-preset data assets, particle systems, materials and MetaSounds with zero own logic. Real logic lives in a handful of core Blueprints: `Ultra_Dynamic_Sky_C`, `Ultra_Dynamic_Weather_C`, `Weather_Override_Volume_C`, `Random_Weather_Variation_C`, plus small manager components (`UDW_Material_State_Manager_C`, `UDW_Temperature_Manager_C`, `UDW_Lightning_Spawn_Manager_C`, `UDS_PlayerOcclusion_C`).
- **Weather already has server-authoritative RPCs built in by the plugin**: `Ultra_Dynamic_Weather_C` has `Change Weather` and `Change to Random Weather Variation` as `FUNC_Net|NetReliable|NetServer`, plus `Clients Transition Start`, `Flash Lightning`, `Global Lightning Internal` as `FUNC_Net|NetMulticast`. `Weather_Override_Volume_C` (per-region weather triggers) similarly has `Change Weather`/`Change to Random Weather Variation` (NetServer) and `Apply Current Single Weather`/`Enable Volume`/`Disable Volume`/`Start Up Random Weather Component`/`Reset Timer` (NetMulticast). This means weather transitions are already replication-aware out of the box — SDO doesn't need to build weather sync from scratch, only verify it fires correctly under the mod's server-authoritative model.
- **Time-of-day has NO net-flagged function or property.** See flagged section below — this is the one feature-relevant gap.
- **Level Blueprints are essentially empty** — every map's Level BP (`LongdownValley_C`, `MainMenu_C`, `PersistentLevel_C`, `ShootingRange_C`, `TutorialWorld_C`, `VehiclePlayground_C`) only has `ExecuteUbergraph_*` + `ReceiveBeginPlay` (2 funcs each), except `MainMenu_C` (menu UI events) and `PersistentLevel_C` (load-screen/level-streaming events). No gameplay trigger logic, no RPCs. `Levels/*.json` are actually full per-map actor dumps (tens of thousands of placed instances), not just the Level BP graph.
- **Terrain/** and **Sequences/** have no meaningful logic: two small water Blueprints (`BP_WaterBox`, `BP_WaterSpline`) with plain overlap events, no net flags; the one level sequence has no director BP functions.
- Offset-enrichment note: matching `.hpp` files exist for every core class in this file (`Ultra_Dynamic_Sky.hpp` 762 fields, `Ultra_Dynamic_Weather.hpp` 548 fields, `Weather_Override_Volume.hpp`, `Random_Weather_Variation.hpp`, `BP_WaterBox.hpp`, `BP_WaterSpline.hpp`), but this catalog's prose only ever names *functions*, never individual properties, for the sky/weather classes (they're huge marketplace-plugin structs with no properties singled out above) — so there was nothing named in-line to attach an offset to except `BP_WaterBox`'s fields, added below. If a future pass needs specific Ultra Dynamic Sky/Weather property offsets (e.g. for a custom time-of-day sync RPC), the full byte layout is already sitting in those two `.hpp` files, just not transcribed into prose here.

---

## UltraDynamicSky — Time-of-Day (flagged for future feature)

**No day/night sync feature currently exists in the mod, and the plugin itself provides no ready-made replication for time.**

`Ultra_Dynamic_Sky_C` (`UltraDynamicSky/Blueprints/Ultra_Dynamic_Sky.json`) is the single central time-of-day Blueprint (attached once per level). Relevant API, all plain `FUNC_Public|BlueprintCallable` (no `FUNC_Net*`):
| Function | Flags |
|---|---|
| Get TimeCode | Public, HasOutParms, BlueprintCallable, BlueprintEvent, BlueprintPure |
| Set Time with Time Code | Public, BlueprintCallable, BlueprintEvent |
| Set Time Cycle Degrees | Protected, BlueprintCallable, BlueprintEvent |
| Time Of Day to H/M/S | Public, HasOutParms, BlueprintCallable, BlueprintEvent, BlueprintPure |
| Time of Day Offset | Protected, HasOutParms, BlueprintCallable, BlueprintEvent, BlueprintPure |
| Time of Day Animation | Protected, BlueprintCallable, BlueprintEvent |

No BP-declared `Replicated` scalar property for current time is visible in the export (this export format doesn't dump BP variable declarations, only components/functions — consistent with the AI/Vehicle catalog's caveat). `Get TimeCode`/`Set Time with Time Code` is the clean read/write pair to build a custom sync feature on: server calls `Set Time with Time Code` and a wrapping `NetMulticast` (or periodic `OnRep`) RPC would need to be added by the mod itself — the plugin does not do this automatically. Only one Net-flagged function exists anywhere in `Ultra_Dynamic_Sky_C`: `Replicate Modifier State` (`FUNC_Net|NetReliable|NetMulticast`) — this is for weather *modifiers* (temporary color/intensity overrides), not raw time-of-day.

## UltraDynamicSky — Weather (`UltraDynamicSky/Blueprints/`)

### Ultra_Dynamic_Weather_C (`Ultra_Dynamic_Weather.json`)
Central weather controller (per-level, pairs with Ultra_Dynamic_Sky). 298 functions total; only these are net-flagged:
| Function | Flags |
|---|---|
| Change Weather | FUNC_Net, NetReliable, NetServer |
| Change to Random Weather Variation | FUNC_Net, NetReliable, NetServer |
| Restart Random Weather Variation | FUNC_Net, NetReliable, NetServer |
| Clients Transition Start | FUNC_Net, NetReliable, NetMulticast |
| Flash Lightning | FUNC_Net, NetMulticast |
| Global Lightning Internal | FUNC_Net, NetMulticast |

Other relevant (non-net) functions: `Get Current Temperature`, `Get Season`/`Set Season`, `Get Display Name for Current Weather`, `Get UDW State for Saving`, `Apply Climate Preset Object`. Owns sub-objects `UDW_Material_State_Manager_C`, `UDW_Temperature_Manager_C`, `UDW_Lightning_Spawn_Manager_C` (component classes, not separately net-flagged in the sampled file).

### Weather_Override_Volume_C (`Weather_Effects/Weather_Override_Volume.json`)
Per-region trigger volume that forces local weather when a player enters it.
| Function | Flags |
|---|---|
| Change Weather | FUNC_Net, NetServer |
| Change to Random Weather Variation | FUNC_Net, NetServer |
| Apply Current Single Weather | FUNC_Net, NetMulticast |
| Enable Volume | FUNC_Net, NetMulticast |
| Disable Volume | FUNC_Net, NetMulticast |
| Start Up Random Weather Component | FUNC_Net, NetMulticast |
| Reset Timer | FUNC_Net, NetMulticast |

### Random_Weather_Variation_C (`Weather_Effects/System/Random_Weather_Variation.json`)
Component driving randomized weather cycling (owned by `Ultra_Dynamic_Weather_C` and `Weather_Override_Volume_C`). 28 functions; only one is net-flagged: `Reset Timer` (`FUNC_Net|NetMulticast|BlueprintCallable|BlueprintEvent`). Everything else (`Apply State`, `Bind to UDS`, `Change to Next Random Weather Type`, `Check For Season Instant Refresh`, `Create Composite Probability Map`, `Select Random Weather Type`, `Recalculate Forecast`, etc.) is plain BlueprintCallable, no net flags — the actual forecast/probability math runs unreplicated per-instance, presumably deterministic or server-driven-then-pushed via the owning `Change Weather` RPCs above.

### Other UltraDynamicSky classes checked, no meaningful net logic found
`UDW_Material_State_Manager_C`, `UDW_Temperature_Manager_C`, `UDS_PlayerOcclusion_C` — plain BlueprintCallable helper functions only, no `FUNC_Net*`.

---

## Terrain (`Terrain/**`, 157 files)

No terrain-specific Blueprint logic found — this folder is materials/textures/foliage/landscape data assets. Only two Blueprints with any functions at all, both trivial and both under `Terrain/Water/`:

### BP_WaterBox_C / BP_WaterSpline_C (`Terrain/Water/BP_WaterBox.json`, `BP_WaterSpline.json`)
6 functions each: `ReceiveBeginPlay`, `UserConstructionScript`, `SetCollisionType`, `ExecuteUbergraph_*`, and two `BndEvt__*ComponentBeginOverlapSignature/ComponentEndOverlapSignature` delegate bindings. All plain `BlueprintCallable`/`BlueprintEvent`, no net flags — simple client-visual overlap-driven collision-type toggling (e.g. swim vs. wade detection), not gameplay-authoritative. Real offsets confirmed via `CXXHeaderDump/BP_WaterBox.hpp` (`ABP_WaterBox_C : public ABP_MasterObject_C`, size 0x2E1): `ShallowWater? (0x02B0, size 0x1)`, `Radiated? (0x02E0, size 0x1)`, `Cube (0x02A8, size 0x8)`, `Material (0x02B8, size 0x8)`, `Scale X (0x02C8, size 0x8)`, `Scale Y (0x02D0, size 0x8)`, `Scale Z (0x02D8, size 0x8)`.

## Levels (`Levels/*.json`, 6 files)

These exports are full per-map actor dumps (every placed instance of every Blueprint in the map, tens of thousands of entries for `LongdownValley.json`), not just Level Blueprint graphs. The actual Level Blueprint class per map has almost no logic:

| Map | Level BP functions | Notes |
|---|---|---|
| LongdownValley_C | ExecuteUbergraph, ReceiveBeginPlay | main game map, no LBP gameplay logic |
| ShootingRange_C | ExecuteUbergraph, ReceiveBeginPlay | same |
| TutorialWorld_C | ExecuteUbergraph, ReceiveBeginPlay | same |
| VehiclePlayground_C | ExecuteUbergraph, ReceiveBeginPlay | same |
| MainMenu_C | ExecuteUbergraph, ReceiveBeginPlay, CreateMenu, Event_DOFToggle, Event_KeyDown | menu UI only, no net flags |
| PersistentLevel_C | ExecuteUbergraph, ReceiveBeginPlay, Event_FadeFinished, Event_LoadLevel, Event_RemoveLoadingScreen, GetLevelName__DelegateSignature, RemoveLoadingScreen__DelegateSignature | loading-screen/level-streaming orchestration only, no net flags |

No Net/Replicated functions or properties found in any Level Blueprint. All real per-map gameplay logic (spawners, POI managers, triggers) lives in the individually placed actor Blueprints already covered by other catalogs (`AI/`, `Blueprints/`), not the Level BP itself.

## Sequences (`Sequences/*.json`, 1 file)

`MainMenuLevelSequence.json` — a `LevelSequence` + `MainMenuLevelSequence_DirectorBP_C` used for the main menu camera fly-through. No functions in the director Blueprint (0 `Function`-type entries) — pure cinematic track data (transform/camera-cut/visibility tracks), no gameplay logic.
