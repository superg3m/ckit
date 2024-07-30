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

$flags = "cl /c /nologo /fsanitize=address /WX /W3 /MP /FC /std:c11 /TC /wd4267"
if ($is_debug) {
    $flags += " /Zi /Od"
} else {
    $flags += " /O2"
}

function Build_Lib($output_name, $includePath, $sourceFile, $buildDir) {
    $command = "$flags /I $includePath $sourceFile"

	$lib_command = "lib /NOLOGO /OUT:$output_name.lib ./*.obj"

    Push-Location $buildDir
    try {
        Invoke-Expression $command
		Invoke-Expression $lib_command
    } finally {
        Pop-Location
    }
}

if ($project_to_build -eq "all" -or $project_to_build -eq "ckg") {
    Build_Lib "ckg" "../Include/ckg" "../ckg.c" "./ckg_build_cl"

}

if ($project_to_build -eq "all" -or $project_to_build -eq "ckit") {
   # Build_Lib "ckit" "../Include/ckit" "../ckit.c" "./ckit_build_cl"
}
