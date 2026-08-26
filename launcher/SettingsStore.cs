using Microsoft.Win32;

namespace SDB;

// Registry-backed settings — no file on disk, same approach as the previous
// WinForms launcher this was rebuilt from.
public static class SettingsStore
{
    const string RegPath = @"Software\SDB";

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
