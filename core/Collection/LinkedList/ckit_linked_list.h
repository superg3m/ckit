#include "ckit_types.h"

typedef struct GenericNode {
	void* data;
	struct GenericNode* next;
	struct GenericNode* previous;
} GenericNode;

typedef struct LinkedList {
	GenericNode* head;
	GenericNode* tail; // Will always be NULLPTR if doubley_linked is FALSE
	Boolean doubley_linked;
} LinkedList;

LinkedList linked_list_create();
LinkedList linked_list_iterator(LinkedList* linked_list, void* destination);
