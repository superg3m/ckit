@echo off
call clean_CKit.bat
powershell -nologo -command "Write-Host 'running build.bat...' -ForegroundColor Green"
if not exist .\build (
    mkdir .\build
)
pushd .\build
cl /std:c++20 /c "..\source\*.cpp"
:: cl -DCUSTOM_PLATFORM_IMPL /std:c++20 /c "..\source\*.cpp"
lib /OUT:"./CKit.lib" "./*.obj"


popd