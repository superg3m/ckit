#pragma once
/*===========================================================
 * File: ckit_arena.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../ckit_types.h"
#include "./ckit_memory.h"
//========================== Begin Types ==========================
typedef enum CKIT_MemoryTag CKIT_MemoryTag;
typedef enum CKG_LogLevel CKG_LogLevel;

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
	u64 memory_tag_values[MEMORY_TAG_COUNT];
	u8 alignment; // if 0 then unaligned memory, default is 8 byte alignment
} CKIT_Arena;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Arena* MACRO_ckit_arena_create(size_t allocation, const char* name, CKIT_ArenaFlags flags, u8 alignment);
	void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size, CKIT_MemoryTag memory_tag);
	void ckit_arena_output_allocations(CKIT_Arena* arena, CKG_LogLevel log_level);
	
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







