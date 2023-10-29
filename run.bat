@echo off
if exist ./Build/Win32_EntryPoint.exe (
    start ./Build/Win32_EntryPoint.exe
) else (
    powershell -nologo -command "Write-Host 'ERROR: check if exe exists and re-build' -ForegroundColor Red"
)
