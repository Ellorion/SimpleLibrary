#pragma once

struct Widget;

typedef void (*Widget_OwnerDraw)
	(Widget *widget_io);
typedef void (*Widget_UpdateCustomInputsSub)
	(Widget *widget_parent_io, u64 sub_index);

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
	WIDGET_PROGRESSBAR
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

struct Widget_Data {
	Color32 color_background       = Color_MakeGrey(0.9f);
	Color32 color_outline          = {0.8f, 0.8f, 0.8f, 1.0f};
	Color32 color_outline_selected = {1.0f, 0.0f, 0.0f, 1.0f};
	Color32 color_outline_inactive = {0.5f, 0.5f, 1.0f, 1.0f};
	Color32 color_progress         = {0.5f, 0.5f, 1.0f, 1.0f};

	u64  active_row_id = 0;

	u32  border_size   = 0;
	u32  spacing       = 1;

	bool is_focusable  = true;
	bool is_scrollable = false;
	bool is_checkable  = false;

	bool can_popout_focus_change = false;

	/// overlay
	bool is_floating   = false; /// is overlay
	bool is_popout     = false; /// visible overlay

	bool has_focus  = false;
	bool is_checked = false;

	bool has_scrollable_list = false;
	WIDGET_SCROLL_TYPE scroll_type = WIDGET_SCROLL_ITEM;

	Array<String> as_row_data;

	String s_row_filter;
	Array<String> a_filter_data;
};

struct Widget {
	Window *window = 0;

	/// Defining Properties
	WIDGET_TYPE type;
	Widget_Data data;
	Widget_Data data_prev;
	Widget_Slide slide;

	bool visible = true;

	/// On Demand
	static Widget *widget_focus_current;
	Widget *widget_focus_on_popout = 0;
	WIDGET_SELECT_TYPE type_select = WIDGET_SELECT_ON_RETURN;

	/// Triggers (per Frame)
	bool trigger_autosize = false;
	bool trigger_popout   = false;

	/// Events (per Frame)
	struct Widget_Events {
		bool on_list_change_index = false;
		bool on_list_change_final = false;

		bool on_text_change = false;

		bool on_trigger = false;
		bool on_trigger_secondary = false;
	} events;

	/// Custom Events
	Widget_OwnerDraw OwnerDraw = 0;
	Widget_UpdateCustomInputsSub UpdateCustomInputs = 0;

	/// Rendering
	Text   text;
	Vertex vertex_rect;
	Vertex vertex_rect_static;

	/// Layout / Size
	Layout_Data layout_data;
	Rect rect_content; /// x,y = offsets

	/// Content
	Array<Widget> a_subwidgets;
};

Widget *Widget::widget_focus_current = 0;

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
		widget->visible = set_visible;
	}
}

instant bool
Layout_Block_IsVisible (
	Layout *layout,
	u64 index_block
) {
	Assert(layout);

	Layout_Block *layout_block = 0;
	bool got_block = Layout_GetBlock(layout, index_block, &layout_block);
	Assert(got_block);

	return layout_block->is_visible;
}

///@Depricated
/// simply create an array of parent widgets
///
/// Widget_UpdateFocus will automatically take
/// care of the child widgets, since those
/// tab-orders are decided upon the parent widget
/// creation anyway (if done correctly)
//instant void
//Widget_AddRenderTabStop(
//	Array<Widget *> *ap_widgets,
//	Widget *widget
//) {
//	Assert(ap_widgets);
//	Assert(widget);
//
//	Array_Add(ap_widgets, widget);
//
//	FOR_ARRAY(widget->a_subwidgets, it_sub) {
//		Widget *t_subwidget = &ARRAY_IT(widget->a_subwidgets, it_sub);
//		Array_Add(ap_widgets, t_subwidget);
//	}
//}

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
	Time_Reset(&widget_io->text.cursor.timer_blinking);
	widget_io->text.cursor.is_blink_on = true;
}

instant bool
Widget_IsListType(
	Widget *widget
) {
	Assert(widget);

	if (widget->type == WIDGET_LISTBOX)  return true;

	return false;
}

instant void
Widget_AddRow(
	Widget *widget_io,
	const char *c_row_data,
	u64 c_length = 0
) {
	if (!c_row_data)
		return;

	/// list contained in subwidgets
	switch (widget_io->type) {
		case WIDGET_COMBOBOX: {
			widget_io = &ARRAY_IT(widget_io->a_subwidgets, 2);
		} break;

		default: {} break;
	}

	String *ts_data;
	Array_AddEmpty(&widget_io->data.as_row_data, &ts_data);
	*ts_data = String_Copy(c_row_data, c_length);
}

instant void
Widget_AddRows(
	Widget *widget_io,
	Array<String> *as_list
) {
	Assert(widget_io);
	Assert(as_list);

	FOR_ARRAY(*as_list, it) {
		String *ts_item = &ARRAY_IT(*as_list, it);

		Widget_AddRow(widget_io, ts_item->value, ts_item->length);
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

instant bool
Widget_HasChanged(
	Widget *widget_io,
	bool update_changes
) {
	Assert(widget_io);

	bool result = false;

	///@Note: updating text changed status will happen
	///       in a Text_Update
	result = Text_HasChanged(&widget_io->text, false);

	if (!result) {
		result = !Memory_Compare(
							&widget_io->layout_data.settings,
							&widget_io->layout_data.settings_prev,
							 sizeof(widget_io->layout_data.settings)
					  );
	}

	if (!result) {
		result = !Memory_Compare(
							&widget_io->data,
							&widget_io->data_prev,
							 sizeof(widget_io->data)
					  );
	}

	if (!result) {
		FOR_ARRAY(widget_io->data.as_row_data, it) {
			String *t_data = &ARRAY_IT(widget_io->data.as_row_data, it);

			if (t_data->changed) {
				result = true;

				if (!update_changes)
					break;

				t_data->changed = false;
			}
		}
	}

	if (update_changes) {
		widget_io->layout_data.settings_prev = widget_io->layout_data.settings;
		widget_io->data_prev = widget_io->data;
	}

/// @NOTE: do NOT check subwidgets, since they will be added to the render list anyway
///        and might mess up the update checking

	return result;
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

		/// border size is used for the checbox,
		/// not the border of the widget itself
		if (widget->type != WIDGET_CHECKBOX)
			*min_width_io += widget->data.border_size << 1;

		*min_width_io +=   widget->text.data.rect_margin.x
					  + widget->text.data.rect_margin.w;
	}

	if (min_height_io) {
		*min_height_io += rect_padding->y + rect_padding->h;

		/// border size is used for the checbox,
		/// not the border of the widget itself
		if (widget->type != WIDGET_CHECKBOX)
			*min_height_io += widget->data.border_size << 1;

		*min_height_io +=  widget->text.data.rect_margin.y
					  + widget->text.data.rect_margin.h;
	}
}

instant void
Widget_SetFocus(
	Widget *widget_io
) {
 	Assert(widget_io);

	if (!widget_io->data.is_focusable)
		return;

    if (widget_io->widget_focus_current) {
    	widget_io->widget_focus_current->data.has_focus = false;
    }

    widget_io->data.has_focus = true;
    widget_io->widget_focus_current = widget_io;

    if (widget_io->data.is_floating)
		widget_io->data.is_popout = true;
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

	/// text needs to be processed at least once when no input in handled,
	/// but do NOT do it for lists. lists take the list data and pass each row
	/// through the text struct to generate text + positions.
	/// using the update function here again, would invalidate that data,
	/// since the data would definitely have changed
	if (!Widget_IsListType(widget_io))
		Text_Update(&widget_io->text);

 	if (widget_io->trigger_autosize){
		Layout_Data_Settings *layout_data = &widget_io->layout_data.settings;
		Text_Data *settings = &widget_io->text.data;

		if (layout_data->auto_width AND settings->content_width) {
			s32 width_auto = settings->content_width;
			Widget_AddBorderSizes(widget_io, &width_auto, 0);

			if (layout_data->rect.w != width_auto) {
				layout_data->rect.w  = width_auto;
				result = true;
			}
		}

		if (layout_data->auto_height AND settings->content_height) {
			s32 height_auto = settings->content_height;
			Widget_AddBorderSizes(widget_io, 0, &height_auto);

			if (layout_data->rect.h != height_auto) {
				layout_data->rect.h  = height_auto;
				result = true;
			}
		}

		widget_io->trigger_autosize = false;
	}

	if (widget_io->trigger_popout) {
		widget_io->data.is_popout = !widget_io->data.is_popout;
		widget_io->trigger_popout = false;

		if (widget_io->data.can_popout_focus_change) {
			if (widget_io->data.is_popout) {
				Widget_SetFocus(widget_io);
			}
			else {
				if (widget_io->widget_focus_on_popout) {
					Widget_SetFocus(widget_io->widget_focus_on_popout);
				}
			}
		}
	}

	if (widget_io->data.s_row_filter.changed) {
		Array_Clear(&widget_io->data.a_filter_data);
		widget_io->data.a_filter_data.by_reference = true;
		widget_io->data.active_row_id = 0;

		if (widget_io->data.s_row_filter.length) {
			FOR_ARRAY(widget_io->data.as_row_data, it_row) {
				String *ts_data = &ARRAY_IT(widget_io->data.as_row_data, it_row);

				if (String_IndexOf(	ts_data,
									widget_io->data.s_row_filter) >= 0
				) {
					Array_Add(&widget_io->data.a_filter_data, *ts_data);
				}
			}
		}

		widget_io->data.s_row_filter.changed = false;
	}

	LOG_STATUS("completed\n");

	return result;
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

	Rect    rect_box =  widget_io->layout_data.settings.rect;

	/// static rect init
	/// -----------------------------------------------------------------------
	Vertex *t_vertex_static = &widget_io->vertex_rect_static;

	if (!t_vertex_static->array_id)
		Vertex_Create(t_vertex_static);
	else
		Vertex_ClearAttributes(t_vertex_static);

	/// dynamic rect init
	/// -----------------------------------------------------------------------
	Vertex *t_vertex_dynamic = &widget_io->vertex_rect;

	if (!t_vertex_dynamic->array_id)
		Vertex_Create(t_vertex_dynamic);
	else
		Vertex_ClearAttributes(t_vertex_dynamic);
	/// -----------------------------------------------------------------------

	switch (widget_io->type) {
		case WIDGET_SPREADER: {
		} break;

		case WIDGET_TEXTBOX: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);

			Widget_Cursor_ResetBlinking(widget_io);
		} break;

		case WIDGET_COMBOBOX: {
			Rect *rect_layout = &widget_io->layout_data.settings.rect;

			rect_layout->h = 0;

			/// calc. max. visible widget height to be used for the layout system,
			/// in order to get preper horizontal alignment
            FOR_ARRAY(widget_io->a_subwidgets, it) {
				Widget *t_widget = &ARRAY_IT(widget_io->a_subwidgets, it);

                if (!t_widget->data.is_floating) {
					rect_layout->h = MAX(rect_layout->h, t_widget->layout_data.settings.rect.h);
                }
            }

			Layout layout;
			Layout_Create(&layout, &widget_io->layout_data, false);
			layout.padding = 0;

			Layout_Block *t_block;
			u64 index_block = 0;

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

			Layout_Arrange(&layout);
		} break;

		case WIDGET_LABEL: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;
			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);
		} break;

		case WIDGET_PICTUREBOX:
		case WIDGET_LISTBOX: {
			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);
		} break;

		case WIDGET_BUTTON: {
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

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
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

			Vertex_AddRect32(t_vertex_static, rect_box, widget_io->data.color_background);

			s32 check_offset = 2;
			s32 check_h = widget_io->text.font->size - (check_offset << 1);
			s32 check_w = check_h;

			Rect rect_check = {
				rect_box.x + check_offset + widget_io->text.data.rect_padding.x,
				rect_box.y + check_offset + widget_io->text.data.rect_padding.y,
				check_w,
				check_h
			};

			widget_io->text.data.rect_margin = {
				(float)check_offset * 2 + check_w + 2,
				0,
				0,
				0
			};

			if (widget_io->data.has_focus)
				Vertex_AddRect32(t_vertex_static, rect_check, widget_io->data.color_outline_selected);
			else
				Vertex_AddRect32(t_vertex_static, rect_check, widget_io->data.color_outline_inactive);

			Assert(widget_io->data.border_size);
			Assert(widget_io->data.border_size < 20);

			widget_io->data.border_size = (widget_io->text.font->size / 10);

			Rect_Resize(&rect_check, -widget_io->data.border_size);
			Vertex_AddRect32(t_vertex_static, rect_check, widget_io->data.color_background);

			if (widget_io->data.is_checked) {
				Rect_Resize(&rect_check, -1);
				Vertex_AddRect32(t_vertex_static, rect_check, widget_io->data.color_outline_selected);
			}
		} break;

		case WIDGET_NUMBERPICKER: {
			Layout layout;
			Layout_Create(&layout, &widget_io->layout_data, false);
			layout.padding = 0;

			Layout_Block *t_block;
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
			widget_io->text.data.rect = widget_io->layout_data.settings.rect;

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

instant void
Widget_GetListArrayFiltered(
	Widget *widget,
	Array<String> **as_row_data_out
) {
	Assert(widget);
	Assert(as_row_data_out);

	Array<String> *as_target = &widget->data.as_row_data;

	if (widget->data.s_row_filter.length) {
		as_target = &widget->data.a_filter_data;
		Clamp(&widget->data.active_row_id, 0, widget->data.a_filter_data.count - 1);
	}

	*as_row_data_out = as_target;
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

	if (!widget_io->visible)
		return 0;

	widget_io->text.shader_set = shader_set;

	/// draw non-list data
	if (!Widget_IsListType(widget_io)) {
		if (widget_io->data.is_floating AND !render_overlay) {
			return widget_io;
		}

		if (Widget_HasChanged(widget_io, true)) {
			Widget_InvalidateBackground(widget_io);
		}

		if (widget_io->vertex_rect_static.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget_io->vertex_rect_static);
		}

		if (widget_io->vertex_rect.a_attributes.count) {
			ShaderSet_Use(shader_set, SHADER_PROG_RECT);
			Rect_Render(shader_set, &widget_io->vertex_rect);
		}

		if (widget_io->vertex_rect.texture.ID) {
			ShaderSet_Use(shader_set, SHADER_PROG_TEXTURE_SIZE);

			s32 width, height;
			Texture_GetSizeAndBind(&widget_io->vertex_rect.texture, &width, &height);

			Rect rect_tex_aspect = widget_io->layout_data.settings.rect;
			Rect_GetAspect(&rect_tex_aspect, width, height);

			static Vertex vertex_texture = Vertex_Create();
			Vertex_SetTexture(shader_set, &vertex_texture, &widget_io->vertex_rect.texture);
			vertex_texture.settings = widget_io->vertex_rect.settings;

			Vertex_AddRectTexture(&vertex_texture, rect_tex_aspect);
			Rect_Render(shader_set, &vertex_texture);

			Vertex_ClearAttributes(&vertex_texture);
		}

		Text_Render(&widget_io->text);
	}
	else {
		Text *text = &widget_io->text;

		/// do not draw a list widget,
		/// if it should not be rendered as a popout
		if (widget_io->data.is_floating AND (!widget_io->data.is_popout OR !render_overlay)) {
			return widget_io;
		}

		///@Refactor: update for list data, might make more sense
		///           to move it into a seperate update function
		if (Widget_HasChanged(widget_io, true)) {
			Vertex_ClearAttributes(&widget_io->vertex_rect);
			Widget_InvalidateBackground(widget_io);

			Text_Clear(text);

			text->data.rect = widget_io->layout_data.settings.rect;
			Rect *rect_text = &text->data.rect;

			s32 pad_left = 2;

			rect_text->x += text->offset_x + pad_left;

			widget_io->rect_content.h = 0;

			Array<String> *as_target;
			Widget_GetListArrayFiltered(widget_io, &as_target);

			FOR_ARRAY(*as_target, it_row) {
				String *ts_data = &ARRAY_IT(*as_target, it_row);

				u64 number_of_lines = Array_SplitWordsBuffer(ts_data, &text->as_words);
				rect_text->h = Text_BuildLines(text, &text->as_words, number_of_lines, &text->a_text_lines);

				Color32 t_color_rect = widget_io->data.color_outline;

				if (widget_io->data.active_row_id == it_row) {
					if (widget_io->data.has_focus)
						t_color_rect = widget_io->data.color_outline_selected;
					else
						t_color_rect = widget_io->data.color_outline_inactive;
				}

				Rect rect_box = *rect_text;
				rect_box.x -= pad_left;
				rect_box.x -= text->offset_x;

				Vertex_AddRect32(&widget_io->vertex_rect, rect_box, t_color_rect);

				Text_AddLines(text);

				s32 height_row_step = rect_text->h + widget_io->data.spacing;
				rect_text->y += height_row_step;
				widget_io->rect_content.h += height_row_step;

				ts_data->changed = false;
			}

			if (widget_io->rect_content.h) {
				widget_io->rect_content.h -= widget_io->data.spacing;
			}

			/// revert for scissor
			*rect_text = widget_io->layout_data.settings.rect;

			text->data_prev = text->data;
		}

		OpenGL_Scissor(shader_set->window, widget_io->layout_data.settings.rect);

		///@Note: using a shader will reset uniform offsets to 0

		/// static rects
		ShaderSet_Use(shader_set, SHADER_PROG_RECT);
		Rect_Render(shader_set, &widget_io->vertex_rect_static);

		/// dynamic rects
		ShaderSet_Use(shader_set, SHADER_PROG_RECT);
		Shader_SetValue(text->shader_set, "x_offset", text->offset_x);
		Shader_SetValue(text->shader_set, "y_offset", text->offset_y);
		Rect_Render(shader_set, &widget_io->vertex_rect);

		Text_Render(&widget_io->text);

		OpenGL_Scissor_Disable();
	}

	Widget *wg_overlay = 0;

	FOR_ARRAY(widget_io->a_subwidgets, it_sub) {
		Widget *t_subwidget = &ARRAY_IT(widget_io->a_subwidgets, it_sub);

		wg_overlay = Widget_Render(shader_set, t_subwidget);
	}

	/// Reset per Frame Event Data
	widget_io->events = {};

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
	Vertex_Destroy(&widget_out->vertex_rect_static);
}

instant void
Widget_Destroy(
	Array<Widget *> *ap_widgets_out
) {
	Assert(ap_widgets_out);

    FOR_ARRAY(*ap_widgets_out, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_out, it_widget);

		Text_Destroy(&t_widget->text);
		Vertex_Destroy(&t_widget->vertex_rect);
		Vertex_Destroy(&t_widget->vertex_rect_static);
    }
}

instant bool
Widget_IsFocusable(
	Widget *widget
) {
	Assert(widget);

	bool is_focusable = widget->data.is_focusable;

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
	Array_ClearContainer(&ap_widgets_all);

	FOR_ARRAY(*ap_widgets_io, it_widget) {
		Widget *t_widget = ARRAY_IT(*ap_widgets_io, it_widget);
		Array_Add(&ap_widgets_all, t_widget);

		FOR_ARRAY(t_widget->a_subwidgets, it_sub) {
			Widget *t_subwidget = &ARRAY_IT(t_widget->a_subwidgets, it_sub);
			Array_Add(&ap_widgets_all, t_subwidget);
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
			AND t_widget->data.is_focusable
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

			if (t_widget->data.is_focusable AND is_visible) {
				t_widget->data.has_focus = true;
				t_widget->widget_focus_current = t_widget;

				break;
			}
		}
	}
}

instant void
Widget_CalcActiveRowRect(
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

		if (widget->data.active_row_id == it_row) {
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
Widget_UpdateInput(
	Widget *widget_io
) {
	Assert(widget_io);
	Assert(widget_io->window);

    Keyboard *keyboard = widget_io->window->keyboard;
    Mouse    *mouse    = widget_io->window->mouse;

    Rect *rect_widget = &widget_io->layout_data.settings.rect;

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

	if (!widget_io->data.is_focusable)
		return;

	if (widget_io->data.is_floating AND !widget_io->data.is_popout)
		return;

	bool got_focus = widget_io->data.has_focus;
	u64  prev_active_row = widget_io->data.active_row_id;

	bool has_text_cursor = widget_io->text.data.is_editable;

	bool is_scrollable_list = widget_io->data.has_scrollable_list;
	bool is_scrollable      = widget_io->data.is_scrollable;

    if (mouse) {
		bool is_hovering = Mouse_IsHovering(widget_io, mouse);

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

			if (is_hovering) {
				widget_io->events.on_trigger = true;
			}

			/// listbox entry selection
			if (is_scrollable_list) {
				u64 new_active_row_id;

				if (Widget_CalcActiveRowID(widget_io, mouse, &new_active_row_id)) {
					if (new_active_row_id != widget_io->data.active_row_id) {
						widget_io->data.active_row_id = new_active_row_id;
						widget_io->events.on_list_change_index = true;
					}
					else {
						widget_io->events.on_list_change_final = true;
					}
				}
			}

			/// checkbox toggle
			if (got_focus AND widget_io->data.is_checkable) {
				widget_io->data.is_checked = !widget_io->data.is_checked;
			}

			/// focus change
			widget_io->data.has_focus = got_focus;
		}

		/// right mouse button
		if (mouse->up[2]) {
			got_focus = is_hovering;

			if (is_hovering) {
				widget_io->events.on_trigger_secondary = true;
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

		if (widget_io->data.is_checkable
			AND (   is_key_return
				 OR is_key_space)
		) {
			widget_io->data.is_checked = !widget_io->data.is_checked;
		}

		if (is_scrollable_list) {
			if (widget_io->data.active_row_id) {
				if (keyboard->down[VK_UP]) {
					--widget_io->data.active_row_id;
					widget_io->events.on_list_change_index = true;
				}

				if (keyboard->down[VK_HOME]) {
					if (widget_io->data.active_row_id != 0) {
						widget_io->data.active_row_id = 0;
						widget_io->events.on_list_change_index = true;
					}
				}
			}

			if (    widget_io->data.as_row_data.count
				AND widget_io->data.active_row_id < widget_io->data.as_row_data.count - 1
			) {
				if (keyboard->down[VK_DOWN]) {
					++widget_io->data.active_row_id;
					widget_io->events.on_list_change_index = true;
				}

				if (keyboard->down[VK_END]) {
					widget_io->data.active_row_id = widget_io->data.as_row_data.count - 1;
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

		widget_io->events.on_text_change = has_text_changed;

		if (has_cursor_changed) {
			Widget_Cursor_ResetBlinking(widget_io);
		}
    }

    if (prev_active_row != widget_io->data.active_row_id AND is_scrollable_list) {
		Rect rect_active_row;
        Widget_CalcActiveRowRect(widget_io, &rect_active_row);

		if (!Rect_IsVisibleFully(&rect_active_row, rect_widget)) {
			if (widget_io->data.scroll_type == WIDGET_SCROLL_ITEM) {
				if (widget_io->data.active_row_id < prev_active_row) {
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
					if (widget_io->data.active_row_id < prev_active_row) {
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

instant void
Widget_GetSelectedRowBuffer(
	Widget *widget,
	String *s_row_data_out
) {
	Assert(widget);
	Assert(s_row_data_out);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	if (!widget->data.as_row_data.count) {
		String_Clear(s_row_data_out);
		return;
	}

	Array<String> *as_target;
	Widget_GetListArrayFiltered(widget, &as_target);

	String *ts_row_data = &ARRAY_IT(*as_target, widget->data.active_row_id);

	String_Clear(s_row_data_out);
	String_Append(s_row_data_out, *ts_row_data);
}

instant void
Widget_GetSelectedRow(
	Widget *widget,
	String *s_data_out
) {
	Assert(widget);
	Assert(s_data_out);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	String_Clear(s_data_out);

	if (!widget->data.as_row_data.count)
		return;

	*s_data_out = ARRAY_IT(widget->data.as_row_data, widget->data.active_row_id);
}

instant String
Widget_GetSelectedRow(
	Widget *widget
) {
	Assert(widget);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	String s_result = {};
	Widget_GetSelectedRow(widget, &s_result);

	return s_result;
}

instant u64
Widget_GetSelectedRowID(
	Widget *widget
) {
	Assert(widget);
	Assert(!widget->data.active_row_id OR widget->data.active_row_id < widget->data.as_row_data.count);

	return widget->data.active_row_id;
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
	Widget *widget_out
) {
	Assert(widget_out);

	Array_Clear(&widget_out->data.as_row_data);
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
	Array_Add(&layout_io->ap_widgets, widget);

	Layout_Block *last_block = 0;
	Layout_GetLastBlock(layout_io, &last_block);

	Array_Add(&last_block->ap_widgets, widget);
}

instant void
Widget_LoadDirectoryList(
	Widget *widget_io,
	String  s_directory,
	Array<Directory_Entry> *a_entries_out,
	bool show_full_path = false
) {
	Assert(widget_io);
	Assert(widget_io->type == WIDGET_LISTBOX);

	/// in case the directory string came from the to be destroyed directory entries
	static String ts_directory;
	String_Append(&ts_directory, s_directory);

	FOR_ARRAY(*a_entries_out, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_out, it);
		String_Destroy(&t_entry->s_name);
	}

	Array_ClearContainer(a_entries_out);

	widget_io->data.active_row_id = 0;

	/// remove "\" from directroy path (f.e. C:\) for consistency
	if (String_EndWith(&ts_directory, S("\\"))) {
		String_Remove(&ts_directory, ts_directory.length - 1, ts_directory.length);
	}

	/// will still include path into dir array, even if it is not rendering,
	/// so the path does not have to concatonate the find the targeted file,
	/// which is more practical, than simply knowing the filename
	File_ReadDirectory(a_entries_out, ts_directory, DIR_LIST_ONLY_DIR  );
	File_ReadDirectory(a_entries_out, ts_directory, DIR_LIST_ONLY_FILES);

	Widget_ClearRows(widget_io);

	FOR_ARRAY(*a_entries_out, it) {
		Directory_Entry *t_entry = &ARRAY_IT(*a_entries_out, it);

		String ts_entry_name = t_entry->s_name;

		if (!show_full_path) {
			ts_entry_name.value  += ts_directory.length;
			ts_entry_name.length -= ts_directory.length;
		}

		/// removing leading "\" to indicate file type
		if (t_entry->type == DIR_ENTRY_FILE) {
			ts_entry_name.value  += 1;
			ts_entry_name.length -= 1;
		}

		Widget_AddRow(widget_io, ts_entry_name.value, ts_entry_name.length);
	}

	String_Clear(&ts_directory);
}

instant Widget
Widget_CreateLabel(
	Window *window,
	Font *font,
	Rect rect_box,
	String s_data,
	TEXT_ALIGN_X_TYPE text_align_x = TEXT_ALIGN_X_LEFT
) {
	Assert(window);
	Assert(font);

	Widget t_widget = {};

	t_widget.text.data.rect_padding = {1, 1, 1, 1};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font);
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_LABEL;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.data.is_focusable = false;

	t_widget.text.data.align_x    = text_align_x;
	t_widget.text.data.rect       = rect_box;

	t_widget.text.font = font;

	if (s_data.length)
		String_Append(&t_widget.text.s_data, s_data);

	t_widget.trigger_autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateButton(
	Window *window,
	Font *font,
	Rect rect_box,
	String s_data
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

	if (s_data.length)
		String_Append(&t_widget.text.s_data, s_data);

	t_widget.trigger_autosize = true;

	return t_widget;
}

instant Widget
Widget_CreateListBox(
	Window *window,
	Font *font,
	Rect rect_box
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

	t_widget.data.is_focusable = true;
	t_widget.data.has_scrollable_list = true;

	t_widget.text.data.rect = rect_box;

	t_widget.text.font = font;

	return t_widget;
}

instant Widget
Widget_CreateCheckBox(
	Window *window,
	Font *font,
	Rect rect_box,
	String s_data,
	bool checked
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

	t_widget.data.is_checkable = true;
	t_widget.data.is_focusable = true;
	t_widget.data.border_size  = 2;
	t_widget.data.is_checked   = checked;

	t_widget.text.font = font;

	String_Append(&t_widget.text.s_data, s_data);

	t_widget.text.data.rect_padding = {2, 2, 2, 2};

	t_widget.trigger_autosize = true;

	return t_widget;
}

instant Widget
Widget_CreatePictureBox(
	Window *window,
	Rect rect_box,
	Texture *texture = 0
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

	t_widget.data.is_focusable = false;

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

    t_widget.data.is_focusable = false;

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

				t_slide->value = ToInt(&tw_label->text.s_data);

				if (sub_index == 1) {
					if (t_slide->value - t_slide->step >= t_slide->start)
						t_slide->value -= t_slide->step;
				}
				else
				if (sub_index == 2) {
					if (t_slide->value + t_slide->step <= t_slide->end)
						t_slide->value += t_slide->step;
				}

				char *c_value = ToCString(t_slide->value);

				String_Clear(&tw_label->text.s_data);
				String_Append(&tw_label->text.s_data, S(c_value));

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

	Vertex *t_vertex = &widget_io->vertex_rect_static;
	Rect    rect_box =  widget_io->layout_data.settings.rect;

	if (!t_vertex->array_id) Vertex_Create(t_vertex);
	else                     Vertex_ClearAttributes(t_vertex);

	if (widget_io->data.has_focus)
		Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline_selected);
	else
		Vertex_AddRect32(t_vertex, rect_box, widget_io->data.color_outline);

	Rect_Resize(&rect_box, -2);
	Vertex_AddRect32(t_vertex, rect_box, {1, 1, 1, 1});

	widget_io->text.data.rect = widget_io->layout_data.settings.rect;
	widget_io->text.data.rect.x += 1;
	widget_io->text.data.rect.h -= 3;
}

instant Widget
Widget_CreateNumberPicker(
	Window *window,
	Font *font,
	Rect rect_box,
	Widget_Slide slide
) {
	Widget t_widget = {};

    t_widget.type = WIDGET_NUMBERPICKER;
    t_widget.window = window;
    t_widget.rect_content = rect_box;
    t_widget.slide = slide;

    t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.data.is_focusable = false;

	Widget wg_label       = Widget_CreateLabel( window, font, {0, 0, 50, 24}, {});
	Widget wg_button_up   = Widget_CreateButton(window, font, {0, 0, 24, 24}, S("<"));
	Widget wg_button_down = Widget_CreateButton(window, font, {0, 0, 24, 24}, S(">"));
	Widget wg_spreader    = Widget_CreateSpreader(window);

	wg_button_up.trigger_autosize   = false;
	wg_button_down.trigger_autosize = false;

	wg_button_up.OwnerDraw   = Widget_RedrawNumberPickerButton;
	wg_button_down.OwnerDraw = Widget_RedrawNumberPickerButton;
	wg_button_up.UpdateCustomInputs   = Widget_UpdateInputNumberPicker;
	wg_button_down.UpdateCustomInputs = Widget_UpdateInputNumberPicker;

	wg_label.text.data.align_x = TEXT_ALIGN_X_RIGHT;
	wg_label.text.data.rect_padding = {2, 2, 2, 2};

	char *c_value = ToCString(slide.value);
	String_Append(&wg_label.text.s_data, S(c_value));
	Memory_Free(c_value);

	Array_Add(&t_widget.a_subwidgets, wg_label);
	Array_Add(&t_widget.a_subwidgets, wg_button_up);
	Array_Add(&t_widget.a_subwidgets, wg_button_down);
	Array_Add(&t_widget.a_subwidgets, wg_spreader);

	return t_widget;
}

instant Widget
Widget_CreateTextBox(
	Window *window,
	Font *font,
	Rect rect_box
) {
	Widget t_widget = {};

	t_widget.text.data.rect_padding = {1, 1, 1, 1};

	Rect *rect_padding = &t_widget.text.data.rect_padding;

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font);
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_TEXTBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.data.is_scrollable = true;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_height = true;
	t_widget.layout_data.settings.auto_width  = true;

	t_widget.text.font = font;

	t_widget.text.data.rect = rect_box;
	t_widget.text.data.is_editable = true;

	t_widget.text.allow_tab_input = true;

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
	widget->trigger_popout = true;
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
					Widget_SetFocus(wg_list);
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

					if (!String_IsEqual(wg_text->text.s_data, s_row_data)) {
						String_Clear(&wg_text->text.s_data);
						String_Append(&wg_text->text.s_data, s_row_data);
						wg_text->events.on_text_change = true;

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

instant Widget
Widget_CreateComboBox(
	Window *window,
	Font *font,
	Rect rect_box,
	s32 combo_height
) {
	Widget t_widget = {};

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font);
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_COMBOBOX;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;

	t_widget.data.is_focusable = false;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width = true;

	Widget wg_text   = Widget_CreateTextBox(window, font, {});
	Widget wg_button = Widget_CreateButton( window, font, {}, S("+"));
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

	Widget *twg_text = Array_Add(&t_widget.a_subwidgets, wg_text);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_TEXT);

	Widget *twg_button = Array_Add(&t_widget.a_subwidgets, wg_button);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_TOGGLE);

	Widget *twg_list   = Array_Add(&t_widget.a_subwidgets, wg_list);
	Assert(t_widget.a_subwidgets.count - 1 == WIDGET_COMBOBOX_LIST);

	twg_list->widget_focus_on_popout = twg_text;

	return t_widget;
}

instant Widget
Widget_CreateProgressbar(
	Window *window,
	Font *font,
	Rect rect_box,
	Widget_Slide slide
) {
	Widget t_widget = {};

	if (!rect_box.w) {
		rect_box.w = font->size;
		Widget_AddBorderSizes(&t_widget, &rect_box.w, 0);
	}

	if (!rect_box.h) {
		rect_box.h = Font_GetLineHeight(font) + 2;
		Widget_AddBorderSizes(&t_widget, 0, &rect_box.h);
	}

	t_widget.type         = WIDGET_PROGRESSBAR;
	t_widget.rect_content = rect_box;
	t_widget.window       = window;
	t_widget.slide        = slide;

	t_widget.data.is_focusable = false;

	t_widget.text.font         = font;
	t_widget.text.data.align_x = TEXT_ALIGN_X_MIDDLE;
	t_widget.text.data.align_y = TEXT_ALIGN_Y_CENTER;

	t_widget.layout_data.settings.rect = rect_box;
	t_widget.layout_data.settings.auto_width = true;

	return t_widget;
}
