 #include "./core_types.h"

struct String {
	u32 length;
	u32 capacity;
	char* data;
};

String string_create(const char* c_string);
void string_free(String* string);

void string_append(String* string, char* source);
void string_append(String* string, const String source);
void string_insert(String* string, const u32 index);
void string_clear(String* string);