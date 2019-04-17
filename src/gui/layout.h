#pragma once

#define LAYOUT_BLOCK_PADDING 4
#define LAYOUT_BLOCK_SPACING 2

#define LAYOUT_PADDING 5

enum LAYOUT_TYPE {
	LAYOUT_TYPE_X,
	LAYOUT_TYPE_Y
};

enum LAYOUT_DOCK_TYPE {
	LAYOUT_DOCK_TOPLEFT,
	LAYOUT_DOCK_BOTTOMRIGHT
};

enum LAYOUT_SECTION_TYPE {
	LAYOUT_SECTION_TOP,
	LAYOUT_SECTION_RIGHT,
	LAYOUT_SECTION_BOTTOM,
	LAYOUT_SECTION_LEFT,
};

struct Layout_Data_Settings {
	Rect rect = {};
	bool auto_width  = false;
	bool auto_height = false;
};

struct Layout_Data {
	Layout_Data_Settings settings;
	Layout_Data_Settings settings_prev;
};

struct Widget;

struct Layout_Block {
	LAYOUT_TYPE type;
	LAYOUT_DOCK_TYPE dock;
	s32 expand_index = -1;
	u32 padding = LAYOUT_BLOCK_PADDING;
	u32 spacing = LAYOUT_BLOCK_SPACING;
	bool is_visible = true;
	Array<Layout_Data *> ap_layout_data;

	/// convenience
	/// - to change widgets data inside a block
	Array<Widget *> ap_widgets;
};

struct Layout;

/// @todo: float size and allow % based for resizing
struct Layout_Section {
	bool is_in_section = false;
	LAYOUT_SECTION_TYPE type;
	s64 size;
	Rect rect_remaining;
};

struct Layout {
	Rect rect_full;
	Rect rect_remaining;
	bool fill_last_block = true;
	u32  padding = LAYOUT_PADDING;
	Layout_Section section;

	Array<Layout_Block> a_layout_blocks;

	/// convenience
	/// -> to be rendered items for this layout
	Array<Widget *> ap_widgets;

	Array<Layout> a_sublayouts;
};

instant void
Layout_Create(
	Layout *layout_out,
	Rect rect_area,
	bool fill_last_block
) {
	Assert(layout_out);
	AssertMessage(!layout_out->a_layout_blocks.count, "[Layout] Layout already created.");

	*layout_out = {};
	layout_out->rect_full       = rect_area;
	layout_out->rect_remaining  = rect_area;
	layout_out->fill_last_block = fill_last_block;
}

instant void
Layout_Create(
	Layout *layout_out,
	Window *window,
	bool fill_last_block
) {
	Assert(window);
	Assert(layout_out);

	Layout_Create(layout_out, {0, 0, window->width, window->height}, fill_last_block);
}

instant void
Layout_Create(
	Layout *layout_out,
	Layout_Data *layout_data,
	bool fill_last_block
) {
	Assert(layout_out);
	Assert(layout_data);

	Rect rect_area = layout_data->settings.rect;

	Layout_Create(layout_out, rect_area, fill_last_block);
}

instant u64
Layout_CreateBlock(
	Layout *layout_io,
	LAYOUT_TYPE type,
	LAYOUT_DOCK_TYPE dock_direction,
	s32 expand_index = -1
) {
	Assert(layout_io);

	Layout_Block *t_block;

	Array_AddEmpty(&layout_io->a_layout_blocks, &t_block);

	t_block->type = type;
	t_block->dock = dock_direction;
	t_block->expand_index = expand_index;

	return layout_io->a_layout_blocks.count - 1;
}

instant bool
Layout_GetLastBlock(
	Layout *layout,
	Layout_Block **layout_block_out
) {
	Assert(layout);

	if (!layout->a_layout_blocks.count) {
		LOG_INFO("GetLastBlock: No blocks in layout found.");
		return false;
	}

	*layout_block_out = &ARRAY_IT(layout->a_layout_blocks, layout->a_layout_blocks.count - 1);

	return true;
}

instant bool
Layout_GetBlock(
	Layout *layout,
	u64 index,
	Layout_Block **layout_block_out
) {
	Assert(layout);

	if (!layout->a_layout_blocks.count) {
		LOG_INFO("GetBlock: No blocks in layout found.");
		return false;
	}

	Assert(index < layout->a_layout_blocks.count);
	*layout_block_out = &ARRAY_IT(layout->a_layout_blocks, index);

	return true;
}

struct Widget;

instant void Layout_Add(Layout *layout_io, Widget *widget);

instant void
Layout_Add(
	Layout *layout_io,
	Layout_Data *layout_data
) {
	Assert(layout_io);
	Assert(layout_data);

	Layout_Block *current_block = 0;

	Layout_GetLastBlock(layout_io, &current_block);
	Array_Add(&current_block->ap_layout_data, layout_data);
}

/// layout_block->expand_index (widgets):
///  0-max: expand widget with matching index
///     -1: balanced size (with auto-size)
///     -2: left-align (no auto-size)
///     -3: center-align (no auto-size)
///     -4: right-align (no auto-size)
///   else: see -1
instant void
Layout_ArrangeBlockX(
	Layout *layout_io,
	Layout_Block *layout_block_io
) {
	Assert(layout_io);
	Assert(layout_block_io);
	Assert(layout_block_io->type == LAYOUT_TYPE_X);

	Layout_Block *t_block_last = 0;
	Layout_GetLastBlock(layout_io, &t_block_last);

	bool is_last_block = (t_block_last == layout_block_io);

	s64 widget_count = layout_block_io->ap_layout_data.count;

	if (!widget_count)
		return;

	s32 padding_size = layout_block_io->padding;

	if (padding_size % 2 != 0)  ++padding_size;

	s32 padding_border   = padding_size >> 1;
	s32 width_remaining  = layout_io->rect_remaining.w;

	float it_x = padding_border;
	float it_y = padding_border;

	bool is_overwriting = (	    layout_block_io->expand_index >= -4
							AND layout_block_io->expand_index != -1
							AND layout_block_io->expand_index < widget_count);

	s32 block_height = 0;
	u64 widget_count_auto = widget_count;

	/// pre-calc height for bottom alignment
	FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		block_height  = MAX(block_height, rect->h);

		if (is_overwriting) {
			if (!found_expander) {
				width_remaining -= rect->w;
				--widget_count_auto;
			}
		}
		else
		if (!t_data->settings.auto_width) {
			width_remaining -= rect->w;
			--widget_count_auto;
		}
	}

    width_remaining -= padding_size;
	width_remaining -= layout_block_io->spacing * (widget_count - 1);

    block_height += padding_size;

    switch (layout_block_io->expand_index) {
    	case -3: {
    		it_x = (width_remaining + padding_border) >> 1;
    	} break;

    	case -4: {
    		it_x = (width_remaining + padding_border);
		} break;

		default: break;
    }

    /// start drawing at the bottom
	if (layout_block_io->dock == LAYOUT_DOCK_BOTTOMRIGHT) {
		if (!is_last_block OR (is_last_block AND !layout_io->fill_last_block)) {
			it_y += layout_io->rect_remaining.h - block_height;
		}
	}

	float width_avg_auto = 0;

	if (widget_count_auto)
		width_avg_auto = ceil((float)width_remaining / widget_count_auto);

	/// align horizontal
    FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		/// center widgets in block
		s32 center_block = (rect->h - (block_height - padding_size)) >> 1;

		rect->x = layout_io->rect_remaining.x + it_x;
		rect->y = layout_io->rect_remaining.y + it_y - center_block;

		if (layout_io->fill_last_block AND is_last_block)
			rect->h = layout_io->rect_remaining.h - padding_size;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		float x_step = rect->w;

		if (is_overwriting) {
			if (found_expander)
				x_step = width_avg_auto;
		}
		else {
			if (t_data->settings.auto_width)
				x_step = width_avg_auto;
		}

		float width_limit = layout_io->rect_remaining.w - it_x;

		if (x_step > width_limit)
			x_step = width_limit;

		rect->w = x_step;
		it_x += x_step + layout_block_io->spacing;
    }

    /// cut of the top
	if (layout_block_io->dock == LAYOUT_DOCK_TOPLEFT)
		layout_io->rect_remaining.y += block_height;

    layout_io->rect_remaining.h -= block_height;
}

/// layout_block->expand_index (widgets):
///  0-max: expand widget with matching index
///     -1: balanced size (with auto-size)
///     -2: top-align (no auto-size)
///     -3: middle-align (no auto-size)
///     -4: bottom-align (no auto-size)
///   else: see -1
instant void
Layout_ArrangeBlockY(
	Layout *layout_io,
	Layout_Block *layout_block_io
) {
	Assert(layout_io);
	Assert(layout_block_io);
	Assert(layout_block_io->type == LAYOUT_TYPE_Y);

	Layout_Block *t_block_last = 0;
	Layout_GetLastBlock(layout_io, &t_block_last);

	bool is_last_block = (t_block_last == layout_block_io);

	s64 widget_count = layout_block_io->ap_layout_data.count;

	if (!widget_count)
		return;

	s32 padding_size     = layout_block_io->padding;

	if (padding_size % 2 != 0)  ++padding_size;

	s32 padding_border   = padding_size >> 1;
	s32 height_remaining = layout_io->rect_remaining.h;

	float it_x = padding_border;
	float it_y = padding_border;

	bool is_overwriting = (	    layout_block_io->expand_index >= -4
							AND layout_block_io->expand_index != -1
							AND layout_block_io->expand_index < widget_count);

	s32 block_width = 0;
	u64 widget_count_auto = widget_count;

	/// pre-calc width for bottom alignment
	FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		block_width = MAX(block_width, rect->w);

		if (is_overwriting) {
			if (!found_expander) {
				height_remaining -= rect->h;
				--widget_count_auto;
			}
		}
		else
		if (!t_data->settings.auto_height) {
			height_remaining -= rect->h;
			--widget_count_auto;
		}
	}

	height_remaining -= padding_size;
	height_remaining -= layout_block_io->spacing * (widget_count - 1);

    block_width += padding_size;

    switch (layout_block_io->expand_index) {
    	case -3: {
    		it_y = (height_remaining + padding_border) >> 1;
    	} break;

    	case -4: {
    		it_y = (height_remaining + padding_border);
    	} break;

		default: break;
    }

    /// start drawing right
	if (layout_block_io->dock == LAYOUT_DOCK_BOTTOMRIGHT) {
		if (!is_last_block OR (is_last_block AND !layout_io->fill_last_block)) {
			it_x += layout_io->rect_remaining.w - block_width;
		}
	}

	float height_avg_auto = 0;

	if (widget_count_auto)
		height_avg_auto = ceil((float)height_remaining / widget_count_auto);

	/// align horizontal
    FOR_ARRAY(layout_block_io->ap_layout_data, it_block) {
		Layout_Data *t_data = ARRAY_IT(layout_block_io->ap_layout_data, it_block);

		Rect *rect = &t_data->settings.rect;

		/// center widgets in block
		s32 center_block = (rect->w - (block_width - padding_size)) >> 1;

		rect->x = layout_io->rect_remaining.x + it_x - center_block;
		rect->y = layout_io->rect_remaining.y + it_y;

		if (layout_io->fill_last_block AND is_last_block)
			rect->w = layout_io->rect_remaining.w - padding_size;

		bool found_expander = ((s64)it_block == layout_block_io->expand_index);

		float y_step = rect->h;

		if (is_overwriting) {
			if (found_expander)
				y_step = height_avg_auto;
		}
		else {
			if (t_data->settings.auto_height)
				y_step = height_avg_auto;
		}

		float height_limit = layout_io->rect_remaining.h - it_y;

		if (y_step > height_limit)
			y_step = height_limit;

		rect->h = y_step;
		it_y += y_step + layout_block_io->spacing;
    }

    /// cut of the left
	if (layout_block_io->dock == LAYOUT_DOCK_TOPLEFT)
		layout_io->rect_remaining.x += block_width;

    layout_io->rect_remaining.w -= block_width;
}

instant void
Layout_ApplySection(
	Layout *layout_io
) {
	Assert(layout_io);

	if (!layout_io->section.is_in_section)
		return;

	Rect rect_remaining = layout_io->rect_full;
	s64 size = layout_io->section.size;

	if (size) {

		/// @Note Padding of the current layout_io
		///       and not the following layout_io.
		///       May end up being confusing,
		///       time will tell...
		u32 padding = (layout_io->padding * 2);

		/// type -> move towards
		switch (layout_io->section.type) {
			case LAYOUT_SECTION_TOP: {
				if (size > 0) {
					layout_io->rect_full.h = size;

					rect_remaining.y += size - padding;
					rect_remaining.h -= size - padding;
				}
				else {
					size = -size;

					rect_remaining.y = (layout_io->rect_full.y + layout_io->rect_full.h) - size;
					rect_remaining.h = size;

					layout_io->rect_full.h -= size - padding;
				}
			} break;

			case LAYOUT_SECTION_RIGHT: {
				if (size > 0) {
					layout_io->rect_full.x = (layout_io->rect_full.x + layout_io->rect_full.w) - size;
					layout_io->rect_full.w = size;

					rect_remaining.w -= size - padding;
				}
				else {
					size = -size;

					layout_io->rect_full.x = (layout_io->rect_full.x + layout_io->rect_full.w) - size;
					layout_io->rect_full.w = size;

					rect_remaining.w -= size - padding;
				}
			} break;

			case LAYOUT_SECTION_BOTTOM: {
				if (size > 0) {
					layout_io->rect_full.y = (layout_io->rect_full.y + layout_io->rect_full.h) - size;
					layout_io->rect_full.h = size;

					rect_remaining.h -= size - padding;
				}
				else {
					size = -size;

					rect_remaining.h = size;

					layout_io->rect_full.y += size - padding;
					layout_io->rect_full.h -= size - padding;
				}
			} break;

			case LAYOUT_SECTION_LEFT: {
				if (size > 0) {
					layout_io->rect_full.w = size;

					rect_remaining.x += size - padding;
					rect_remaining.w -= size - padding;
				}
				else {
					size = -size;

					rect_remaining.x = (layout_io->rect_full.x + layout_io->rect_full.w) - size;
					rect_remaining.w = size;

					layout_io->rect_full.w -= size - padding;
				}
			} break;
		}
	}

	layout_io->section.rect_remaining = rect_remaining;
}

instant void
Layout_Arrange(
	Layout *layout_io
) {
	Assert(layout_io);

	Layout_ApplySection(layout_io);

	if (!layout_io->rect_full.w)  return;
	if (!layout_io->rect_full.h)  return;

	layout_io->rect_remaining = layout_io->rect_full;

	if (layout_io->padding % 2 != 0) ++layout_io->padding;

	Rect_AddPadding(&layout_io->rect_remaining, {(float)layout_io->padding,
                                                 (float)layout_io->padding,
                                                 (s32)layout_io->padding,
                                                 (s32)layout_io->padding});

	FOR_ARRAY(layout_io->a_layout_blocks, it) {
		Layout_Block *t_block = &ARRAY_IT(layout_io->a_layout_blocks, it);

		if (!t_block->is_visible)
			continue;

		if (t_block->type == LAYOUT_TYPE_X)
			Layout_ArrangeBlockX(layout_io, t_block);
		else
		if (t_block->type == LAYOUT_TYPE_Y)
			Layout_ArrangeBlockY(layout_io, t_block);
	}

	FOR_ARRAY(layout_io->a_sublayouts, it) {
		Layout *t_sublayout = &ARRAY_IT(layout_io->a_sublayouts, it);

		t_sublayout->rect_full = layout_io->section.rect_remaining;
		Layout_Arrange(t_sublayout);
	}
}


instant void
Layout_Rearrange(
	Layout *layout_io,
	Rect rect_resize
) {
	Assert(layout_io);

	/// always arrange, since the elements in the layout
	/// could have changed while the layout size has
	/// stayed the same
	layout_io->rect_full       = rect_resize;
	layout_io->rect_remaining  = rect_resize;

	Layout_Arrange(layout_io);
}

instant void
Layout_Rearrange(
	Layout *layout_io,
	Window *window
) {
	Assert(layout_io);
	Assert(window);

	Layout_Rearrange(layout_io, {0, 0, window->width, window->height});
}

///@Note: do not use this when tab-order is important.
///       instead create an own array which will be
///       in the tab-stop order
instant Array<Widget *>
Layout_GetWidgetArray(
	Layout *layout
) {
	Assert(layout);

	return layout->ap_widgets;
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

instant void
Layout_SplitSection(
	Layout  *layout_io,
	Layout **layout_remaining_out,
	LAYOUT_SECTION_TYPE type,
	s64 size
) {
	Assert(layout_io);
	Assert(layout_remaining_out);

	layout_io->section.is_in_section = true;
	layout_io->section.type = type;
	layout_io->section.size = size;

	Layout *t_layout;
	Array_AddEmpty(&layout_io->a_sublayouts, &t_layout);

	/// rect_full will be updated with remaining size during arrangment
	Layout_Create(t_layout, {0, 0, 0, 0}, layout_io->fill_last_block);

	*layout_remaining_out = t_layout;
}
