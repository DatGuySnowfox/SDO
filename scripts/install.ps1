# install.ps1 - End-user installer for SurrounDead Online
#
# Installs the UE4SS mod loader and the SurrounDead Online client into the game.
# Run this once; then launch SurrounDead normally through Steam.
#
# The simplest usage - extract the release zip and run:
#   .\install.ps1 -Ticket "eyJ..."
#
# All parameters are optional except -Ticket (or answer the prompt):
#   -Ticket         Join ticket provided by the server admin
#   -GatewayHost    Gateway server hostname/IP (default: 127.0.0.1)
#   -GatewayPort    Gateway TCP port           (default: 42200)
#   -Win64          Override auto-detected game Binaries\Win64 path
#   -BundleDir      Folder containing UE4SS files + main.dll
#                   (default: same folder as this script)
#   -Uninstall      Remove all SDO files and saved settings

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

    if ($env:SDO_GAME_WIN64) { $candidates += $env:SDO_GAME_WIN64 }

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
        "Set the SDO_GAME_WIN64 environment variable or pass -Win64 <path>."
    )
    exit 1
}

# ── Resolve the UE4SS layout ──────────────────────────────────────────────────
#
# UE4SS changed its on-disk layout after v3.0.1. The loader (dwmapi.dll) still
# sits next to the game exe in both, but UE4SS.dll, the settings file and Mods/
# moved into a `ue4ss/` subdirectory. Installing into the wrong one fails
# silently - UE4SS loads and the mod simply never appears - so work out which
# applies instead of assuming, the same way scripts/deploy.ps1 does.
#
# Precedence: an existing install in the game directory wins, because matching
# whatever UE4SS is already there is always right. Otherwise mirror the shape
# of the bundle being installed from.

function Resolve-Ue4ssRoot {
    param([string]$Win64, [string]$BundleDir)

    if (Test-Path -LiteralPath (Join-Path $Win64 'ue4ss\UE4SS.dll')) {
        return @{ Root = (Join-Path $Win64 'ue4ss'); Modern = $true;  Source = 'existing install' }
    }
    if (Test-Path -LiteralPath (Join-Path $Win64 'UE4SS.dll')) {
        return @{ Root = $Win64; Modern = $false; Source = 'existing install' }
    }
    if ($BundleDir -and (Test-Path -LiteralPath (Join-Path $BundleDir 'ue4ss\UE4SS.dll'))) {
        return @{ Root = (Join-Path $Win64 'ue4ss'); Modern = $true;  Source = 'bundle layout' }
    }
    return @{ Root = $Win64; Modern = $false; Source = 'legacy default' }
}

$layout    = Resolve-Ue4ssRoot -Win64 $Win64 -BundleDir $BundleDir
$ue4ssRoot = $layout.Root
$layoutName = if ($layout.Modern) { 'modern' } else { 'legacy v3.0.1' }
Write-Host ("UE4SS layout: {0} ({1}) - {2}" -f $layoutName, $ue4ssRoot, $layout.Source)

# ── Uninstall path ────────────────────────────────────────────────────────────

if ($Uninstall) {
    Write-Host "`nUninstalling SurrounDead Online ..."

    # Remove mod folder
    $modRoot = Join-Path $ue4ssRoot 'Mods\SDO'
    if (Test-Path -LiteralPath $modRoot) {
        Remove-Item -LiteralPath $modRoot -Recurse -Force
        Write-Host "  Removed: $modRoot"
    }

    # Disable entry in mods.txt (don't delete the file - other mods may be listed)
    $modsTxt = Join-Path $ue4ssRoot 'Mods\mods.txt'
    if (Test-Path -LiteralPath $modsTxt) {
        $lines = @(Get-Content -LiteralPath $modsTxt)
        $lines = @($lines | ForEach-Object {
            if ($_ -match '^\s*SDO\s*:') { 'SDO : 0' }
            else { $_ }
        })
        $lines | Set-Content -LiteralPath $modsTxt -Encoding ASCII
        Write-Host "  Disabled SDO in mods.txt"
    }

    # Remove UE4SS files only if no other mods remain enabled
    $otherModsEnabled = $false
    if (Test-Path -LiteralPath $modsTxt) {
        $otherModsEnabled = (Get-Content -LiteralPath $modsTxt) |
            Where-Object { $_ -match ':\s*1' -and $_ -notmatch 'SDO' } |
            Select-Object -First 1
    }
    if (-not $otherModsEnabled) {
        # dwmapi.dll is the loader and sits beside the game exe in both
        # layouts; UE4SS.dll and its settings live under the UE4SS root, which
        # is the ue4ss/ subdirectory on anything newer than v3.0.1.
        $targets = @((Join-Path $Win64 'dwmapi.dll'),
                     (Join-Path $ue4ssRoot 'UE4SS.dll'),
                     (Join-Path $ue4ssRoot 'UE4SS-settings.ini'))
        foreach ($p in $targets) {
            if (Test-Path -LiteralPath $p) { Remove-Item -LiteralPath $p -Force; Write-Host "  Removed: $p" }
        }
        # On the modern layout the ue4ss/ directory is ours to clean up once
        # nothing is left in it. Leave it alone if anything remains.
        if ($layout.Modern -and (Test-Path -LiteralPath $ue4ssRoot) -and
            -not (Get-ChildItem -LiteralPath $ue4ssRoot -Force)) {
            Remove-Item -LiteralPath $ue4ssRoot -Force
            Write-Host "  Removed: $ue4ssRoot"
        }
    } else {
        Write-Host "  Kept UE4SS files (other mods are still enabled)"
    }

    # Clear saved env vars
    foreach ($v in 'SDO_JOIN_TICKET', 'SDO_GATEWAY_HOST', 'SDO_GATEWAY_PORT') {
        [System.Environment]::SetEnvironmentVariable($v, $null, 'User')
    }
    Write-Host "  Cleared SDO_* user environment variables"

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

Write-Host "`nInstalling UE4SS mod loader ..."
if ($layout.Modern) { New-Item -ItemType Directory -Force -Path $ue4ssRoot | Out-Null }
foreach ($name in 'dwmapi.dll', 'UE4SS.dll', 'UE4SS-settings.ini') {
    # The bundle may itself be either layout, so look in both places for each
    # file rather than assuming where it sits inside the bundle.
    $src = @((Join-Path $BundleDir $name), (Join-Path $BundleDir "ue4ss\$name")) |
           Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
    if (-not $src) { continue }

    # dwmapi.dll is the loader and must stay beside the game exe in both
    # layouts; everything else belongs under the UE4SS root.
    $dest = if ($name -eq 'dwmapi.dll') { Join-Path $Win64 $name }
            else                        { Join-Path $ue4ssRoot $name }
    Copy-Item -LiteralPath $src -Destination $dest -Force
    Write-Host "  $dest"
}

# ── Create mod directory structure ────────────────────────────────────────────

$modsRoot   = Join-Path $ue4ssRoot 'Mods'
$modRoot    = Join-Path $modsRoot 'SDO'
$dllsDir    = Join-Path $modRoot 'dlls'
$enabledTxt = Join-Path $modRoot 'enabled.txt'
$modsTxt    = Join-Path $modsRoot 'mods.txt'

New-Item -ItemType Directory -Force -Path $dllsDir | Out-Null
if (-not (Test-Path -LiteralPath $enabledTxt)) {
    New-Item -ItemType File -Force -Path $enabledTxt | Out-Null
}

# ── Install main.dll ──────────────────────────────────────────────────────────

Write-Host "`nInstalling SurrounDead Online mod ..."
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

# Ours is enabled by the enabled.txt created above, NOT by mods.txt.
#
# UE4SS runs both mechanisms in sequence: mods.txt load order first, then a
# sweep for enabled.txt in any mod folder not already started. Listing the mod
# as `: 1` here *and* giving it an enabled.txt starts it twice, which a C++
# mod's global state does not survive. Force the mods.txt entry to 0 so exactly
# one mechanism is live - scripts/deploy.ps1 was fixed for this same bug.
$ourMod = 'SDO'
$lines  = @($lines | ForEach-Object {
    if ($_ -match "^\s*$([regex]::Escape($ourMod))\s*:") { "$ourMod : 0" } else { $_ }
})
$lines | Set-Content -LiteralPath $modsTxt -Encoding ASCII

# ── Save connection settings as user environment variables ─────────────────────
#
# Setting them at User scope makes them visible to any process the current
# user starts, including Steam and the game.  No reboot required - just
# restart Steam after running this installer.

Write-Host "`nSaving connection settings ..."

[System.Environment]::SetEnvironmentVariable('SDO_JOIN_TICKET', $Ticket, 'User')
Write-Host "  SDO_JOIN_TICKET = (set)"

if ($GatewayHost) {
    [System.Environment]::SetEnvironmentVariable('SDO_GATEWAY_HOST', $GatewayHost, 'User')
    Write-Host "  SDO_GATEWAY_HOST = $GatewayHost"
} else {
    # Remove any stale override so the DLL uses its built-in default (127.0.0.1).
    [System.Environment]::SetEnvironmentVariable('SDO_GATEWAY_HOST', $null, 'User')
}

if ($GatewayPort) {
    [System.Environment]::SetEnvironmentVariable('SDO_GATEWAY_PORT', $GatewayPort, 'User')
    Write-Host "  SDO_GATEWAY_PORT = $GatewayPort"
} else {
    [System.Environment]::SetEnvironmentVariable('SDO_GATEWAY_PORT', $null, 'User')
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
