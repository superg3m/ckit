#pragma once
/*===========================================================
 * File: core_arena.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_memory.h"

//========================== Begin Structs ==========================
typedef enum ArenaFlags {
  ARENA_DEFAULT = 0x1,
  ARENA_CIRCULAR = 0x2,
  ARENA_VECTOR = 0x4,
} ArenaFlags;

typedef struct Arena {
  const char* name;
  u64 capacity;
  u64 used;
  u32 flags;
  void* base_address;
  u64 memory_tag_values[MEMORY_TAG_ARENA];
} Arena;

//=========================== End Structs ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
  Arena MACRO_arena_create(u32 allocation, const char* name, ArenaFlags flags);
  void* MACRO_arena_push(Arena* arena, u32 element_size, MemoryTag memory_tag);
  void  arena_free(Arena* arena);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define arena_create(allocation_size, name, memory_tag) MACRO_arena_create(allocation_size, name, ARENA_DEFAULT)
#define arena_create_custom(allocation_size, name, memory_tag, flags) MACRO_arena_create(allocation_size, name, flags)

#define arena_push(arena, type, memory_tag) ((type*)MACRO_arena_push(arena, sizeof(type), memory_tag))
#define arena_push_array(arena, type, element_count, memory_tag) ((type*)MACRO_arena_push(arena, sizeof(type) * element_count, memory_tag))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++







