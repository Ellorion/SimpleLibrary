#include "src/SLib.h"
#include "src/test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;

	Font font = Font_Load("default.ttf", 20);

	Rect rect_box = {10, 20, 300, 200};

	Vertex vertex_rect = Vertex_Create();
	Vertex_AddRect32(&vertex_rect, rect_box, Color_MakeGrey(1.0f));

	Rect_Resize(&rect_box, -1);
	Vertex_AddRect32(&vertex_rect, rect_box, Color_MakeGrey(0.0f));

	Rect_Resize(&rect_box, -2);
	Vertex_AddRect32(&vertex_rect, rect_box, Color_MakeGrey(1.0f));

	Text text_box;
	{
		String s_box_data;
		String_Append(&s_box_data, "Hello");

		text_box.shader_set = &shader_set;
		text_box.data.rect  = rect_box;
		text_box.font       = &font;
		text_box.data.color = Color_MakeGrey(0.4f);

		String_Append(&text_box.s_data, s_box_data.value, s_box_data.length);

		String_Destroy(&s_box_data);

		Text_Update(&text_box);
	}

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window);
		OpenGL_AdjustScaleViewport(window);

		if (keyboard->up[VK_ESCAPE])
			running = false;

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		ShaderSet_Use(&shader_set, SHADER_PROG_RECT);
		Rect_Render(&shader_set, &vertex_rect);

		ShaderSet_Use(&shader_set, SHADER_PROG_TEXT);
		Text_Render(&text_box);

		Window_UpdateAndResetInput(window);
	}

	Text_Destroy(&text_box);
	Vertex_Destroy(&vertex_rect);
	ShaderSet_Destroy(&shader_set);
}

int main() {
	Window window;

	Keyboard keyboard;
	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, false);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
