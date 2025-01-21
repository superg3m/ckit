# -------------------------------- GENERATED BY C_BUILD --------------------------------
from c_build.source.Utilities import *
from c_build.source.Manager import *
# --------------------------------------------------------------------------------------s

compiler_name = C_BUILD_COMPILER_NAME() if C_BUILD_IS_DEPENDENCY() else "cl"

if not IS_WINDOWS():
    compiler_name = "gcc"
	
compiler_warning_level = ""
compiler_disable_specific_warnings = []
compiler_treat_warnings_as_errors = True
compiler_disable_warnings = False

project_name = "ckit"
project_dependencies = ["ckg"]
project_rebuild_project_dependencies = True
project_debug_with_visual_studio = True
project_executable_procedures = ["ckit_test.exe"]

# Do different things depending on the platform
if compiler_name == "cl":
	compiler_warning_level = "4"
	compiler_disable_specific_warnings = ["5105", "4668", "4820", "4996"]
elif compiler_name in ["gcc", "cc", "clang"]:
	compiler_warning_level = "all"
	compiler_disable_specific_warnings = ["deprecated", "parentheses"]

executable_procedure_libs = [f"../../build_{compiler_name}/{C_BUILD_LIB('ckit', compiler_name)}"]

if IS_WINDOWS():
	windows_libs = ["User32.lib", "Gdi32.lib"] if compiler_name == "cl" else ["-lUser32", "-lGdi32"]
	executable_procedure_libs += windows_libs
	
procedures_config = {
    "ckit_lib": {
        "build_directory": f"./build_{compiler_name}",
        "output_name": C_BUILD_LIB('ckit', compiler_name),
        "source_files": ["../ckg/ckg.c", "../ckit.c"],
        "additional_libs": [],
        "compile_time_defines": ["CKIT_WSL"],
        "include_paths": [],
    },
    "ckit_core_test": {
        "build_directory": f"./Tests/build_{compiler_name}",
        "output_name": "ckit_test.exe",
        "source_files": ["../*.c"],
        "additional_libs": executable_procedure_libs,
        "compile_time_defines": ["CKIT_WSL"],
        "include_paths": [],
    },
}

compiler_config = {key: value for key, value in locals().items() if key.startswith('compiler_')}
project_config = {key: value for key, value in locals().items() if key.startswith('project_')}

manager: Manager = Manager(compiler_config, project_config, procedures_config)
manager.build_project()
# -------------------------------------------------------------------------------------
