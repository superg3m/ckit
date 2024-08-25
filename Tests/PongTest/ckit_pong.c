#include "../../ckit.h"

int main() {
	ckit_init();

	u32 width = 1600;
	u32 height = 800;

	u32 padding = 40;

	u32 width_with_padding = width - padding;
	u32 height_with_padding = height - padding;

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_SUCCESS("TESTING\n");

	ckit_window_bind_icon("../../../assets/c_original_logo_icon_146611.ico");
	CKIT_Window* window = ckit_window_create(width, height, "GameEngine");

	const s32 player_x_pos = 100;
	const s32 player_y_pos = height / 2;
	const u32 player_width = 12;
	const u32 player_height = 50;

	const u32 half_player_width = player_width / 2;
	const u32 half_player_height = player_height / 2;
	Rectangle2D player = ckit_rectangle_create(player_x_pos, player_y_pos, player_width, player_height);

	const s32 ball_x_pos = width / 2;
	const s32 ball_y_pos = height / 2;
	const u32 ball_radius = 5;
	const u32 ball_diameter = ball_radius * 2;
	float ball_x_velocity = 1.0;
	float ball_y_velocity = 1.0;
	Rectangle2D ball = ckit_rectangle_create(ball_x_pos, ball_y_pos, ball_diameter, ball_diameter);

	
	const s32 ai_x_pos = width_with_padding - 100;
	const s32 ai_y_pos = height / 2;
	const u32 ai_width = 12;
	const u32 ai_height = 50;

	const u32 half_ai_width = ai_width / 2;
	const u32 half_ai_height = ai_height / 2;
	Rectangle2D ai = ckit_rectangle_create(ai_x_pos, ai_y_pos, ai_width, ai_height);

	u32 player_score = 0;
	u32 ai_score = 0;

	Boolean ai_hit = FALSE;
	Boolean player_hit = FALSE;

	CKIT_Color player_color = CKIT_COLOR_BLUE;
	CKIT_Color ai_color = CKIT_COLOR_RED;
	CKIT_Color default_color = CKIT_COLOR_WHITE;

	ckit_window_set_cursor_state(window, DISABLED);

	int mouse_x, mouse_y = 0;
	while (!ckit_window_should_quit(window)) {
		{ // UPDATE
			Boolean left_check   = ball.x <= 0;
			Boolean right_check  = (ball.x + ball.width) >= width_with_padding;

			Boolean bottom_check = ball.y <= 0;
			Boolean top_check    = (ball.y + ball.height) >= height_with_padding;

			if (left_check || right_check) {
				ball_x_velocity *= -1;
				ai_score += left_check;
				player_score += right_check;
				LOG_DEBUG("Player Score: %d\n", player_score);
				LOG_DEBUG("AI Score: %d\n\n", ai_score);
			}

			if (bottom_check || top_check) {
				ball_y_velocity *= -1;
			}

			if (ckit_rectangle_check_aabb_collision(player, ball)) {
				player_hit = TRUE;
				ai_hit = FALSE;
				ball_x_velocity *= -1;
			}

			if (ckit_rectangle_check_aabb_collision(ai, ball)) {
				player_hit = FALSE;
				ai_hit = TRUE;
				ball_x_velocity *= -1;
			}
						
			ai.y = ball.y;

			ball.x += ball_x_velocity;
			ball.y += ball_y_velocity;

			ckit_window_get_mouse_position(window, &mouse_x, &mouse_y);
			player.y = mouse_y;
		}


		{ // RENDER
			ckit_window_clear_color(window, CKIT_COLOR_BLACK);

			ckit_window_draw_quad(window, 0, height_with_padding, width_with_padding, 1, CKIT_COLOR_RED); // x axis
			ckit_window_draw_quad(window, width_with_padding, 0, 1, height_with_padding, CKIT_COLOR_BLUE); // y axis

			// s32 player_center_y = ((float)player.y - (half_player_height));

			ckit_window_draw_quad(window, player.x, player.y, player.width, player.height, player_color); // ACTUAL PLAYER_PADDLE

			CKIT_Color ball_color = default_color;

			if (ai_hit) {
				ball_color = ai_color;
			} else if (player_hit) {
				ball_color = player_color;
			}

			ckit_window_draw_quad(window, (s32)ball.x, (s32)ball.y, ball_diameter, ball_diameter, ball_color); // ball
			ckit_window_draw_quad(window, (s32)ai.x, (s32)ai.y, ai.width, ai.height, ai_color); // ai_paddle

			// ckit_window_draw_circle(window, (s32)10, (s32)0, ball_radius, TRUE, (CKIT_Color){255, 255, 255, 255});
			// For some reason the draw circle is not correct it seems like the pixel testing is broken??? Somehow?

			ckit_window_draw_bitmap(window);
		}
	}

	ckit_window_free(window); // hmm how can I make this safer?

	ckit_cleanup();
	return 0;
}