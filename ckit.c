

#include "ckit.h"

// maybe? "./ckg.c"

#include "./Source/ckit/ckit_logger.c"
#include "./Source/ckit/ckit_memory.c"
#include "./Source/ckit/ckit_arena.c"
#include "./Source/ckit/ckit_memory_tracker.c"
#include "./Source/ckit/ckit_event_system.c"
#include "./Source/ckit/ckit_string.c"
#include "./Source/ckit/ckit_file_system.c"
#include "./Source/ckit/ckit_math.c"
#include "./Source/ckit/ckit_vector.c"
#include "./Source/ckit/ckit_hashing.c"
#include "./Source/ckit/ckit_hashmap.c"
#include "./Source/ckit/ckit_hashset.c"
#include "./Source/ckit/ckit_linked_list.c"
#include "./Source/ckit/ckit_queue.c"
#include "./Source/ckit/ckit_stack.c"
#include "./Source/ckit/File_Format_Parser/Asset/BMP/bmp_parser.c"
#include "./Source/ckit/Lexer/ckit_lexer.c"

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

