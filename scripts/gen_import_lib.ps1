# gen_import_lib.ps1 – run once from a Visual Studio Developer PowerShell.
#
# Generates vendor/ue4ss-stub/lib/UE4SS.lib from the .def file so that
# CMake's find_library(UE4SS_LIB ...) resolves at configure time.
#
# Usage (from repo root):
#   .\scripts\gen_import_lib.ps1
#
# Requires: lib.exe on PATH (open "x64 Native Tools Command Prompt for VS 20xx")

$ErrorActionPreference = "Stop"

$root   = Split-Path $PSScriptRoot -Parent
$def    = Join-Path $root "vendor\ue4ss-stub\UE4SS.def"
$outDir = Join-Path $root "vendor\ue4ss-stub\lib"
$out    = Join-Path $outDir "UE4SS.lib"

if (-not (Test-Path $def)) {
    Write-Error "DEF file not found: $def"
    exit 1
}

New-Item -ItemType Directory -Force -Path $outDir | Out-Null

Write-Host "Generating $out ..."
lib.exe /def:"$def" /out:"$out" /machine:x64 /nologo

if ($LASTEXITCODE -ne 0) {
    Write-Error "lib.exe failed (exit $LASTEXITCODE)"
    exit $LASTEXITCODE
}

Write-Host "Done: $out"
