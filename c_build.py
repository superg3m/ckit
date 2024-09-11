import sys
sys.path.append("./c-build/new_stuff")
from new_project import *

from enum import Enum
class Arg(Enum):
	COMPILER_ARG = 2

COMPILER = sys.argv[Arg.COMPILER_ARG.value] if len(sys.argv) > 1 else "cl"
project = Project("CKIT", COMPILER)

# Do different things depending on the platform
if COMPILER == "cl":
	project.set_compiler_warning_level("")
	project.disable_specific_warnings(["5105", "4668", "4820"])
elif COMPILER in ["gcc", "cc", "clang"]:
	project.set_compiler_warning_level("all")

project.set_treat_warnings_as_errors(True)
project.set_debug_with_visual_studio(True)
project.set_rebuild_project_dependencies(True)
# If project.set_rebuild_project_dependencies is set to (False)
# then by default it will look at the source files and check if they have been modified since the cache

project.set_project_dependencies(["ckg"])
# -------------------------------------------------------------------------------------
lib_procedure = project.add_procedure(f"ckit_build_{COMPILER}")
lib_procedure.set_output_name("ckit.lib")
lib_procedure.set_compile_time_defines([""])
lib_procedure.set_include_paths([""])
lib_procedure.set_source_files(["../*.c"], False)
lib_procedure.set_additional_libs([f"../ckg/build_{COMPILER}"])
# -------------------------------------------------------------------------------------
# test_procedure = project.add_procedure("")
# test_procedure.set_output_name("test.exe")
# test_procedure.set_compile_time_defines([""])
# test_procedure.set_include_paths([""])
# test_procedure.set_source_files(["../*.c"], False)
# test_procedure.set_additional_libs([""])
# -------------------------------------------------------------------------------------

#project.set_executables_to_run(["test.exe"])
project.build()
