#pragma once

#define DEBUG_TEXT_DRAW_BACKGROUND	0

/// ::: Font (TrueType)
/// ===========================================================================
struct Font;

struct Codepoint {
	Font *font = 0;
	s32 codepoint = 0;
	s32 advance = 0;
	s32 left_side_bearing = 0;
	RectI rect_subpixel = {};
	Texture texture = {};
};

struct Font {
	stbtt_fontinfo info = {};
	String s_data;
	String s_error;
	s32 size = 0;
	s32 ascent = 0;
	s32 descent = 0;
	s32 linegap = 0;
	float scale = 0.0f;
	bool filter_linear = false;
	Array<Codepoint> a_codepoint;

	struct Font_Events {
		bool on_size_changed = false;
	} events;

};

bool
operator == (
	Codepoint &cp_1,
	Codepoint &cp_2
) {
	if (cp_1.codepoint == cp_2.codepoint) {
		/// check size this way
		return (cp_1.rect_subpixel == cp_2.rect_subpixel);
	}

	return false;
}

/// @Important: must call this at the end of each frame,
///             in case an event trigger has fired
instant void
Font_ResetEvents(
	Font *font
) {
	Assert(font);

	font->events = {};
}

instant bool
Font_HasError(
	Font *font
) {
	Assert(font);

	return (!String_IsEmpty(&font->s_error));
}

instant u32
Font_GetSize(
	Font *font
) {
	Assert(font);

	return font->size;
}

instant void
Font_SetScale(
	Font *font
) {
	Assert(font);

	font->scale = stbtt_ScaleForPixelHeight(&font->info, font->size);

	stbtt_GetFontVMetrics(&font->info, &font->ascent, &font->descent, &font->linegap);
	font->ascent  *= font->scale;
	font->descent *= font->scale;
	font->linegap *= font->scale;

	font->events.on_size_changed = true;
}

instant void
Font_SetSize(
	Font *font,
	s32   size
) {
	Assert(font);

	if (size < 1)
		size = 1;

	if (font->size == size)
		return;

	font->size = size;

	Font_SetScale(font);

	font->events.on_size_changed = true;
}

instant Font
Font_Load(
	String s_file,
	u32 size
) {
    Font font = {};
    String s_font_data = File_ReadAll(s_file, true);

    if (String_IsEmpty(&s_font_data, true)) {
		String_Append(&font.s_error, S("Font \""));
		String_Append(&font.s_error, s_file);
		String_Append(&font.s_error, S("\" does not exist."));
		String_Append(&font.s_error, S("\0", 1));
    }
    else {
		if (stbtt_GetNumberOfFonts((u8 *)s_font_data.value) == 0) {
			String_Append(&font.s_error, S("Font \""));
			String_Append(&font.s_error, s_file);
			String_Append(&font.s_error, S("\" is corrupted or not a valid TrueType font file."));
			String_Append(&font.s_error, S("\0", 1));
		}
		else {
			font.s_data = s_font_data;

			const u8 *c_data = (u8 *)font.s_data.value;

			stbtt_InitFont(&font.info, c_data, stbtt_GetFontOffsetForIndex(c_data, 0));
			Font_SetSize(&font, size);
		}
    }

    return font;
}

instant void Codepoint_Destroy(Codepoint *);

instant void
Font_Destroy(
	Font *font_out
) {
	Assert(font_out);

	FOR_ARRAY(font_out->a_codepoint, it) {
		Codepoint *t_codepoint = &ARRAY_IT(font_out->a_codepoint, it);
        Codepoint_Destroy(t_codepoint);
	}

	String_Destroy(&font_out->s_data);
	String_Destroy(&font_out->s_error);

	*font_out = {};
}

instant s32
Font_GetLineHeight(
	Font *font
) {
	Assert(font);

	return (font->ascent - font->descent + font->linegap + 1);
}

/// ::: Font (TrueType) ::: Codepoint
/// ===========================================================================
instant Texture
Codepoint_ToTexture(
	Font *font,
	s32 codepoint
) {
	Assert(font);

	Texture result = {};

	s32 w, h, x_off, y_off;

	unsigned char *bitmap = stbtt_GetCodepointBitmap(
			&font->info,
			0,
			font->scale,
			codepoint,
			&w,
			&h,
			&x_off,
			&y_off
		);

	if (bitmap) {
		result = Texture_Load(bitmap, w, h, GL_ALPHA, font->filter_linear);
		free(bitmap);
	}

	return result;
}

/// does NOT return codepoint data,
/// because it will be stored inside the font-struct for fast access,
/// otherwise it could be assumed it would need freeing, which would
/// corrupt the array data inside the font-struct
/// -> font => codepoint-array has ownership
instant void
Codepoint_GetData(
	Font *font,
	s32 codepoint,
	Codepoint *entry_out
) {
	Assert(font);
	Assert(entry_out);

    u64 t_index_find;

    /// when font changes (f.e. in size), comparison would not work by
    /// just checking the event on_size_changed, since the font struct
    /// is referenced to every codepoint. the old stored codepoints would
    /// hold the updated font data and could not be differentiated from
    /// the new one with a different texture size. that is why the subpixel
    /// data is used to find out, if the fontsize has changed.
    ///
    /// @Important: if the font data would change during runtime
    ///             (stored in the same referenced location) and would
    ///             result in the same subpixel data as the prev. font
    ///             (maybe for some codepoints?), the lookup would fail
    ///             and retreive the old, stored texture
    ///
    /// @TODO:		generate a checksum for font-data and store it in the
    ///             (referenced) font struct and each codepoint for
    ///             comparison
    ///
    /// @Idea:      (maybe) use texture up/down-sampling instead to
	///             reduce memory overhead?
    Codepoint t_codepoint_find;
    {
		t_codepoint_find.font      = font;
		t_codepoint_find.codepoint = codepoint;

		/// get advance / left side bearing
		stbtt_GetCodepointHMetrics(&font->info,
									codepoint,
									&t_codepoint_find.advance,
									&t_codepoint_find.left_side_bearing);

		t_codepoint_find.advance *= font->scale;
		t_codepoint_find.left_side_bearing *= font->scale;

		/// get subpixel
		stbtt_GetCodepointBitmapBoxSubpixel(&font->info,
											codepoint,
											font->scale,
											font->scale,
											0,
											0,
											&t_codepoint_find.rect_subpixel.x,
											&t_codepoint_find.rect_subpixel.y,
											&t_codepoint_find.rect_subpixel.w,
											&t_codepoint_find.rect_subpixel.h);
    }

    Codepoint *t_entry;

    if (!Array_FindOrAdd(&font->a_codepoint, t_codepoint_find, &t_entry)) {
		/// get texture
		t_entry->texture = Codepoint_ToTexture(font, codepoint);
    }

	*entry_out = *t_entry;
}

instant s32
Codepoint_GetTabWidth(
	float x,
	s32 advance_space,
	u32 tab_space_count = 4
) {
	s32 tab_width = (advance_space * (tab_space_count));

  	s32 tab_width_remaining = tab_width - (s32)x % tab_width;

	/// extend max. tab width for compatability with other editors
	if (tab_width_remaining < advance_space)
		tab_width_remaining += tab_width;

	return tab_width_remaining;
}

instant void
Codepoint_GetDataConditional(
	Font *font,
	s32 codepoint,
	Codepoint *entry_out,
	float x,
	s32 advance_space
) {
	Assert(font);
	Assert(entry_out);

	Codepoint_GetData(font, codepoint, entry_out);

	if (entry_out->codepoint == '\t')
		entry_out->advance = Codepoint_GetTabWidth(x, advance_space);
}

instant s32
Codepoint_GetAdvance(
	Font *font,
	s32 codepoint
) {
	Assert(font);

	s32 advance;

	stbtt_GetCodepointHMetrics(&font->info, codepoint, &advance, 0);
	advance *= font->scale;

	return advance;
}

instant u64
Codepoint_GetStringAdvance(
	Font *font,
	float x,
	s32 advance_space,
	String *s_data
) {
	Assert(font);
	Assert(s_data);

	u64 advance_word = 0;

	FOR(s_data->length, it) {
		char ch = s_data->value[it];

		if (ch != '\t')
			advance_word += Codepoint_GetAdvance(font, ch);
		else
			advance_word += Codepoint_GetTabWidth(x, advance_space);
	}

	return advance_word;
}

instant void
Codepoint_Destroy(
	Codepoint *codepoint_out
) {
	Assert(codepoint_out);
	Assert(codepoint_out->font);

	Texture_Destroy(&codepoint_out->texture);
}

enum CODEPOINT_TYPE {
	CODEPOINT_CONTROL,
	CODEPOINT_PUNCTUATION,
	CODEPOINT_ALPHANUMERIC
};

instant CODEPOINT_TYPE
Codepoint_GetType(
	s32 codepoint
) {
	if (codepoint <  32                    )  return CODEPOINT_CONTROL;
	if (codepoint >  31 AND codepoint <  48)  return CODEPOINT_PUNCTUATION;
	if (codepoint >  57 AND codepoint <  64)  return CODEPOINT_PUNCTUATION;
	if (codepoint >  90 AND codepoint <  97)  return CODEPOINT_PUNCTUATION;
	if (codepoint > 122 AND codepoint < 127)  return CODEPOINT_PUNCTUATION;

	return CODEPOINT_ALPHANUMERIC;
}

/// ::: Text (OpenGL rendering)
/// ===========================================================================
enum TEXT_ALIGN_X_TYPE {
	TEXT_ALIGN_X_LEFT,
	TEXT_ALIGN_X_MIDDLE,
	TEXT_ALIGN_X_RIGHT
};

enum TEXT_ALIGN_Y_TYPE {
	TEXT_ALIGN_Y_TOP,
	TEXT_ALIGN_Y_CENTER,
	TEXT_ALIGN_Y_BOTTOM
};

enum CURSOR_MOVE_TYPE {
	CURSOR_MOVE_NONE,
	CURSOR_MOVE_X,
	CURSOR_MOVE_Y,
	CURSOR_MOVE_LINE_BORDER,
	CURSOR_MOVE_PAGE
};

struct Text_Line {
	u64 width_pixel;
	String s_data;
};

struct Text_Cursor_Data {
	u64 index_select_start = 0;
	u64 index_select_end   = 0; /// also current cursor position

	Color32 color_cursor = {1.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_select = {0.5f, 0.5f, 1.0f, 1.0f};
};

struct Text_Cursor {
	CURSOR_MOVE_TYPE move_type = CURSOR_MOVE_NONE;
	s64 move_index_x = 0;
	s64 move_index_y = 0;

	Text_Cursor_Data data;
	Text_Cursor_Data data_prev;

	bool is_selecting = false;

	u32 blink_inverval_ms = 500;
	Timer timer_blinking;
	bool is_blink_on = false;
	bool show_cursor = false;

	Vertex vertex_select;
	Vertex vertex_cursor;
};

/// @todo: - rename to text_config
///        - make data from word split
///        - put that in a_row_data in widget
///        - render with updating default text
struct Text_Data {
	Rect rect 			= {}; /// draw area
	Rect rect_padding 	= {};
	Rect rect_margin    = {};
	Color32 color 		= {0.0f, 0.0f, 0.0f, 1.0f};

	TEXT_ALIGN_X_TYPE align_x = TEXT_ALIGN_X_LEFT;
	TEXT_ALIGN_Y_TYPE align_y = TEXT_ALIGN_Y_TOP;

	s32 content_width  = 0;
	s32 content_height = 0;

	bool is_editable      = false;
	bool use_word_wrap    = false;
	bool use_no_linebreak = false;
};

struct Text {
	ShaderSet *shader_set = 0;
	Font *font = 0;
	String s_data = {};

	bool allow_tab_input = false;

	/// always update with shader uniform
	float offset_x = 0.0f;
	float offset_y = 0.0f;

	Text_Data data;
	Text_Data data_prev;

	Text_Cursor cursor;

	Array<String>    as_words;
	Array<Text_Line> a_text_lines;
	Array<Vertex>    a_vertex_chars;
};

instant Text
Text_Create(
	ShaderSet *shader_set,
	Font *font,
	String *s_data_opt,
	Rect rect,
	TEXT_ALIGN_X_TYPE align_x
) {
	Assert(shader_set);
	Assert(font);

	Text text;

	text.shader_set = shader_set;
	text.font       = font;

	text.data.rect		= rect;
	text.data.align_x	= align_x;
	text.data.color		= {1, 1, 1, 1};

	if (s_data_opt)
		String_Append(&text.s_data, *s_data_opt);

	return text;
}

instant void
Text_Destroy(
	Text *text_out
) {
	Assert(text_out);

	String_Destroy(&text_out->s_data);
}

instant bool
Text_HasChanged(
	Text *text_io,
	bool update_changes
) {
	Assert(text_io);

	bool has_changed = text_io->s_data.changed;

	has_changed |= !Memory_Compare(&text_io->data, &text_io->data_prev, sizeof(text_io->data));

	if (has_changed AND update_changes) {
		text_io->data_prev = text_io->data;
		text_io->s_data.changed = false;
	}

	if (text_io->font)
		has_changed |= text_io->font->events.on_size_changed;

	return has_changed;
}

instant bool
Text_HasChanged(
	Array<String> *as_data
) {
	Assert(as_data);

	bool anything_changed = false;

	FOR_ARRAY(*as_data, it) {
		String *ts_data = &ARRAY_IT(*as_data, it);

		if (ts_data->changed) {
			anything_changed = true;
			break;
		}
	}

	return anything_changed;
}

/// returns number of lines
instant u64
Text_CalcLineCount(
	Text *text,
	Array<String> *as_words
) {
	Assert(text);
	Assert(as_words);

	u64 count_lines = 0;

	MEASURE_START();

	Font *font = text->font;
	Rect  rect = text->data.rect;

	Rect_AddPadding(&rect, text->data.rect_margin);
	Rect_AddPadding(&rect, text->data.rect_padding);

	Rect rect_line_current = {rect.x, rect.y, 0, 0};

	u64 advance_space = Codepoint_GetAdvance(font, ' ');
	u64 index_line = 0;
	bool line_start = true;

	if (as_words->count)
		++count_lines;

	u64 index_data = 0;

	FOR_ARRAY(*as_words, it_word) {
		String *ts_word = &ARRAY_IT(*as_words, it_word);

		u64 advance_word = Codepoint_GetStringAdvance(
								font,
								rect_line_current.x - rect.x,
								advance_space,
								ts_word
							);

		/// word wrap
		if (    text->data.use_word_wrap
			AND !line_start
			AND rect.w > 0
			AND (rect_line_current.x - rect.x) + advance_word > rect.w
			AND !text->data.use_no_linebreak
		) {
			++count_lines;
			line_start = true;

			rect_line_current.x  = rect.x;
		}

		index_data += ts_word->length;

		if (String_EndWith(ts_word, S("\n"), true)) {
			++count_lines;
			line_start = true;

			rect_line_current.x = rect.x;

			continue;
		}

		rect_line_current.x += advance_word;
		line_start = false;
	}

	MEASURE_END("");

	return count_lines;
}

/// returns height in pixel
instant s32
Text_BuildLines(
	Text *text,
	Array<String> *as_words,
	u64 number_of_linebreaks,
	Array<Text_Line> *a_text_line_out
) {
	///@Note: number_of_linebreaks can be 0 when resizing
	///       the text rect, since the words might have
	///       already been processed and will not calc.
	///       the number of static linebreaks again.
	///       it is also not nessesary here (to remember
	///       the amount of linebreaks), since it will
	///       just be used to reserve an array buffer, which
	///       would have already been adjusted when the words
	///       have been processed and resulted in a value in
	///       number_of_linebreaks which is >0.

	Assert(text);
	Assert(as_words);
 	Assert(a_text_line_out);

	Font *font = text->font;
	u64 advance_space = Codepoint_GetAdvance(font, ' ');

	Text_Line *text_line = 0;

 	s32 line_height    = Font_GetLineHeight(text->font);
	s32 current_height = 0;

	MEASURE_START();

	Array_ClearContainer(a_text_line_out);

	/// convert to number of fixed line-breaks
	if (!text->data.use_word_wrap) {
		Array_Reserve(a_text_line_out, number_of_linebreaks + 1);

		/// words will be in sequential order in memory!
		u64 index_data = 0;
		String *ts_data = &ARRAY_IT(*as_words, 0);

		if (as_words->count) {
			Array_AddEmpty(a_text_line_out, &text_line);
			text_line->s_data.value = &ts_data->value[index_data];

			current_height += line_height;
		}

		FOR_ARRAY(*as_words, it_word) {
			String *ts_word = &ARRAY_IT(*as_words, it_word);

			text_line->s_data.length += ts_word->length;

			u64 advance_word = Codepoint_GetStringAdvance(
									font,
									0,
									advance_space,
									ts_word
								);

			text_line->width_pixel += advance_word;

			index_data += ts_word->length;

			if (String_EndWith(ts_word, S("\n"), true)) {
				Array_AddEmpty(a_text_line_out, &text_line);
				text_line->s_data.value = &ts_data->value[index_data];

				/// height for the next (empty) line
				current_height += line_height;
			}
		}

		MEASURE_END("(static line-break) ");
	}
	else {
		/// do NOT calc. number of linebreaks (incl. word-wrap) and reserve
		/// the amount of space needed to fit into the array
		/// there is no benefit, since the amount of time needed to read all
		/// the codepoint data is greater than the reduction in speed by
		/// preallocating memory.
		///
		/// this works without word-wrap (in the code above) due to the lack
		/// of codepoint data retrieval

		Rect rect = text->data.rect;

		Rect_AddPadding(&rect, text->data.rect_padding);

		Rect rect_line_current = {rect.x, rect.y, 0, 0};

		s32 line_height = Font_GetLineHeight(font);
		u64 index_line = 0;
		bool line_start = true;

		Text_Line *text_line = 0;

		/// words will be in sequential order in memory!
		String *ts_data = &ARRAY_IT(*as_words, 0);

		if (as_words->count) {
			Array_AddEmpty(a_text_line_out, &text_line);
			current_height += line_height;
			text_line->s_data.value = ts_data->value;
		}

		u64 index_data = 0;

		FOR_ARRAY(*as_words, it_word) {
			String *ts_word = &ARRAY_IT(*as_words, it_word);

			u64 advance_word = Codepoint_GetStringAdvance(
									font,
									rect_line_current.x - rect.x,
									advance_space,
									ts_word
								);

			/// word wrap
			if (    text->data.use_word_wrap
				AND !line_start
				AND rect.w > 0
				AND (rect_line_current.x - rect.x) + advance_word > rect.w
				AND !text->data.use_no_linebreak
			) {
				Array_AddEmpty(a_text_line_out, &text_line);
				current_height += line_height;
				line_start = true;

				text_line->s_data.value = &ts_data->value[index_data];

				rect_line_current.x  = rect.x;
				rect_line_current.y += line_height;
			}

			text_line->s_data.length += ts_word->length;

			index_data += ts_word->length;

			if (String_EndWith(ts_word, S("\n"), true)) {
				Array_AddEmpty(a_text_line_out, &text_line);
				line_start = true;

				text_line->s_data.value = &ts_data->value[index_data];

				rect_line_current.x  = rect.x;
				rect_line_current.y += line_height;

				current_height += line_height;

				continue;
			}

			rect_line_current.x    += advance_word;
			text_line->width_pixel += advance_word;
			line_start = false;
		}

		MEASURE_END("(word-wrap) ");
	}

	return current_height;
}

///@Hint: line will NOT adjust, wenn word-wrap is disabled AND
///       the text-line is visibly longer, otherwise it would not
///       be possible to see the cursor at the beginning of such
///       a line, if text editing is allowed
instant u64
Text_GetAlignOffsetX(
	Text *text,
	u64 max_width,
	s32 advance_space,
	Text_Line *text_line
) {
	Assert(text);
	Assert(text_line);

	u64 x_align_offset = 0;

	if (    text->data.align_x != TEXT_ALIGN_X_LEFT
		AND text_line->width_pixel == 0
	) {
		text_line->width_pixel = Codepoint_GetStringAdvance(
									text->font,
									0.0f,
									advance_space,
									&text_line->s_data
								);
	}

	if (max_width > text_line->width_pixel) {
		if (text->data.align_x == TEXT_ALIGN_X_MIDDLE)
			x_align_offset = (max_width - text_line->width_pixel) >> 1;
		else
		if (text->data.align_x == TEXT_ALIGN_X_RIGHT)
			x_align_offset = (max_width - text_line->width_pixel);
	}

	return x_align_offset;
}

instant void
Text_ReserveMemory(
	Font             *font,
	Array<Vertex>    *a_vertex_chars_io,
	Array<Text_Line> *a_text_lines
) {
	Assert(font);
	Assert(a_text_lines);
	Assert(a_vertex_chars_io);

	FOR_ARRAY(*a_text_lines, it_line) {
		Text_Line *text_line = &ARRAY_IT(*a_text_lines, it_line);

		String s_data_it = S(text_line->s_data);

		while(!String_IsEmpty(&s_data_it)) {
			Codepoint codepoint;
			s32 utf_byte_count = 0;

			s32 cp = String_GetCodepoint(&s_data_it, &utf_byte_count);

 			Codepoint_GetDataConditional(
				font,
				cp,
				&codepoint,
				0,
				0
			);

			/// for unavailable characters like ' '
			if (!Texture_IsEmpty(&codepoint.texture) AND codepoint.codepoint > 32) {
				Vertex *t_vertex;
				Vertex_Buffer<float> *t_attribute;

				if (!Vertex_FindOrAdd(a_vertex_chars_io, &codepoint.texture, &t_vertex)) {
					Vertex_FindOrAddAttribute(t_vertex, 2, "vertex_position", &t_attribute);
					Vertex_FindOrAddAttribute(t_vertex, 3, "text_color", &t_attribute);
				}
				{
					t_attribute = &ARRAY_IT(t_vertex->a_attributes, 0);
					Assert(S("vertex_position") == t_attribute->name);

					t_attribute->group_count += 2;
				}

				{
					t_attribute = &ARRAY_IT(t_vertex->a_attributes, 1);
					Assert(S("text_color") == t_attribute->name);

					t_attribute->group_count += 3;
				}
			}

			String_AddOffset(&s_data_it, utf_byte_count);
		}
	}

	Vertex_Buffer<float> *t_attribute;

	FOR_ARRAY(*a_vertex_chars_io, it) {
		Vertex *t_vertex = &ARRAY_IT(*a_vertex_chars_io, it);
		t_attribute = &ARRAY_IT(t_vertex->a_attributes, 0);
		Array_Reserve(&t_attribute->a_buffer, t_attribute->group_count);
		t_attribute->group_count = 2;

		t_attribute = &ARRAY_IT(t_vertex->a_attributes, 1);
		Array_Reserve(&t_attribute->a_buffer, t_attribute->group_count);
		t_attribute->group_count = 3;
	}
}

instant void
Text_AddLines(
	Text *text,
	Array<Vertex>    *a_vertex_chars_io,
	Array<Text_Line> *a_text_lines
) {
	Assert(a_text_lines);

	Rect rect = text->data.rect;

	Rect_AddPadding(&rect, text->data.rect_margin);
	Rect_AddPadding(&rect, text->data.rect_padding);

	u64 width_max = rect.w;

	if (!width_max) {
		width_max = text->data.content_width;
	}

	float x_line_start = rect.x;

	RectF rect_position = {	x_line_start, rect.y, 0, 0 };

	bool has_cursor = text->data.is_editable;

	if (has_cursor)
		Vertex_ClearAttributes(&text->cursor.vertex_select);

	Codepoint codepoint_space;
	Codepoint_GetData(text->font, ' ', &codepoint_space);

	FOR_ARRAY(*a_text_lines, it_line) {
		Text_Line *text_line = &ARRAY_IT(*a_text_lines, it_line);

		String s_data_it = S(text_line->s_data);

		while(!String_IsEmpty(&s_data_it)) {
			Codepoint codepoint;
			s32 utf_byte_count = 0;

			s32 cp = String_GetCodepoint(&s_data_it, &utf_byte_count);

 			Codepoint_GetDataConditional(
				text->font,
				cp,
				&codepoint,
				rect_position.x - x_line_start,
				codepoint_space.advance
			);

			rect_position.x += codepoint.left_side_bearing;
			rect_position.y  =
					rect.y
				+ 	codepoint.rect_subpixel.y
				+ 	Font_GetLineHeight(text->font)
				+ 	codepoint.font->descent
			;

			u64 x_align_offset = Text_GetAlignOffsetX(text, width_max, codepoint_space.advance, text_line);

			/// for unavailable characters like ' '
			if (!Texture_IsEmpty(&codepoint.texture) AND codepoint.codepoint > 32) {
				Vertex *t_vertex;
				Vertex_Buffer<float> *t_attribute;

				if (!Vertex_FindOrAdd(a_vertex_chars_io, &codepoint.texture, &t_vertex)) {
					Vertex_FindOrAddAttribute(t_vertex, 2, "vertex_position", &t_attribute);
					Vertex_FindOrAddAttribute(t_vertex, 3, "text_color", &t_attribute);
				}
				{
					t_attribute = &ARRAY_IT(t_vertex->a_attributes, 0);
					Assert(S("vertex_position") == t_attribute->name);

					Array_ReserveAdd(&t_attribute->a_buffer, 2);
					Array_Add(&t_attribute->a_buffer, rect_position.x + x_align_offset);
					Array_Add(&t_attribute->a_buffer, rect_position.y);
				}

				{
					t_attribute = &ARRAY_IT(t_vertex->a_attributes, 1);
					Assert(S("text_color") == t_attribute->name);

					Array_ReserveAdd(&t_attribute->a_buffer, 3);
					Array_Add(&t_attribute->a_buffer, text->data.color.r);
					Array_Add(&t_attribute->a_buffer, text->data.color.g);
					Array_Add(&t_attribute->a_buffer, text->data.color.b);
				}
			}

			rect_position.x += codepoint.advance - codepoint.left_side_bearing;

			String_AddOffset(&s_data_it, utf_byte_count);
		}

		rect_position.x  = x_line_start;
		rect.y          += Font_GetLineHeight(text->font);
	}
}

instant void
Text_AddLines(
	Text *text_io
) {
	Assert(text_io);

	MEASURE_START();

	Text_AddLines(text_io, &text_io->a_vertex_chars, &text_io->a_text_lines);

	MEASURE_END("");
}

instant void
Text_Clear(
	Text *text_out
) {
	Assert(text_out);

	Vertex_ClearAttributes(&text_out->a_vertex_chars);
}

instant void
Text_RenderLines(
	Text *text
) {
	Assert(text);

	Vertex_Render(text->shader_set, &text->a_vertex_chars);
}

instant void Text_Cursor_Update(Text *text);

/// @Useful?
instant bool
Text_Exists(
	Text *text
) {
	Assert(text);

	return (!String_IsEmpty(&text->s_data));
}

instant bool
Text_Update(
	Text *text_io
) {
	Assert(text_io);

	if (!text_io->font) {
		if (!String_IsEmpty(&text_io->s_data))
			LOG_INFO("No font is set, while text is available.");

		return false;
	}

	if (!Text_HasChanged(text_io, false))
		return false;

	MEASURE_START();

	/// redraw text
	if (text_io->data.use_no_linebreak) {
		String_Replace(&text_io->s_data, S("\n"), S(""));
		String_Replace(&text_io->s_data, S("\r"), S(""));
	}

	u64 number_of_line_breaks = 0;

	if (text_io->s_data.changed)
		number_of_line_breaks = Array_SplitWordsBuffer(&text_io->s_data, &text_io->as_words);

	s32 text_height = Text_BuildLines(text_io, &text_io->as_words, number_of_line_breaks, &text_io->a_text_lines);

	if (text_io->data.rect.h) {
		s32 pad_height = text_io->data.rect_padding.y + text_io->data.rect_padding.h;

		if (text_io->data.align_y == TEXT_ALIGN_Y_CENTER)
			text_io->offset_y = (text_io->data.rect.h - pad_height - text_height) >> 1;
		else
		if (text_io->data.align_y == TEXT_ALIGN_Y_BOTTOM)
			text_io->offset_y = (text_io->data.rect.h - pad_height - text_height);
	}

	text_io->data.content_height = text_height;
	text_io->data.content_width = 0;

	FOR_ARRAY(text_io->a_text_lines, it_line) {
		Text_Line *t_line = &ARRAY_IT(text_io->a_text_lines, it_line);
		text_io->data.content_width = MAX(text_io->data.content_width, (s64)t_line->width_pixel);
	}

	u64 old_area = text_io->data_prev.content_width * text_io->data_prev.content_height;
	u64 new_area = text_io->data.content_width      * text_io->data.content_height;

	if (new_area > old_area) {
		/// because chunky memory allocation is the devil
		Text_ReserveMemory(text_io->font, &text_io->a_vertex_chars, &text_io->a_text_lines);
	}

	Text_Clear(text_io);
	Text_AddLines(text_io);

	Text_Cursor_Update(text_io);

	MEASURE_END("");

	text_io->data_prev = text_io->data;
	text_io->s_data.changed = false;

	return true;
}

instant u64
Text_Cursor_FindIndex(
	Text *text,
	Point point
) {
	Assert(text);

	u64 cursor_index = 0;
	bool found_index = false;

	Text_Cursor *cursor = &text->cursor;

	Rect rect = text->data.rect;

	Rect_AddPadding(&rect, text->data.rect_margin);
	Rect_AddPadding(&rect, text->data.rect_padding);

	Codepoint codepoint_space;
	Codepoint_GetData(text->font, ' ', &codepoint_space);

	u64 width_max = rect.w;

	if (!width_max) {
		FOR_ARRAY(text->a_text_lines, it_line) {
			Text_Line *t_text_line = &ARRAY_IT(text->a_text_lines, it_line);

			width_max = MAX(width_max, t_text_line->width_pixel);
		}
	}

	float x_pos_start = rect.x + text->offset_x;

	Rect rect_position_it = {
		x_pos_start,
		rect.y + text->offset_y,
		0,
		Font_GetLineHeight(text->font)
	};

	/// check the area above the text
	/// to select the first char
	if (point.y < rect_position_it.y) {
		return cursor_index;
	}

	FOR(text->a_text_lines.count, it_line) {
		Text_Line *text_line = &ARRAY_IT(text->a_text_lines, it_line);

		/// skip to the target line
		if (rect_position_it.y + rect_position_it.h <= point.y) {
			cursor_index += text_line->s_data.length;
			rect_position_it.y += rect_position_it.h;
			continue;
		}

		u64 x_align_offset = Text_GetAlignOffsetX(text, width_max, codepoint_space.advance, text_line);
		rect_position_it.x += x_align_offset;

		bool is_newline_char_once = false;

		String s_data_it = S(text_line->s_data);

		while(!String_IsEmpty(&s_data_it)) {
			Codepoint codepoint;
			s32 utf_byte_count = 0;

			s32 cp = String_GetCodepoint(&s_data_it, &utf_byte_count);

			Codepoint_GetDataConditional(
				text->font,
				cp,
				&codepoint,
				rect_position_it.x - x_pos_start,
				codepoint_space.advance
			);

			rect_position_it.w = codepoint.advance;

			bool is_newline_char = (cp == '\r' OR cp == '\n');

			if (is_newline_char)
				return cursor_index;

			if (point.x < rect_position_it.x + rect_position_it.w)
				return cursor_index;

			cursor_index += utf_byte_count;
			rect_position_it.x += rect_position_it.w;

			String_AddOffset(&s_data_it, utf_byte_count);
		}
	}

	return cursor_index;
}

instant void
Text_Cursor_Flush(
	Text *text_io
) {
	Assert(text_io);

	Text_Cursor *cursor = &text_io->cursor;

	cursor->move_index_x = 0;
	cursor->move_index_y = 0;

	if (!text_io->cursor.is_selecting)
		text_io->cursor.data.index_select_start = text_io->cursor.data.index_select_end;
}

instant void
Text_Cursor_FlushFull(
	Text *text_io
) {
	Assert(text_io);

	text_io->cursor.move_type = CURSOR_MOVE_NONE;
	Text_Cursor_Flush(text_io);
}

instant bool
Text_Cursor_HasChanged(
	Text_Cursor *cursor,
	bool update_changes
) {
	Assert(cursor);

	bool has_changed = !Memory_Compare(&cursor->data, &cursor->data_prev, sizeof(cursor->data));

	if (has_changed AND update_changes) {
		cursor->data_prev = cursor->data;
	}

	return has_changed;
}

instant void
Text_Cursor_Render(
	Text *text_io,
	Rect rect_cursor,
	s32 width_cursor
) {
	Text_Cursor *cursor = &text_io->cursor;

	rect_cursor.w = width_cursor;

	if (!cursor->vertex_cursor.array_id)
		cursor->vertex_cursor = Vertex_Create();

	Vertex_ClearAttributes(&cursor->vertex_cursor);

	Rect rect_no_offset = rect_cursor;
	rect_no_offset.x -= text_io->offset_x;
	rect_no_offset.y -= text_io->offset_y;

	Vertex_AddRect32(
		&cursor->vertex_cursor,
		rect_no_offset,
		cursor->data.color_cursor
	);
}

/// returns info if scrolling has happened
instant bool
Text_Cursor_Scroll(
	Text *text_io,
	Rect rect_cursor,
	s32 advance_space
) {
	Rect rect = text_io->data.rect;

	Text_Cursor *cursor = &text_io->cursor;

	float *x_offset = &text_io->offset_x;
	float *y_offset = &text_io->offset_y;

	Rect_AddPadding(&rect, text_io->data.rect_margin);
	Rect_AddPadding(&rect, text_io->data.rect_padding);

	/// exclude offsets
	float cursor_pos_x         = rect_cursor.x - *x_offset;
	float cursor_pos_y         = rect_cursor.y - *y_offset;

	bool is_past_right_border  = (rect_cursor.x + rect_cursor.w > rect.x + rect.w);
	bool is_past_left_border   = (rect_cursor.x < rect.x);

	bool is_past_top_border    = (rect_cursor.y < rect.y);
	bool is_past_bottom_border = (rect_cursor.y + rect_cursor.h > rect.y + rect.h);

	if (is_past_top_border OR is_past_bottom_border) {
		*x_offset = 0;

		if (is_past_top_border) {
			*y_offset = rect.y - cursor_pos_y;
		}
		else
		if (is_past_bottom_border) {
			*y_offset = (rect.y + rect.h) - (cursor_pos_y + rect_cursor.h);
		}

		Text_Cursor_Flush(text_io);
		return true;
	}

	if (is_past_right_border OR is_past_left_border) {
		/// skip to see which chars are coming in either direction
		u32 skip_space_mul_x = advance_space * 4;

		if (is_past_left_border) {
			*x_offset = rect.x - cursor_pos_x + skip_space_mul_x;

			if (*x_offset > 0)
				*x_offset = 0;
		}
		else
		if (is_past_right_border) {
			*x_offset = (rect.x + rect.w) - (cursor_pos_x + rect_cursor.w) - skip_space_mul_x;
		}

		Text_Cursor_Flush(text_io);
		return true;
	}

	return false;
}

/// returns true if data is invalid and need updating
instant bool
Text_Cursor_Move(
	Text *text_io,
	Rect rect_position,
	u64 index_cursor,
	u64 index_line_x,
	Text_Line *text_line,
	bool *found_cursor_start_out,
	bool *found_cursor_end_out
) {
	Assert(text_io);
	Assert(text_line);
	Assert(found_cursor_start_out);
	Assert(found_cursor_end_out);

	Text_Cursor *cursor = &text_io->cursor;

	switch (cursor->move_type) {
		case CURSOR_MOVE_NONE:
		case CURSOR_MOVE_X: {
			*found_cursor_end_out = true;
		} break;

		case CURSOR_MOVE_Y: {
			if (cursor->move_index_y == 0) {
				*found_cursor_end_out = true;
			}
			else
			if (cursor->move_index_y > 0) {
				Point pt_line = {
					rect_position.x,
					rect_position.y + Font_GetLineHeight(text_io->font)
				};

				if (!cursor->is_selecting)
					*found_cursor_start_out = false;

				cursor->data.index_select_end = Text_Cursor_FindIndex(text_io, pt_line);

				Text_Cursor_Flush(text_io);
			}
			else {
				Point pt_line = {
					rect_position.x,
					rect_position.y - Font_GetLineHeight(text_io->font)
				};

				cursor->data.index_select_end = Text_Cursor_FindIndex(text_io, pt_line);

				Text_Cursor_Flush(text_io);

				return true;
			}
		} break;

		case CURSOR_MOVE_LINE_BORDER: {
			if (cursor->move_index_x == 0) {
				*found_cursor_end_out = true;
			}
			else
			if (cursor->move_index_x > 0) {
				cursor->data.index_select_end -= index_line_x;
				cursor->data.index_select_end += text_line->s_data.length;

				if (cursor->data.index_select_end < text_io->s_data.length)
					cursor->data.index_select_end -= 1;

				if (String_EndWith(&text_line->s_data, S("\r\n"), true))
					cursor->data.index_select_end -= 1;

				if (cursor->data.index_select_end != index_cursor) {
					if (!cursor->is_selecting)
						*found_cursor_start_out = false;
				}
				else {
					*found_cursor_end_out = true;
				}

				Text_Cursor_Flush(text_io);
				cursor->move_type = CURSOR_MOVE_X;
			}
			else {
				cursor->data.index_select_end -= index_line_x;

				Text_Cursor_Flush(text_io);
				cursor->move_type = CURSOR_MOVE_X;

				return true;
			}
		} break;

		case CURSOR_MOVE_PAGE: {
			if (cursor->move_index_y == 0) {
				*found_cursor_end_out = true;
			}
			else
			if (cursor->move_index_y > 0) {
				Point pt_line = {
					rect_position.x,
					rect_position.y + text_io->data.rect.h
				};

				if (!cursor->is_selecting)
					*found_cursor_start_out = false;

				cursor->data.index_select_end = Text_Cursor_FindIndex(text_io, pt_line);

				Text_Cursor_Flush(text_io);
			}
			else {
				Point pt_line = {
					rect_position.x,
					rect_position.y - text_io->data.rect.h
				};

				cursor->data.index_select_end = Text_Cursor_FindIndex(text_io, pt_line);

				Text_Cursor_Flush(text_io);

				return true;
			}
		} break;
	}

	return false;
}

instant void
Text_Cursor_Update(
    Text *text_io
) {
	Assert(text_io);

	if (!text_io->data.is_editable)
		return;

	/// cursor selection -> end = current
	bool found_start    = false;
	bool found_end      = false;
	bool found_end_once = false;

	const s32 width_cursor = 2;

	u64 index_cursor = 0;
	u64 index_line_x = 0;

	Text_Cursor *cursor = &text_io->cursor;

	Rect rect = text_io->data.rect;

	Rect_AddPadding(&rect, text_io->data.rect_margin);
	Rect_AddPadding(&rect, text_io->data.rect_padding);

	Codepoint codepoint_space;
	Codepoint_GetData(text_io->font, ' ', &codepoint_space);

	u64 width_max = rect.w;

	if (!width_max) {
		width_max = text_io->data.content_width;

//		FOR_ARRAY(text_io->a_text_lines, it_line) {
//			Text_Line *t_text_line = &ARRAY_IT(text_io->a_text_lines, it_line);
//
//			width_max = MAX(width_max, t_text_line->width_pixel);
//		}
	}

	if (!cursor->vertex_select.array_id)
		cursor->vertex_select = Vertex_Create();

	Vertex_ClearAttributes(&cursor->vertex_select);

	float x_pos_start = rect.x + text_io->offset_x;

	Rect rect_position_it = {
		x_pos_start,
		rect.y + text_io->offset_y,
		0,
		Font_GetLineHeight(text_io->font)
	};

	/// lower boundary index check
	if (cursor->move_index_x < 0) {
		if (cursor->data.index_select_end + cursor->move_index_x > cursor->data.index_select_end) {
			cursor->move_index_x = 0;

			if (!cursor->is_selecting)
				cursor->data.index_select_start = 0;

			cursor->data.index_select_end = 0;
		}
	}

	if (cursor->move_type == CURSOR_MOVE_X) {
		if (!cursor->is_selecting)
			cursor->data.index_select_start += cursor->move_index_x;

		cursor->data.index_select_end += cursor->move_index_x;

		if (cursor->data.index_select_start > text_io->s_data.length)
			cursor->data.index_select_start = text_io->s_data.length;

		if (cursor->data.index_select_end > text_io->s_data.length)
			cursor->data.index_select_end = text_io->s_data.length;
	}

	Text_Line *text_line = 0;

	FOR(text_io->a_text_lines.count, it_line) {
		text_line = &ARRAY_IT(text_io->a_text_lines, it_line);

		if (    cursor->data.index_select_start > index_cursor + text_line->s_data.length
			AND cursor->data.index_select_end   > index_cursor + text_line->s_data.length
		) {
			index_cursor += text_line->s_data.length;
			rect_position_it.y += rect_position_it.h;
			continue;
		}

		u64 x_align_offset = Text_GetAlignOffsetX(text_io, width_max, codepoint_space.advance, text_line);
		rect_position_it.x += x_align_offset;

		bool is_newline_char_once = false;

		String s_data_it = S(text_line->s_data);

		index_line_x = 0;

		while(!String_IsEmpty(&s_data_it)) {
			Codepoint codepoint;
			s32 utf_byte_count = 0;

			s32 cp = String_GetCodepoint(&s_data_it, &utf_byte_count);

			Codepoint_GetDataConditional(
				text_io->font,
				cp,
				&codepoint,
				rect_position_it.x - x_pos_start,
				codepoint_space.advance
			);

			rect_position_it.w = codepoint.advance;

			bool is_newline_char = (cp == '\r' OR cp == '\n');

			if (cursor->data.index_select_start == index_cursor AND !found_start) {
				found_start = true;
			}

			if (cursor->data.index_select_end == index_cursor AND !found_end) {
				bool is_invalid = Text_Cursor_Move(
					text_io,
					rect_position_it,
					index_cursor,
					index_line_x,
					text_line,
					&found_start,
					&found_end
				);

				if (is_invalid)
					return Text_Cursor_Update(text_io);
			}

			if (text_io->cursor.is_selecting AND XOR(found_start, found_end)) {
				/// do not show multible ' ' in case of '\r\n'
				/// or selection would look strechted
				if (is_newline_char)
					rect_position_it.w = (is_newline_char_once
											? 0
											: codepoint_space.advance
										 );

				Rect rect_no_offset = rect_position_it;
				rect_no_offset.x -= text_io->offset_x;
				rect_no_offset.y -= text_io->offset_y;

				Vertex_AddRect32(
					&cursor->vertex_select,
					rect_no_offset,
					cursor->data.color_select
				);
			}

			if (found_end AND !found_end_once) {
				if (!is_newline_char_once) {
					/// update index, if you seek to '\r' and skip to '\n'
					cursor->data.index_select_end = index_cursor;

					Rect rect_cursor = rect_position_it;
					Text_Cursor_Render(text_io, rect_cursor, width_cursor);

					bool has_scrolled = Text_Cursor_Scroll(text_io, rect_cursor, codepoint_space.advance);

					if (has_scrolled) {
						return;
					}

					found_end_once = true;
				}
				else {
					/// to move to the start of the
					/// line-break when using '\r\n'
					if (cursor->move_index_x < 0) {
						Text_Cursor_Flush(text_io);

						cursor->move_type    = CURSOR_MOVE_X;
						cursor->move_index_x = -1;

						return Text_Cursor_Update(text_io);
					}

					/// skipping '\n' here when '\r\n'
					/// is present in the current line
				}
			}

			index_cursor += utf_byte_count;
			index_line_x += utf_byte_count;

			rect_position_it.x += rect_position_it.w;

			is_newline_char_once = is_newline_char;

			String_AddOffset(&s_data_it, utf_byte_count);
		}

		if (it_line + 1 < text_io->a_text_lines.count) {
			rect_position_it.x  = x_pos_start;
			rect_position_it.y += rect_position_it.h;
		}
	}

	if (!found_end_once) {
		cursor->data.index_select_end = index_cursor;

		if (text_line) {
			bool is_invalid = Text_Cursor_Move(
				text_io,
				rect_position_it,
				index_cursor,
				text_line->s_data.length,
				text_line,
				&found_start,
				&found_end
			);

			if (is_invalid)
				return Text_Cursor_Update(text_io);
		}

		Text_Cursor_Render(text_io, rect_position_it, width_cursor);
		Text_Cursor_Scroll(text_io, rect_position_it, codepoint_space.advance);
	}


 	Text_Cursor_FlushFull(text_io);
}

instant bool
Text_GetSelection(
	Text *text,
	String *s_data_out
) {
	Assert(text);
	Assert(s_data_out);

	String_Clear(s_data_out);

	Text_Cursor *cursor = &text->cursor;

	if (cursor->data.index_select_start > cursor->data.index_select_end)
		SWAP(u64, &cursor->data.index_select_start, &cursor->data.index_select_end);

	u64 length = cursor->data.index_select_end - cursor->data.index_select_start;

	/// nothing selected
	if (!length)
		return false;

	s_data_out->value   = text->s_data.value + cursor->data.index_select_start;
	s_data_out->length  = length;
	s_data_out->changed = true;

	return true;
}

instant bool
Text_RemoveSelection(
	Text *text_io
) {
	Text_Cursor *cursor = &text_io->cursor;

	bool was_selection_removed = false;

	/// overwrite selected text
	if (cursor->data.index_select_start != cursor->data.index_select_end) {
		String_Remove(
			&text_io->s_data,
			cursor->data.index_select_start,
			cursor->data.index_select_end
		);

		/// cursor and selection bounds will start at the
		/// beginning of the selection
		if (cursor->data.index_select_end > cursor->data.index_select_start)
			cursor->data.index_select_end   = cursor->data.index_select_start;
		else
			cursor->data.index_select_start = cursor->data.index_select_end;

		was_selection_removed = true;
	}

	return was_selection_removed;
}

instant void
Text_UpdateInput(
    Text *text_io,
    Keyboard *keyboard,
    bool *text_changed_out   = 0,
    bool *cursor_changed_out = 0
) {
	Assert(text_io);
	Assert(keyboard);

	IF_SET(text_changed_out)   = false;

	///@TODO: use (prev_)data to retrieve this info
	IF_SET(cursor_changed_out) = false;

	if (!text_io->data.is_editable)
		return;

	s8 offset_index_x = 0;
	s8 offset_index_y = 0;

	/// cursor direction
	/// -----------------------------------------------------------------------
	CURSOR_MOVE_TYPE *move_type = &text_io->cursor.move_type;

	Text_Cursor *cursor = &text_io->cursor;

	*move_type = CURSOR_MOVE_NONE;

	if (keyboard->down[VK_LEFT])  { offset_index_x = -1;
									*move_type = CURSOR_MOVE_X; }
	if (keyboard->down[VK_RIGHT]) { offset_index_x =  1;
									*move_type = CURSOR_MOVE_X; }

	if (keyboard->down[VK_UP])    { offset_index_y = -1;
									*move_type = CURSOR_MOVE_Y; }
	if (keyboard->down[VK_DOWN])  { offset_index_y =  1;
									*move_type = CURSOR_MOVE_Y; }

	if (keyboard->down[VK_HOME])  { offset_index_x = -1;
									*move_type = CURSOR_MOVE_LINE_BORDER; }
	if (keyboard->down[VK_END])   { offset_index_x =  1;
									*move_type = CURSOR_MOVE_LINE_BORDER; }

	if (keyboard->down[VK_PRIOR]) { offset_index_y = -1;
									*move_type = CURSOR_MOVE_PAGE; }
	if (keyboard->down[VK_NEXT])  { offset_index_y =  1;
									*move_type = CURSOR_MOVE_PAGE; }

	cursor->is_selecting = (keyboard->pressing[VK_SHIFT] AND !keyboard->is_key_sym);

	bool was_selecting = (cursor->data.index_select_start != cursor->data.index_select_end);

	/// cursor end selection positioning
	/// -----------------------------------------------------------------------
	/// move the cursor to the start or end of the selection,
	/// when selection ended and the cursor was moved
	if (was_selecting AND !cursor->is_selecting AND offset_index_x != 0) {
		if (offset_index_x < 0)
			cursor->data.index_select_start = MIN(cursor->data.index_select_start, cursor->data.index_select_end);
		else
			cursor->data.index_select_start = MAX(cursor->data.index_select_start, cursor->data.index_select_end);

		cursor->data.index_select_end = cursor->data.index_select_start;

		offset_index_x = 0;

		Text_Cursor_Update(text_io);
	}

	/// character block selection
	/// -----------------------------------------------------------------------
	if (offset_index_x != 0 OR offset_index_y != 0) {
		if (!cursor->is_selecting)
			cursor->data.index_select_start = cursor->data.index_select_end;

		s32 utf_byte_count = 0;

		u64 cursor_index = cursor->data.index_select_end;

		if (offset_index_x < 0 AND cursor_index)
			--cursor_index;

		s32 codepoint = String_GetCodepointAtIndex(&text_io->s_data, cursor_index, &utf_byte_count);

		if (    codepoint > 0
			AND keyboard->pressing[VK_CONTROL]
			AND offset_index_x != 0
		) {
            if (offset_index_x > 0) {
				if (cursor_index < text_io->s_data.length) {
					CODEPOINT_TYPE cp_type = Codepoint_GetType(codepoint);

					while(    cp_type == Codepoint_GetType(codepoint)
						  AND cursor_index < text_io->s_data.length
					) {
						cursor_index += utf_byte_count;
						codepoint = String_GetCodepointAtIndex(&text_io->s_data, cursor_index, &utf_byte_count);
						Assert(codepoint >= 0);
					}

					if (cursor_index > 0)
						cursor->data.index_select_end = cursor_index - 1;
				}
            }
            else {
				if (cursor_index) {
					s32 codepoint = String_GetCodepointAtIndex(&text_io->s_data, cursor_index - 1, &utf_byte_count);
					Assert(codepoint >= 0);

					CODEPOINT_TYPE cp_type = Codepoint_GetType(codepoint);

					do {
						codepoint = String_GetCodepointAtIndex(&text_io->s_data, --cursor_index - 1, &utf_byte_count);
						Assert(codepoint >= 0);
					} while(
							cp_type == Codepoint_GetType(codepoint)
						AND cursor_index > 0
					);

					cursor->data.index_select_end = cursor_index + 1;
				}
            }
		}

		cursor->move_index_x += (offset_index_x * utf_byte_count);
		cursor->move_index_y += offset_index_y;

		IF_SET(cursor_changed_out) = true;

		Text_Cursor_Update(text_io);

		return;
	}

	/// keyboard input handling
	/// -----------------------------------------------------------------------
	if (keyboard->is_key_sym AND keyboard->is_down) {
		String s_selection;

		char key = LOWORD(keyboard->key_sym);

		bool key_is_linebreak = (key == '\n' OR key == '\r');

		if (text_io->data.use_no_linebreak AND key_is_linebreak)
			return;

		switch (key) {
			/// control + a
			/// - select all text
			case 1: {
				cursor->data.index_select_start = 0;
				cursor->data.index_select_end = text_io->s_data.length;
				cursor->is_selecting = true;

				IF_SET(cursor_changed_out) = true;

				Text_Cursor_Update(text_io);
			} break;

			/// control + c
			/// - copy to clipboard text
			case 3: {
				if (Text_GetSelection(text_io, &s_selection)) {
					Clipboard_SetText(&s_selection);
				}
			} break;

			/// control + v
			/// - insert clipboard text
			case 22: {
				*move_type = CURSOR_MOVE_X;
				bool was_selection_removed = Text_RemoveSelection(text_io);

				String s_clipboard = Clipboard_GetText();

				cursor->move_index_x = 	String_Insert(
											&text_io->s_data,
											s_clipboard,
											cursor->data.index_select_end
										);

				String_Destroy(&s_clipboard);

				IF_SET(cursor_changed_out) = true;

				Text_Cursor_Update(text_io);
			} break;

			/// control + x
			/// - copy to clipboard and remove selection
			case 24: {
				if (Text_GetSelection(text_io, &s_selection)) {
					Clipboard_SetText(&s_selection);

					Text_RemoveSelection(text_io);
					Text_Cursor_Update(text_io);

					Assert(text_io->s_data.changed);
				}
			} break;

			default: {
				*move_type = CURSOR_MOVE_X;
				bool was_selection_removed = Text_RemoveSelection(text_io);

				bool is_char_valid = (      key != '\t'
                                        OR (key == '\t' AND text_io->allow_tab_input));

				bool can_insert_char = (   !was_selection_removed
										OR (was_selection_removed AND key != '\b'));

				if (is_char_valid AND can_insert_char) {
					cursor->move_index_x = String_Insert(
												&text_io->s_data,
												key,
												cursor->data.index_select_end
											);

					IF_SET(cursor_changed_out) = true;

					Text_Update(text_io);

					/// to trigger on_text_changed event,
					/// since this would be set to false
					/// with text_update
					text_io->s_data.changed = true;
				}
			}
		}
	}

	if (keyboard->is_down AND keyboard->down[VK_DELETE]) {
		bool was_selection_removed = Text_RemoveSelection(text_io);

		if (!was_selection_removed) {
			if (    text_io->s_data.length
				AND cursor->data.index_select_end + 1 <= text_io->s_data.length) {

				String_Insert(
					&text_io->s_data,
					'\b',
					cursor->data.index_select_end + 1
				);
			}
		}
	}

	/// string could have been appended, removed or cleared
	IF_SET(text_changed_out) = text_io->s_data.changed;
}

instant void
Text_Render(
	Text *text_io
) {
	Assert(text_io);

	bool is_fixed_size = (text_io->data.rect.w OR text_io->data.rect.h);

	if (is_fixed_size)
		OpenGL_Scissor(text_io->shader_set->window, text_io->data.rect);

	/// DEBUG render background rect
	ShaderSet_Use(text_io->shader_set, SHADER_PROG_RECT);

#if DEBUG_TEXT_DRAW_BACKGROUND
	Vertex v_bgn = Vertex_Create();
	Vertex_AddRect32(&v_bgn, text_io->data.rect, {0, 0, 1, 1});
	Rect_Render(text_io->shader_set, &v_bgn);
	Vertex_Destroy(&v_bgn);
#endif // DEBUG_TEXT_DRAW_BACKGROUND

	/// redraw selection
	if (text_io->data.is_editable AND text_io->cursor.vertex_select.a_attributes.count) {
		ShaderSet_Use(text_io->shader_set, SHADER_PROG_RECT);
		Shader_SetValue(text_io->shader_set, "x_offset", text_io->offset_x);
		Shader_SetValue(text_io->shader_set, "y_offset", text_io->offset_y);

		Rect_Render(text_io->shader_set, &text_io->cursor.vertex_select);
	}

	/// redraw cursor
	if (    text_io->cursor.show_cursor
		AND text_io->data.is_editable
		AND text_io->cursor.vertex_cursor.a_attributes.count
	) {
		if (Time_HasElapsed(&text_io->cursor.timer_blinking, text_io->cursor.blink_inverval_ms)) {
			text_io->cursor.is_blink_on = !text_io->cursor.is_blink_on;
		}

		if (text_io->cursor.is_blink_on) {
			AssertMessage(!Vertex_IsEmpty(&text_io->cursor.vertex_cursor), "[Text] Cursor vertex data does not exists.");

			ShaderSet_Use(text_io->shader_set, SHADER_PROG_RECT);
			Shader_SetValue(text_io->shader_set, "x_offset", text_io->offset_x);
			Shader_SetValue(text_io->shader_set, "y_offset", text_io->offset_y);

			Rect_Render(text_io->shader_set, &text_io->cursor.vertex_cursor);
		}
	}

	if (text_io->a_vertex_chars.count) {
		/// redraw last computed text
		ShaderSet_Use(text_io->shader_set, SHADER_PROG_TEXT);
		Shader_SetValue(text_io->shader_set, "x_offset", text_io->offset_x);
		Shader_SetValue(text_io->shader_set, "y_offset", text_io->offset_y);

		Vertex_Render(text_io->shader_set, &text_io->a_vertex_chars);
	}

	if (is_fixed_size)
		OpenGL_Scissor_Disable();
}

instant void
Text_GetSize(
	Text *text,
	s32 *width_out,
	s32 *height_out
) {
	Assert(text);

	/// seperate storage, so independent text container keep
	/// their already processed data
	static Array<String>    as_words;
	static Array<Text_Line> a_text_lines;

	if (height_out) {
		u64 number_of_lines = Array_SplitWordsBuffer(&text->s_data, &as_words);
		*height_out = Text_BuildLines(text, &as_words, number_of_lines, &a_text_lines);
	}

	IF_SET(width_out)  = text->data.rect.w;
}

instant void
Text_ScrollVerticalEnd(
	Text *text_io
) {
	Assert(text_io);

	Text_Update(text_io);

	s32 rect_height    = text_io->data.rect.h;
	s32 content_height = text_io->data.content_height;

	text_io->offset_x = 0;

	if (content_height > rect_height)
		text_io->offset_y = -(content_height - rect_height);
	else
		text_io->offset_y = 0;
}

instant void
Text_GetString(
	Text *text,
	String **s_data
) {
	Assert(text);
	Assert(s_data);

	*s_data = &text->s_data;
}

instant void
Text_UseWordWrap(
	Text *text,
	bool enable
) {
	Assert(text);

	text->data.use_word_wrap = enable;
}
