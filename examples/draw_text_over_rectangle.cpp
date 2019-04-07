#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;

	Font font = Font_Load(S("default.ttf"), 20);

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
		String_Append(&s_box_data, S("Hello"));

		text_box.shader_set = &shader_set;
		text_box.data.rect  = rect_box;
		text_box.font       = &font;
		text_box.data.color = Color_MakeGrey(0.4f);

		String_Append(&text_box.s_data, s_box_data);

		String_Destroy(&s_box_data);

		Text_Update(&text_box);
	}

	while(window->is_running) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);

		if (keyboard->up[VK_ESCAPE])
			window->is_running = false;

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		ShaderSet_Use(&shader_set, SHADER_PROG_RECT);
		Rect_Render(&shader_set, &vertex_rect);

		ShaderSet_Use(&shader_set, SHADER_PROG_TEXT);
		Text_Render(&text_box);

		Window_UpdateAndResetInput(window);
		Font_ResetEvents(&font);
	}

	Text_Destroy(&text_box);
	Vertex_Destroy(&vertex_rect);
	ShaderSet_Destroy(&shader_set);
}

int main() {
	Keyboard keyboard;
	Window window = Window_Create("Hello, World!", 800, 480, true, &keyboard);
	Window_Show(&window);

	OpenGL_UseVSync(&window, false);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
