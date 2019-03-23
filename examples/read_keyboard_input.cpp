#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(Window *window) {
	MSG msg;
	bool running = true;
	bool ui_zoom_enabled = true;

	Keyboard *keyboard = window->keyboard;
	String s_keyboard;

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(window, &msg, &running, false);
		OpenGL_AdjustScaleViewport(window, ui_zoom_enabled);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		if (keyboard) {
			if (0) {}
			else if (keyboard->is_up) {
				if (keyboard->up[VK_ESCAPE])
					running = false;
			}
			else if (keyboard->is_down) {
				Keyboard_AppendKey(keyboard, &s_keyboard);
				char *c_buffer = String_CreateCBufferCopy(s_keyboard);
				LOG_DEBUG(c_buffer);
				Memory_Free(c_buffer);
			}
		}

		Window_UpdateAndResetInput(window);
	}
}

int main() {
	Keyboard keyboard;
	Window window;

	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
