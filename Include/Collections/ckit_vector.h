#pragma once

#include "../ckit_types.h"
#include "../Basic/ckit_assert.h"
#include "../Basic/ckit_memory.h"
//========================== Begin Types ==========================
typedef struct CKIT_VectorHeader {
	u32 count;
	u32 capacity;
	char* magic;
	// Boolean is_pointer_type (if I know this then I can use arenas way way smarter, but actually maybe it doesn't matter maybe I need to rework arenas s
	// it adds pages of memory and it links those together instead of copying and reallocing.)
} CKIT_VectorHeader;

//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow, const char* file, const u32 line, const char* function);
    void* MACRO_ckit_vector_reserve(size_t element_size, u32 capacity, const char* file, const u32 line, const char* function);
	void* MACRO_ckit_vector_free(void* vector);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

// Date: July 13, 2024
// NOTE(Jovanni): This doesn't have very good checking if the vector is NULLPTR
// Attemptying to get the length/cap/pop of a NULLPTR will result in a uncontrolled crash

// Date: September 12, 2024
// TODO(Jovanni): MAKE SURE YOU TEST ckit_vector_remove_at() and ckit_vector_insert_at()
//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_vector_base(vector) ((CKIT_VectorHeader*)(((u8*)vector) - sizeof(CKIT_VectorHeader)))
#define ckit_vector_count(vector) (*ckit_vector_base(vector)).count
#define ckit_vector_capacity(vector) (*ckit_vector_base(vector)).capacity
#define ckit_vector_push(vector, element) vector = ckit_vector_grow(vector, sizeof(element), FALSE, __FILE__, __LINE__, __func__); vector[ckit_vector_base(vector)->count++] = element
#define ckit_vector_reserve(capactiy, type) (type*)MACRO_ckit_vector_reserve(sizeof(type), capactiy, __FILE__, __LINE__, __func__)
#define ckit_vector_pop(vector) vector[--ckit_vector_base(vector)->count]
#define ckit_vector_remove_at(vector, index) ckit_memory_delete_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), index); ckit_vector_base(vector)->count--
#define ckit_vector_insert_at(vector, element, index) ckit_memory_insert_index(vector, ckit_vector_count(vector), ckit_vector_capacity(vector), element, index); ckit_vector_base(vector)->count++
#define ckit_vector_free(vector) vector = MACRO_ckit_vector_free(vector)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
	#include "../Basic/ckit_memory.h"
	#include "../Basic/ckit_string.h"

	// Start experimenting with returning an index back
	// - [ ] Serializable
	// - [ ] Deletable
	// - [ ] Relocatable (shuffuling the data) (update the referece so you have the right index?)
	// - [ ] No dangling pointers

	// typedef struct CKIT_MemoryIndex {
	//     u32 index;
	// } CKIT_MemoryIndex;

	#define VECTOR_DEFAULT_CAPACITY 1
	#define CKIT_VECTOR_MAGIC "CKIT_MAGIC_VECTOR"

	internal void ckit_vector_check_magic(void* vector) {
		ckit_assert_msg(ckit_str_equal(ckit_vector_base(vector)->magic, CKIT_VECTOR_MAGIC), "Vector has wrong magic: {%s} got: {%s} \n", CKIT_VECTOR_MAGIC, ckit_vector_base(vector)->magic);
	}

	void* ckit_vector_grow(void* vector, size_t element_size, Boolean force_grow, const char* file, const u32 line, const char* function) {
		if (vector == NULLPTR) {
			vector = ckit_alloc_custom(sizeof(CKIT_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size), TAG_CKIT_CORE_VECTOR);
			vector = (u8*)vector + sizeof(CKIT_VectorHeader);
			ckit_vector_base(vector)->count = 0;
			ckit_vector_base(vector)->capacity = VECTOR_DEFAULT_CAPACITY;
			ckit_vector_base(vector)->magic = CKIT_VECTOR_MAGIC;
		}

		ckit_vector_check_magic(vector);
		u32 count = ckit_vector_count(vector);
		u32 capactiy = ckit_vector_capacity(vector);

		if (force_grow || capactiy < count + 1) {
			u32 new_capactiy = capactiy * 2;
			size_t new_allocation_size = sizeof(CKIT_VectorHeader) + (new_capactiy * element_size);

			vector = MACRO_ckit_realloc(ckit_vector_base(vector), new_allocation_size, file, line, function);
			vector = (u8*)vector + sizeof(CKIT_VectorHeader);

			ckit_vector_base(vector)->count = count;
			ckit_vector_base(vector)->capacity = new_capactiy;
		}

		return vector;
	}

	void* MACRO_ckit_vector_reserve(size_t element_size, u32 inital_capacity, const char* file, const u32 line, const char* function) {
		void* vector = MACRO_ckit_alloc(sizeof(CKIT_VectorHeader) + (inital_capacity * element_size), TAG_CKIT_CORE_VECTOR, file, line, function);
		vector = (u8*)vector + sizeof(CKIT_VectorHeader);
		ckit_vector_base(vector)->count = 0;
		ckit_vector_base(vector)->capacity = inital_capacity;
		ckit_vector_base(vector)->magic = CKIT_VECTOR_MAGIC;
		return vector;
	}

	void* MACRO_ckit_vector_free(void* vector) {
		ckit_vector_check_magic(vector);
		CKIT_VectorHeader* vector_base = ckit_vector_base(vector);
		ckit_assert_msg(vector_base, "WHAT"); 
		vector_base->count = 0;
		vector_base->capacity = 0;
		ckit_free(vector_base);

		return vector_base;
	}
#endif