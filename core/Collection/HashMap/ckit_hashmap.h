

/*===========================================================
 * File: ckit_hashmap.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../../ckit_types.h"
#include "../../String/ckit_string.h"

typedef u32(CKIT_HashFunction)(void* key);
typedef u32(CKIT_CompareFunction)(void* element);

//========================== Begin Types ==========================

typedef struct CKIT_HashMapEntry {
  	String key;
	void* value;
} CKIT_HashMapEntry;

typedef struct CKIT_HashMap {
	CKIT_HashFunction* hash_func;
	CKIT_CompareFunction* compare_func;
	CKIT_HashMapEntry* entries;
	float loadfactor;
	u32 capacity;
	size_t element_size;
} CKIT_HashMap;
//=========================== End Types ===========================

u32 compare_integer(void* known_integer_one, void* known_integer_two) {
	int* integer_one = (int*)known_integer_one;
	int* integer_two = (int*)known_integer_two;
	return *integer_one == *integer_one;
}

u32 compare_string(void* known_string_one, void* known_string_two) {
	char* string_one = (char*)known_string_one;
	char* string_two = (char*)known_string_two;
	return ckit_str_equal(string_one, string_two);
}

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
u32 ckit_hash_value(char *str);
CKIT_HashMap* MACRO_ckit_hashmap_create(CKIT_CompareFunction* compare_func, size_t element_size);
void MACRO_ckit_hashmap_create_custom(CKIT_HashFunction* hash_func, CKIT_CompareFunction* compare_func);
void* MACRO_ckit_hashmap_free(CKIT_HashMap* hashmap);

// Returns previous value
void MACRO_ckit_hashmap_put(CKIT_HashMap* hashmap, char* key, void* value, void* returned_value);
void ckit_hashmap_get(CKIT_HashMap* hashmap, char* key, void* returned_value);
Boolean ckit_hashmap_has(CKIT_HashMap* hashmap, char* key);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_hashmap_create(compare_func, type) MACRO_ckit_hashmap_create(compare_func, sizeof(type))
#define ckit_hashmap_put(hashmap, key, element) ckit_hashmap_put(hash_map, key, element)
#define ckit_hashmap_put(hashmap, key, element) ckit_hashmap_put(hash_map, key, element)
#define ckit_hashmap_free(hashmap) hashmap = MACRO_ckit_hashmap_free(hash_map)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
