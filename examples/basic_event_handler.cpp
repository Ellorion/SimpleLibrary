#include "src/SLib.h"
#include "test/core.h"

int main() {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_Create(
		"Demo-Application",
		800, 480,
		true,
		&keyboard, &mouse
	);

	Window_Show(&window);
	OpenGL_UseVSync(&window, true);

	while(window.is_running) {
		Window_ReadMessage(&window);

		/// Events
		/// ===================================================================
		if (keyboard.up[VK_ESCAPE])
			window.is_running = false;

		/// Render
		/// ===================================================================
		Window_Render(&window);
	}

	Window_Destroy(&window);

	return 0;
}
