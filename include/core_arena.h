#include "./core_memory.h"

struct Arena;

void _arena_create(Arena *arena, const char* name);

/**
 * @brief Modifies the provided arena
 * 
 */
#define arena_create(arena) _arena_create(&arena, stringify(arena))


