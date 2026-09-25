# asset-export

Headless asset extractor - a scriptable replacement for FModel's
**Export Folder Packages (.json)**, built on the same underlying library
([CUE4Parse](https://www.nuget.org/packages/CUE4Parse)).

It exists because regenerating `server/src/data/world-data.json` after a game
update used to mean clicking through a GUI, and the one asset that matters most
(`LongdownValley.umap`, ~200 MB of JSON) is easy to get wrong. Two failure modes
in particular are silent:

- **Double-clicking an asset in FModel only previews it.** It loads into a tab
  and writes nothing to disk. Only right-click -> *Export Folder Packages* saves.
- **FModel's engine version is a per-game setting that does not follow the game.**
  Extracting a UE 5.6 pak while the profile still says 5.3 fails with
  `VersionException: Read size is smaller than zero` at byte 28 - which reads
  like a corrupt asset rather than a misconfiguration.

This tool takes the version as code, not as a remembered setting.

## Usage

```sh
dotnet run -c Release -- <paksDir> <usmap> <outDir> <assetPath>...
```

Regenerating the level and the zombie archetypes:

```sh
cd tools/asset-export
dotnet run -c Release -- \
  "C:\Program Files (x86)\Steam\steamapps\common\SurrounDead\SurrounDead\Content\Paks" \
  "..\..\research\Mappings.usmap" \
  "..\..\research\Exports" \
  "SurrounDead/Content/Levels/LongdownValley.umap"

cd ../..
node server/scripts/extract-zombie-data.js
```

Output mirrors FModel's layout - `<outDir>/<asset/path>.json` - so
`extract-zombie-data.js` reads either interchangeably.

## Keeping it working across game updates

The engine version is hardcoded as `EGame.GAME_UE5_6` in `Program.cs`. When the
game's engine version changes, update that constant. Confirm the new version
from the game's own executable or from UE4SS's log, not by guessing - a wrong
value produces the byte-28 failure above rather than a clear error.

`Mappings.usmap` must also be regenerated for the new version (UE4SS `Keybinds`
mod, Ctrl+Num6), or every property lookup silently returns nothing.

## Known asset quirks

`LongdownValley.umap` takes roughly two minutes and produces ~200 MB. That is
expected, not a hang.
