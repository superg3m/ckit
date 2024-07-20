#include "../../core/ckit_types.h"

typedef struct CKIT_Lexer {
	u32 line_number;
	u32 character_index;
	u8* file_data;
} CKIT_Lexer;

typedef enum CKIT_Tokens {
	// First 128 characters are covered

	// Identifers
	TOKEN_KEYWORD,
	TOKEN_IDENTIFIER,
	TOKEN_STRING_LITERAL,
	TOKEN_INTERGER_LITERAL,
	TOKEN_STRUCT_LITERAL,
	TOKEN_ARRAY_LITERAL,

	// Comparisons
	TOKEN_EQUALS_EQUALS,
	TOKEN_LESS_THAN_OR_EQUAL,
	TOKEN_GREATER_THAN_OR_EQUAL,
	TOKEN_NOT_EQUALS,

	// Bitwise
	TOKEN_BITWISE_AND,
	TOKEN_BITWISE_XOR,
	TOKEN_BITWISE_OR,
	TOKEN_BITWISE_LEFT_SHIFT,
	TOKEN_BITWISE_RIGHT_SHIFT,
} CKIT_Tokens;

void ckit_lexer_get_file_data(CKIT_Lexer* lexer, char* file_path);
char ckit_lexer_consume_next_char(CKIT_Lexer* lexer);
char ckit_lexer_peek_next_char(CKIT_Lexer* lexer);