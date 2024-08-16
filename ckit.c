
#define CKIT_IMPL
#include "./ckit.h"

void ckit_str_register_arena();
void ckit_memory_arena_unregister_all();
void platform_console_init();
void platform_console_shutdown();

void ckit_init() {
	ckit_tracker_init();
	memory_init();
	platform_console_init();
	ckit_str_register_arena();
}

void ckit_cleanup() {
	ckit_memory_arena_unregister_all();
	ckit_memory_report(CKG_LOG_LEVEL_WARN);
}