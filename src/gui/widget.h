#pragma once

struct Widget;

struct Widget_Item {
    String s_name;
    Color32 colorText;
};

typedef void (*Widget_OwnerDraw)
	(Widget *widget_io);

typedef void (*Widget_UpdateCustomInputsSub)
	(Widget *widget_parent_io, u64 sub_index);

typedef void (*Widget_DrawItem)
	(Widget *widget_io, Widget_Item *item);

instant bool
Widget_Update(Widget *widget_io);

enum WIDGET_TYPE {
	WIDGET_LABEL,
	WIDGET_BUTTON,
	WIDGET_LISTBOX,
	WIDGET_CHECKBOX,
	WIDGET_PICTUREBOX,
	WIDGET_SPREADER,
	WIDGET_NUMBERPICKER,
	WIDGET_TEXTBOX,
	WIDGET_COMBOBOX,
	WIDGET_PROGRESSBAR,
	WIDGET_LISTVIEW
};

enum WIDGET_COMBOBOX_TYPE {
	WIDGET_COMBOBOX_TEXT = 0,
	WIDGET_COMBOBOX_TOGGLE,
	WIDGET_COMBOBOX_LIST
};

enum WIDGET_SCROLL_TYPE {
	WIDGET_SCROLL_ITEM,
	WIDGET_SCROLL_BLOCK
};

enum WIDGET_SELECT_TYPE {
	WIDGET_SELECT_ON_RETURN,
	WIDGET_SELECT_ON_INDEX_CHANGE
};

struct Widget_Slide {
	s64 start = 0;
	s64 end   = 0;
	s64 value = 0;
	s64 step  = 0;
};

struct Widget_Column {
	String s_name;
	s32 width;
	s32 height;
	s32 spacing = 5;
	bool is_dragging = false;
};

struct Widget;

struct Widget_Data {
	Color32 color_background       = Color_MakeGrey(0.9f);
	Color32 color_outline          = {0.8f, 0.8f, 0.8f, 1.0f};
	Color32 color_outline_selected = {1.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_outline_inactive = {0.5f, 0.5f, 1.0f, 1.0f};
	Color32 color_outline_checked  = {0.5f, 0.5f, 1.0f, 1.0f};
	Color32 color_progress         = {0.5f, 0.5f, 1.0f, 1.0f};

	u64  selected_row_id = 0;

	u32  border_size = 0;
	u32  spacing     = 1;

	bool is_visible  = true;
	bool is_dragging = false;
	bool is_filter_case_sensitive = true;
	bool can_popout_focus_change  = false;

	/// overlay
	bool is_floating = false; /// is overlay
	bool is_popout   = false; /// visible overlay

	bool has_focus  = false;
	bool is_checked = false;

	WIDGET_SCROLL_TYPE scroll_type = WIDGET_SCROLL_ITEM;

	/// ListBox Data
	Array<String> as_row_data;

	/// ListBox filter
	String s_row_filter;
	Array<String> as_filter_data;

	/// ListView Data
	bool is_header_visible = true;
	Array<Widget_Column>  a_table_columns;
	Array<Array<String>> *a_table_data = 0;

	/// Radiogroup link
	Array<Widget *> *ap_radiogroup = 0;
};

struct Widget {
	Window *window = 0;

	/// Defining Properties
	WIDGET_TYPE type;
	Widget_Data data;
	Widget_Data data_prev;
	Widget_Slide slide;

	/// type specific
	bool is_focusable  = true;
	bool is_scrollable = false;
	bool is_checkable  = false;
	bool has_scrollable_list = false;

	/// On Demand
	static Widget *widget_focus_current;
	Widget *widget_focus_on_popout = 0;
	WIDGET_SELECT_TYPE type_select = WIDGET_SELECT_ON_RETURN;

	/// Triggers (per Frame)
	struct Widget_Trigger {
		bool autosize = false;
		bool popout   = false;
		bool update   = false;
	} trigger;

	/// Events (per Frame)
	struct Widget_Events {
		bool on_list_change_index = false;
		bool on_list_change_final = false;

		bool on_text_changed = false;

		bool on_trigger = false;
		bool on_trigger_secondary = false;

		bool on_updated_input = false;
	} events;

	/// Custom Events
	Widget_OwnerDraw OwnerDraw = 0;
	Widget_UpdateCustomInputsSub UpdateCustomInputs = 0;
	Widget_DrawItem OnDrawItem = 0;

	/// Rendering
	Text   text;
	Vertex vertex_rect;
	Vertex vertex_rect_sublayer;	/// f.e. hover, check-status (color changes)
	Array<Vertex> a_vertex_fans;	/// f.e. circles

	/// Layout / Size
	Layout_Data layout_data;
	Rect rect_content; /// x,y = offsets

	/// Content
	Array<Widget> a_subwidgets;

	Array<void *> a_dataLink;
};

Widget *Widget::widget_focus_current = 0;

instant void
Widget_SetFilterCaseSensitive(
    Widget &widget,
    bool value
) {
    widget.data.is_filter_case_sensitive = value;
}

instant void
Array_Destroy(
	Array<Widget_Column> *a_columns
) {
	Assert(a_columns);

	FOR_ARRAY(*a_columns, it) {
		Widget_Column *t_column = &ARRAY_IT(*a_columns, it);
		String_Destroy(t_column->s_name);
	}

	Array_DestroyContainer(*a_columns);
}

instant void
Layout_Block_SetVisible (
	Layout *layout,
	u64 index_block,
	bool set_visible
) {
	Assert(layout);

	Layout_Block *layout_block = 0;
	bool got_block = Layout_GetBlock(layout, index_block, &layout_block);
	Assert(got_block);

	layout_block->is_visible = set_visible;

	FOR_ARRAY(layout_block->ap_widgets, it) {
		Widget *widget = ARRAY_IT(layout_block->ap_widgets, it);
		widget->data.is_visible = set_visible;
	}
}

instant Rect *
Widget_GetRectRef(
	Widget *widget
) {
	Assert(widget);

	return &widget->layout_data.settings.rect;
}

instant Widget *
Widget_GetSubWidget(
	Widget *widget,
	u64 index
) {
	Assert(widget);

	if (!widget->a_subwidgets.count) {
		LOG_INFO("Subwidget does not exists. Default back to main widget.");
		return widget;
	}

	if (index >= widget->a_subwidgets.count) {
		LOG_INFO("Subwidget index out of bounds. Default back to main widget.");
		return widget;
	}

	return &ARRAY_IT(widget->a_subwidgets, index);
}

instant void
Widget_Cursor_ResetBlinking(
	Widget *widget_io
) {
	Assert(widget_io);

	widget_io->text.cursor.show_cursor = widget_io->data.has_focus;
	Time_Reset(widget_io->text.cursor.timer_blinking);
	widget_io->text.cursor.is_blink_on = true;
}

instant bool
Widget_IsListType(
	Widget *widget
) {
	Assert(widget);

	if (widget->type == WIDGET_LISTBOX)   return true;
	if (widget->type == WIDGET_LISTVIEW)  return true;

	return false;
}

instant void
Widget_AddRowSingle(
	Widget *widget_io,
	const String &s_row_data
) {
	/// list contained in subwidgets
	switch (widget_io->type) {
		case WIDGET_COMBOBOX: {
			widget_io = &ARRAY_IT(	widget_io->a_subwidgets,
									WIDGET_COMBOBOX_LIST);
		} break;

		default: {} break;
	}

	String *ts_data;
	Array_AddEmpty(widget_io->data.as_row_data, &ts_data);
	*ts_data = String_Copy(s_row_data.value, s_row_data.length);
}

instant void
Widget_AddRowsSingle(
	Widget *widget_io,
	Array<String> *as_list
) {
	Assert(widget_io);
	Assert(as_list);

	FOR_ARRAY(*as_list, it) {
		String *ts_item = &ARRAY_IT(*as_list, it);

		Widget_AddRowSingle(widget_io, *ts_item);
	}
}

instant bool
Mouse_IsHovering(
	Widget *widget,
	Mouse *mouse_out = 0
) {
	Assert(widget);

	Point t_point;
	bool is_hovering_popout = false;

	if (mouse_out) {
		Mouse_GetPosition(mouse_out, widget->window);
		t_point = mouse_out->point;
	}
	else {
		Mouse_GetPosition(&t_point.x, &t_point.y, widget->window);
	}

	/// overlay check
	if (    widget != widget->widget_focus_current
		AND widget->widget_focus_current
		AND widget->widget_focus_current->data.is_floating
	) {
		if (widget->widget_focus_current->data.is_popout) {
			is_hovering_popout = Mouse_IsHovering(widget->widget_focus_current, mouse_out);
		}
	}

	if (is_hovering_popout) {
		return false;
	}

    return Rect_IsIntersecting(&t_point, &widget->layout_data.settings.rect);
}

/// @Todo: check and update recursively
instant bool
Widget_HasChanged(
	Widget *widget_io,
	bool update_changes
) {
	Assert(widget_io);

	bool has_changed = widget_io->trigger.update;

	/// a list would iterate over the list items and reuse "text"
	/// for every list element incl. might change (some) of its settings,
	/// that is why the list data change checking will happen seperately
	if (!has_changed AND !Widget_IsListType(widget_io)) {
		///@Note: updating text changed status will happen
		///       in a Text_Update
		has_changed |= Text_HasChanged(&widget_io->text, false);
	}

	if (!has_changed AND widget_io->text.font)
		has_changed |= (widget_io->text.font->events.flags != 0);

	if (!has_changed) {
		/// check list entries
		FOR_ARRAY(widget_io->data.as_row_data, it) {
			String *t_data = &ARRAY_IT(widget_io->data.as_row_data, it);

			has_changed |= String_HasChanged(*t_data, update_changes);
		}
	}

	if (!has_changed) {
		has_changed |= !Memory_Compare(
							&widget_io->layout_data.settings,
							&widget_io->layout_data.settings_prev,
							 sizeof(widget_io->layout_data.settings)
					  );
	}

	if (!has_changed) {
		has_changed |= !Memory_Compare(
							&widget_io->data,
							&widget_io->data_prev,
							 sizeof(widget_io->data)
					  );
	}

	if (update_changes) {
		widget_io->layout_data.settings_prev = widget_io->layout_data.settings;
		widget_io->data_prev = widget_io->data;
	}

/// @NOTE: do NOT check subwidgets, since they will be added to the render list anyway
///        and might mess up the update checking

	return has_changed;
}

instant void
Widget_AddBorderSizes(
	Widget *widget,
	s32 *min_width_io,
	s32 *min_height_io
) {
	Assert(widget);

	Rect *rect_padding = &widget->text.data.rect_padding;

	if (min_width_io) {
		*min_width_io += rect_padding->x + rect_padding->w;

		/// border size is used for the checkbox,
		/// not the border of the widget itself
		if (widget->type != WIDGET_CHECKBOX)
			*min_width_io += widget->data.border_size << 1;

		*min_width_io += widget->text.data.rect_margin.x
					  +  widget->text.data.rect_margin.w;
	}

	if (min_height_io) {
		*min_height_io += rect_padding->y + rect_padding->h;

		/// border size is used for the checbkox,
		/// not the border of the widget itself
		if (widget->type != WIDGET_CHECKBOX)
			*min_height_io += widget->data.border_size << 1;

		*min_height_io += widget->text.data.rect_margin.y
					   +  widget->text.data.rect_margin.h;
	}
}

/// @idea: might need a subindex to set focus to widgets,
///        which are embedded into a widget
instant void
Widget_SetFocus(
	Widget &widget
) {
	if (!widget.is_focusable)
		return;

    if (widget.widget_focus_current) {
    	widget.widget_focus_current->data.has_focus = false;
    }

    widget.data.has_focus = true;
    widget.widget_focus_current = &widget;

    if (widget.data.is_floating)
		widget.data.is_popout = true;
}

instant void
Widget_Redraw(
	Widget *widget_io
) {
	Assert(widget_io);

	if (widget_io->OwnerDraw) {
		widget_io->OwnerDraw(widget_io);
		return;
	}

	Rect rect_box = *Widget_GetRectRef(widget_io);

	/// rect sublayer init
	/// -----------------------------------------------------------------------
	Vertex *t_vertex_static = &widget_io->vertex_rect_sublayer;

	if (!t_vertex_static->array_id)
		Vertex_Create(t_vertex_static, VERTEX_RECT);
	else
		Vertex_ClearAttributes(t_vertex_static);

	/// dynamic rect init
	/// -----------------------------------------------------------------------
	Vertex *t_vertex_dynamic = &widget_io->vertex_rect;

	if (!t_vertex_dynamic->array_id)
		Vertex_Create(t_vertex_dynamic, VERTEX_RECT);
	else
		Vertex_ClearAttributes(t_vertex_dynamic);
	/// -----------------------------------------------------------------------

	switch (widget_io->type) {
		case WIDGET_SPREADER: {
		} break;

		case WIDGET_TEXTBOX: {
			widget_io->text.data.rect = *Widget_GetRectRef(widget_io);

			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);

			Widget_Cursor_ResetBlinking(widget_io);
		} break;

		case WIDGET_COMBOBOX: {
			Rect *rect_layout = Widget_GetRectRef(widget_io);

			rect_layout->h = 0;

			/// calc. max. visible widget height to be used for the layout system,
			/// in order to get proper horizontal alignment
            FOR_ARRAY(widget_io->a_subwidgets, it) {
				Widget *t_widget = &ARRAY_IT(widget_io->a_subwidgets, it);

                if (!t_widget->data.is_floating) {
					rect_layout->h = MAX(rect_layout->h, t_widget->layout_data.settings.rect.h);
                }
            }

			Layout layout;
			Layout_Create(&layout, &widget_io->layout_data, false);
			layout.padding = 0;

			{
				u64 index_block = 0;
				Layout_Block *t_block = 0;

				index_block = Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0);

				if (!Layout_GetBlock(&layout, index_block, &t_block))
					Assert(false);

				t_block->padding = 0;

				Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, WIDGET_COMBOBOX_TEXT));
				Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, WIDGET_COMBOBOX_TOGGLE));

				index_block = Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0);

				if (!Layout_GetBlock(&layout, index_block, &t_block))
					Assert(false);

				t_block->padding = 0;

				Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, WIDGET_COMBOBOX_LIST));
			}

			Layout_Arrange(&layout);
		} break;

		case WIDGET_LABEL: {
			widget_io->text.data.rect = *Widget_GetRectRef(widget_io);
			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);
		} break;

		case WIDGET_PICTUREBOX:
		case WIDGET_LISTBOX:
		case WIDGET_LISTVIEW: {
			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);
		} break;

		case WIDGET_BUTTON: {
			widget_io->text.data.rect = *Widget_GetRectRef(widget_io);

			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);

			if (widget_io->data.border_size) {
				Rect_Resize(&rect_box, -1);

				if (widget_io->data.has_focus)
					Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_outline_selected);
				else
					Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_outline_inactive);

				Rect_Resize(&rect_box, -widget_io->data.border_size);
				Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);
			}
		} break;

		case WIDGET_CHECKBOX: {
			widget_io->text.data.rect = *Widget_GetRectRef(widget_io);

			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);

			s32 check_offset = 2;
			s32 check_h = widget_io->text.font->size - (check_offset << 1);
			s32 check_w = check_h;

			widget_io->text.data.rect_margin = {
				(float)check_offset * 2 + check_w + 2,
				0,
				0,
				0
			};

			float radius = (check_w / 2.0f);

			Rect rect_check = {
				rect_box.x + check_offset + widget_io->text.data.rect_padding.x,
				rect_box.y + check_offset + widget_io->text.data.rect_padding.y,
				check_w,
				check_h
			};


			/// draw radiobuttons when radiogroup is used
			if (widget_io->data.ap_radiogroup) {
				Point pt_center = {
					rect_check.x + radius,
					rect_check.y + radius,
				};

				Array_Destroy(&widget_io->a_vertex_fans);

				Vertex *vertex_new;
				Array_AddEmpty(widget_io->a_vertex_fans, &vertex_new);

				if (widget_io->data.has_focus) {
					Vertex_CreateCircleBuffer(vertex_new,
											  pt_center,
											  radius,
											  widget_io->data.color_outline_selected);
				}
				else {
					Vertex_CreateCircleBuffer(vertex_new,
											  pt_center,
											  radius,
											  widget_io->data.color_outline_inactive);
				}

				widget_io->data.border_size = (widget_io->text.font->size / 10);

				Array_AddEmpty(widget_io->a_vertex_fans, &vertex_new);

				Vertex_CreateCircleBuffer(vertex_new,
										  pt_center,
										  radius - widget_io->data.border_size,
										  widget_io->data.color_background);

				if (widget_io->data.is_checked) {
					Array_AddEmpty(widget_io->a_vertex_fans, &vertex_new);

					Vertex_CreateCircleBuffer(vertex_new,
										  pt_center,
										  radius - widget_io->data.border_size - 1,
										  widget_io->data.color_outline_checked);
				}
			}
			/// draw checkboxes when radiogroup is not used
			else {
				if (widget_io->data.has_focus) {
					Vertex_AddRect32(t_vertex_static,
									 rect_check,
									 widget_io->data.color_outline_selected);
				}
				else {
					Vertex_AddRect32(t_vertex_static,
									 rect_check,
									 widget_io->data.color_outline_inactive);

				}

				widget_io->data.border_size = (widget_io->text.font->size / 10);

				Rect_Resize(&rect_check, -widget_io->data.border_size);

				Vertex_AddRect32(t_vertex_static,
								 rect_check,
								 widget_io->data.color_background);

				if (widget_io->data.is_checked) {
					Rect_Resize(&rect_check, -1);

					Vertex_AddRect32(t_vertex_static,
									 rect_check,
									 widget_io->data.color_outline_checked);
				}
			}
		} break;

		case WIDGET_NUMBERPICKER: {
			Layout layout;
			Layout_Create(&layout, &widget_io->layout_data, false);
			layout.padding = 0;

			Layout_Block *t_block = 0;
			u64 index_block = Layout_CreateBlock(&layout, LAYOUT_TYPE_X, LAYOUT_DOCK_TOPLEFT, 0);

			if (!Layout_GetBlock(&layout, index_block, &t_block))
				Assert(false);

			t_block->padding = 0;
			t_block->spacing = 2;

			/// label
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 0));
			/// spreader
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 3));
			/// button up
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 1));
			/// button down
			Layout_Add(&layout, &ARRAY_IT(widget_io->a_subwidgets, 2));
			Layout_Arrange(&layout);
		} break;

		case WIDGET_PROGRESSBAR: {
			widget_io->text.data.rect = *Widget_GetRectRef(widget_io);

			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);

			Rect rect_progress = rect_box;
			Widget_Slide *slide = &widget_io->slide;

			float percent = (float)slide->value / (slide->end - slide->start);

			rect_progress.w *= percent;

			Vertex_AddRect32(t_vertex_static, rect_progress, widget_io->data.color_progress);
		} break;

		default:
			AssertMessage(	false,
							"[Widget] Redrawing: Unhandled widget background drawing.");
	}
}

instant void
Widget_InvalidateBackground(
	Widget *widget_io
) {
	Assert(widget_io);

	Widget_Redraw(widget_io);
}

instant Array<String> *
Widget_GetListArray(
    Widget &widget
) {
    return &widget.data.as_row_data;
}

instant void
Widget_GetListArrayFiltered(
	Widget *widget,
	Array<String> **as_row_data_out
) {
	Assert(widget);
	Assert(as_row_data_out);

	Array<String> *as_target = Widget_GetListArray(*widget);

	if (widget->data.s_row_filter.length) {
		as_target = &widget->data.as_filter_data;
		Clamp(&widget->data.selected_row_id, 0, widget->data.as_filter_data.count - 1);
	}

	*as_row_data_out = as_target;
}

instant void
MemorySegment_AddWidgets(
	Array<MemorySegment> *a_segments,
	Array<Widget>         *a_widgets
) {
	Assert(a_segments);
	Assert(a_widgets);

    FOR_ARRAY(*a_widgets, it_widget) {
		Widget *t_widget = &ARRAY_IT(*a_widgets, it_widget);

		MemorySegment_AddWidgets(a_segments, &t_widget->a_subwidgets);
		MemorySegment_Add(a_segments, t_widget->events);
    }
}

instant void
MemorySegment_AddWidgets(
	Array<MemorySegment> *a_segments,
	Array<Widget *>       *ap_widgets
) {
	Assert(a_segments);
	Assert(ap_widgets);

    FOR_ARRAY(*ap_widgets, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it_widget);

		MemorySegment_AddWidgets(a_segments, &t_widget->a_subwidgets);
		MemorySegment_Add(a_segments, t_widget->events);
    }
}

instant void
Widget_UseScrollDefault(
	Widget *widget_io
) {
	Assert(widget_io);

	widget_io->text.offset_x = 0;
	widget_io->text.offset_y = 0;
}

instant void
Widget_UpdateListBox(
	Widget *widget_io
) {
	Assert(widget_io);

	if (widget_io->type != WIDGET_LISTBOX)
		return;

	if (!Widget_HasChanged(widget_io, true))
		return;

	Text *text = &widget_io->text;

	Vertex_ClearAttributes(&widget_io->vertex_rect);
	Text_Clear(text);

	Widget_InvalidateBackground(widget_io);

	text->data.rect = *Widget_GetRectRef(widget_io);
	Rect *rect_text = &text->data.rect;

	s32 pad_left = 2;

	rect_text->x += text->offset_x + pad_left;

	widget_io->rect_content.h = 0;

	Array<String> *as_target;
	Widget_GetListArrayFiltered(widget_io, &as_target);

	FOR_ARRAY(*as_target, it_row) {
	    /// if the string is changing in OnDrawItem,
	    /// the data in s_name must be overwritten,
	    /// otherwise this line would require a copy
	    /// even when OnDrawItem would not be used or
	    /// the string is not changing
		String s_data = ARRAY_IT(*as_target, it_row);

		Widget_Item item = {S(s_data), text->data.color};

		if (widget_io->OnDrawItem) {
            widget_io->OnDrawItem(widget_io, &item);
            text->data.color = item.colorText;
            s_data = item.s_name;
		}

		Color32 t_color_rect = widget_io->data.color_outline;

		if (widget_io->data.selected_row_id == it_row) {
			if (widget_io->data.has_focus)
				t_color_rect = widget_io->data.color_outline_selected;
			else
				t_color_rect = widget_io->data.color_outline_inactive;
		}

		u64 number_of_lines = Array_SplitWordsBuffer(s_data, &text->as_words);
		rect_text->h = Text_BuildLines(text, &text->as_words, number_of_lines, &text->a_text_lines);

		Rect rect_box = *rect_text;
		rect_box.x -= pad_left;
		rect_box.x -= text->offset_x;

		Vertex_AddRect32(&widget_io->vertex_rect, rect_box, t_color_rect);

		Text_AddLines(text);

		s32 height_row_step = rect_text->h + widget_io->data.spacing;
		rect_text->y += height_row_step;
		widget_io->rect_content.h += height_row_step;

		/// reset custom item settings
		if (widget_io->OnDrawItem) {
            text->data.color = text->data_prev.color;
		}

		String_Destroy(item.s_name);
	}

	if (widget_io->rect_content.h) {
		widget_io->rect_content.h -= widget_io->data.spacing;
	}

	/// revert for scissor
	*rect_text = *Widget_GetRectRef(widget_io);
}

/// display only at this point, without any other features
instant void
Widget_UpdateListView(
	Widget *widget_io
) {
	Assert(widget_io);

	constexpr u16 cellspacing = 0;

	if (widget_io->type != WIDGET_LISTVIEW)
		return;

	if (!Widget_HasChanged(widget_io, true))
		return;

	Text *text = &widget_io->text;

	text->data.rect = *Widget_GetRectRef(widget_io);

	Text_Clear(text);

	Rect rect = Text_GetRect(text);
	float rect_x_base = rect.x;
	s32   line_height = Font_GetLineHeight(text->font);
	rect.h = line_height;

	/// prepare to redraw everything
	Widget_InvalidateBackground(widget_io);

	if (widget_io->data.is_header_visible) {
		FOR_ARRAY(widget_io->data.a_table_columns, it_item) {
			Widget_Column *header_column = &ARRAY_IT(widget_io->data.a_table_columns, it_item);

			rect.w = header_column->width;

			Vertex_AddRect32(&widget_io->vertex_rect, rect, {0.7, 0.7, 0.7, 1});

			Rect rect_crop = rect;

			Vertex_AddText(&text->a_vertex_chars, text->shader_set, text->font,
							rect, rect_crop, {0, 0, 0.8, 1}, text->data.align_x, header_column->s_name);

			rect.x += rect.w + cellspacing + header_column->spacing;
		}

		rect.x  = rect_x_base;
		rect.y += line_height + cellspacing;
	}

	if (!widget_io->data.a_table_data)
		return;

	AssertMessage([&]() {
		if (widget_io->data.is_header_visible) {
			FOR_ARRAY(*widget_io->data.a_table_data, it_row) {
				Array<String> *as_items = &ARRAY_IT(*widget_io->data.a_table_data, it_row);

				if (as_items->count != widget_io->data.a_table_columns.count)
					return false;
			}
		}

		return true;
	}(),
	"Amount of header does not match table column count.");

	FOR_ARRAY(*widget_io->data.a_table_data, it_row) {
		Array<String> *as_items = &ARRAY_IT(*widget_io->data.a_table_data, it_row);

		FOR_ARRAY(*as_items, it_item) {
			String        *s_item        = &ARRAY_IT(*as_items, it_item);
			Widget_Column *header_column = &ARRAY_IT(widget_io->data.a_table_columns, it_item);

			rect.w = header_column->width;

			if (it_row == widget_io->data.selected_row_id) {
				Vertex_AddRect32(&widget_io->vertex_rect, rect, {0.50, 0.50, 0.75, 1});
			}
			else {
				if (it_row % 2)
					Vertex_AddRect32(&widget_io->vertex_rect, rect, {0.75, 0.75, 0.75, 1});
				else
					Vertex_AddRect32(&widget_io->vertex_rect, rect, {0.8, 0.8, 0.8, 1});
			}

			Rect rect_crop = rect;

			Vertex_AddText(&text->a_vertex_chars, text->shader_set, text->font,
							rect, rect_crop, text->data.color, text->data.align_x, *s_item);

			rect.x += rect.w + cellspacing + header_column->spacing;
		}

		rect.x  = rect_x_base;
		rect.y += line_height + cellspacing;
	}
}

instant bool
Widget_Update(
	Widget *widget_io
) {
	Assert(widget_io);

	bool result = false;

	LOG_STATUS("Update: " << widget_io->type << " ");

	if (widget_io->a_subwidgets.count) {
		LOG_STATUS("- Subwidgets: " << widget_io->a_subwidgets.count << " ");
		LOG_STATUS("\n----------------------\n");
	}

	FOR_ARRAY(widget_io->a_subwidgets, it) {
		Widget *t_widget = &ARRAY_IT(widget_io->a_subwidgets, it);
		Widget_Update(t_widget);
	}

	if (widget_io->a_subwidgets.count) {
		LOG_STATUS("----------------------");
		LOG_STATUS("\nResume update: " << widget_io->type << " ");
	}

	Text *text = &widget_io->text;

	/// text needs to be processed at least once when no input in handled,
	/// but do NOT do it for lists. lists take the list data and pass each row
	/// through the text struct to generate text + positions.
	/// using the update function here again, would invalidate that data,
	/// since the data would definitely have changed
	if (!Widget_IsListType(widget_io)) {
		if (Widget_HasChanged(widget_io, true)) {
			/// margin, padding, aso. could change,
			/// which would be needed for trigger_autosize
			Widget_InvalidateBackground(widget_io);
		}

		widget_io->events.on_text_changed = Text_Update(text);
	}
	else {
		Widget_UpdateListBox(widget_io);
		Widget_UpdateListView(widget_io);
	}

	/// Triggers
	/// =======================================================================
 	if (widget_io->trigger.autosize) {
		Layout_Data_Settings *layout_data = &widget_io->layout_data.settings;
		Text_Data *settings = &text->data;

		/// minimum width / height (to not make widgets unvisible)
		/// -------------------------------------------------------------------
		if (widget_io->text.font) {
			if (!layout_data->rect.w) {
				s32 min_width  = widget_io->text.font->size;
				Widget_AddBorderSizes(widget_io, &min_width , 0);

				layout_data->rect.w = min_width;
				settings->rect.w = min_width;
				widget_io->rect_content.w = min_width;
			}

			if (!layout_data->rect.h) {
				s32 min_height = Font_GetLineHeight(widget_io->text.font);
				Widget_AddBorderSizes(widget_io, &min_height, 0);

				layout_data->rect.h = min_height;
				settings->rect.h = min_height;
				widget_io->rect_content.h = min_height;
			}
		}
		/// auto width / height (depends on content-size)
		/// -------------------------------------------------------------------
		if (layout_data->auto_width) {
			if (settings->content_width) {
				s32 width_auto = settings->content_width;
				Widget_AddBorderSizes(widget_io, &width_auto, 0);

				if (layout_data->rect.w != width_auto) {
					layout_data->rect.w  = width_auto;
					result = true;
				}
			}
		}

		if (layout_data->auto_height) {
			if (settings->content_height) {
				s32 height_auto = settings->content_height;
				Widget_AddBorderSizes(widget_io, 0, &height_auto);

				if (layout_data->rect.h != height_auto) {
					layout_data->rect.h  = height_auto;
					result = true;
				}
			}
		}
	}

	if (widget_io->trigger.popout) {
		widget_io->data.is_popout = !widget_io->data.is_popout;

		if (widget_io->data.can_popout_focus_change) {
			if (widget_io->data.is_popout) {
				Widget_SetFocus(*widget_io);
			}
			else {
				if (widget_io->widget_focus_on_popout) {
					Widget_SetFocus(*widget_io->widget_focus_on_popout);
				}
			}
		}
	}

	if (widget_io->data.s_row_filter.has_changed) {
		widget_io->data.as_filter_data.by_reference = true;

		Widget_UseScrollDefault(widget_io);

		Array_Filter(
			widget_io->data.as_filter_data,
			widget_io->data.as_row_data,

			[&](String s_value) {
				return (String_IndexOf(
							s_value,
							widget_io->data.s_row_filter,
							0,
							widget_io->data.is_filter_case_sensitive) >= 0
						);
			}
		);

		widget_io->data.s_row_filter.has_changed = false;
	}

	widget_io->trigger = {};

	LOG_STATUS("completed\n");

	return result;
}

/// return an non-rendered overlay widget (if exists)
instant Widget *
Widget_Render(
	ShaderSet *shader_set,
	Widget *widget_io,
	bool render_overlay = false
) {
	Assert(shader_set);
	Assert(widget_io);

	if (!widget_io->data.is_visible)
		return 0;

	widget_io->text.shader_set = shader_set;

	/// draw non-list data
	if (!Widget_IsListType(widget_io)) {
		if (widget_io->data.is_floating AND !render_overlay) {
			return widget_io;
		}

		/// for input changing rects (hover, checked, aso.)
		if (widget_io->vertex_rect_sublayer.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget_io->vertex_rect_sublayer);
		}

		/// for input changing circles
		if (widget_io->a_vertex_fans.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_TRIANGLE_FAN);
			Vertex_Render(shader_set, &widget_io->a_vertex_fans);
		}

		/// - draws rects
		/// - draws rects beneath assigned texture, in case it is
		///   rendered smaller to keep aspect ratio
		if (widget_io->vertex_rect.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget_io->vertex_rect);
		}

		if (widget_io->vertex_rect.texture.ID) {
			ShaderSet_Use(shader_set, SHADER_PROG_TEXTURE_SIZE);

			s32 width, height;
			Texture_GetSizeAndBind(&widget_io->vertex_rect.texture, &width, &height);

			Rect rect_tex_aspect = *Widget_GetRectRef(widget_io);

			Rect_GetAspect(&rect_tex_aspect, width, height);

			static Vertex vertex_texture = Vertex_Create(VERTEX_RECT);
			Vertex_SetTexture(shader_set, &vertex_texture, &widget_io->vertex_rect.texture);
			vertex_texture.settings = widget_io->vertex_rect.settings;

			Vertex_AddRectTexture(&vertex_texture, rect_tex_aspect);
			Rect_Render(shader_set, &vertex_texture);

			Vertex_ClearAttributes(&vertex_texture);
		}

		Text_Render(&widget_io->text);
	}
	else {
		/// do not draw a list widget,
		/// if it should not be rendered as a popout
		if (widget_io->data.is_floating AND (!widget_io->data.is_popout OR !render_overlay)) {
			return widget_io;
		}

		Text *text = &widget_io->text;

		OpenGL_Scissor(shader_set->window, widget_io->layout_data.settings.rect);

		/// static rects
		///@Note: using a shader will reset uniform offsets to 0
		ShaderSet_Use(shader_set, SHADER_PROG_RECT);
		Rect_Render(shader_set, &widget_io->vertex_rect_sublayer);

		ShaderSet_Use(shader_set, SHADER_PROG_TRIANGLE_FAN);
		Vertex_Render(shader_set, &widget_io->a_vertex_fans);

		/// dynamic rects
		ShaderSet_Use(shader_set, SHADER_PROG_RECT);
		Shader_SetValue(text->shader_set, "offset_x", text->offset_x);
		Shader_SetValue(text->shader_set, "offset_y", text->offset_y);
		Rect_Render(shader_set, &widget_io->vertex_rect);

		Text_Render(&widget_io->text);

		OpenGL_Scissor_Disable();
	}

	Widget *wg_overlay = 0;

	FOR_ARRAY(widget_io->a_subwidgets, it_sub) {
		Widget *t_subwidget = &ARRAY_IT(widget_io->a_subwidgets, it_sub);

		wg_overlay = Widget_Render(shader_set, t_subwidget);
	}

	return wg_overlay;
}

instant void
Widget_Render(
	ShaderSet *shader_set,
	Array<Widget *> *ap_widgets_io
) {
	Assert(ap_widgets_io);

	Widget *wg_overlay = 0;

    FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);

		Widget *twg_overlay_buffer = Widget_Render(shader_set, t_widget, false);

		if (twg_overlay_buffer) {
			///@Note: there should be only one visible overlay
			if (!wg_overlay)
				wg_overlay = twg_overlay_buffer;
			else
				Assert(false);
		}
    }

    if (wg_overlay) {
    	Widget_Render(shader_set, wg_overlay, true);
    }
}

instant void
Widget_Destroy(
	Widget *widget_out
) {
	Assert(widget_out);

	Text_Destroy(&widget_out->text);
	Vertex_Destroy(&widget_out->vertex_rect);
	Vertex_Destroy(&widget_out->vertex_rect_sublayer);
	Array_Destroy(&widget_out->a_vertex_fans);
	Array_Destroy(&widget_out->data.a_table_columns);
}

instant void
Widget_Destroy(
	Array<Widget *> *ap_widgets_out
) {
	Assert(ap_widgets_out);

    FOR_ARRAY(*ap_widgets_out, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_out, it_widget);
		Widget_Destroy(t_widget);
    }
}

instant bool
Widget_IsFocusable(
	Widget *widget
) {
	Assert(widget);

	bool is_focusable = widget->is_focusable;

	if (is_focusable)  return is_focusable;

	FOR_ARRAY(widget->a_subwidgets, it) {
		Widget *t_widget = &ARRAY_IT(widget->a_subwidgets, it);
		is_focusable = Widget_IsFocusable(t_widget);

		if (is_focusable)
			break;
	}

	return is_focusable;
}

instant void
Widget_UpdateFocus(
	Array<Widget *> *ap_widgets_io,
	bool check_backward
) {
	Assert(ap_widgets_io);

	bool focus_set_next = false;

	if (!ap_widgets_io->count)
		return;

	static Array<Widget *> ap_widgets_all;
	Array_ClearContainer(ap_widgets_all);

	FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);
		Array_Add(ap_widgets_all, t_widget);

		FOR_ARRAY(t_widget->a_subwidgets, it_sub) {
			Widget *t_subwidget = &ARRAY_IT(t_widget->a_subwidgets, it_sub);
			Array_Add(ap_widgets_all, t_subwidget);
		}
	}

	s64 start = 0;
	s64 end   = ap_widgets_all.count;
	s64 step  = 1;

	if (check_backward) {
		start =  end - 1;
		end   = -1;
		step  = -1;
	}

	for (s64 it_widget = start; it_widget != end; it_widget += step) {
		Widget *t_widget = ARRAY_IT(ap_widgets_all, it_widget);

		if (!Widget_IsFocusable(t_widget))
			continue;

		Keyboard *keyboard = t_widget->window->keyboard;
		Mouse    *mouse    = t_widget->window->mouse;

		bool is_visible = (   !t_widget->data.is_floating
		                   OR  t_widget->data.is_popout);

		/// if the next possible widget is supposed to get
		/// focus, check here if the widget is still coming
		/// in the list.
		/// if it is not in the list, check from the beginning
		/// of the widget list again (check section below this
		/// loop) so a possible target can be chosen
		if (    focus_set_next
			AND is_visible
			AND t_widget->is_focusable
		) {
			t_widget->data.has_focus = true;
			focus_set_next = false;
		}
		else {
			if (t_widget->data.has_focus) {
				if (IF_USE(keyboard).up[VK_TAB]) {
					if (t_widget->text.allow_tab_input) {
						if (keyboard->pressing[VK_CONTROL]) {
							t_widget->data.has_focus = false;
							focus_set_next = true;
						}
					}
					else {
						t_widget->data.has_focus = false;
						focus_set_next = true;
					}
				}
			}
		}

		if (t_widget->data.has_focus) {
			if (!is_visible) {
				t_widget->data.has_focus = false;
                focus_set_next = true;
			}
			else {
				t_widget->widget_focus_current = t_widget;
			}
		}
	}

	/// search for next focus from the beginning,
	/// if there was no focusable left past the last focused
	/// widget
	if (focus_set_next OR !Widget::widget_focus_current) {
		for (s64 it_widget = start; it_widget != end; it_widget += step) {
			Widget *t_widget = ARRAY_IT(ap_widgets_all, it_widget);

			bool is_visible = (   !t_widget->data.is_floating
							   OR  t_widget->data.is_popout);

			if (t_widget->is_focusable AND is_visible) {
				t_widget->data.has_focus = true;
				t_widget->widget_focus_current = t_widget;

				break;
			}
		}
	}
}

instant void
Widget_CalcSelectedRowRect(
	Widget *widget,
	Rect *rect_row_out
) {
	Assert(widget);
    Assert(rect_row_out);

    *rect_row_out = {};

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->layout_data.settings.rect;
	rect_item.x    += widget->text.offset_x;
	rect_item.y    += widget->text.offset_y;

    FOR_ARRAY(widget->data.as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->data.as_row_data, it_row);
		widget->text.s_data = *ts_data;

		s32 width, height;
		Text_GetSize(&widget->text, &width, &height);

		rect_item.w = width;
		rect_item.h = height;

		if (widget->data.selected_row_id == it_row) {
			*rect_row_out = rect_item;
			break;
		}

		rect_item.y += rect_item.h + widget->data.spacing;
    }

    widget->text.s_data = ts_data_backup;
}

instant bool
Widget_CalcActiveRowID(
	Widget *widget,
	Mouse *mouse,
	u64 *active_row_id_out
) {
    Assert(widget);
    Assert(mouse);

    bool result = false;

    if (!Mouse_IsHovering(widget, mouse))
		return false;

    String ts_data_backup = widget->text.s_data;

	Rect rect_item  = widget->layout_data.settings.rect;
	rect_item.x    += widget->text.offset_x;
	rect_item.y    += widget->text.offset_y;

    FOR_ARRAY(widget->data.as_row_data, it_row) {
		String *ts_data = &ARRAY_IT(widget->data.as_row_data, it_row);
		widget->text.s_data = *ts_data;

		s32 width, height;
		Text_GetSize(&widget->text, &width, &height);

		rect_item.w = width;
		rect_item.h = height;

		if (Rect_IsIntersecting(&mouse->point, &rect_item)) {
			if (active_row_id_out)
				*active_row_id_out = it_row;

			result = true;
			break;
		}

		rect_item.y += rect_item.h + widget->data.spacing;
    }

    widget->text.s_data = ts_data_backup;

    return result;
}

instant void
Widget_LinkRadiogroup(
	Array<Widget *> *ap_radiogroup
) {
	Assert(ap_radiogroup);

	FOR_ARRAY(*ap_radiogroup, it) {
		Widget *t_widget = ARRAY_IT(*ap_radiogroup, it);
		t_widget->data.ap_radiogroup = ap_radiogroup;
	}
}

instant void
Widget_ToggleCheckbox(
	Widget *widget_io
) {
	Assert(widget_io);
	Assert(widget_io->is_checkable);

	if (!widget_io->data.ap_radiogroup) {
		widget_io->data.is_checked = !widget_io->data.is_checked;
		return;
	}

	FOR_ARRAY(*widget_io->data.ap_radiogroup, it) {
		Widget *t_widget = ARRAY_IT(*widget_io->data.ap_radiogroup, it);
		Assert(t_widget->is_checkable);

		t_widget->data.is_checked = false;
	}

	widget_io->data.is_checked = true;
}

instant void
Widget_UpdateInput(
	Widget *widget_io
) {
	Assert(widget_io);
	Assert(widget_io->window);

    Keyboard *keyboard = widget_io->window->keyboard;
    Mouse    *mouse    = widget_io->window->mouse;

    Rect *rect_widget = Widget_GetRectRef(widget_io);

    if (widget_io->a_subwidgets.count) {
		FOR_ARRAY(widget_io->a_subwidgets, it_sub) {
			Widget *t_subwidget = &ARRAY_IT(widget_io->a_subwidgets, it_sub);

			if (t_subwidget->UpdateCustomInputs)
				t_subwidget->UpdateCustomInputs(widget_io, it_sub);
			else
				Widget_UpdateInput(t_subwidget);
		}

		return;
    }

    if (widget_io->events.on_updated_input)
		return;

	if (!widget_io->is_focusable)
		return;

	if (widget_io->data.is_floating AND !widget_io->data.is_popout)
		return;

	bool got_focus = widget_io->data.has_focus;
	u64  prev_active_row = widget_io->data.selected_row_id;

	bool has_text_cursor = widget_io->text.data.is_editable;

	bool is_scrollable_list = widget_io->has_scrollable_list;
	bool is_scrollable      = widget_io->is_scrollable;

	/// mouse input handling
	/// -----------------------------------------------------------------------
    if (mouse) {
		bool is_hovering = Mouse_IsHovering(widget_io, mouse);

		/// drag & drop
		/// -------------------------------------------------------------------
		int mouse_button_index_dragging = 0;

		/// only activate, so others flags can be processed before it disables
		if (!widget_io->data.is_dragging)
			widget_io->data.is_dragging = (    mouse->pressing[mouse_button_index_dragging]
									       AND mouse->is_moving);

		if (widget_io->data.is_dragging OR mouse->down[mouse_button_index_dragging]) {
			/// resize listview columns
			if (widget_io->type == WIDGET_LISTVIEW) {
				Rect column_rect = *Widget_GetRectRef(widget_io);

				if (Rect_IsIntersecting(&mouse->point, &column_rect)) {
					s32 row_items = widget_io->data.a_table_data->count;
					/// add header row -> vertical column spacer from header to last entry
					row_items += 1;

					/// grab after every row
					column_rect.h  = Font_GetLineHeight(widget_io->text.font);
					column_rect.h *= row_items;

					/// check and set possible dragging column
					FOR_ARRAY(widget_io->data.a_table_columns, it) {
						Widget_Column *column = &ARRAY_IT(widget_io->data.a_table_columns, it);

						column_rect.x += column->width;
						column_rect.w  = column->spacing;

						/// fire only once and use flag to check, since the mouse cursor
						/// can get out of bounds with fast movement, which would cancel
						/// the unfinished resizing
						if (    mouse->down[mouse_button_index_dragging]
							AND Rect_IsIntersecting(&mouse->point, &column_rect)
						) {
							column->is_dragging = true;
						}

						if (column->is_dragging) {
							column->width += mouse->point_relative.x;
							widget_io->trigger.update = true;
						}

						column_rect.x += column_rect.w;
					}
				}
			}
		}

		/// text selection
		if (has_text_cursor AND is_hovering) {
			Text *text = &widget_io->text;

			text->cursor.is_selecting = mouse->pressing[0];

			if (text->cursor.is_selecting) {
				widget_io->data.has_focus = true;

				if (mouse->is_down) {
					u64 index = Text_Cursor_FindIndex(text, mouse->point);

					text->cursor.data.index_select_start = index;
					text->cursor.data.index_select_end   = index;
				}
				else {
					u64 index = Text_Cursor_FindIndex(text, mouse->point);

					text->cursor.data.index_select_end   = index;

					Widget_Cursor_ResetBlinking(widget_io);
				}

				Text_Cursor_Update(text);
			}
		}

		/// left mouse button
		if (mouse->up[0]) {
			got_focus = is_hovering;

			if (widget_io->type != WIDGET_TYPE::WIDGET_TEXTBOX) {
                widget_io->events.on_trigger = is_hovering;
			}

			/// listbox entry selection
			if (is_scrollable_list) {
				u64 new_active_row_id;

				if (Widget_CalcActiveRowID(widget_io, mouse, &new_active_row_id)) {
					if (new_active_row_id != widget_io->data.selected_row_id) {
						widget_io->data.selected_row_id = new_active_row_id;
						widget_io->events.on_list_change_index = true;
					}
					else {
						widget_io->events.on_list_change_final = true;
					}
				}
			}

			/// checkbox toggle + radiogroup option
			if (got_focus AND widget_io->is_checkable) {
				Widget_ToggleCheckbox(widget_io);
			}

			widget_io->data.has_focus = got_focus;
		}

		/// right mouse button
		if (mouse->up[2]) {
            if (widget_io->type != WIDGET_TYPE::WIDGET_TEXTBOX) {
                widget_io->events.on_trigger_secondary = is_hovering;
            }
		}

		/// disable mouse-drag-mode
		if (mouse->up[mouse_button_index_dragging]) {
			if (widget_io->data.is_dragging) {
				if (widget_io->type == WIDGET_LISTVIEW) {
					FOR_ARRAY(widget_io->data.a_table_columns, it) {
						Widget_Column *column = &ARRAY_IT(widget_io->data.a_table_columns, it);
						column->is_dragging = false;
					}
				}

				widget_io->data.is_dragging = false;
			}
		}

		/// widget_io + list scrolling
		if (is_hovering AND (is_scrollable_list OR is_scrollable)) {
			///@TODO: scroll horizontally when pressing shift + wheel(?)
			widget_io->text.offset_y += mouse->wheel;

			if (is_scrollable_list) {
				widget_io->text.data.content_width  = widget_io->rect_content.w;
				widget_io->text.data.content_height = widget_io->rect_content.h;
			}

			Rect_ClampY(
				&widget_io->text.offset_y,
				widget_io->text.data.content_height,
				widget_io->text.data.rect.h
			);
		}
    }

    /// keyboard input handling
    /// -----------------------------------------------------------------------
    if (keyboard AND widget_io->data.has_focus) {
		bool is_key_return = keyboard->up[VK_RETURN];
		bool is_key_space  = keyboard->up[VK_SPACE];
		bool is_key_f10    = keyboard->up[VK_F10];

        if (is_key_return OR is_key_space) {
        	widget_io->events.on_trigger = true;
        }

        if (is_key_f10) {
        	widget_io->events.on_trigger_secondary = true;
        }

		if (    widget_io->is_checkable
			AND (is_key_return OR is_key_space)
		) {
			Widget_ToggleCheckbox(widget_io);
		}

		if (is_scrollable_list) {
			/// list-item navigation and triggering
			/// ---------------------------------------------------------------
			if (widget_io->data.selected_row_id) {
				if (keyboard->down[VK_UP]) {
					--widget_io->data.selected_row_id;
					widget_io->events.on_list_change_index = true;
				}

				if (keyboard->down[VK_HOME]) {
					if (widget_io->data.selected_row_id != 0) {
						widget_io->data.selected_row_id = 0;
						widget_io->events.on_list_change_index = true;
					}
				}
			}

			bool can_listbox_rowid_advance =
				(    widget_io->data.as_row_data.count
				 AND widget_io->data.selected_row_id < widget_io->data.as_row_data.count - 1);

			bool can_listview_rowid_advance =
				(    widget_io->data.a_table_data
				 AND widget_io->data.a_table_data->count
				 AND widget_io->data.selected_row_id < widget_io->data.a_table_data->count - 1);

			if (can_listbox_rowid_advance OR can_listview_rowid_advance) {
				if (keyboard->down[VK_DOWN]) {
					++widget_io->data.selected_row_id;
					widget_io->events.on_list_change_index = true;
				}

				if (keyboard->down[VK_END]) {
					if (widget_io->type == WIDGET_LISTBOX) {
						widget_io->data.selected_row_id = widget_io->data.as_row_data.count - 1;
					}
					else if (widget_io->type == WIDGET_LISTVIEW) {
						widget_io->data.selected_row_id = widget_io->data.a_table_data->count - 1;
					}
					else {
						Assert("Unhandled list type!");
					}

					widget_io->events.on_list_change_index = true;
				}
			}

			if (    widget_io->data.as_row_data.count
				AND (is_key_return OR is_key_space)
			) {
				widget_io->events.on_list_change_final = true;
			}
		}

		bool has_text_changed;
		bool has_cursor_changed;

		Text_UpdateInput(&widget_io->text, keyboard, &has_text_changed, &has_cursor_changed);

		widget_io->events.on_text_changed = has_text_changed;

		if (has_cursor_changed) {
			Widget_Cursor_ResetBlinking(widget_io);
		}
    }

    /// list scrolling (if to be selected list-item is not (fully) visible)
    /// -----------------------------------------------------------------------
    if (widget_io->type == WIDGET_LISTBOX) {
		if (prev_active_row != widget_io->data.selected_row_id AND is_scrollable_list) {
			Rect rect_active_row;
			Widget_CalcSelectedRowRect(widget_io, &rect_active_row);

			if (!Rect_IsVisibleFully(&rect_active_row, rect_widget)) {
				if (widget_io->data.scroll_type == WIDGET_SCROLL_ITEM) {
					if (widget_io->data.selected_row_id < prev_active_row) {
						widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
					}
					else {
						widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
						widget_io->text.offset_y += (rect_widget->h - rect_active_row.h);
					}
				}
				else
				if (widget_io->data.scroll_type == WIDGET_SCROLL_BLOCK) {
					if (!Rect_IsVisibleFully(&rect_active_row, rect_widget)) {
						if (widget_io->data.selected_row_id < prev_active_row) {
							widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
							widget_io->text.offset_y += (rect_widget->h - rect_active_row.h);
						}
						else {
							widget_io->text.offset_y -= (rect_active_row.y - rect_widget->y);
						}
					}
				}
				else {
					AssertMessage(	false,
									"[Widget] Unhandled widget scroll type.");
				}
			}

			Rect_ClampY(&widget_io->rect_content, *rect_widget);
		}
    }

    widget_io->events.on_updated_input = true;
}

instant void
Widget_GetSelectedRowBuffer(
	Widget *widget,
	String *s_row_data_out
) {
	Assert(widget);
	Assert(s_row_data_out);
	Assert(!widget->data.selected_row_id OR widget->data.selected_row_id < widget->data.as_row_data.count);

	if (!widget->data.as_row_data.count) {
		String_Clear(*s_row_data_out);
		return;
	}

	Array<String> *as_target;
	Widget_GetListArrayFiltered(widget, &as_target);

	String *ts_row_data = &ARRAY_IT(*as_target, widget->data.selected_row_id);

	String_Clear(*s_row_data_out);
	String_Append(*s_row_data_out, *ts_row_data);
}

instant String
Widget_GetSelectedRowRef(
	const Widget &widget
) {
	Assert(!widget.data.selected_row_id OR widget.data.selected_row_id < widget.data.as_row_data.count);

	String s_result = {};

	if (!widget.data.as_row_data.count)
		return s_result;

	if (widget.data.as_filter_data.count) {
		s_result = S(ARRAY_IT(widget.data.as_filter_data, widget.data.selected_row_id));
	}
	else {
		if (String_IsEmpty(widget.data.s_row_filter, false))
			s_result = S(ARRAY_IT(widget.data.as_row_data, widget.data.selected_row_id));
	}

	return s_result;
}

instant u64
Widget_GetSelectedRowID(
	const Widget &widget
) {
	Assert(!widget.data.selected_row_id OR widget.data.selected_row_id < widget.data.as_row_data.count);

	return widget.data.selected_row_id;
}

instant bool
Widget_Update(
	Array<Widget *> *ap_widgets_io,
	Keyboard *keyboard
) {
	Assert(ap_widgets_io);
	Assert(keyboard);

	bool result = false;

	FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);
		Widget_UpdateInput(t_widget);
	}

	FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);

		bool did_update = Widget_Update(t_widget);

		if (did_update)  result = true;
	}

	Widget_UpdateFocus(ap_widgets_io, keyboard->pressing[VK_SHIFT]);

	return result;
}

instant void
Widget_ClearRows(
	Widget *widget_io
) {
	Assert(widget_io);
	Assert(widget_io->type == WIDGET_LISTBOX);

	Array_Clear(widget_io->data.as_row_data);

	Widget_UseScrollDefault(widget_io);
}

instant void
Layout_Add(
	Layout *layout_io,
	Widget *widget
) {
	Assert(layout_io);
	Assert(widget);

	Layout_Add(layout_io, &widget->layout_data);

	/// convinence
	Array_Add(layout_io->ap_widgets, widget);

	Layout_Block *last_block = 0;
	Layout_GetLastBlock(layout_io, &last_block);

	Array_Add(last_block->ap_widgets, widget);
}

instant void
Widget_LoadDirectoryList(
	Widget *widget_io,
	String  s_directory,
	Array<Directory_Entry> *a_entries_out,
	bool show_full_path
) {
	Assert(widget_io);
	Assert(widget_io->type == WIDGET_LISTBOX);

	/// in case the directory string came from the to be destroyed directory entries
	/// -> copy a to be destroyed string for a new path
	static String ts_directory_buffer;
	String_Append(ts_directory_buffer, s_directory);

	FOR_ARRAY(*a_entries_out, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_out, it);
		String_Destroy(t_entry->s_name);
	}

	Array_ClearContainer(*a_entries_out);

	widget_io->data.selected_row_id = 0;

	/// remove "\" from directory path (f.e. C:\) for consistency
	if (String_EndWith(ts_directory_buffer, S("\\"), true)) {
		String_Remove(ts_directory_buffer,
                      ts_directory_buffer.length - 1,
                      ts_directory_buffer.length);
	}

	if (!String_IsEmpty(ts_directory_buffer)) {
		File_ReadDirectory(a_entries_out, ts_directory_buffer, DIR_LIST_ONLY_DIR  , show_full_path);
		File_ReadDirectory(a_entries_out, ts_directory_buffer, DIR_LIST_ONLY_FILES, show_full_path);

		Array_Sort(a_entries_out, Directory_Entry_Compare);
	}
	else {
		File_GetDrives(a_entries_out);
	}

	Widget_ClearRows(widget_io);

	FOR_ARRAY(*a_entries_out, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_out, it);

		String ts_entry_name = t_entry->s_name;
		ts_entry_name.is_reference = true;

		Widget_AddRowSingle(widget_io, ts_entry_name);
	}

	String_Clear(ts_directory_buffer);
}

instant Widget
Widget_CreateLabel(
	Window *window,
	Font *font,
	String s_data,
	TEXT_ALIGN_X_TYPE text_align_x = TEXT_ALIGN_X_LEFT,
    Rect rect_box = {}
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.text.data.rect_padding = {1, 1, 1, 1};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	t_widget.type         = WIDGET_LABEL;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.is_focusable = false;

	t_widget.text.data.align_x    = text_align_x;
	t_widget.text.data.rect       = rect_box;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width  = true;
	t_widget.layout_data.settings.auto_height = true;

	t_widget.text.font = font;

	if (!String_IsEmpty(s_data))
		String_Append(t_widget.text.s_data, s_data);

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateButton(
	Window *window,
	Font *font,
	String s_data,
	Rect rect_box = {}
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.data.border_size = 2;

	t_widget.text.data.rect_margin = {2, 1, 2, 1};
	t_widget.text.data.rect_padding = {2, 2, 2, 2};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	t_widget.type         = WIDGET_BUTTON;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.data.align_x = TEXT_ALIGN_X_MIDDLE;
	t_widget.text.data.align_y = TEXT_ALIGN_Y_CENTER;

	t_widget.text.font = font;

	if (!String_IsEmpty(s_data))
		String_Append(t_widget.text.s_data, s_data);

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateListBox(
	Window *window,
	Font *font,
	Rect rect_box = {}
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.data.color_outline = {0.8f, 0.8f, 0.88f};

	t_widget.type         = WIDGET_LISTBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.is_focusable = true;
	t_widget.has_scrollable_list = true;

	t_widget.text.data.rect = rect_box;

	t_widget.text.font = font;

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateCheckBox(
	Window *window,
	Font *font,
	String s_data,
	bool checked,
    Rect rect_box = {}
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.type         = WIDGET_CHECKBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.is_checkable = true;
	t_widget.is_focusable = true;

	t_widget.data.border_size = 2;
	t_widget.data.is_checked  = checked;

	t_widget.text.font = font;

	String_Append(t_widget.text.s_data, s_data);

	t_widget.text.data.rect_padding = {1, 1, 3, 1};

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant Widget
Widget_CreatePictureBox(
	Window *window,
	Texture *texture = 0,
	Rect rect_box = {}
) {
	Assert(window);

	Widget t_widget = {};

	t_widget.type         = WIDGET_PICTUREBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	if (texture)
		t_widget.vertex_rect.texture = *texture;

	t_widget.is_focusable = false;

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateSpreader(
	Window *window
) {
	Assert(window);

	Widget t_widget = {};

    t_widget.type = WIDGET_SPREADER;
    t_widget.window = window;

    t_widget.is_focusable = false;

    return t_widget;
}

instant void
Widget_UpdateInputNumberPicker(
	Widget *widget_parent_io,
	u64 sub_index
) {
	Assert(widget_parent_io);

	Widget *widget = &ARRAY_IT(widget_parent_io->a_subwidgets, sub_index);

	Widget_UpdateInput(widget);

	Widget *tw_label = &ARRAY_IT(widget_parent_io->a_subwidgets, 0);

	/// based on subwidget array index
	switch (sub_index) {
		case 1:
		case 2: {
			if (widget->events.on_trigger) {
				Widget_Slide *t_slide = &widget_parent_io->slide;

				t_slide->value = Convert_ToInt(tw_label->text.s_data);

				if (sub_index == 1) {
					if (t_slide->value - t_slide->step >= t_slide->start)
						t_slide->value -= t_slide->step;
				}
				else
				if (sub_index == 2) {
					if (t_slide->value + t_slide->step <= t_slide->end)
						t_slide->value += t_slide->step;
				}

				char *c_value = Convert_ToCString(t_slide->value);

				String_Clear(tw_label->text.s_data);
				String_Append(tw_label->text.s_data, S(c_value));

				Memory_Free(c_value);
			}
		} break;
	}

}

instant void
Widget_RedrawNumberPickerButton(
	Widget *widget_io
) {
	Assert(widget_io);

	/// @Investigate
	Vertex *t_vertex = &widget_io->vertex_rect_sublayer;

	Rect rect_box = *Widget_GetRectRef(widget_io);

	if (!t_vertex->array_id) Vertex_Create(t_vertex, VERTEX_RECT);
	else                     Vertex_ClearAttributes(t_vertex);

	if (widget_io->data.has_focus)
		Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline_selected);
	else
		Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline);

	Rect_Resize(&rect_box, -2);
	Vertex_AddRect32(t_vertex, rect_box, {1, 1, 1, 1});

	widget_io->text.data.rect = *Widget_GetRectRef(widget_io);
	widget_io->text.data.rect.x += 1;
	widget_io->text.data.rect.h -= 3;
}

instant Widget
Widget_CreateNumberPicker(
	Window *window,
	Font *font,
	Widget_Slide slide,
	Rect rect_box = {}
) {
	Widget t_widget = {};

    t_widget.type = WIDGET_NUMBERPICKER;
    t_widget.window = window;
    t_widget.rect_content = rect_box;
    t_widget.slide = slide;

    t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.is_focusable = false;

	Widget wg_label       = Widget_CreateLabel( window, font, S(""), TEXT_ALIGN_X_LEFT, {0, 0, 50, 24});
	Widget wg_button_up   = Widget_CreateButton(window, font, S("<"), {0, 0, 24, 24});
	Widget wg_button_down = Widget_CreateButton(window, font, S(">"), {0, 0, 24, 24});
	Widget wg_spreader    = Widget_CreateSpreader(window);

	wg_button_up.OwnerDraw   = Widget_RedrawNumberPickerButton;
	wg_button_down.OwnerDraw = Widget_RedrawNumberPickerButton;
	wg_button_up.UpdateCustomInputs   = Widget_UpdateInputNumberPicker;
	wg_button_down.UpdateCustomInputs = Widget_UpdateInputNumberPicker;

	wg_label.text.data.align_x = TEXT_ALIGN_X_RIGHT;
	wg_label.text.data.rect_padding = {2, 2, 2, 2};

	char *c_value = Convert_ToCString(slide.value);
	String_Append(wg_label.text.s_data, S(c_value));
	Memory_Free(c_value);

	Array_Add(t_widget.a_subwidgets, wg_label);
	Array_Add(t_widget.a_subwidgets, wg_button_up);
	Array_Add(t_widget.a_subwidgets, wg_button_down);
	Array_Add(t_widget.a_subwidgets, wg_spreader);

	return t_widget;
}

instant Widget
Widget_CreateTextBox(
	Window *window,
	Font *font,
	bool is_multiline,
	Rect rect_box = {}
) {
	Widget t_widget = {};

	t_widget.text.data.rect_padding = {1, 1, 1, 1};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	t_widget.type         = WIDGET_TEXTBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.is_scrollable = true;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.font = font;

	t_widget.text.data.rect = rect_box;
	t_widget.text.data.is_editable = true;

	if (is_multiline) {
		t_widget.text.allow_tab_input = true;
	}
	else {
		t_widget.text.data.use_no_linebreak = true;
	}

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant bool
Widget_HasFocusInclSub(
	Widget *widget
) {
	Assert(widget);

	bool result = false;

	result = widget->data.has_focus;
	if (result)  return result;

	FOR_ARRAY(widget->a_subwidgets, it_sub) {
		Widget *t_subwidget = &ARRAY_IT(widget->a_subwidgets, it_sub);

		result = Widget_HasFocusInclSub(t_subwidget);
		if (result)  return result;
	}

	return result;
}

instant void
Widget_TriggerPopout(
	Widget *widget,
	bool can_focus_change
) {
	Assert(widget);

	widget->data.can_popout_focus_change = can_focus_change;
	widget->trigger.popout = true;
}

instant void
Widget_UpdateInputComboBox(
	Widget *widget_parent_io,
	u64 sub_index
) {
	Assert(widget_parent_io);

	Widget *widget = &ARRAY_IT(widget_parent_io->a_subwidgets, sub_index);
	Widget_UpdateInput(widget);

	Widget *wg_text   = &ARRAY_IT(widget_parent_io->a_subwidgets, WIDGET_COMBOBOX_TEXT);
	Widget *wg_button = &ARRAY_IT(widget_parent_io->a_subwidgets, WIDGET_COMBOBOX_TOGGLE);
	Widget *wg_list   = &ARRAY_IT(widget_parent_io->a_subwidgets, WIDGET_COMBOBOX_LIST);

	Assert(wg_list->window);

	Keyboard *keyboard = wg_list->window->keyboard;
	Mouse    *mouse    = wg_list->window->mouse;

	switch (sub_index) {
		case WIDGET_COMBOBOX_TEXT: {
			if (widget->data.has_focus) {
				if (keyboard->up[VK_F4]) {
					Widget_TriggerPopout(wg_list, false);
				}

				if (keyboard->up[VK_DOWN]) {
					Widget_SetFocus(*wg_list);
				}
			}
		} break;

		case WIDGET_COMBOBOX_TOGGLE: {
			if (widget->events.on_trigger) {
				Widget_TriggerPopout(wg_list, true);
			}
		} break;

		case WIDGET_COMBOBOX_LIST: {
			bool hide_popout = false;
			bool update_label = false;

			if (wg_list->data.has_focus) {
				if (widget_parent_io->type_select == WIDGET_SELECT_ON_INDEX_CHANGE) {
					update_label = true;
				}

				if (    widget_parent_io->type_select == WIDGET_SELECT_ON_RETURN
					AND keyboard->up[VK_RETURN]
				) {
					update_label = true;
				}

				if (keyboard->up[VK_RETURN]) {
					hide_popout = true;

					Keyboard_ResetKey(keyboard, VK_RETURN);
				}

				if (keyboard->up[VK_ESCAPE]) {
					hide_popout = true;

					Keyboard_ResetKey(keyboard, VK_ESCAPE);
				}

				if (widget->events.on_trigger) {
					if (!wg_list->events.on_list_change_index) {
						update_label = true;
						hide_popout = true;
					}
				}

				if (update_label) {
					static String s_row_data;
					Widget_GetSelectedRowBuffer(wg_list, &s_row_data);

					if (wg_text->text.s_data != s_row_data) {
						String_Clear(wg_text->text.s_data);
						String_Append(wg_text->text.s_data, s_row_data);
						wg_text->events.on_text_changed = true;

						wg_text->text.cursor.data.index_select_end   = s_row_data.length;
						wg_text->text.cursor.data.index_select_start = 0;

						/// if this is not set, flushing cursor data after cursor was updated
						/// will set both cursor indexes to the same value.
						/// due to that, it won't be possible to check of something was prev.
						/// selected, even if the selection is still visible (because flushing
						/// is happening after selection process of done)
						wg_text->text.cursor.is_selecting = true;
					}
				}
			}
			else {
				/// so text can be typed into the combobox
				/// while the list is visible
				if (!Widget_HasFocusInclSub(widget_parent_io)) {
					hide_popout = true;
				}
			}

			if (hide_popout) {
				if (wg_list->data.is_popout) {
					widget->events.on_list_change_final = true;

					Widget_TriggerPopout(widget, true);
				}
			}
		} break;
	}
}

instant String *
Widget_GetTextData(
	Widget &widget
) {
	return &widget.text.s_data;
}

instant String
Widget_GetTextDataRef(
	const Widget &widget
) {
	return S(widget.text.s_data);
}

instant void
Widget_SetText(
    Widget &widget,
    const String &s_data
) {
    String_Overwrite(widget.text.s_data, s_data);
}

instant Widget
Widget_CreateComboBox(
	Window *window,
	Font *font,
	s32 combo_height,
	Rect rect_box = {}
) {
	Widget t_widget = {};

	t_widget.type         = WIDGET_COMBOBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.is_focusable = false;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width = true;

	Widget wg_text   = Widget_CreateTextBox(window, font, false);
	Widget wg_button = Widget_CreateButton( window, font, S("+"));
	Widget wg_list   = Widget_CreateListBox(window, font, {0, 0, 0, combo_height});

	wg_text.UpdateCustomInputs   = Widget_UpdateInputComboBox;
	wg_button.UpdateCustomInputs = Widget_UpdateInputComboBox;
	wg_list.UpdateCustomInputs   = Widget_UpdateInputComboBox;

	wg_list.data.is_floating = true;
	wg_list.data.color_background = Color_MakeGrey(0.8f);

	wg_text.text.data.rect_padding  = {2, 1, 2, 1};
	wg_text.text.data.use_no_linebreak = true;

	wg_button.text.data.rect_margin  = {};
	wg_button.text.data.rect_padding = {3, -1, 3, -1};
	wg_button.data.border_size = 2;

	Widget *twg_text = Array_Add(t_widget.a_subwidgets, wg_text);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_TEXT);

	Widget *twg_button = Array_Add(t_widget.a_subwidgets, wg_button);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_TOGGLE);

	Widget *twg_list   = Array_Add(t_widget.a_subwidgets, wg_list);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_LIST);

	twg_list->widget_focus_on_popout = twg_text;

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateProgressbar(
	Window *window,
	Font *font,
	Widget_Slide slide,
	Rect rect_box = {}
) {
	Widget t_widget = {};

	t_widget.type         = WIDGET_PROGRESSBAR;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;
	t_widget.slide        = slide;

	t_widget.is_focusable = false;

	t_widget.text.font         = font;
	t_widget.text.data.align_x = TEXT_ALIGN_X_MIDDLE;
	t_widget.text.data.align_y = TEXT_ALIGN_Y_CENTER;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width = true;

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateListView(
	Window *window,
	Font *font,
	Rect rect_box = {}
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.type         = WIDGET_LISTVIEW;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.is_scrollable       = true;
	t_widget.has_scrollable_list = true;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.font = font;
	t_widget.text.data.rect = rect_box;

	t_widget.trigger.autosize = true;

	return t_widget;
}

instant void
Widget_AddColumn(
	Widget *widget_io,
	String s_header,
	s32 width_in_pixel = -1
) {
	Assert(widget_io);

	/// copy, so it can be changed at runtime
	Widget_Column column;
	column.s_name = String_Copy(s_header);

	s32 column_width;
	Text_GetSize(widget_io->text.font, s_header, &column_width, &column.height);

	if (width_in_pixel < 0)
		column.width = column_width;
	else
		column.width = width_in_pixel;

	Array_Add(widget_io->data.a_table_columns, column);
}

instant void
Widget_LoadFile(
	Widget *widget_io,
	String s_filename
) {
	Assert(widget_io);
	Assert(widget_io->type == WIDGET_TEXTBOX);

	if (!File_Exists(s_filename))
		LOG_WARNING("Trying to load file content into a textbox widget has failed.");

	String *s_data = &widget_io->text.s_data;

	String_Clear(*s_data);
	String_Append(*s_data, File_ReadAll(s_filename, true));
}

instant void
Widget_SwapLayout(
	Array<Widget *> **ap_widgets_active,
	Layout          **layout_active,
	Array<Widget *>  *ap_widgets_swap_to,
	Layout           *layout_swap_to
) {
	Assert(ap_widgets_active);
	Assert(layout_active);

	Assert(ap_widgets_swap_to);
	Assert(layout_swap_to);

	*ap_widgets_active = ap_widgets_swap_to;
	*layout_active     = layout_swap_to;

	if ((*ap_widgets_active)->count)
		Widget_SetFocus(*ARRAY_IT((**ap_widgets_active), 0));
}

template <typename Func>
instant void
Widget_ForEach(
	Array<Widget *> *ap_widgets,
	Func OnWidgetChange
) {
	Assert(ap_widgets);

	FOR_ARRAY(*ap_widgets, it) {
		Widget *t_widget = ARRAY_IT(*ap_widgets, it);

		OnWidgetChange(t_widget);

		FOR_ARRAY(t_widget->a_subwidgets, it_sub) {
			Widget *t_widget_sub = &ARRAY_IT(t_widget->a_subwidgets, it_sub);

			OnWidgetChange(t_widget_sub);
		}
	}
}

instant void
Widget_LinkTableData(
	Widget *widget_io,
	Array<Array<String>> *a_tabledata
) {
	Assert(widget_io);
	Assert(widget_io->type == WIDGET_LISTVIEW);

	Assert(a_tabledata);

	if (a_tabledata->count) {
		Array<String> *as_row = &ARRAY_IT(*a_tabledata, 0);
		AssertMessage(as_row->count == widget_io->data.a_table_columns.count,
					  "Amount of header does not match table column count.");
	}

	widget_io->data.a_table_data = a_tabledata;
}
