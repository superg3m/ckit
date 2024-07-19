#include "./ckit_queue.h"

CKIT_Queue* MACRO_ckit_queue_create(u32 inital_capacity, size_t element_size_in_bytes) {
	CKIT_Queue* ret = ckit_alloc(sizeof(CKIT_Queue), MEMORY_TAG_TEMPORARY);
	ret->capacity = inital_capacity;
	ret->count = 0;
	ret->data = ckit_alloc(element_size_in_bytes * ret->capacity);
	ret->data = ckit_alloc(element_size_in_bytes * ret->capacity);
}

void ckit_dequeue(CKIT_Queue* queue, void* returned_elemenet) {
	ckit_assert(queue->has_next_to_read && queue->read_index != queue->write_index);
	queue->has_next_to_read = FALSE;

	ckit_memory_copy(queue->data, returned_elemenet, queue->element_size_in_bytes, queue->element_size_in_bytes);

}

void ckit_enqueue(CKIT_Queue* queue, void* element) {
	ckit_assert(!queue->has_next_to_read && queue->read_index != queue->write_index);

	queue->has_next_to_read = TRUE;
}

u32 ckit_queue_capacity(CKIT_Queue* queue) {
	return queue->capacity;
}

u32 ckit_queue_count(CKIT_Queue* queue) {
	return queue->count;
}
