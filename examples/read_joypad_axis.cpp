#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(Window *window) {
	Joypad joypad;
	Joypad_Init_XInput(&joypad);

	Memory_AddSegment(&window->a_segments_reset, window->events);

	while(Window_IsRunning(window)) {
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
	Window window = Window_Create("Hello, World!", 800, 480, true, true, 0, 0);
	OpenGL_UseVSync(&window, true);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
