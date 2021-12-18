#include "../SimpleLibrary/src/SLib.h"

struct Game {
    String s_name_console;
    String s_name;
    bool completed = false;
    u8 ranking = 0;
};

constexpr
Stream &
operator<<(Stream &out, const Array<Game> a_data) {
    out << S("# Console, Name, Completed, Ranking\n");

    FOR_ARRAY_AUTO(a_data, it) {
        if (String_IsEmpty(it->s_name)) {
            continue;
        }

        if (!String_IsEmpty(it->s_name_console)) {
            out << it->s_name_console;
        }
        out << S(";");

        out << it->s_name;
        out << S(";");

        if (it->completed) {
            out << S("X");
        }
        out << S(";");

        if (it->ranking) {
            out << Convert_IntToString(it->ranking);
        }
        out << S(";");

        out << S("\n");
    }

    out << S("\n");

    return out;
}

void Storage_Load(Array<Game> &a_data, File &file) {
    String s_data;

    while (File_ReadUntil(file, &s_data, S("\n"))) {
        if (String_IsEmpty(s_data, true))
            continue;

        if (String_StartWith(s_data, S("#"), false))
            continue;

        auto row = String_Split(s_data, S(";"), DELIMITER_IGNORE, true);

        Game game = {ARRAY_IT(row, 0), ARRAY_IT(row, 1)};

        if (Array_Count(row) > 2) {
            game.completed = !String_IsEmpty(ARRAY_IT(row, 2));
        }

        if (Array_Count(row) > 3) {
            game.ranking = Convert_ToInt(ARRAY_IT(row, 3));
        }

        Array_Add(a_data, game);
    }
}

String createGameLabel(Game *game) {
    String s_text;

    if (!String_IsEmpty(game->s_name_console)) {
        String_Append(s_text, S("["));
        String_Append(s_text, game->s_name_console);
        String_Append(s_text, S("] "));
    }

    String_Append(s_text, game->s_name);

    if (game->ranking) {
        auto rank = Convert_IntToString(game->ranking);
        String_Append(s_text, S(" [*"));
        String_Append(s_text, rank);
        String_Append(s_text, S("] "));
        String_Destroy(rank);
    }

    return s_text;
}

Game *findGameByListItemName(const Widget &widget, const String &s_name) {
    auto a_games = (Array<Game> *)ARRAY_IT(widget.a_dataLink, 0);

    u64 index;

    Array_Find(*a_games, nullptr, &index, [&](auto g1, auto g2) {
        auto game_item = createGameLabel(&g1);
        bool is_equal = (game_item == s_name);
        String_Destroy(game_item);
        return is_equal;
    });

    Game *game = nullptr;

    if (index >= 0) {
        game = &ARRAY_IT(*a_games, index);
    }

    return game;
}

void onListBoxDrawItem(Widget *widget, Widget_Item *item) {
    Assert(!Array_IsEmpty(widget->a_dataLink));

    auto curGame = findGameByListItemName(*widget, item->s_name);
    Assert(curGame);

    if (curGame->completed) {
        item->colorText = Color::Blue;
    }

    item->s_name = createGameLabel(curGame);
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

    Array_Add(wgList.a_dataLink, (void *)&a_games);
	wgList.OnDrawItem = onListBoxDrawItem;

    FOR_ARRAY_AUTO(a_games, game) {
        auto item = createGameLabel(game);
        Array_Add(*a_list, item);
    }

	Array_Sort_Ascending(a_list);
	Widget_SetFilterCaseSensitive(wgList, false);

	while(window.is_running) {
        Window_ReadMessage(window);
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

        if (wgList.events.on_list_change_final) {
            auto s_name = Widget_GetSelectedRowRef(wgList);
            auto game = findGameByListItemName(wgList, s_name);

            String s_url_amazon;
            String_Append(s_url_amazon, S("https://www.amazon.de/s?k="));
            String_Append(s_url_amazon, game->s_name_console);

            auto as_words = String_SplitRef(game->s_name, S(" "), DELIMITER_IGNORE, false);

            FOR_ARRAY_AUTO(as_words, it) {
                String_Append(s_url_amazon, S("+"));
                String_Append(s_url_amazon, *it);
            }

            Application_OpenURL(s_url_amazon);
            String_Destroy(s_url_amazon);
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
