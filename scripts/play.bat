@echo off
setlocal

set NODE=C:\Users\mccau\AppData\Local\Programs\SD-Online\runtime\node.exe
set SCRIPT=C:\temp\GIT\sd-online-bridge\scripts\play.js
set SERVER=C:\temp\GIT\sd-online-bridge\server\src\index.js
set LOG=C:\temp\sdb-server.log

:: Start server if not already running
netstat -an | findstr ":42200 " | findstr LISTEN >nul 2>&1
if %errorlevel% neq 0 (
    echo Starting SDB server...
    start /B "%NODE%" "%SERVER%" > "%LOG%" 2>&1
    timeout /t 3 /nobreak >nul
)

:: Get ticket and launch game
"%NODE%" "%SCRIPT%" %1

endlocal
