#include "./ckit_queue.h"
#include "../../Assert/ckit_assert.h"

CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes) {
	CKIT_Queue* ret = ckit_alloc(sizeof(CKIT_Queue), MEMORY_TAG_TEMPORARY);
	ret->element_size_in_bytes = element_size_in_bytes;
	ret->capacity = inital_capacity;
	ret->count = 0;
	ret->data = ckit_alloc(element_size_in_bytes * ret->capacity, MEMORY_TAG_TEMPORARY);
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
void ckit_dequeue(CKIT_Queue* queue, void* returned_element) {
	ckit_assert(queue->count > 0);
	ckit_assert(queue->has_next_to_read || queue->read_index != queue->write_index);
	queue->has_next_to_read = FALSE;
	
	queue->read_index = queue->read_index % queue->capacity;
	ckit_memory_copy((u8*)queue->data + (queue->element_size_in_bytes * queue->read_index++), returned_element, queue->element_size_in_bytes, queue->element_size_in_bytes);
	queue->count--;
}

void ckit_enqueue(CKIT_Queue* queue, void* element) {
	ckit_assert(queue->count < queue->capacity);
	ckit_assert(!queue->has_next_to_read || queue->read_index != queue->write_index);
	queue->has_next_to_read = TRUE;

	queue->write_index = queue->write_index % queue->capacity;
	ckit_memory_copy(element, (u8*)queue->data + (queue->element_size_in_bytes * queue->write_index++), queue->element_size_in_bytes, queue->element_size_in_bytes);
	queue->count++;
}

u32 ckit_queue_capacity(CKIT_Queue* queue) {
	return queue->capacity;
}

u32 ckit_queue_count(CKIT_Queue* queue) {
	return queue->count;
}
