#include "../../ckit.h"

int main() {
	ckit_init();

	u32 width = 800;
	u32 height = 600;


	ckit_window_bind_icon("../../../assets/c_original_logo_icon_146611.ico");
	CKIT_Window* window = ckit_window_create(width, height, "GameEngine");


	while (!ckit_window_should_quit(window)) {

	}

	ckit_free(window);

	ckit_cleanup();
	return 0;
}