#pragma once
/*===========================================================
 * File: ckit_arena.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckit_types.h"
#include "ckit_memory.h"
//========================== Begin Types ==========================
typedef struct CKIT_MemoryTag CKIT_MemoryTag;

typedef enum CKIT_ArenaFlags {
  CKIT_ARENA_FLAG_DEFAULT = 0x1,
  CKIT_ARENA_FLAG_CIRCULAR = 0x2,
  CKIT_ARENA_FLAG_VECTOR = 0x4,
  CKIT_ARENA_FLAG_COUNT = 0x4,
} CKIT_ArenaFlags;

typedef struct CKIT_Arena {
	const char* name;
	u64 capacity;
	u64 used;
	CKIT_ArenaFlags flags;
	void* base_address;
	u64 memory_tag_values[TAG_CKIT_RESERVED_COUNT];
	u8 alignment; // if 0 then unaligned memory, default is 8 byte alignment
} CKIT_Arena;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Arena* MACRO_ckit_arena_create(size_t allocation, const char* name, CKIT_ArenaFlags flags, u8 alignment);
	// Has the opporutnity to realloc (I think what i'm going to do moving forward is just another padge of memory on the end that way pointers don't get messed up)
	void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size, CKIT_MemoryTagID memory_tag);
	// void ckit_arena_output_allocations(CKIT_Arena* arena, CKG_LogLevel log_level);
	
	// Date: May 11, 2024
	// NOTE(Jovanni): I want better names for this action
	void ckit_arena_write_tags(CKIT_Arena arena);	
	void* ckit_arena_free(CKIT_Arena* arena);
	void ckit_arena_clear(CKIT_Arena* arena);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_arena_create(allocation_size, name, alignment) MACRO_ckit_arena_create(allocation_size, name, CKIT_ARENA_FLAG_DEFAULT, alignment)
#define ckit_arena_create_custom(allocation_size, name, flags) MACRO_ckit_arena_create(allocation_size, name, flags)

#define ckit_arena_push(arena, type, memory_tag) ((type*)MACRO_ckit_arena_push(arena, sizeof(type), memory_tag))
#define ckit_arena_push_array(arena, type, element_count, memory_tag) ((type*)MACRO_ckit_arena_push(arena, sizeof(type) * element_count, memory_tag))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
	#include "ckit_memory.h"
	#include "ckit_assert.h"

	// extern char known_memory_tag_strings[TAG_CKIT_RESERVED_COUNT][MEMORY_TAG_CHARACTER_LIMIT];

	#define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

	// void ckit_memory_arena_register(CKIT_Arena* arena);
	// void ckit_memory_arena_unregister(CKIT_Arena* arena);

	void arena_init() {

	}

	// Date: August 09, 2024
	// TODO(Jovanni): WHEN ARENA GROWS IT GROWS BY ADDING ANTOHER PAGE NOT REALLOCATING THAT WAY POINTERS ARE STILL VALID
	// TODO(Jovanni): WHEN ARENA GROWS IT GROWS BY ADDING ANTOHER PAGE NOT REALLOCATING THAT WAY POINTERS ARE STILL VALID
	// TODO(Jovanni): WHEN ARENA GROWS IT GROWS BY ADDING ANTOHER PAGE NOT REALLOCATING THAT WAY POINTERS ARE STILL VALID

	CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, const char* name, CKIT_ArenaFlags flags, u8 alignment) {
		ckit_assert_msg((alignment & 1) == 0, "Arena alignment is not a power of two\n");
		CKIT_Arena* arena = ckit_alloc_custom(sizeof(CKIT_Arena), TAG_CKIT_CORE_ARENA);
		arena->alignment = alignment == 0 ? 8 : alignment;
		arena->name = name;
		arena->flags = flags;
		arena->capacity = allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE;
		arena->used = 0;
		ckit_memory_zero(arena->memory_tag_values, sizeof(u64));
		arena->base_address = ckit_alloc_custom(arena->capacity, TAG_CKIT_CORE_ARENA);

		//ckit_memory_arena_register(arena);

		return arena;
	}

	void* ckit_arena_free(CKIT_Arena* arena) {
		ckit_assert(arena && arena->base_address);
		//ckit_memory_arena_unregister(arena);
		ckit_free(arena->base_address);
		ckit_free(arena);
		return arena;
	}

	void ckit_arena_clear(CKIT_Arena* arena) {
		ckit_assert(arena && arena->base_address);
		ckit_memory_zero(arena->base_address, arena->used);
		arena->used = 0;
	}

	void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size, CKIT_MemoryTagID tag_id) {
		ckit_assert_msg(arena && arena->base_address, "arena_push: arena is null\n");
		ckit_assert_msg((arena->used + element_size < arena->capacity), "arena_push: can't push element ran out of memory\n");

		if (arena->flags == CKIT_ARENA_FLAG_DEFAULT) {
			ckit_assert((arena->used + element_size <= arena->capacity));
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
		arena->memory_tag_values[tag_id] += element_size;
		arena->used += element_size;
		if ((arena->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
			arena->used += (arena->alignment - (arena->used & (arena->alignment - 1)));
		}
		
		return ret;
	}

	/*
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
		for (int level = 0; level < TAG_CKIT_RESERVED_COUNT; level++) {
			if (arena->memory_tag_values[level] == 0) {
				continue;
			} 

			ckit_memory_zero(out_message, sizeof(out_message));
			ckit_memory_zero(out_message2, sizeof(out_message2));
			ckit_memory_zero(out_message3, sizeof(out_message3));

			// sprintf(out_message, "  %s", known_memory_tag_strings[level]);
			sprintf(out_message2, " %lld", arena->memory_tag_values[level]);
			sprintf(out_message3, " %s%s", out_message, out_message2);
			log_output(log_level, " %s\n", out_message3);
		}
		log_output(log_level, "     - ============== Total used: %lld ==============\n", arena->used);
	}
	*/
#endif // CKIT_IMPL

