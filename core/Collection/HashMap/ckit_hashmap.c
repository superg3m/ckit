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
	return (float)hashmap->count / (float)hashmap->capacity;
}

internal u64 ckit_hashmap_resolve_collision(CKIT_HashMap* hashmap, char* key, u64 inital_hash_index) {
    u64 cannonical_hash_index = inital_hash_index;
    while (hashmap->entries[cannonical_hash_index].key && !ckit_str_equal(hashmap->entries[cannonical_hash_index].key, key)) {
        cannonical_hash_index++;
        cannonical_hash_index = cannonical_hash_index % hashmap->capacity;
    }
    return cannonical_hash_index;
}

void ckit_hashmap_grow(CKIT_HashMap* hashmap) {
	if (ckit_hashmap_load_factor(hashmap) < CKIT_HASHMAP_DEFAULT_LOAD_FACTOR) {
		return;
	}

	u32 old_capacity = hashmap->capacity;
	hashmap->capacity *= 2;
	CKIT_HashMapEntry* temp_entries = ckit_alloc(sizeof(CKIT_HashMapEntry) * hashmap->capacity, MEMORY_TAG_TEMPORARY);
	ckit_memory_copy(hashmap->entries, temp_entries, sizeof(CKIT_HashMapEntry) * old_capacity, sizeof(CKIT_HashMapEntry) * hashmap->capacity);

	
	// rehash
	for (int i = 0; i < old_capacity; i++) {
		if (hashmap->entries[i].key != NULLPTR) {
			u32 index =  ckit_hash_value(hashmap->entries[i].key) % hashmap->capacity;
			u32 real_index = ckit_hashmap_resolve_collision(hashmap, hashmap->entries[i].key, index);

			temp_entries[real_index] = hashmap->entries[i];
		}
	}

	ckit_free(hashmap->entries);
	hashmap->entries = temp_entries;
}

CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size) {
	CKIT_HashMap* ret = ckit_alloc(sizeof(CKIT_HashMap), MEMORY_TAG_TEMPORARY);
	ret->capacity = 1;
	ret->entries = (CKIT_HashMapEntry*)ckit_alloc(sizeof(CKIT_HashMapEntry) * hashmap_capacity, MEMORY_TAG_TEMPORARY);
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
	hashmap->count = 0;
	hashmap->capacity = 0;
	ckit_vector_free(hashmap->entries);
	hashmap->element_size = 0;

	return hashmap;
}


Boolean ckit_hashmap_entry_exists(CKIT_HashMap* hashmap, u32 index) {
	return hashmap->entries[index].key != NULLPTR;
}

void ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value, void* possible_value_returned) {
	// update
	u32 index =  ckit_hash_value(key) % hashmap->capacity;
	u32 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

	if (ckit_hashmap_entry_exists(hashmap, real_index)) {
		//possible_value_returned = hashmap->entries[real_index].value;
	} else {
		hashmap->count++;
		//possible_value_returned = NULLPTR;
	}

	ckit_hashmap_grow(hashmap);

	if (hashmap->entries[real_index].key == NULLPTR) {
		hashmap->entries[real_index].key = key;
		hashmap->entries[real_index].value = ckit_alloc(hashmap->element_size, MEMORY_TAG_TEMPORARY);
	}

	ckit_memory_copy(value, hashmap->entries[real_index].value, hashmap->element_size, hashmap->element_size);
}

void* ckit_hashmap_get(CKIT_HashMap* hashmap, char* key) {
	u32 index =  ckit_hash_value(key) % hashmap->capacity;
	return hashmap->entries[index].value;
}

Boolean ckit_hashmap_has(CKIT_HashMap* hashmap, char* key) {
	u32 index =  ckit_hash_value(key) % hashmap->capacity;
	return hashmap->entries[index].key != NULLPTR;
}

// open addressing
// probing (linear or with double hashing)