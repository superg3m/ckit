#include "ckg_linked_list.h"
#include "ckg_memory.h" 
#include "ckg_assert.h"

CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes, Boolean is_pointer_type) {
    CKG_LinkedList* ret = ckg_alloc(sizeof(CKG_LinkedList));
    ret->count = 0;
    ret->element_size_in_bytes = element_size_in_bytes;
    ret->head = NULLPTR;
    ret->tail = NULLPTR;
    ret->is_pointer_type = is_pointer_type;
    return ret;
}

CKG_Node* MACRO_ckg_node_create(CKG_LinkedList* linked_list, void* data) {
    CKG_Node* ret = ckg_alloc(sizeof(CKG_Node));
    if (linked_list->is_pointer_type) {
        ret->data = data;
    } else {
        ret->data = ckg_alloc(linked_list->element_size_in_bytes); // user has to free
        ckg_memory_copy(data, ret->data, linked_list->element_size_in_bytes, linked_list->element_size_in_bytes); 
    }

    ret->element_size_in_bytes = linked_list->element_size_in_bytes;
    ret->next = NULLPTR;
    ret->prev = NULLPTR;
    return ret;
}
#define ckg_node_create(linked_list, data) MACRO_ckg_node_create(linked_list, data)

/**
 * @brief returns a null ptr
 * 
 * @param node 
 * @return CKG_Node* 
 */
internal CKG_Node* MACRO_ckg_node_free(CKG_LinkedList* linked_list, CKG_Node* node) {
    ckg_assert(linked_list);
    ckg_assert(node);
    node->element_size_in_bytes = 0;
    node->next = NULLPTR;
    node->prev = NULLPTR;
    ckg_free(node);

    return node;
}
#define ckg_node_free(linked_list, node) node = MACRO_ckg_node_free(linked_list, node)


/**
 * @brief returns a null ptr
 * 
 * @param node 
 * @return CKG_Node* 
 */
internal CKG_Node* MACRO_ckg_node_data_free(CKG_LinkedList* linked_list, CKG_Node* node) {
    ckg_assert(linked_list);
    ckg_assert(node);
    ckg_assert(node->data);
    node->element_size_in_bytes = 0;
    node->next = NULLPTR;
    node->prev = NULLPTR;
    if (!linked_list->is_pointer_type) {
        ckg_free(node->data);
    }
    ckg_free(node);

    return node;
}
#define ckg_node_data_free(linked_list, node) node = MACRO_ckg_node_data_free(linked_list, node)

CKG_Node* ckg_linked_list_insert(CKG_LinkedList* linked_list, u32 index, void* data) {
    ckg_assert(linked_list);
    ckg_assert(data);
    ckg_assert(index >= 0);

    u32 old_count = linked_list->count++;
    if (linked_list->head == NULLPTR) { // there is not head and by definition no tail
        CKG_Node* new_node_to_insert = ckg_node_create(linked_list, data);
        linked_list->head = new_node_to_insert;
        linked_list->tail = new_node_to_insert;

        return linked_list->head;
    }

    ckg_assert(index <= old_count);
    CKG_Node* new_node_to_insert = ckg_node_create(linked_list, data);

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

    // Date: July 19, 2024
    // TODO(Jovanni): check if index is closer to count or not then reverse the loop if approaching from the tail end.
    // as opposed to the head end.
    CKG_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    new_node_to_insert->prev = current_node;
    new_node_to_insert->next = current_node->next;

    current_node->next->prev = new_node_to_insert;
    current_node->next = new_node_to_insert;

    return new_node_to_insert;
}

CKG_Node* ckg_linked_list_get_node(CKG_LinkedList* linked_list, u32 index) {
    ckg_assert(linked_list);
    CKG_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node;
}

void* ckg_linked_list_get(CKG_LinkedList* linked_list, u32 index) {
    return ckg_linked_list_get_node(linked_list, index)->data;
}

CKG_Node* ckg_linked_list_push(CKG_LinkedList* linked_list, void* data) {
    return ckg_linked_list_insert(linked_list, linked_list->count, data);
}

u32 ckg_linked_list_node_to_index(CKG_LinkedList* linked_list, CKG_Node* address) {
    CKG_Node* current_node = linked_list->head; 
    for (int i = 0; i < linked_list->count + 1; i++) {
        if (current_node == address) {
            return i;
        }
        current_node = current_node->next;
    }

    ckg_assert(FALSE); // couldn't match a node to an address
    return -1; // should never get here
}

CKG_Node ckg_linked_list_pop(CKG_LinkedList* linked_list) {
    return ckg_linked_list_remove(linked_list, linked_list->count - 1);
}

CKG_Node ckg_linked_list_remove(CKG_LinkedList* linked_list, u32 index) {
    ckg_assert(linked_list); 
    ckg_assert(linked_list->count > 0); 
    ckg_assert(index >= 0);

    u32 old_count = linked_list->count--;
    if (index == 0 && old_count == 1) { // removing the head fully
        CKG_Node ret = *linked_list->head;
        ckg_node_free(linked_list, linked_list->head);
        linked_list->head = NULLPTR;

        return ret;
    }

    if (index == 0) { // remove head
        CKG_Node* cached_head = linked_list->head;
        linked_list->head = linked_list->head->next;
        CKG_Node ret = *cached_head; 
        ckg_node_free(linked_list, cached_head);

        return ret;
    }

    ckg_assert(index < old_count);
    if (index == (old_count - 1)) { // remove tail
        CKG_Node* cached_tail = linked_list->tail;
        linked_list->tail = linked_list->tail->prev;
        CKG_Node ret = *cached_tail; 
        ckg_node_free(linked_list, cached_tail);

        return ret;
    }

    CKG_Node* current_node = linked_list->head; 
    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    current_node->next->prev = current_node->prev;
    current_node->prev->next = current_node->next;
    CKG_Node ret = *current_node; 
    ckg_node_free(linked_list, current_node);

    return ret;
}

void* MACRO_ckg_linked_list_free(CKG_LinkedList* linked_list) {
    ckg_assert(linked_list); 
    CKG_Node* current_node = linked_list->head; 
    CKG_Node* next_node = NULLPTR; 
    for (int i = 0; i < linked_list->count; i++) {
        next_node = current_node->next;
        ckg_node_data_free(linked_list, current_node);
        current_node = next_node;
    }
    ckg_free(linked_list);

    return linked_list;
}