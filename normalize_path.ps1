Write-Host "running normalize_path.ps1..." -ForegroundColor Green

$rootpath = (Get-Location).Path
$tempfile = "compilation_errors_temp.txt"

# Create a new, empty temporary file
New-Item -ItemType File -Path $tempfile -Force | Out-Null

# Read each line from compilation_errors.txt
Get-Content -Path "compilation_errors.txt" | ForEach-Object {
    $line = $_

    # Replace rootpath with nothing in the line
    $line = $line -Replace [Regex]::Escape($rootpath), ""

    # Write the modified line to the temporary file
    Add-Content -Path $tempfile -Value $line
}

# Move the temporary file to compilation_errors.txt
Move-Item -Path $tempfile -Destination "compilation_errors.txt" -Force

# Display the contents of compilation_errors.txt
Get-Content -Path "compilation_errors.txt"

# Delete compilation_errors.txt
Remove-Item -Path "compilation_errors.txt"