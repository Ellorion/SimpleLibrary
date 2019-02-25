#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	Timer timer_mouse_move;
	Time_Reset(&timer_mouse_move);

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window, false);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		if (Time_HasElapsed(&timer_mouse_move, 100)) {
			Point t_point;

			if (window->mouse)
				t_point = window->mouse->point;

			LOG_DEBUG("Mouse pos: x = " << t_point.x << " - y: " << t_point.y);
		}

		Window_UpdateAndResetInput(window);
	}
}

int main() {
	Mouse mouse;
	Window window;

	Keyboard keyboard;

	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
