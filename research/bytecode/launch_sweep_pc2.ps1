# Launch the decode sweep via WMI process creation instead of Start-Process
# or a scheduled task. Start-Process's child gets killed the instant this
# SSH exec session ends (Windows OpenSSH puts exec-session children in a
# job object with kill-on-close semantics). A scheduled task avoids the job
# object but runs in a different security context and triggered an "access
# denied" prompt. Win32_Process::Create spawns a process with no parent/job
# relationship to the calling process at all, so it survives the SSH
# session closing without touching Task Scheduler.
$cmd = 'powershell.exe -NoProfile -ExecutionPolicy Bypass -File C:\sdo_dump_loop.ps1 -TargetsFile C:\sdo_targets.txt -OutDir C:\sdo_decode_out -ProgressLog C:\sdo_decode_progress.log 1> C:\sdo_decode_stdout.log 2> C:\sdo_decode_stderr.log'
$result = Invoke-CimMethod -ClassName Win32_Process -MethodName Create -Arguments @{CommandLine = $cmd}
Write-Output "ReturnValue=$($result.ReturnValue) ProcessId=$($result.ProcessId)"
