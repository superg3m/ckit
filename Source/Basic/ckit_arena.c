#pragma once

#include "../../Include/Basic/ckit_arena.h"

#include "../../Include/Basic/ckit_memory.h"
#include "../../Include/Basic/ckit_assert.h"
#include "../../Include/Collections/ckit_linked_list.h"

#define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

//========================== Begin Types ==========================
typedef struct CKIT_ArenaPage {
	void* base_address;
	u64 capacity;
	u64 used;
} CKIT_ArenaPage;

typedef struct CKIT_Arena {
	const char* name;
	CKIT_LinkedList* pages;
	CKIT_ArenaFlag flag;
	u8 alignment;
} CKIT_Arena;
//=========================== End Types ===========================

void ckit_memory_arena_register(CKIT_Arena* arena);
void ckit_memory_arena_unregister(CKIT_Arena* arena);

Boolean ckit_CKIT_ARENA_FLAG_is_set(CKIT_Arena* arena, CKIT_ArenaFlag flag) {
	return arena->flag == flag;
}

internal CKIT_ArenaPage* ckit_arena_page_create(size_t allocation_size) {
	CKIT_ArenaPage* ret = ckit_alloc_custom(sizeof(CKIT_ArenaPage), TAG_CKIT_CORE_ARENA);
	ret->used = 0;
	ret->capacity = allocation_size;
	ret->base_address = ckit_alloc_custom(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE, TAG_CKIT_CORE_ARENA);

	return ret;
}

CKIT_Arena* MACRO_ckit_arena_create(size_t allocation_size, const char* name, CKIT_ArenaFlag flag, u8 alignment) {
	CKIT_Arena* arena = ckit_alloc_custom(sizeof(CKIT_Arena), TAG_CKIT_CORE_ARENA);
	arena->alignment = alignment == 0 ? 8 : alignment;
	arena->name = name;
	arena->flag = flag;
	arena->pages = ckit_linked_list_create(CKIT_ArenaPage*, TRUE);
	CKIT_ArenaPage* inital_page = ckit_arena_page_create(allocation_size);
	ckit_linked_list_push(arena->pages, inital_page);
	ckit_memory_arena_register(arena);

	return arena;
}

CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena) {
	ckit_assert(arena);

	u32 cached_count = arena->pages->count;
	for (u32 i = 0; i < cached_count; i++) {
		CKIT_ArenaPage* page = ckit_linked_list_remove(arena->pages, 0).data;
		ckit_assert(page->base_address);
		ckit_free(page->base_address);
		ckit_free(page);
	}
	ckit_linked_list_free(arena->pages);

	ckit_memory_arena_unregister(arena);
	ckit_free(arena);

	return arena;
}

void ckit_arena_clear(CKIT_Arena* arena) {
	ckit_assert(arena);

	for (u32 i = 0; i < arena->pages->count; i++) {
		CKIT_ArenaPage* page = ckit_linked_list_get(arena->pages, i);
		ckit_assert(page->base_address);
		ckit_memory_zero(page->base_address, page->used);
		page->used = 0;
	}
}

void* MACRO_ckit_arena_push(CKIT_Arena* arena, size_t element_size) {
	ckit_assert(arena);

	CKIT_ArenaPage* last_page = ckit_linked_list_peek_tail(arena->pages);
	if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_FIXED)) { // single page assert if you run out of memory
		ckit_assert((last_page->used + element_size <= last_page->capacity));
	} else if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_CIRCULAR)) { // single page circle around if you run out of memory
		if ((last_page->used + element_size > last_page->capacity)) {
			last_page->used = 0;
			ckit_assert((last_page->used + element_size <= last_page->capacity));
		}
	} else if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_EXTENDABLE_PAGES)) { // Allocate memory, doesn't invalidate pointers
		ckit_assert(last_page->base_address);
		if ((last_page->used + element_size > last_page->capacity)) {
			CKIT_ArenaPage* next_page = ckit_arena_page_create((last_page->capacity + element_size) * 2);
			ckit_assert(next_page->base_address);
			ckit_linked_list_push(arena->pages, next_page);
		}
	} else {
		ckit_assert(FALSE);
	}

	last_page = ckit_linked_list_peek_tail(arena->pages); // tail might change

	u8* ret = ((u8*)last_page->base_address) + last_page->used;
	last_page->used += element_size;
	if ((last_page->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
		last_page->used += (arena->alignment - (last_page->used & (arena->alignment - 1)));
	}

	return ret;
}