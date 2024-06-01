$executableFilePath = ".\examples\cl\CKit_Test.exe"

if (!(Test-Path -Path $executableFilePath)) {
    Write-Host "ERROR: Can't find exe, building..." -ForegroundColor Red
    & "build_example.bat"
    & "raddbg" $executableFilePath
} else {
    & "raddbg" $executableFilePath
}