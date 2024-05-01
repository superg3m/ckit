@echo off
setlocal enabledelayedexpansion

set "rootpath=C:\Users\Jovanni Djonaj\Documents\Coding\vscode\C++\CKit\"
set "tempfile=unique_errors.txt"

for /f "tokens=*" %%f in (compilation_errors.txt) do (
    set "line=%%f"
    if not "%%f" == "!line:%rootpath%=%!" (
        set "line=!line:%rootpath%=%!"
        echo !line! >> !tempfile!
    )
)

type !tempfile!
del !tempfile!