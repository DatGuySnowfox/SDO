# deploy.ps1 – Build main.dll and install it into Surroundead.
#
# Run from a VS 2022 x64 Native Tools Command Prompt (needed for cmake + cl.exe).
# Or just supply -SkipBuild if you already built.
#
# The DLL connects directly to the gateway server via TCP.
# Set these env vars before launching the game (or add them to UE4SS-settings.ini):
#   SDO_GATEWAY_HOST   gateway hostname or IP  (default: 127.0.0.1)
#   SDO_GATEWAY_PORT   gateway TCP port        (default: 42200)
#   SDO_JOIN_TICKET    HMAC-signed join ticket (required; obtain from server admin)
#   SDO_MOVE_INTERVAL_MS  movement send interval in ms (default: 50)
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
    if ($env:SDO_GAME_WIN64) { $candidates += $env:SDO_GAME_WIN64 }

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
        "Set the SDO_GAME_WIN64 environment variable or pass -Win64 <path>."
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
# No longer fatal: this script does not install UE4SS itself (see below), so a
# missing vendored bundle is irrelevant. It used to `exit 1` here, which made
# deploying the mod impossible whenever that path had moved or been cleaned up.
if (Test-Path -LiteralPath (Join-Path $UE4SSSrc 'UE4SS.dll')) {
    Write-Host "UE4SS bundle (informational only, not installed): $UE4SSSrc"
}

# ── Generate import library (only if not already present) ─────────────────────

$libPath = Join-Path $root 'vendor\ue4ss-stub\lib\UE4SS.lib'
if (-not (Test-Path -LiteralPath $libPath)) {
    Write-Host "`nGenerating UE4SS.lib …"
    & "$PSScriptRoot\gen_import_lib.ps1"
}

# ── Build ─────────────────────────────────────────────────────────────────────
#
# xmake is the maintained build path; CMake still exists but has drifted (it
# was missing the Shell32 link for a while, for instance). Build with xmake,
# and accept either tool's output location when locating the DLL — xmake emits
# build/out/main.dll, CMake's multi-config generators emit
# build/out/<Config>/main.dll.

$buildDir = Join-Path $root 'build'

if (-not $SkipBuild) {
    Write-Host "`nBuilding (xmake) …"
    Push-Location $root
    try {
        xmake build
        if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    } finally { Pop-Location }
}

$candidates = @(
    (Join-Path $buildDir 'out\main.dll'),           # xmake
    (Join-Path $buildDir 'out\Release\main.dll')    # CMake multi-config
)
$outputDll = $candidates | Where-Object { Test-Path -LiteralPath $_ } |
             Sort-Object { (Get-Item -LiteralPath $_).LastWriteTime } -Descending |
             Select-Object -First 1

if (-not $outputDll) {
    Write-Error ("Build output not found. Looked in:`n  " + ($candidates -join "`n  ") +
                 "`nRun without -SkipBuild first.")
    exit 1
}

# ── Locate the UE4SS install ──────────────────────────────────────────────────
#
# UE4SS changed its on-disk layout after v3.0.1: the loader (dwmapi.dll) still
# sits next to the game exe, but UE4SS.dll, the settings file and Mods/ all
# moved into a `ue4ss/` subdirectory. Detect which layout is present rather
# than assuming, so this script works against both.

$modernRoot = Join-Path $Win64 'ue4ss'
$IsModernLayout = Test-Path -LiteralPath (Join-Path $modernRoot 'UE4SS.dll')
$ue4ssRoot = if ($IsModernLayout) { $modernRoot } else { $Win64 }
Write-Host ("UE4SS layout: {0} ({1})" -f $(if ($IsModernLayout) { 'modern' } else { 'legacy v3.0.1' }), $ue4ssRoot)

# Deliberately does NOT copy UE4SS itself any more. The old behaviour
# overwrote UE4SS.dll/settings from a vendored bundle on every deploy, which
# silently reverted a hand-updated UE4SS (and its settings) back to the
# pinned copy — exactly the wrong thing while chasing engine-version support.
# Install/update UE4SS yourself; this script only deploys the mod.

# ── Create mod directory structure ────────────────────────────────────────────

$modsRoot  = Join-Path $ue4ssRoot 'Mods'
$modRoot   = Join-Path $modsRoot 'SDO'
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

# Ours is enabled via enabled.txt (created above), NOT via mods.txt.
#
# UE4SS runs both mechanisms in sequence: mods.txt load order first, then a
# sweep for enabled.txt in any mod folder not already started. Listing the mod
# in mods.txt as `: 1` *and* giving it an enabled.txt means it can be started
# twice, which is not something a C++ mod's global state survives. Force the
# mods.txt entry to 0 so exactly one mechanism is live.
$ourMod = 'SDO'
$lines = @($lines | ForEach-Object {
    if ($_ -match "^\s*$([regex]::Escape($ourMod))\s*:") { "$ourMod : 0" } else { $_ }
})

$lines | Set-Content -LiteralPath $modsTxt -Encoding ASCII
Write-Host "`nMods.txt updated:"
Get-Content -LiteralPath $modsTxt | ForEach-Object { Write-Host "  $_" }

Write-Host "`nDeploy complete.  Launch SurrounDead and check the UE4SS console."
