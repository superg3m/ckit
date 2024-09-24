#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#define CKG_EXTERN
#include "../ckg/ckg.h"
//========================== Begin Types ==========================
typedef struct CKIT_Bitmap {
	u32 height;
	u32 width;
	u8 bytes_per_pixel;
	u8* memory;
} CKIT_Bitmap;
//=========================== End Types ===========================

#define MEMORY_TAG_CHARACTER_LIMIT 16

// Date: May 04, 2024
// TODO(Jovanni): This is gonna be a problem no doubt
#define PLATFORM_CHARACTER_LIMIT 200
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++