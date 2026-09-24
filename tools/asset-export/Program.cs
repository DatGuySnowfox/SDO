using CUE4Parse.FileProvider;
using CUE4Parse.MappingsProvider.Usmap;
using CUE4Parse.UE4.Versions;
using Newtonsoft.Json;

// aexport <paksDir> <usmapPath> <outDir> <assetPath> [assetPath...]
//
// Headless replacement for FModel's "Export Folder Packages (.json)".
// Exists so the level can be re-extracted from a script after a game
// update instead of by clicking through a GUI.

if (args.Length < 4)
{
    Console.Error.WriteLine("usage: aexport <paksDir> <usmap> <outDir> <assetPath>...");
    return 2;
}

var paks = args[0];
var usmap = args[1];
var outDir = args[2];
var assets = args[3..];

Console.WriteLine($"paks  : {paks}");
Console.WriteLine($"usmap : {usmap}");
Console.WriteLine($"out   : {outDir}");

var provider = new DefaultFileProvider(
    paks, SearchOption.TopDirectoryOnly, new VersionContainer(EGame.GAME_UE5_6));

provider.Initialize();
provider.Mount();
provider.MappingsContainer = new FileUsmapTypeMappingsProvider(usmap);

Console.WriteLine($"mounted files: {provider.Files.Count}");

int ok = 0, fail = 0;
foreach (var asset in assets)
{
    try
    {
        var sw = System.Diagnostics.Stopwatch.StartNew();
        var pkg = provider.LoadPackage(asset);
        var exports = pkg.GetExports();

        // Mirror FModel's layout: <outDir>/<full/asset/path>.json
        var rel = Path.ChangeExtension(asset, ".json");
        var dest = Path.Combine(outDir, rel.Replace('/', Path.DirectorySeparatorChar));
        Directory.CreateDirectory(Path.GetDirectoryName(dest)!);

        using (var fs = File.Create(dest))
        using (var tw = new StreamWriter(fs))
        using (var jw = new JsonTextWriter(tw) { Formatting = Formatting.Indented })
        {
            JsonSerializer.CreateDefault().Serialize(jw, exports);
        }

        var len = new FileInfo(dest).Length;
        Console.WriteLine($"  OK   {asset}  ->  {len:N0} bytes  ({sw.ElapsedMilliseconds:N0} ms)");
        ok++;
    }
    catch (Exception e)
    {
        Console.WriteLine($"  FAIL {asset}");
        Console.WriteLine($"       {e.GetType().Name}: {e.Message}");
        fail++;
    }
}

Console.WriteLine($"\n{ok} ok, {fail} failed");
return fail == 0 ? 0 : 1;
