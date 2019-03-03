#include "src/SLib.h"
#include "test/core.h"

/// toggle auto-click with right-click (mouse)
/// then hover over the to-be-auto-clicked
/// area for repeated left-clicks (mouse)

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	Timer timer_autoclick;
	Time_Reset(&timer_autoclick);

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;
	Assert(keyboard);

	String s_font;
	String_Append(&s_font, S("default.ttf"));
	Font font_20 = Font_Load(s_font, 20);

	if (Font_HasError(&font_20)) {
		MessageBox(window->hWnd, font_20.s_error.value, 0, MB_OK);
		return;
	}

	Widget wg_autoclick = Widget_CreateButton(window, &font_20, {}, S("Auto-Click"));

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_autoclick);

	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout, &wg_autoclick);
	}

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window, false);
		OpenGL_AdjustScaleViewport(window, false);

		Layout_Rearrange(&layout, {0, 0, window->width, window->height});

		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE])
			running = false;

		if (wg_autoclick.events.on_trigger_secondary)  {
			wg_autoclick.data.is_checked = !wg_autoclick.data.is_checked;

			if (wg_autoclick.data.is_checked)
				wg_autoclick.text.data.color = {0.0f, 0.6f, 0.0f, 1.0f};
			else
				wg_autoclick.text.data.color = {0.6f, 0.0f, 0.0f, 1.0f};
		}

		if (wg_autoclick.data.is_checked) {
			if (Time_HasElapsed(&timer_autoclick, 30)) {
				Mouse_AutoClickLeft();
			}
		}

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);

		Window_UpdateAndResetInput(window);
	}

	Widget_Destroy(&ap_widgets);
	ShaderSet_Destroy(&shader_set);
}

int main() {
	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, "Auto-Clicker", 320, 240, 32, &keyboard, &mouse);
	Window_Show(&window);
	Window_AlwaysOnTop(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);
	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}