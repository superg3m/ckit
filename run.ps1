@echo off
$timer = [Diagnostics.Stopwatch]::new() # Create a timer
$timerAndStart = [Diagnostics.Stopwatch]::StartNew() # Create a timer and start it
$timer.Start() # Start the timer
$timer.Stop() # Stop the timer
$timer.Elapsed # Get time elapsed

call build.bat

if not exist .\examples\cl (
    mkdir .\examples\cl
)

if not exist .\examples\gcc (
    mkdir .\examples\gcc
)

echo "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
echo =========================== CL ===========================
echo "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
pushd .\examples\cl
cl /Fe: ".\CKit_Test.exe" /Zi "..\*.c" "..\..\build_cl\CKit.lib"
".\CKit_Test.exe"
popd


:: echo "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
:: echo =========================== GCC ==========================
:: echo "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
:: pushd .\examples\gcc
:: gcc "..\*.c" -o "CKit_Test.exe" -g -L"../../build_gcc" -lCKit
:: ".\CKit_Test.exe"
:: popd