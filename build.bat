@echo off
powershell -nologo -command "Write-Host 'running build.bat...' -ForegroundColor Green"
if not exist .\Build (
    mkdir .\Build"
)
pushd .\Build
cl /Zi ..\Source\*.cpp user32.lib Gdi32.lib
popd