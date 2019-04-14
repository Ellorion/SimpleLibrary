#pragma once

struct Rect {
	float x = 0.0f;
	float y = 0.0f;
	s32 w = 0;
	s32 h = 0;
};

struct RectF {
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
};

struct RectI {
	s32 x = 0;
	s32 y = 0;
	s32 w = 0;
	s32 h = 0;
};

instant void
Rect_GetAspect(
	s32 width_src,
	s32 height_src,
	s32 *width_dst,
	s32 *height_dst,
	float *off_x_io,
	float *off_y_io,
	float *ratio_x_out = 0,
	float *ratio_y_out = 0,
	bool scale_to_dest = false
) {
	Assert(width_dst);
	Assert(height_dst);

	float aspect_src = (float) width_src /  height_src;
    float aspect_dst = (float)*width_dst / *height_dst;

	if (aspect_dst > aspect_src) {
		float ratio = (aspect_dst / aspect_src);
		float offset = *width_dst / ratio;

		if (off_x_io)  *off_x_io += floor((*width_dst - offset) / 2);
		if (ratio_x_out)  *ratio_x_out = ratio;
		if (ratio_y_out)  *ratio_y_out = 1;

		*width_dst = floor(offset);
    }
    else
	if (aspect_dst < aspect_src) {
		float ratio = (aspect_src / aspect_dst);
		float offset = *height_dst / ratio;

		if (off_y_io)  *off_y_io += floor((*height_dst - offset) / 2);
		if (ratio_x_out)  *ratio_x_out = 1;
		if (ratio_y_out)  *ratio_y_out = ratio;

		*height_dst = floor(offset);
    }

    if (scale_to_dest) {
		if (ratio_x_out)  *ratio_x_out = (float)*width_dst  / width_src;
		if (ratio_y_out)  *ratio_y_out = (float)*height_dst / height_src;
    }
}

instant void
Rect_GetAspect(
	Rect *rect_convert_to_dest,
	s32 width_src,
	s32 height_src,
	float *ratio_x_out = 0,
	float *ratio_y_out = 0,
	bool scale_to_dest = false
) {
	Assert(rect_convert_to_dest);

	Rect_GetAspect(	 width_src,
					 height_src,
					&rect_convert_to_dest->w,
					&rect_convert_to_dest->h,
					&rect_convert_to_dest->x,
					&rect_convert_to_dest->y,
					ratio_x_out,
					ratio_y_out,
					scale_to_dest);
}

instant bool
Rect_IsIntersecting(
	Rect *rect_inner,
	Rect *rect_outer,
	Point pt_inner_offset
) {
	Assert(rect_inner);
	Assert(rect_outer);

	if (rect_inner->x + pt_inner_offset.x + rect_inner->w < rect_outer->x)  return false;
	if (rect_inner->x + pt_inner_offset.x > rect_outer->x + rect_outer->w)  return false;

	if (rect_inner->y + pt_inner_offset.y + rect_inner->h < rect_outer->y)  return false;
	if (rect_inner->y + pt_inner_offset.y > rect_outer->y + rect_outer->h)  return false;

	return true;
}

instant bool
Rect_IsIntersecting(
	Rect *rect_inner,
	Rect *rect_outer
) {
	Assert(rect_inner);
	Assert(rect_outer);

	return Rect_IsIntersecting(rect_inner, rect_outer, {0, 0});
}

instant bool
Rect_IsIntersecting(
	Point *point,
	Rect  *rect
) {
	Assert(point);
	Assert(rect);

	if (point->x < rect->x)  return false;
	if (point->y < rect->y)  return false;

	if (point->x > rect->x + rect->w)  return false;
	if (point->y > rect->y + rect->h)  return false;

	return true;
}

instant bool
Rect_IsIntersectingBorderless(
	Point *point,
	Rect  *rect
) {
	Assert(point);
	Assert(rect);

	if (point->x < rect->x)  return false;
	if (point->y < rect->y)  return false;

	if (point->x >= rect->x + rect->w)  return false;
	if (point->y >= rect->y + rect->h)  return false;

	return true;
}

instant bool
Rect_IsVisibleFully(
	Rect *rect_inner,
	Rect *rect_outer
) {
	Assert(rect_inner);
	Assert(rect_outer);


	if (rect_inner->x < rect_outer->x)  return false;
	if (rect_inner->y < rect_outer->y)  return false;

	if (rect_inner->x + rect_inner->w > rect_outer->x + rect_outer->w)  return false;
	if (rect_inner->y + rect_inner->h > rect_outer->y + rect_outer->h)  return false;

	return true;
}

instant void
Rect_Resize(
	Rect *rect_io,
	s32 pixel_offset
) {
	Assert(rect_io);

	rect_io->x -= pixel_offset;
	rect_io->y -= pixel_offset;
	rect_io->w += (pixel_offset << 1);
	rect_io->h += (pixel_offset << 1);
}

instant void
Rect_AddPadding(
	Rect *rect_io,
	Rect rect_padding
) {
	Assert(rect_io);

	rect_io->x += rect_padding.x;
	rect_io->y += rect_padding.y;
	rect_io->w -= (rect_padding.x + rect_padding.w);
	rect_io->h -= (rect_padding.y + rect_padding.h);
}

instant void
Rect_ClampY(
	float *y_io,
	s32 content_height,
	s32 visible_height
) {
	Assert(y_io);

	/// do not show negative space above the first visible
	if (*y_io > 0)
		*y_io = 0;

	/// do not show negative space below the last visible
	if (*y_io + content_height < visible_height)
		*y_io = visible_height - content_height;

	/// avoid scrolling, when there is nothing to scroll
	if (content_height <= visible_height)
		*y_io = 0;
}

instant void
Rect_ClampY(
	Rect *rect_io,
	Rect  rect_limit
) {
	Assert(rect_io);

	/// do not show negative space below the last visible
	if (rect_io->y + rect_io->h < rect_limit.h)
		rect_io->y = rect_limit.h - rect_io->h;

	/// do not show negative space above the first visible
	if (rect_io->y > 0)
		rect_io->y = 0;

	/// avoid scrolling, when all content is visible
	if (rect_io->h <= rect_limit.h)
		rect_io->y = 0;
}

/// ::: Operator
/// ===========================================================================
bool
operator != (
	Rect &r1,
	Rect &r2
) {
	if (r1.x != r2.x)  return true;
	if (r1.y != r2.y)  return true;
	if (r1.w != r2.w)  return true;
	if (r1.h != r2.h)  return true;

	return false;
}

bool
operator == (
	RectI &r1,
	RectI &r2
) {
	if (r1.x != r2.x)  return false;
	if (r1.y != r2.y)  return false;
	if (r1.w != r2.w)  return false;
	if (r1.h != r2.h)  return false;

	return true;
}
