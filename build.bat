@echo off

if not exist .\build_gcc (
    mkdir .\build_gcc
)

if not exist .\build_cl (
    mkdir .\build_cl
)

powershell -nologo -command "Write-Host 'running CKit build.bat...' -ForegroundColor Green"

if exist compilation_errors.txt (
	del compilation_errors.txt
)

pushd .\build_cl
:: MAKE SURE YOU HAVE AN OPTION FOR DEBUG LIBS
:: cl -DCUSTOM_PLATFORM_IMPL /std:c++20 /c "..\source\*.cpp"
cl /Zi /std:c99 /FC /c "..\source\*.c" >> "..\compilation_errors.txt" 2>nul
lib /OUT:".\CKit.lib" "User32.lib" ".\*.obj" > NUL
popd

pushd .\build_gcc

gcc -c -std=c99 -g "..\source\*.c"
ar rcs CKit.lib ".\*.o"

popd

call normalize_path.bat