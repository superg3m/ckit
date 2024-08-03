param (
    [Parameter(Mandatory=$false)]
    [string]$project_to_build = "all",

    [Parameter(Mandatory=$false)]
    [string]$is_debug = $true
)

. ./vars.ps1

if ($project_to_build -ne "all" -and $project_to_build -ne "ckg" -and $project_to_build -ne "ckit") {
    Write-Error "Invalid project to build. Expected: all, ckg, or ckit. Got: $project_to_build"
    exit
}

$exe_flags = "cl /nologo /fsanitize=address /WX /MP /W3 /FC /std:c11 /TC /wd4267"
$lib_flags = "cl /c /nologo /fsanitize=address /WX /MP /W3 /FC /std:c11 /TC /wd4267"
if ($is_debug) {
    $exe_flags += " /Zi /Od"
    $lib_flags += " /Zi /Od"
} else {
    $exe_flags += " /O2"
    $lib_flags += " /O2"
}

function Clean_Directory($directory) {
    Get-ChildItem -Path $directory -File -Recurse | Remove-Item -Force
}

function Build_Lib($output_name, $include_paths, $source_files, $buildDir) {
    $include_args = @()
    foreach ($include in $include_paths) {
        if ($include -is [System.Array]) {
            foreach ($inc in $include) {
                $include_args += "/I"
                $include_args += $inc
            }
        } else {
            $include_args += "/I"
            $include_args += $include
        }
    }
    $source_args = @()
    foreach ($source in $source_files) {
        $source_args += $source
    }
    $command = "$lib_flags $include_args $source_args"
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
        if ($include -is [System.Array]) {
            foreach ($inc in $include) {
                $include_args += "/I"
                $include_args += $inc
            }
        } else {
            $include_args += "/I"
            $include_args += $include
        }
    }
    $source_args = @()
    foreach ($source in $source_files) {
        $source_args += $source
    }
    $lib_args = @()
    foreach ($lib in $additional_libs) {
        $lib_args += $lib 
    }

    $command = "$exe_flags $include_args $source_args /link $lib_args"
    Write-Host "================================== $output_name ==================================" -ForegroundColor Green
    $timer = [Diagnostics.Stopwatch]::StartNew()
    Push-Location $buildDir
    try {
        Invoke-Expression $command
    } finally {
        Pop-Location
    }
    $timer.Stop()
    Write-Host "=========================== Build Time: $($timer.Elapsed.Seconds).$($timer.Elapsed.Milliseconds)s ===========================" -ForegroundColor Green
}

if ($project_to_build -eq "all" -or $project_to_build -eq "ckg") {
    $build_directory = "./ckg_build_cl"
    $test_directory = "./Tests/ckg_build_cl"
    Clean_Directory $build_directory

    Build_Lib "ckg" @("../Include/ckg") @("../Source/ckg/*.c") $build_directory
    Build_Exe "ckg_test" @("../../Include/ckg") @("../ckg/__test_ckg.c") @("../../ckg_build_cl/ckg.lib") $test_directory
}

if ($project_to_build -eq "all" -or $project_to_build -eq "ckit") {
    $build_directory = "./ckit_build_cl"
    Clean_Directory $build_directory

    Push-Location "./Include/ckit"
    $module_directories_to_include = Get-ChildItem -Recurse -Directory | ForEach-Object { $_.FullName }
    Pop-Location

    Write-Host "Directories to include:" -ForegroundColor Yellow
    $module_directories_to_include | ForEach-Object { Write-Host $_ -ForegroundColor Yellow }

    Build_Lib "ckit" @("../Include/ckg", "../Include/ckit", $module_directories_to_include) @("../ckg.c", "../ckit.c") $build_directory
    # Build_Lib "ckit" "../Include/ckit ../Include/ckg" "../Source/ckit/*.c" $build_directory
}
