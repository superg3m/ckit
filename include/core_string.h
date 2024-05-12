#pragma once
/*===========================================================
 * File: core_string.h
 * Date: May 10, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_types.h"
#include "core_arena.h"

typedef char* String;

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
  u32 c_string_length(const char* c_string);
  /**
   * @brief string_create allocates memory
   * use string_destroy to free
   * 
   * @param cString 
   */
  String string_create_custom(Arena* arena, const char* c_string, u32 capacity);
  String MACRO_string_free(String string);
  u32 string_length(String string);

  Boolean string_compare(const char* s1, const char* s2);

  String MACRO_string_append(String string, const char* source);
  String MACRO_string_append_char(String string, const char source);

  void string_insert(String string, const u32 index);
  void string_clear(String string);

  void string_copy(String* string, const char* source); // Careful about the header
  //void string_copy(String* string, const char* source); // Careful about the header
  //void string_copy(String* string, const char* source); // Careful about the header

  void string_concat();
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define string_create(arena, string) string_create_custom(arena, string, 0)
#define string_free(string) string = MACRO_string_free(string);
#define string_append(string, source) string = MACRO_string_append(string, source);
#define string_append_char(string, source) string = MACRO_string_append_char(string, source);
//++++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++
