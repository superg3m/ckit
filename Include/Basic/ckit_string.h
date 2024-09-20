#pragma once

#include "../ckit_types.h"
//========================== Begin Types ==========================
typedef char* String;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	String ckit_str_create_custom(const char* c_str, size_t capacity);
	u32 ckit_cstr_length(const char* str);
	u32 ckit_str_length(const String str);
	Boolean ckit_str_equal(const char* str1, const char* str2);
	void ckit_str_copy(String str, const char* source);

	String MACRO_ckit_str_append(String str, const char* source);
	String MACRO_ckit_str_append_char(String str, const char source);

	String MACRO_ckit_str_insert(String str, const char* to_insert, const u32 index);
	String MACRO_ckit_str_insert_char(String str, const char to_insert, const u32 index);
	void ckit_str_clear(char* str1);

	// If you are copying data to the string and need to update the header state specifically for length
	void ckit_str_recanonicalize_header_length(String str);

	String ckit_substring(const char* string_buffer, u32 start_range, u32 end_range);

	// Little bit tricky. This method returns a vector of strings so 
	// ckit_vector_count(): to get the number of strings it returned
	//
	//  for (u32 i = 0; i < ckit_vector_count(string_vector); i++) {
	//     LOG_PRINT("%s\n", string_vector[i]);
	//  }
	//  ckit_vector_free(string_vector);
	String* ckit_str_split(const char* string_buffer, const char* delimitor);
	// Date: September 09, 2024
	// TODO(Jovanni): String* ckit_str_split_with_char(const char* string_buffer, const char delimitor);

	Boolean ckit_str_contains(const char* string_buffer, const char* contains);
	s32 ckit_str_index_of(const char* string_buffer, const char* sub_string);
	s32 ckit_str_last_index_of(const char* string_buffer, const char* sub_string);
	Boolean ckit_str_starts_with(const char* string_buffer, const char* starts_with);
	Boolean ckit_str_ends_with(const char* string_buffer, const char* ends_with);
	String ckit_str_reverse(const char* string_buffer);
	String ckit_str_int_to_str(int number);
	int ckit_str_to_int(const char* ascii_number);
	String ckit_str_between_delimiters(const char* str, const char* start_delimitor, const char* end_delimitor);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define ckit_str_create(str) ckit_str_create_custom(str, 0)

#define ckit_str_insert(str, source, index) str = MACRO_ckit_str_insert(str, source, index);
#define ckit_str_insert_char(str, source, index) str = MACRO_ckit_str_insert_char(str, source, index);

#define ckit_str_append(str, source) str = MACRO_ckit_str_append(str, source);
#define ckit_str_append_char(str, source) str = MACRO_ckit_str_append_char(str, source);
//++++++++++++++++++++++++++++ End Macros ++++++++++++++++++++++++++