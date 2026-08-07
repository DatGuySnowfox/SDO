// SurrounDead Bridge Launcher
// Build: dotnet publish -c Release -r win-x64 --self-contained
//        Output: out/SDBLauncher.exe  (no .NET install required on target machine)

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Net.Http;
using System.Net.Http.Json;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.Win32;

namespace SDB;

// ── Data model ────────────────────────────────────────────────────────────────

record ServerEntry(string Name, string Host, string Port);

class Settings
{
    public string PlayerId   { get; set; } = Guid.NewGuid().ToString();
    public string Nickname   { get; set; } = "";
    public List<ServerEntry> Servers { get; set; } = [];
    public string DirectHost { get; set; } = "";
    public string DirectPort { get; set; } = "42201";
    public string AdminToken { get; set; } = "";
}

// ── Entry point ───────────────────────────────────────────────────────────────

static class Program
{
    [STAThread]
    static void Main()
    {
        Application.EnableVisualStyles();
        Application.SetCompatibleTextRenderingDefault(false);
        Application.Run(new LauncherForm());
    }
}

// ── Launcher form ─────────────────────────────────────────────────────────────

class LauncherForm : Form
{
    // ── Theme ─────────────────────────────────────────────────────────────────
    static readonly Color BG     = Color.FromArgb(24,  24,  24);
    static readonly Color PANEL  = Color.FromArgb(36,  36,  36);
    static readonly Color FIELD  = Color.FromArgb(52,  52,  52);
    static readonly Color BORDER = Color.FromArgb(70,  70,  70);
    static readonly Color FG     = Color.FromArgb(224, 224, 224);
    static readonly Color MUTED  = Color.FromArgb(140, 140, 140);
    static readonly Color BLUE   = Color.FromArgb(0,   120, 215);
    static readonly Color GOOD   = Color.FromArgb(78,  201,  78);
    static readonly Color BAD    = Color.FromArgb(244,  71,  71);
    static readonly Color WARN   = Color.FromArgb(220, 170,  50);

    static readonly Font FontNormal = new("Segoe UI", 9f);
    static readonly Font FontBold   = new("Segoe UI", 9f,  FontStyle.Bold);
    static readonly Font FontTitle  = new("Segoe UI", 16f, FontStyle.Bold);
    static readonly Font FontSmall  = new("Segoe UI", 8f);

    // ── State ─────────────────────────────────────────────────────────────────
    Settings    _cfg;
    string?     _gameExe;
    HttpClient  _http = new() { Timeout = TimeSpan.FromSeconds(6) };

    static readonly JsonSerializerOptions JsonOpts = new()
    {
        PropertyNamingPolicy        = JsonNamingPolicy.CamelCase,
        WriteIndented               = true,
        DefaultIgnoreCondition      = JsonIgnoreCondition.WhenWritingNull,
    };

    // ── Controls ──────────────────────────────────────────────────────────────
    TextBox   _txtNick  = null!;
    TextBox   _txtDHost = null!;
    TextBox   _txtDPort = null!;
    TextBox   _txtDToken = null!;
    ListView  _lvServers = null!;
    Label     _lblStatus = null!;
    Button    _btnConnect = null!;
    Button    _btnLaunch  = null!;
    Panel  _pServers      = null!;
    Panel  _pDirect       = null!;
    Button _btnTabServers = null!;
    Button _btnTabDirect  = null!;
    int    _activeTab     = 0;

    // ── Constructor ───────────────────────────────────────────────────────────

    public LauncherForm()
    {
        _cfg     = LoadSettings();
        _gameExe = FindGameExe();
        BuildUI();
        RebuildList();
        Load += (_, _) => _ = WarmUpAsync();
    }

    async Task WarmUpAsync()
    {
        // Fire a health ping in the background so the HTTP connection and Node.js
        // JIT are warmed up before the user clicks Launch Game.
        var host = _activeTab == 0 && _lvServers.SelectedItems.Count > 0
            ? ((ServerEntry)_lvServers.SelectedItems[0].Tag!).Host
            : _cfg.DirectHost;
        var port = _activeTab == 0 && _lvServers.SelectedItems.Count > 0
            ? ((ServerEntry)_lvServers.SelectedItems[0].Tag!).Port
            : _cfg.DirectPort;
        if (string.IsNullOrEmpty(host)) return;
        try { await _http.GetAsync($"http://{host}:{port}/v1/health"); } catch { }
    }

    // ── Settings (stored in Windows Registry — no file on disk) ──────────────

    const string RegPath = @"Software\SDB";

    Settings LoadSettings()
    {
        var s = new Settings();
        try
        {
            using var key = Registry.CurrentUser.OpenSubKey(RegPath);
            if (key is null) return s;

            s.PlayerId   = key.GetValue("PlayerId")   as string ?? s.PlayerId;
            s.Nickname   = key.GetValue("Nickname")   as string ?? s.Nickname;
            s.DirectHost = key.GetValue("DirectHost") as string ?? s.DirectHost;
            s.DirectPort = key.GetValue("DirectPort") as string ?? s.DirectPort;
            s.AdminToken = key.GetValue("AdminToken") as string ?? s.AdminToken;

            var sv = key.GetValue("Servers") as string;
            if (!string.IsNullOrEmpty(sv))
                s.Servers = JsonSerializer.Deserialize<List<ServerEntry>>(sv, JsonOpts) ?? [];
        }
        catch { }
        return s;
    }

    void SaveSettings()
    {
        try
        {
            using var key = Registry.CurrentUser.CreateSubKey(RegPath);
            key.SetValue("PlayerId",   _cfg.PlayerId);
            key.SetValue("Nickname",   _cfg.Nickname);
            key.SetValue("DirectHost", _cfg.DirectHost);
            key.SetValue("DirectPort", _cfg.DirectPort);
            key.SetValue("AdminToken", _cfg.AdminToken);
            key.SetValue("Servers",    JsonSerializer.Serialize(_cfg.Servers, JsonOpts));
        }
        catch { }
    }

    // ── Game detection ────────────────────────────────────────────────────────

    string? FindGameExe()
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
        catch { }

        foreach (var dir in candidates)
        {
            var exe = Path.Combine(dir, "SurrounDead-Win64-Shipping.exe");
            if (File.Exists(exe)) return exe;
        }
        return null;
    }

    // ── UI helpers ────────────────────────────────────────────────────────────

    static Label MkLabel(string text, int x, int y, int w, int h,
                         Font? font = null, Color col = default)
    {
        var l = new Label
        {
            Text      = text,
            Left      = x, Top = y, Width = w, Height = h,
            Font      = font ?? FontNormal,
            ForeColor = col == default ? FG : col,
            BackColor = Color.Transparent,
            AutoSize  = false,
        };
        return l;
    }

    static TextBox MkBox(string text, int x, int y, int w, int h = 26, bool masked = false)
    {
        var t = new TextBox
        {
            Text        = text,
            Left        = x, Top = y, Width = w, Height = h,
            Font        = FontNormal,
            BackColor   = FIELD,
            ForeColor   = FG,
            BorderStyle = BorderStyle.FixedSingle,
        };
        if (masked) t.PasswordChar = '*';
        return t;
    }

    static Button MkBtn(string text, int x, int y, int w, int h, bool primary = false)
    {
        var b = new Button
        {
            Text      = text,
            Left      = x, Top = y, Width = w, Height = h,
            Font      = FontBold,
            FlatStyle = FlatStyle.Flat,
            BackColor = primary ? BLUE : PANEL,
            ForeColor = FG,
            Cursor    = Cursors.Hand,
            UseVisualStyleBackColor = false,
        };
        b.FlatAppearance.BorderColor = primary ? BLUE : BORDER;
        b.FlatAppearance.BorderSize  = 1;
        return b;
    }

    // ── Build UI ──────────────────────────────────────────────────────────────

    void BuildUI()
    {
        Text            = "SurrounDead Bridge";
        ClientSize      = new Size(600, 580);
        MinimumSize     = new Size(600, 500);
        StartPosition   = FormStartPosition.CenterScreen;
        BackColor       = BG;
        ForeColor       = FG;
        Font            = FontNormal;
        FormBorderStyle = FormBorderStyle.Sizable;
        Icon            = SystemIcons.Application;
        FormClosing    += (_, _) => PersistFields();

        // WinForms Controls.Add inserts at index 0 (BringToFront); the dock engine
        // processes from index 0 outward.  To let edge controls (Top/Bottom) claim
        // their strips BEFORE the Fill panels expand, Fill panels must live at HIGH
        // indices — so they must be Controls.Add-ed FIRST and then pushed outward by
        // subsequent adds.
        _pServers = new Panel { Dock = DockStyle.Fill, BackColor = BG };
        _pDirect  = new Panel { Dock = DockStyle.Fill, BackColor = BG, Visible = false };
        Controls.Add(_pServers);   // will be pushed to index 4
        Controls.Add(_pDirect);    // will be pushed to index 3

        // Bottom bar — added next, ends up at index 2
        var pBot = new Panel { Dock = DockStyle.Bottom, Height = 60, BackColor = PANEL };
        _lblStatus  = MkLabel("Ready.", 12, 20, 320, 22, col: MUTED);
        _btnConnect = MkBtn("Connect",     354, 15, 104, 32, true);
        _btnLaunch  = MkBtn("Launch Game", 464, 15, 122, 32, false);

        _btnLaunch.Enabled = !string.IsNullOrEmpty(_gameExe);
        if (string.IsNullOrEmpty(_gameExe)) _btnLaunch.ForeColor = MUTED;

        _btnConnect.Click += async (_, _) => await OnConnectAsync();
        _btnLaunch.Click  += async (_, _) => await OnLaunchAsync();

        pBot.Controls.AddRange([_lblStatus, _btnConnect, _btnLaunch]);
        Controls.Add(pBot);        // ends up at index 2

        // Tab selector bar — ends up at index 1, docks below the header
        var pTabBar = new Panel { Dock = DockStyle.Top, Height = 40, BackColor = PANEL };
        _btnTabServers = MkBtn("Servers",        4,   5, 100, 30, false);
        _btnTabDirect  = MkBtn("Direct Connect", 108, 5, 130, 30, false);
        _btnTabServers.Click += (_, _) => SwitchTab(0);
        _btnTabDirect.Click  += (_, _) => SwitchTab(1);
        pTabBar.Controls.AddRange([_btnTabServers, _btnTabDirect]);
        Controls.Add(pTabBar);     // ends up at index 1

        // Header — added LAST so it lands at index 0, processed first, docks at y=0
        var pHead = new Panel { Dock = DockStyle.Top, Height = 72, BackColor = PANEL };
        pHead.Controls.Add(MkLabel("SurrounDead Bridge", 16, 10, 340, 34, FontTitle));
        pHead.Controls.Add(MkLabel("Nickname", 16, 46, 74, 22, col: MUTED));

        _txtNick = MkBox(_cfg.Nickname, 94, 43, 200, 24);
        _txtNick.MaxLength = 32;
        pHead.Controls.Add(_txtNick);

        pHead.Controls.Add(MkLabel($"ID: {_cfg.PlayerId[..8]}…", 306, 46, 270, 22, FontSmall, MUTED));
        Controls.Add(pHead);       // lands at index 0 → docks first → y=0

        BuildServersContent();
        BuildDirectContent();
        SwitchTab(0);
    }

    void BuildServersContent()
    {
        _lvServers = new ListView
        {
            View          = View.Details,
            FullRowSelect = true,
            GridLines     = false,
            MultiSelect   = false,
            BackColor     = FIELD,
            ForeColor     = FG,
            Font          = FontNormal,
            BorderStyle   = BorderStyle.None,
            HeaderStyle   = ColumnHeaderStyle.Nonclickable,
            Dock          = DockStyle.Fill,
        };

        foreach (var (title, w) in new (string, int)[] {
            ("Name", 190), ("Host", 162), ("Players", 74), ("Ping", 58), ("Status", 88) })
            _lvServers.Columns.Add(title, w);

        _lvServers.DoubleClick += (_, _) =>
        {
            if (_lvServers.SelectedItems.Count == 0) return;
            var sv = (ServerEntry)_lvServers.SelectedItems[0].Tag!;
            _txtDHost.Text = sv.Host;
            _txtDPort.Text = sv.Port;
            SwitchTab(1);
        };

        var bRefresh = MkBtn("Refresh",     8,  8, 100, 30);
        var bAdd     = MkBtn("Add Server", 114, 8, 110, 30);
        var bRemove  = MkBtn("Remove",     230, 8,  90, 30);

        bRefresh.Click += async (_, _) => await OnRefreshAsync();
        bAdd.Click     += (_, _)        => OnAddServer();
        bRemove.Click  += (_, _)        => OnRemoveServer();

        var btnBar = new Panel { Dock = DockStyle.Bottom, Height = 46, BackColor = BG };
        btnBar.Controls.AddRange([bRefresh, bAdd, bRemove]);

        // WinForms docks last-added first; add Fill list first so it docks second
        // (fills remaining space), and add Bottom bar second so it docks first (claims bottom).
        _pServers.Controls.Add(_lvServers);
        _pServers.Controls.Add(btnBar);
    }

    void BuildDirectContent()
    {
        void Row(string lbl, ref TextBox target, string val, int top, int bw = 260, bool masked = false)
        {
            _pDirect.Controls.Add(MkLabel(lbl, 16, top + 3, 130, 22, col: MUTED));
            target = MkBox(val, 150, top, bw, 26, masked);
            _pDirect.Controls.Add(target);
        }

        Row("Host / IP",   ref _txtDHost,  _cfg.DirectHost, 24);
        Row("HTTP port",   ref _txtDPort,  _cfg.DirectPort, 64, 80);
        Row("Admin token", ref _txtDToken, _cfg.AdminToken, 104, 260, true);

        _pDirect.Controls.Add(MkLabel(
            "Leave admin token blank for public servers.", 16, 148, 440, 20, FontSmall, MUTED));
    }

    // ── Server list ───────────────────────────────────────────────────────────

    void RebuildList()
    {
        _lvServers.Items.Clear();
        foreach (var sv in _cfg.Servers)
        {
            var item = new ListViewItem(sv.Name) { Tag = sv, ForeColor = FG };
            item.SubItems.Add($"{sv.Host}:{sv.Port}");
            item.SubItems.Add("--");
            item.SubItems.Add("--");
            item.SubItems.Add("--");
            _lvServers.Items.Add(item);
        }
    }

    // ── Event: Refresh ────────────────────────────────────────────────────────

    async Task OnRefreshAsync()
    {
        if (_lvServers.Items.Count == 0) { SetStatus("No servers — click Add Server first.", WARN); return; }
        SetStatus("Pinging...", WARN);
        _btnConnect.Enabled = false;

        foreach (ListViewItem item in _lvServers.Items)
        {
            var sv = (ServerEntry)item.Tag!;
            try
            {
                var sw   = Stopwatch.StartNew();
                var resp = await _http.GetFromJsonAsync<JsonElement>($"http://{sv.Host}:{sv.Port}/v1/health");
                sw.Stop();

                var players    = resp.TryGetProperty("players",    out var p) ? p.GetInt32() : 0;
                var maxPlayers = resp.TryGetProperty("maxPlayers", out var mp) ? mp.GetInt32() : 0;
                var online     = resp.TryGetProperty("hostOnline", out var ho) && ho.GetBoolean();

                item.SubItems[2].Text = maxPlayers > 0 ? $"{players}/{maxPlayers}" : $"{players}";
                item.SubItems[3].Text = $"{sw.ElapsedMilliseconds} ms";
                item.SubItems[4].Text = online ? "Online" : "No host";
                item.ForeColor        = online ? GOOD : WARN;
            }
            catch
            {
                item.SubItems[2].Text = "--";
                item.SubItems[3].Text = "--";
                item.SubItems[4].Text = "Offline";
                item.ForeColor        = BAD;
            }
        }

        _btnConnect.Enabled = true;
        SetStatus("Refresh complete.", MUTED);
    }

    // ── Event: Add Server ─────────────────────────────────────────────────────

    void OnAddServer()
    {
        using var dlg = new AddServerForm();
        if (dlg.ShowDialog(this) != DialogResult.OK) return;
        _cfg.Servers.Add(new ServerEntry(dlg.ServerName, dlg.ServerHost, dlg.ServerPort));
        SaveSettings();
        RebuildList();
    }

    // ── Event: Remove Server ──────────────────────────────────────────────────

    void OnRemoveServer()
    {
        if (_lvServers.SelectedItems.Count == 0) { SetStatus("Select a server to remove.", WARN); return; }
        var sv = (ServerEntry)_lvServers.SelectedItems[0].Tag!;
        if (MessageBox.Show($"Remove '{sv.Name}'?", "Confirm", MessageBoxButtons.YesNo, MessageBoxIcon.Question)
            != DialogResult.Yes) return;
        _cfg.Servers.Remove(sv);
        SaveSettings();
        RebuildList();
    }

    // ── Event: Connect ────────────────────────────────────────────────────────

    async Task OnConnectAsync()
    {
        if (await GetTicketAsync())
            SetStatus("Connected — press Launch Game when ready.", GOOD);
    }

    // ── Event: Launch Game (auto-connects first) ──────────────────────────────

    async Task OnLaunchAsync()
    {
        if (string.IsNullOrEmpty(_gameExe) || !File.Exists(_gameExe))
        {
            SetStatus("Game not found — launch SurrounDead through Steam.", WARN);
            return;
        }
        if (!await GetTicketAsync()) return;
        SetStatus("Launching...", GOOD);
        Process.Start(new ProcessStartInfo(_gameExe) { UseShellExecute = true });
    }

    // ── Shared ticket fetch ───────────────────────────────────────────────────

    async Task<bool> GetTicketAsync()
    {
        var nick = _txtNick.Text.Trim();
        if (string.IsNullOrEmpty(nick)) { SetStatus("Enter a nickname first.", BAD); return false; }

        string host, port, token;
        if (_activeTab == 0)
        {
            if (_lvServers.SelectedItems.Count == 0) { SetStatus("Select a server first.", BAD); return false; }
            var sv = (ServerEntry)_lvServers.SelectedItems[0].Tag!;
            host = sv.Host; port = sv.Port; token = "";
        }
        else
        {
            host  = _txtDHost.Text.Trim();
            port  = _txtDPort.Text.Trim();
            token = _txtDToken.Text.Trim();
        }

        if (string.IsNullOrEmpty(host)) { SetStatus("No host specified.", BAD); return false; }

        _cfg.Nickname = nick;
        SaveSettings();

        SetStatus($"Contacting {host}...", WARN);
        _btnConnect.Enabled = false;
        _btnLaunch.Enabled  = false;

        try
        {
            using var req = new HttpRequestMessage(HttpMethod.Post,
                $"http://{host}:{port}/v1/tickets");
            req.Content = JsonContent.Create(new { playerId = _cfg.PlayerId, displayName = nick });
            if (!string.IsNullOrEmpty(token))
                req.Headers.TryAddWithoutValidation("Authorization", $"Bearer {token}");

            using var res  = await _http.SendAsync(req);
            var       body = await res.Content.ReadFromJsonAsync<JsonElement>();

            if (!res.IsSuccessStatusCode)
            {
                var err = body.TryGetProperty("error", out var e) ? e.GetString() : res.ReasonPhrase;
                SetStatus($"Server error: {err}", BAD);
                return false;
            }

            var ticket = body.GetProperty("ticket").GetString()!;
            var gwHost = body.GetProperty("gatewayHost").GetString()!;
            var gwPort = body.GetProperty("gatewayPort").GetInt32().ToString();

            // User-level SetEnvironmentVariable broadcasts WM_SETTINGCHANGE to all
            // windows synchronously — do it off the UI thread to stay responsive.
            await Task.Run(() =>
            {
                Environment.SetEnvironmentVariable("SDB_JOIN_TICKET",  ticket, EnvironmentVariableTarget.User);
                Environment.SetEnvironmentVariable("SDB_GATEWAY_HOST", gwHost, EnvironmentVariableTarget.User);
                Environment.SetEnvironmentVariable("SDB_GATEWAY_PORT", gwPort, EnvironmentVariableTarget.User);
            });
            Environment.SetEnvironmentVariable("SDB_JOIN_TICKET",  ticket);
            Environment.SetEnvironmentVariable("SDB_GATEWAY_HOST", gwHost);
            Environment.SetEnvironmentVariable("SDB_GATEWAY_PORT", gwPort);

            return true;
        }
        catch (Exception ex)
        {
            SetStatus($"Error: {ex.Message}", BAD);
            return false;
        }
        finally
        {
            _btnConnect.Enabled = true;
            _btnLaunch.Enabled  = !string.IsNullOrEmpty(_gameExe);
        }
    }

    // ── Tab switching ─────────────────────────────────────────────────────────

    void SwitchTab(int idx)
    {
        _activeTab        = idx;
        _pServers.Visible = idx == 0;
        _pDirect.Visible  = idx == 1;

        _btnTabServers.BackColor = idx == 0 ? BLUE : PANEL;
        _btnTabServers.FlatAppearance.BorderColor = idx == 0 ? BLUE : BORDER;
        _btnTabDirect.BackColor  = idx == 1 ? BLUE : PANEL;
        _btnTabDirect.FlatAppearance.BorderColor  = idx == 1 ? BLUE : BORDER;
    }

    // ── Helpers ───────────────────────────────────────────────────────────────

    void SetStatus(string msg, Color col)
    {
        _lblStatus.Text      = msg;
        _lblStatus.ForeColor = col;
    }

    void PersistFields()
    {
        _cfg.Nickname   = _txtNick.Text.Trim();
        _cfg.DirectHost = _txtDHost?.Text.Trim() ?? _cfg.DirectHost;
        _cfg.DirectPort = _txtDPort?.Text.Trim() ?? _cfg.DirectPort;
        _cfg.AdminToken = _txtDToken?.Text.Trim() ?? _cfg.AdminToken;
        SaveSettings();
    }
}

// ── Add-Server dialog ─────────────────────────────────────────────────────────

class AddServerForm : Form
{
    static readonly Color PANEL  = Color.FromArgb(36, 36, 36);
    static readonly Color FIELD  = Color.FromArgb(52, 52, 52);
    static readonly Color BORDER = Color.FromArgb(70, 70, 70);
    static readonly Color FG     = Color.FromArgb(224, 224, 224);
    static readonly Color MUTED  = Color.FromArgb(140, 140, 140);
    static readonly Color BLUE   = Color.FromArgb(0,  120, 215);
    static readonly Font  Fnt    = new("Segoe UI", 9f);
    static readonly Font  FntB   = new("Segoe UI", 9f, FontStyle.Bold);

    public string ServerName { get; private set; } = "";
    public string ServerHost { get; private set; } = "";
    public string ServerPort { get; private set; } = "42201";

    readonly TextBox _tName, _tHost, _tPort;

    public AddServerForm()
    {
        Text            = "Add Server";
        ClientSize      = new Size(360, 190);
        StartPosition   = FormStartPosition.CenterParent;
        FormBorderStyle = FormBorderStyle.FixedDialog;
        MaximizeBox     = false; MinimizeBox = false;
        BackColor       = PANEL;  ForeColor  = FG;
        Font            = Fnt;

        Label MkLbl(string t, int x, int y) => new() {
            Text = t, Left = x, Top = y, Width = 110, Height = 20,
            ForeColor = MUTED, BackColor = Color.Transparent, AutoSize = false };

        TextBox MkBox(string v, int x, int y, int w) => new() {
            Text = v, Left = x, Top = y, Width = w, Height = 26,
            Font = Fnt, BackColor = FIELD, ForeColor = FG, BorderStyle = BorderStyle.FixedSingle };

        Button MkBtn(string t, int x, bool primary) {
            var b = new Button {
                Text = t, Left = x, Top = 148, Width = 84, Height = 30,
                Font = FntB, FlatStyle = FlatStyle.Flat, ForeColor = FG,
                BackColor = primary ? BLUE : PANEL, Cursor = Cursors.Hand,
                UseVisualStyleBackColor = false };
            b.FlatAppearance.BorderColor = primary ? BLUE : BORDER;
            b.FlatAppearance.BorderSize  = 1;
            return b; }

        Controls.Add(MkLbl("Server name", 16, 18)); _tName = MkBox("",      130, 15, 210); Controls.Add(_tName);
        Controls.Add(MkLbl("Host / IP",   16, 54)); _tHost = MkBox("",      130, 51, 210); Controls.Add(_tHost);
        Controls.Add(MkLbl("HTTP port",   16, 90)); _tPort = MkBox("42201", 130, 87,  80); Controls.Add(_tPort);

        var ok  = MkBtn("Add",    170, true);  ok.DialogResult  = DialogResult.OK;
        var can = MkBtn("Cancel", 262, false); can.DialogResult = DialogResult.Cancel;
        Controls.AddRange([ok, can]);
        AcceptButton = ok; CancelButton = can;
    }

    protected override void OnFormClosing(FormClosingEventArgs e)
    {
        base.OnFormClosing(e);
        if (DialogResult != DialogResult.OK) return;
        if (string.IsNullOrWhiteSpace(_tName.Text) || string.IsNullOrWhiteSpace(_tHost.Text))
        {
            MessageBox.Show("Name and host are required.", "Validation", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            e.Cancel = true; return;
        }
        ServerName = _tName.Text.Trim();
        ServerHost = _tHost.Text.Trim();
        ServerPort = string.IsNullOrWhiteSpace(_tPort.Text) ? "42201" : _tPort.Text.Trim();
    }
}
