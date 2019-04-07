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
instant void Sort_Quick(Sort_Data<T> sort_data);

template <typename T>
instant void Sort_Quick_Custom(Sort_Data<T> sort_data);


template <typename T>
instant u64 WINAPI
Sort_Quick_Thread (
	void *data
) {
	auto sort_data = *(Sort_Data<T> *)(data);
	Sort_Quick(sort_data);

	return 0;
}

template <typename T>
instant u64 WINAPI
Sort_Quick_Custom_Thread (
	void *data
) {
	auto sort_data = *(Sort_Data<T> *)(data);
	Sort_Quick_Custom(sort_data);

	return 0;
}

template <typename T>
instant void
Sort_Quick(
	Sort_Data<T> sort_data
) {
	T *begin_io = sort_data.begin_io;
	T *end_io   = sort_data.end_io;

	Assert(begin_io);
	Assert(end_io);

	if (begin_io == end_io)
		return;

    T *pivot = begin_io;
    T *next  = begin_io;
	++next;

	while(next <= end_io) {
		if (sort_data.type == SORT_ORDER_ASCENDING) {
			if (*next < *pivot) {
				Swap(next, pivot);

				if (pivot < next) {
					++pivot;
					continue;
				}
			}
		}
		else
		if (sort_data.type == SORT_ORDER_DESCENDING) {
			if (*next > *pivot) {
				Swap(next, pivot);

				if (pivot < next) {
					++pivot;
					continue;
				}
			}
		}
		else {
			Assert(false);
		}
		++next;
	}

	Thread thread_one;
	Thread thread_two;

    if (begin_io < pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.end_io       = pivot - 1;

		thread_one = Thread_Create(&t_sort_data, Sort_Quick_Thread<T>);
		Thread_Execute(&thread_one);
    }

	if (end_io > pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.begin_io     = pivot + 1;

		thread_two = Thread_Create(&t_sort_data, Sort_Quick_Thread<T>);
		Thread_Execute(&thread_two);
	}

	if (Thread_WasStarted(&thread_one))  Thread_WaitFor(&thread_one);
	if (Thread_WasStarted(&thread_two))  Thread_WaitFor(&thread_two);
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

	if (begin_io == end_io)
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

	Thread thread_one;
	Thread thread_two;

    if (begin_io < pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.end_io       = pivot - 1;

		thread_one = Thread_Create(&t_sort_data, Sort_Quick_Custom_Thread<T>);
		Thread_Execute(&thread_one);
    }

	if (end_io > pivot) {
		Sort_Data<T> t_sort_data = sort_data;
		t_sort_data.begin_io     = pivot + 1;

		thread_two = Thread_Create(&t_sort_data, Sort_Quick_Custom_Thread<T>);
		Thread_Execute(&thread_two);
	}

	if (Thread_WasStarted(&thread_one))  Thread_WaitFor(&thread_one);
	if (Thread_WasStarted(&thread_two))  Thread_WaitFor(&thread_two);
}
