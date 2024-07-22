#pragma once

#include "../../ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Stack CKIT_Stack;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Stack* MACRO_ckit_stack_create(size_t size_in_bytes, Boolean is_pointer_type);
	void* ckit_stack_push(void* data);
	// Date: July 22, 2024
	// NOTE(Jovanni): If pointer type is true you must free this yourself, trivially this should be able to go into an arena.
	void* ckit_stack_pop();
	void* MACRO_ckit_stack_free();
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_stack_create(type, is_pointer_type) MACRO_ckit_stack_create(sizeof(type), is_pointer_type)
#define ckit_stack_free(stack) MACRO_ckit_stack_stack(stack)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++