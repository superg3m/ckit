
#define CKIT_IMPL
#include "./ckit.h"

void ckit_str_register_arena();
void ckit_memory_arena_unregister_all();

void ckit_init() {
	ckit_tracker_init();
	memory_init();
	ckit_str_register_arena();
}

void ckit_cleanup() {
	// ckit_str_arena_free()
	ckit_memory_arena_unregister_all();
	ckit_memory_report(CKG_LOG_LEVEL_WARN);
}