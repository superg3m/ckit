#include "./ckit_hashset.h"
#include "../Vector/ckit_vector.h"

u32 ckit_hash_value(char *str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

float ckit_hashset_load_factor(CKIT_HashSet* hashset) {
	return (float)hashset->count / (float)hashset->capacity;
}

internal u64 ckit_hashset_resolve_collision(CKIT_HashSet* hashset, char* key, u64 inital_hash_index) {
    u64 cannonical_hash_index = inital_hash_index;
    while (hashset->entries[cannonical_hash_index] && !ckit_str_equal(hashset->entries[cannonical_hash_index], key)) {
        cannonical_hash_index++;
        cannonical_hash_index = cannonical_hash_index % hashset->capacity;
    }
    return cannonical_hash_index;
}

void ckit_hashset_grow(CKIT_HashSet* hashset) {
	if (ckit_hashset_load_factor(hashset) < CKIT_HASHSET_DEFAULT_LOAD_FACTOR) {
		return;
	}

	u32 old_capacity = hashset->capacity;
	hashset->capacity *= 2;
	char** temp_entries = ckit_alloc(sizeof(char*) * hashset->capacity, MEMORY_TAG_TEMPORARY);
	
	// rehash
	for (int i = 0; i < old_capacity; i++) {
		if (hashset->entries[i] != NULLPTR) {
			u32 index =  ckit_hash_value(hashset->entries[i]) % hashset->capacity;
			LOG_PRINT("String: %s\n", hashset->entries[i]);
			char** cached_ptr = hashset->entries;
			hashset->entries = temp_entries;
			u32 real_index = ckit_hashset_resolve_collision(hashset, cached_ptr[i], index);
			hashset->entries = cached_ptr;

			temp_entries[real_index] = hashset->entries[i];
		}
	}

	ckit_free(hashset->entries);
	hashset->entries = temp_entries;
}

CKIT_HashSet* MACRO_ckit_hashset_create(u32 hashset_capacity) {
	CKIT_HashSet* ret = ckit_alloc(sizeof(CKIT_HashSet), MEMORY_TAG_TEMPORARY);
	ret->capacity = 1;
	ret->entries = (char**)ckit_alloc(sizeof(char*) * hashset_capacity, MEMORY_TAG_TEMPORARY);
	ret->count = 0;
	ret->capacity = hashset_capacity;

	return ret;
}

CKIT_HashSet* MACRO_ckit_hashset_free(CKIT_HashSet* hashset) {
	ckit_free(hashset);

	return hashset;
}


Boolean ckit_hashset_entry_exists(CKIT_HashSet* hashset, u32 index) {
	return hashset->entries[index] != NULLPTR;
}

void ckit_hashset_put(CKIT_HashSet* hashset, char* key) {
	ckit_hashset_grow(hashset);

	u32 index =  ckit_hash_value(key) % hashset->capacity;
	u32 real_index = ckit_hashset_resolve_collision(hashset, key, index);
	if (hashset->entries[real_index] == NULLPTR) {
		hashset->entries[real_index] = key;
	}
}

Boolean ckit_hashset_has(CKIT_HashSet* hashset, char* key) {
	u32 index =  ckit_hash_value(key) % hashset->capacity;
	u32 real_index = ckit_hashset_resolve_collision(hashset, key, index);

	return hashset->entries[real_index] != NULLPTR;
}

// open addressing
// probing (linear or with double hashing)