# play.ps1 - Fetch a join ticket and launch SurrounDead.
# Usage:  .\scripts\play.ps1 -Name "YourName"

param(
    [Parameter(Mandatory)][string]$Name,
    [string]$Server   = "game.ristl.org",
    [int]$DirPort     = 31100,
    [int]$GwPort      = 31000,
    [string]$ServerId = "customer-world-1",
    [string]$PlayerId = ""
)
$ErrorActionPreference = "Stop"

# Stable per-machine player ID
if (-not $PlayerId) {
    $idFile = "$env:APPDATA\SurrounDeadBridge\player.id"
    if (Test-Path $idFile) {
        $PlayerId = (Get-Content $idFile -Raw).Trim()
    } else {
        New-Item -ItemType Directory -Force -Path (Split-Path $idFile) | Out-Null
        $PlayerId = [System.Guid]::NewGuid().ToString("N")
        Set-Content -Path $idFile -Value $PlayerId -NoNewline
    }
}

Write-Host "Player : $Name  ($PlayerId)"
Write-Host "Server : ${Server}:${GwPort}"

# Fetch join ticket
$uri  = "http://${Server}:${DirPort}/v1/join"
$body = "{`"serverId`":`"$ServerId`",`"playerId`":`"$PlayerId`",`"displayName`":`"$Name`"}"
try {
    $resp = Invoke-RestMethod -Method Post -Uri $uri -ContentType "application/json" -Body $body
} catch {
    Write-Error "Join request failed: $_"
    exit 1
}

if ($resp.error -or -not $resp.ticket) {
    Write-Error "Server rejected join: $($resp.error)"
    exit 1
}

$ticket = $resp.ticket
Write-Host "Ticket : $($ticket.Substring(0, 20))...  (valid 2 min)"

# Write session config so the mod can read it regardless of Steam restart state
$cfgDir  = "$env:APPDATA\SurrounDeadBridge"
$cfgFile = "$cfgDir\session.cfg"
New-Item -ItemType Directory -Force -Path $cfgDir | Out-Null
$cfgLines = "SDB_GATEWAY_HOST=$Server`r`nSDB_GATEWAY_PORT=$GwPort`r`nSDB_JOIN_TICKET=$ticket`r`nSDB_MOVE_INTERVAL_MS=50"
[System.IO.File]::WriteAllText($cfgFile, $cfgLines, [System.Text.Encoding]::ASCII)
Write-Host "Config : $cfgFile"

Write-Host "Launching via Steam..."
Start-Process "steam://run/1645820//"
Write-Host "Done. Watch UE4SS console for: SDB: session latched"
