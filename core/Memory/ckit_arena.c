#include "./ckit_arena.h"
#include "./ckit_memory.h"

// should register arena with memory

typedef struct Arena {
	const char* name;
	u64 capacity;
	u64 used;
	u32 flags;
	void* base_address;
	u64 memory_tag_values[MEMORY_TAG_ARENA];
} Arena;

internal Boolean arena_is_initalized = FALSE;

extern char known_memory_tag_strings[MEMORY_TAG_COUNT][MEMORY_TAG_CHARACTER_LIMIT];

#define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

Boolean arena_init() {
    arena_is_initalized = TRUE;
    return arena_is_initalized;
}

Arena* MACRO_arena_create(size_t allocation_size, const char* name, ArenaFlags flags) {
    ckit_assert_msg(arena_is_initalized, "arena_create: call CKit_init() first\n");
    Arena* arena = (Arena*)ckit_alloc(sizeof(Arena), MEMORY_TAG_ARENA);
    arena->name = name;
    arena->flags = flags;
    arena->capacity = allocation_size;
    arena->used = 0;
    ckg_memory_zero(arena->memory_tag_values, sizeof(u64) * MEMORY_TAG_ARENA);
    arena->base_address = ckit_alloc(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE, MEMORY_TAG_ARENA);
    // memory_arena_register(&arena);
    return arena;
}

void arena_free(Arena* arena) {
    ckit_assert_msg(arena && arena->base_address, "arena_free: arena is null\n");
    // Date: May 12, 2024
    // TODO(Jovanni): Make memory_arena_unregister and memory_arena_register not accessable.
    // memory_arena_unregister(&arena);
    ckit_free(arena->base_address);
    ckit_free(arena);
}

void arena_clear(Arena* arena) {
    ckit_assert_msg(arena && arena->base_address, "arena_free: arena is null\n");
    ckg_memory_zero(arena->base_address, arena->used);
    arena->used = 0;
}

void* MACRO_arena_push(Arena* arena, size_t element_size, MemoryTag memory_tag) {
    // Date: May 11, 2024
    // TODO(Jovanni): For right now just assert if you don't have enough memory but later on make it grow.
    ckit_assert_msg(arena && arena->base_address, "arena_push: arena is null\n");
    ckit_assert_msg((arena->used + element_size < arena->capacity), "arena_push: can't push element ran out of memory\n");
    if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("arena_push: memory tag unknown\n");
    }
    if ((arena->used + element_size >= arena->capacity)) {
        arena->capacity += element_size;
        arena->capacity *= 2;
        arena->base_address = ckit_realloc(arena->base_address, arena->capacity);
    }

    u8* ret = (u8*)arena->base_address + arena->used;

    arena->memory_tag_values[memory_tag] += element_size;
    arena->used += element_size;
    
    return ret;
}

void arena_output_allocations(Arena* arena, LogLevel log_level) {
    if (arena->used == 0) {
        log_output(log_level, "No arena allocations!\n");
        return;
    }

    char out_message[PLATFORM_CHARACTER_LIMIT];
    char out_message2[PLATFORM_CHARACTER_LIMIT];
    char out_message3[PLATFORM_CHARACTER_LIMIT];
    
    log_output(log_level, "=========== %s: %lld ===========\n", arena->name, arena->capacity);
    u32 sum = 0;
    for (int level = 0; level < MEMORY_TAG_ARENA; level++) {
        if (arena->memory_tag_values[level] == 0) {
            continue;
        } 

        ckg_memory_zero(out_message, sizeof(out_message));
        ckg_memory_zero(out_message2, sizeof(out_message2));
        ckg_memory_zero(out_message3, sizeof(out_message3));

        sprintf(out_message, "%s", known_memory_tag_strings[level]);
        sprintf(out_message2, "%lld", arena->memory_tag_values[level]);
        sprintf(out_message3, "%s%s", out_message, out_message2);
        log_output(log_level, "%s\n", out_message3);
    }
    log_output(log_level, "============== Total used: %lld ==============\n", arena->used);
}
