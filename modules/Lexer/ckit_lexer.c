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

void ckit_lexer_load_string(CKIT_Lexer* lexer, char* string) {
	lexer->file_data = string;
	lexer->file_size = ckit_cstr_length(string) + 1;
	lexer->character_index = 0;
}

char ckit_lexer_consume_next_char(CKIT_Lexer* lexer) {
	ckit_assert(lexer->scratch_buffer_index > CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);

	char c = lexer->file_data[lexer->character_index++];
	if (c == '\n') {
		lexer->line_number++;
	}

	return c;
}

char ckit_lexer_peek_next_char(CKIT_Lexer* lexer) {
	char c = lexer->file_data[lexer->character_index];
	return c;
}

CKIT_Tokens ckit_lexer_classify_token(CKIT_Lexer* lexer) {
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

	if (ckit_char_is_alpha(lexer->scratch_buffer[0])) {
		u32 count = 1;
		Boolean is_float = FALSE;
		while (!ckit_char_is_digit(lexer->scratch_buffer[count] || lexer->scratch_buffer[count] == '.')) {
			if (lexer->scratch_buffer[count] == '.') {
				if (is_float) {
					// bad token 1..10512 (might be interesting for: for loops like range)
				}
				is_float = TRUE;
				ret = TOKEN_FLOAT_LITERAL;
				break;
			}
		}

		if (!is_float) {
			ret = TOKEN_INTERGER_LITERAL;
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
			ret = TOKEN_DIRECTIVE;
			break;
		}
	}

	for (int i = 0; i < ArrayCount(intrinsics); i++) {
		if (ckit_str_equal(intrinsics[i], lexer->scratch_buffer)) {
			ret = TOKEN_INTRINSIC;
			break;
		}
	}

	for (int i = 0; i < ArrayCount(primative_types); i++) {
		if (ckit_str_equal(primative_types[i], lexer->scratch_buffer)) {
			ret = TOKEN_PRIMATIVE;
			break;
		}
	}

	ckit_str_clear(lexer->scratch_buffer);

	return ret;
}

CKIT_Tokens ckit_lexer_generate_token(CKIT_Lexer* lexer) {
	// populate the scratch buffer
	char c = '\0';
	while (TRUE) {
		c = ckit_lexer_consume_next_char(lexer);
		if (c == ' ' || c == '\n') {
			break;
		}
		lexer->scratch_buffer[lexer->scratch_buffer_index++] = c;
	}

	ckit_lexer_classify_token(lexer);
}

CKIT_Tokens* ckit_lexer_generate_token_stream(CKIT_Lexer* lexer) {
	CKIT_Tokens* ret = lexer->token_stream;
	lexer->token_stream = NULLPTR;
	return ret;
}

CKIT_Tokens* ckit_lexer_consume_token_stream(CKIT_Lexer* lexer) {
	CKIT_Tokens* ret = lexer->token_stream;
	lexer->token_stream = NULLPTR;
	return ret;
}

CKIT_Tokens* ckit_lexer_peek_token_stream(CKIT_Lexer* lexer) {
	CKIT_Tokens* ret = lexer->token_stream;
	lexer->token_stream = NULLPTR;
	return ret;
}

