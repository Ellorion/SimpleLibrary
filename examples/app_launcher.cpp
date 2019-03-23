#include "src/SLib.h"
#include "test/core.h"

/// app.dat example
/// ===========================================================================
/// name_of_app_in_list_1, C:/path/to/app.exe
/// name_of_app_in_list_2, D:/path/to-another/app.exe
/// name_of_app_in_list_3, E:\\path\\to-yet-another\app.exe
/// name_of_app_in_list_4, F:\\my path\\to yet another again\app.exe

instant void
Window_HandleEvents(
	Window *window
) {
	MSG msg;
	bool running = true;

	/// @Info: "tray.ico" has to be in app-folder before startup,
	///        to support the tray-icon system
	window->icon.always_visible = true;

	ShaderSet shader_set = ShaderSet_Create(window);

	OpenGL_SetBlending(true);

	Keyboard *keyboard = window->keyboard;
	Assert(keyboard);

	String s_font;
	String_Append(&s_font, S("default.ttf"));
	Font font_16 = Font_Load(s_font, 16);
	Font font_20 = Font_Load(s_font, 20);

	/// make sure a font file exists
	if (Font_HasError(&font_16)) {
		MessageBox(window->hWnd, font_16.s_error.value, 0, MB_OK);
		return;
	}

	if (Font_HasError(&font_20)) {
		MessageBox(window->hWnd, font_20.s_error.value, 0, MB_OK);
		return;
	}

	Widget wg_exit_on_launch = Widget_CreateCheckBox(window, &font_20, {}, S("Exit on launch"), true);
	Widget wg_list           = Widget_CreateListBox( window, &font_20, {});
	Widget wg_filter_label   = Widget_CreateLabel(   window, &font_20, {}, S("Filter:"));
	Widget wg_filter_data    = Widget_CreateTextBox( window, &font_20, {}, false);

	wg_list.data.is_filter_case_sensitive = false;

	Rect *rect_filter_label_padding = &wg_filter_label.text.data.rect_padding;
	rect_filter_label_padding->x =+ 2;
	rect_filter_label_padding->w =+ 2;

	/// tab-stop order (for tab-ables)
	Array<Widget *> ap_widgets;
	Array_Add(&ap_widgets, &wg_list);
	Array_Add(&ap_widgets, &wg_filter_label);
	Array_Add(&ap_widgets, &wg_filter_data);
	Array_Add(&ap_widgets, &wg_exit_on_launch);

	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
	{
		Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT, 1);
		Layout_Add(&layout, &wg_filter_label);
		Layout_Add(&layout, &wg_filter_data);
		Layout_Add(&layout, &wg_exit_on_launch);

		Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout, &wg_list);
	}

	String s_app_data_file = S("app.dat");

	File_Watcher watch_app_data;
	File_Watch(&watch_app_data, s_app_data_file);

	Array<String> as_appdata;

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(window, &msg, &running, false);
		OpenGL_AdjustScaleViewport(window, false);
		Layout_Rearrange(&layout, window);

		Widget_Update(&ap_widgets, keyboard);

		/// hot-load app list data
		if (File_HasChanged(&watch_app_data)) {
			String s_data = File_ReadAll(watch_app_data.s_filename);

			if (!s_data.value) {
				Window_SetTitle(window, "File: \"app.dat\" does not exists.");
			}
			else {
				as_appdata = String_Split(&s_data, S("\r\n"), DELIMITER_IGNORE, false);

				Widget_ClearRows(&wg_list);

				/// fill list with app-names
				FOR_ARRAY(as_appdata, it) {
					String *ts_line = &ARRAY_IT(as_appdata, it);
					String s_section = String_GetDelimiterSection(ts_line, S(","), 0);
					Widget_AddRow(&wg_list, s_section);
					String_Destroy(&s_section);
				}
			}
		}

		/// append text to filter, if list has focus
		/// (does support backspace, but will not show text cursor,
		///  since textbox does not have focus)
		if (wg_list.data.has_focus AND keyboard->is_key_sym AND keyboard->is_down) {
			String *ts_data = Widget_GetTextData(&wg_filter_data);
			bool is_tab = ((char)keyboard->key_sym == '\t');

			if (XOR(wg_filter_data.text.allow_tab_input, !is_tab)) {
				String_Insert(ts_data, (char)keyboard->key_sym, ts_data->length);
				Widget_Update(&wg_filter_data);
			}
		}

		/// update list filter
        if (wg_filter_data.events.on_text_change) {
			String *s_dest   = &wg_list.data.s_row_filter;
			String *s_source = Widget_GetTextData(&wg_filter_data);

        	String_Overwrite(s_dest, *s_source);
        	Widget_Update(&wg_list);
        }

        /// exit program
		if (keyboard->up[VK_ESCAPE])
			running = false;

		/// start selected app from list
		if (wg_list.events.on_list_change_final) {
			String s_select = Widget_GetSelectedRow(&wg_list);

			if (!String_IsEmpty(&s_select)) {
				FOR_ARRAY(as_appdata, it) {
					String *ts_app = &ARRAY_IT(as_appdata, it);

					if (!String_StartWith(ts_app, s_select, true))
						continue;

					String s_file = String_GetDelimiterSection(ts_app, S(","), 1);
					File_Execute(s_file);
					String_Destroy(&s_file);
				}

				String_Destroy(&s_select);

				if (wg_exit_on_launch.data.is_checked)
					running = false;
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

	Window_Create(&window, "Application-Launcher", 512, 512 / 16 * 9, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
