@echo off
powershell -nologo -command "Write-Host 'running build.bat...' -ForegroundColor Green"
if not exist .\Build (
    mkdir .\Build"
)
pushd .\Build
cl /W2 /std:c++20 /Fe: Win32_EntryPoint.exe /Zi ..\Source\*.cpp user32.lib Gdi32.lib
:: g++ -std=c++14 ../Source/*.cpp -o Win32_EntryPoint -luser32 -lGdi32
popd