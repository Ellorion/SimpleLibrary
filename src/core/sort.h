#pragma once

enum SORT_ORDER_TYPE {
	SORT_ORDER_ASCENDING,
	SORT_ORDER_DESCENDING
};

template <typename T>
instant void
Sort_Quick(
	T *begin_io,
	T *end_io,
	SORT_ORDER_TYPE type
) {
	Assert(begin_io);
	Assert(end_io);

	if (begin_io == end_io)
		return;

    T *pivot = begin_io;
    T *next  = begin_io;
	++next;

	if (type == SORT_ORDER_ASCENDING) {
		while(next <= end_io) {
			if (*next < *pivot) {
				Swap(next, pivot);

				/// next will be past pivot in next loop
				/// pivot will follow next, but never catches up
				if (pivot < next) {
					++pivot;
					continue;
				}
			}
			++next;
		}
	}
	else
	if (type == SORT_ORDER_DESCENDING) {
		while(next <= end_io) {
			if (*next > *pivot) {
				Swap(next, pivot);

				/// next will be past pivot in next loop
				/// pivot will follow next, but never catches up
				if (pivot < next) {
					++pivot;
					continue;
				}
			}
			++next;
		}
	}
	else {
		AssertMessage(false, "[Sort] Unknown sort type.");
	}

	/// advance pivot, if nothing was swapped
	/// -> early out for already sorted data
	if (pivot == begin_io) {
		if (type == SORT_ORDER_ASCENDING) {
			while(pivot < end_io) {
				if (*pivot > *(pivot + 1))
					break;

				++pivot;
			}
		}
		else
		if (type == SORT_ORDER_DESCENDING) {
			while(pivot < end_io) {
				if (*pivot < *(pivot + 1))
					break;

				++pivot;
			}
		}

		if (pivot == end_io)
			return;

		Sort_Quick(pivot, end_io, type);
		return;
	}

    if (begin_io < pivot)  Sort_Quick(begin_io	, pivot - 1, type);
	if (end_io   > pivot)  Sort_Quick(pivot + 1	, end_io,	 type);
}
