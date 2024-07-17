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

void ckit_hashmap_insert(HashMap* hashmap, String key, void* value) {
	if (hashmap->loadfactor >= 0.75) {
		ckit_vector_grow(hashmap->data, hashmap->);
	}

	// update

	// set

	u32 index =  ckit_hash_value(key) % vector_capacity(hashmap->data);
	// vector_insert(hash_map->data, index, value);
}

// open addressing
// probing (linear or with double hashing)