param (
    [Parameter(Mandatory=$false)]
    [string]$project_to_build = "all",

    [Parameter(Mandatory=$false)]
    [string]$is_debug = $false
)

. ./vars.ps1

if ($project_to_build -ne "all" -and $project_to_build -ne "ckg" -and $project_to_build -ne "ckit") {
    Write-Error "Invalid project to build. Expected: all, ckg, or ckit. Got: $project_to_build"
    exit
}

# Define common flags
$flags = "cl /c /nologo /fsanitize=address /WX /MP /W3 /FC /std:c11 /TC /wd4267"
if ($is_debug) {
    $flags += " /Zi /Od"
} else {
    $flags += " /O2"
}

function Clean_Directory($directory) {
    Get-ChildItem -Path $directory -File -Recurse | Remove-Item -Force
}

function Build_Lib($output_name, [string[]]$include_paths, [string[]]$source_files, $buildDir) {
    $include_args = @()
    foreach ($include in $include_paths) {
        $include_args += "/I " 
        $include_args += $include
    }
    $source_args = @()
    foreach ($source in $source_files) {
        $source_args += $source
    }
    $command = "$flags $include_args $source_args"
    $lib_command = "lib /NOLOGO /OUT:$output_name.lib ./*.obj"

    Write-Host "================================== $output_name ==================================" -ForegroundColor Green
    $timer = [Diagnostics.Stopwatch]::StartNew()
    Push-Location $buildDir
    try {
        Invoke-Expression $command
        Invoke-Expression $lib_command
    } finally {
        Pop-Location
    }
    $timer.Stop()
    Write-Host "=========================== Build Time: $($timer.Elapsed.Seconds).$($timer.Elapsed.Milliseconds)s ===========================" -ForegroundColor Green
}

function Build_Exe($output_name, [string[]]$include_paths, [string[]]$source_files, [string[]]$additional_libs, $buildDir) {
    $include_args = @()
    foreach ($include in $include_paths) {
        $include_args += "/I " 
        $include_args += $include
    }
    $source_args = @()
    foreach ($source in $source_files) {
        $source_args += $source
    }
    $command = "$flags $include_args $source_args"
    $lib_command = "lib /NOLOGO /OUT:$output_name.lib ./*.obj"

    Write-Host "================================== $output_name ==================================" -ForegroundColor Green
    $timer = [Diagnostics.Stopwatch]::StartNew()
    Push-Location $buildDir
    try {
        Invoke-Expression $command
        Invoke-Expression $lib_command
    } finally {
        Pop-Location
    }
    $timer.Stop()
    Write-Host "=========================== Build Time: $($timer.Elapsed.Seconds).$($timer.Elapsed.Milliseconds)s ===========================" -ForegroundColor Green
}

if ($project_to_build -eq "all" -or $project_to_build -eq "ckg") {
    $build_directory = "./ckg_build_cl"
    Clean_Directory $build_directory

    # Build_Lib "ckg" @("../Include/ckg") @("../ckg.c") $build_directory
    Build_Lib "ckg" @("../Include/ckg") @("../Source/ckg/*.c") $build_directory
    Build_Exe "ckg_test" @("../Include/ckg") @("../Source/ckg/*.c") $build_directory
}

if ($project_to_build -eq "all" -or $project_to_build -eq "ckit") {
    $build_directory = "./ckit_build_cl"
    Clean_Directory $build_directory

    #Build_Lib "ckit" @("../Include/ckit", "../Include/ckg") @("../ckit.c", "../ckg.c") $build_directory
    # Build_Lib "ckit" "../Include/ckit ../Include/ckg" "../Source/ckit/*.c" $build_directory
}
