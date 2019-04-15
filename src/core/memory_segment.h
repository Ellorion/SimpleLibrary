#pragma once

struct Memory_Segment {
	void *memory = 0;
	u64   size   = 0;
};

instant void
Memory_AddSegment(
	Array<Memory_Segment> *a_segments,
	Memory_Segment segment
) {
	Assert(a_segments);

	Array_Add(a_segments, segment);
}

instant void
Memory_ResetSegments(
	Array<Memory_Segment> *a_segments
) {
	Assert(a_segments);

	FOR_ARRAY(*a_segments, it) {
		Memory_Segment *t_segment = &ARRAY_IT(*a_segments, it);
		Memory_Set(t_segment->memory, 0, t_segment->size);
	}
}

#define Memory_AddSegment(_a_segments, _object) \
	Memory_AddSegment(_a_segments, {&_object, sizeof(_object)});
