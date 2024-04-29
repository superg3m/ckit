#pragma once
#include "../core_types.h"

typedef char* String;

u32 c_string_length(const char* c_string);
/**
 * @brief string_create allocates memory
 * use string_destroy to free
 * 
 * @param cString 
 */
String string_create(const char* c_string);

void string_copy(); // Careful about the header
void string_concat();
u64 string_length(String string);
Boolean string_compare();
// void string_append(String &string, );
void string_destroy(String* string);

