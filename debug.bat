@echo off

if not exist .\examples\CKit_Test.sln (
    if not exist .\examples\CKit_Test.exe (
        powershell -nologo -command "Write-Host 'ERROR: check if exe exists and re-build' -ForegroundColor Red"
    ) else (
        devenv .\examples\cl\CKit_Test.exe
    )
) else (
    devenv .\examples\cl\CKit_Test.sln
)
