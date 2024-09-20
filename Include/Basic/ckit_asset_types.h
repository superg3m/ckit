#include "../ckit_types.h"

typedef struct CKIT_BitmapAsset {
	u32 height;
	u32 width;
	u8 bytes_per_pixel;
	u8* memory;
} CKIT_BitmapAsset;