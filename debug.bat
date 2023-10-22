@echo off
if not exist .\Build\Win64_EntryPoint.sln (
    devenv .\Build\Win64_EntryPoint.exe
) else (
    devenv .\Build\Win64_EntryPoint.sln
)
