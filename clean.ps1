Write-Host "running CKit clean.bat..." -ForegroundColor Green

Remove-Item -Path ".\build_cl\*", ".\build_gcc\*", ".\examples\cl\*", ".\examples\gcc\*" -Force -ErrorAction SilentlyContinue -Confirm:$false -Recurse