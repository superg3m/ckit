

/*===========================================================
 * File: ckit_hashmap.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckit_types.h"
#include "ckit_string.h"

typedef u32(HashFunction)(void* key);
typedef u32(CompareFunction)(void* element);

//========================== Begin Types ==========================

typedef struct HashMapEntry {
  	String key;
	void* element;
	size_t element_size;
} HashMapEntry;

typedef struct HashMap {

	HashFunction* hash_func;
	CompareFunction* compare_func;
	HashMapEntry* data;
	float loadfactor;
	u32 capacity;
} HashMap;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
u32 hash_value(unsigned char *str);
void hashmap_create(HashFunction* hash_func, CompareFunction* compare_func);
void* hashmap_insert(HashMap hash_map, void* key, void* element);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
