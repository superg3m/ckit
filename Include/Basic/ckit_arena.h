#pragma once

#include "../ckit_types.h"
//========================== Begin Types ==========================
typedef enum CKIT_ArenaFlag {
  CKIT_ARENA_FLAG_FIXED,
  CKIT_ARENA_FLAG_CIRCULAR,
  CKIT_ARENA_FLAG_EXTENDABLE_PAGES,
  CKIT_ARENA_FLAG_COUNT
} CKIT_ArenaFlag;

typedef struct CKIT_Arena CKIT_Arena;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, const char* name, CKIT_ArenaFlag flag, u8 alignment);
	void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size);	
	
	CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena);
	void ckit_arena_clear(CKIT_Arena* arena);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_arena_create(allocation_size, name) MACRO_ckit_arena_create(allocation_size, name, CKIT_ARENA_FLAG_EXTENDABLE_PAGES, 0)
#define ckit_arena_create_custom(allocation_size, name, flags, alignment) MACRO_ckit_arena_create(allocation_size, name, flags, alignment)

#define ckit_arena_free(arena) arena = MACRO_ckit_arena_free(arena)

#define ckit_arena_push(arena, type) ((type*)MACRO_ckit_arena_push(arena, sizeof(type)))
#define ckit_arena_push_array(arena, type, element_count) ((type*)MACRO_ckit_arena_push(arena, sizeof(type) * element_count))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++