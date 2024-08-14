#pragma once

#include "ckit_types.h"
#include "ckit_linked_list.h"
//========================== Begin Types ==========================
typedef struct CKIT_ArenaPage {
	void* base_address;
	u64 capacity;
	u64 used;
} CKIT_ArenaPage;

typedef struct CKIT_Arena {
	const char* name;
	CKIT_LinkedList* pages;
	u32 flag;
	u8 alignment;
} CKIT_Arena;

typedef enum CKIT_ArenaFlag {
  CKIT_ARENA_FLAG_FIXED,
  CKIT_ARENA_FLAG_CIRCULAR,
  CKIT_ARENA_FLAG_EXTENDABLE_PAGES,
  CKIT_ARENA_FLAG_COUNT
} CKIT_ArenaFlag;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Arena* MACRO_ckit_arena_create(u32 allocation_size, const char* name, CKIT_ArenaFlag flag, u8 alignment);
	void* MACRO_ckit_arena_push(CKIT_Arena* arena, u32 element_size);	
	
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

#if defined(CKIT_IMPL)
	#include "ckit_memory.h"
	#include "ckit_assert.h"

	#define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

	Boolean ckit_CKIT_ARENA_FLAG_is_set(CKIT_Arena* arena, CKIT_ArenaFlag flag) {
		return arena->flag == flag;
	}

	internal CKIT_ArenaPage* ckit_arena_page_create(u32 allocation_size) {
		CKIT_ArenaPage* ret = ckit_alloc_custom(sizeof(CKIT_ArenaPage), TAG_CKIT_CORE_ARENA);
		ret->used = 0;
		ret->capacity = allocation_size;
		ret->base_address = ckit_alloc(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE);

		return ret;
	}

	CKIT_Arena* MACRO_ckit_arena_create(u32 allocation_size, const char* name, CKIT_ArenaFlag flag, u8 alignment) {
		ckit_assert_msg((alignment & 1) == 0, "Arena alignment is not a power of two\n");
		CKIT_Arena* arena = ckit_alloc(sizeof(CKIT_Arena));
		arena->alignment = alignment == 0 ? 8 : alignment;
		arena->name = name;
		arena->flag = flag;
		arena->pages = ckit_linked_list_create(CKIT_ArenaPage*, TRUE);
		CKIT_ArenaPage* inital_page = ckit_arena_page_create(allocation_size);
		ckit_linked_list_push(arena->pages, inital_page);

		return arena;
	}

	CKIT_Arena* MACRO_ckit_arena_free(CKIT_Arena* arena) {
		ckit_assert(arena);

		for (int i = 0; i < arena->pages->count; i++) {
			CKIT_ArenaPage* page = ckit_linked_list_remove(arena->pages, 0).data;
			ckit_assert(page->base_address);
			ckit_free(page->base_address);
			ckit_free(page);
		}
		ckit_linked_list_free(arena->pages);
		ckit_free(arena);

		return arena;
	}

	void ckit_arena_clear(CKIT_Arena* arena) {
		ckit_assert(arena);

		for (int i = 0; i < arena->pages->count; i++) {
			CKIT_ArenaPage* page = ckit_linked_list_get(arena->pages, i);
			ckit_assert(page->base_address);
			ckit_memory_zero(page->base_address, page->used);
			page->used = 0;
		}
	}

	void* MACRO_ckit_arena_push(CKIT_Arena* arena, u32 element_size) {
		ckit_assert(arena);

		CKIT_ArenaPage* last_page = ckit_linked_list_peek_tail(arena->pages);
		if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_FIXED)) { // single page assert if you run out of memory
			ckit_assert((last_page->used + element_size <= last_page->capacity));
		} else if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_CIRCULAR)) { // single page circle around if you run out of memory
			if ((last_page->used + element_size > last_page->capacity)) {
				last_page->used = 0;
				ckit_assert((last_page->used + element_size <= last_page->capacity));
			}
		} else if (ckit_CKIT_ARENA_FLAG_is_set(arena, CKIT_ARENA_FLAG_EXTENDABLE_PAGES)) {
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
#endif // CKIT_IMPL
