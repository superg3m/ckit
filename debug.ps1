$executableFilePath = ".\examples\cl\ckit_test.exe"

if (!(Test-Path -Path $executableFilePath)) {
    Write-Host "ERROR: Can't find exe, building..." -ForegroundColor Red
    & "build_example.ps1"
    & "raddbg" $executableFilePath
} else {
    & "raddbg" $executableFilePath
}