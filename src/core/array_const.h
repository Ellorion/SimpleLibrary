#pragma once

template <typename T, int Count>
struct Array_Const {
	T     memory[Count];
	u64   count  = 0;
};

template <typename T, int Count>
constexpr instant T *
Array_Add(
	Array_Const<T, Count> *array,
	T element
) {
	Assert(array);

	constexpr u64 length = 1;

	if (array->count + 1 > Count) {
        return nullptr;
	}

	u64 target = array->count; /// "convert" to index
	++array->count;

	array->memory[target] = element;

	return &array->memory[target];
}

template <typename T, int Count>
constexpr instant u64
Array_AddEmpty(
	Array_Const<T, Count> *array,
	T **element_empty_out
) {
	Assert(array);
	Assert(element_empty_out);

	T t_element_empty = {};
	*element_empty_out = Array_Add(array, t_element_empty);

	return array->count - 1;
}

template <typename T, int Count>
constexpr instant void
Array_ClearContainer(
	Array_Const<T, Count> *array_out
) {
	Assert(array_out);

	array_out->count = 0;
}

template <typename T = String, int Count>
instant void
Array_PrintList(
	Array_Const<T, Count> *a_data
) {
	Assert(a_data);

	FOR_ARRAY(*a_data, it) {
		String t_data = ARRAY_IT(*a_data, it);

		String_PrintLine(t_data);
	}
}
