
Write-Host $clAvailable

& build.bat

if(!(Test-Path -Path ".\examples\cl")) {
    mkdir ".\examples\cl"
}

if(!(Test-Path -Path ".\examples\gcc")) {
    mkdir ".\examples\gcc"
}

Push-Location -Path ".\examples\cl"
& cl /Fe: ".\CKit_Test.exe" /Zi "..\*.c" "..\..\build_cl\CKit.lib"
Pop-Location


# pushd .\examples\gcc
# gcc "..\*.c" -o "CKit_Test.exe" -g -L"../../build_gcc" -lCKit
# ".\CKit_Test.exe"
# popd