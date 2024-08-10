
#define CKIT_IMPL
#include "ckit.h"

#include "./Source/ckit_logger.c"
#include "./Source/ckit_memory.c"
#include "./Source/ckit_arena.c"
#include "./Source/ckit_platform_services.c"
#include "./Source/ckit_memory_tracker.c"
#include "./Source/ckit_event_system.c"
#include "./Source/ckit_string.c"
#include "./Source/ckit_file_system.c"
#include "./Source/ckit_math.c"
#include "./Source/ckit_vector.c"
#include "./Source/ckit_hashing.c"
#include "./Source/ckit_hashmap.c"
#include "./Source/ckit_hashset.c"
#include "./Source/ckit_linked_list.c"
#include "./Source/ckit_queue.c"
#include "./Source/ckit_stack.c"

#include "./Source/bmp_parser.c"
#include "./Source/ckit_lexer.c"

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

