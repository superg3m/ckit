

/*===========================================================
 * File: ckit_hashmap.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckit_types.h"
#include "ckit_string.h"

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
