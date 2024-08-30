
#define CKIT_IMPL
#include "./ckit.h"

void ckit_str_register_arena();
void ckit_memory_arena_unregister_all();
void platform_console_init();
void platform_console_shutdown();
void ckit_platform_dll_init();
void ckit_platform_dll_free();

void ckit_init() {
	ckit_tracker_init();
	memory_init();
	ckit_platform_dll_init();
	//platform_console_init();
	ckit_str_register_arena();
}

void ckit_cleanup() {
	ckit_memory_arena_unregister_all();
	ckit_platform_dll_free();
	//platform_console_shutdown();

	ckit_memory_report(CKG_LOG_LEVEL_WARN);
}