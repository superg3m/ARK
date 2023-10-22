@echo off
if exist ./Build/Win64_EntryPoint.exe (
    start ./Build/Win64_EntryPoint.exe
) else (
    powershell -nologo -command "Write-Host 'ERROR: check if exe exists and re-build' -ForegroundColor Red"
)
