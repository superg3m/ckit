./build.ps1

Push-Location ".\examples\cl"
cl /Fe: ".\ckit_Test.exe" /Zi "..\*.c" "..\..\build_cl\ckit.lib"
& "./ckit_Test.exe"
Pop-Location

# Push-Location .\examples\gcc
# gcc "..\*.c" -o "CKit_Test.exe" -g -L"../../build_gcc" -lCKit
# ".\CKit_Test.exe"
# Pop-Location