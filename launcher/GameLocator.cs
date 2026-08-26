using System.IO;
using System.Text.RegularExpressions;
using Microsoft.Win32;

namespace SDB;

public enum ModStatus
{
    NotFound,       // Win64 dir itself couldn't be located
    NotInstalled,   // Win64 found, but no UE4SS/mod files present
    Installed,      // UE4SS + mod DLL both present
}

public record GameInfo(string? Win64Path, string? GameExePath, ModStatus ModStatus);

// Steam-path detection ported from the previous WinForms launcher
// (launcher/Launcher.cs, since removed) — same logic, just relocated.
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

        var env = Environment.GetEnvironmentVariable("SDB_GAME_WIN64");
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
        catch { /* no Steam registry entry — fall through to whatever else was found */ }

        return candidates.FirstOrDefault(Directory.Exists);
    }

    static ModStatus DetectModStatus(string win64)
    {
        var ue4ssPresent = File.Exists(Path.Combine(win64, "UE4SS.dll"))
                         && File.Exists(Path.Combine(win64, "dwmapi.dll"));
        var modDllPresent = File.Exists(Path.Combine(win64, "Mods", "SurrounDeadBridge", "dlls", "main.dll"));
        return ue4ssPresent && modDllPresent ? ModStatus.Installed : ModStatus.NotInstalled;
    }
}
