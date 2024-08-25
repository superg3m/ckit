#include "../../ckit.h"

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

	float x_pos = 100;
	float y_pos = height / 2;

	float x_velocity = 1.0;
	float y_velocity = 1.0;

	u32 player_width = 10;
	u32 player_height = 40;
	u32 half_player_width = player_width / 2;
	u32 half_player_height = player_height / 2;

	u32 ball_x_position = 10;
	u32 ball_y_position = 10;
	float ball_x_velocity = 1.0;
	float ball_y_velocity = 1.0;
	u32 ball_radius = 30;

	u32 player_score = 0;
	u32 ai_score = 0;


	while (!ckit_window_should_quit(window)) {
		ckit_window_clear_color(window, (CKIT_Color){0, 0, 0, 255});

		float offset_to_center_x = ((float)x_pos + (half_player_width));
		float offset_to_center_y = ((float)y_pos - (half_player_height));

		ckit_window_draw_quad(window, (s32)offset_to_center_x, (s32)offset_to_center_y, player_width, player_height, (CKIT_Color){255, 255, 255, 255});
		// ckit_window_draw_circle(window, (s32)ball_x_position, (s32)ball_y_position, ball_radius, TRUE, (CKIT_Color){255, 255, 255, 255});
		// For some reason the draw circle is not correct it seems like the pixel testing is broken??? Somehow?

		Boolean left_check   = ball_x_position <= 0;
		Boolean right_check  = (ball_x_velocity + ball_radius) >= width;

		Boolean bottom_check = ball_y_velocity <= 0;
		Boolean top_check    = (ball_y_velocity + ball_radius) >= height;

		if (left_check || right_check) {
			ball_x_velocity *= -1;
			ai_score += left_check;
			player_score += right_check;
			// LOG_DEBUG("Player Score: %d\n", player_score);
			// LOG_DEBUG("AI Score: %d\n", ai_score);
		}

		if (bottom_check || top_check) {
			ball_y_velocity *= -1;
		}

		int x, y = -1;
		ckit_window_get_mouse_position(window, &x, &y);
		y_pos = y;

		ckit_window_draw_bitmap(window);
	}

	ckit_window_free(window); // hmm how can I make this safer?

	ckit_cleanup();
	return 0;
}