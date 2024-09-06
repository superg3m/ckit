#include "../../ckit.h"

#if defined(PLATFORM_WINDOWS)
	void set_bitmap_gradient(CKIT_Window* window, u32 x_offset, u32 y_offset) {
		int stride = window->bitmap.width * window->bitmap.bytes_per_pixel;

		u8* row = window->bitmap.memory;    
		for(u32 y = 0; y < window->bitmap.height; y++)
		{
			u32* pixel = (u32*)row;
			for(u32 x = 0; x < window->bitmap.width; x++)
			{
				const u32 red = ((100 + x + x_offset) << 16);
				const u32 green = (10 << 8);
				const u32 blue = ((100 + y + y_offset) << 0);
				
				const u32 rgb = red|green|blue;

				*pixel++ = rgb;
			}
			row += stride;
		}
	}
#elif defined(PLATFORM_LINUX)
	void set_bitmap_gradient(CKIT_Window* window, u32 x_offset, u32 y_offset) {
		
	}
#endif

int main() {
	ckit_init();

	u32 width = 800;
	u32 height = 800;
	u32 width_padding = 18;
	u32 height_padding = 42;
	u32 width_with_padding = width - width_padding;
	u32 height_with_padding = height - height_padding;

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_SUCCESS("TESTING\n");

	ckit_window_bind_icon("../../../assets/c_original_logo_icon_146611.ico");
	CKIT_Window* window = ckit_window_create(width, height, "GameEngine");

	float x_pos = 40;
	float y_pos = 40;

	float x_velocity = 2.0;
	float y_velocity = 1.5;

	u32 player_width = 64;
	u32 player_height = 64;
	u32 half_player_width = player_width / 2;
	u32 half_player_height = player_height / 2;

	u32 center_width = 10;
	u32 center_height = 10;
	u32 half_center_width = center_width / 2;
	u32 half_center_height = center_height / 2;

	u32 border_size = 5; 
	u32 close_factor = 0;

	size_t file_size = 0;
	u8* ckit_sword_bmp_data = ckit_os_read_entire_file("../../../assets/Sword.bmp", &file_size);
	CKIT_Bitmap sword_bitmap = ckit_graphics_load_bmp(ckit_sword_bmp_data, file_size);
	
	int mouse_x = 0;
	int mouse_y = 0;
	while (!ckit_window_should_quit(window)) {
		// set_bitmap_gradient(window, x_offset, y_offset);

		{ // UPDATE
			s32 left_threshold   = 0 + close_factor;
			s32 right_threshold  = width_with_padding - close_factor;
			s32 top_threshold    =  0 + close_factor;
			s32 bottom_threshold = height_with_padding - close_factor;

			Boolean left_check   = x_pos < left_threshold - 1;
			Boolean right_check  = (x_pos + player_width) > right_threshold + 1;
			Boolean top_check    = (y_pos) < top_threshold - 1;
			Boolean bottom_check = (y_pos + player_height) > bottom_threshold + 1;


			if (left_check || right_check) {
				x_velocity *= -1.025;
				close_factor += 2;
				x_pos += x_velocity * 5;
			}

			if (bottom_check || top_check) {
				y_velocity *= -1.025;
				close_factor += 2;
				y_pos += y_velocity * 5;
			}

			x_pos += x_velocity;
			y_pos += y_velocity;

			ckit_window_get_mouse_position(window, &mouse_x, &mouse_y);
		}


		{ // RENDER
			ckit_window_clear_color(window, (CKIT_Color){55, 55, 55, 255});

			ckit_window_draw_bitmap(window, 10, 0, sword_bitmap);

			ckit_window_draw_circle(window, 10, 10, 25, TRUE, (CKIT_Color){0, 0, 255, 255});
			// ckit_window_draw_quad_custom(window, 10, 0, 50, 50, ((CKIT_Color){0, 255, 0, 105}));


			ckit_window_draw_quad_custom(window, 0 + close_factor, 0 + close_factor, border_size, height_with_padding - (close_factor * 2), CKIT_COLOR_GREEN); // left
			ckit_window_draw_quad_custom(window, 0 + close_factor, 0 + close_factor, width_with_padding - (close_factor * 2), border_size, CKIT_COLOR_PURPLE); // top
			ckit_window_draw_quad_custom(window, 0 + close_factor, height_with_padding - close_factor, width_with_padding - (close_factor * 2), border_size, CKIT_COLOR_RED); // bottom
			ckit_window_draw_quad_custom(window, width_with_padding - close_factor, 0 + close_factor, border_size, height_with_padding - (close_factor * 2), CKIT_COLOR_BLUE); // right

			float offset_to_center_x = ((float)x_pos + (half_player_width)) - half_center_width;
			float offset_to_center_y = ((float)y_pos + (half_player_height)) - half_center_height;

			ckit_window_draw_quad_custom(window, (s32)width / 4, (s32)height / 4, 400, 200, ((CKIT_Color){20, 20, 20, 150}));
			ckit_window_draw_quad_custom(window, (s32)x_pos, (s32)y_pos, player_width, player_height, ((CKIT_Color){0, 255, 0, 5}));

			//ckit_window_draw_quad_custom(window, (s32)mouse_x, (s32)mouse_y, player_width, player_height, ((CKIT_Color){0, 255, 0, 10}));

			ckit_window_draw_quad_custom(window, (s32)offset_to_center_x, (s32)offset_to_center_y, center_width, center_height, CKIT_COLOR_PURPLE);

			ckit_window_swap_buffers(window);
		}
	}

	ckit_window_free(window); // hmm how can I make this safer?
	ckit_free(ckit_sword_bmp_data);

	ckit_cleanup();
	return 0;
}