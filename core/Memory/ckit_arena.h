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

typedef enum ArenaFlags {
  ARENA_FLAG_DEFAULT = 0x1,
  ARENA_FLAG_CIRCULAR = 0x2,
  ARENA_FLAG_VECTOR = 0x4,
  ARENA_FLAG_COUNT = 0x4,
} ArenaFlags;

typedef struct CKIT_Arena {
	const char* name;
	u64 capacity;
	u64 used;
	u32 flags;
	void* base_address;
	u64 memory_tag_values[MEMORY_TAG_COUNT];
} CKIT_Arena;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Arena* MACRO_ckit_arena_create(size_t allocation, const char* name, ArenaFlags flags);
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
#define arena_create(allocation_size, name) MACRO_ckit_arena_create(allocation_size, name, ARENA_FLAG_DEFAULT)
#define arena_create_custom(allocation_size, name, flags) MACRO_ckit_arena_create(allocation_size, name, flags)

#define arena_push(arena, type, memory_tag) ((type*)MACRO_ckit_arena_push(arena, sizeof(type), memory_tag))
#define arena_push_array(arena, type, element_count, memory_tag) ((type*)MACRO_ckit_arena_push(arena, sizeof(type) * element_count, memory_tag))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++







