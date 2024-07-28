#pragma once

#include "../ckg_types.h"

//========================== Begin Types ==========================
typedef struct CKG_Arena {
	const char* name;
    void* base_address;
    u64 capacity;
	u64 used;
	u32 flag;
} CKG_Arena;

typedef enum CKG_ArenaFlag {
  CKG_ARENA_FLAG_DEFAULT,
  CKG_ARENA_FLAG_CIRCULAR,
  CKG_ARENA_FLAG_VECTOR,
  CKG_ARENA_FLAG_COUNT
} CKG_ArenaFlag;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKG_Arena MACRO_ckg_arena_create(u32 allocation, const char* name, CKG_ArenaFlag flag);
	void* MACRO_ckg_arena_push(CKG_Arena* arena, u32 element_size);	
	
	void ckg_arena_free(CKG_Arena* arena);
	void ckg_arena_clear(CKG_Arena* arena);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckg_arena_create(allocation_size, name) MACRO_ckg_arena_create(allocation_size, name, CKG_ARENA_FLAG_DEFAULT)
#define ckg_arena_create_custom(allocation_size, name, flags) MACRO_ckg_arena_create(allocation_size, name, flags)

#define ckg_arena_push(arena, type) ((type*)MACRO_ckg_arena_push(arena, sizeof(type)))
#define ckg_arena_push_array(arena, type, element_count) ((type*)MACRO_ckg_arena_push(arena, sizeof(type) * element_count))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++