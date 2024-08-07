#pragma once

#include "ckit_types.h"
#define CKIT_LEXER_SCRATCH_BUFFER_CAPACITY 512

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

void ckit_lexer_load_file_data(CKIT_Lexer* lexer, char* file_path);
void ckit_lexer_load_string(CKIT_Lexer* lexer, char* string);
void ckit_lexer_print_token(CKIT_Token token);
CKIT_Token ckit_lexer_generate_next_token(CKIT_Lexer* lexer);


CKIT_Token* ckit_lexer_generate_token_stream(CKIT_Lexer* lexer);
CKIT_Token* ckit_lexer_consume_token_stream(CKIT_Lexer* lexer);
CKIT_Token* ckit_lexer_peek_token_stream(CKIT_Lexer* lexer);