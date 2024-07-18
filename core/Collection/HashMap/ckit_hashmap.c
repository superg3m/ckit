#include "./ckit_hashmap.h"
#include "../Vector/ckit_vector.h"

u32 ckit_hash_value(char *str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

float ckit_hashmap_load_factor(CKIT_HashMap* hashmap) {
	return (float)ckit_vector_count(hashmap->entries) / (float)ckit_vector_capacity(hashmap->entries);
}

void ckit_hashmap_grow(CKIT_HashMap* hashmap) {
	u32 old_capacity = hashmap->capacity;
	hashmap->capacity *= 2;
	CKIT_HashMapEntry* temp_entries = ckit_alloc(hashmap->element_size * hashmap->capacity, MEMORY_TAG_TEMPORARY);
	ckit_memory_copy(hashmap->entries, temp_entries, hashmap->element_size * old_capacity, hashmap->element_size * hashmap->capacity);

	
	// rehash
	for (int i = 0; i < old_capacity; i++) {
		if (hashmap->entries[i].key != NULLPTR) {
			u32 index =  ckit_hash_value(hashmap->entries[i].key) % vector_capacity(hashmap->entries);
			u32 real_index = ckit_hashmap_resolve_collision(hashmap, hashmap->entries[i].key, index);

			temp_entries[real_index] = hashmap->entries[i];
		}
	}

	ckit_free(hashmap->entries);
	hashmap->entries = temp_entries;
}

CKIT_HashMap* MACRO_ckit_hashmap_create(CKIT_CompareFunction* compare_func, u32 hashmap_capacity, size_t element_size) {
	CKIT_HashMap* ret = ckit_alloc(sizeof(CKIT_HashMap), MEMORY_TAG_TEMPORARY);
	ret->capacity = 1;
	ret->compare_func = compare_func;
	ret->entries = (CKIT_HashMapEntry*)ckit_alloc(element_size * hashmap_capacity, MEMORY_TAG_TEMPORARY);
	ret->element_size = element_size;
	ret->count = 0;
	ret->capacity = hashmap_capacity;

	return ret;
}

CKIT_HashMapEntry ckit_hashmap_entry_create(char* key, void* value) {
	CKIT_HashMapEntry entry;
	entry.key = key;
	value = value;
	return entry;
}

CKIT_HashMap* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap) {
	hashmap->capacity = 0;
	hashmap->compare_func = 0;
	ckit_vector_free(hashmap->entries);
	hashmap->element_size = 0;
	hashmap->count = 0;
	hashmap->capacity = 0;

	return hashmap;
}

// Date: July 01, 2024
// TODO(Jovanni): This is complete trash i'm to tired to actually code this right lmao
// Date: July 17, 2024
// NOTE(Jovanni): I'm a bit less tired not and this doesn't seem to bad, its just linear probing nothing diabolical here???
internal u64 ckit_hashmap_resolve_collision(CKIT_HashMap* hashmap, char* key, u64 inital_hash_index) {
    u64 cannonical_hash_index = inital_hash_index;
    while (!ckit_str_equal(hashmap->entries[cannonical_hash_index].key, key)) {
        cannonical_hash_index++;
        cannonical_hash_index = cannonical_hash_index % (hashmap->capacity - 1);
    }
    return cannonical_hash_index;
}

Boolean ckit_hashmap_entry_exists(CKIT_HashMap* hashmap, u32 index) {
	return hashmap->entries[index].key != NULLPTR;
}

void MACRO_ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value, void* possible_value_returned) {
	// update
	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->entries);
	u32 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

	if (ckit_hashmap_entry_exists(hashmap, real_index)) {
		hashmap->count++;
		possible_value_returned = NULLPTR;
	} else {
		possible_value_returned = hashmap->entries[real_index].value;
	}

	if (ckit_hashmap_load_factor(hashmap) >= CKIT_HASHMAP_DEFAULT_LOAD_FACTOR) {
		ckit_hashmap_grow(hashmap->entries, hashmap->element_size);
	}

	// set
	if (hashmap->entries[real_index].value == NULLPTR) {
		hashmap->entries[real_index].value = ckit_alloc(hashmap->element_size, MEMORY_TAG_TEMPORARY);
	}

	ckit_memory_copy(value, hashmap->entries[real_index].value, hashmap->element_size, hashmap->element_size);
}

void* MACRO_ckit_hashmap_get(CKIT_HashMap* hashmap, char* key) {
	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->entries);
	return NULLPTR;
}

Boolean MACRO_ckit_hashmap_has(CKIT_HashMap* hashmap, char* key) {
	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->entries);
	return (hashmap->entries + index) == NULLPTR;
}

// open addressing
// probing (linear or with double hashing)