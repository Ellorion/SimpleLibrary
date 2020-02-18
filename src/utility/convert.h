#pragma once

instant String
Convert_IntToString(
	s64 value,
	int base = 10
) {
	String s_result;

	while(value) {
		int remainder = value % base;
		{
			char ch = (remainder > 9)
						? (remainder - 10) + 'a'
						:  remainder       + '0';

			String_Insert(&s_result, ch, 0);
		}
		value /= base;
	}

	return s_result;
}

instant String
Convert_DoubleToString(
	double value,
	u8 num_of_remainders = 2
) {
	String s_result;

	s64 number = value;

	String s_number = Convert_IntToString(number);
	String_Append(&s_result, s_number);
	String_Destroy(&s_number);

	u64 pow = 1;
	u8  num_of_remainders_it = num_of_remainders;

	while(num_of_remainders_it) {
		pow *= 10;
		--num_of_remainders_it;
	}

	if (pow > 1) {
	    double remainder = (value - number) * pow;

		String_Append(&s_result, S("."));

		if (remainder) {
			String s_remainder = Convert_IntToString(remainder);

			String_Append(&s_result, s_remainder);
			num_of_remainders -= s_remainder.length;

			String_Destroy(&s_remainder);
		}

		while(num_of_remainders) {
			String_Append(&s_result, S("0"));
			--num_of_remainders;
		}
	}

	return s_result;
}

/// does NOT add memory for '\0'
/// make sure you add an additional byte
/// in the buffer so you don't truncate the string
instant void
Convert_ToCString(
	char *c_buffer_out,
	u64   c_length,
	String *s_data
) {
	Assert(s_data);

	if (!c_length)
		return;

	if (String_IsEmpty(s_data)) {
		c_buffer_out[0] = '\0';
		return;
	}

	/// stop copying memory after '\0'
	if (c_length > s_data->length + 1) {
		c_length = s_data->length + 1;
	}

	/// stop before '\0'
	FOR(c_length - 1, it) {
		c_buffer_out[it] = s_data->value[it];
	}

	/// set final '\0'
	c_buffer_out[c_length - 1] = '\0';
}

instant void
Convert_ToStringBuffer(
	String *s_data_out,
	const char *c_data,
	u64 c_length = 0
) {
	Assert(s_data_out);
	Assert(c_data);

	if (!c_length)
		c_length = String_GetLength(c_data);

	String s_data = S(c_data, c_length);

	String_Destroy(s_data_out);
	String_Append(s_data_out, s_data);
}


instant String
Convert_ToString(
	const char *c_data,
	u64 c_length = 0
) {
	Assert(c_data);

	String s_result;
	Convert_ToStringBuffer(&s_result, c_data, c_length);

	return s_result;
}

instant String
Convert_SizeToString(
	double size
) {
	u32 index = 0;

	const char *units_size[] = {" bytes", " KB", " MB", " GB", " TB"};

	while(size >= 1024 AND index < ARRAY_COUNT(units_size) ) {
		size /= 1024;
		++index;
	}

	u8 num_remainders = (index > 0) ? 2 : 0;

	String s_result = Convert_DoubleToString(size, num_remainders);
	String_Append(&s_result, S(units_size[index]));

	return s_result;
}
