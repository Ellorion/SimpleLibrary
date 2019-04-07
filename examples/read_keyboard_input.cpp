#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(Window *window) {
	Keyboard *keyboard = window->keyboard;
	String s_keyboard;

	while(window->is_running) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		if (keyboard) {
			if (0) {}
			else if (keyboard->is_up) {
				if (keyboard->up[VK_ESCAPE])
					window->is_running = false;
			}
			else if (keyboard->is_down) {
				Keyboard_AppendKey(keyboard, &s_keyboard);
				char *c_buffer = String_CreateCBufferCopy(s_keyboard);
				LOG_DEBUG(c_buffer);
				Memory_Free(c_buffer);
			}
		}

		Window_Render(window);
	}
}

int main() {
	Keyboard keyboard;

	Window window = Window_Create("Hello, World!", 800, 480, true, &keyboard);
	Window_Show(&window);

	OpenGL_UseVSync(&window, true);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
