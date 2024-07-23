// could be implmeneted with a linked list I don't mind that 
#include "./ckit_stack.h"
#include "../LinkedList/ckit_linked_list.h"
#include "../../Memory/ckit_memory.h"

typedef struct CKIT_Stack {
	CKIT_LinkedList* linked_list;
} CKIT_Stack;

CKIT_Stack* MACRO_ckit_stack_create(size_t size_in_bytes, Boolean is_pointer_type, char* file, u32 line, char* function) {
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
