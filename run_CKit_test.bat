call build_CKit_Lib.bat

pushd .\examples
cl  /Fe: ".\CKit_Test.exe" /Zi ".\*.cpp" "..\build\CKit.lib"
popd
".\examples\CKit_Test.exe"