
#include "../CKit.h"
/*===========================================================
 * File: CKit.c
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

global_variable Boolean CKit_is_initalized = FALSE;

//************************* Begin Functions *************************
Boolean memory_init();
Boolean arena_init();
extern Arena** arena_vector;


#ifdef __cplusplus
extern "C" {
#endif
  void CKit_init() {
    assert_in_function(!CKit_is_initalized, "Ckit_init: CKit is already initalized")

    assert_in_function(memory_init(), "memory_init: failed\n");
    assert_in_function(arena_init(), "arena_init: failed\n");
    arena_vector = vector_create(Arena*);
    //assert_in_function(vector_init(), "vector_init: failed\n");
    //assert_in_function(string_init(), "string_init: failed\n");
    //assert_in_function(event_init(), "event_init: failed\n");
    //assert_in_function(memory_init(), "memory_init: failed\n");

    CKit_is_initalized = TRUE;
  }
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
