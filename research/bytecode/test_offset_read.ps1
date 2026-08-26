$FlagDir = "$env:APPDATA\SurrounDeadBridge"
$DebugLog = "$FlagDir\debug.log"

function Get-NewLogText([string]$path, [long]$sinceOffset) {
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

$startOffset = (Get-Item $DebugLog).Length
Write-Output "startOffset=$startOffset"
[System.IO.File]::WriteAllText("$FlagDir\bytecode_dump.flag", "BP_PlayerCharacter_C`nToggleSprint`n")

for ($i=0; $i -lt 15; $i++) {
    Start-Sleep -Milliseconds 150
    $curLen = (Get-Item $DebugLog).Length
    $newText = Get-NewLogText -path $DebugLog -sinceOffset $startOffset
    $hasWrote = $newText -like "*wrote *bytes*"
    Write-Output "poll $i : curLen=$curLen newTextLen=$($newText.Length) hasWrote=$hasWrote"
    if ($hasWrote) {
        Write-Output "--- matched text (last 300 chars) ---"
        Write-Output $newText.Substring([Math]::Max(0,$newText.Length-300))
        break
    }
}
