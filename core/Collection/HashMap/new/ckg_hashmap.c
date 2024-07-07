#include "../ckg_hashmap.h"
#include "../../core/ckg_memory.h"
#include "../../core/ckg_assert.h"
#include "../ckg_hashing.h"

typedef struct CKG_HashMapEntry {
    char* key;
    void* value;
} CKG_HashMapEntry;

typedef struct CKG_HashMap {
    Boolean isValid;
    CKG_HashMapEntry* entries;
    size_t element_size;
    size_t capacity; // must be a power of two for the masking to work
    u32 count;
} CKG_HashMap;

#define HASHMAP_DEFAULT_LOAD_FACTOR 0.75

Boolean hashmap_entry_is_null(CKG_HashMapEntry entry) {
    return entry.key == NULLPTR && entry.value == NULLPTR;
}

internal CKG_HashMapEntry hashmap_entry_create() {
    CKG_HashMapEntry ret;
    ret.key = NULLPTR;
    ret.value = NULLPTR;
    return ret;
}

internal void hashmap_assert_if_not_valid(CKG_HashMap hashmap) {
    ckg_assert(hashmap.isValid);
}

internal float hashmap_load_factor(CKG_HashMap hashmap) {
    return hashmap.count / hashmap.capacity;
}

internal void hashmap_grow(CKG_HashMap* hashmap) {
    // new_hashmap = hashmap_create()
    for (int i = 0; i < hashmap->capacity; i++) {
        if (!hashmap_entry_is_null(hashmap->entries[i])) {
            continue;
        }

        // hashmap_put(*hashmap, )
    }
}

CKG_HashMap hashmap_create(size_t element_size) {
    CKG_HashMap hashmap;
    ckg_memory_zero(&hashmap, sizeof(CKG_HashMap));

    hashmap.capacity = 128;
    hashmap.entries = ckg_alloc(sizeof(CKG_HashMapEntry) * hashmap.capacity);
    hashmap.isValid = TRUE;
    hashmap.element_size = element_size;

    return hashmap;
}

void hashmap_free(CKG_HashMap* hashmap) {
    hashmap->isValid = FALSE;
    ckg_free(hashmap->entries);
    hashmap->element_size = 0;
    hashmap->capacity = 0;
    hashmap->count = 0;
}

u32 hashmap_get_valid_hash_index(CKG_HashMap hashmap, char* key) {
    return ckg_hash((unsigned char*)key) & (hashmap.capacity - 1);
}

// Date: July 01, 2024
// TODO(Jovanni): This is complete trash i'm to tired to actually code this right lmao
internal u64 hashmap_resolve_collision(CKG_HashMap hashmap, char* key, u64 inital_hash_index) {
    u64 cannonical_hash_index = inital_hash_index;
    while (!ckg_str_equal(hashmap.entries[cannonical_hash_index].value, key)) {
        cannonical_hash_index++;
        cannonical_hash_index = cannonical_hash_index % (hashmap.capacity - 1);
    }
    return cannonical_hash_index;
}

void hashmap_get(CKG_HashMap hashmap, char* key, void* value_returned) {
    hashmap_assert_if_not_valid(hashmap);
    u64 inital_hash_index = hashmap_get_valid_hash_index(hashmap, key);

    u64 cannonical_hash_index = hashmap_resolve_collision(hashmap, key, inital_hash_index);
    ckg_memory_copy(hashmap.entries[cannonical_hash_index].value, value_returned, hashmap.element_size, hashmap.element_size);
}

Boolean hashmap_has(CKG_HashMap hashmap, char* key) {
    hashmap_assert_if_not_valid(hashmap);
    u64 inital_hash_index = hashmap_get_valid_hash_index(hashmap, key);
    u64 cannonical_hash_index = hashmap_resolve_collision(hashmap, key, inital_hash_index);

    return hashmap_entry_is_null(hashmap.entries[cannonical_hash_index]);
}

void hashmap_put(CKG_HashMap* hashmap, char* key, void* value_set, void* possible_value_returned) {
    hashmap_assert_if_not_valid(*hashmap);
    if (hashmap_load_factor(*hashmap) >= HASHMAP_DEFAULT_LOAD_FACTOR) {
        hashmap_grow(hashmap);
    }


}
