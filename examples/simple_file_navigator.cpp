#include "src/SLib.h"
#include "test/core.h"

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

    String s_path = String_Copy("C:");
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
		}

		if (wg_listbox.events.on_list_change_final) {
			u64 index = Widget_GetSelectedRowID(&wg_listbox);

			Directory_Entry *t_entry = &ARRAY_IT(a_listing, index);

			if (t_entry->type == DIR_ENTRY_DIR) {
				File_ChangePath(&s_path, t_entry->s_name);
				Widget_LoadDirectoryList(&wg_listbox, s_path, &a_listing, false);
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
