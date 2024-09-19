#pragma once

#include "../ckit_types.h"
//========================== Begin Types ==========================
typedef enum CKIT_KeyCode {
    // Letters
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    // Numbers
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    // Function keys
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    // Arrow keys
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,

    // Modifier keys
    KEY_SHIFT,
    KEY_CONTROL,
    KEY_ALT,
    KEY_CAPS_LOCK,

    // Other common keys
    KEY_TAB,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_SPACE,
    KEY_ESCAPE,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_INSERT,
    KEY_DELETE,
    KEY_PRINT_SCREEN,
    KEY_SCROLL_LOCK,
    KEY_PAUSE,
    KEY_NUM_LOCK,

    // Special characters
    KEY_MINUS,
    KEY_EQUALS,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_BACKSLASH,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_GRAVE,

    // Windows/Command keys
    KEY_LEFT_WINDOWS,
    KEY_RIGHT_WINDOWS,
    KEY_MENU,

    // Total key count
    KEY_COUNT
} CKIT_KeyCode;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
    Boolean input_get_key_down(CKIT_KeyCode key); // The frame it is down
    Boolean input_get_key_up(CKIT_KeyCode key);   // The frame it is up
    Boolean input_get_key(CKIT_KeyCode key);	  // The state of the key down or up

    Boolean input_get_key_group_down(CKIT_KeyCode key, ...); // The frame it is down
    Boolean input_get_key_group_up(CKIT_KeyCode key, ...);   // The frame it is up
    Boolean input_get_key_group(CKIT_KeyCode key, ...);	  // The state of the key down or up
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************