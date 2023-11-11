@echo off

if exist ./Build/Win32_EntryPoint.exe (
    if NOT [%1] EQU [] (
        if [%1] EQU [-debug] (
            call gdb "./Build/Win32_EntryPoint.exe"
        ) 
    ) else (
        call "./Build/Win32_EntryPoint.exe"
    )
) else (
    powershell -nologo -command "Write-Host 'ERROR: check if exe exists and re-build' -ForegroundColor Red"
)
