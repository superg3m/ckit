#include "./ckit_linked_list.h"
#include "../../Memory/ckit_memory.h" 
#include "../../Assert/ckit_assert.h"

CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type) {
    CKIT_LinkedList* ret = ckit_alloc(sizeof(CKIT_LinkedList), MEMORY_TAG_TEMPORARY);
    ret->count = 0;
    ret->element_size_in_bytes = element_size_in_bytes;
    ret->head = NULLPTR;
    ret->tail = NULLPTR;
    ret->is_pointer_type = is_pointer_type;
    return ret;
}

CKIT_Node* MACRO_ckit_node_create(CKIT_LinkedList* linked_list, void* data) {
    CKIT_Node* ret = ckit_alloc(sizeof(CKIT_Node), MEMORY_TAG_TEMPORARY);
    if (linked_list->is_pointer_type) {
        ret->data = data;
    } else {
        ret->data = ckit_alloc(linked_list->element_size_in_bytes, MEMORY_TAG_TEMPORARY);
        ckit_memory_copy(data, ret->data, linked_list->element_size_in_bytes, linked_list->element_size_in_bytes); 
    }

    ret->element_size_in_bytes = linked_list->element_size_in_bytes;
    ret->next = NULLPTR;
    ret->prev = NULLPTR;
    return ret;
}
#define ckit_node_create(linked_list, data) MACRO_ckit_node_create(linked_list, data)

/**
 * @brief returns a null ptr
 * 
 * @param node 
 * @return CKIT_Node* 
 */
CKIT_Node* MACRO_ckit_node_free(CKIT_LinkedList* linked_list, CKIT_Node* node) {
    node->element_size_in_bytes = 0;
    node->next = NULLPTR;
    node->prev = NULLPTR;
    ckit_free(node);
    return node;
}

CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data) {
    if (linked_list->head == NULLPTR) { // there is not head and by definition no tail
        CKIT_Node* new_node_to_insert = ckit_node_create(linked_list, data);
        linked_list->head = new_node_to_insert;
        linked_list->tail = new_node_to_insert;
        linked_list->count++;
        return linked_list->head;
    }

    ckit_assert(index <= linked_list->count);
    ckit_assert(index >= 0);

    CKIT_Node* new_node_to_insert = ckit_node_create(linked_list, data);
    linked_list->count++;
    if (index == 0) { // insert at head
        linked_list->head->prev = new_node_to_insert;
        new_node_to_insert->next = linked_list->head;
        linked_list->head = new_node_to_insert;
        
        return new_node_to_insert;
    }

    if (index == linked_list->count - 1) { // insert at tail
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
        current_node = current_node->next;
    }

    new_node_to_insert->prev = current_node;
    new_node_to_insert->next = current_node->next;

    current_node->next->prev = new_node_to_insert;
    current_node->next = new_node_to_insert;

    return new_node_to_insert;
}

CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index) {
    CKIT_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node;
}

void* ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index) {
    CKIT_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node->data;
}

CKIT_Node* ckit_linked_list_push(CKIT_LinkedList* linked_list, void* data) {
    return ckit_linked_list_insert(linked_list, linked_list->count, data);
}

CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list) {
    return ckit_linked_list_remove(linked_list, linked_list->count);
}

CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index) {
    ckit_assert(linked_list->count != 0); 
    ckit_assert(index <= linked_list->count);
    ckit_assert(index >= 0);


    if (index == 0) { // remove head
        CKIT_Node* cached_head = linked_list->head;
        linked_list->head = linked_list->head->next;
        CKIT_Node ret = *cached_head; 
        ckit_node_free(linked_list, cached_head);
        linked_list->count--;

        return ret;
    }

    if (index == linked_list->count) { // remove tail
        CKIT_Node* cached_tail = linked_list->tail;
        linked_list->tail = linked_list->tail->prev;
        CKIT_Node ret = *cached_tail; 
        ckit_node_free(linked_list, cached_tail);
        linked_list->count--;

        return ret;
    }

    CKIT_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    current_node->next->prev = current_node->prev;
    current_node->prev->next = current_node->next;
    CKIT_Node ret = *current_node; 
    ckit_node_free(linked_list, current_node);

    linked_list->count--;

    return ret;
}
