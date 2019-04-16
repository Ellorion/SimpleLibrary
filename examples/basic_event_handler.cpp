#include "src/SLib.h"
#include "test/core.h"

int main() {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_Create(
		"Demo-Application",
		800, 480,
		true, true,
		&keyboard, &mouse
	);

	OpenGL_UseVSync(&window, true);

	MemorySegment_Add(&window.a_segments_reset, window.events);

	while(Window_IsRunning(&window)) {
		Window_ReadMessage(&window);

		/// Events
		/// ===================================================================
		if (keyboard.up[VK_ESCAPE])
			Window_Close(&window);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();
	}

	Window_Destroy(&window);

	return 0;
}
