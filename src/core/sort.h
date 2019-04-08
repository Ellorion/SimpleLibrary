#pragma once

enum SORT_ORDER_TYPE {
	SORT_ORDER_ASCENDING,
	SORT_ORDER_DESCENDING
};

template <typename T>
struct Sort_Data {
	T *begin_io = 0;
	T *end_io   = 0;
	SORT_ORDER_TYPE type = SORT_ORDER_ASCENDING;
	bool (*OnCompare)(T one, T two) = 0;
};

template <typename T>
instant void
Sort_Quick(
	Sort_Data<T> sort_data
) {
	T *begin_io          = sort_data.begin_io;
	T *end_io            = sort_data.end_io;
	SORT_ORDER_TYPE type = sort_data.type;

	Assert(begin_io);
	Assert(end_io);

	if (begin_io >= end_io)
		return;

    T *pivot = begin_io;
    T *next  = begin_io;
	++next;

	if (type == SORT_ORDER_ASCENDING) {
		while(next <= end_io) {
			if (*next < *pivot) {
				Swap(next, pivot);

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

				if (pivot < next) {
					++pivot;
					continue;
				}
			}
			++next;
		}
	}
	else {
		Assert(false);
	}

    if (begin_io < pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.end_io       = pivot - 1;

		Sort_Quick(t_sort_data);
    }

	if (end_io > pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.begin_io     = pivot + 1;

		Sort_Quick(t_sort_data);
	}
}

template <typename T>
instant void
Sort_Quick_Custom(
	Sort_Data<T> sort_data
) {
	T *begin_io = sort_data.begin_io;
	T *end_io   = sort_data.end_io;

	Assert(begin_io);
	Assert(end_io);
	Assert(sort_data.OnCompare);

	if (begin_io >= end_io)
		return;

    T *pivot = begin_io;
    T *next  = begin_io;
	++next;

	while(next <= end_io) {
		if (sort_data.OnCompare(*next, *pivot)) {
			Swap(next, pivot);

			if (pivot < next) {
				++pivot;
				continue;
			}
		}
		++next;
	}

    if (begin_io < pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.end_io       = pivot - 1;

		Sort_Quick_Custom(t_sort_data);
    }

	if (end_io > pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.begin_io     = pivot + 1;

		Sort_Quick_Custom(t_sort_data);
	}
}
