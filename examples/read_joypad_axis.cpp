#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(Window *window) {
	Joypad joypad;
	Joypad_Init_XInput(&joypad);

	while(window->is_running) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);

		Joypad_GetInput(&joypad, 0);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		LOG_DEBUG("X-Axis Section: " << Joypad_GetSection(joypad.state.thumb_left_x, 3000, 10));
		LOG_DEBUG("Y-Axis Section: " << Joypad_GetSection(joypad.state.thumb_left_y, 3000, 10));

		Window_Render(window);
	}
}

int main() {
	Window window = Window_Create("Hello, World!", 800, 480, true);
	Window_Show(&window);

	OpenGL_UseVSync(&window, true);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
