# Launch SDO against PC1's local dev server (<pc1-lan-ip>:42200/42201) as PC2.
# Fetches a fresh ticket each run (tickets are short-lived) and writes
# session.cfg before launching the game via Steam.

$ErrorActionPreference = "Stop"
$GatewayHost = "<pc1-lan-ip>"

$idFile = "$env:APPDATA\SDO\player2.id"
if (Test-Path $idFile) {
    $playerId = (Get-Content $idFile -Raw).Trim()
} else {
    New-Item -ItemType Directory -Force -Path (Split-Path $idFile) | Out-Null
    $playerId = [System.Guid]::NewGuid().ToString("N")
    Set-Content -Path $idFile -Value $playerId -NoNewline
}

$body = "{`"playerId`":`"$playerId`",`"displayName`":`"PC2`"}"
$resp = Invoke-RestMethod -Method Post -Uri "http://${GatewayHost}:42201/v1/tickets" -ContentType "application/json" -Body $body

$cfgDir  = "$env:APPDATA\SDO"
$cfgFile = "$cfgDir\session.cfg"
New-Item -ItemType Directory -Force -Path $cfgDir | Out-Null
$cfgLines = "SDO_GATEWAY_HOST=$GatewayHost`r`nSDO_GATEWAY_PORT=42200`r`nSDO_JOIN_TICKET=$($resp.ticket)`r`nSDO_MOVE_INTERVAL_MS=50"
[System.IO.File]::WriteAllText($cfgFile, $cfgLines, [System.Text.Encoding]::ASCII)

Write-Host "Ticket fetched, session.cfg written. Launching..."
Start-Process "steam://run/1645820//"
