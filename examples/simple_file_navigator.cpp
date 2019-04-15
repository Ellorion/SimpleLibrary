#include "src/SLib.h"
#include "test/core.h"

const char *app_title = "File-Navigator";

instant void
Window_SetTitle(
	Window *window_io,
	String s_title
) {
	Assert(window_io);

	String s_buffer;

	if (!String_IsEmpty(&s_title, false)) {
		String_Append(&s_buffer, S("["));
		String_Append(&s_buffer, s_title);
		String_Append(&s_buffer, S("]"));
		String_Append(&s_buffer, S(" :: "));
	}

	String_Append(&s_buffer, S(app_title));
	String_Append(&s_buffer, S("\0", 1));

	Window_SetTitle(window_io, s_buffer.value);

	String_Destroy(&s_buffer);
}

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
	String s_section_id,
	Config config
) {
	String s_config;

	String_Append(&s_config, S(R"(
{/:}basic
path_latest	"{path_latest}"
font-file	"{font-file}"
font-size	{font-size}

{/:}window
is_zooming	{is_zooming}
	)"));

	char c_font_size[20];
	itoa(config.basic.font_size, c_font_size, 10);

	String_Replace(&s_config, S("{/:}")         , s_section_id);
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
Config_Parse(
	Config *config,
	String s_section_id,
	String s_config
) {
	Assert(config);

	String s_data;

	Parser parser_config = Parser_Load(s_config);

	while (Parser_IsRunning(&parser_config)) {
		/// skip non-section identifier
		if (!Parser_GetSectionNameRef(&parser_config, &s_data, s_section_id))
			continue;

		if (s_data == "basic") {
			while (Parser_IsRunning(&parser_config)) {
				/// do not skip section data -> parsing in outer loop
				if (Parser_IsSection(&parser_config, s_section_id))
					break;

				Parser_GetStringRef(&parser_config, &s_data, PARSER_MODE_SEEK, false);

				if (s_data == "font-file") {
					Parser_GetStringRef(&parser_config, &s_data, PARSER_MODE_SEEK, false);

					if (File_Exists(s_data)) {
						String_Clear(&config->basic.s_font);
						String_Append(&config->basic.s_font, s_data);
					}
				}
				else
				if (s_data == "font-size") {
					Parser_GetNumber(&parser_config, &s_data);

					if (!Parser_HasError(&parser_config)) {
						config->basic.font_size = ToInt(s_data);
					}
				}
				else
				if (s_data == "path_latest") {
					Parser_GetStringRef(&parser_config, &s_data, PARSER_MODE_SEEK, false);

					/// is directory or list drives if empty
					if (   File_IsDirectory(s_data)
						OR String_IsEmpty(&s_data)
					) {
						String_Clear(&config->basic.s_path);
						String_Append(&config->basic.s_path, s_data);
					}
				}
			}
		}
		else
		if (s_data == "window") {
			while (Parser_IsRunning(&parser_config)) {
				/// do not skip section data -> parsing in outer loop
				if (Parser_IsSection(&parser_config, s_section_id))
					break;

				Parser_GetStringRef(&parser_config, &s_data, PARSER_MODE_SEEK, false);

				if (s_data == "is_zooming") {
					Parser_GetBoolean(&parser_config, &config->window.is_zooming);
				}
			}
		}
	}
}

instant void
Window_HandleEvents(Window *window) {
	/// configuration
	Config config;
	config.basic.s_path      = String_Copy("");
	config.basic.s_font      = String_Copy("default.ttf");
	config.basic.font_size   = 18;
	config.window.is_zooming = true;

	String s_config_section_id = S("/:");
    String s_config_file       = S("default.cfg");
	String s_config            = File_ReadAll(s_config_file);

	Config_Parse(&config, s_config_section_id, s_config);

	/// basics
	ShaderSet 	 shader_set = ShaderSet_Create(window);
	Font 		 font 		= Font_Load(config.basic.s_font, config.basic.font_size);
	Keyboard	*keyboard	= window->keyboard;

	if (Font_HasError(&font)) {
		MessageBox(window->hWnd, font.s_error.value, 0, MB_OK);
		return;
	}

	/// widgets
	Widget wg_listbox = Widget_CreateListBox(window, &font, {});

    Array<Widget *> ap_widgets;
    Array_Add(&ap_widgets, &wg_listbox);

    /// layout
	Layout layout;
	Layout_Create(&layout, {0, 0, window->width, window->height}, true);
	Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT);
    Layout_Add(&layout, &wg_listbox);

    Layout_Arrange(&layout);

    /// load list on startup
	Array<Directory_Entry> a_listing;
	Widget_LoadDirectoryList(&wg_listbox, config.basic.s_path, &a_listing, false);
	Window_SetTitle(window, config.basic.s_path);

	Memory_AddSegment(&window->a_segments_reset, window->events);
	Memory_AddSegment(&window->a_segments_reset, font.events);

	while(Window_IsRunning(window)) {
		/// events
		Window_ReadMessage(window);
		Layout_Rearrange(&layout, window);

		/// update
		Widget_Update(&ap_widgets, keyboard);

		if(keyboard->up[VK_ESCAPE]) {
			Window_Close(window);
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
					Application_Execute(config.basic.s_path);
					String_Cut(&config.basic.s_path, length_path);
				} break;

				default:
					AssertMessage(false, "Unhandled directory type.");
			}
		}

		/// render
		OpenGL_ClearScreen();

		Widget_Render(&shader_set, &ap_widgets);

		Window_Render(window);
		Widget_Reset(&ap_widgets);
	}

	/// save changes on exit
	Config_Save(s_config_file, s_config_section_id, config);
}

int main() {
	Keyboard keyboard;
	Mouse    mouse;

	Window window = Window_Create(app_title, 800, 480, true, true, &keyboard, &mouse);
	OpenGL_UseVSync(&window, true);

	Window_HandleEvents(&window);

	Window_Destroy(&window);

	return 0;
}
