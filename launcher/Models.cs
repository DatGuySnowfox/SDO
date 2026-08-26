using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text.Json.Serialization;

namespace SDB;

// Matches directory-worker's GET /v1/servers response shape exactly
// (directory-worker/src/index.js: listServers()). Implements
// INotifyPropertyChanged so the DataGrid picks up PingAsync's results
// live — these come in asynchronously well after the initial data-bind,
// not just once at load.
public class ServerEntry : INotifyPropertyChanged
{
    [JsonPropertyName("serverId")]
    public string ServerId { get; set; } = "";

    [JsonPropertyName("name")]
    public string Name { get; set; } = "";

    [JsonPropertyName("host")]
    public string Host { get; set; } = "";

    [JsonPropertyName("port")]
    public int Port { get; set; }

    [JsonPropertyName("playerCount")]
    public int PlayerCount { get; set; }

    [JsonPropertyName("maxPlayers")]
    public int MaxPlayers { get; set; }

    // UI-only, filled in by DirectoryClient.PingAsync — not part of the
    // directory payload. Pinged distinguishes "not checked yet" from
    // "checked and confirmed offline" (IsOnline=false covers both, which
    // isn't enough for the status column to render correctly before the
    // first ping completes).
    long?  _pingMs;
    bool   _isOnline;
    bool   _pinged;

    public long? PingMs   { get => _pingMs;   set { _pingMs = value;   Raise(); Raise(nameof(StatusText)); } }
    public bool  IsOnline { get => _isOnline; set { _isOnline = value; Raise(); Raise(nameof(StatusText)); } }
    public bool  Pinged   { get => _pinged;   set { _pinged = value;   Raise(); Raise(nameof(StatusText)); } }

    public string StatusText => !Pinged ? "…" : IsOnline ? "Online" : "Offline";

    public event PropertyChangedEventHandler? PropertyChanged;
    void Raise([CallerMemberName] string? name = null) =>
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
}

class ServersResponse
{
    [JsonPropertyName("ok")]
    public bool Ok { get; set; }

    [JsonPropertyName("servers")]
    public List<ServerEntry> Servers { get; set; } = [];
}

class TicketResponse
{
    [JsonPropertyName("ticket")]
    public string Ticket { get; set; } = "";

    [JsonPropertyName("gatewayHost")]
    public string GatewayHost { get; set; } = "";

    [JsonPropertyName("gatewayPort")]
    public int GatewayPort { get; set; }
}

public class Settings
{
    public string PlayerId { get; set; } = Guid.NewGuid().ToString("N");
    public string Nickname { get; set; } = "";
}
