#pragma once

#define ARRAY_IT(_array, _it) \
	((_array).memory)[_it]

#define FOR_ARRAY(_array, _it) 		\
	for(u64 _it = 0;        		\
		_it < (_array).count;       \
		++_it)

#define FOR_ARRAY_START(_array, _it, _start) 	\
	for(u64 _it = (_start);   					\
		_it < (_array).count;       			\
		++_it)

#define FOR_ARRAY_AUTO(_array, _it) 		            \
	for(auto _it = &(_array).memory[0];                 \
		_it <= &(_array).memory[(_array).count - 1];    \
		++_it)

#define FOR_ARRAY_AUTO_START(_array, _it, _start) 		\
	for(auto _it = &(_array).memory[_start];            \
		_it <= &(_array).memory[(_array).count - 1];    \
		++_it)

#define FOR_ARRAY_REV(_array, _it)	    \
	for(s64 _it = (_array).count - 1;	\
		_it >= 0;						\
		--_it)

#define ARRAY_COUNT(_array) \
	(sizeof(_array)/sizeof(_array[0]))

template <typename T>
struct Array {
    T    *memory = 0;
    u64   count  = 0;
    u64   max    = 0;

    u64   last_search_index_found = 0;

    /// f.e. for string chunks
    bool  by_reference = false;
};

template <typename T>
instant void
Array_Clear(
    Array<T> *array_out
) {
    Assert(array_out);

    if (    !array_out->by_reference
            AND !std::is_pointer<T>::value
       ) {
        AssertMessage(false, "Trying to clear unspecialized array.");
    }

    Array_ClearContainer(array_out);
}

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
    *element_empty_out = Array_Add(array_io, t_element_empty);

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

template<typename T>
instant void
Array_Destroy(
    Array<T> *array_out
) {
    if (std::is_pointer<T>::value) {
        if (!array_out->by_reference)
            LOG_WARNING("Array clearing of *data. Assuming array does not have ownership over content.");

        Array_DestroyContainer(array_out);
        return;
    }

    AssertMessage(false, 	"Trying to destroy non-* array data.\n"
                  "Custom overload of Array_Destroy routine is required "
                  "to avoid accidental memory leaks.");
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
    Array<T> &array,
    T find,
    u64 *index = 0
) {
    /// in case of content removal
    Clamp(&array.last_search_index_found, 0, array.count);

    FOR_START(array.last_search_index_found, array.count, it) {
        if (ARRAY_IT(array, it) == find) {
            if (index)
                *index = it;

            return true;
        }
    }

    FOR_START(0, array.last_search_index_found, it) {
        if (ARRAY_IT(array, it) == find) {
            if (index)
                *index = it;

            return true;
        }
    }

    return false;
}

template <typename T, typename Func>
instant bool
Array_Find(
    Array<T> &array,
    T find,
    u64 *index_opt,
    Func OnSearch
) {
    /// in case of content removal
    Clamp(&array.last_search_index_found, 0, array.count);

    FOR_START(array.last_search_index_found, array.count, it) {
        if (OnSearch(ARRAY_IT(array, it), find)) {
            if (index_opt)
                *index_opt = it;

            array.last_search_index_found = it;

            return true;
        }
    }

    FOR_START(0, array.last_search_index_found, it) {
        if (OnSearch(ARRAY_IT(array, it), find)) {
            if (index_opt)
                *index_opt = it;

            array.last_search_index_found = it;

            return true;
        }
    }

    return false;
}

/// true if found / existed already
template <typename T>
instant bool
Array_FindOrAdd(
    Array<T> *array_io,
    T find,
    T **entry_out_opt = 0
) {
    Assert(array_io);

    u64 t_index_find;
    bool found_element = Array_Find(*array_io, find, &t_index_find);

    if (!found_element) {
        Array_Add(array_io, find);
        t_index_find = array_io->count - 1;
    }

    if (entry_out_opt)
        *entry_out_opt = &ARRAY_IT(*array_io, t_index_find);

    return found_element;
}

/// true if found / existed already
template <typename T, typename Func>
instant bool
Array_FindOrAdd(
    Array<T> &array_io,
    T find,
    T **entry_out,
    Func OnSearch
) {
    Assert(entry_out);

    u64 t_index_find;
    bool found_element = Array_Find(array_io, find, &t_index_find, OnSearch);

    if (found_element) {
        *entry_out = &ARRAY_IT(array_io, t_index_find);
    } else {
        Array_AddEmpty(&array_io, entry_out);

        /// store what you want to find, if it does not exists,
        /// so it does not have to be assigned manually all the time
        **entry_out = find;
    }

    return found_element;
}

template <typename T>
instant bool
Array_AddUnique(
    Array<T> *array_io,
    T   element,
    T** added_element = 0
) {
    Assert(array_io);

    return !Array_FindOrAdd(array_io, element, added_element);
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
    FOR_ARRAY_START(*array_io, it, index) {
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

/// does NOT work when lamba is declared as auto,
/// since template typename has to match the
/// event function return type
template <typename T>
instant void
Array_Filter(
    Array<T> *a_data_io,
    bool (*OnKeepIfMatch) (T value)
) {
    Assert(a_data_io);

    FOR_ARRAY(*a_data_io, it) {
        T t_data = ARRAY_IT(*a_data_io, it);

        if (!OnKeepIfMatch(t_data)) {
            Array_Remove(a_data_io, it);
            --it;
        }
    }
}

template <typename T, typename Func>
instant void
Array_Filter(
    Array<T> *a_data_io,
    Func OnKeepIfMatch
) {
    Assert(a_data_io);

    FOR_ARRAY(*a_data_io, it) {
        T t_data = ARRAY_IT(*a_data_io, it);

        if (!OnKeepIfMatch(t_data)) {
            Array_Remove(a_data_io, it);
            --it;
        }
    }
}

template <typename T, typename Func>
instant void
Array_Filter(
    Array<T> *a_data_io,
    Array<Func> a_OnKeepIfMatch
) {
    Assert(a_data_io);

    FOR_ARRAY(*a_data_io, it_data) {
        T t_data = ARRAY_IT(*a_data_io, it_data);

        FOR_ARRAY(a_OnKeepIfMatch, it_func) {
            Func OnKeepIfMatch = ARRAY_IT(a_OnKeepIfMatch, it_func);

            bool result = OnKeepIfMatch(t_data);

            if (!result) {
                Array_Remove(a_data_io, it_data);
                --it_data;
                break;
            }
        }
    }
}

template <typename T>
instant void
Array_Filter(
    Array<T> *a_dest_out,
    Array<T> *a_source,
    bool (*OnKeepIfMatch) (T value)
) {
    Assert(a_source);
    Assert(a_dest_out);

    Array_Clear(a_dest_out);

    FOR_ARRAY(*a_source, it) {
        T t_data = ARRAY_IT(*a_source, it);

        if (OnKeepIfMatch(t_data)) {
            Array_Add(a_dest_out, t_data);
        }
    }
}

template <typename T, typename Func>
instant void
Array_Filter(
    Array<T> *a_dest_out,
    Array<T> *a_source,
    Func OnKeepIfMatch
) {
    Assert(a_source);
    Assert(a_dest_out);

    Array_Clear(a_dest_out);

    FOR_ARRAY(*a_source, it) {
        T t_data = ARRAY_IT(*a_source, it);

        if (OnKeepIfMatch(t_data)) {
            Array_Add(a_dest_out, t_data);
        }
    }
}

template <typename T>
constexpr
instant bool
MatchesAny(T &checkAgainst, Array<T> &a_oneOf) {
    return Array_Find(a_oneOf, checkAgainst);
}
