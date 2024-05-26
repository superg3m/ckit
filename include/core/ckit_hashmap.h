

/*===========================================================
 * File: ckit_hashmap.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "ckit_types.h"
#include "ckit_string.h"



//========================== Begin Types ==========================

typedef struct HashMapEntry {
  String key;
  void* element;
} HashMapEntry;

typedef struct HashMap {
  
} HashMap;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif

void hashmap_create();
void* hashmap_insert(String key, void* element);

#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
