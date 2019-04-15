#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	Timer timer_fps;
	Time_Reset(&timer_fps);

	Timer timer_fps_log;
	Time_Reset(&timer_fps_log);

	Font font = Font_Load(S("default.ttf"), 20);

	ShaderSet shader_set = ShaderSet_Create(window);

	Keyboard *keyboard = window->keyboard;

	String s_file;
	String_Append(&s_file, S(__FILE__));

	String s_data = File_ReadAll(s_file);
	String_Destroy(&s_file);

	String_Replace(&s_data, S("\r\n"), S("\n"));

	Text text = Text_Create(&shader_set, &font, &s_data, {10, 10, window->width - 20, window->height}, TEXT_ALIGN_X_LEFT);
	text.data.color = {1, 0, 0, 1};

	Memory_AddSegment(&window->a_segments_reset, window->events);
	Memory_AddSegment(&window->a_segments_reset, font.events);

	while(Window_IsRunning(window)) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);

		if (keyboard->up[VK_ESCAPE])
			Window_Close(window);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Text_Update(&text);
		Text_Render(&text);

		Window_Render(window);

		u32 fps = Time_GetFPS(&timer_fps);

		if (Time_HasElapsed(&timer_fps_log, 1000)) {
			LOG_DEBUG(fps << " fps");
		}
	}

	Text_Destroy(&text);
	Font_Destroy(&font);
	ShaderSet_Destroy(&shader_set);

	String_Destroy(&s_data);
}

int main() {
	Keyboard keyboard;
	Window window = Window_Create("Hello, World!", 800, 480, true, true, &keyboard, 0);
	OpenGL_UseVSync(&window, false);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
