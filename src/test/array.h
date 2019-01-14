#pragma once

void
Test_Arrays(
) {
	{
		String s_data;
		String_Append(&s_data, "test");

		Array<String> as_test;
		Array_Add(&as_test, s_data);

		FOR_ARRAY(as_test, it) {
			String s_data_it = ARRAY_IT(as_test, it);
			AssertMessage(String_IsEqual(&s_data_it, "test"), "Array_Add failed.");
		}

		Array_ClearContainer(&as_test);
		AssertMessage(      as_test.count == 0
						AND as_test.memory > 0, "Clearing array failed.");

		/// Memory ownership in local scope and not in array
		String_Destroy(&s_data);
	}

	{
		Array<String> as_test;
		String *s_item = 0;

		Array_AddEmpty(&as_test, &s_item);
		String_Append(s_item, "bla");

		FOR_ARRAY(as_test, it) {
			String s_data_it = ARRAY_IT(as_test, it);
			AssertMessage(String_IsEqual(&s_data_it, "bla"), "Array_AddEmpty failed.");
		}

		Array_AddEmpty(&as_test, &s_item);
		String_Append(s_item, "blub");

		u64 index_found = 0;
		Array_Find(&as_test, *s_item, &index_found);
		AssertMessage(index_found == 1, "Array_Find failed.");

		while(as_test.count) {
			String s_data_it = Array_Remove(&as_test, 0);
			String_Destroy(&s_data_it);
		}
		AssertMessage(as_test.count == 0, "Array_Remove failed.");
		Array_Destroy(&as_test);
	}

	{
		String s_split;
		String_Append(&s_split, "aaa");

		Array<String> as_split;
		as_split = String_Split(&s_split, "\n");

		AssertMessage(as_split.count == 1, "Incorrect amount of items split from string (1).");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 0), "aaa"), "First Array item does not match.");

		Array_Destroy(&as_split);
	}

	{
		Array<String> as_data;
		String s_data;
		String_Append(&s_data, "demo");
		Array_Add(&as_data, s_data);
		AssertMessage(as_data.max == 1, "Data size missmatch.");
		/// Will result in 1 (existing) + 3 (new empty) slots
		Array_ReserveAdd(&as_data, 3, true);
		AssertMessage(as_data.max == 4 AND as_data.count == 1, "Array reservation failed.");

		/// non-generic array_destroy will free the string
		Array_Destroy(&as_data);
	}

	{
		String s_split;
		String_Append(&s_split, "aaa\nbbb");

		Array<String> as_split;
		as_split = String_Split(&s_split, "\n");

		AssertMessage(as_split.count == 2, "Incorrect amount of items split from string (2).");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 0), "aaa"), "First Array item does not match.");
		AssertMessage(String_IsEqual(&ARRAY_IT(as_split, 1), "bbb"), "Second Array item does not match.");

		Array_Destroy(&as_split);
		String_Destroy(&s_split);
	}

	{
		Array<String> as_data;
		String s_data1;
		String s_data2;
		String s_data3;
		String s_data4;

		String_Append(&s_data3, "3");
		String_Append(&s_data2, "2");
		String_Append(&s_data4, "4");
		String_Append(&s_data1, "1");

		Array_Add(&as_data, s_data3);
		Array_Add(&as_data, s_data2);
		Array_Add(&as_data, s_data4);
		Array_Add(&as_data, s_data1);

		Array_Sort_Ascending(&as_data);

		AssertMessage(		String_IsEqual(&ARRAY_IT(as_data, 0), "1")
						AND String_IsEqual(&ARRAY_IT(as_data, 1), "2")
						AND String_IsEqual(&ARRAY_IT(as_data, 2), "3")
						AND String_IsEqual(&ARRAY_IT(as_data, 3), "4")
							, "String array sorting failed (ascending).");

		Array_Sort_Descending(&as_data);

		AssertMessage(		String_IsEqual(&ARRAY_IT(as_data, 0), "4")
						AND String_IsEqual(&ARRAY_IT(as_data, 1), "3")
						AND String_IsEqual(&ARRAY_IT(as_data, 2), "2")
						AND String_IsEqual(&ARRAY_IT(as_data, 3), "1")
							, "String array sorting failed (descending).");

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
							, "Integer array sorting failed (ascending).");

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
							, "Integer array sorting failed (descending).");
	}

	{
		String s_data;
		String_Append(&s_data, "Lorem ipsum dolor\r\n\r\nLorem ipsum dolor sit amet.");

		///@Performance: text could be processed without having to split
		///              data multiple times
		Array<String> as_lines = Array_Split(&s_data, "\r\n", 2, DELIMITER_ADD_BACK);
		String_Destroy(&s_data);

		Array<String> as_words;

		FOR_ARRAY(as_lines, it_lines) {
			String *ts_line = &ARRAY_IT(as_lines, it_lines);

			Array<String> tas_words = Array_Split(ts_line, " ", 1, DELIMITER_ADD_FRONT);

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
		AssertMessage(as_lines.count == 3, "Splitting lines failed.");

///		"Lorem",
///		" ipsum",
///		" dolor\r\n",
///		"\r\n",
///		"Lorem",
///		" ipsum",
///		" dolor",
///		" sit",
///		" amet."
		AssertMessage(as_words.count == 9, "Splitting words failed.");

		Array_Destroy(&as_lines);
		Array_Destroy(&as_words);
	}
}
