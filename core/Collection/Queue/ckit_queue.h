// used a circular buffer

// read, next_read. and write pointers

// if the write pointer is greater than the next_read pointer then assert
// if read pointer is on next read pointer and you try to read again assert because you will be ahead of the write pointer
#include "../ckit_types.h"

typedef struct CKIT_Queue {
	u32 read_index;
	u32 next_index_to_read;
	u32 write_index;
} CKIT_Queue;

void ckit_queue_create(CKIT_Queue queue, void* returned_elemenet);
void ckit_queue_read(CKIT_Queue queue, void* returned_elemenet);
void ckit_queue_write(CKIT_Queue queue, void* element);