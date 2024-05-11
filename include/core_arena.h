#pragma once
/*===========================================================
 * File: core_arena.h
 * Date: April 30, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_memory.h"

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

Arena _arena_create(const char* name, ArenaFlags flags);

/**
 * @brief Modifies the provided arena
 * 
 */
#define arena_create(arena, flags) arena = _arena_create(stringify(arena), flags)


