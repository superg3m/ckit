#pragma once

/*
    This hashmap uses double hashing
*/

#include "../core/ckg_types.h"
//========================== Begin Types ==========================
typedef struct CKG_HashMap CKG_HashMap;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    CKG_HashMap ckg_hashmap_create(size_t element_size);
    void ckg_hashmap_free(CKG_HashMap* hashmap);
    void ckg_hashmap_get(CKG_HashMap hashmap, char* key, void* value_returned);
    Boolean ckg_hashmap_has(CKG_HashMap hashmap, char* key);
    void ckg_hashmap_put(CKG_HashMap* hashmap, char* key, void* value_set, void* possible_value_returned);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++