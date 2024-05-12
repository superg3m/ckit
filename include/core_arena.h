#pragma once
/*===========================================================
 * File: core_arena.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_types.h"

//========================== Begin Types ==========================
typedef enum MemoryTag MemoryTag;
typedef struct Arena Arena;

typedef enum ArenaFlags {
  ARENA_FLAG_DEFAULT = 0x1,
  ARENA_FLAG_CIRCULAR = 0x2,
  ARENA_FLAG_VECTOR = 0x4,
  ARENA_FLAG_COUNT = 0x4,
} ArenaFlags;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
  void memory_register_arena(Arena** arena);

  Arena* MACRO_arena_create(u32 allocation, const char* name, ArenaFlags flags);
  void* MACRO_arena_push(Arena* arena, u32 element_size, MemoryTag memory_tag);
  
  // Date: May 11, 2024
  // NOTE(Jovanni): I want better names for this action
  void  arena_write_tags(Arena arena);

  void  arena_free(Arena* arena);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define arena_create(allocation_size, name) MACRO_arena_create(allocation_size, name, ARENA_FLAG_DEFAULT)
#define arena_create_custom(allocation_size, name, flags) MACRO_arena_create(allocation_size, name, flags)

#define arena_push(arena, type, memory_tag) ((type*)MACRO_arena_push(arena, sizeof(type), memory_tag))
#define arena_push_array(arena, type, element_count, memory_tag) ((type*)MACRO_arena_push(arena, sizeof(type) * element_count, memory_tag))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++







