# Runs LOCALLY on PC2 (no per-iteration ssh cost). Reads a tab-separated
# targets file (ClassName<TAB>FunctionName per line), and for each target:
#   1. Records debug.log's current byte length (so only NEW content counts).
#   2. Writes bytecode_dump.flag (2 lines: class, function).
#   3. Polls debug.log by reading from that fixed byte offset to current
#      EOF each iteration (NOT Get-Content -Tail N: this log is extremely
#      chatty — 08:18:41 alone had 10+ lines land within a single ms window
#      — so a fixed-line tail can blow past the completion line before the
#      next 200ms poll ever looks, causing false "timed out" skips even
#      though the dump itself completed in under a millisecond, confirmed
#      2026-08-17 against "AI Camera Activate": wrote its .bin at .618,
#      script still reported a timeout 10s later).
#   4. Moves the resulting .bin into $OutDir.
# Progress is written to stdout AND a log file so it can be tailed remotely.
param(
    [string]$TargetsFile = "C:\sdo_targets.txt",
    [string]$OutDir      = "C:\sdo_decode_out",
    [string]$ProgressLog = "C:\sdo_decode_progress.log"
)

$FlagDir = "$env:APPDATA\SurrounDeadBridge"
$DebugLog = "$FlagDir\debug.log"

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
if (Test-Path $ProgressLog) { Remove-Item $ProgressLog -Force }

function Write-Progress2($msg) {
    $line = "[$(Get-Date -Format 'HH:mm:ss')] $msg"
    Write-Output $line
    Add-Content -Path $ProgressLog -Value $line
}

function Get-NewLogText([string]$path, [long]$sinceOffset) {
    # Reads only the bytes appended to $path since $sinceOffset. Opens with
    # FileShare ReadWrite so it can read a file the game process still has
    # open for writing.
    if (-not (Test-Path $path)) { return "" }
    $fs = [System.IO.File]::Open($path, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::ReadWrite)
    try {
        $len = $fs.Length - $sinceOffset
        if ($len -le 0) { return "" }
        $fs.Seek($sinceOffset, [System.IO.SeekOrigin]::Begin) | Out-Null
        $buf = New-Object byte[] $len
        $fs.Read($buf, 0, $len) | Out-Null
        return [System.Text.Encoding]::UTF8.GetString($buf)
    } finally {
        $fs.Close()
    }
}

$targets = Get-Content $TargetsFile | Where-Object { $_ -and -not $_.StartsWith("#") }
$total = $targets.Count
$i = 0
$okCount = 0
$skipCount = 0

Write-Progress2 "=== Starting dump loop: $total targets ==="

foreach ($line in $targets) {
    $i++
    $parts = $line -split "`t"
    if ($parts.Count -lt 2) { continue }
    $className = $parts[0]
    $funcName  = $parts[1]
    $realBinName = "${className}_${funcName}.bin"  # what the mod actually names it (unsanitized, literal spaces included)

    Write-Progress2 "[$i/$total] $className`::$funcName ..."

    # Snapshot debug.log's current length BEFORE writing the flag, so the
    # poll below only ever looks at content appended after this point.
    $startOffset = 0
    if (Test-Path $DebugLog) { $startOffset = (Get-Item $DebugLog).Length }

    # Write the flag file (2 lines: class, function).
    $flagPath = "$FlagDir\bytecode_dump.flag"
    [System.IO.File]::WriteAllText($flagPath, "$className`n$funcName`n")

    # Poll local debug.log for the completion line — read everything from
    # $startOffset to current EOF each iteration (not just the last N
    # lines), so heavy interleaved logging can't push the target line out
    # of view before we see it.
    $deadline = (Get-Date).AddSeconds(10)
    $found = $false
    $notFound = $false
    $noInstance = $false
    while ((Get-Date) -lt $deadline) {
        Start-Sleep -Milliseconds 150
        $newText = Get-NewLogText -path $DebugLog -sinceOffset $startOffset
        if (-not $newText) { continue }
        if ($newText -like "*wrote *bytes to $realBinName*") { $found = $true; break }
        if ($newText -like "*$funcName*not found*") { $notFound = $true; break }
        # "<ClassName> instance/CDO not found" — no live instance exists right
        # now (e.g. no zombie/vehicle currently spawned nearby). Distinct from
        # the function-not-found case above; without this check the loop just
        # burns the full 10s timeout instead of failing fast.
        if ($newText -like "*$className instance/CDO not found*") { $noInstance = $true; break }
    }

    if ($noInstance) {
        Write-Progress2 "    SKIP: no live instance of $className right now"
        $skipCount++
        continue
    }
    if ($notFound) {
        Write-Progress2 "    SKIP: function not found on live class"
        $skipCount++
        continue
    }
    if (-not $found) {
        Write-Progress2 "    SKIP: timed out waiting for dump"
        $skipCount++
        continue
    }

    $srcBin = "$FlagDir\$realBinName"
    if (Test-Path $srcBin) {
        Move-Item -Force -Path $srcBin -Destination "$OutDir\$realBinName"
        Write-Progress2 "    OK -> $OutDir\$realBinName"
        $okCount++
    } else {
        Write-Progress2 "    SKIP: dump reported success but .bin not found at $srcBin"
        $skipCount++
    }
}

Write-Progress2 "=== Done: $okCount ok, $skipCount skipped, $total total ==="
