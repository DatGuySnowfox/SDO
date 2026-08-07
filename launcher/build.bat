@echo off
cd /d "%~dp0"
echo Building SurrounDead Bridge Launcher...
dotnet publish SDBLauncher.csproj -c Release -r win-x64 --self-contained -o out --nologo
if %ERRORLEVEL% neq 0 ( echo Build failed. & pause & exit /b 1 )
echo.
echo Done: %~dp0out\SDBLauncher.exe
echo       (no .NET install required on player machines)
pause
