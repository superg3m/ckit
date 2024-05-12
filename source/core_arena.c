#include "../include/core_arena.h"
#include "../include/core_assert.h"

// should register arena with memory

#define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

Arena MACRO_arena_create(u32 allocation_size, const char* name, ArenaFlags flags) {
    Arena arena;
    arena.name = name;
    arena.flags = flags;
    arena.capacity = allocation_size;
    arena.used = 0;
    memory_zero(arena.memory_tag_values, sizeof(u64) * MEMORY_TAG_ARENA);
    arena.base_address = memory_allocate(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE, MEMORY_TAG_ARENA);
    return arena;
}

void arena_free(Arena* arena) {
    assert_in_function(arena && arena->base_address, "arena_free: arena is null\n");
    memory_free(arena->base_address);
    memory_zero(arena, sizeof(Arena));
}

void* MACRO_arena_push(Arena* arena, u32 element_size, MemoryTag memory_tag) {
    // Date: May 11, 2024
    // TODO(Jovanni): For right now just assert if you don't have enough memory but later on make it grow.
    assert_in_function(arena && arena->base_address, "arena_push: arena is null\n");
    assert_in_function((arena->used + element_size < arena->capacity), "arena_push: can't push element ran out of memory\n");
    if (memory_tag_is_unknown(memory_tag)) {
        LOG_WARN("arena_push: memory tag unknown\n");
    }
    if ((arena->used + element_size >= arena->capacity)) {
        arena->capacity += element_size;
        arena->capacity *= 2;
        arena->base_address = memory_reallocate(arena->base_address, arena->capacity);
    }

    u8* ret = memory_advance_new_ptr(arena->base_address, arena->used);
    arena->used += element_size;
    
    return ret;
}
