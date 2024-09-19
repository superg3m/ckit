
#pragma once
/*===========================================================
 * File: ckit_hashmap.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../Basic/ckit_types.h"
#include "../Basic/ckit_string.h"

#define CKIT_HASHMAP_DEFAULT_LOAD_FACTOR 0.75
//========================== Begin Types ==========================

typedef struct CKIT_HashMapEntry {
  	String key;
	void* value;
} CKIT_HashMapEntry;

typedef struct CKIT_HashMap {
	u32 capacity;
	u32 count;
	CKIT_HashMapEntry* entries; // not a vector
	size_t element_size;
	Boolean is_pointer_type;
} CKIT_HashMap;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
u32 ckit_hash_value(char *str);
CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size, Boolean is_pointer_type);
CKIT_HashMap* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap);

// Returns previous value
void* ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value);
void* ckit_hashmap_get(CKIT_HashMap* hashmap, char* key);
Boolean ckit_hashmap_has(CKIT_HashMap* hashmap, char* key);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_hashmap_create(capacity, type, is_pointer_type) MACRO_ckit_hashmap_create(capacity, sizeof(type), is_pointer_type)
#define ckit_hashmap_free(hashmap) hashmap = MACRO_ckit_hashmap_free(hashmap)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
	#include "./ckit_vector.h"

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
		CKIT_HashMapEntry* new_entries = ckit_alloc_custom(sizeof(CKIT_HashMapEntry) * hashmap->capacity, TAG_CKIT_CORE_HASHMAP);
		
		// rehash
		for (u32 i = 0; i < old_capacity; i++) {
			if (hashmap->entries[i].key != NULLPTR) {
				u32 index =  ckit_hash_value(hashmap->entries[i].key) % hashmap->capacity;
				LOG_PRINT("String: %s\n", hashmap->entries[i].key);
				CKIT_HashMapEntry* cached_ptr = hashmap->entries;
				hashmap->entries = new_entries;
				u64 real_index = ckit_hashmap_resolve_collision(hashmap, cached_ptr[i].key, index);
				hashmap->entries = cached_ptr;

				new_entries[real_index] = hashmap->entries[i];
			}
		}

		ckit_free(hashmap->entries);
		hashmap->entries = new_entries;
	}

	CKIT_HashMap* MACRO_ckit_hashmap_create(u32 hashmap_capacity, size_t element_size, Boolean is_pointer_type) {
		CKIT_HashMap* ret = ckit_alloc_custom(sizeof(CKIT_HashMap), TAG_CKIT_CORE_HASHMAP);
		ret->capacity = 1;
		ret->entries = (CKIT_HashMapEntry*)ckit_alloc_custom(sizeof(CKIT_HashMapEntry) * hashmap_capacity, TAG_CKIT_CORE_HASHMAP);
		ret->element_size = element_size;
		ret->count = 0;
		ret->capacity = hashmap_capacity;
		ret->is_pointer_type = is_pointer_type;

		return ret;
	}

	CKIT_HashMapEntry ckit_hashmap_entry_create(CKIT_HashMap* hashmap, char* key, void* value) {
		CKIT_HashMapEntry entry;
		entry.key = key;
		if (hashmap->is_pointer_type) {
			entry.value = value;
		} else {
			entry.value = ckit_alloc_custom(hashmap->element_size, TAG_CKIT_EXPECTED_USER_FREE);
			ckit_memory_copy(value, entry.value, hashmap->element_size, hashmap->element_size);
		}

		return entry;
	}

	CKIT_HashMap* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap) {
		if (!hashmap->is_pointer_type) {
			for (u32 i = 0; i < hashmap->capacity; i++) {
				if (hashmap->entries[i].value) {
					ckit_free(hashmap->entries[i].value);
				}
			}
		}
		ckit_free(hashmap->entries);
		ckit_free(hashmap);

		return hashmap;
	}


	Boolean ckit_hashmap_entry_exists(CKIT_HashMap* hashmap, u32 index) {
		return hashmap->entries[index].key != NULLPTR;
	}

	void* ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value) {
		ckit_hashmap_grow(hashmap);

		void* ret = NULLPTR;

		u32 index =  ckit_hash_value(key) % hashmap->capacity;
		u64 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

		if (ckit_hashmap_entry_exists(hashmap, (u32)real_index)) {
			ret = hashmap->entries[real_index].value; // get previous value
			hashmap->entries[real_index] = ckit_hashmap_entry_create(hashmap, key, value);
		} else { // don't have the value
			hashmap->count++;
			hashmap->entries[real_index] = ckit_hashmap_entry_create(hashmap, key, value);
		}

		return ret;
	}

	void* ckit_hashmap_get(CKIT_HashMap* hashmap, char* key) {
		u32 index =  ckit_hash_value(key) % hashmap->capacity;
		u64 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

		return hashmap->entries[real_index].value;
	}

	Boolean ckit_hashmap_has(CKIT_HashMap* hashmap, char* key) {
		u32 index =  ckit_hash_value(key) % hashmap->capacity;
		u64 real_index = ckit_hashmap_resolve_collision(hashmap, key, index);

		return hashmap->entries[real_index].key != NULLPTR;
	}

	// open addressing
	// probing (linear or with double hashing)

#endif // CKIT_IMPL