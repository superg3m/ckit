#pragma once
/*===========================================================
 * File: ckg_file_system.h
 * Date: May 14, 2024
 * Creator: Jovanni Djonaj
===========================================================*/
#include <stdio.h>
#include "ckg_assert.h"

typedef struct CKG_FileSystem {
	char* file_name;
	FILE* handle;
	u8* data;
	size_t file_size;
	Boolean reachedEOF;
} CKG_FileSystem;

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKG_FileSystem ckg_file_system_create(char* file_name);
	void ckg_file_open(CKG_FileSystem* file_system);
	void ckg_file_close(CKG_FileSystem* file_system);
	size_t ckg_file_size(CKG_FileSystem* file_system);
	char* ckg_file_get_next_line(CKG_FileSystem* file_system);
	char ckg_file_get_next_char(CKG_FileSystem* file_system);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

