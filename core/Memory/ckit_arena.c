#include "./ckit_arena.h"
#include "./ckit_memory.h"
#include "../Assert/ckit_assert.h"

extern char known_memory_tag_strings[MEMORY_TAG_COUNT][MEMORY_TAG_CHARACTER_LIMIT];

#define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

void ckit_memory_arena_register(CKIT_Arena* arena);
void ckit_memory_arena_unregister(CKIT_Arena* arena);
// void ckit_memory_arena_unregister_all();


void arena_init() {

}

CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, const char* name, CKIT_ArenaFlags flags, u8 alignment) {
    ckit_assert_msg((alignment & 1) == 0, "Arena alignment is not a power of two\n");
    CKIT_Arena* arena = ckit_alloc(sizeof(CKIT_Arena), MEMORY_TAG_ARENA);
    arena->alignment = alignment == 0 ? 8 : alignment;
    arena->name = name;
    arena->flags = flags;
    arena->capacity = allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE;
    arena->used = 0;
    ckit_memory_zero(arena->memory_tag_values, sizeof(u64));
    arena->base_address = ckit_alloc(arena->capacity, MEMORY_TAG_ARENA);
    ckit_memory_arena_register(arena);
    return arena;
}

void* ckit_arena_free(CKIT_Arena* arena) {
    ckit_assert(arena && arena->base_address);
    ckit_memory_arena_unregister(arena);
    ckit_free(arena->base_address);
    ckit_free(arena);
    return arena;
}

void ckit_arena_clear(CKIT_Arena* arena) {
    ckit_assert(arena && arena->base_address);
    ckit_memory_zero(arena->base_address, arena->used);
    arena->used = 0;
}

void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size, CKIT_MemoryTag memory_tag) {
    ckit_assert_msg(arena && arena->base_address, "arena_push: arena is null\n");
    ckit_assert_msg((arena->used + element_size < arena->capacity), "arena_push: can't push element ran out of memory\n");

    if (memory_tag == MEMORY_TAG_UNKNOWN) {
        LOG_WARN("arena_push: memory tag unknown\n");
    }

    if (arena->flags == CKIT_ARENA_FLAG_DEFAULT) {
        ckg_assert((arena->used + element_size <= arena->capacity));
    } else if (arena->flags == CKIT_ARENA_FLAG_CIRCULAR) {
		if ((arena->used + element_size > arena->capacity)) {
			arena->used = 0;
			ckit_assert((arena->used + element_size <= arena->capacity));
        }
    } else if (arena->flags == CKIT_ARENA_FLAG_VECTOR) {
        if ((arena->used + element_size > arena->capacity)) {
            arena->capacity += element_size;
            arena->capacity *= 2;
            arena->base_address = ckit_realloc(arena->base_address, arena->capacity * 2);
        	ckit_assert(arena->base_address);
        }
    } else {
        ckit_assert(FALSE);
    }

    u8* ret = (u8*)arena->base_address + arena->used;
    arena->memory_tag_values[memory_tag] += element_size;
    arena->used += element_size;
    if ((arena->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
        arena->used += (arena->alignment - (arena->used & (arena->alignment - 1)));
    }
    
    return ret;
}

void ckit_arena_output_allocations(CKIT_Arena* arena, CKG_LogLevel log_level) {
    if (arena->used == 0) {
        log_output(log_level, " - %s not used! | struct_size: %d | allocation size: %d => total_allocation_size: %d \n", arena->name, sizeof(CKIT_Arena), arena->capacity, sizeof(CKIT_Arena) + arena->capacity);
        return;
    }

    char out_message[PLATFORM_CHARACTER_LIMIT];
    char out_message2[PLATFORM_CHARACTER_LIMIT];
    char out_message3[PLATFORM_CHARACTER_LIMIT];
    
    log_output(log_level, "     - =========== %s: %lld ===========\n", arena->name, arena->capacity);
    u32 sum = 0;
    for (int level = 0; level < MEMORY_TAG_ARENA; level++) {
        if (arena->memory_tag_values[level] == 0) {
            continue;
        } 

        ckit_memory_zero(out_message, sizeof(out_message));
        ckit_memory_zero(out_message2, sizeof(out_message2));
        ckit_memory_zero(out_message3, sizeof(out_message3));

        sprintf(out_message, "  %s", known_memory_tag_strings[level]);
        sprintf(out_message2, " %lld", arena->memory_tag_values[level]);
        sprintf(out_message3, " %s%s", out_message, out_message2);
        log_output(log_level, " %s\n", out_message3);
    }
    log_output(log_level, "     - ============== Total used: %lld ==============\n", arena->used);
}
