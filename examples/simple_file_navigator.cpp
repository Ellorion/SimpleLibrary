#include "src/SLib.h"
#include "test/core.h"

const char *app_title = "File-Navigator";

struct Config_Basic {
	String s_path;
	String s_font;
	u32 font_size;
};

struct Config_Window {
	bool is_zooming;
};

struct Config {
	Config_Basic   basic;
    Config_Window  window;
};

instant void
Config_Save(
	String s_filename,
	Config config
) {
	String s_config;

	String_Append(&s_config, S(R"(
/:basic
path_latest	"{path_latest}"
font-file	"{font-file}"
font-size	{font-size}

/:window
is_zooming	{is_zooming}
	)"));

	char c_font_size[20];
	itoa(config.basic.font_size, c_font_size, 10);

	String_Replace(&s_config, S("{path_latest}"), config.basic.s_path);
	String_Replace(&s_config, S("{font-file}")  , config.basic.s_font);
	String_Replace(&s_config, S("{font-size}")  , S(c_font_size));

	String_Replace(&s_config, S("{is_zooming}"), config.window.is_zooming ? S("true") : S("false"));

	String_TrimLeft(&s_config);
	String_TrimRight(&s_config);

	File file_config = File_Open(s_filename, "w");
	File_Write(&file_config, s_config);
	File_Close(&file_config);

	String_Destroy(&s_config);
}

instant void
Window_SetTitle(
	Window *window,
	String s_title
) {
	Assert(window);

	String s_buffer;
	String_Append(&s_buffer, S(app_title));

	if (s_title.length) {
		String_Append(&s_buffer, S(" :: "));
		String_Append(&s_buffer, s_title);
	}

	String_Append(&s_buffer, S("\0", 1));

	Window_SetTitle(window, s_buffer.value);

	String_Destroy(&s_buffer);
}

instant void
Window_HandleEvents(Window *window) {
	MSG msg;
	bool running = true;

	Config config;

	config.basic.s_path    = String_Copy("");
	config.basic.s_font    = String_Copy("default.ttf");
	config.basic.font_size = 18;

	config.window.is_zooming = true;

    String s_config_section_id 	= S("/:");
    String s_config_file 		= S("default.cfg");
	String s_config      		= File_ReadAll(s_config_file);

	String s_data;

	Parser parser_config = Parser_Load(s_config);

	while (Parser_IsRunning(&parser_config)) {
		/// skip non-section identifier
		if (!Parser_GetSectionName(&parser_config, &s_data, s_config_section_id))
			continue;

		if (s_data == "basic") {
			while (Parser_IsRunning(&parser_config)) {
				/// do not skip section data -> parsing in outer loop
				if (Parser_IsSection(&parser_config, s_config_section_id))
					break;

				Parser_GetString(&parser_config, &s_data);

				if (s_data == "font-file") {
					Parser_GetString(&parser_config, &s_data);

					if (File_Exists(s_data)) {
						String_Clear(&config.basic.s_font);
						String_Append(&config.basic.s_font, s_data);
					}
				}
				else
				if (s_data == "font-size") {
					Parser_GetNumber(&parser_config, &s_data);

					if (!Parser_HasError(&parser_config)) {
						config.basic.font_size = ToInt(&s_data);
					}
				}
				else
				if (s_data == "path_latest") {
					Parser_GetString(&parser_config, &s_data);

					/// is directory or list drives if empty
					if (   File_IsDirectory(s_data)
						OR !s_data.length
					) {
						String_Clear(&config.basic.s_path);
						String_Append(&config.basic.s_path, s_data);
					}
				}
			}
		}
		else
		if (s_data == "window") {
			while (Parser_IsRunning(&parser_config)) {
				/// do not skip section data -> parsing in outer loop
				if (Parser_IsSection(&parser_config, s_config_section_id))
					break;

				Parser_GetString(&parser_config, &s_data);

				if (s_data == "is_zooming") {
					Parser_GetBoolean(&parser_config, &config.window.is_zooming);
				}
			}
		}
	}

	ShaderSet 	 shader_set = ShaderSet_Create(window);
	Font 		 font 		= Font_Load(config.basic.s_font,config.basic.font_size);
	Keyboard	*keyboard	= window->keyboard;

	Widget wg_listbox = Widget_CreateListBox(window, &font, {});

    Array<Widget *> a_widgets;
    Array_Add(&a_widgets, &wg_listbox);

	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
	Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
    Layout_Add(&layout, &wg_listbox);

    Layout_Arrange(&layout);

	/// assumes that the at least the default path exists,
	/// or there will be no list entries
	Array<Directory_Entry> a_listing;
	Widget_LoadDirectoryList(&wg_listbox, config.basic.s_path, &a_listing, false);

	Window_SetTitle(window, config.basic.s_path);

	bool is_zooming = config.window.is_zooming;

	while(running) {
		msg = {};

		/// Events
		/// ===================================================================
		Window_ReadMessage(msg, running, window);

		OpenGL_AdjustScaleViewport(window, is_zooming);

		if (!is_zooming)
			Layout_Rearrange(&layout, window);

		Widget_Update(&a_widgets, keyboard);

		if(keyboard->up[VK_ESCAPE]) {
			running = false;
		}
		else
		if (keyboard->up[VK_BACK]) {
			File_ChangePath(&config.basic.s_path, S(".."));
			Widget_LoadDirectoryList(&wg_listbox, config.basic.s_path, &a_listing, false);

			Window_SetTitle(window, config.basic.s_path);
		}

		if (wg_listbox.events.on_list_change_final) {
			u64 index = Widget_GetSelectedRowID(&wg_listbox);

			Directory_Entry *t_entry = &ARRAY_IT(a_listing, index);

			switch (t_entry->type) {
				case DIR_ENTRY_DIR:
				case DIR_ENTRY_DRIVE: {
					File_ChangePath(&config.basic.s_path, t_entry->s_name);
					Widget_LoadDirectoryList(&wg_listbox, config.basic.s_path, &a_listing, false);

					Window_SetTitle(window, config.basic.s_path);
				} break;

				case DIR_ENTRY_FILE: {
					u64 length_path = config.basic.s_path.length;

					File_ChangePath(&config.basic.s_path, t_entry->s_name);
					File_Execute(config.basic.s_path);
					String_Cut(&config.basic.s_path, length_path);
				} break;

				default:
					AssertMessage(false, "Unhandled directory type.");
			}
		}

		/// Render
		/// ===================================================================
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &a_widgets);

		Window_UpdateAndResetInput(window);
	}

	Config_Save(s_config_file, config);
}

int main() {
	Test_Run();

	Window window;

	Keyboard keyboard;
	Mouse    mouse;

	Window_Create(&window, app_title, 800, 480, 32, &keyboard, &mouse);
	Window_Show(&window);

	OpenGL_Init(&window);
	OpenGL_SetVSync(&window, true);

	Window_HandleEvents(&window);

	OpenGL_Destroy(&window);
	Window_Destroy(&window);

	return 0;
}
