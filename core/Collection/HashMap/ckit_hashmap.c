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

CKIT_HashMap MACRO_ckit_hashmap_create(CKIT_CompareFunction* compare_func, size_t element_size) {
	CKIT_HashMap ret;
	ret.capacity = 1;
	ret.compare_func = compare_func;
	ret.entries = NULLPTR;
	ret.element_size = element_size;
	ret.loadfactor = 0.0;
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

void* MACRO_ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value) {
	if (hashmap->loadfactor >= 0.75) {
		ckit_vector_grow(hashmap->entries, hashmap->element_size, TRUE);
	}

	// update

	// set

	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->entries);
	u32 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);
	hashmap->entries[real_index].value = value;
 
	// vector_insert(hash_map->data, index, value);
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