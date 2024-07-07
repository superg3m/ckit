#pragma once

#include "../core/ckg_types.h"
//========================== Begin Types ==========================
typedef struct CKG_Node {
    struct CKG_Node* next;
    struct CKG_Node* prev;
    size_t element_size_in_bytes;
    void* data;
} CKG_Node;

typedef struct CKG_LinkedList {
    CKG_Node* head;
    CKG_Node* tail;
    size_t element_size_in_bytes;
    u32 count;
} CKG_LinkedList;
//=========================== End Types ===========================

// Date: July 05, 2024
// TODO(Jovanni): THIS IS ALL BAD NEED TO RETHINK
//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes);
    CKG_Node* MACRO_ckg_node_create(void* data, size_t element_size_in_bytes);
    void ckg_node_get(CKG_Node* node, void* returned_value);
    CKG_Node* ckg_linked_list_insert(CKG_LinkedList linked_list, u32 index);
    CKG_Node* ckg_linked_list_get_node(CKG_LinkedList linked_list, u32 index);
    void ckg_linked_list_get(CKG_LinkedList linked_list, u32 index, void* data);
    CKG_Node* ckg_linked_list_push(CKG_LinkedList linked_list, CKG_Node* node);
    CKG_Node* ckg_linked_list_pop(CKG_LinkedList linked_list, CKG_Node* node);
    void ckg_linked_list_remove(CKG_LinkedList linked_list, u32 index);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckg_linked_list_create(type) MACRO_ckg_linked_list_create(sizeof(type))
#define ckg_node_create(data_t) MACRO_ckg_node_create(data_t, sizeof(data_t))
#define ckg_node_free(node) node = MACRO_ckg_node_free(node)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++