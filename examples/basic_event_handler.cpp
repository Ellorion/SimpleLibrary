#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(Window *window) {
	MSG msg;
	bool running = true;

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window, false);
		OpenGL_AdjustScaleViewport(window, false);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Window_UpdateAndResetInput(window);
	}
}

int main() {
	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
