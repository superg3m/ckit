
project = project_create("CKIT")
project_set_github_root(project, "https://github.com/superg3m")
project_set_compiler(project, "cl")
project_set_std_version(project, "c11")

-- Do different things depending on the platform
if (PLATFORM_WINDOW) then
	project_set_compiler_warning_level(project, "w4") -- w0 to disable warnings  (not sure if that is accurate but just putting this here)
elseif (PLATFORM_LINUX) then
	project_set_compiler_warning_level(project, "wall") -- w0 to disable warnings (not sure if that is accurate but just putting this here)
end()

project_disable_specific_warnings(project, ["5105", "4668", "4820"])
project_set_treat_warnings_as_errors(project, true)
project_set_debug_with_visual_studio(project, true)
project_set_rebuild_project_dependencies(project, true)
project_set_project_dependencies(project, ["ckg"])

-- Do different things depending on the platform
if (PLATFORM_WINDOW) then
	project_inject_as_argument(project, "some other thing here") -- w0 to disable warnings  (not sure if that is accurate but just putting this here)
elseif (PLATFORM_LINUX) then
	project_inject_as_argument(project, "/MP") -- w0 to disable warnings (not sure if that is accurate but just putting this here)
end()

-- -------------------------------------------------------------------------------------

lib_procedure = procedure_create("./ckit_build_" .. project.compiler_type)
procedure_set_output_name(lib_procedure, "ckit.lib")

procedure_set_compile_time_defines(lib_procedure, [""])
procedure_set_include_paths(lib_procedure, [""])
procedure_set_source_files(lib_procedure, ["../ckg/ckg.c", "../ckit.c"])
procedure_set_additional_libs(lib_procedure, [""])

-- -------------------------------------------------------------------------------------

test_exe_procedure = procedure_create("./Tests/CoreTest/build_" .. project.compiler_type)
procedure_set_output_name(test_exe_procedure, "test_ckit.exe")

procedure_set_compile_time_defines(test_exe_procedure, [""])
procedure_set_include_paths(test_exe_procedure, [""])
procedure_set_source_files(test_exe_procedure, ["../*.c"])
procedure_set_additional_libs(test_exe_procedure, ["../../../ckit_build_$compiler_type/ckit.lib"])

-- -------------------------------------------------------------------------------------

graphics_exe_procedure = procedure_create("./Tests/GraphicsTest/build_" .. project.compiler_type)
set_output_name(graphics_exe_procedure, "test_graphics_ckit.exe")

procedure_set_compile_time_defines(graphics_exe_procedure, [""])
procedure_set_include_paths(graphics_exe_procedure, [""])
procedure_set_source_files(graphics_exe_procedure, ["../*.c"])
procedure_set_additional_libs(graphics_exe_procedure, ["../../../ckit_build_$compiler_type/ckit.lib"])

-- -------------------------------------------------------------------------------------

project_set_executables_to_run(project, ["test_graphics_ckit.exe"])