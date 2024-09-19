#pragma once

#include "../ckit_graphics_types.h"
#include "../ckit_graphics_shapes.h"
//========================== Begin Types ==========================
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	void ckit_graphics_software_backend_draw_quad(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, CKIT_Rectangle2D quad, CKIT_Color color);
	void ckit_graphics_software_backend_draw_bitmap(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap);
	void ckit_graphics_software_backend_draw_circle(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color);
	void ckit_graphics_software_backend_clear_color(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, CKIT_Color color);
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++
#if defined(CKIT_IMPL)
	void ckit_graphics_software_backend_draw_quad(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, CKIT_Rectangle2D quad, CKIT_Color color) {
		const s32 VIEWPORT_WIDTH = framebuffer_width;
		const s32 VIEWPORT_HEIGHT = framebuffer_height;

		s32 true_x = quad.x - (quad.width / 2); 
		s32 true_y = quad.y - (quad.height / 2); 

		u32 left = (u32)CLAMP(true_x, 0, VIEWPORT_WIDTH);
		u32 right = (u32)CLAMP(true_x + (s32)quad.width, 0, VIEWPORT_WIDTH);
		u32 top = (u32)CLAMP(true_y, 0, VIEWPORT_HEIGHT);
		u32 bottom = (u32)CLAMP(true_y + (s32)quad.height, 0, VIEWPORT_HEIGHT);

		u32* dest = (u32*)framebuffer;

		for (u32 y = top; y < bottom; y++) {
			for (u32 x = left; x < right; x++) {
				size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);

				CKIT_Color new_back_buffer_color = ckit_color_u32_blend_alpha(dest[final_pixel_index], ckit_color_to_u32(color)); // alpha blending
				dest[final_pixel_index] = ckit_color_to_u32(new_back_buffer_color);
			}
		}
	}

	void ckit_graphics_software_backend_draw_bitmap(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, s32 start_x, s32 start_y, u32 scale_factor, CKIT_Bitmap bitmap) {
		const s32 VIEWPORT_WIDTH = framebuffer_width;
		const s32 VIEWPORT_HEIGHT = framebuffer_height;

		const s32 scaled_bmp_width = bitmap.width * scale_factor;
		const s32 scaled_bmp_height = bitmap.height * scale_factor;

		s32 true_x = start_x - (scaled_bmp_width / 2);
		s32 true_y = start_y - (scaled_bmp_height / 2);

		u32 left = (u32)CLAMP(true_x, 0, VIEWPORT_WIDTH);
		u32 right = (u32)CLAMP(true_x + scaled_bmp_width, 0, VIEWPORT_WIDTH);
		u32 top = (u32)CLAMP(true_y, 0, VIEWPORT_HEIGHT);
		u32 bottom = (u32)CLAMP(true_y + scaled_bmp_height, 0, VIEWPORT_HEIGHT);

		u32* dest = (u32*)framebuffer;
		u32* bmp_memory = (u32*)bitmap.memory + ((bitmap.height - 1) * bitmap.width);

		// Date: August 31, 2024
		// TODO(Jovanni): SIMD for optimizations
		for (u32 y = top; y < bottom; y++) { 
			for (u32 x = left; x < right; x++) {
				const s64 bmp_x = (x - true_x) / scale_factor;
				const s64 bmp_y = (y - true_y) / scale_factor;

				s64 color_index = bmp_x - (bmp_y * bitmap.width);
				u32 color = bmp_memory[color_index];
				u8 alpha = (color >> 24);
				if (alpha == 0) {
					continue;
				}

				size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);
				dest[final_pixel_index] = color;
			}
		}
	}

	internal Boolean is_pixel_inside_circle(s32 test_point_x, s32 test_point_y, s32 center_x, s32 center_y, u32 radius) {
		double dx = center_x - test_point_x;
		double dy = center_y - test_point_y;
		dx *= dx;
		dy *= dy;
		double distanceSquared = dx + dy;
		double radiusSquared = radius * radius;
		return distanceSquared < radiusSquared;
	}

	internal Boolean is_pixel_on_circle_line(s32 test_point_x, s32 test_point_y, s32 center_x, s32 center_y, u32 radius) {
		double dx = center_x - test_point_x;
		double dy = center_y - test_point_y;
		dx *= dx;
		dy *= dy;
		double distanceSquared = dx + dy;
		double radiusSquared = radius * radius;
		return distanceSquared == radiusSquared;
	}

	// Date: September 09, 2024
	// TODO(Jovanni): Switch to the midpoint circle algo because its just better
	// - https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	// - https://noobtomaster.com/computer-graphics/circle-drawing-algorithms-midpoint-algorithm/
	// - https://www.thecrazyprogrammer.com/2016/12/bresenhams-midpoint-circle-algorithm-c-c.html
	// - https://www.youtube.com/watch?v=hpiILbMkF9w
	void ckit_graphics_software_backend_draw_circle(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, s32 start_x, s32 start_y, s32 radius, Boolean is_filled, CKIT_Color color) {
		if (radius <= 0) {
			return;
		}

		const uint32_t VIEWPORT_WIDTH = framebuffer_width;
		const uint32_t VIEWPORT_HEIGHT = framebuffer_height;

		const u32 diameter = radius * 2;

		s32 true_x = start_x - (radius);
		s32 true_y = start_y - (radius);

		u32 left = CLAMP(true_x, 0, VIEWPORT_WIDTH);
		u32 right = CLAMP(true_x + (s32)diameter, 0, VIEWPORT_WIDTH); // add one so there is a real center point in the circle
		u32 top = CLAMP(true_y, 0, VIEWPORT_HEIGHT);
		u32 bottom = CLAMP(true_y + (s32)diameter, 0, VIEWPORT_HEIGHT); // kyle wuz here skool sux

		u32* dest = (u32*)framebuffer;

		if (is_filled) {
			for (s32 y = top; y < bottom; y++) {
				for (s32 x = left; x < right; x++) {
					size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);
					u32 center_x = true_x + radius;
					u32 center_y = true_y + radius;

					if (is_pixel_inside_circle(x, y, center_x, center_y, radius)) {
						CKIT_Color new_back_buffer_color = ckit_color_u32_blend_alpha(dest[final_pixel_index], ckit_color_to_u32(color)); // alpha blending
						dest[final_pixel_index] = ckit_color_to_u32(new_back_buffer_color);
					}
				}
			}
		} else {
			ckit_assert_msg(FALSE, "Non-filled circle is not implemented yet!");
			for (s32 y = top; y < bottom; y++) {
				for (s32 x = left; x < right; x++) {
					size_t final_pixel_index = x + (y * VIEWPORT_WIDTH);
					u32 center_x = start_x + radius;
					u32 center_y = start_y + radius;

					if (is_pixel_on_circle_line(x, y, center_x, center_y, radius)) {
						CKIT_Color new_back_buffer_color = ckit_color_u32_blend_alpha(dest[final_pixel_index], ckit_color_to_u32(color)); // alpha blending
						dest[final_pixel_index] = ckit_color_to_u32(new_back_buffer_color);
					}
				}
			}
		}
	}

	void ckit_graphics_software_backend_clear_color(u8* framebuffer, u32 framebuffer_width, u32 framebuffer_height, CKIT_Color color) {
		int stride = framebuffer_width * 4;
		u8* row = framebuffer;    
		for(u32 y = 0; y < framebuffer_height; y++) {
			u32* pixel = (u32*)row;
			for(u32 x = 0; x < framebuffer_width; x++)
			{
				*pixel++ = ckit_color_to_u32(color);
			}
			row += stride;
		}
	}
#endif // CKIT_IMPL
