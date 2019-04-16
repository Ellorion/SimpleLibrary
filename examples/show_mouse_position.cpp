#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	Timer timer_mouse_move;
	Time_Reset(&timer_mouse_move);

	MemorySegment_Add(&window->a_segments_reset, window->events);

	while(Window_IsRunning(window)) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		if (Time_HasElapsed(&timer_mouse_move, 100)) {
			Point t_point;

			if (window->mouse)
				t_point = window->mouse->point;

			LOG_DEBUG("Mouse pos: x = " << t_point.x << " - y: " << t_point.y);
		}
	}
}

int main() {
	Mouse mouse;
	Keyboard keyboard;

	Window window = Window_Create("Hello, World!", 800, 480, true, true, &keyboard, &mouse);
	OpenGL_UseVSync(&window, true);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
