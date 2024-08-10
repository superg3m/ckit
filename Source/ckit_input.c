#include "ckit_input.h"
#include "ckit_memory.h"

#include <stdarg.h>

Boolean previous_key_map[KEY_COUNT] = {0}; // previous frame
Boolean current_key_map[KEY_COUNT] = {0};  // current frame

Boolean input_get_key_down(CKIT_KeyCode key);
Boolean input_get_key_up(CKIT_KeyCode key);
Boolean input_get_key(CKIT_KeyCode key);

Boolean input_get_key_group_down(int number_of_keys, ...) {
	va_list variable_args;

	CKIT_KeyCode* key_group = ckit_alloc(sizeof(CKIT_KeyCode) * number_of_keys);

	va_start(variable_args, number_of_keys);
	for (int i = 0; i < number_of_keys; i++) {
		key_group[i] = va_arg(variable_args, CKIT_KeyCode);
	}
	va_end(variable_args);

	for (int i = 0; i < number_of_keys; i++) {
		// previous_key_map[key_group[i]] == FALSE;
	}
}

Boolean input_get_key_group_up(CKIT_KeyCode key, ...) {

}

Boolean input_get_key_group(CKIT_KeyCode key, ...) {

}
