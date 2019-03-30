#include "src/SLib.h"
#include "test/core.h"

String s_setting_data_example = S(R"(
# app.dat example
# ===========================================================================
# name_of_app_in_list_1, C:/path/to/app.exe
# name_of_app_in_list_2, D:/path/to-another/app.exe
# name_of_app_in_list_3, E:\\path\\to-yet-another\app.exe
# name_of_app_in_list_4, F:\\my path\\to yet another again\app.exe
# open_download_link_since_file_does_not_exist, abc:/invalid.exe, http://www.example.com
# open_url, , http://www.example.com
# store_test_as_list_item_for_remembering
# open_folder, C:/
)");

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

	/// main widget layout
	/// =======================================================================
	Widget wg_list           = Widget_CreateListBox( window, &font_20, {});
	Widget wg_filter_label   = Widget_CreateLabel(   window, &font_20, {}, S("Filter:"));
	Widget wg_filter_data    = Widget_CreateTextBox( window, &font_20, {}, false);
	Widget wg_settings_show  = Widget_CreateButton(  window, &font_16, {}, S("Settings"));

	wg_list.data.is_filter_case_sensitive = false;

	Rect *rect_filter_label_padding = &wg_filter_label.text.data.rect_padding;
	rect_filter_label_padding->x =+ 2;
	rect_filter_label_padding->w =+ 2;

	/// tab-stop order (for tab-ables)
	Array<Widget *> ap_widgets_main;
	Array_Add(&ap_widgets_main, &wg_list);
	Array_Add(&ap_widgets_main, &wg_filter_label);
	Array_Add(&ap_widgets_main, &wg_filter_data);
	Array_Add(&ap_widgets_main, &wg_settings_show);

	Layout layout_main;
	Layout_Create(&layout_main, {0, 0, window->width, window->height}, true);
	{
		Layout_CreateBlock(&layout_main, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT, 1);
		Layout_Add(&layout_main, &wg_filter_label);
		Layout_Add(&layout_main, &wg_filter_data);
		Layout_Add(&layout_main, &wg_settings_show);

		Layout_CreateBlock(&layout_main, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout_main, &wg_list);
	}

	/// settings widget layout
	/// =======================================================================
	Widget wg_settings_text   = Widget_CreateTextBox(window, &font_20, {}, true);
	Widget wg_settings_cancel = Widget_CreateButton( window, &font_16, {}, S("Cancel"));
	Widget wg_settings_apply  = Widget_CreateButton( window, &font_16, {}, S("Apply"));

	Array<Widget *> ap_widgets_settings;
	Array_Add(&ap_widgets_settings, &wg_settings_text);
	Array_Add(&ap_widgets_settings, &wg_settings_cancel);
	Array_Add(&ap_widgets_settings, &wg_settings_apply);

	Layout layout_settings;
	Layout_Create(&layout_settings, {0, 0, window->width, window->height}, true);
	{
		Layout_CreateBlock(&layout_settings, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT);
		Layout_Add(&layout_settings, &wg_settings_cancel);
		Layout_Add(&layout_settings, &wg_settings_apply);

		Layout_CreateBlock(&layout_settings, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
		Layout_Add(&layout_settings, &wg_settings_text);
	}

	String s_app_data;
	String s_app_data_file = S("app.dat");

	File_Watcher watch_app_data;
	File_Watch(&watch_app_data, s_app_data_file);

	Array<String> as_appdata;

	Layout          *layout_active     = &layout_main;
	Array<Widget *> *ap_widgets_active = &ap_widgets_main;

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(window, &msg, &running, false);
		OpenGL_AdjustScaleViewport(window, false);
		Layout_Rearrange(layout_active, window);

		if (window->hotkey_triggered[KEYBOARD_HOTKEY_01])
			Window_Show(window);

		Widget_Update(ap_widgets_active, keyboard);

		/// hot-load app list data
		if (File_HasChanged(&watch_app_data)) {
			String_Destroy(&s_app_data);
			s_app_data = File_ReadAll(watch_app_data.s_filename);

			if (!s_app_data.value) {
				Window_SetTitle(window, "File: \"app.dat\" does not exists.");
			}
			else {
				Array_Clear(&as_appdata);
				as_appdata = String_SplitLinesRef(&s_app_data, false);

				Widget_ClearRows(&wg_list);

				/// fill list with app-names
				FOR_ARRAY(as_appdata, it) {
					String *ts_line = &ARRAY_IT(as_appdata, it);

					/// skip comment
					if (String_StartWith(ts_line, S("#"), true))
						continue;

					String s_section = String_GetDelimiterSectionRef(ts_line, S(","), 0);
					Widget_AddRow(&wg_list, s_section);
				}
			}
		}

		/// append text to filter, if list has focus
		/// (does support backspace, but will not show text cursor,
		///  since textbox does not have focus)
		if (wg_list.data.has_focus) {
			if (keyboard->is_key_sym AND keyboard->is_down) {
				String *ts_data = Widget_GetTextData(&wg_filter_data);
				bool is_tab = ((char)keyboard->key_sym == '\t');

				if (    !keyboard->pressing[VK_CONTROL]
					AND XOR(wg_filter_data.text.allow_tab_input, !is_tab)
				) {
					String_Insert(ts_data, (char)keyboard->key_sym, ts_data->length);
					Widget_Update(&wg_filter_data);
				}
			}
			else
			if (keyboard->up[VK_DELETE]) {
				String *ts_data = Widget_GetTextData(&wg_filter_data);
				String_Clear(ts_data);
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

        /// hide program
		if (keyboard->up[VK_ESCAPE])
			Window_Hide(window);

		/// keyboard way to exit app
		if (keyboard->up[VK_F4])
			running = false;

		/// center window, in case of ocd
		if (keyboard->up[VK_F12])
			Window_ToCenterPosition(window);

		/// start selected app from list
		if (wg_list.events.on_list_change_final) {
			String s_select = Widget_GetSelectedRowRef(&wg_list);

			if (!String_IsEmpty(&s_select)) {
				FOR_ARRAY(as_appdata, it) {
					String *ts_app = &ARRAY_IT(as_appdata, it);

					if (!String_StartWith(ts_app, s_select, true))
						continue;

					bool did_execute = true;

					String s_file = String_GetDelimiterSectionRef(ts_app, S(","), 1);

					/// execute application
					if (!Application_Execute(s_file)) {
						/// maybe a directory was supposed to be opened in explorer?
						if (!Application_OpenDirectory(s_file)) {
							/// open download url in default browser (if available)
							String s_download_url = String_GetDelimiterSectionRef(ts_app, S(","), 2);
							did_execute = Application_OpenURL(s_download_url);
						}
					}

					if (did_execute)
						Window_Hide(window);

					break;
				}

				String_Destroy(&s_select);
			}
		}

		/// settings handling
		if (wg_settings_show.events.on_trigger) {
			Widget_SwapLayout(	&ap_widgets_active,
								&layout_active,
								&ap_widgets_settings,
								&layout_settings);

			String *s_settings_data = Widget_GetTextData(&wg_settings_text);
			*s_settings_data = File_ReadAll(s_app_data_file);

			if (String_IsEmpty(s_settings_data)) {
				String_Append(s_settings_data, s_setting_data_example);
				String_TrimLeft(s_settings_data);
			}
		}

		if (wg_settings_cancel.events.on_trigger) {
			Widget_SwapLayout(	&ap_widgets_active,
								&layout_active,
								&ap_widgets_main,
								&layout_main);
		}

		if (wg_settings_apply.events.on_trigger) {
			Widget_SwapLayout(	&ap_widgets_active,
								&layout_active,
								&ap_widgets_main,
								&layout_main);

			String *s_settings_data = Widget_GetTextData(&wg_settings_text);

			File file_settings = File_Open(s_app_data_file, "wb+");
			File_Write(&file_settings, *s_settings_data);
			File_Close(&file_settings);
		}

		/// Render
		/// ===================================================================
		if (Window_IsVisible(window)) {
			OpenGL_ClearScreen();

			Widget_Render(&shader_set, ap_widgets_active);
		}

		Window_UpdateAndResetInput(window);
		Widget_Reset(ap_widgets_active);
	}

	Widget_Destroy(&ap_widgets_main);
	Widget_Destroy(&ap_widgets_settings);
	ShaderSet_Destroy(&shader_set);
}

APPLICATION_MAIN {
	Application app;
	Window      window;
	Keyboard    keyboard;
	Mouse       mouse;

	bool hide_window_startup = false;

	/// parse command line arguments
	Parser parser_args = Parser_Load(S(_cmd_text));

	String s_command_arg;
	while(Parser_IsRunning(&parser_args)) {
		Parser_GetStringRef(&parser_args, &s_command_arg, PARSER_MODE_SEEK, false);

		if (s_command_arg == "/minimize") {
			hide_window_startup = true;
		}
	}

	/// random const guid
    if (!Application_IsSingleInstance(&app, S("056d62b3-1263-4a57-82e1-11e238fec793")))
		return 0;

	/// press "control + shift + l" to show application window from everywhere while running
	Application_RegisterHotKey(KEYBOARD_HOTKEY_01, MOD_CONTROL | MOD_SHIFT, 'l');

	Window_Create(&window, "Application-Launcher", 512, 512 / 16 * 9, 32, &keyboard, &mouse);

	if (hide_window_startup)
		Window_Hide(&window);
	else
		Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	Application_Destroy(&app);

	return 0;
}
