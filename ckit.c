
#define CKIT_IMPL
#include "ckit.h"

#include "./Source/ckit_platform_services.c"
#include "./Source/ckit_string.c"
#include "./Source/ckit_vector.c"
#include "./Source/ckit_queue.c"
#include "./Source/ckit_stack.c"

void ckit_memory_arena_unregister_all();

void ckit_init() {
	ckit_tracker_init();
	// string_arena()
}

void ckit_cleanup() {
	// string_arena_free()
	// ckit_memory_arena_unregister_all();
	ckit_memory_report(CKG_LOG_LEVEL_WARN);
}