@echo off

powershell -nologo -command "Write-Host 'running normalize_path.bat...' -ForegroundColor Green"

setlocal enabledelayedexpansion

set "rootpath=%~dp0"
set "tempfile=compilation_errors_temp.txt"

rem Create a new, empty temporary file
type nul > %tempfile%

rem Read each line from compilation_errors.txt
for /f "tokens=*" %%f in (compilation_errors.txt) do (
    set "line=%%f"
    
    rem Replace rootpath with nothing in the line
    set "line=!line:%rootpath%=!"

    rem Write the modified line to the temporary file
    echo !line! >> !tempfile!
)

move /y %tempfile% compilation_errors.txt > nul
type compilation_errors.txt
del compilation_errors.txt

endlocal
