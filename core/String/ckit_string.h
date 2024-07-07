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
  String ckit_str_create_custom(const char* c_str, u32 capacity);
  String MACRO_ckit_str_free(String str);
  u32 ckit_str_length(const String str);
  Boolean ckit_str_equal(const String str1, const String str2);

  String MACRO_ckit_str_append(String str, const char* source);
  String MACRO_ckit_str_append_char(String str, const char source);

  void ckit_str_insert(String str, const u32 index);
  void ckit_str_insert_char(String str, const u32 index);
  void ckit_str_clear(String str);

  void ckit_str_copy(String* str, const char* source); // Careful about the header
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_str_create(str) ckit_str_create_custom(str, 0)
#define ckit_str_free(str) str = MACRO_ckit_str_free(str);
#define ckit_str_append(str, source) str = MACRO_ckit_str_append(str, source);
#define ckit_str_append_char(str, source) str = MACRO_ckit_str_append_char(str, source);
//++++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++
