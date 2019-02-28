#include "src/SLib.h"
#include "test/core.h"

/// app.dat example
/// =================================================
/// name_of_app_in_list_1, C:/path/to/app.exe
/// name_of_app_in_list_2, D:/path/to-another/app.exe

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;
	Assert(keyboard);

	String s_font;
	String_Append(&s_font, S("default.ttf"));
	Font font_16 = Font_Load(s_font, 16);
	Font font_20 = Font_Load(s_font, 20);

	if (Font_HasError(&font_16)) {
		MessageBox(window->hWnd, font_16.s_error.value, 0, MB_OK);
		return;
	}

	if (Font_HasError(&font_20)) {
		MessageBox(window->hWnd, font_20.s_error.value, 0, MB_OK);
		return;
	}

	Widget wg_exit           = Widget_CreateButton(  window, &font_16, {}, S("Exit (Esc)"));
	Widget wg_exit_on_launch = Widget_CreateCheckBox(window, &font_20, {}, S("Exit on launch"), true);
	Widget wg_list           = Widget_CreateListBox( window, &font_20, {});

	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_list);
	Array_Add(&ap_widgets, &wg_exit_on_launch);
	Array_Add(&ap_widgets, &wg_exit);

	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
		Layout_Add(&layout, &wg_exit_on_launch);
		Layout_Add(&layout, &wg_exit);

		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout, &wg_list);
	}

	String s_app_data_file = S("app.dat");

	File_Watcher watch_app_data;
	File_Watch(&watch_app_data, s_app_data_file);

	Array<String> as_appdata;

	bool event_app_start = true;

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window, false);
		OpenGL_AdjustScaleViewport(window, false);
		Layout_Rearrange(&layout, window);

		Keyboard_WaitForIdle(keyboard, &event_app_start);

		Widget_Update(&ap_widgets, keyboard);

		if (File_HasChanged(&watch_app_data)) {
			String s_data = File_ReadAll(watch_app_data.s_filename);

			if (!s_data.value) {
				Window_SetTitle(window, "File: \"app.dat\" does not exists.");
			}
			else {
				as_appdata = String_Split(&s_data, S("\r\n"), DELIMITER_IGNORE, false);

				Widget_ClearRows(&wg_list);

				FOR_ARRAY(as_appdata, it) {
					String *ts_line = &ARRAY_IT(as_appdata, it);
					String s_section = String_GetDelimiterSection(ts_line, S(","), 0);
					Widget_AddRow(&wg_list, s_section);
					String_Destroy(&s_section);
				}
			}
		}

		if (keyboard->up[VK_ESCAPE])
			running = false;

		if (wg_list.events.on_list_change_final) {
			String *ts_line = &ARRAY_IT(as_appdata, wg_list.data.active_row_id);
			String s_file = String_GetDelimiterSection(ts_line, S(","), 1);
			File_Execute(s_file);
			String_Destroy(&s_file);

			if (wg_exit_on_launch.data.is_checked)
				running = false;
		}

		if (wg_exit.events.on_trigger) {
			running = false;
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

	Window_Create(&window, "Application-Launcher", 512, 512 / 16 * 9, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
