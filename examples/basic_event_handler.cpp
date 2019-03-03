#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	ShaderSet  shader_set = ShaderSet_Create(window);
	Keyboard  *keyboard   = window->keyboard;
	Font       font       = Font_Load(S("default.ttf"), 16);

	Assert(keyboard);

	if (Font_HasError(&font)) {
		MessageBox(window->hWnd, font.s_error.value, 0, MB_OK);
		return;
	}

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window, false);
		OpenGL_AdjustScaleViewport(window, false);

		if (keyboard->up[VK_ESCAPE])
			running = false;

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();
		Window_UpdateAndResetInput(window);
	}

	ShaderSet_Destroy(&shader_set);
}

int main() {
	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, "Demo-Application", 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
