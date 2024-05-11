#pragma once
/*===========================================================
 * File: core_arena.h
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_memory.h"

//========================== Begin Structs ==========================
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
//=========================== End Structs ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
  Arena _arena_create(const char* name, ArenaFlags flags);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define arena_create(arena, flags) arena = _arena_create(stringify(arena), flags)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++







