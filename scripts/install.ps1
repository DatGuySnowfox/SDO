# install.ps1 – End-user installer for SurrounDead Bridge
#
# Installs the UE4SS mod loader and the SurrounDead Bridge client into the game.
# Run this once; then launch SurrounDead normally through Steam.
#
# The simplest usage – extract the release zip and run:
#   .\install.ps1 -Ticket "eyJ..."
#
# All parameters are optional except -Ticket (or answer the prompt):
#   -Ticket         Join ticket provided by the server admin
#   -GatewayHost    Gateway server hostname/IP (default: 127.0.0.1)
#   -GatewayPort    Gateway TCP port           (default: 42200)
#   -Win64          Override auto-detected game Binaries\Win64 path
#   -BundleDir      Folder containing UE4SS files + main.dll
#                   (default: same folder as this script)
#   -Uninstall      Remove all SDB files and saved settings

param(
    [string]$Ticket      = '',
    [string]$GatewayHost = '',
    [string]$GatewayPort = '',
    [string]$Win64       = '',
    [string]$BundleDir   = '',
    [switch]$Uninstall   = $false
)

$ErrorActionPreference = 'Stop'

# ── Locate bundle (UE4SS files + main.dll) ────────────────────────────────────

if (-not $BundleDir) { $BundleDir = $PSScriptRoot }

function Test-Bundle([string]$dir) {
    return (Test-Path -LiteralPath (Join-Path $dir 'UE4SS.dll')) -and
           (Test-Path -LiteralPath (Join-Path $dir 'main.dll'))
}

if (-not (Test-Bundle $BundleDir)) {
    Write-Error (
        "Could not find UE4SS.dll + main.dll in: $BundleDir`n" +
        "Extract the full release zip and run install.ps1 from inside it,`n" +
        "or pass -BundleDir <path>."
    )
    exit 1
}

# ── Locate game Win64 ─────────────────────────────────────────────────────────

function Find-Win64 {
    $candidates = @()

    if ($env:SDB_GAME_WIN64) { $candidates += $env:SDB_GAME_WIN64 }

    $launcherSettings = Join-Path $env:LOCALAPPDATA 'SurrounDeadOnline\launcher-settings.json'
    if (Test-Path -LiteralPath $launcherSettings) {
        try {
            $s = Get-Content -Raw -LiteralPath $launcherSettings | ConvertFrom-Json
            if ($s.gameWin64Path) { $candidates += [string]$s.gameWin64Path }
        } catch {}
    }

    $steamReg = Get-ItemProperty -Path 'HKCU:\Software\Valve\Steam' -ErrorAction SilentlyContinue
    if ($steamReg) {
        $steamPath = $steamReg.SteamPath
        $candidates += Join-Path $steamPath 'steamapps\common\SurrounDead\SurrounDead\Binaries\Win64'
        $vdf = Join-Path $steamPath 'steamapps\libraryfolders.vdf'
        if (Test-Path -LiteralPath $vdf) {
            [regex]::Matches((Get-Content -Raw $vdf), '"path"\s+"([^"]+)"') |
            ForEach-Object {
                $candidates += Join-Path $_.Groups[1].Value.Replace('\\', '\') `
                    'steamapps\common\SurrounDead\SurrounDead\Binaries\Win64'
            }
        }
    }

    foreach ($c in $candidates) {
        if ($c -and (Test-Path -LiteralPath (Join-Path $c 'SurrounDead-Win64-Shipping.exe'))) {
            return (Resolve-Path -LiteralPath $c).Path
        }
    }
    return $null
}

if (-not $Win64) { $Win64 = Find-Win64 }
if (-not $Win64) {
    Write-Error (
        "Could not find the SurrounDead Win64 directory.`n" +
        "Set the SDB_GAME_WIN64 environment variable or pass -Win64 <path>."
    )
    exit 1
}

# ── Uninstall path ────────────────────────────────────────────────────────────

if ($Uninstall) {
    Write-Host "`nUninstalling SurrounDead Bridge …"

    # Remove mod folder
    $modRoot = Join-Path $Win64 'Mods\SurrounDeadBridge'
    if (Test-Path -LiteralPath $modRoot) {
        Remove-Item -LiteralPath $modRoot -Recurse -Force
        Write-Host "  Removed: $modRoot"
    }

    # Disable entry in mods.txt (don't delete the file – other mods may be listed)
    $modsTxt = Join-Path $Win64 'Mods\mods.txt'
    if (Test-Path -LiteralPath $modsTxt) {
        $lines = @(Get-Content -LiteralPath $modsTxt)
        $lines = @($lines | ForEach-Object {
            if ($_ -match '^\s*SurrounDeadBridge\s*:') { 'SurrounDeadBridge : 0' }
            else { $_ }
        })
        $lines | Set-Content -LiteralPath $modsTxt -Encoding ASCII
        Write-Host "  Disabled SurrounDeadBridge in mods.txt"
    }

    # Remove UE4SS files only if no other mods remain enabled
    $otherModsEnabled = $false
    if (Test-Path -LiteralPath $modsTxt) {
        $otherModsEnabled = (Get-Content -LiteralPath $modsTxt) |
            Where-Object { $_ -match ':\s*1' -and $_ -notmatch 'SurrounDeadBridge' } |
            Select-Object -First 1
    }
    if (-not $otherModsEnabled) {
        foreach ($f in 'dwmapi.dll', 'UE4SS.dll', 'UE4SS-settings.ini') {
            $p = Join-Path $Win64 $f
            if (Test-Path -LiteralPath $p) { Remove-Item -LiteralPath $p -Force; Write-Host "  Removed: $p" }
        }
    } else {
        Write-Host "  Kept UE4SS files (other mods are still enabled)"
    }

    # Clear saved env vars
    foreach ($v in 'SDB_JOIN_TICKET', 'SDB_GATEWAY_HOST', 'SDB_GATEWAY_PORT') {
        [System.Environment]::SetEnvironmentVariable($v, $null, 'User')
    }
    Write-Host "  Cleared SDB_* user environment variables"

    Write-Host "`nUninstall complete."
    exit 0
}

# ── Ticket ────────────────────────────────────────────────────────────────────

if (-not $Ticket) {
    Write-Host ""
    $Ticket = Read-Host "Enter your join ticket (provided by the server admin)"
}
if (-not $Ticket) {
    Write-Error "A join ticket is required.  Ask the server admin for one."
    exit 1
}

# ── Install UE4SS loader ──────────────────────────────────────────────────────

Write-Host "`nInstalling UE4SS mod loader …"
foreach ($name in 'dwmapi.dll', 'UE4SS.dll', 'UE4SS-settings.ini') {
    $src = Join-Path $BundleDir $name
    if (Test-Path -LiteralPath $src) {
        $dest = Join-Path $Win64 $name
        Copy-Item -LiteralPath $src -Destination $dest -Force
        Write-Host "  $dest"
    }
}

# ── Create mod directory structure ────────────────────────────────────────────

$modsRoot   = Join-Path $Win64 'Mods'
$modRoot    = Join-Path $modsRoot 'SurrounDeadBridge'
$dllsDir    = Join-Path $modRoot 'dlls'
$enabledTxt = Join-Path $modRoot 'enabled.txt'
$modsTxt    = Join-Path $modsRoot 'mods.txt'

New-Item -ItemType Directory -Force -Path $dllsDir | Out-Null
if (-not (Test-Path -LiteralPath $enabledTxt)) {
    New-Item -ItemType File -Force -Path $enabledTxt | Out-Null
}

# ── Install main.dll ──────────────────────────────────────────────────────────

Write-Host "`nInstalling SurrounDead Bridge mod …"
$srcDll  = Join-Path $BundleDir 'main.dll'
$destDll = Join-Path $dllsDir 'main.dll'
Copy-Item -LiteralPath $srcDll -Destination $destDll -Force
Write-Host "  $destDll"

# ── Update mods.txt ───────────────────────────────────────────────────────────

$lines = @()
if (Test-Path -LiteralPath $modsTxt) {
    $lines = @(Get-Content -LiteralPath $modsTxt)
}

# Disable conflicting SDO mods.
$disableNames = @('SurrounDeadOnlineCpp', 'SurrounDeadOnline')
$lines = @($lines | ForEach-Object {
    foreach ($n in $disableNames) {
        if ($_ -match "^\s*$([regex]::Escape($n))\s*:") { return "$n : 0" }
    }
    $_
})

# Enable ours.
$ourMod  = 'SurrounDeadBridge'
$matched = $false
$lines   = @($lines | ForEach-Object {
    if ($_ -match "^\s*$([regex]::Escape($ourMod))\s*:") { $matched = $true; "$ourMod : 1" }
    else { $_ }
})
if (-not $matched) { $lines += "$ourMod : 1" }
$lines | Set-Content -LiteralPath $modsTxt -Encoding ASCII

# ── Save connection settings as user environment variables ─────────────────────
#
# Setting them at User scope makes them visible to any process the current
# user starts, including Steam and the game.  No reboot required – just
# restart Steam after running this installer.

Write-Host "`nSaving connection settings …"

[System.Environment]::SetEnvironmentVariable('SDB_JOIN_TICKET', $Ticket, 'User')
Write-Host "  SDB_JOIN_TICKET = (set)"

if ($GatewayHost) {
    [System.Environment]::SetEnvironmentVariable('SDB_GATEWAY_HOST', $GatewayHost, 'User')
    Write-Host "  SDB_GATEWAY_HOST = $GatewayHost"
} else {
    # Remove any stale override so the DLL uses its built-in default (127.0.0.1).
    [System.Environment]::SetEnvironmentVariable('SDB_GATEWAY_HOST', $null, 'User')
}

if ($GatewayPort) {
    [System.Environment]::SetEnvironmentVariable('SDB_GATEWAY_PORT', $GatewayPort, 'User')
    Write-Host "  SDB_GATEWAY_PORT = $GatewayPort"
} else {
    [System.Environment]::SetEnvironmentVariable('SDB_GATEWAY_PORT', $null, 'User')
}

# ── Done ──────────────────────────────────────────────────────────────────────

Write-Host @"

Installation complete!

  Game path : $Win64
  Mod path  : $dllsDir\main.dll

IMPORTANT: Restart Steam before launching SurrounDead so it picks up the
saved connection settings.  If the game was already running, close it first.

To update your ticket later, re-run:
  .\install.ps1 -Ticket "eyJ..."

To uninstall:
  .\install.ps1 -Uninstall
"@
