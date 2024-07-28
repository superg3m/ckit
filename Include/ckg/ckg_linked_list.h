#pragma once

#include "ckg_types.h"
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
    Boolean is_pointer_type;
} CKG_LinkedList;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type);
    CKG_Node* ckg_linked_list_insert(CKG_LinkedList* linked_list, u32 index, void* data);
    CKG_Node* ckg_linked_list_get_node(CKG_LinkedList* linked_list, u32 index);
    void* ckg_linked_list_get(CKG_LinkedList* linked_list, u32 index);
    CKG_Node* ckg_linked_list_push(CKG_LinkedList* linked_list, void* data);
    CKG_Node ckg_linked_list_pop(CKG_LinkedList* linked_list);
    CKG_Node ckg_linked_list_remove(CKG_LinkedList* linked_list, u32 index);
    void* MACRO_ckg_linked_list_free(CKG_LinkedList* linked_list);
    u32 ckg_linked_list_node_to_index(CKG_LinkedList* linked_list, CKG_Node* address);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckg_linked_list_create(type, is_pointer_type) MACRO_ckg_linked_list_create(sizeof(type), is_pointer_type)
#define ckg_linked_list_free(linked_list) linked_list = MACRO_ckg_linked_list_free(linked_list)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++