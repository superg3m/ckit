
#include "../CKit.h"
/*===========================================================
 * File: CKit.c
 * Date: May 11, 2024
 * Creator: Jovanni Djonaj
===========================================================*/

internal Boolean CKit_is_initalized = FALSE;
Boolean memory_init();
Boolean arena_init();
Boolean string_init();
extern Arena** arena_vector;


void CKit_init() {
  	assert_in_function(!CKit_is_initalized, "Ckit_init: CKit is already initalized")

  	assert_in_function(memory_init(), "memory_init: failed\n");
  	assert_in_function(arena_init(), "arena_init: failed\n");
  	arena_vector = vector_create(Arena*);
  	assert_in_function(string_init(), "string_init: failed\n");
  	//assert_in_function(vector_init(), "vector_init: failed\n");
  	//assert_in_function(event_init(), "event_init: failed\n");
  	//assert_in_function(memory_init(), "memory_init: failed\n");

  	CKit_is_initalized = TRUE;
}

void CKit_cleanup() {
  	assert_in_function(!CKit_is_initalized, "Ckit_init: CKit is already initalized")

  	assert_in_function(memory_init(), "memory_init: failed\n");
  	assert_in_function(arena_init(), "arena_init: failed\n");
  	arena_vector = vector_create(Arena*);
  	assert_in_function(string_init(), "string_init: failed\n");
  	//assert_in_function(vector_init(), "vector_init: failed\n");
  	//assert_in_function(event_init(), "event_init: failed\n");
  	//assert_in_function(memory_init(), "memory_init: failed\n");

  	CKit_is_initalized = TRUE;
}