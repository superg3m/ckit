#pragma once
// used a circular buffer

// read, next_read. and write pointers

// if the write pointer is greater than the next_read pointer then assert
// if read pointer is on next read pointer and you try to read again assert because you will be ahead of the write pointer

#include "ckit_types.h"
#include "ckit_memory.h"
//========================== Begin Types ==========================
typedef struct CKIT_Queue {
	u32 capacity;
	u32 count;
	void* data;
	size_t element_size_in_bytes;

	u32 read_index;
	Boolean has_next_to_read;
	u32 write_index;
	Boolean is_pointer_type;
} CKIT_Queue;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function);
	CKIT_Queue* MACRO_ckit_queue_free(CKIT_Queue* queue);
	void ckit_enqueue(CKIT_Queue* queue, void* element);
	/*
	 You are responsible for freeing the result, if not a pointer type, I think arenas is gonna be pretty big for this whole thing.
	*/
	void* ckit_dequeue(CKIT_Queue* queue);
	u32 ckit_queue_capacity(CKIT_Queue* queue);
	u32 ckit_queue_count(CKIT_Queue* queue);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_queue_create(inital_capacity, type, is_pointer_type) MACRO_ckit_queue_create(inital_capacity, sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__);
#define ckit_queue_free(queue) MACRO_ckit_queue_free(queue);
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

