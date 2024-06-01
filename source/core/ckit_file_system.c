#include "../../include/core/ckit_file_system.h"
#include "../../include/core/ckit_string.h"

u8* file_data(FILE* handle, size_t file_size) {
	u8* buffer = (u8*)memory_allocate(file_size, MEMORY_TAG_TEMPORARY);
	assert_in_function(fread(buffer, file_size, 1 , handle) != file_size, "Error reading file");
	rewind(handle);
	return buffer;
}

void file_open(FileSystem* file_system) {
	file_system->handle = fopen(file_system->file_name, "r");
	assert_in_function(file_system->handle != NULLPTR, "FILE IS NULL, CHECK INITIAL FILE NAME\n");
	fseek(file_system->handle, 0L, SEEK_END);
	file_system->file_size = ftell(file_system->handle);
	rewind(file_system->handle);

	file_system->data = file_data(file_system->handle, file_system->file_size);
}

String file_get_next_line(FileSystem* file_system) {
	String line = string_create("");
	char c;
	do {
		c = fgetc(file_system->handle);
		if (c != '\n' && c != EOF) {
		string_append_char(line, c);
		}
		if (c == EOF) {
		file_system->reachedEOF = TRUE;
		}
	} while (c != '\n' && c != EOF);
	return line;
}

char file_get_next_char(FileSystem* file_system) {
    return fgetc(file_system->handle);
}

void file_close(FileSystem* file_system) { 
    fclose(file_system->handle); 
}

FileSystem file_system_create(String file_name) {
	FileSystem file_system;
	file_system.handle = NULLPTR;
	file_system.reachedEOF = FALSE;
	file_system.file_name = file_name;
	file_system.data = NULLPTR;
	return file_system;
}
