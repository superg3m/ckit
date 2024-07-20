#include "./ckit_lexer.h"
#include "../../core/String/ckit_char.h"
#include "../../core/FileIO/ckit_file_system.h"
#include "../../core/Memory/ckit_memory.h"
#include "../../core/Collection/Vector/ckit_vector.h"
#include "../../core/Collection/HashMap/ckit_hashmap.h"

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
CKIT_HashMap* token_map = NULLPTR; 

void ckit_token_map_init() {
	if (!token_map) {
		CKIT_Token tokens[] = {
			TOKEN_ASSIGNMENT_EQUAL,
			TOKEN_COMPARE_EQUALS,
			TOKEN_COMPARE_NOT_EQUALS,
			TOKEN_COMPARE_LESS_THAN,
			TOKEN_COMPARE_GREATER_THAN,
			TOKEN_COMPARE_LESS_THAN_OR_EQUAL,
			TOKEN_COMPARE_GREATER_THAN_OR_EQUAL,
			TOKEN_COMPARE_AND,
			TOKEN_COMPARE_OR,
			TOKEN_ASSIGNMENT_ADDITION,
			TOKEN_ASSIGNMENT_SUBTRACTION,
			TOKEN_ASSIGNMENT_AND,
			TOKEN_ASSIGNMENT_OR,
			TOKEN_ASSIGNMENT_XOR,
			TOKEN_ASSIGNMENT_LEFT_SHIFT,
			TOKEN_ASSIGNMENT_RIGHT_SHIFT,
			TOKEN_BITWISE_AND,
			TOKEN_BITWISE_XOR,
			TOKEN_BITWISE_OR,
			TOKEN_BITWISE_LEFT_SHIFT,
			TOKEN_BITWISE_RIGHT_SHIFT,
			TOKEN_SYNTAX_SEMICOLON,
			TOKEN_SYNTAX_COMMA,
			TOKEN_SYNTAX_LEFT_PAREN,
			TOKEN_SYNTAX_RIGHT_PAREN,
			TOKEN_SYNTAX_LEFT_BRACE,
			TOKEN_SYNTAX_RIGHT_BRACE,
			TOKEN_COMMENT_SINGLE_LINE,
			TOKEN_COMMENT_MULTI_LINE_START,
			TOKEN_COMMENT_MULTI_LINE_END,
			TOKEN_END_OF_FILE
		};

		char* token_strings[] = {
			"=", "==", "!=", "<", ">", "<=", ">=", 
			"&&", "||", 
			"+=", "-=", 
			"&=", "|=", "^=", "<<=", ">>=",
			"&", "|", "^", "<<", ">>",
			";", ",", "(", ")", "{", "}", "//", "/*", "*/", ""
		};

		token_map = ckit_hashmap_create(32, CKIT_Token);

		for (u32 i = 0; i < ArrayCount(tokens); i++) {
			ckit_hashmap_put(token_map, token_strings[i], &tokens[i], NULLPTR);
		}
	}
}

void ckit_lexer_load_file_data(CKIT_Lexer* lexer, char* file_path) {
	ckit_token_map_init();
	FileSystem fs = file_system_create(file_path);
	file_open(&fs);

	lexer->file_data = fs.data;
	lexer->file_size = fs.file_size;
	lexer->character_index = 0;
	lexer->scratch_buffer_index = 0;
	lexer->token_stream = NULLPTR;
	ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
	file_close(&fs);
}

void ckit_lexer_load_string(CKIT_Lexer* lexer, char* string) {
	ckit_token_map_init();
	lexer->file_data = string;
	lexer->file_size = ckit_cstr_length(string) + 1;
	lexer->character_index = 0;
	lexer->scratch_buffer_index = 0;
	lexer->token_stream = NULLPTR;
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
CKIT_Token ckit_lexer_classify_token(CKIT_Lexer* lexer) {
	// take the scratch buffer and generate a token
	CKIT_Token ret = TOKEN_ILLEGAL;
	if (ckit_hashmap_has(token_map, lexer->scratch_buffer)) {
		ret = *((CKIT_Token*)ckit_hashmap_get(token_map, lexer->scratch_buffer));
		ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
		lexer->scratch_buffer_index = 0;
		return ret;
	}

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
		u32 count = 0;
		Boolean is_float = FALSE;
		while (ckit_char_is_digit(lexer->scratch_buffer[count]) || lexer->scratch_buffer[count] == '.') {
			if (lexer->scratch_buffer[count++] == '.') {
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

CKIT_Token ckit_lexer_generate_next_token(CKIT_Lexer* lexer) {
	// populate the scratch buffer
	char c = '\0';
	while (TRUE) {
		// Date: July 20, 2024
		// TODO(Jovanni): Might not be a good idea tbh seperating the scratch buffer might not a be a good idea
		// This doesn't wrok at all because when you consume a token you will know exectly what token it should be so this needs a rewrite.
		c = ckit_lexer_consume_next_char(lexer);
		if (c == ' ' || c == '\n') {
			break;
		}
		lexer->scratch_buffer[lexer->scratch_buffer_index++] = c;
	}

	ckit_vector_push(lexer->token_stream, ckit_lexer_classify_token(lexer));

	return lexer->token_stream[ckit_vector_count(lexer->token_stream) - 1];
}

char* ckit_lexer_token_to_string(CKIT_Token token) {
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

CKIT_Token* ckit_lexer_generate_token_stream(CKIT_Lexer* lexer) {
	CKIT_Token* ret = lexer->token_stream;
	lexer->token_stream = NULLPTR;
	return ret;
}

CKIT_Token* ckit_lexer_consume_token_stream(CKIT_Lexer* lexer) {
	CKIT_Token* ret = lexer->token_stream;
	lexer->token_stream = NULLPTR;
	return ret;
}

CKIT_Token* ckit_lexer_peek_token_stream(CKIT_Lexer* lexer) {
	CKIT_Token* ret = lexer->token_stream;
	lexer->token_stream = NULLPTR;
	return ret;
}

