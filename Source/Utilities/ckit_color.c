#pragma once

#include "../../Include/Utilities/ckit_color.h"

u32 ckit_color_to_u32(CKIT_Color color) {
	const u32 alpha = ((int)(color.a) << 24);
	const u32 red = ((int)(color.r) << 16);
	const u32 green = ((int)(color.g) << 8);
	const u32 blue = ((int)(color.b) << 0);
					
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
	u8 b = (u8)(((color >> 0) & 0xFF)  * value);
	u8 g = (u8)(((color >> 8) & 0xFF)  * value);
	u8 r = (u8)(((color >> 16) & 0xFF) * value);
	u8 a = (u8)(((color >> 24) & 0xFF) * value);

	color = r|g|b|a;

	return color;
}

CKIT_Color ckit_color_alpha_blend(CKIT_Color front_color, CKIT_Color back_color) {
	CKIT_Color ret = {0};

	float normalized_back_alpha = (float)back_color.a / 255.0;

	ret.a = back_color.a;
	ret.r = (back_color.r * normalized_back_alpha) + ((u32)front_color.r * (1 - normalized_back_alpha));
	ret.g = (back_color.g * normalized_back_alpha) + ((u32)front_color.g * (1 - normalized_back_alpha));
	ret.b = (back_color.b * normalized_back_alpha) + ((u32)front_color.b * (1 - normalized_back_alpha));

	return ret;
}

CKIT_Color ckit_color_u32_alpha_blend(u32 front_color_u32, u32 back_color_u32) {
	CKIT_Color front_color = {0};
	front_color.a = (u8)(((u32)front_color_u32 >> 24) & 0xFF);
	front_color.r = (u8)(((u32)front_color_u32 >> 16) & 0xFF);
	front_color.g = (u8)(((u32)front_color_u32 >> 8) & 0xFF);
	front_color.b = (u8)(((u32)front_color_u32 >> 0) & 0xFF);

	CKIT_Color back_color = {0};
	back_color.a = (u8)(((u32)back_color_u32 >> 24) & 0xFF);
	back_color.r = (u8)(((u32)back_color_u32 >> 16) & 0xFF);
	back_color.g = (u8)(((u32)back_color_u32 >> 8) & 0xFF);
	back_color.b = (u8)(((u32)back_color_u32 >> 0) & 0xFF);
				
	return ckit_color_alpha_blend(front_color, back_color);
}