#include "ckit_queue.h"
#include "ckit_assert.h"

CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function) {
	CKIT_Queue* ret = MACRO_ckit_alloc(sizeof(CKIT_Queue), TAG_CKIT_CORE_QUEUE, file, line, function);
	ret->element_size_in_bytes = element_size_in_bytes;
	ret->capacity = inital_capacity;
	ret->count = 0;
	ret->is_pointer_type = is_pointer_type;
	ret->data = MACRO_ckit_alloc(element_size_in_bytes * ret->capacity, TAG_CKIT_CORE_QUEUE, file, line, function);
	return ret;
}

CKIT_Queue* MACRO_ckit_queue_free(CKIT_Queue* queue) {
	queue->element_size_in_bytes = 0;
	queue->capacity = 0;
	queue->count = 0;
	ckit_free(queue->data);
	ckit_free(queue);
	return queue;
}


// Date: July 19, 2024
// NOTE(Jovanni): If you have a count it kind of defeats the purpose sort of
void* ckit_dequeue(CKIT_Queue* queue) {
	ckit_assert(queue->count > 0);
	ckit_assert(queue->has_next_to_read || queue->read_index != queue->write_index);
	queue->has_next_to_read = FALSE;

	void* ret = NULLPTR;
	
	queue->read_index = queue->read_index % queue->capacity;
	if (queue->is_pointer_type) {
		u8** temp_ptr = (u8**)((u8*)queue->data + (queue->element_size_in_bytes * queue->read_index++));
		ret = *temp_ptr;
	} else {
		ret = ckit_alloc_custom(queue->element_size_in_bytes, TAG_CKIT_EXPECTED_USER_FREE);
		ckit_memory_copy((u8*)queue->data + (queue->element_size_in_bytes * queue->read_index++), ret, queue->element_size_in_bytes, queue->element_size_in_bytes);
	}
	queue->count--;

	return ret;
}

void ckit_enqueue(CKIT_Queue* queue, void* element) {
	ckit_assert(queue->count < queue->capacity);
	ckit_assert(!queue->has_next_to_read || queue->read_index != queue->write_index);
	queue->has_next_to_read = TRUE;

	queue->write_index = queue->write_index % queue->capacity;
	if (queue->is_pointer_type) {
		u8** temp_ptr = (u8**)((u8*)queue->data + (queue->element_size_in_bytes * queue->write_index++));
		*temp_ptr  = element;
	}  else {
		ckit_memory_copy(element, (u8*)queue->data + (queue->element_size_in_bytes * queue->write_index++), queue->element_size_in_bytes, queue->element_size_in_bytes);
	}

	queue->count++;
}

u32 ckit_queue_capacity(CKIT_Queue* queue) {
	return queue->capacity;
}

u32 ckit_queue_count(CKIT_Queue* queue) {
	return queue->count;
}
