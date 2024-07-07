#pragma once

#include "../ckit_types.h"
//========================== Begin Types ==========================
typedef char* String;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
  /**
   * @brief string_create allocates memory
   * use string_destroy to free
   * 
   * @param cString 
   */
  String ckit_str_create_custom(const char* c_string, u32 capacity);
  String MACRO_ckit_str_free(String string);
  u32 ckit_str_length(String string);

  String MACRO_ckit_str_append(String string, const char* source);
  String MACRO_ckit_str_append_char(String string, const char source);

  void ckit_str_insert(String string, const u32 index);
  void ckit_str_insert_char(String string, const u32 index);
  void ckit_str_clear(String string);

  void ckit_str_copy(String* string, const char* source); // Careful about the header
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_str_create(string) ckit_str_create_custom(string, 0)
#define ckit_str_free(string) string = MACRO_ckit_str_free(string);
#define ckit_str_append(string, source) string = MACRO_ckit_str_append(string, source);
#define ckit_str_append_char(string, source) string = MACRO_ckit_str_append_char(string, source);
//++++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++
