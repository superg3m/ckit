# -------------------------------- GENERATED BY C_BUILD --------------------------------
import argparse
from c_build.new_stuff.globals import *
from c_build.new_stuff.new_project import *

parser = argparse.ArgumentParser(description='c_build_script')
parser.add_argument('--compiler', type=str, help='Compiler to use (e.g. gcc, clang)')
parser.add_argument('--build_type', type=str, help='Build type (e.g. debug, release)')
parser.add_argument('--depth', type=str, help='depth (e.g. 1, 2, 4)')
args = parser.parse_args()
build_type = args.build_type

if args.depth:
	SET_LEVEL(int(args.depth))
# --------------------------------------------------------------------------------------

COMPILER = args.compiler or "cl"
project = Project("ckit", COMPILER)

# Do different things depending on the platform
if COMPILER == "cl":
	project.set_compiler_warning_level("1")
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
ckg_lib_procedure = project.add_procedure(f"./build_{COMPILER}")
ckg_lib_procedure.set_output_name("ckit.lib")
ckg_lib_procedure.set_compile_time_defines([""])
ckg_lib_procedure.set_include_paths([""])
ckg_lib_procedure.set_source_files(["../ckit.c"])
ckg_lib_procedure.set_additional_libs([f"../../ckg/build_{COMPILER}/ckg.lib"])
# -------------------------------------------------------------------------------------
ckg_test_procedure = project.add_procedure(f"./Tests/CoreTest/build_{COMPILER}")
ckg_test_procedure.set_output_name("ckit_test.exe")
ckg_test_procedure.set_compile_time_defines([""])
ckg_test_procedure.set_include_paths([""])
ckg_test_procedure.set_source_files(["../*.c"])
ckg_test_procedure.set_additional_libs([f"../../../build_{COMPILER}/ckit.lib"])
# -------------------------------------------------------------------------------------
project.set_executables_to_run(["ckit_test.exe"])
project.build(build_type)
