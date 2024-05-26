#include "../include/core_file_system.h"
#include "../include/core_string.h"

void file_open(FileSystem* fileFramework) {
	fileFramework->file = fopen(fileFramework->fileName, "r");
	assert_in_function(fileFramework->file != NULL, "FILE IS NULL, CHECK INITIAL FILE NAME\n");
}

String file_get_next_line(FileSystem* fileFramework) {
	String line = string_create("");
	char c;
	do {
		c = fgetc(fileFramework->file);
		if (c != '\n' && c != EOF) {
		string_append_char(line, c);
		}
		if (c == EOF) {
		fileFramework->reachedEOF = TRUE;
		}
	} while (c != '\n' && c != EOF);
	return line;
}

char file_get_next_char(FileSystem* fileFramework) {
    return fgetc(fileFramework->file);
}

void file_close(FileSystem* fileFramework) { 
    fclose(fileFramework->file); 
}

FileSystem file_system_create(const char* fileName) {
	FileSystem file_system;
	file_system.file = NULL;
	file_system.reachedEOF = FALSE;
	file_system.fileName = fileName;
	return file_system;
}
