# Non-Gameplay Asset Catalog

Covers folders spot-checked and skipped from the main gameplay catalogs. Re-verified via `grep -rl '"Type": "Function"' <folder>` against the full JSON export.

## Audio (538 files)

538 files, almost entirely SoundWave/SoundCue/SoundClass/SoundMix/SoundAttenuation assets — no gameplay logic. One Blueprint with functions:

### BP_AmbientSoundController.json
Ambient audio state controller, presumably reacting to Ultra Dynamic Sky (UDS) / Ultra Dynamic Weather (UDW) plugin dispatchers to drive weather/time-of-day ambience.

Functions: Bind to Dispatchers, Check if UDS and UDW Are Both Gone, ExecuteUbergraph_BP_AmbientSoundController, Finished Dust, Finished Raining, Finished Snowing, Get Starting Dispatchers State, Get UDS and UDW Reference, ReceiveBeginPlay, Start Up Sound, Started Dust, Started Raining, Started Snowing, State Change Fog, Sunrise, Sunset, UDS End Play, UDS Ending Play, UDS Starting Up, UDW End Play, UDW Ending Play, UDW Starting Up, Update Volume Multiplier, UserConstructionScript.

No Net-flagged properties (single-player/local ambience actor, no replication).

## Meshes (897 files)

897 files, entirely SkeletalMesh/StaticMesh/PhysicsAsset/Skeleton assets for vehicles and characters, no standalone Blueprint classes. One AnimBlueprint with functions:

### Vehicles/TestChargerVehicle.json
Test/placeholder vehicle AnimBlueprint. Functions: AnimGraph, ExecuteUbergraph_TestChargerVehicle (standard anim-graph plumbing only, no custom gameplay functions). No Net-flagged properties.

## ButtonIcons (55 files)

55 files, confirmed zero Function entries — all Texture2D icon assets for UI buttons/prompts.

## PolygonFiles (2251 files)

2251 files, confirmed zero Function entries — StaticMesh/Material/Texture assets (Synty-style "Polygon" asset pack props/environment pieces).

## EditorOnly (2 files)

2 files. One has functions:

### DEBUG_Marker.json
Editor-only debug marker actor. Single function: UserConstructionScript (placement/visualization logic only). No Net-flagged properties.

### DEBUG_Marker_QuestLoc.json
Zero Function entries — static/data-only variant, presumably a marker subtype used to tag quest locations in-editor.

## Summary

Total files covered: 3743 (538 Audio + 897 Meshes + 55 ButtonIcons + 2251 PolygonFiles + 2 EditorOnly).
Files with real Blueprint logic: 3 (BP_AmbientSoundController, TestChargerVehicle, DEBUG_Marker) — all documented above.
All confirmed asset-only otherwise: no Net-flagged/replicated properties found anywhere in this scope, consistent with these folders being cosmetic/editor-only and irrelevant to multiplayer sync work.
