@echo off
if not exist .\Build\Win32_EntryPoint.sln (
    if not exist .\Build\Win32_EntryPoint.exe (
        powershell -nologo -command "Write-Host 'ERROR: check if exe exists and re-build' -ForegroundColor Red"
    ) else (
        devenv .\Build\Win32_EntryPoint.exe
    )
) else (
    devenv .\Build\Win32_EntryPoint.sln
)
