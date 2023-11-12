@echo off
powershell -nologo -command "Write-Host 'running build.bat...' -ForegroundColor Green"
if not exist .\Build (
    mkdir .\Build"
)
pushd .\Build
if NOT [%1] EQU [] (
    if [%1] EQU [-g++] (
        g++ -std=c++14 -Wformat=2 ../Source/*.cpp -o Win32_EntryPoint -luser32 -lGdi32
    ) else (
        powershell -nologo -command "Write-Host 'Wrong Command' -ForegroundColor Red"
    )
) else (
    cl /W2 /std:c++20 /Fe: Win32_EntryPoint.exe /Zi ..\Source\*.cpp user32.lib Gdi32.lib
)
popd