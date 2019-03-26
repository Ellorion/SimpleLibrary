#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;

	Font font_20 = Font_Load(S("default.ttf"), 20);

	Widget wg_radio_1 = Widget_CreateCheckBox(window, &font_20, {}, S("radio 1"), true);
	Widget wg_radio_2 = Widget_CreateCheckBox(window, &font_20, {}, S("radio 2"), false);
	Widget wg_radio_3 = Widget_CreateCheckBox(window, &font_20, {}, S("radio 3"), false);
	Widget wg_radio_4 = Widget_CreateCheckBox(window, &font_20, {}, S("radio 4"), false);

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_radio_1);
	Array_Add(&ap_widgets, &wg_radio_2);
	Array_Add(&ap_widgets, &wg_radio_3);
	Array_Add(&ap_widgets, &wg_radio_4);

	Array<Widget *> ap_radiogroup;
	Array_Add(&ap_radiogroup, &wg_radio_1);
	Array_Add(&ap_radiogroup, &wg_radio_2);
	Array_Add(&ap_radiogroup, &wg_radio_3);
	Array_Add(&ap_radiogroup, &wg_radio_4);

	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, false);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout, &wg_radio_1);
		Layout_Add(&layout, &wg_radio_2);
		Layout_Add(&layout, &wg_radio_3);
		Layout_Add(&layout, &wg_radio_4);
	}

	Widget_RadioGroup_Link(&ap_radiogroup);

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(window, &msg, &running, false);
		OpenGL_AdjustScaleViewport(window);
		Layout_Rearrange(&layout, window);

		/// hold shift-key to get reverse order
		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE])
			running = false;

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);

		Window_UpdateAndResetInput(window);
		Widget_Reset(&ap_widgets);
	}

	Widget_Destroy(&ap_widgets);

	ShaderSet_Destroy(&shader_set);
}

int main() {
	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
