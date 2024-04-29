#include "../include/core_arena.h"

struct Arena {
    char* name;
    u64 memory_tag_values[MEMORY_TAG_COUNT];
    u64 size;
    u64 used;
};

void _arena_create(Arena arena, char* name) {

}
