
Write-Host $clAvailable

./build.ps1

if(!(Test-Path -Path ".\examples\cl")) {
    mkdir ".\examples\cl"
}

if(!(Test-Path -Path ".\examples\gcc")) {
    mkdir ".\examples\gcc"
}

Push-Location -Path ".\examples\cl"
cl /Fe: ".\ckit_test.exe" /Zi "..\*.c" "..\..\build_cl\ckit.lib"
Pop-Location


# pushd .\examples\gcc
# gcc "..\*.c" -o "CKit_Test.exe" -g -L"../../build_gcc" -lCKit
# ".\CKit_Test.exe"
# popd