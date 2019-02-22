#include "src/SLib.h"
#include "test/core.h"

instant void
Config_Save(
	String s_filename,
	String s_path
) {
	String s_config;

	String_Append(&s_config, S("/:default\n"));
	String_Append(&s_config, S("path\t\"{path}\"\n"));

	String_Replace(&s_config, S("{path}"), s_path);

	File file_config = File_Open(s_filename, "w");
	File_Write(&file_config, s_config);
	File_Close(&file_config);

	String_Destroy(&s_config);
}

instant void
Window_HandleEvents(Window *window) {
	MSG msg;
	bool running = true;

	ShaderSet 	 shader_set = ShaderSet_Create(window);
	Font 		 font 		= Font_Load(S("default.ttf"), 18);
	Keyboard	*keyboard	= window->keyboard;

	Widget wg_listbox = Widget_CreateListBox(window, &font, {});

    Array<Widget *> a_widgets;
    Array_Add(&a_widgets, &wg_listbox);

	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
	Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
    Layout_Add(&layout, &wg_listbox);

    Layout_Arrange(&layout);

    String s_config_section_id 	= S("/:");
    String s_config_file 		= S("default.cfg");
	String s_config      		= File_ReadAll(s_config_file);

	String s_data;
    String s_path = String_Copy("C:\\");

	if (String_IsEmpty(&s_config)) {
		Config_Save(s_config_file, s_path);
	}
	else {
		String s_data;

		Parser parser_config = Parser_Load(s_config);

		while (Parser_IsRunning(&parser_config)) {
			/// skip non-section identifier
			if (!Parser_GetSectionName(&parser_config, &s_data, s_config_section_id))
				continue;

			if (s_data == "default") {
				while (Parser_IsRunning(&parser_config)) {
					/// do not skip section data for parsing in outer loop
					if (Parser_IsSection(&parser_config, s_config_section_id))
						break;

					Parser_GetString(&parser_config, &s_data);

					if (s_data == "path") {
						Parser_GetString(&parser_config, &s_data);

						if (File_IsDirectory(s_data)) {
							String_Clear(&s_path);
							String_Append(&s_path, s_data);
						}
					}
				}
			}
		}
	}

	/// assumes that the at least the default path exists,
	/// or there will be no list entries
	Array<Directory_Entry> a_listing;
	Widget_LoadDirectoryList(&wg_listbox, s_path, &a_listing);

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window);

#if 0	/// resize window to observe changes
		OpenGL_AdjustScaleViewport(window, false);
		Layout_Rearrange(&layout, window);
#else
		OpenGL_AdjustScaleViewport(window, true);
#endif // 0

		Widget_Update(&a_widgets, keyboard);

		if(keyboard->up[VK_ESCAPE]) {
			running = false;
		}
		else
		if (keyboard->up[VK_BACK]) {
			File_ChangePath(&s_path, S(".."));
			Widget_LoadDirectoryList(&wg_listbox, s_path, &a_listing, false);

			Config_Save(s_config_file, s_path);
		}

		if (wg_listbox.events.on_list_change_final) {
			u64 index = Widget_GetSelectedRowID(&wg_listbox);

			Directory_Entry *t_entry = &ARRAY_IT(a_listing, index);

			if (t_entry->type == DIR_ENTRY_DIR) {
				File_ChangePath(&s_path, t_entry->s_name);
				Widget_LoadDirectoryList(&wg_listbox, s_path, &a_listing, false);

				Config_Save(s_config_file, s_path);
			}
			else
			if (t_entry->type == DIR_ENTRY_FILE) {
				u64 length_path = s_path.length;

				File_ChangePath(&s_path, t_entry->s_name);
				File_Execute(s_path);
				String_Cut(&s_path, length_path);
			}
		}

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &a_widgets);

		Window_UpdateAndResetInput(window);
	}
}

int main() {
	Test_Run();

	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, "Simple File-Navigator", 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
