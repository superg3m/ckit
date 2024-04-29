@echo off
call clean_CKit.bat
powershell -nologo -command "Write-Host 'running CKit build.bat...' -ForegroundColor Green"
if not exist .\build (
    mkdir .\build
)
pushd .\build

:: MAKE SURE YOU HAVE AN OPTION FOR DEBUG LIBS
:: cl -DCUSTOM_PLATFORM_IMPL /std:c++20 /c "..\source\*.cpp"
cl /std:c++20 /c "..\source\*.cpp"
lib /OUT:"./CKit.lib" "./*.obj"

popd
cmd