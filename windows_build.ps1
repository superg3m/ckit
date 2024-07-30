param(
	[Parameter(Mandatory=$false)]
	[string]$project_to_build = "all",

	[Parameter(Mandatory=$false)]
	[string]$is_debug = $false
)

if ($project_to_build -ne "all" -or $project_to_build -ne "ckg" -or $project_to_build -ne "ckit") {
	# Message
	# Error Exit
}

# Compile CKG
# vars
# cl $flags ./ckg.c OUTPUT:"./ckg_build_cl/ckg.lib" produce a lib

# Compile CKIT
# vars
# cl $flags ./ckit.c OUTPUT:"./ckit_build_cl/ckit.lib" link with ckg.lib produce a lib