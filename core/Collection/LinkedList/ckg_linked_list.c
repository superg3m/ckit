#include "../ckg_linked_list.h"
#include "../../core/ckg_memory.h" 

CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes) {
    CKG_LinkedList* ret = ckg_alloc(sizeof(CKG_LinkedList));
    ret->count = 0;
    ret->element_size_in_bytes = element_size_in_bytes;
    ret->head = NULLPTR;
    ret->tail = NULLPTR;
    return ret;
}

// Date: July 05, 2024
// TODO(Jovanni): BADD THIS DOESN"T WORK
CKG_Node* MACRO_ckg_node_create(void* data, size_t element_size_in_bytes) {
    CKG_Node* ret = ckg_alloc(sizeof(CKG_Node)); 
    ret->data = ckg_alloc(element_size_in_bytes);
    ckg_memory_copy(data, ret->data, element_size_in_bytes, element_size_in_bytes); 
    ret->element_size_in_bytes = element_size_in_bytes;
    ret->next = NULLPTR;
    ret->prev = NULLPTR;
    return ret;
}

/**
 * @brief returns a null ptr
 * 
 * @param node 
 * @return CKG_Node* 
 */
CKG_Node* MACRO_ckg_node_free(CKG_Node* node) {
    ckg_free(node->data);
    node->element_size_in_bytes = 0;
    node->next = NULLPTR;
    node->prev = NULLPTR;
    ckg_free(node);
    return node;
}

void ckg_node_get(CKG_Node* node, void* returned_value) {
    ckg_memory_copy(node->data, returned_value, node->element_size_in_bytes, node->element_size_in_bytes);
}

CKG_Node* ckg_linked_list_insert(CKG_LinkedList linked_list, u32 index) {
    return NULL;
}

CKG_Node* ckg_linked_list_get_node(CKG_LinkedList linked_list, u32 index) {
    return NULL;
}

void ckg_linked_list_get(CKG_LinkedList linked_list, u32 index, void* data) {

}

CKG_Node* ckg_linked_list_push(CKG_LinkedList linked_list, CKG_Node* node) {
    return NULL;
}

CKG_Node* ckg_linked_list_pop(CKG_LinkedList linked_list, CKG_Node* node) {
    return NULL;
}

void ckg_linked_list_remove(CKG_LinkedList linked_list, u32 index) {

}
