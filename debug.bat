@echo off

set "executable_file_path=.\examples\cl\CKit_Test.exe"

if not exist "%executable_file_path%" (
    powershell -nologo -command "Write-Host 'ERROR: Can''t find exe, building...' -ForegroundColor Red"
    call build_example.bat
    raddbg "%executable_file_path%"
) else (
    raddbg "%executable_file_path%"
)
