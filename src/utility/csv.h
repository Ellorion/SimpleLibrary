#pragma once

instant Array<Array<String>>
CSV_Load(
	const String &s_data
) {
	Array<Array<String>> a_csv;
	Array<String>        as_rowitem;

	if (!String_IsEmpty(s_data)) {
		Array<String> as_rows = Array_SplitLinesRef(s_data, false);

		FOR_ARRAY(as_rows, it_row) {
			String *s_row = &ARRAY_IT(as_rows, it_row);

			/// can get overwritten after the prev. split was copied
			as_rowitem = Array_Split(s_row, S(","), DELIMITER_IGNORE, true);
			/// will copy the content, so do not destroy it
			Array_Add(a_csv, as_rowitem);
		}
	}

	return a_csv;
}
