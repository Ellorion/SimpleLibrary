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

	Widget w_button_menu_1   = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, S("menu_1"));
	Widget w_button_menu_2   = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, S("menu_2"));
	Widget w_button_menu_3   = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, S("menu_3"));

	Widget w_button_top_1    = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, S("top_1"));
	Widget w_button_top_2    = Widget_CreateButton(window, &font_20, {0, 0, 100, 30}, S("top_2"));
	Widget w_combobox        = Widget_CreateComboBox(window, &font_20, {}, 200);

	Widget *w_combo_list = &ARRAY_IT(w_combobox.a_subwidgets, 2);

	Widget_AddRow(&w_combobox, S("Hello"));

	w_combo_list->data.color_background = Color_MakeGrey(0.5f);

	Widget w_button_center_1 = Widget_CreateCheckBox(window, &font_20, {0, 0, 100, 30}, S("center_1"), false);
	Widget w_spreader        = Widget_CreateSpreader(window);
	Widget w_numpic          = Widget_CreateNumberPicker(window, &font_20, {0, 0, 100, 30}, {0, 5, 3, 1});

	w_button_top_2.layout_data.settings.auto_width = false;

	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, true);

	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
	Layout_Add(&layout, &w_numpic);
	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, -1);
	Layout_Add(&layout, &w_button_top_1);
	Layout_Add(&layout, &w_button_top_2);
	Layout_Add(&layout, &w_combobox);
	Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT,  -1);
	Layout_Add(&layout, &w_button_menu_1);
	Layout_Add(&layout, &w_button_menu_2);
	Layout_Add(&layout, &w_spreader);
	Layout_Add(&layout, &w_button_menu_3);
	Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
	Layout_Add(&layout, &w_button_center_1);

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &w_button_top_1);
	Array_Add(&ap_widgets, &w_button_top_2);
	Array_Add(&ap_widgets, &w_combobox);
	Array_Add(&ap_widgets, &w_button_menu_1);
	Array_Add(&ap_widgets, &w_button_menu_2);
	Array_Add(&ap_widgets, &w_button_menu_3);
	Array_Add(&ap_widgets, &w_button_center_1);
	Array_Add(&ap_widgets, &w_numpic);

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(window, &msg, &running, false);
		OpenGL_AdjustScaleViewport(window);

		Layout_Rearrange(&layout, {0, 0, window->width, window->height});
		Layout_Arrange(&layout);

		/// hold shift-key to get reverse tab order
		Widget_Update(&ap_widgets, keyboard);

		if (keyboard->up[VK_ESCAPE])
			running = false;

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

	Window_Create(&window, "Hello, World!", 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);
	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
