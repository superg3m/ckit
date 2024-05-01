#include "../include/core_assert.h"
#include "../include/core_types.h"

void _assert_in_function(char* message, char* file, int line, char* function) {
	assert_info(0, message, file, line, function);
}