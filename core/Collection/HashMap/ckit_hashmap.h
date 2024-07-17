

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
	void* element;
} HashMapEntry;

typedef struct CKIT_HashMap {
	CKIT_HashFunction* hash_func;
	CKIT_CompareFunction* compare_func;
	HashMapEntry* data;
	float loadfactor;
	u32 capacity;
	size_t element_size;
} HashMap;
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
void MACRO_ckit_hashmap_create(CKIT_CompareFunction* compare_func, size_t element_size);
void MACRO_ckit_hashmap_create_custom(CKIT_HashFunction* hash_func, CKIT_CompareFunction* compare_func);
void ckit_hashmap_insert(HashMap* hash_map, void* key, void* element);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define MACRO_ckit_hashmap_create(compare_func, type) MACRO_ckit_hashmap_create(0, sizeof(type))
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
