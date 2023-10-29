@echo off
if not exist .\Build\Win32_EntryPoint.sln (
    devenv .\Build\Win32_EntryPoint.exe
) else (
    devenv .\Build\Win32_EntryPoint.sln
)
