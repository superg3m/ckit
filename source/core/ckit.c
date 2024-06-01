#include "../../ckit.h"

extern Arena* base_area;

void ckit_init() {
	const u64 arena_allocation_size = MegaBytes(2);
	base_area = arena_create_custom(arena_allocation_size, "base_arena", ARENA_FLAG_VECTOR);
}

void ckit_cleanup() {
	arena_free(base_area);
}