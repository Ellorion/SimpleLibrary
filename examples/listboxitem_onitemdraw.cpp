#include "../SimpleLibrary/src/SLib.h"

struct Game {
    String s_name;
    bool completed = false;
};

constexpr
Stream &
operator<<(Stream &out, const Array<Game> a_data) {
    FOR_ARRAY_AUTO(a_data, it) {
        out << it->s_name;
        out << S(";");

        if (it->completed) {
            out << S("X");
        }

        out << S("\n");
    }

    out << S("\n");

    return out;
}

void Storage_Load(Array<Game> &a_data, File &file) {
    String s_data;

    while (File_ReadUntil(file, &s_data, S("\n"))) {
        if (String_IsEmpty(s_data, true))
            break;

        auto row = String_Split(s_data, S(";"), DELIMITER_IGNORE, true);

        if (!Array_IsEmpty(row)) {
            Game game = {ARRAY_IT(row, 0)};

            if (Array_Count(row) > 1) {
                game.completed = !String_IsEmpty(ARRAY_IT(row, 1));
            }

            Array_Add(a_data, game);
        }
    }
}

void onListBoxDrawItem(Widget *widget, Widget_Item *item) {
    Assert(!Array_IsEmpty(widget->a_customData));

    auto a_games = (Array<Game> *)ARRAY_IT(widget->a_customData, 0);

    u64 index;

    Array_Find(*a_games, (Game){S(item->s_name)}, &index, [](auto g1, auto g2) {
        return (g1.s_name == g2.s_name);
    });

    Assert(index >= 0);

    if (ARRAY_IT(*a_games, index).completed) {
        item->colorText = Color::Blue;
    }
}

APPLICATION_MAIN {
    Keyboard keyboard;
    Mouse mouse;

    auto window = Window_CreateOGL("Game Library", 400, 230, true, &keyboard, &mouse);
    auto shader_set = ShaderSet_CreateAndSet(window);
    auto font_16 = Font_Create(window, S("default.ttf"), 16, true);

    // main
	Widget wgLabelHeader = Widget_CreateLabel(&window, &font_16, S("Game Library"), TEXT_ALIGN_X_MIDDLE);
	Widget wgList = Widget_CreateListBox(&window, &font_16);

	// input
	Widget wgLabelFilter = Widget_CreateLabel(&window, &font_16, S("Filter:"));
    Widget wgTextBoxFilter = Widget_CreateTextBox(&window, &font_16, false);

	Array<Widget *> ap_widgets;
	Array_Add(ap_widgets, &wgLabelHeader);
	Array_Add(ap_widgets, &wgList);
	Array_Add(ap_widgets, &wgLabelFilter);
	Array_Add(ap_widgets, &wgTextBoxFilter);
	MemorySegment_AddWidgets(&window.a_segments_reset, &ap_widgets);

	Layout layout;
	Layout_Create(&layout, {0, 0, window.width, window.height}, true);
	{
	    Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_BOTTOMRIGHT, 1);
		Layout_Add(&layout, &wgLabelFilter);
		Layout_Add(&layout, &wgTextBoxFilter);
		Layout_CreateBlock(&layout, LAYOUT_TYPE_Y, LAYOUT_DOCK_TOPLEFT, 1);
		Layout_Add(&layout, &wgLabelHeader);
		Layout_Add(&layout, &wgList);
	}

	// load data
	Array<Game> a_games;
	auto a_list = Widget_GetListArray(wgList);
	auto file = File_Open(S("data.txt"), "rb");
	Storage_Load(a_games, file);
	File_Close(file);

    Array_Add(wgList.a_customData, (void *)&a_games);
	wgList.OnDrawItem = onListBoxDrawItem;

    FOR_ARRAY_AUTO(a_games, game) {
        Array_Add(*a_list, game->s_name);
    }

	Array_Sort_Ascending(a_list);
	Widget_SetFilterCaseSensitive(wgList, false);

	while(window.is_running) {
        Window_ReadMessage(&window);
        Layout_Rearrange(&layout, &window);

        Widget_Update(&ap_widgets, &keyboard);

        if (wgTextBoxFilter.events.on_text_changed) {
            auto filter = Widget_GetTextData(wgTextBoxFilter);
            String_Overwrite(wgList.data.s_row_filter, *filter);
        }

        if (wgTextBoxFilter.events.on_trigger) {
            if (keyboard.pressing[VK_CONTROL]) {
                Widget_AddRowSingle(&wgList, Widget_GetTextDataRef(wgTextBoxFilter));
                Widget_SetText(wgTextBoxFilter, S(""));

                Array_Sort_Ascending(a_list);
            }
        }

        if (    keyboard.pressing[VK_CONTROL]
            AND keyboard.is_up
            AND keyboard.key_sym == KEYSYM_CONTROL('f'))
        {
            Widget_SetFocus(wgTextBoxFilter);
        }

        if (Widget_HasFocusInclSub(&wgList)) {
            if (keyboard.pressing[VK_CONTROL] AND keyboard.up[VK_DELETE]) {
                auto a_list = Widget_GetListArray(wgList);
                Array_Remove(*a_list, Widget_GetSelectedRowID(wgList));
            }
        }

        if (keyboard.up[VK_ESCAPE])
			window.is_running = false;

        OpenGL_ClearScreen();
        Widget_Render(&shader_set, &ap_widgets);
	}

	// save data
	auto [stream, success] = Stream_Open(File_Open(S("data.txt"), "wb"));
	stream << a_games;
	Stream_Close(stream);

    return 0;
}
