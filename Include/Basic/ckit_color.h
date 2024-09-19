#pragma once

#include "../../Core/Basic/ckit_types.h"
#include "./ckit_math.h"
//========================== Begin Types ==========================
typedef CKIT_Vector4 CKIT_Color;

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
	u32 ckit_color_to_u32(CKIT_Color color);
	CKIT_Color ckit_color_from_u32(u32 color);
	CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color);
	CKIT_Color ckit_color_u32_blend_alpha(u32 front_color_u32, u32 back_color_u32);

	/**
	 * @brief value from 0.0 to 1.0
	 * 
	 * @param color 
	 * @param value 
	 * @return CKIT_Color 
	 */
	CKIT_Color ckit_color_multiply(CKIT_Color color, float value);
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
	const u32 alpha = ((color.a) << 24);
	const u32 red = ((color.r) << 16);
	const u32 green = ((color.g) << 8);
	const u32 blue = ((color.b) << 0);
					
	const u32 rgba = alpha|red|green|blue;

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

CKIT_Color ckit_color_multiply(CKIT_Color color, float value) {
	CKIT_Color ret = {0};
	ret.r = color.r * value;
	ret.g = color.g * value;
	ret.b = color.b * value;
	ret.a = color.a * value;

	return ret;
}

u32 ckit_color_u32_multiply(u32 color, float value) {
	CKIT_Color ret = {0};
	u8 b = ((color >> 0) & 0xFF)  * value;
	u8 g = ((color >> 8) & 0xFF)  * value;
	u8 r = ((color >> 16) & 0xFF) * value;
	u8 a = ((color >> 24) & 0xFF) * value;

	color = r|g|b|a;

	return color;
}

CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color) {
	CKIT_Color ret = {0};

	float normalized_back_alpha = (float)back_color.a / 255.0;

	ret.a = back_color.a;
	ret.r = (back_color.r * normalized_back_alpha) + (front_color.r * (1 - normalized_back_alpha));
	ret.g = (back_color.g * normalized_back_alpha) + (front_color.g * (1 - normalized_back_alpha));
	ret.b = (back_color.b * normalized_back_alpha) + (front_color.b * (1 - normalized_back_alpha));

	return ret;
}

CKIT_Color ckit_color_u32_alpha_blend(u32 front_color_u32, u32 back_color_u32) {
	CKIT_Color front_color = {0};
	front_color.a = (u8)((front_color_u32 >> 24) & 0xFF);
	front_color.r = (u8)((front_color_u32 >> 16) & 0xFF);
	front_color.g = (u8)((front_color_u32 >> 8) & 0xFF);
	front_color.b = (u8)((front_color_u32 >> 0) & 0xFF);

	CKIT_Color back_color = {0};
	back_color.a = (u8)((back_color_u32 >> 24) & 0xFF);
	back_color.r = (u8)((back_color_u32 >> 16) & 0xFF);
	back_color.g = (u8)((back_color_u32 >> 8) & 0xFF);
	back_color.b = (u8)((back_color_u32 >> 0) & 0xFF);
				
	return ckit_color_alpha_blend(front_color, back_color);
}