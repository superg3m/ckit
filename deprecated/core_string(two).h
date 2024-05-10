#pragma once
/*===========================================================
 * File: core_string.h
 * Date: April 30, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include "core_types.h"

typedef struct String {
	u32 length;
	u32 capacity;
	char* data;
} String;

String string_create(const char* c_string);
void string_free(String* string);

Boolean string_compare(const char* s1, const char* s2);
Boolean string_compare(const String* s1, const char* s2);
Boolean string_compare(const String* s1, const String* s2);

void string_append(String* string, char* source);
void string_append(String* string, const String source);
void string_insert(String* string, const u32 index);
void string_clear(String* string);