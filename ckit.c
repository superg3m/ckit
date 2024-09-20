// Basic
#include "./Source/Basic/ckit_logger.c"
#include "./Source/Basic/ckit_memory.c"
#include "./Source/Basic/ckit_arena.c"
#include "./Source/Basic/ckit_string.c"
#include "./Source/Basic/ckit_math.c"
#include "./Source/Basic/ckit_os.c"
#include "./Source/Basic/ckit_platform_services.c"

// Collections
#include "./Source/Collections/ckit_vector.c"
#include "./Source/Collections/ckit_hashmap.c"
#include "./Source/Collections/ckit_hashset.c"
#include "./Source/Collections/ckit_linked_list.c"
#include "./Source/Collections/ckit_queue.c"
#include "./Source/Collections/ckit_stack.c"

// Utilities
#include "./Source/Utilities/ckit_file_system.c"

// Parser_and_Lexer
#include "./Source/Parser_and_Lexer/ckit_bmp_parser.c"

void ckit_str_register_arena();
void ckit_memory_arena_unregister_all();
void platform_console_init();
void platform_console_shutdown();

void ckit_init() {
	ckit_tracker_init();
	memory_init();
	//platform_console_init();
	ckit_str_register_arena();
}

void ckit_cleanup() {
	ckit_memory_arena_unregister_all();
	//platform_console_shutdown();

	ckit_memory_report(LOG_LEVEL_WARN);
}