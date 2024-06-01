@echo off

SET "executable_file_path=.\examples\cl\CKit_Test.exe"

IF not exist "%executable_file_path%" (
    POWERSHELL -nologo -command "Write-Host 'ERROR: Can''t find exe, building...' -ForegroundColor Red"
    CALL build_example.bat
    START "" raddbg "%executable_file_path%"
) else (
    START "" raddbg "%executable_file_path%"
)
