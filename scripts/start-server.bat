@echo off
setlocal

set NODE=C:\Users\mccau\AppData\Local\Programs\SD-Online\runtime\node.exe
set SERVER=C:\temp\GIT\SDO\server\src\index.js
set LOG=C:\temp\sdb-server.log

:: Check if server is already running
netstat -an | findstr ":42200 " | findstr LISTEN >nul 2>&1
if %errorlevel%==0 (
    echo Server already running on port 42200.
) else (
    echo Starting SDB server...
    start /B "%NODE%" "%SERVER%" > "%LOG%" 2>&1
    timeout /t 2 /nobreak >nul
    echo Server started. Log: %LOG%
)
