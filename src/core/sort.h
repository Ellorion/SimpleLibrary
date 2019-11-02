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
Sort_Bubble(
	Sort_Data<T> sort_data
) {
	u64 count = (sort_data.end_io - sort_data.begin_io) + 1;

	if (count <= 1)
		return;

    FOR(count - 1, i) {
		FOR(count - i - 1, j) {
			if (sort_data.OnCompare(
					sort_data.begin_io[j],
					sort_data.begin_io[j+1]
				) > 0
			) {
				SWAP(
					T,
					&sort_data.begin_io[j],
					&sort_data.begin_io[j+1]
				);
			}
		}
    }
}

template <typename T>
instant void
Sort_Insertion(
	Sort_Data<T> sort_data
) {
	u64 count = (sort_data.end_io - sort_data.begin_io) + 1;

	if (count <= 1)
		return;

	T *element = sort_data.begin_io;

    FOR_START(1, count, it) {
        T value = element[it];
		s64 index_find = it - 1;

		while(    index_find >= 0
			  AND (sort_data.OnCompare(element[index_find], value) > 0)
		) {
			element[index_find + 1] = element[index_find];
			--index_find;
		}

		element[index_find + 1] = value;
    }
}

template <typename T>
instant void
Sort_Quick(
	Sort_Data<T> sort_data
) {
	T *left  = sort_data.begin_io;
	T *right = sort_data.end_io;

	/// middle of the continuous memory
	T  pivot = *(left + ((right - left) >> 1));

	if ((right - left) < 30) {
		Sort_Insertion(sort_data);
		return;
	}

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

template <typename T>
instant void
Array_Sort(
	Array<T> *array_io,
	s32 (*OnCompare)(T one, T two)
) {
	Assert(array_io);

	if(!array_io->count)
		return;

	Sort_Data<T> sort_data;
	sort_data.begin_io  = &array_io->memory[0];
	sort_data.end_io    = &array_io->memory[array_io->count - 1];
	sort_data.OnCompare = OnCompare;

	Sort_Quick(sort_data);
}

/// non-custom version
template <typename T>
instant void
Array_Sort(
	Array<T> *array_io,
	SORT_ORDER_TYPE type
) {
	Assert(array_io);

	if(!array_io->count)
		return;

	Sort_Data<T> sort_data;
	sort_data.begin_io  = &array_io->memory[0];
	sort_data.end_io    = &array_io->memory[array_io->count - 1];

	if (type == SORT_ORDER_ASCENDING)
		sort_data.OnCompare = Sort_OnCompareAscending;
	else
	if (type == SORT_ORDER_DESCENDING)
		sort_data.OnCompare = Sort_OnCompareDescending;
	else
		Assert(false);

	Sort_Quick(sort_data);
}
