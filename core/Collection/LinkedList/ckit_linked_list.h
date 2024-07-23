#pragma once

#include "../../ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Node {
    struct CKIT_Node* next;
    struct CKIT_Node* prev;
    size_t element_size_in_bytes;
    void* data;
} CKIT_Node;

typedef struct CKIT_LinkedList {
    CKIT_Node* head;
    CKIT_Node* tail;
    size_t element_size_in_bytes;
    u32 count;
    Boolean is_pointer_type;
} CKIT_LinkedList;
//=========================== End Types ===========================

// Date: July 05, 2024
// TODO(Jovanni): THIS IS ALL BAD NEED TO RETHINK
//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type, char* file, u32 line, char* function);
    CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data);
    CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index);
    void* ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index);
    CKIT_Node* ckit_linked_list_push(CKIT_LinkedList* linked_list, void* data);
    CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list);
    CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index);
    void* MACRO_ckit_linked_list_free(CKIT_LinkedList* linked_list);
    u32 ckit_linked_list_node_to_index(CKIT_LinkedList* linked_list, CKIT_Node* address);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_linked_list_create(type, is_pointer_type) MACRO_ckit_linked_list_create(sizeof(type), is_pointer_type, __FILE__, __LINE__, __func__)
#define ckit_linked_list_free(linked_list) linked_list = MACRO_ckit_linked_list_free(linked_list)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++