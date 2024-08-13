#pragma once

#include "ckit_types.h"
#define CKIT_LEXER_SCRATCH_BUFFER_CAPACITY 512
//========================== Begin Types ==========================
typedef enum CKIT_Token_Type {
	TOKEN_KEYWORD = 200,
	TOKEN_DIRECTIVE,
	TOKEN_IDENTIFIER,
	TOKEN_INTRINSIC,
	TOKEN_PRIMATIVE,

	// Literals (naming here is a bit inconsistent)
	TOKEN_STRING_LITERAL = 300,
	TOKEN_CHARACTER_LITERAL,
	TOKEN_INTEGER_LITERAL, // Allow 1_000_000
	TOKEN_FLOAT_LITERAL,
	TOKEN_STRUCT_LITERAL,
	TOKEN_ARRAY_LITERAL,

	// Syntax
	TOKEN_SYNTAX_SEMICOLON = 400,
	TOKEN_SYNTAX_COMMA,
	TOKEN_SYNTAX_DOUBLE_QUOTE,
	TOKEN_SYNTAX_SINGLE_QUOTE,
	TOKEN_SYNTAX_POINTER,
	TOKEN_SYNTAX_REFERENCE,
	TOKEN_SYNTAX_LEFT_PAREN,
	TOKEN_SYNTAX_RIGHT_PAREN,
	TOKEN_SYNTAX_LEFT_BRACE,
	TOKEN_SYNTAX_RIGHT_BRACE,

	// Comments
	TOKEN_COMMENT_SINGLE_LINE = 500,      // "//"
	TOKEN_COMMENT_MULTI_LINE_START,       // "/*"
	TOKEN_COMMENT_MULTI_LINE_END,         // "*/"

	// Comparisons
	TOKEN_COMPARE_EQUALS = 600,           // "=="
	TOKEN_COMPARE_NOT_EQUALS,             // "!="
	TOKEN_COMPARE_LESS_THAN,              // "<"
	TOKEN_COMPARE_GREATER_THAN,           // ">"
	TOKEN_COMPARE_LESS_THAN_OR_EQUAL,     // "<="
	TOKEN_COMPARE_GREATER_THAN_OR_EQUAL,  // ">="
	TOKEN_COMPARE_AND, 					  // "&&"
	TOKEN_COMPARE_OR,  					  // "||"

	// Assignments
	TOKEN_ASSIGNMENT_EQUAL = 700,  // "="
	TOKEN_ASSIGNMENT_INCREMENT,    // "++" (pre is before identifer, and most is after identifer)
	TOKEN_ASSIGNMENT_DECREMENT,    // "--" (pre is before identifer, and most is after identifer)
	TOKEN_ASSIGNMENT_ADDITION,     // "+="
	TOKEN_ASSIGNMENT_SUBTRACTION,  // "-="
	TOKEN_ASSIGNMENT_OR,           // "&="
	TOKEN_ASSIGNMENT_AND,          // "|="
	TOKEN_ASSIGNMENT_XOR,          // "^="
	TOKEN_ASSIGNMENT_LEFT_SHIFT,   // "<<=
	TOKEN_ASSIGNMENT_RIGHT_SHIFT,  // ">>=

	// Bitwise
	TOKEN_BITWISE_AND = 800,   // "&"
	TOKEN_BITWISE_XOR,         // "|"
	TOKEN_BITWISE_OR,          // "^"
	TOKEN_BITWISE_LEFT_SHIFT,  // "<<"
	TOKEN_BITWISE_RIGHT_SHIFT, // ">>"

	// MISC
	TOKEN_END_OF_FILE = 900,
	TOKEN_ILLEGAL,
} CKIT_Token_Type;

typedef struct CKIT_Token {
	CKIT_Token_Type type;
	size_t value_size_in_bytes;
	void* value;
} CKIT_Token;

typedef struct CKIT_Lexer {
	u32 line_number;
	u32 character_index;
	u32 file_size;
	u8* file_data;
	CKIT_Token token; // current token this is a good idea I promise
	CKIT_Token* token_stream; // vector
	u32 scratch_buffer_index;
	char scratch_buffer[CKIT_LEXER_SCRATCH_BUFFER_CAPACITY]; // used to hold the number of things resets when you generate a new token
	char c;
} CKIT_Lexer;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	void ckit_lexer_load_file_data(CKIT_Lexer* lexer, char* file_path);
	void ckit_lexer_load_string(CKIT_Lexer* lexer, char* string);
	void ckit_lexer_print_token(CKIT_Token token);
	CKIT_Token ckit_lexer_generate_next_token(CKIT_Lexer* lexer);


	CKIT_Token* ckit_lexer_generate_token_stream(CKIT_Lexer* lexer);
	CKIT_Token* ckit_lexer_consume_token_stream(CKIT_Lexer* lexer);
	CKIT_Token* ckit_lexer_peek_token_stream(CKIT_Lexer* lexer);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	#include "ckit_char.h"
	#include "ckit_file_system.h"
	#include "ckit_memory.h"
	#include "ckit_vector.h"
	#include "ckit_hashmap.h"
	#include "ckit_hashset.h"

	char* keywords[] = {
		"if",
		"else",
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

	CKIT_HashMap* syntax_token_type_map = NULLPTR;
	CKIT_HashSet* keyword_set = NULLPTR;
	CKIT_HashSet* primative_types_set = NULLPTR;

	void ckit_lexer_token_map_init() {
		if (!syntax_token_type_map) {
			CKIT_Token_Type syntax_token_types[] = {
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
				TOKEN_SYNTAX_POINTER,
				TOKEN_COMMENT_SINGLE_LINE,
				TOKEN_COMMENT_MULTI_LINE_START,
				TOKEN_COMMENT_MULTI_LINE_END,

				TOKEN_END_OF_FILE
			};

			char* syntax_token_type_strings[] = {
				"=", "==", "!=", "<", ">", "<=", ">=", 
				"&&", "||", 
				"+=", "-=", 
				"&=", "|=", "^=", "<<=", ">>=",
				"&", "|", "^", "<<", ">>",
				";", ",", "(", ")", "{", "}", "*", "//", "/*", "*/", ""
			};

			syntax_token_type_map = ckit_hashmap_create(32, CKIT_Token_Type, FALSE);

			for (u32 i = 0; i < ArrayCount(syntax_token_types); i++) {
				ckit_hashmap_put(syntax_token_type_map, syntax_token_type_strings[i], &syntax_token_types[i]);
			}
		}
	}

	void ckit_lexer_init_keyword_set() {
		keyword_set = ckit_hashset_create(32);

		for (u32 i = 0; i < ArrayCount(keywords); i++) {
			ckit_hashset_put(keyword_set, keywords[i]);
		}
	}

	void ckit_lexer_init_primative_types_set() {
		primative_types_set = ckit_hashset_create(32);

		for (u32 i = 0; i < ArrayCount(primative_types); i++) {
			ckit_hashset_put(primative_types_set, primative_types[i]);
		}
	}


	internal char ckit_lexer_peek_next_char(CKIT_Lexer* lexer) {
		char c = lexer->file_data[lexer->character_index];
		return c;
	}

	internal char ckit_lexer_consume_next_char(CKIT_Lexer* lexer) {
		ckit_assert(lexer->scratch_buffer_index < CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
		lexer->scratch_buffer[lexer->scratch_buffer_index++] = lexer->c;
		lexer->c = lexer->file_data[++lexer->character_index];
		if (lexer->c == '\n') {
			lexer->line_number++;
		}
		return lexer->c;
	}

	void ckit_lexer_load_file_data(CKIT_Lexer* lexer, char* file_path) {
		ckit_lexer_token_map_init();
		ckit_lexer_init_keyword_set();
		ckit_lexer_init_primative_types_set();

		FileSystem fs = file_system_create(file_path);
		file_open(&fs);
		lexer->file_data = fs.data;
		lexer->file_size = fs.file_size;
		lexer->character_index = 0;
		lexer->scratch_buffer_index = 0;
		lexer->token_stream = NULLPTR;
		lexer->c = ckit_lexer_peek_next_char(lexer);
		ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
		file_close(&fs);
	}

	void ckit_lexer_load_string(CKIT_Lexer* lexer, char* string) {
		ckit_lexer_token_map_init();
		ckit_lexer_init_keyword_set();
		ckit_lexer_init_primative_types_set();
		lexer->file_data = string;
		lexer->file_size = ckit_cstr_length(string) + 1;
		lexer->character_index = 0;
		lexer->scratch_buffer_index = 0;
		lexer->token_stream = NULLPTR;
		lexer->c = ckit_lexer_peek_next_char(lexer);
		ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
	}

	internal void ckit_lexer_skip_whitespace(CKIT_Lexer* lexer) { // actually bad behavior you are consuming the whitespace and putting it into the scratch buffer
		while (lexer->c == ' ' || lexer->c == '\t') {
			ckit_lexer_consume_next_char(lexer);
		}
	}

	// skip_white_space
	// check if literal
	// check if keyword
	// check if identifer
	// check if ...

	// int x = 4.9;

	internal CKIT_Token ckit_lexer_null_token() {
		CKIT_Token ret; 
		ret.value_size_in_bytes = 0;
		ret.value = NULLPTR;
		ret.type = TOKEN_ILLEGAL;
		return ret;
	}

	CKIT_Token MACRO_ckit_lexer_token_create(CKIT_Token_Type token_type, size_t value_size_in_bytes, void* value) {
		CKIT_Token ret = ckit_lexer_null_token(); 
		if (token_type == TOKEN_ILLEGAL) {
			ret.value_size_in_bytes = 0;
			ret.value = NULLPTR;
			ret.type = TOKEN_ILLEGAL;
			return ret;
		}



		ret.value_size_in_bytes = value_size_in_bytes;
		ret.value = ckit_alloc_custom(value_size_in_bytes, TAG_CKIT_MODULE_LEXER);
		ckit_memory_copy(value, ret.value, ret.value_size_in_bytes, ret.value_size_in_bytes);
		ret.type = token_type;

		return ret;
	}
	#define ckit_lexer_token_create(type, value) MACRO_ckit_lexer_token_create(type, sizeof(value), value)



	internal CKIT_Token ckit_lexer_maybe_consume_number_literal(CKIT_Lexer* lexer) {
		CKIT_Token_Type ret_type = TOKEN_ILLEGAL;

		Boolean is_float = FALSE;
		while (ckit_char_is_digit(lexer->c)) {
			ckit_lexer_skip_whitespace(lexer);
			ckit_lexer_consume_next_char(lexer);
			if (!is_float) {
				ret_type = TOKEN_INTEGER_LITERAL;
			}
			if (lexer->c == '.') { // float
				is_float = TRUE;
				ret_type = TOKEN_FLOAT_LITERAL;
				ckit_lexer_consume_next_char(lexer);
			}
		}

		CKIT_Token ret = ckit_lexer_null_token();

		if (ret_type == TOKEN_FLOAT_LITERAL) {
			double float_literal = atof(lexer->scratch_buffer);
			ret = ckit_lexer_token_create(ret_type, &float_literal); 
		} else if (ret_type == TOKEN_INTEGER_LITERAL) {
			int integer_literal = atoi(lexer->scratch_buffer);
			ret = ckit_lexer_token_create(ret_type, &integer_literal);
		}

		return ret;
	}

	internal void ckit_lexer_report_error(CKIT_Lexer* lexer, char* message) {

	}

	internal CKIT_Token ckit_lexer_maybe_consume_string_literal(CKIT_Lexer* lexer) {
		CKIT_Token_Type ret_type = TOKEN_ILLEGAL;

		Boolean first_character_processed = FALSE;
		if (lexer->c == '\"') {
			ckit_lexer_consume_next_char(lexer);
			while (lexer->c != '\"' && (lexer->file_size > lexer->character_index)) {
				ret_type = TOKEN_STRING_LITERAL;
				first_character_processed = TRUE;
				ckit_lexer_consume_next_char(lexer);
			}
			ckit_lexer_consume_next_char(lexer); // add the last '\"'
		}

		CKIT_Token ret = ckit_lexer_token_create(ret_type, lexer->scratch_buffer);

		return ret;
	}

	internal CKIT_Token ckit_lexer_maybe_consume_identifier(CKIT_Lexer* lexer) {
		CKIT_Token_Type ret_type = TOKEN_ILLEGAL;

		Boolean first_character_processed = FALSE;
		while (ckit_char_is_alpha(lexer->c) || (first_character_processed && ckit_char_is_alpha_numeric(lexer->c))) {
			first_character_processed = TRUE;
			ckit_lexer_consume_next_char(lexer);
			ret_type = TOKEN_IDENTIFIER;
		}

		if (ckit_hashset_has(keyword_set, lexer->scratch_buffer)) {
			ret_type = TOKEN_KEYWORD;
		} else if (ckit_hashset_has(primative_types_set, lexer->scratch_buffer)) {
			ret_type = TOKEN_PRIMATIVE;
		}

		CKIT_Token ret = ckit_lexer_token_create(ret_type, lexer->scratch_buffer);
		return ret;
	}

	internal CKIT_Token ckit_lexer_maybe_consume_syntax_token(CKIT_Lexer* lexer) {
		CKIT_Token_Type ret_type = TOKEN_ILLEGAL;

		if (lexer->c != '\0' && lexer->c != EOF) {
			ckit_lexer_consume_next_char(lexer); // should be pretty much guarenteed
		}

		switch (lexer->scratch_buffer[0]) {
			case '<': {
				if (ckit_lexer_peek_next_char(lexer) == '<') ckit_lexer_consume_next_char(lexer);
				if (ckit_lexer_peek_next_char(lexer) == '=') ckit_lexer_consume_next_char(lexer);
			} break;

			case '>': {
				if (ckit_lexer_peek_next_char(lexer) == '>') ckit_lexer_consume_next_char(lexer);
				if (ckit_lexer_peek_next_char(lexer) == '=') ckit_lexer_consume_next_char(lexer);
			} break;

			case '-': {
				if (ckit_lexer_peek_next_char(lexer) == '=') ckit_lexer_consume_next_char(lexer);
				if (ckit_lexer_peek_next_char(lexer) == '-') ckit_lexer_consume_next_char(lexer);
			} break;

			case '+': {
				if (ckit_lexer_peek_next_char(lexer) == '=') ckit_lexer_consume_next_char(lexer);
				if (ckit_lexer_peek_next_char(lexer) == '+') ckit_lexer_consume_next_char(lexer);
			} break;
			
			case '/': {
				if (ckit_lexer_peek_next_char(lexer) == '*') ckit_lexer_consume_next_char(lexer);
				if (ckit_lexer_peek_next_char(lexer) == '/') ckit_lexer_consume_next_char(lexer);
			} break;

			case '!':
			case '|':
			case '^':
			case '*':
			case '=':
			case '&': {
				if (ckit_lexer_peek_next_char(lexer) == '=') ckit_lexer_consume_next_char(lexer);
			} break;
		}

		if (ckit_hashmap_has(syntax_token_type_map, lexer->scratch_buffer)) {
			ret_type = *((CKIT_Token_Type*)ckit_hashmap_get(syntax_token_type_map, lexer->scratch_buffer));
		}

		CKIT_Token ret = ckit_lexer_token_create(ret_type, lexer->scratch_buffer);

		return ret;
	}

	internal char* ckit_lexer_token_to_string(CKIT_Token token) {
		switch (token.type) {
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

	internal char* ckit_lexer_token_value_to_string(CKIT_Token token) {
		char* big_buffer = ckit_alloc_custom(1024, TAG_CKIT_EXPECTED_USER_FREE);
		ckit_memory_zero(big_buffer, 1024);
		switch (token.type) {
			case TOKEN_KEYWORD: 
			case TOKEN_DIRECTIVE:
			case TOKEN_IDENTIFIER:
			case TOKEN_INTRINSIC:
			case TOKEN_PRIMATIVE:
			case TOKEN_STRING_LITERAL:
			case TOKEN_SYNTAX_SEMICOLON:
			case TOKEN_SYNTAX_COMMA:
			case TOKEN_SYNTAX_DOUBLE_QUOTE:
			case TOKEN_SYNTAX_SINGLE_QUOTE:
			case TOKEN_SYNTAX_POINTER:
			case TOKEN_SYNTAX_REFERENCE:
			case TOKEN_SYNTAX_LEFT_PAREN:
			case TOKEN_SYNTAX_RIGHT_PAREN:
			case TOKEN_SYNTAX_LEFT_BRACE:
			case TOKEN_SYNTAX_RIGHT_BRACE:
			case TOKEN_COMMENT_SINGLE_LINE:
			case TOKEN_COMMENT_MULTI_LINE_START:
			case TOKEN_COMMENT_MULTI_LINE_END:
			case TOKEN_COMPARE_EQUALS:
			case TOKEN_COMPARE_NOT_EQUALS:
			case TOKEN_COMPARE_LESS_THAN:
			case TOKEN_COMPARE_GREATER_THAN:
			case TOKEN_COMPARE_LESS_THAN_OR_EQUAL:
			case TOKEN_COMPARE_GREATER_THAN_OR_EQUAL:
			case TOKEN_COMPARE_AND:
			case TOKEN_COMPARE_OR:
			case TOKEN_ASSIGNMENT_EQUAL:
			case TOKEN_ASSIGNMENT_INCREMENT:
			case TOKEN_ASSIGNMENT_DECREMENT:
			case TOKEN_ASSIGNMENT_ADDITION:
			case TOKEN_ASSIGNMENT_SUBTRACTION:
			case TOKEN_ASSIGNMENT_OR:
			case TOKEN_ASSIGNMENT_AND:
			case TOKEN_ASSIGNMENT_XOR:
			case TOKEN_ASSIGNMENT_LEFT_SHIFT:
			case TOKEN_ASSIGNMENT_RIGHT_SHIFT:
			case TOKEN_BITWISE_AND:
			case TOKEN_BITWISE_XOR:
			case TOKEN_BITWISE_OR:
			case TOKEN_BITWISE_LEFT_SHIFT:
			case TOKEN_BITWISE_RIGHT_SHIFT:
			case TOKEN_ILLEGAL:
			case TOKEN_CHARACTER_LITERAL: sprintf(big_buffer, "%s", ((char*)token.value)); return big_buffer;

			case TOKEN_END_OF_FILE: sprintf(big_buffer, "%s", "EOF"); return big_buffer;

			case TOKEN_INTEGER_LITERAL: sprintf(big_buffer, "%d", *((int*)token.value)); return big_buffer;
			case TOKEN_FLOAT_LITERAL: sprintf(big_buffer, "%f", *((float*)token.value)); return big_buffer;
		}

		return big_buffer;
	}

	void ckit_lexer_print_token(CKIT_Token token) {
		char* value =  ckit_lexer_token_value_to_string(token);
		if (token.type == TOKEN_ILLEGAL) {
			LOG_ERROR("%s | %s\n", ckit_lexer_token_to_string(token), value);
		} else {
			LOG_SUCCESS("%s | %s\n", ckit_lexer_token_to_string(token), value);
		}
		
		ckit_free(value);
	}

	CKIT_Token ckit_lexer_generate_next_token(CKIT_Lexer* lexer) {
		ckit_lexer_skip_whitespace(lexer);

		ckit_memory_zero(lexer->scratch_buffer, CKIT_LEXER_SCRATCH_BUFFER_CAPACITY);
		lexer->scratch_buffer_index = 0;
		CKIT_Token ret = ckit_lexer_null_token();

		ret = ckit_lexer_maybe_consume_string_literal(lexer);
		if (ret.type != TOKEN_ILLEGAL) {
			return ret;
		}

		ret = ckit_lexer_maybe_consume_number_literal(lexer);
		if (ret.type != TOKEN_ILLEGAL) {
			return ret;
		}

		ret = ckit_lexer_maybe_consume_identifier(lexer); // primative, identifer, keyword
		if (ret.type != TOKEN_ILLEGAL) {
			return ret;
		}

		ret = ckit_lexer_maybe_consume_syntax_token(lexer);
		if (ret.type != TOKEN_ILLEGAL) {
			return ret;
		}

		// ret.value = ckit_alloc(2, MEMORY_TAG_TEMPORARY);
		// ckit_memory_copy(&lexer->c, ret.value, 2, 2);
		return ret;
	}

	CKIT_Token* ckit_lexer_generate_token_stream(CKIT_Lexer* lexer) {
		lexer->token_stream = NULLPTR;
		CKIT_Token token = ckit_lexer_null_token();
		while (token.type != TOKEN_END_OF_FILE) {
			token = ckit_lexer_generate_next_token(lexer);
			ckit_vector_push(lexer->token_stream, token);
		}

		return lexer->token_stream;
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

	// ckit_lexer_skip_whitespace(lexer);
	// ckit_lexer_maybe_consume_string_literal
	// ckit_lexer_maybe_consume_number_literal
	// ckit_lexer_maybe_consume_indentifer_literal
	// ckit_lexer_maybe_consume_keyword_literal
#endif // CKIT_IMPL