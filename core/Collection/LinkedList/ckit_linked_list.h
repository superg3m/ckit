#pragma once

#include "../core/ckit_types.h"
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
} CKIT_LinkedList;
//=========================== End Types ===========================

// Date: July 05, 2024
// TODO(Jovanni): THIS IS ALL BAD NEED TO RETHINK
//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes);
    CKIT_Node* MACRO_ckit_node_create(void* data, size_t element_size_in_bytes);
    void ckit_node_get(CKIT_Node* node, void* returned_value);
    CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data);
    CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index);
    void ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index, void* data);
    void ckit_linked_list_push(CKIT_LinkedList* linked_list, CKIT_Node* node);
    CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list, CKIT_Node* node);
    CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_linked_list_create(type) MACRO_ckit_linked_list_create(sizeof(type))
#define ckit_node_create(data_t) MACRO_ckit_node_create(data_t, sizeof(data_t))
#define ckit_node_free(node) node = MACRO_ckit_node_free(node)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++