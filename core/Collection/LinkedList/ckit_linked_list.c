#include "./ckit_linked_list.h"
#include "../../Memory/ckit_memory.h" 

CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes) {
    CKIT_LinkedList* ret = ckit_alloc(sizeof(CKIT_LinkedList), MEMORY_TAG_TEMPORARY);
    ret->count = 0;
    ret->element_size_in_bytes = element_size_in_bytes;
    ret->head = NULLPTR;
    ret->tail = NULLPTR;
    return ret;
}

// Date: July 05, 2024
// TODO(Jovanni): BADD THIS DOESN"T WORK
// Date: July 19, 2024
// NOTE(Jovanni): I have no idea why this doesn't work...? Was this guy ok or i'm I not ok?
CKIT_Node* MACRO_ckit_node_create(void* data, size_t element_size_in_bytes) {
    CKIT_Node* ret = ckit_alloc(sizeof(CKIT_Node), MEMORY_TAG_TEMPORARY); 
    ret->data = ckit_alloc(element_size_in_bytes, MEMORY_TAG_TEMPORARY);
    ckit_memory_copy(data, ret->data, element_size_in_bytes, element_size_in_bytes); 
    ret->element_size_in_bytes = element_size_in_bytes;
    ret->next = NULLPTR;
    ret->prev = NULLPTR;
    return ret;
}

/**
 * @brief returns a null ptr
 * 
 * @param node 
 * @return CKIT_Node* 
 */
CKIT_Node* MACRO_ckit_node_free(CKIT_Node* node) {
    ckit_free(node->data);
    node->element_size_in_bytes = 0;
    node->next = NULLPTR;
    node->prev = NULLPTR;
    ckit_free(node);
    return node;
}

void ckit_node_get(CKIT_Node* node, void* returned_value) {
    ckit_memory_copy(node->data, returned_value, node->element_size_in_bytes, node->element_size_in_bytes);
}

CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data) {
    CKIT_Node* new_node_to_insert = ckit_node_create(data); 
    if (index == 0) { // insert head
        linked_list->head->prev = new_node_to_insert;
        new_node_to_insert->next = linked_list->head;
        linked_list->head = new_node_to_insert;
        return new_node_to_insert;
    }

    if (linked_list->count == index - 1) { // insert tail
        linked_list->tail->next = new_node_to_insert;
        new_node_to_insert->prev = linked_list->tail;
        linked_list->tail = new_node_to_insert;
        return new_node_to_insert;
    }

    // Date: July 19, 2024
    // TODO(Jovanni): check if index is closer to count or not then reverse the loop if approaching from the tail end.
    // as opposed to the head end.
    CKIT_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        CKIT_Node* current_node = current_node->next;
    }

    new_node_to_insert->prev = current_node;
    new_node_to_insert->next = current_node->next;

    current_node->next->prev = new_node_to_insert;
    current_node->next = new_node_to_insert;

    return new_node_to_insert;
}

CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index) {
    return NULL;
}

void ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index, void* data) {

}

void ckit_linked_list_push(CKIT_LinkedList* linked_list, CKIT_Node* node) {
    return;
}

CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list, CKIT_Node* node) {
    return ckit_linked_list_remove(linked_list, linked_list->count - 1);
}

CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index) {
    if (index == 0) { // remove head
        CKIT_Node* cached_head = linked_list->head;
        linked_list->head = linked_list->head->next;
        CKIT_Node ret = *cached_head; 
        ckit_node_free(cached_head);

        return ret;
    }

    if (linked_list->count == index - 1) { // remove tail
        CKIT_Node* cached_tail = linked_list->tail;
        linked_list->tail = linked_list->tail->prev;
        CKIT_Node ret = *cached_tail; 
        ckit_node_free(cached_tail);

        return ret;
    }

    CKIT_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        CKIT_Node* current_node = current_node->next;
    }

    current_node->next->prev = current_node->prev;
    current_node->prev->next = current_node->next;
    CKIT_Node ret = *current_node; 
    ckit_node_free(current_node);

    return ret;
}
