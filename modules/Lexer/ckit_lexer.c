#include "./ckit_lexer.h"
#include "../../core/String/ckit_char.h"
#include "../../core/FileIO/ckit_file_system.h"
#include "../../core/Memory/ckit_memory.h"

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
	"char",
	"int",
	"float",
	"double",
	"bool",

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

void ckit_lexer_load_file_data(CKIT_Lexer* lexer, char* file_path) {
	FileSystem fs = file_system_create(file_path);
	file_open(&fs);

	lexer->file_data = fs.data;
	lexer->file_size = fs.file_size;
	lexer->character_index = 0;
	lexer->scratch_buffer_index = 0;
	ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
	file_close(&fs);
}

void ckit_lexer_load_string(CKIT_Lexer* lexer, char* string) {
	lexer->file_data = string;
	lexer->file_size = ckit_cstr_length(string) + 1;
	lexer->character_index = 0;
	lexer->scratch_buffer_index = 0;
	ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
}

internal char ckit_lexer_consume_next_char(CKIT_Lexer* lexer) {
	ckit_assert(lexer->scratch_buffer_index < CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);

	char c = lexer->file_data[lexer->character_index++];
	if (c == '\n') {
		lexer->line_number++;
	}

	return c;
}

internal char ckit_lexer_peek_next_char(CKIT_Lexer* lexer) {
	char c = lexer->file_data[lexer->character_index];
	return c;
}

// Date: July 20, 2024
// TODO(Jovanni): Fix this because its really really messy
CKIT_Tokens ckit_lexer_classify_token(CKIT_Lexer* lexer) {
	// take the scratch buffer and generate a token
	CKIT_Tokens ret = TOKEN_ILLEGAL;

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
			ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
			lexer->scratch_buffer_index = 0;
			return ret;
		}
	}

	for (int i = 0; i < ArrayCount(directives); i++) {
		if (ckit_str_equal(directives[i], lexer->scratch_buffer)) {
			ret = TOKEN_DIRECTIVE;
			ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
			lexer->scratch_buffer_index = 0;
			return ret;
		}
	}

	for (int i = 0; i < ArrayCount(intrinsics); i++) {
		if (ckit_str_equal(intrinsics[i], lexer->scratch_buffer)) {
			ret = TOKEN_INTRINSIC;
			ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
			lexer->scratch_buffer_index = 0;
			return ret;
		}
	}

	for (int i = 0; i < ArrayCount(primative_types); i++) {
		if (ckit_str_equal(primative_types[i], lexer->scratch_buffer)) {
			ret = TOKEN_PRIMATIVE;
			ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
			lexer->scratch_buffer_index = 0;
			return ret;
		}
	}

	if (ckit_char_is_digit(lexer->scratch_buffer[0])) {
		u32 count = 1;
		Boolean is_float = FALSE;
		while (ckit_char_is_digit(lexer->scratch_buffer[count] || lexer->scratch_buffer[count++] == '.')) {
			if (lexer->scratch_buffer[count] == '.') {
				if (is_float) {
					// bad token 1..10512 (might be interesting for: for loops like range)
				}
				is_float = TRUE;
				ret = TOKEN_FLOAT_LITERAL;
			}

			if (!is_float) {
				ret = TOKEN_INTEGER_LITERAL;
			}

			if (lexer->scratch_buffer[count] == '\0') {
				ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
				lexer->scratch_buffer_index = 0;
				return ret;
			}
		}
	}

	
	if (ckit_char_is_alpha(lexer->scratch_buffer[0])) {
		u32 count = 0;
		while (ckit_char_is_alpha_numeric(lexer->scratch_buffer[count++])) {
			if (lexer->scratch_buffer[count] == '\0') {
				ret = TOKEN_IDENTIFIER;
				break;
			}
		}
	}

	ckit_str_clear(lexer->scratch_buffer);
	lexer->scratch_buffer_index = 0;

	return ret;
}

CKIT_Tokens ckit_lexer_generate_next_token(CKIT_Lexer* lexer) {
	// populate the scratch buffer
	char c = '\0';
	while (TRUE) {
		c = ckit_lexer_consume_next_char(lexer);
		if (c == ' ' || c == '\n') {
			break;
		}
		lexer->scratch_buffer[lexer->scratch_buffer_index++] = c;
	}

	return ckit_lexer_classify_token(lexer);
}

char* ckit_lexer_token_to_string(CKIT_Tokens token) {
    switch (token) {
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_DIRECTIVE: return "TOKEN_DIRECTIVE";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_INTRINSIC: return "TOKEN_INTRINSIC";
        case TOKEN_PRIMATIVE: return "TOKEN_PRIMATIVE";
        case TOKEN_STRING_LITERAL: return "TOKEN_STRING_LITERAL";
        case TOKEN_CHARACTER_LITERAL: return "TOKEN_CHARACTER_LITERAL";
        case TOKEN_INTEGER_LITERAL: return "TOKEN_INTEGER_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "TOKEN_FLOAT_LITERAL";
        case TOKEN_STRUCT_LITERAL: return "TOKEN_STRUCT_LITERAL";
        case TOKEN_ARRAY_LITERAL: return "TOKEN_ARRAY_LITERAL";
        case TOKEN_SYNTAX_SEMICOLON: return "TOKEN_SYNTAX_SEMICOLON";
        case TOKEN_SYNTAX_COMMA: return "TOKEN_SYNTAX_COMMA";
        case TOKEN_SYNTAX_DOUBLE_QUOTE: return "TOKEN_SYNTAX_DOUBLE_QUOTE";
        case TOKEN_SYNTAX_SINGLE_QUOTE: return "TOKEN_SYNTAX_SINGLE_QUOTE";
        case TOKEN_SYNTAX_POINTER: return "TOKEN_SYNTAX_POINTER";
        case TOKEN_SYNTAX_REFERENCE: return "TOKEN_SYNTAX_REFERENCE";
        case TOKEN_SYNTAX_LEFT_PAREN: return "TOKEN_SYNTAX_LEFT_PAREN";
        case TOKEN_SYNTAX_RIGHT_PAREN: return "TOKEN_SYNTAX_RIGHT_PAREN";
        case TOKEN_SYNTAX_LEFT_BRACE: return "TOKEN_SYNTAX_LEFT_BRACE";
        case TOKEN_SYNTAX_RIGHT_BRACE: return "TOKEN_SYNTAX_RIGHT_BRACE";
        case TOKEN_COMMENT_SINGLE_LINE: return "TOKEN_COMMENT_SINGLE_LINE";
        case TOKEN_COMMENT_MULTI_LINE_START: return "TOKEN_COMMENT_MULTI_LINE_START";
        case TOKEN_COMMENT_MULTI_LINE_END: return "TOKEN_COMMENT_MULTI_LINE_END";
        case TOKEN_COMPARE_EQUALS: return "TOKEN_COMPARE_EQUALS";
        case TOKEN_COMPARE_NOT_EQUALS: return "TOKEN_COMPARE_NOT_EQUALS";
        case TOKEN_COMPARE_LESS_THAN: return "TOKEN_COMPARE_LESS_THAN";
        case TOKEN_COMPARE_GREATER_THAN: return "TOKEN_COMPARE_GREATER_THAN";
        case TOKEN_COMPARE_LESS_THAN_OR_EQUAL: return "TOKEN_COMPARE_LESS_THAN_OR_EQUAL";
        case TOKEN_COMPARE_GREATER_THAN_OR_EQUAL: return "TOKEN_COMPARE_GREATER_THAN_OR_EQUAL";
        case TOKEN_COMPARE_AND: return "TOKEN_COMPARE_AND";
        case TOKEN_COMPARE_OR: return "TOKEN_COMPARE_OR";
        case TOKEN_ASSIGNMENT_EQUAL: return "TOKEN_ASSIGNMENT_EQUAL";
        case TOKEN_ASSIGNMENT_INCREMENT: return "TOKEN_ASSIGNMENT_INCREMENT";
        case TOKEN_ASSIGNMENT_DECREMENT: return "TOKEN_ASSIGNMENT_DECREMENT";
        case TOKEN_ASSIGNMENT_ADDITION: return "TOKEN_ASSIGNMENT_ADDITION";
        case TOKEN_ASSIGNMENT_SUBTRACTION: return "TOKEN_ASSIGNMENT_SUBTRACTION";
        case TOKEN_ASSIGNMENT_OR: return "TOKEN_ASSIGNMENT_OR";
        case TOKEN_ASSIGNMENT_AND: return "TOKEN_ASSIGNMENT_AND";
        case TOKEN_ASSIGNMENT_XOR: return "TOKEN_ASSIGNMENT_XOR";
        case TOKEN_ASSIGNMENT_LEFT_SHIFT: return "TOKEN_ASSIGNMENT_LEFT_SHIFT";
        case TOKEN_ASSIGNMENT_RIGHT_SHIFT: return "TOKEN_ASSIGNMENT_RIGHT_SHIFT";
        case TOKEN_BITWISE_AND: return "TOKEN_BITWISE_AND";
        case TOKEN_BITWISE_XOR: return "TOKEN_BITWISE_XOR";
        case TOKEN_BITWISE_OR: return "TOKEN_BITWISE_OR";
        case TOKEN_BITWISE_LEFT_SHIFT: return "TOKEN_BITWISE_LEFT_SHIFT";
        case TOKEN_BITWISE_RIGHT_SHIFT: return "TOKEN_BITWISE_RIGHT_SHIFT";
        case TOKEN_END_OF_FILE: return "TOKEN_END_OF_FILE";
        case TOKEN_ILLEGAL: return "TOKEN_ILLEGAL";
        default: return "UNKNOWN_TOKEN";
    }
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

