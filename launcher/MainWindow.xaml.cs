using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Windows;
using System.Windows.Media;

namespace SDB;

public partial class MainWindow : Window
{
    // Hardcoded rather than configurable: this is the one directory this
    // launcher is built for, same as the URL baked into directory-worker's
    // own served join.ps1 template (see that file's history — the earlier
    // PowerShell approach this replaces).
    const string DirectoryUrl = "https://sdo.ristl.org";
    const string SteamAppId   = "1645820";

    readonly Settings        _settings = SettingsStore.Load();
    readonly DirectoryClient _directory = new(DirectoryUrl);
    readonly ObservableCollection<ServerEntry> _servers = [];
    GameInfo _game;

    public MainWindow()
    {
        InitializeComponent();
        TxtNickname.Text  = _settings.Nickname;
        GridServers.ItemsSource = _servers;

        _game = GameLocator.Locate();
        UpdateModStatusText();

        Loaded += async (_, _) => await RefreshAsync();
        Closing += (_, _) => { _settings.Nickname = TxtNickname.Text.Trim(); SettingsStore.Save(_settings); };
    }

    void UpdateModStatusText()
    {
        (string text, Brush color) = _game.ModStatus switch
        {
            ModStatus.NotFound     => ("Game not found — launch through Steam at least once first.", (Brush)FindResource("Bad")),
            ModStatus.NotInstalled => ("Mod not installed. See the project README to install it manually for now.", (Brush)FindResource("Warn")),
            ModStatus.Installed    => ("Mod installed.", (Brush)FindResource("Good")),
            _                      => ("", (Brush)FindResource("Muted")),
        };
        TxtModStatus.Text = text;
        TxtModStatus.Foreground = color;
        BtnLaunch.IsEnabled = _game.ModStatus == ModStatus.Installed && _game.GameExePath != null;
    }

    async Task RefreshAsync()
    {
        SetStatus("Loading server list…", (Brush)FindResource("Warn"));
        BtnRefresh.IsEnabled = false;
        try
        {
            var servers = await _directory.FetchServersAsync();
            _servers.Clear();
            foreach (var s in servers) _servers.Add(s);

            if (_servers.Count == 0)
            {
                SetStatus("No servers currently up.", (Brush)FindResource("Muted"));
                return;
            }

            SetStatus($"{_servers.Count} server(s) found. Pinging…", (Brush)FindResource("Muted"));
            await Task.WhenAll(_servers.Select(s => _directory.PingAsync(s)));
            SetStatus("Ready.", (Brush)FindResource("Muted"));
        }
        catch (Exception ex)
        {
            SetStatus($"Could not reach the directory: {ex.Message}", (Brush)FindResource("Bad"));
        }
        finally
        {
            BtnRefresh.IsEnabled = true;
        }
    }

    async void OnRefreshClick(object sender, RoutedEventArgs e) => await RefreshAsync();

    void OnServerSelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        BtnLaunch.IsEnabled = GridServers.SelectedItem is ServerEntry
                              && _game.ModStatus == ModStatus.Installed
                              && _game.GameExePath != null;
    }

    async void OnLaunchClick(object sender, RoutedEventArgs e)
    {
        if (GridServers.SelectedItem is not ServerEntry server)
        {
            SetStatus("Select a server first.", (Brush)FindResource("Bad"));
            return;
        }
        var nickname = TxtNickname.Text.Trim();
        if (string.IsNullOrEmpty(nickname))
        {
            SetStatus("Enter a nickname first.", (Brush)FindResource("Bad"));
            return;
        }
        if (_game.GameExePath is null)
        {
            SetStatus("Game not found — launch through Steam at least once first.", (Brush)FindResource("Bad"));
            return;
        }

        _settings.Nickname = nickname;
        SettingsStore.Save(_settings);

        BtnLaunch.IsEnabled = false;
        SetStatus($"Contacting {server.Host}…", (Brush)FindResource("Warn"));
        try
        {
            var (ticket, gwHost, gwPort) = await _directory.GetTicketAsync(
                server.Host, server.Port + 1, _settings.PlayerId, nickname);

            // Steam launch-option handoff (mod.cpp: merge_command_line_args)
            // — the mod reads these three as command-line switches, same
            // convention as directory-worker's served join.ps1 used before
            // this launcher replaced it. No local file write, no env var,
            // just launch args straight to the process.
            var args = $"-sdb_host={gwHost} -sdb_port={gwPort} -sdb_ticket={ticket}";
            Process.Start(new ProcessStartInfo(_game.GameExePath, args) { UseShellExecute = true });
            SetStatus("Launching…", (Brush)FindResource("Good"));
        }
        catch (Exception ex)
        {
            SetStatus($"Could not join: {ex.Message}", (Brush)FindResource("Bad"));
        }
        finally
        {
            BtnLaunch.IsEnabled = true;
        }
    }

    void SetStatus(string text, Brush color)
    {
        TxtStatus.Text = text;
        TxtStatus.Foreground = color;
    }
}
