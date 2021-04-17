#pragma once

struct MemorySegment {
	void *memory = 0;
	u64   size   = 0;
};

bool
operator == (
	MemorySegment &mem1,
	MemorySegment &mem2
) {
	return mem1.memory == mem2.memory;
}

instant void
MemorySegment_Add(
	Array<MemorySegment> *a_segments,
	MemorySegment segment
) {
	Assert(a_segments);

	Array_AddUnique(*a_segments, segment);
}

instant void
MemorySegment_Reset(
	Array<MemorySegment> *a_segments
) {
	Assert(a_segments);

	FOR_ARRAY(*a_segments, it) {
		MemorySegment *t_segment = &ARRAY_IT(*a_segments, it);
		Memory_Set(t_segment->memory, 0, t_segment->size);
	}
}

#define MemorySegment_Add(_a_segments, _object) \
	MemorySegment_Add(_a_segments, {&_object, sizeof(_object)});
