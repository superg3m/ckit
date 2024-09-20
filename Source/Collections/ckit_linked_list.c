#pragma once

#include "../../Include/Collections/ckit_linked_list.h"

#include  "../../Include/Basic/ckit_memory.h" 
#include  "../../Include/Basic/ckit_assert.h"

CKIT_LinkedList* MACRO_ckit_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type, const char* file, const u32 line, const char* function) {
    CKIT_LinkedList* ret = MACRO_ckit_alloc(sizeof(CKIT_LinkedList), TAG_CKIT_CORE_LINKED_LIST, file, line, function);
    ret->count = 0;
    ret->element_size_in_bytes = element_size_in_bytes;
    ret->head = NULLPTR;
    ret->tail = NULLPTR;
    ret->is_pointer_type = is_pointer_type;
    return ret;
}

CKIT_Node* MACRO_ckit_node_create(CKIT_LinkedList* linked_list, void* data) {
    CKIT_Node* ret = ckit_alloc_custom(sizeof(CKIT_Node), TAG_CKIT_CORE_LINKED_LIST);
    if (linked_list->is_pointer_type) {
        ret->data = data;
    } else {
        ret->data = ckit_alloc_custom(linked_list->element_size_in_bytes, TAG_CKIT_EXPECTED_USER_FREE); // user has to free
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
internal CKIT_Node* MACRO_ckit_node_free(CKIT_LinkedList* linked_list, CKIT_Node* node) {
    ckit_assert(linked_list);
    ckit_assert(node);
    node->element_size_in_bytes = 0;
    node->next = NULLPTR;
    node->prev = NULLPTR;
    ckit_free(node);

    return node;
}
#define ckit_node_free(linked_list, node) node = MACRO_ckit_node_free(linked_list, node)


/**
 * @brief returns a null ptr
 * 
 * @param node 
 * @return CKIT_Node* 
 */
internal CKIT_Node* MACRO_ckit_node_data_free(CKIT_LinkedList* linked_list, CKIT_Node* node) {
    ckit_assert(linked_list);
    ckit_assert(node);
    ckit_assert(node->data);
    node->element_size_in_bytes = 0;
    node->next = NULLPTR;
    node->prev = NULLPTR;
    if (!linked_list->is_pointer_type) {
        ckit_free(node->data);
    }
    ckit_free(node);

    return node;
}
#define ckit_node_data_free(linked_list, node) node = MACRO_ckit_node_data_free(linked_list, node)

CKIT_Node* ckit_linked_list_insert(CKIT_LinkedList* linked_list, u32 index, void* data) {
    ckit_assert(linked_list);
    ckit_assert(data);
    ckit_assert(index >= 0);

    u32 old_count = linked_list->count++;
    if (linked_list->head == NULLPTR) { // there is not head and by definition no tail
        CKIT_Node* new_node_to_insert = ckit_node_create(linked_list, data);
        linked_list->head = new_node_to_insert;
        linked_list->tail = new_node_to_insert;

        return linked_list->head;
    }

    ckit_assert(index <= old_count);
    CKIT_Node* new_node_to_insert = ckit_node_create(linked_list, data);

    if (index == 0) { // insert at head
        linked_list->head->prev = new_node_to_insert;
        new_node_to_insert->next = linked_list->head;
        linked_list->head = new_node_to_insert;
        
        return new_node_to_insert;
    }

    if (index == old_count) { // insert at tail
        linked_list->tail->next = new_node_to_insert;
        new_node_to_insert->prev = linked_list->tail;
        linked_list->tail = new_node_to_insert;

        return new_node_to_insert;
    }

    CKIT_Node* current_node = linked_list->head; 
    for (u32 i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    new_node_to_insert->prev = current_node;
    new_node_to_insert->next = current_node->next;

    current_node->next->prev = new_node_to_insert;
    current_node->next = new_node_to_insert;

    return new_node_to_insert;
}

CKIT_Node* ckit_linked_list_get_node(CKIT_LinkedList* linked_list, u32 index) {
    ckit_assert(linked_list);
    CKIT_Node* current_node = linked_list->head; 
    for (u32 i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node;
}

void* ckit_linked_list_get(CKIT_LinkedList* linked_list, u32 index) {
    return ckit_linked_list_get_node(linked_list, index)->data;
}

void* ckit_linked_list_peek_head(CKIT_LinkedList* linked_list) {
    return ckit_linked_list_get_node(linked_list, 0)->data;
}

void* ckit_linked_list_peek_tail(CKIT_LinkedList* linked_list) {
    return ckit_linked_list_get_node(linked_list, linked_list->count - 1)->data;
}

CKIT_Node* ckit_linked_list_push(CKIT_LinkedList* linked_list, void* data) {
    return ckit_linked_list_insert(linked_list, linked_list->count, data);
}

u32 ckit_linked_list_node_to_index(CKIT_LinkedList* linked_list, CKIT_Node* address) {
    CKIT_Node* current_node = linked_list->head; 
    for (u32 i = 0; i < linked_list->count + 1; i++) {
        if (current_node == address) {
            return i;
        }
        current_node = current_node->next;
    }

    ckit_assert(FALSE); // couldn't match a node to an address
    return 0; // should never get here
}

CKIT_Node ckit_linked_list_pop(CKIT_LinkedList* linked_list) {
    return ckit_linked_list_remove(linked_list, linked_list->count - 1);
}

CKIT_Node ckit_linked_list_remove(CKIT_LinkedList* linked_list, u32 index) {
    ckit_assert(linked_list); 
    ckit_assert(linked_list->count > 0); 
    ckit_assert(index >= 0);

    u32 old_count = linked_list->count--;
    if (index == 0 && old_count == 1) { // removing the head fully
        CKIT_Node ret = *linked_list->head;
        ckit_node_free(linked_list, linked_list->head);
        linked_list->head = NULLPTR;

        return ret;
    }

    if (index == 0) { // remove head
        CKIT_Node* cached_head = linked_list->head;
        linked_list->head = linked_list->head->next;
        CKIT_Node ret = *cached_head; 
        ckit_node_free(linked_list, cached_head);

        return ret;
    }

    ckit_assert(index < old_count);
    if (index == (old_count - 1)) { // remove tail
        CKIT_Node* cached_tail = linked_list->tail;
        linked_list->tail = linked_list->tail->prev;
        CKIT_Node ret = *cached_tail; 
        ckit_node_free(linked_list, cached_tail);

        return ret;
    }

    CKIT_Node* current_node = linked_list->head; 
    for (u32 i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    current_node->next->prev = current_node->prev;
    current_node->prev->next = current_node->next;
    CKIT_Node ret = *current_node; 
    ckit_node_free(linked_list, current_node);

    return ret;
}

void* MACRO_ckit_linked_list_free(CKIT_LinkedList* linked_list) {
    ckit_assert(linked_list); 
    CKIT_Node* current_node = linked_list->head; 
    CKIT_Node* next_node = NULLPTR; 
    for (u32 i = 0; i < linked_list->count; i++) {
        next_node = current_node->next;
        ckit_node_data_free(linked_list, current_node);
        current_node = next_node;
    }
    ckit_free(linked_list);

    return linked_list;
}