#include "./ckit.h"

void ckit_init() {
	// string_arena()
}
void ckit_cleanup() {
	// string_arena_free()
	memory_output_allocations(CKG_LOG_LEVEL_WARN);
}