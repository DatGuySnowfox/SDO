using Microsoft.Win32;

namespace SDO;

// Registry-backed settings — no file on disk, same approach as the previous
// WinForms launcher this was rebuilt from.
public static class SettingsStore
{
    const string RegPath = @"Software\SDO";

    // No directory is baked in: this repo does not ship anyone's server
    // address. Point the launcher at one of
    //   • SDO_DIRECTORY_URL in the environment (wins; easiest per-launch), or
    //   • HKCU\Software\SDO\DirectoryUrl (persistent; what an installer or a
    //     one-line `reg add` would set for testers).
    // Returns empty when unconfigured, which the UI surfaces rather than
    // silently failing against a hardcoded host.
    public static string DirectoryUrl()
    {
        var env = Environment.GetEnvironmentVariable("SDO_DIRECTORY_URL");
        if (!string.IsNullOrWhiteSpace(env)) return env.Trim();
        try
        {
            using var key = Registry.CurrentUser.OpenSubKey(RegPath);
            if (key?.GetValue("DirectoryUrl") is string v && !string.IsNullOrWhiteSpace(v))
                return v.Trim();
        }
        catch { /* registry unavailable — fall through to unconfigured */ }
        return string.Empty;
    }

    public static Settings Load()
    {
        var s = new Settings();
        try
        {
            using var key = Registry.CurrentUser.OpenSubKey(RegPath);
            if (key is null) return s;
            s.PlayerId = key.GetValue("PlayerId") as string ?? s.PlayerId;
            s.Nickname = key.GetValue("Nickname") as string ?? s.Nickname;
        }
        catch { /* first run, or registry unavailable — defaults are fine */ }
        return s;
    }

    public static void Save(Settings s)
    {
        try
        {
            using var key = Registry.CurrentUser.CreateSubKey(RegPath);
            key.SetValue("PlayerId", s.PlayerId);
            key.SetValue("Nickname", s.Nickname);
        }
        catch { /* best-effort — a failed save just means we ask again next launch */ }
    }
}
