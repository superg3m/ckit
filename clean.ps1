@echo off
powershell -nologo -command "Write-Host 'running CKit clean.bat...' -ForegroundColor Green"
del .\build_cl /q
del .\build_gcc /q

del .\examples\cl /q
del .\examples\gcc /q
