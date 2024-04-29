@echo off
:: Enable Visual Studio Vars if on amd system make sure you are in cmd
powershell -nologo -command "Write-Host 'running vars.bat...' -ForegroundColor Green"
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
:: Otherwise use this one to enable vars call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"