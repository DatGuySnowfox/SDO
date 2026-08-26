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

$targets = @(
    @{cls="BP_PlayerCharacter_C"; fn="ToggleSprint"},
    @{cls="BP_PlayerCharacter_C"; fn="SetFirstPersonView"},
    @{cls="BP_PlayerCharacter_C"; fn="BindEvent_SetFirstPersonView"}
)

foreach ($t in $targets) {
    $className = $t.cls
    $funcName = $t.fn
    $realBinName = "${className}_${funcName}.bin"
    $startOffset = (Get-Item $DebugLog).Length
    Write-Output ">>> target $funcName startOffset=$startOffset"
    [System.IO.File]::WriteAllText("$FlagDir\bytecode_dump.flag", "$className`n$funcName`n")
    $deadline = (Get-Date).AddSeconds(5)
    $found = $false
    $notFound = $false
    $polls = 0
    while ((Get-Date) -lt $deadline) {
        Start-Sleep -Milliseconds 150
        $polls++
        $newText = Get-NewLogText -path $DebugLog -sinceOffset $startOffset
        if (-not $newText) { continue }
        if ($newText -like "*wrote *bytes to $realBinName*") { $found = $true; break }
        if ($newText -like "*$funcName*not found*" -or $newText -like "*$funcName* function not found*") { $notFound = $true; break }
    }
    Write-Output "    polls=$polls found=$found notFound=$notFound newTextLen=$($newText.Length)"
}
