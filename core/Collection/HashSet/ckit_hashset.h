/*===========================================================
 * File: ckit_hashset.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "../../ckit_types.h"
#include "../../String/ckit_string.h"

typedef u32(CKIT_HashFunction)(void* key);
#define CKIT_HASHSET_DEFAULT_LOAD_FACTOR 0.75
//========================== Begin Types ==========================

typedef struct CKIT_HashSet {
	CKIT_HashFunction* hash_func;
	u32 capacity;
	u32 count;
	char** entries; // not a vector
} CKIT_HashSet;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
u32 ckit_hash_value(char *str);
CKIT_HashSet* MACRO_ckit_hashset_create(u32 hashset_capacity);
void MACRO_ckit_hashset_create_custom(CKIT_HashFunction* hash_func);
CKIT_HashSet* MACRO_ckit_hashset_free(CKIT_HashSet* hashset);

// Returns previous value
void ckit_hashset_put(CKIT_HashSet* hashset, char* key);
Boolean ckit_hashset_has(CKIT_HashSet* hashset, char* key);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_hashset_create(capacity, type) MACRO_ckit_hashset_create(capacity, sizeof(type))
#define ckit_hashset_free(hashset) hashset = MACRO_ckit_hashset_free(hashset)
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
