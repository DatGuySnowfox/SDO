# deploy.ps1 – Build main.dll and install it into Surroundead.
#
# Run from a VS 2022 x64 Native Tools Command Prompt (needed for cmake + cl.exe).
# Or just supply -SkipBuild if you already built.
#
# The DLL connects directly to the gateway server via TCP.
# Set these env vars before launching the game (or add them to UE4SS-settings.ini):
#   SDB_GATEWAY_HOST   gateway hostname or IP  (default: 127.0.0.1)
#   SDB_GATEWAY_PORT   gateway TCP port        (default: 42200)
#   SDB_JOIN_TICKET    HMAC-signed join ticket (required; obtain from server admin)
#   SDB_MOVE_INTERVAL_MS  movement send interval in ms (default: 50)
#
# Usage:
#   .\scripts\deploy.ps1
#   .\scripts\deploy.ps1 -Win64 "C:\...\SurrounDead\Binaries\Win64"
#   .\scripts\deploy.ps1 -SkipBuild      # deploy last build without rebuilding

param(
    [string]$Win64     = '',   # override auto-detected game path
    [string]$UE4SSSrc  = '',   # override path to source UE4SS bundle dir
    [switch]$SkipBuild = $false
)

$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent

# ── Locate game Win64 ─────────────────────────────────────────────────────────

function Find-Win64 {
    $candidates = @()

    # 1. Env var shortcut
    if ($env:SDB_GAME_WIN64) { $candidates += $env:SDB_GAME_WIN64 }

    # 2. SDO launcher settings (if SDO is installed)
    $launcherSettings = Join-Path $env:LOCALAPPDATA 'SurrounDeadOnline\launcher-settings.json'
    if (Test-Path -LiteralPath $launcherSettings) {
        try {
            $s = Get-Content -Raw -LiteralPath $launcherSettings | ConvertFrom-Json
            if ($s.gameWin64Path) { $candidates += [string]$s.gameWin64Path }
        } catch {}
    }

    # 3. Steam registry → default library
    $steamReg = Get-ItemProperty -Path 'HKCU:\Software\Valve\Steam' -ErrorAction SilentlyContinue
    if ($steamReg) {
        $steamPath = $steamReg.SteamPath
        $candidates += Join-Path $steamPath 'steamapps\common\SurrounDead\SurrounDead\Binaries\Win64'
        $vdf = Join-Path $steamPath 'steamapps\libraryfolders.vdf'
        if (Test-Path -LiteralPath $vdf) {
            [regex]::Matches((Get-Content -Raw $vdf), '"path"\s+"([^"]+)"') |
            ForEach-Object {
                $candidates += Join-Path $_.Groups[1].Value.Replace('\\','\') `
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
        "Could not find SurrounDead Win64 directory.`n" +
        "Set the SDB_GAME_WIN64 environment variable or pass -Win64 <path>."
    )
    exit 1
}
Write-Host "Game Win64: $Win64"

# ── Locate UE4SS bundle (dwmapi.dll + UE4SS.dll + settings) ──────────────────

if (-not $UE4SSSrc) {
    # Default: extracted SD-Online package sitting next to this repo
    $UE4SSSrc = Join-Path (Split-Path $root -Parent) `
        'sd-online-inspect\payload\bridge-runtime\ue4ss'
}
if (-not (Test-Path -LiteralPath (Join-Path $UE4SSSrc 'UE4SS.dll'))) {
    Write-Error (
        "UE4SS.dll not found at: $UE4SSSrc`n" +
        "Extract SD-Online beside this repo or pass -UE4SSSrc <path>."
    )
    exit 1
}
Write-Host "UE4SS bundle: $UE4SSSrc"

# ── Generate import library (only if not already present) ─────────────────────

$libPath = Join-Path $root 'vendor\ue4ss-stub\lib\UE4SS.lib'
if (-not (Test-Path -LiteralPath $libPath)) {
    Write-Host "`nGenerating UE4SS.lib …"
    & "$PSScriptRoot\gen_import_lib.ps1"
}

# ── CMake build ───────────────────────────────────────────────────────────────

$buildDir  = Join-Path $root 'build'
$outputDll = Join-Path $buildDir 'out\Release\main.dll'

if (-not $SkipBuild) {
    Write-Host "`nConfiguring …"
    $ue4ssDll = Join-Path $UE4SSSrc 'UE4SS.dll'
    cmake -B $buildDir `
        -DCMAKE_BUILD_TYPE=Release `
        "-DSDB_UE4SS_DLL=$ue4ssDll"
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    Write-Host "`nBuilding …"
    cmake --build $buildDir --config Release
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

if (-not (Test-Path -LiteralPath $outputDll)) {
    Write-Error "Build output not found: $outputDll`nRun without -SkipBuild first."
    exit 1
}

# ── Install UE4SS files into Win64 ────────────────────────────────────────────

Write-Host "`nInstalling UE4SS loader …"
foreach ($name in 'dwmapi.dll', 'UE4SS.dll', 'UE4SS-settings.ini') {
    $src  = Join-Path $UE4SSSrc $name
    $dest = Join-Path $Win64 $name
    if (Test-Path -LiteralPath $src) {
        Copy-Item -LiteralPath $src -Destination $dest -Force
        Write-Host "  $dest"
    }
}

# ── Create mod directory structure ────────────────────────────────────────────

$modsRoot  = Join-Path $Win64 'Mods'
$modRoot   = Join-Path $modsRoot 'SurrounDeadBridge'
$dllsDir   = Join-Path $modRoot 'dlls'
$enabledTxt= Join-Path $modRoot 'enabled.txt'
$modsTxt   = Join-Path $modsRoot 'mods.txt'

New-Item -ItemType Directory -Force -Path $dllsDir | Out-Null
if (-not (Test-Path -LiteralPath $enabledTxt)) {
    New-Item -ItemType File -Force -Path $enabledTxt | Out-Null
}

# ── Copy main.dll ─────────────────────────────────────────────────────────────

Write-Host "`nInstalling main.dll …"
Copy-Item -LiteralPath $outputDll -Destination (Join-Path $dllsDir 'main.dll') -Force
Write-Host "  $dllsDir\main.dll"

# ── Update mods.txt ───────────────────────────────────────────────────────────

$lines = @()
if (Test-Path -LiteralPath $modsTxt) {
    $lines = @(Get-Content -LiteralPath $modsTxt)
}

# Disable the old SDO mods (they conflict with ours).
$disableNames = @('SurrounDeadOnlineCpp', 'SurrounDeadOnline')
$lines = @($lines | ForEach-Object {
    foreach ($n in $disableNames) {
        if ($_ -match "^\s*$([regex]::Escape($n))\s*:") { return "$n : 0" }
    }
    $_
})

# Enable ours.
$ourMod = 'SurrounDeadBridge'
$matched = $false
$lines = @($lines | ForEach-Object {
    if ($_ -match "^\s*$([regex]::Escape($ourMod))\s*:") { $matched = $true; "$ourMod : 1" }
    else { $_ }
})
if (-not $matched) { $lines += "$ourMod : 1" }

$lines | Set-Content -LiteralPath $modsTxt -Encoding ASCII
Write-Host "`nMods.txt updated:"
Get-Content -LiteralPath $modsTxt | ForEach-Object { Write-Host "  $_" }

Write-Host "`nDeploy complete.  Launch SurrounDead and check the UE4SS console."
