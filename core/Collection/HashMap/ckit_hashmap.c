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
	ret.data = NULLPTR;
	ret.element_size = element_size;
	ret.loadfactor = 0.0;
}

void* MACRO_ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value) {
	if (hashmap->loadfactor >= 0.75) {
		ckit_vector_grow(hashmap->data, hashmap->element_size, TRUE);
	}

	// update

	// set

	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->data);
	// vector_insert(hash_map->data, index, value);
}

void* MACRO_ckit_hashmap_get(CKIT_HashMap* hashmap, char* key) {
	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->data);
	return NULLPTR;
}

Boolean MACRO_ckit_hashmap_has(CKIT_HashMap* hashmap, char* key) {
	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->data);
	return (hashmap->data + index) == NULLPTR;
}

// open addressing
// probing (linear or with double hashing)