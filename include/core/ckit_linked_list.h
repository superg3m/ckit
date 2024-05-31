#include "ckit_types.h"

typedef struct GenericNode {
	struct GenericNode* next;
	struct GenericNode* previous;
	void* data;
} GenericNode;

typedef struct LinkedList {
	GenericNode* head;
	GenericNode* tail; // Will always be NULLPTR if doubley_linked is FALSE
	Boolean doubley_linked;
} LinkedList;