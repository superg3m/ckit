#include "../../ckit.h"

void set_bitmap_gradient(CKIT_Window* window, u32 x_offset, u32 y_offset) {
	int stride = window->bitmap->width * window->bitmap->bytes_per_pixel;

	u8* row = window->bitmap->memory;    
	for(u32 y = 0; y < window->bitmap->height; y++)
	{
		u32* pixel = (u32*)row;
		for(u32 x = 0; x < window->bitmap->width; x++)
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

int main() {
	ckit_init();

	u32 width = 800;
	u32 height = 600;

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_SUCCESS("TESTING\n");

	ckit_window_bind_icon("../../../assets/c_original_logo_icon_146611.ico");
	CKIT_Window* window = ckit_window_create(width, height, "GameEngine");

	float x_pos = 40;
	float y_pos = 40;

	float x_velocity = 1.0;
	float y_velocity = 1.0;

	u32 player_width = 40;
	u32 player_height = 40;
	u32 half_player_width = player_width / 2;
	u32 half_player_height = player_height / 2;

	u32 center_width = 10;
	u32 center_height = 10;
	u32 half_center_width = center_width / 2;
	u32 half_center_height = center_height / 2;


	while (!ckit_window_should_quit(window)) {
		// set_bitmap_gradient(window, x_offset, y_offset);
		ckit_window_clear_color(window, (CKIT_Color){0, 0, 0, 255});
		
		ckit_window_draw_quad(window, 0, height, width, 1, (CKIT_Color){255, 0, 0, 255}); // x axis
		ckit_window_draw_quad(window, width, 0, 1, height, (CKIT_Color){0, 0, 255, 255}); // y axis

		float offset_to_center_x = ((float)x_pos + (half_player_width)) - half_center_width;
		float offset_to_center_y = ((float)y_pos + (half_player_height)) - half_center_height;

		ckit_window_draw_quad(window, x_pos, y_pos, player_width, player_height, (CKIT_Color){0, 255, 0, 255});
		ckit_window_draw_quad(window, offset_to_center_x, offset_to_center_y, center_width, center_height, (CKIT_Color){255, 0, 255, 255});

		ckit_window_draw_bitmap(window);

		Boolean left_check   = x_pos <= 0;
		Boolean right_check  = (x_pos + player_width) >= width;

		Boolean bottom_check = y_pos <= 0;
		Boolean top_check    = (y_pos + player_height) >= height;

		if (left_check || right_check) {
			x_velocity *= -1;
		}

		if (bottom_check || top_check) {
			y_velocity *= -1;
		}
		
		x_pos += x_velocity;
		y_pos += y_velocity;
	}

	ckit_window_free(window); // hmm how can I make this safer?

	ckit_cleanup();
	return 0;
}