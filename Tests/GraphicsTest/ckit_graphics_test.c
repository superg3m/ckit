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

	u32 x_offset = 0;
	u32 y_offset = 0;

	while (!ckit_window_should_quit(window)) {
		set_bitmap_gradient(window, x_offset, y_offset);
		ckit_window_draw_quad(window, 100, 100, 128, 128);



		ckit_window_draw_bitmap(window);
		
		x_offset++;
		// y_offset++;
	}

	ckit_free(window);

	ckit_cleanup();
	return 0;
}