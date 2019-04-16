#include "src/SLib.h"
#include "test/core.h"

instant void
Window_HandleEvents(
	Window *window
) {
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

	Widget_LinkRadiogroup(&ap_radiogroup);

	MemorySegment_Add(&window->a_segments_reset, window->events);
	MemorySegment_Add(&window->a_segments_reset, font_20.events);
	MemorySegment_AddWidgets(&window->a_segments_reset, &ap_widgets);

	while(Window_IsRunning(window)) {
		/// Events
		/// ===================================================================
		Window_ReadMessage(window);
		Layout_Rearrange(&layout, window);

		/// hold shift-key to get reverse order
		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE])
			Window_Close(window);

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);
	}

	Widget_Destroy(&ap_widgets);

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
