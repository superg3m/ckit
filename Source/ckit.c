

#include "./ckit.h"

#include "./ckit/ckit_logger.c"
#include "./ckit/ckit_memory.c"
#include "./ckit/ckit_arena.c"
#include "./ckit/ckit_memory_tracker.c"
#include "./ckit/ckit_assert.c"
#include "./ckit/ckit_event_system.c"
#include "./ckit/ckit_string.c"
#include "./ckit/ckit_char.c"
#include "./ckit/ckit_file_system.c"
#include "./ckit/ckit_math.c"
#include "./ckit/ckit_vector.c"
#include "./ckit/ckit_hashing.c"
#include "./ckit/ckit_hashmap.c"
#include "./ckit/ckit_hashset.c"
#include "./ckit/ckit_linked_list.c"
#include "./ckit/ckit_queue.c"
#include "./ckit/ckit_stack.c"
#include "./ckit/modules/File_Format_Parser/Asset/BMP/bmp_parser.h"
#include "./ckit/modules/Lexer/ckit_lexer.c"

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

