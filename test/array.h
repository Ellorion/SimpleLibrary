#pragma once

template <typename T>
instant bool
Array_OnSearch(
	T element,
	T find
) {
	return (element.a == find.a);
}

void
Test_Arrays(
) {
	{
		String s_data;
		String_Append(&s_data, S("test"));

		Array<String> as_test;
		Array_Add(&as_test, s_data);

		FOR_ARRAY(as_test, it) {
			String ts_data = ARRAY_IT(as_test, it);
			AssertMessage(ts_data == "test", "[Test] Array_Add failed.");
		}

		Array_ClearContainer(&as_test);
		AssertMessage(      as_test.count == 0
						AND as_test.memory > 0, "[Test] Clearing array failed.");

		/// Memory ownership in local scope and not in array
		String_Destroy(&s_data);
	}

	{
		Array<String> as_test;
		String *s_item = 0;

		Array_AddEmpty(&as_test, &s_item);
		String_Append(s_item, S("bla"));

		FOR_ARRAY(as_test, it) {
			String *ts_data = &ARRAY_IT(as_test, it);
			AssertMessage(*ts_data == "bla", "[Test] Array_AddEmpty failed.");
		}

		Array_AddEmpty(&as_test, &s_item);
		String_Append(s_item, S("blub"));

		u64 index_found = 0;
		Array_Find(&as_test, *s_item, &index_found);
		AssertMessage(index_found == 1, "[Test] Array_Find failed.");

		while(as_test.count) {
			String s_data_it = Array_Remove(&as_test, 0);
			String_Destroy(&s_data_it);
		}
		AssertMessage(as_test.count == 0, "[Test] Array_Remove failed.");
		Array_Destroy(&as_test);
	}

	{
		String s_split;
		String_Append(&s_split, S("aaa"));

		Array<String> as_split;
		as_split = String_Split(&s_split, S("\n"), DELIMITER_IGNORE, true);

		AssertMessage(as_split.count == 1, "[Test] Incorrect amount of items split from string (1).");
		AssertMessage(ARRAY_IT(as_split, 0) == "aaa", "[Test] First Array item does not match.");

		Array_Destroy(&as_split);
	}

	{
		Array<String> as_data;
		String s_data;
		String_Append(&s_data, S("demo"));
		Array_Add(&as_data, s_data);
		AssertMessage(as_data.max == 1, "[Test] Data size missmatch.");
		/// Will result in 1 (existing) + 3 (new empty) slots
		Array_ReserveAdd(&as_data, 3, true);
		AssertMessage(as_data.max == 4 AND as_data.count == 1, "[Test] Array reservation failed.");

		/// non-generic array_destroy will free the string
		Array_Destroy(&as_data);
	}

	{
		String s_split;
		String_Append(&s_split, S("aaa\nbbb"));

		Array<String> as_split;
		as_split = String_Split(&s_split, S("\n", 1), DELIMITER_IGNORE, true);

		AssertMessage(as_split.count == 2, "[Test] Incorrect amount of items split from string (2).");
		AssertMessage(ARRAY_IT(as_split, 0) == "aaa", "[Test] First Array item does not match.");
		AssertMessage(ARRAY_IT(as_split, 1) == "bbb", "[Test] Second Array item does not match.");

		Array_Destroy(&as_split);
		String_Destroy(&s_split);
	}

	{
		Array<String> as_data;
		String s_data1;
		String s_data2;
		String s_data3;
		String s_data4;

		String_Append(&s_data3, S("3"));
		String_Append(&s_data2, S("2"));
		String_Append(&s_data4, S("4"));
		String_Append(&s_data1, S("1"));

		Array_Add(&as_data, s_data3);
		Array_Add(&as_data, s_data2);
		Array_Add(&as_data, s_data4);
		Array_Add(&as_data, s_data1);

		Array_Sort_Ascending(&as_data);

		AssertMessage(		ARRAY_IT(as_data, 0) == "1"
						AND ARRAY_IT(as_data, 1) == "2"
						AND ARRAY_IT(as_data, 2) == "3"
						AND ARRAY_IT(as_data, 3) == "4"
							, "[Test] String array sorting failed (ascending).");

		Array_Sort_Descending(&as_data);

		AssertMessage(		ARRAY_IT(as_data, 0) == "4"
						AND ARRAY_IT(as_data, 1) == "3"
						AND ARRAY_IT(as_data, 2) == "2"
						AND ARRAY_IT(as_data, 3) == "1"
							, "[Test] String array sorting failed (descending).");

		Array_Destroy(&as_data);
	}

	{
		Array<s32> a_sort;

		Array_Add(&a_sort, 3);
		Array_Add(&a_sort, 1);
		Array_Add(&a_sort, 4);
		Array_Add(&a_sort, 1);
		Array_Add(&a_sort, 5);
		Array_Add(&a_sort, 9);
		Array_Add(&a_sort, 2);
		Array_Add(&a_sort, 6);
		Array_Add(&a_sort, 5);
		Array_Add(&a_sort, 2);
		Array_Add(&a_sort, 12);

		Array_Sort_Ascending(&a_sort);

		AssertMessage(		ARRAY_IT(a_sort, 0)  == 1
						AND ARRAY_IT(a_sort, 1)  == 1
						AND ARRAY_IT(a_sort, 2)  == 2
						AND ARRAY_IT(a_sort, 3)  == 2
						AND ARRAY_IT(a_sort, 4)  == 3
						AND ARRAY_IT(a_sort, 5)  == 4
						AND ARRAY_IT(a_sort, 6)  == 5
						AND ARRAY_IT(a_sort, 7)  == 5
						AND ARRAY_IT(a_sort, 8)  == 6
						AND ARRAY_IT(a_sort, 9)  == 9
						AND ARRAY_IT(a_sort, 10) == 12
							, "[Test] Integer array sorting failed (ascending).");

		Array_Sort_Descending(&a_sort);

		AssertMessage(		ARRAY_IT(a_sort, 0)  == 12
						AND ARRAY_IT(a_sort, 1)  == 9
						AND ARRAY_IT(a_sort, 2)  == 6
						AND ARRAY_IT(a_sort, 3)  == 5
						AND ARRAY_IT(a_sort, 4)  == 5
						AND ARRAY_IT(a_sort, 5)  == 4
						AND ARRAY_IT(a_sort, 6)  == 3
						AND ARRAY_IT(a_sort, 7)  == 2
						AND ARRAY_IT(a_sort, 8)  == 2
						AND ARRAY_IT(a_sort, 9)  == 1
						AND ARRAY_IT(a_sort, 10) == 1
							, "[Test] Integer array sorting failed (descending).");
	}

	{
		String s_data;
		String_Append(&s_data, S("Lorem ipsum dolor\r\n\r\nLorem ipsum dolor sit amet."));

		///@Performance: text could be processed without having to split
		///              data multiple times
		Array<String> as_lines = Array_Split(&s_data, S("\r\n"), DELIMITER_ADD_BACK, true);
		String_Destroy(&s_data);

		Array<String> as_words;

		FOR_ARRAY(as_lines, it_lines) {
			String *ts_line = &ARRAY_IT(as_lines, it_lines);

			Array<String> tas_words = Array_Split(ts_line, S(" "), DELIMITER_ADD_FRONT, true);

			FOR_ARRAY(tas_words, it_words) {
				String *ts_word = &ARRAY_IT(tas_words, it_words);
				Array_Add(&as_words, *ts_word);
			}

			/// keep string values in as_words valid
			Array_DestroyContainer(&tas_words);
		}

///		"Lorem ipsum dolor\r\n",
///		"\r\n",
///		"Lorem ipsum dolor sit amet."
		AssertMessage(as_lines.count == 3, "[Test] Splitting lines failed.");

///		"Lorem",
///		" ipsum",
///		" dolor\r\n",
///		"\r\n",
///		"Lorem",
///		" ipsum",
///		" dolor",
///		" sit",
///		" amet."
		AssertMessage(as_words.count == 9, "[Test] Splitting words failed.");

		Array_Destroy(&as_lines);
		Array_Destroy(&as_words);
	}

	{
		typedef bool (*OnKeepIfMatch)(s64);

		Array<s64> a_numbers;
		Array_AddRange(&a_numbers, 1, 30, [](auto data_ptr, auto it){
			*data_ptr = it;
		});

		/// useable variants
		OnKeepIfMatch   fn_2 =       [](s64 value)         { return value % 2 != 0; };
		bool          (*fn_3)(s64) = [](s64 value)         { return value % 3 != 0; };
		auto            fn_5 =       [](s64 value) -> bool { return value % 5 != 0; };

		Array<OnKeepIfMatch> a_filters;
		Array_Add(&a_filters, fn_2);
		Array_Add(&a_filters, fn_3);

		/// does not work without casting
		Array_Add(&a_filters, (OnKeepIfMatch)fn_5);

		Array_Filter(&a_numbers, a_filters);

		s32 counter = 0;

		FOR_ARRAY(a_numbers, it) {
			s64 t_number = ARRAY_IT(a_numbers, it);

			switch (t_number) {
				case 1:
				case 7:
				case 11:
				case 13:
				case 17:
				case 19:
				case 23:
				case 29: {
					++counter;
				} break;

				default:
					AssertMessage(false, "[test] Array filter failed #1.1 (prime numbers).");
			}
		}

		AssertMessage(counter == 8, "[test] Array filter failed #1.2 (prime numbers).");

		Array_DestroyContainer(&a_numbers);
	}

	{
		Array<s64> a_numbers;
		Array_AddRange(&a_numbers, 1, 30, [](auto data_ptr, auto it){
			*data_ptr = it;
		});

		bool (*OnKeepIfMatch)(s64) = [](s64 value) {
			if (value % 2 == 0)  return false;
			if (value % 3 == 0)  return false;
			if (value % 5 == 0)  return false;

			return true;
		};

		Array_Filter(&a_numbers, OnKeepIfMatch);

		s32 counter = 0;

		FOR_ARRAY(a_numbers, it) {
			s64 t_number = ARRAY_IT(a_numbers, it);

			switch (t_number) {
				case 1:
				case 7:
				case 11:
				case 13:
				case 17:
				case 19:
				case 23:
				case 29: {
					++counter;
				} break;

				default:
					AssertMessage(false, "[test] Array filter failed #2.1 (prime numbers).");
			}
		}

		AssertMessage(counter == 8, "[test] Array filter failed #2.2 (prime numbers).");

		Array_DestroyContainer(&a_numbers);
	}

	{
		Array<s64> a_numbers;
		Array_AddRange(&a_numbers, 1, 30, [](auto data_ptr, auto it){
			*data_ptr = it;
		});

		auto OnKeepIfMatch = [](s64 value) -> bool {
			if (value % 2 == 0)  return false;
			if (value % 3 == 0)  return false;
			if (value % 5 == 0)  return false;

			return true;
		};

		Array_Filter(&a_numbers, OnKeepIfMatch);

		s32 counter = 0;

		FOR_ARRAY(a_numbers, it) {
			s64 t_number = ARRAY_IT(a_numbers, it);

			switch (t_number) {
				case 1:
				case 7:
				case 11:
				case 13:
				case 17:
				case 19:
				case 23:
				case 29: {
					++counter;
				} break;

				default:
					AssertMessage(false, "[test] Array filter failed #3.1 (prime numbers).");
			}
		}

		AssertMessage(counter == 8, "[test] Array filter failed #3.2 (prime numbers).");

		Array_DestroyContainer(&a_numbers);
	}

	{
		Array<s64> a_numbers;
		Array_AddRange(&a_numbers, 1, 30, [](auto data_ptr, auto it){
			*data_ptr = it;
		});

		/// auto function parameter variance
		auto OnKeepIfMatch = [](auto value) -> bool {
			if (value % 2 == 0)  return false;
			if (value % 3 == 0)  return false;
			if (value % 5 == 0)  return false;

			return true;
		};

		Array_Filter(&a_numbers, OnKeepIfMatch);

		s32 counter = 0;

		FOR_ARRAY(a_numbers, it) {
			s64 t_number = ARRAY_IT(a_numbers, it);

			switch (t_number) {
				case 1:
				case 7:
				case 11:
				case 13:
				case 17:
				case 19:
				case 23:
				case 29: {
					++counter;
				} break;

				default:
					AssertMessage(false, "[test] Array filter failed #3.1 (prime numbers).");
			}
		}

		AssertMessage(counter == 8, "[test] Array filter failed #3.2 (prime numbers).");

		Array_DestroyContainer(&a_numbers);
	}

	{
		Array<s64> a_numbers;
		Array_AddRange(&a_numbers, 1, 30, [](auto data_ptr, auto it){
			*data_ptr = it;
		});

		/// auto function parameter variance
		auto OnKeepIfMatch = [](s64 value) {
			if (value % 2 == 0)  return false;
			if (value % 3 == 0)  return false;
			if (value % 5 == 0)  return false;

			return true;
		};

		Array_Filter(&a_numbers, OnKeepIfMatch);

		s32 counter = 0;

		FOR_ARRAY(a_numbers, it) {
			s64 t_number = ARRAY_IT(a_numbers, it);

			switch (t_number) {
				case 1:
				case 7:
				case 11:
				case 13:
				case 17:
				case 19:
				case 23:
				case 29: {
					++counter;
				} break;

				default:
					AssertMessage(false, "[test] Array filter failed #3.1 (prime numbers).");
			}
		}

		AssertMessage(counter == 8, "[test] Array filter failed #3.2 (prime numbers).");

		Array_DestroyContainer(&a_numbers);
	}

	{
		Array<s64> a_numbers;
		Array_AddRange(&a_numbers, 1, 30, [](auto data_ptr, auto it){
			*data_ptr = it;
		});

		/// auto function parameter variance
		auto OnKeepIfMatch = [](auto value) {
			if (value % 2 == 0)  return false;
			if (value % 3 == 0)  return false;
			if (value % 5 == 0)  return false;

			return true;
		};

		Array_Filter(&a_numbers, OnKeepIfMatch);

		s32 counter = 0;

		FOR_ARRAY(a_numbers, it) {
			s64 t_number = ARRAY_IT(a_numbers, it);

			switch (t_number) {
				case 1:
				case 7:
				case 11:
				case 13:
				case 17:
				case 19:
				case 23:
				case 29: {
					++counter;
				} break;

				default:
					AssertMessage(false, "[test] Array filter failed #3.1 (prime numbers).");
			}
		}

		AssertMessage(counter == 8, "[test] Array filter failed #3.2 (prime numbers).");

		Array_DestroyContainer(&a_numbers);
	}

	{
		Array<s64> a_numbers;
		Array_AddRange(&a_numbers, 1, 30, [](auto data_ptr, auto it){
			*data_ptr = it;
		});

		Array_Filter(&a_numbers,
			[](s64 value) {
				if (value % 2 == 0)  return false;
				if (value % 3 == 0)  return false;
				if (value % 5 == 0)  return false;

				return true;
			}
		);

		s32 counter = 0;

		FOR_ARRAY(a_numbers, it) {
			s64 t_number = ARRAY_IT(a_numbers, it);

			switch (t_number) {
				case 1:
				case 7:
				case 11:
				case 13:
				case 17:
				case 19:
				case 23:
				case 29: {
					++counter;
				} break;

				default:
					AssertMessage(false, "[test] Array filter failed #4.1 (prime numbers).");
			}
		}

		AssertMessage(counter == 8, "[test] Array filter failed #4.2 (prime numbers).");

		Array_DestroyContainer(&a_numbers);
	}

	{
		u64 max = 10000;

		Array<u64> a_test;
		Array_Fill(&a_test, max);

		Array_Sort_Descending(&a_test);
		Array_FillTest(&a_test, SORT_ORDER_DESCENDING);

		Array_Sort_Ascending(&a_test);
		Array_FillTest(&a_test, SORT_ORDER_ASCENDING);

		Array_Sort(&a_test, Sort_OnCompareDescending);
		Array_FillTest(&a_test, SORT_ORDER_DESCENDING);

		Array_Sort(&a_test, Sort_OnCompareAscending);
		Array_FillTest(&a_test, SORT_ORDER_ASCENDING);
	}

	{
		struct test {
			int  a = 0;
			char b = 0;
		};

		Array<test> a_test;
		Array_Add(&a_test, {1, 'a'});
		Array_Add(&a_test, {2, 'b'});
		Array_Add(&a_test, {3, 'c'});
		Array_Add(&a_test, {4, 'd'});
		Array_Add(&a_test, {5, 'e'});

		u64 index = 0;
		bool success = false;

		{
			success = Array_Find(&a_test, {3, 0}, &index, [](auto element, auto find) {
				return (element.a == find.a);
			});

			Assert(success AND index == 2);

		}
		{
			success = Array_Find(&a_test, {6, 0}, &index, [](auto element, auto find) {
				return (element.a == find.a);
			});

			Assert(!success);
		}
		{
			success = Array_Find(&a_test, {1, 0}, &index, Array_OnSearch<test>);
			Assert(success AND index == 0);
		}
		{
			success = Array_Find(&a_test, {0, 0}, &index, Array_OnSearch<test>);
			Assert(!success);
		}
	}
}
