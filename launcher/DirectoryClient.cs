using System.Diagnostics;
using System.Net.Http;
using System.Net.Http.Json;

namespace SDB;

// Talks to directory-worker (the free Cloudflare Worker) for the live
// server list, and to each individual server's own gateway for ping/ticket.
public class DirectoryClient(string directoryUrl)
{
    readonly HttpClient _http = new() { Timeout = TimeSpan.FromSeconds(6) };

    public async Task<List<ServerEntry>> FetchServersAsync()
    {
        var resp = await _http.GetFromJsonAsync<ServersResponse>($"{directoryUrl}/v1/servers");
        return resp?.Ok == true ? resp.Servers : [];
    }

    // Pings a server's own health endpoint directly (not through the
    // Worker) — this is the actual gateway's own reachability/latency, not
    // the directory's. httpPort convention: gatewayPort+1 (see
    // scripts/join.ps1 and directory-worker's handleJoin, which both assume
    // this same relationship).
    public async Task PingAsync(ServerEntry server)
    {
        try
        {
            var sw = Stopwatch.StartNew();
            var res = await _http.GetAsync($"http://{server.Host}:{server.Port + 1}/v1/health");
            sw.Stop();
            server.IsOnline = res.IsSuccessStatusCode;
            server.PingMs   = sw.ElapsedMilliseconds;
        }
        catch
        {
            server.IsOnline = false;
            server.PingMs   = null;
        }
        finally
        {
            server.Pinged = true;
        }
    }

    // Fetches a ticket directly from the target server's own HTTP API (not
    // proxied through the Worker — unlike the browser status page, a native
    // app has no mixed-content restriction, so there's no need for the
    // /v1/join indirection directory-worker's handleJoin exists for).
    public async Task<(string ticket, string gatewayHost, int gatewayPort)> GetTicketAsync(
        string host, int httpPort, string playerId, string displayName)
    {
        var res = await _http.PostAsJsonAsync($"http://{host}:{httpPort}/v1/tickets",
            new { playerId, displayName });
        res.EnsureSuccessStatusCode();
        var body = await res.Content.ReadFromJsonAsync<TicketResponse>()
            ?? throw new InvalidOperationException("empty ticket response");
        return (body.Ticket, body.GatewayHost, body.GatewayPort);
    }
}
