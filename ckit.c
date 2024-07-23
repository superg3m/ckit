#include "./ckit.h"

void ckit_memory_arena_unregister_all();

void ckit_init() {
	ckit_tracker_init();
	// string_arena()
}
void ckit_cleanup() {


	// string_arena_free()
	// ckit_memory_arena_unregister_all();
	// memory_output_allocations(CKG_LOG_LEVEL_WARN);
}