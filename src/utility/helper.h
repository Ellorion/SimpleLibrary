#pragma once

template <typename T, typename Func>
instant void
Array_AddRange(
	Array<T> *a_data_io,
	u64 min,
	u64 max,
	Func OnAssignData
) {
	Assert(a_data_io);

	u64 t_min = MIN(min, max);
	u64 t_max = MAX(min, max);

	u64 t_delta = t_max - t_min;

	FOR_START(t_min, t_max, it) {
		T data;
		OnAssignData(&data, it);
		Array_Add(a_data_io, data);
	}
}

template <typename T>
instant void
Array_Fill(
	Array<T> *a_data_io,
	u64 count
) {
	Assert(a_data_io);

	FOR(count, it)
		Array_Add(a_data_io, it);
}

template <typename T>
instant void
Array_FillTest(
	Array<T> *a_data,
	SORT_ORDER_TYPE type
) {
	Assert(a_data);

	if (type == SORT_ORDER_ASCENDING) {
		FOR_ARRAY(*a_data, it) {
			T value = ARRAY_IT(*a_data, it);
			Assert(value == it);
		}
	}
	else
	if (type == SORT_ORDER_DESCENDING) {
		FOR_ARRAY(*a_data, it) {
			T value = ARRAY_IT(*a_data, it);
			Assert(value == (a_data->count - (it+1)));
		}
	}
}

template <typename T>
instant void
Array_SortTest(
	Array<T> *a_data,
	SORT_ORDER_TYPE type
) {
	Assert(a_data);
	Assert(a_data->count > 1);

	T t_prev = ARRAY_IT(*a_data, 0);

	if (type == SORT_ORDER_ASCENDING) {
		FOR_ARRAY_START(*a_data, 1, it) {
			T value = ARRAY_IT(*a_data, it);
			Assert(value >= t_prev);
			t_prev = value;
		}
	}
	else
	if (type == SORT_ORDER_DESCENDING) {
		FOR_ARRAY_START(*a_data, 1, it) {
			T value = ARRAY_IT(*a_data, it);
			Assert(value <= t_prev);
			t_prev = value;
		}
	}
}

template <typename T>
instant void
Array_Sort_Ascending(
	Array<T> *array_io
) {
	Assert(array_io);

	if(!array_io->count)
		return;

	/// @Rant: noticably faster than the below lambda version,
	///        which would be even worst without force-inlining
	Array_Sort(array_io, SORT_ORDER_ASCENDING);

	///	Array_Sort(array_io, [](T one, T two) __forceinline -> bool {
	///		return (one < two);
	///	});
}

template <typename T>
instant void
Array_Sort_Descending(
	Array<T> *array_io
) {
	Assert(array_io);

	if(!array_io->count)
		return;

	/// @Rant: noticably faster than the below lambda version,
	///        which would be even worst without force-inlining
	Array_Sort(array_io, SORT_ORDER_DESCENDING);

	///	Array_Sort(array_io, [](T one, T two) __forceinline -> {
	///		return (one > two);
	///	});
}
