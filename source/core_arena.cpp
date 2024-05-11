#include "../include/core_arena.h"

// should register arena with memory

#define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(50)

typedef enum ArenaFlags {
    ARENA_CIRCULAR = 0x1,
    ARENA_VECTOR = 0x2,
} ArenaFlags;

typedef struct Arena {
    const char* name;
    u64 capacity;
    u64 used;
    u32 flags;
    void* base_address;
    u64 memory_tag_values[MEMORY_TAG_COUNT];
} Arena;

Arena _arena_create(const char* name, ArenaFlags flags) {
    Arena arena;
    arena.name = name;
    arena.flags = flags;
    arena.base_address = memory_allocate(ARENA_DEFAULT_ALLOCATION_SIZE, MEMORY_TAG_ARENA);
    return arena;
}

Arena _arena_push(Arena arena, const void* element, u32 element_size) {
    if ((arena.used + element_size >= arena.capacity) && !(arena.flags & ARENA_CIRCULAR) ) {
        arena.capacity += element_size;
        arena.capacity *= 2;
        arena.base_address = memory_reallocate(arena.base_address, arena.capacity);
    }
    u8* dest_ptr = memory_advance_new_ptr(arena.base_address, element_size);
    memory_copy(element, dest_ptr, element_size, element_size);
    memory_byte_advance(arena.base_address, element_size);
    return arena;
}

#define arena_push(arena, element) arena = _arena_push(arena, element, size(element))


