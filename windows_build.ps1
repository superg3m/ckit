param(
	[Parameter(Mandatory=$false)]
	[string]$to_build = "all",

	[Parameter(Mandatory=$false)]
	[string]$is_debug = $false
)

if ($to_build -ne "all" -or $to_build -ne "ckg" -or $to_build -ne "ckit") {
	# panic
}

# Compile CKG
# vars
# cl $flags ./ckg.c OUTPUT:"./ckg_build_cl/ckg.lib" produce a lib

# Compile CKIT
# vars
# cl $flags ./ckit.c OUTPUT:"./ckit_build_cl/ckit.lib" link with ckg.lib produce a lib