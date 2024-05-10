#include "../include/core_arena.h"

typedef struct Arena {
    char* name;
    u64 size;
    u64 used;
    u64 memory_tag_values[MEMORY_TAG_COUNT];
} Arena;

void _arena_create(Arena arena, const char* name) {

}
