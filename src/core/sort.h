#pragma once

enum SORT_ORDER_TYPE {
	SORT_ORDER_ASCENDING,
	SORT_ORDER_DESCENDING
};

template<typename T>
s32 Sort_OnCompareAscending(T one, T two) {
	if (one < two)  return -1;
	if (one > two)  return  1;

	return 0;
}

template<typename T>
s32 Sort_OnCompareDescending(T one, T two) {
	if (one < two)  return  1;
	if (one > two)  return -1;

	return 0;
}

template <typename T>
struct Sort_Data {
	T *begin_io = 0;
	T *end_io   = 0;
	s32 (*OnCompare)(T one, T two) = 0;
};

template <typename T>
instant void
Sort_Quick(
	Sort_Data<T> sort_data
) {
	T *left  = sort_data.begin_io;
	T *right = sort_data.end_io;

	/// middle of the continuous memory
	T  pivot = *(left + ((right - left) >> 1));

	while(left <= right) {
        while(sort_data.OnCompare(*left , pivot) < 0)
			++left;

		while(sort_data.OnCompare(*right, pivot) > 0)
			--right;

		if (left <= right) {
			SWAP(T, left, right);

            ++left;
            --right;
		}
	}

	/// "left" and "right" switched sides at ths point
	if (sort_data.begin_io < right) {
		Sort_Data<T> t_sort_data;
		t_sort_data.begin_io  = sort_data.begin_io;
		t_sort_data.end_io    = right;
		t_sort_data.OnCompare = sort_data.OnCompare;

		Sort_Quick(t_sort_data);
	}

	if (left < sort_data.end_io) {
		Sort_Data<T> t_sort_data;
		t_sort_data.begin_io  = left;
		t_sort_data.end_io    = sort_data.end_io;
		t_sort_data.OnCompare = sort_data.OnCompare;

		Sort_Quick(t_sort_data);
	}
}
