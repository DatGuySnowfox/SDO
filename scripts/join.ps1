# Launch SDO by picking a live server from the directory (directory-worker/)
# instead of hardcoding a gateway IP like launch-pc1.ps1/launch-pc2.ps1 do
# for this project's own PC1/PC2 dev-test setup. For an actual player who
# doesn't know any host's address ahead of time.
#
# Usage:
#   .\scripts\join.ps1                                  # uses $env:SDB_DIRECTORY_URL
#   .\scripts\join.ps1 -DirectoryUrl "https://sdo-directory.<you>.workers.dev"

param(
    [string]$DirectoryUrl = $env:SDB_DIRECTORY_URL
)

$ErrorActionPreference = "Stop"

if (-not $DirectoryUrl) {
    Write-Error "No directory URL. Pass -DirectoryUrl or set `$env:SDB_DIRECTORY_URL."
    exit 1
}

$listing = Invoke-RestMethod -Method Get -Uri "$DirectoryUrl/v1/servers"
if (-not $listing.ok -or $listing.servers.Count -eq 0) {
    Write-Error "No servers currently listed in the directory."
    exit 1
}

$servers = @($listing.servers)
if ($servers.Count -eq 1) {
    $chosen = $servers[0]
    Write-Host "One server up: $($chosen.name) ($($chosen.host):$($chosen.port), $($chosen.playerCount)/$($chosen.maxPlayers) players)"
} else {
    Write-Host "Servers currently up:"
    for ($i = 0; $i -lt $servers.Count; $i++) {
        $s = $servers[$i]
        Write-Host "  [$i] $($s.name) — $($s.host):$($s.port) ($($s.playerCount)/$($s.maxPlayers) players)"
    }
    $selection = Read-Host "Pick a server by number"
    $index = [int]$selection
    if ($index -lt 0 -or $index -ge $servers.Count) {
        Write-Error "Invalid selection."
        exit 1
    }
    $chosen = $servers[$index]
}

$GatewayHost = $chosen.host
$GatewayPort = $chosen.port
$HttpPort    = $GatewayPort + 1   # SDB_HTTP_PORT is conventionally gatewayPort+1 (42200/42201 default)

$idFile = "$env:APPDATA\SurrounDeadBridge\player.id"
if (Test-Path $idFile) {
    $playerId = (Get-Content $idFile -Raw).Trim()
} else {
    New-Item -ItemType Directory -Force -Path (Split-Path $idFile) | Out-Null
    $playerId = [System.Guid]::NewGuid().ToString("N")
    Set-Content -Path $idFile -Value $playerId -NoNewline
}

$body = "{`"playerId`":`"$playerId`",`"displayName`":`"Player`"}"
$resp = Invoke-RestMethod -Method Post -Uri "http://${GatewayHost}:${HttpPort}/v1/tickets" -ContentType "application/json" -Body $body

$cfgDir  = "$env:APPDATA\SurrounDeadBridge"
$cfgFile = "$cfgDir\session.cfg"
New-Item -ItemType Directory -Force -Path $cfgDir | Out-Null
$cfgLines = "SDB_GATEWAY_HOST=$GatewayHost`r`nSDB_GATEWAY_PORT=$GatewayPort`r`nSDB_JOIN_TICKET=$($resp.ticket)`r`nSDB_MOVE_INTERVAL_MS=50"
[System.IO.File]::WriteAllText($cfgFile, $cfgLines, [System.Text.Encoding]::ASCII)

Write-Host "Ticket fetched, session.cfg written. Launching..."
Start-Process "steam://run/1645820//"
