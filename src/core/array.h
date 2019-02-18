#pragma once

#define ARRAY_IT(_array, _it) \
	((_array).memory)[_it]

#define FOR_ARRAY(_array, _it) 		\
	for(u64 _it = 0;        		\
		_it < (_array).count;       \
		++_it)

#define FOR_ARRAY_REV(_array, _it)	\
	for(s64 _it = (_array).count - 1;	\
		_it >= 0;						\
		--_it)

#define FOR_ARRAY_START(_array, _start, _it) 	\
	for(u64 _it = (_start);   					\
		_it < (_array).count;       			\
		++_it)

#define ARRAY_COUNT(_array) \
	(sizeof(_array)/sizeof(_array[0]))

template <typename T>
struct Array {
	T    *memory = 0;
	u64   count  = 0;
	u64   max    = 0;

	/// f.e. for string chunks
	bool  by_reference = false;
};

///@Info: will copy the struct element pointers only
///
///       if the array should take ownership of all data
///       make sure you copy every data that is connected
///       to a pointer (clone) or don't free the
///       passed / connected data
template <typename T>
instant T *
Array_Add(
	Array<T> *array_io,
	T element
) {
	Assert(array_io);

	constexpr u64 length = 1;

	if (array_io->count + length > array_io->max) {
		array_io->max += length;
		array_io->memory = (T *)_Memory_Resize( array_io->memory,
												array_io->max * sizeof(T));
	}

	array_io->count += length;
	u64 target = array_io->count - 1; /// convert to index

	array_io->memory[target] = element;

	return &array_io->memory[target];
}

///@Hint: only use the output immediately after using this fuction
///       otherwise another call to this function could invalid
///       the prev. output because the base memory pointer could
///       have changed after resizing
template <typename T>
instant u64
Array_AddEmpty(
	Array<T> *array_io,
	T **element_empty_out
) {
	Assert(array_io);
	Assert(element_empty_out);

	T t_element_empty = {};
	Array_Add(array_io, t_element_empty);
	*element_empty_out = &ARRAY_IT(*array_io, array_io->count - 1);

	return array_io->count - 1;
}

template<typename T>
instant void
Array_ClearContainer(
	Array<T> *array_out
) {
	Assert(array_out);

	array_out->count = 0;
}

template<typename T>
instant void
Array_DestroyContainer(
	Array<T> *array_out
) {
	Assert(array_out);

	Memory_Free(array_out->memory);
	*array_out = {};
}

/// Will add memory slots on top of existing ones and add to that count
template <typename T>
instant void
Array_ReserveAdd(
	Array<T> *array_io,
	u64 count_delta,
	bool clear_zero = false
) {
	Assert(array_io);

	u64 old_limit = array_io->max;

	if (array_io->count + count_delta > array_io->max) {
		array_io->max += count_delta;
		array_io->memory = (T *)_Memory_Resize( array_io->memory,
												array_io->max * sizeof(T));
	}

	if (clear_zero) {
		/// only clear new reserved data
		Memory_Set( array_io->memory + array_io->count,
					0,
					(array_io->max - old_limit) * sizeof(T));
	}
}

template <typename T>
instant void
Array_Reserve(
	Array<T> *array_io,
	u64 count,
	bool clear_zero = false
) {
	Assert(array_io);

	u64 old_max = array_io->max;
	u64 new_max = array_io->count + count;

	if (new_max > old_max) {
		array_io->max = new_max;
		array_io->memory = (T *)_Memory_Resize( array_io->memory,
												array_io->max  * sizeof(T));
	}

	if (clear_zero) {
		/// only clear new reserved data
		Memory_Set( array_io->memory + array_io->count,
					0,
					(new_max - old_max) * sizeof(T));
	}
}

/// search availability or index
template <typename T>
instant bool
Array_Find(
	Array<T> *array,
	T find,
	u64 *index = 0
) {
	Assert(array);

	FOR_ARRAY(*array, it) {
		if (ARRAY_IT(*array, it) == find) {
			if (index)
				*index = it;

			return true;
		}
	}

	return false;
}

template <typename T>
instant bool
Array_FindOrAdd(
	Array<T> *array_io,
	T find,
	T **entry_out
) {
	Assert(array_io);
	Assert(entry_out);

	u64 t_index_find;
	bool found_element = Array_Find(array_io, find, &t_index_find);

	if (found_element) {
		*entry_out = &ARRAY_IT(*array_io, t_index_find);
	}
	else {
        Array_AddEmpty(array_io, entry_out);

        /// store what you want to find, if it does not exists,
        /// so it does not have to be assigned manually all the time
        **entry_out = find;
	}

	return found_element;
}

/// Returns T, so dynamic memory can still be free'd
template <typename T>
instant T
Array_Remove(
	Array<T> *array_io,
	u64 index
) {
	Assert(array_io);
	Assert(index < array_io->count);

	T result = ARRAY_IT(*array_io, index);

	/// overwrite current entry with next
	FOR_ARRAY_START(*array_io, index, it) {
		if (it + 1 >= array_io->count)
			break;

		ARRAY_IT(*array_io, it) = ARRAY_IT(*array_io, it + 1);
	}

	array_io->count -= 1;

	return result;
}

template <typename T>
instant Array<T>
Array_CreateBuffer(
	u64 count
) {
	Array<T> a_buffer;
	Array_ReserveAdd(&a_buffer, count, true);
	a_buffer.count = count;
	a_buffer.size = a_buffer.max;

	return a_buffer;
}

template <typename T>
instant void
Array_Sort_Ascending(
	Array<T> *array_io
) {
	Assert(array_io);

	if(!array_io->count)
		return;

	Sort_Quick( &array_io->memory[0],
				&array_io->memory[array_io->count - 1],
				SORT_ORDER_ASCENDING);
}

template <typename T>
instant void
Array_Sort_Descending(
	Array<T> *array_io
) {
	Assert(array_io);

	if(!array_io->count)
		return;

	Sort_Quick( &array_io->memory[0],
				&array_io->memory[array_io->count - 1],
				SORT_ORDER_DESCENDING);
}
