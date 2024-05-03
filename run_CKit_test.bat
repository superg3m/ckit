pushd .\examples
cl /Zi /Fo /c ".\*.cpp"
link /out:".\CKit_Test.exe" ".\*.obj" "..\build\CKit.lib"
popd
".\examples\CKit_Test.exe"