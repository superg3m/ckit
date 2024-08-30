#pragma once

#include "../../Core/Basic/ckit_types.h"
//========================== Begin Types ==========================
typedef struct CKIT_Color {
	u8 r;
	u8 g;
	u8 b;
	u8 a;
} CKIT_Color;

typedef struct CKIT_Bitmap {
	u32 height;
	u32 width;
	u8 bytes_per_pixel;
	u8* memory;
} CKIT_Bitmap;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	CKIT_Bitmap ckit_graphics_load_bmp(u8* bmp_file_data, size_t file_size);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
#define CKIT_COLOR_BLACK (CKIT_Color){0, 0, 0, 255}
#define CKIT_COLOR_RED (CKIT_Color){255, 0, 0, 255}
#define CKIT_COLOR_BLUE (CKIT_Color){0, 0, 255, 255}
#define CKIT_COLOR_GREEN (CKIT_Color){0, 255, 0, 255}
#define CKIT_COLOR_WHITE (CKIT_Color){255, 255, 255, 255}
#define CKIT_COLOR_PINK (CKIT_Color){255, 105, 180, 255}
#define CKIT_COLOR_LIME (CKIT_Color){0, 255, 128, 255}
#define CKIT_COLOR_CYAN (CKIT_Color){0, 255, 255, 255}
#define CKIT_COLOR_PURPLE (CKIT_Color){128, 0, 128, 255}
#define CKIT_COLOR_YELLOW (CKIT_Color){255, 255, 0, 255}
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	CKIT_Bitmap ckit_graphics_load_bmp(u8* bmp_file_data, size_t file_size) {
		CKIT_Bitmap ret = {0};

		BmpHeader bmp_header;
		ckit_memory_copy(bmp_file_data, &bmp_header, sizeof(bmp_header), file_size);
		ret.bytes_per_pixel = (bmp_header.bits_per_pixel / 8);
		size_t bitmap_size = (bmp_header.width * bmp_header.height) * ret.bytes_per_pixel;
		u8* bmp_data = bmp_file_data + bmp_header.data_offset;

		size_t bmp_size = file_size - sizeof(BmpHeader);
		ret.width = bmp_header.width;
		ret.height = bmp_header.height;
		ret.memory = bmp_data;
		
		return ret;
	}
#endif // CKIT_IMPL