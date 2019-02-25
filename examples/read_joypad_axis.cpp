#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(Window *window) {
	MSG msg;
	bool running = true;
	bool ui_zoom_enabled = true;

	Joypad joypad;
	Joypad_Init_XInput(&joypad);

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window, false);
		OpenGL_AdjustScaleViewport(window, ui_zoom_enabled);

		Joypad_GetInput(&joypad, 0);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		LOG_DEBUG("X-Axis Section: " << Joypad_GetSection(joypad.state.thumb_left_x, 3000, 10));
		LOG_DEBUG("Y-Axis Section: " << Joypad_GetSection(joypad.state.thumb_left_y, 3000, 10));

		Window_UpdateAndResetInput(window);
	}
}

int main() {
	Window window;

	Window_Create(&window, "Hello, World!", 800, 480);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
