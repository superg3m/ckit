#include "../../include/core/ckit_hashmap.h"
#include "../../include/core/ckit_vector.h"

u32 hash_value(unsigned char *str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}
		

	return hash;
}

void hash_map_insert(HashMap* hash_map, String key, void* value) {
	if (hash_map->loadfactor >= 0.75) {
		vector_grow(hash_map->data);
	}

	// update

	// set

	u32 index =  hash_value(key) % vector_capacity(hash_map->data);
	vector_insert(hash_map->data, index, value);
}

