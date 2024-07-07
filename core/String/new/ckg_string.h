#pragma once

#include "../ckit_types.h"
#include "../../ckg/core/String/ckg_cstring.h"
//========================== Begin Types ==========================
typedef char* CKG_String;
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
  CKG_String ckg_str_create_custom(const char* c_string, size_t capacity);
  CKG_String MACRO_ckg_str_free(CKG_String string);

  CKG_String MACRO_ckg_str_insert(CKG_String string, const u32 index, const char* to_insert);
  CKG_String MACRO_ckg_str_insert_char(CKG_String string, const u32 index, const char to_insert);

  CKG_String MACRO_ckg_str_append(CKG_String string, const char* to_append);
  CKG_String MACRO_ckg_str_append_char(CKG_String string, const char to_append);

  void ckg_str_clear(CKG_String string);
  void ckg_str_copy(CKG_String string, const char* to_copy); // Careful about the header
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define string_create(string) ckg_str_create_custom_custom(string, 0)
#define ckg_str_free(string) string = MACRO_ckg_str_free(string);
#define ckg_str_insert(string, index, source) string = MACRO_ckg_str_insert(string, index, source);
#define ckg_str_insert_char(string, source) string = MACRO_ckg_str_insert_char(string, index, source);
#define ckg_str_append(string, source) string = MACRO_ckg_str_append(string, source);
#define ckg_str_append_char(string, source) string = MACRO_ckg_str_append_char(string, source);
//++++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++
