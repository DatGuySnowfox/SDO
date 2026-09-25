using System.IO;
using System.Text.RegularExpressions;
using Microsoft.Win32;

namespace SDO;

public enum ModStatus
{
    NotFound,       // Win64 dir itself couldn't be located
    NotInstalled,   // Win64 found, but no UE4SS/mod files present
    Installed,      // UE4SS + mod DLL both present
}

public record GameInfo(string? Win64Path, string? GameExePath, ModStatus ModStatus);

// Steam-path detection ported from the previous WinForms launcher
// (launcher/Launcher.cs, since removed) - same logic, just relocated.
public static class GameLocator
{
    public static GameInfo Locate()
    {
        var win64 = FindWin64Dir();
        if (win64 is null) return new GameInfo(null, null, ModStatus.NotFound);

        var exe = Path.Combine(win64, "SurrounDead-Win64-Shipping.exe");
        var status = DetectModStatus(win64);
        return new GameInfo(win64, File.Exists(exe) ? exe : null, status);
    }

    static string? FindWin64Dir()
    {
        var candidates = new List<string>();

        var env = Environment.GetEnvironmentVariable("SDO_GAME_WIN64");
        if (!string.IsNullOrEmpty(env)) candidates.Add(env);

        try
        {
            using var key = Registry.CurrentUser.OpenSubKey(@"Software\Valve\Steam");
            var steamPath = key?.GetValue("SteamPath") as string;
            if (steamPath != null)
            {
                candidates.Add(Path.Combine(steamPath,
                    @"steamapps\common\SurrounDead\SurrounDead\Binaries\Win64"));

                var vdf = Path.Combine(steamPath, @"steamapps\libraryfolders.vdf");
                if (File.Exists(vdf))
                    foreach (Match m in Regex.Matches(File.ReadAllText(vdf), @"""path""\s+""([^""]+)"""))
                        candidates.Add(Path.Combine(
                            m.Groups[1].Value.Replace(@"\\", @"\"),
                            @"steamapps\common\SurrounDead\SurrounDead\Binaries\Win64"));
            }
        }
        catch { /* no Steam registry entry - fall through to whatever else was found */ }

        return candidates.FirstOrDefault(Directory.Exists);
    }

    // UE4SS moved its layout after v3.0.1: the loader (dwmapi.dll) still sits
    // next to the game exe, but UE4SS.dll, the settings file and Mods/ all
    // dropped into a ue4ss/ subdirectory. Both are in the wild, so resolve
    // which one is present rather than assuming - scripts/deploy.ps1 already
    // does exactly this, and this file assuming the old one meant a correctly
    // installed mod on a current UE4SS reported as missing, which disables the
    // Launch button.
    public static string? FindUe4ssRoot(string win64)
    {
        var modern = Path.Combine(win64, "ue4ss");
        if (File.Exists(Path.Combine(modern, "UE4SS.dll"))) return modern;
        if (File.Exists(Path.Combine(win64, "UE4SS.dll"))) return win64;
        return null;
    }

    // Where the mod's DLL belongs for a given install. Returns null when UE4SS
    // itself is absent, since there is no correct answer in that case.
    public static string? ModDllPath(string win64)
    {
        var root = FindUe4ssRoot(win64);
        return root is null ? null : Path.Combine(root, "Mods", "SDO", "dlls", "main.dll");
    }

    static ModStatus DetectModStatus(string win64)
    {
        // dwmapi.dll is the loader and stays beside the exe in both layouts.
        if (!File.Exists(Path.Combine(win64, "dwmapi.dll"))) return ModStatus.NotInstalled;

        var dll = ModDllPath(win64);
        return dll != null && File.Exists(dll) ? ModStatus.Installed : ModStatus.NotInstalled;
    }
}
