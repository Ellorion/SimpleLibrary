#pragma once

#define ARRAY_IT(_array, _it) \
	((_array).memory)[_it]

#define ARRAY_COUNT(_array) \
	(sizeof(_array)/sizeof(_array[0]))

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
		(_array).count AND _it <= &(_array).memory[(_array).count - 1];    \
		++_it)

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
constexpr
instant void
Array_Clear(
    Array<T> &arr_out
) {
    if (    !arr_out.by_reference
        AND !std::is_pointer<T>::value
       ) {
        AssertMessage(false, "Trying to clear unspecialized array.");
    }

    Array_ClearContainer(&arr_out);
}

///@Info: will copy the struct element pointers only
///
///       if the array should take ownership of all data
///       make sure you copy every data that is connected
///       to a pointer (clone) or don't free the
///       passed / connected data
template <typename T>
constexpr
instant T *
Array_Add(
    Array<T> &arr,
    const T element
) {
    constexpr u64 length = 1;

    if (arr.count + length > arr.max) {
        arr.max += length;
        arr.memory = (T *)_Memory_Resize(arr.memory,
                                         arr.max * sizeof(T));
    }

    arr.count += length;
    u64 target = arr.count - 1; /// convert to index

    arr.memory[target] = element;

    return &arr.memory[target];
}

template <typename T>
constexpr
instant void
Array_Add(
    Array<T> &arr,
    std::initializer_list<T> list
) {
    for (const auto &item : list) {
        Array_Add(&arr, item);
    }
}

///@Hint: only use the output immediately after using this fuction
///       otherwise another call to this function could invalid
///       the prev. output because the base memory pointer could
///       have changed after resizing
template <typename T>
constexpr
instant u64
Array_AddEmpty(
    Array<T> &arr,
    T **element_empty_out
) {
    Assert(element_empty_out);

    T t_element_empty = {};
    *element_empty_out = Array_Add(arr, t_element_empty);

    return arr.count - 1;
}

template<typename T>
constexpr
instant void
Array_ClearContainer(
    Array<T> &arr_out
) {
    arr_out.count = 0;
}

template<typename T>
constexpr
instant void
Array_DestroyContainer(
    Array<T> &arr_out
) {
    Memory_Free(arr_out.memory);
    arr_out = {};
}

template<typename T>
constexpr
instant void
Array_Destroy(
    Array<T> &arr_out
) {
    if (std::is_pointer<T>::value) {
        if (!arr_out.by_reference)
            LOG_WARNING("Array clearing of *data. Assuming array does not have ownership over content.");

        Array_DestroyContainer(arr_out);
        return;
    }

    AssertMessage(false, 	"Trying to destroy non-* array data.\n"
                  "Custom overload of Array_Destroy routine is required "
                  "to avoid accidental memory leaks.");
}

/// Will add memory slots on top of existing ones and add to that count
template <typename T>
constexpr
instant void
Array_ReserveAdd(
    Array<T> &arr,
    u64 count_delta,
    bool clear_zero = false
) {
    u64 old_limit = arr.max;

    if (arr.count + count_delta > arr.max) {
        arr.max += count_delta;
        arr.memory = (T *)_Memory_Resize( arr.memory,
                                                arr.max * sizeof(T));
    }

    if (clear_zero) {
        /// only clear new reserved data
        Memory_Set( arr.memory + arr.count,
                    0,
                    (arr.max - old_limit) * sizeof(T));
    }
}

template <typename T>
constexpr
instant void
Array_Reserve(
    Array<T> &arr,
    u64 count,
    bool clear_zero = false
) {
    u64 old_max = arr.max;
    u64 new_max = arr.count + count;

    if (new_max > old_max) {
        arr.max = new_max;
        arr.memory = (T *)_Memory_Resize( arr.memory,
                                                arr.max  * sizeof(T));
    }

    if (clear_zero) {
        /// only clear new reserved data
        Memory_Set( arr.memory + arr.count,
                    0,
                    (new_max - old_max) * sizeof(T));
    }
}

/// search availability or index
template <typename T>
constexpr
instant bool
Array_Find(
    Array<T> &arr,
    T find,
    u64 *index = 0
) {
    /// in case of content removal
    Clamp(&arr.last_search_index_found, 0, arr.count);

    FOR_START(arr.last_search_index_found, arr.count, it) {
        if (ARRAY_IT(arr, it) == find) {
            if (index)
                *index = it;

            return true;
        }
    }

    FOR_START(0, arr.last_search_index_found, it) {
        if (ARRAY_IT(arr, it) == find) {
            if (index)
                *index = it;

            return true;
        }
    }

    return false;
}

template <typename T, typename Func>
constexpr
instant bool
Array_Find(
    Array<T> &arr,
    T find,
    u64 *index_opt,
    Func OnSearch
) {
    /// in case of content removal
    Clamp(&arr.last_search_index_found, 0, arr.count);

    FOR_START(arr.last_search_index_found, arr.count, it) {
        if (OnSearch(ARRAY_IT(arr, it), find)) {
            if (index_opt)
                *index_opt = it;

            arr.last_search_index_found = it;

            return true;
        }
    }

    FOR_START(0, arr.last_search_index_found, it) {
        if (OnSearch(ARRAY_IT(arr, it), find)) {
            if (index_opt)
                *index_opt = it;

            arr.last_search_index_found = it;

            return true;
        }
    }

    return false;
}

/// true if found / existed already
template <typename T>
constexpr
instant bool
Array_FindOrAdd(
    Array<T> &arr,
    T find,
    T **entry_out_opt = 0
) {
    u64 t_index_find;
    bool found_element = Array_Find(arr, find, &t_index_find);

    if (!found_element) {
        Array_Add(arr, find);
        t_index_find = arr.count - 1;
    }

    if (entry_out_opt)
        *entry_out_opt = &ARRAY_IT(arr, t_index_find);

    return found_element;
}

/// true if found / existed already
template <typename T, typename Func>
constexpr
instant bool
Array_FindOrAdd(
    Array<T> &arr,
    T find,
    T **entry_out,
    Func OnSearch
) {
    Assert(entry_out);

    u64 t_index_find;
    bool found_element = Array_Find(arr, find, &t_index_find, OnSearch);

    if (found_element) {
        *entry_out = &ARRAY_IT(arr, t_index_find);
    } else {
        Array_AddEmpty(arr, entry_out);

        /// store what you want to find, if it does not exists,
        /// so it does not have to be assigned manually all the time
        **entry_out = find;
    }

    return found_element;
}

template <typename T>
constexpr
instant bool
Array_AddUnique(
    Array<T> &arr,
    T   element,
    T** added_element = 0
) {
    return !Array_FindOrAdd(arr, element, added_element);
}

/// Returns T, so dynamic memory can still be free'd
template <typename T>
constexpr
instant T
Array_Remove(
    Array<T> &arr,
    u64 index
) {
    Assert(index < arr.count);

    T result = ARRAY_IT(arr, index);

    /// overwrite current entry with next
    FOR_ARRAY_START(arr, it, index) {
        if (it + 1 >= arr.count)
            break;

        ARRAY_IT(arr, it) = ARRAY_IT(arr, it + 1);
    }

    arr.count -= 1;

    return result;
}

template <typename T>
constexpr
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
constexpr
instant void
Array_Filter(
    Array<T> &a_data,
    bool (*OnKeepIfMatch) (T value)
) {
    FOR_ARRAY(a_data, it) {
        T t_data = ARRAY_IT(a_data, it);

        if (!OnKeepIfMatch(t_data)) {
            Array_Remove(a_data, it);
            --it;
        }
    }
}

template <typename T, typename Func>
constexpr
instant void
Array_Filter(
    Array<T> &a_data,
    Func OnKeepIfMatch
) {
    FOR_ARRAY(a_data, it) {
        T t_data = ARRAY_IT(a_data, it);

        if (!OnKeepIfMatch(t_data)) {
            Array_Remove(a_data, it);
            --it;
        }
    }
}

template <typename T, typename Func>
constexpr
instant void
Array_Filter(
    Array<T> &a_data,
    Array<Func> a_OnKeepIfMatch
) {
    FOR_ARRAY(a_data, it_data) {
        T t_data = ARRAY_IT(a_data, it_data);

        FOR_ARRAY(a_OnKeepIfMatch, it_func) {
            Func OnKeepIfMatch = ARRAY_IT(a_OnKeepIfMatch, it_func);

            bool result = OnKeepIfMatch(t_data);

            if (!result) {
                Array_Remove(a_data, it_data);
                --it_data;
                break;
            }
        }
    }
}

template <typename T>
constexpr
instant void
Array_Filter(
    Array<T> &a_dest_out,
    const Array<T> &a_source,
    bool (*OnKeepIfMatch) (T value)
) {
    Array_Clear(a_dest_out);

    FOR_ARRAY(a_source, it) {
        T t_data = ARRAY_IT(a_source, it);

        if (OnKeepIfMatch(t_data)) {
            Array_Add(a_dest_out, t_data);
        }
    }
}

template <typename T, typename Func>
constexpr
instant void
Array_Filter(
    Array<T> &a_dest_out,
    const Array<T> &a_source,
    Func OnKeepIfMatch
) {
    Array_Clear(a_dest_out);

    FOR_ARRAY(a_source, it) {
        T t_data = ARRAY_IT(a_source, it);

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

/// Usage:
///     Array<String> a_test;
///     Iter(a_test, [&](String item, u64 index) {...
///     Iter(a_test, [&](auto item, u64 index) {...
///     Iter(a_test, [&](const String &item, u64 index) {...
template <class T, typename Func>
constexpr
instant void
Iter(
    Array<T> arr,
    Func func
) {
    u64 index = 0;

    FOR_ARRAY_START(arr, it, index) {
        auto item = ARRAY_IT(arr, it);
        func(item, index);
        ++index;
    }
}

template <class T, typename Func>
constexpr
instant void
Iter(
    Array<T> arr,
    u64 indexStart,
    Func func
) {
    u64 index = indexStart;

    FOR_ARRAY_START(arr, it, index) {
        auto item = ARRAY_IT(arr, it);
        func(item, index);
        ++index;
    }
}
