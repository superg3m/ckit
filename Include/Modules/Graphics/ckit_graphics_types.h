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

	u32 ckit_color_to_u32(CKIT_Color color) {
		// Pre-multiply RGB components by the alpha value
		u32 red = (color.r * color.a) / 255;
		u32 green = (color.g * color.a) / 255;
		u32 blue = (color.b * color.a) / 255;

		u32 alpha = ((color.a) << 24);
		red = (red << 16);
		green = (green << 8);
		blue = (blue << 0);

		u32 rgba = alpha|red|green|blue;

		return rgba;
	}

	CKIT_Color ckit_color_from_u32(u32 color) {
		CKIT_Color ret = {0};

		ret.b = ((color >> 0) & 0xFF); 
		ret.g = ((color >> 8) & 0xFF); 
		ret.r = ((color >> 16) & 0xFF); 
		ret.a = ((color >> 24) & 0xFF); 
					
		return ret;
	}

	CKIT_Color ckit_color_blend_alpha(CKIT_Color color1, CKIT_Color color2) {
		CKIT_Color ret = {0};

		ret.a = color1.a + color2.a * (255 - color1.a) / 255;

		if (ret.a > 0) {
			ret.r = (color1.r * color1.a + color2.r * color2.a * (255 - color1.a) / 255) / ret.a * 255;
			ret.g = (color1.g * color1.a + color2.g * color2.a * (255 - color1.a) / 255) / ret.a * 255;
			ret.b = (color1.b * color1.a + color2.b * color2.a * (255 - color1.a) / 255) / ret.a * 255;
		}

		return ret;
	}

	CKIT_Color ckit_color_u32_blend_alpha(u32 color) {
		ckit_assert_msg(FALSE, "NOT IMPLEMENTED YET\n");

		CKIT_Color ret = {0};

		ret.b = ((color >> 0) & 0xFF); 
		ret.g = ((color >> 8) & 0xFF); 
		ret.r = ((color >> 16) & 0xFF); 
		ret.a = ((color >> 24) & 0xFF); 
					
		return ret;
	}
#endif // CKIT_IMPL