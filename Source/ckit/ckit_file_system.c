#include "ckit_file_system.h"
#include "ckit_string.h"
#include "ckit_memory.h"
u8* file_data(FILE* handle, size_t file_size) {
	u8* buffer = (u8*)ckit_alloc_custom(file_size, TAG_CKIT_CORE_FILE_SYSTEM);
	ckit_assert_msg(fread(buffer, file_size, 1 , handle) != file_size, "Error reading file");
	rewind(handle);
	return buffer;
}

void file_open(FileSystem* file_system) {
	file_system->handle = fopen(file_system->file_name, "r");
	ckit_assert_msg(file_system->handle != NULLPTR, "FILE IS NULL, CHECK INITIAL FILE NAME\n");
	fseek(file_system->handle, 0L, SEEK_END);
	file_system->file_size = ftell(file_system->handle);
	rewind(file_system->handle);

	file_system->data = file_data(file_system->handle, file_system->file_size);
}

String file_get_next_line(FileSystem* file_system) {
	String line = ckit_str_create("");
	char c;
	do {
		c = fgetc(file_system->handle);
		if (c != '\n' && c != EOF) {
		ckit_str_append_char(line, c);
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
	ckit_free(file_system->data);
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
