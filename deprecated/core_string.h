#pragma once
#include "core_types.h"

typedef char* String;

u32 c_string_length(const char* c_string);
/**
 * @brief string_create allocates memory
 * use string_destroy to free
 * 
 * @param cString 
 */
String string_create(const char* c_string);



String string_create(const char* c_string);
void string_free(String* string);
u64 string_length(String string);

Boolean string_compare(const char* s1, const char* s2);
Boolean string_compare(const String* s1, const char* s2);
Boolean string_compare(const String* s1, const String* s2);

void string_append(String* string, const char* source);
void string_append(String* string, const String source);
void string_insert(String* string, const u32 index);
void string_clear(String* string);

void string_copy(); // Careful about the header
void string_concat();

