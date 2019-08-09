#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	Keyboard *keyboard = window->keyboard;
	Font font = Font_Load(S("default.ttf"), 20);
	ShaderSet shader_set = ShaderSet_Create(window);

	MemorySegment_Add(&window->a_segments_reset, window->events);

	Array<Vertex> a_vertex_chars;

	Vertex_AddText(&a_vertex_chars, &shader_set, &font,
				 {0, 0, 200, font.size},
				 {1, 1, 1, 1},
				 S("Hello, World!"),
				 TEXT_ALIGN_X_MIDDLE);

	while(Window_IsRunning(window)) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);

		if (keyboard->up[VK_ESCAPE])
			Window_Close(window);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Vertex_RenderText(&shader_set, &a_vertex_chars);
	}

	ShaderSet_Destroy(&shader_set);
}

int main() {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_Create("Hello, World!", 800, 480, true, true, &keyboard, &mouse);

	OpenGL_UseVSync(&window, true);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
