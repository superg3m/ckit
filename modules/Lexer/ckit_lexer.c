#include "./ckit_lexer.h"
#include "../../core/String/ckit_char.h"
#include "../../core/FileIO/ckit_file_system.h"
void ckit_lexer_load_file_data(CKIT_Lexer* lexer, char* file_path) {
	FileSystem fs = file_system_create(file_path);
	file_open(&fs);

	lexer->file_data = fs.data;
	lexer->file_size = fs.file_size;
	lexer->character_index = 0;
	file_close(&fs);
}

char ckit_lexer_consume_next_char(CKIT_Lexer* lexer) {
	char c = lexer->file_data[lexer->character_index++];
	ckit_assert(lexer->scratch_buffer_index > CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
	if (c != ' ') { // whitespace
		lexer->scratch_buffer[lexer->scratch_buffer_index++] = c;
	}
	return c;
}

char ckit_lexer_peek_next_char(CKIT_Lexer* lexer) {
	char c = lexer->file_data[lexer->character_index];
	return c;
}

CKIT_Tokens ckit_lexer_generate_token(CKIT_Lexer* lexer) {
	// take the scratch buffer and generate a token
	CKIT_Tokens ret = TOKEN_ILLEGAL;

	char* keywords[] = {
		"if",
		"while",
		"struct",
		"typedef",
		"return",
		"continue",
		"break"
	};

	char* directives[] = {
		"#if",
		"#ifndef",
		"#ifdef",
		"#include",
		"#define"
	};

	char* intrinsics[] = {
		"sizeof()",
		"typeof()",
	};

	char* primative_types[] = {
		"char"
		"int"
		"float"
		"double"
		"bool"

		"u8",
		"u16",
		"u32",
		"u64",

		"s8"
		"s16"
		"s32"
		"s64"

		"f32",
		"f64",
	};

	if (lexer->scratch_buffer[0] == '\"') {
		u32 count = 1;
		while (lexer->file_size < count++) {
			if (lexer->scratch_buffer[count++] == '\"') {
				ret = TOKEN_STRING_LITERAL;
				break;
			}
		}
	}

	for (int i = 0; i < ArrayCount(keywords); i++) {
		if (ckit_str_equal(keywords[i], lexer->scratch_buffer)) {
			ret = TOKEN_KEYWORD;
			break;
		}
	}

	for (int i = 0; i < ArrayCount(directives); i++) {
		if (ckit_str_equal(directives[i], lexer->scratch_buffer)) {
			ret = TOKEN_KEYWORD;
			break;
		}
	}

	for (int i = 0; i < ArrayCount(intrinsics); i++) {
		if (ckit_str_equal(intrinsics[i], lexer->scratch_buffer)) {
			ret = TOKEN_KEYWORD;
			break;
		}
	}

	for (int i = 0; i < ArrayCount(primative_types); i++) {
		if (ckit_str_equal(primative_types[i], lexer->scratch_buffer)) {
			ret = TOKEN_KEYWORD;
			break;
		}
	}

	ckit_str_clear(lexer->scratch_buffer);

	return ret;
}

CKIT_Tokens* ckit_lexer_consume_token_stream(CKIT_Lexer* lexer) {
	CKIT_Tokens* ret = lexer->token_stream;
	lexer->token_stream = NULLPTR;
	return ret;
}