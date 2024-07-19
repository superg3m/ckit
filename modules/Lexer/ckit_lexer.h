#include "../../core/ckit_types.h"

typedef struct CKIT_Lexer {
	u32 line_number;
	u32 character_index;
	u8* file_data;
} CKIT_Lexer;