#pragma once

#include "ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Stack CKIT_Stack;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Stack* MACRO_ckit_stack_create(size_t size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
	void* ckit_stack_push(CKIT_Stack* stack, void* data);
	// Date: July 22, 2024
	// NOTE(Jovanni): If pointer type is true you must free this yourself, trivially this should be able to go into an arena.
	void* ckit_stack_pop(CKIT_Stack* stack);
	void* MACRO_ckit_stack_free(CKIT_Stack* stack);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_stack_create(type, is_pointer_type) MACRO_ckit_stack_create(sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__)
#define ckit_stack_free(stack) MACRO_ckit_stack_free(stack)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
	#include "ckit_linked_list.h"
	#include "ckit_memory.h"

	typedef struct CKIT_Stack {
		CKIT_LinkedList* linked_list;
	} CKIT_Stack;

	CKIT_Stack* MACRO_ckit_stack_create(size_t size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function) {
		CKIT_Stack* stack = MACRO_ckit_alloc(sizeof(CKIT_Stack), TAG_CKIT_CORE_STACK, file, line, function);
		stack->linked_list = MACRO_ckit_linked_list_create(size_in_bytes, is_pointer_type, file, line, function);

		return stack;
	}

	void* ckit_stack_push(CKIT_Stack* stack, void* data) {
		return ckit_linked_list_push(stack->linked_list, data);
	}

	void* ckit_stack_pop(CKIT_Stack* stack) {
		return ckit_linked_list_pop(stack->linked_list).data;
	}

	void* MACRO_ckit_stack_free(CKIT_Stack* stack) {
		ckit_linked_list_free(stack->linked_list);
		ckit_free(stack);

		return stack;
	}
#endif