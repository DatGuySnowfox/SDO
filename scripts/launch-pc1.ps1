# Launch SDO against the local dev server (127.0.0.1:42200/42201) as PC1.
# Fetches a fresh ticket each run (tickets are short-lived) and writes
# session.cfg before launching the game via Steam.

$ErrorActionPreference = "Stop"

$idFile = "$env:APPDATA\SurrounDeadBridge\player.id"
if (Test-Path $idFile) {
    $playerId = (Get-Content $idFile -Raw).Trim()
} else {
    New-Item -ItemType Directory -Force -Path (Split-Path $idFile) | Out-Null
    $playerId = [System.Guid]::NewGuid().ToString("N")
    Set-Content -Path $idFile -Value $playerId -NoNewline
}

$body = "{`"playerId`":`"$playerId`",`"displayName`":`"PC1`"}"
$resp = Invoke-RestMethod -Method Post -Uri "http://127.0.0.1:42201/v1/tickets" -ContentType "application/json" -Body $body

$cfgDir  = "$env:APPDATA\SurrounDeadBridge"
$cfgFile = "$cfgDir\session.cfg"
New-Item -ItemType Directory -Force -Path $cfgDir | Out-Null
$cfgLines = "SDB_GATEWAY_HOST=127.0.0.1`r`nSDB_GATEWAY_PORT=42200`r`nSDB_JOIN_TICKET=$($resp.ticket)`r`nSDB_MOVE_INTERVAL_MS=50"
[System.IO.File]::WriteAllText($cfgFile, $cfgLines, [System.Text.Encoding]::ASCII)

Write-Host "Ticket fetched, session.cfg written. Launching..."
Start-Process "steam://run/1645820//"
