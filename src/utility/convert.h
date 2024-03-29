#pragma once

instant String
Convert_IntToString(
	s64 value,
	int base = 10
) {
	String s_result;

	if (value == 0) {
		String_Insert(s_result, '0', 0);
	}
	else {
		while(value) {
			int remainder = value % base;
			{
				char ch = (remainder > 9)
							? (remainder - 10) + 'a'
							:  remainder       + '0';

				String_Insert(s_result, ch, 0);
			}
			value /= base;
		}
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
	String_Append(s_result, s_number);
	String_Destroy(s_number);

	u64 pow = 1;
	u8  num_of_remainders_it = num_of_remainders;

	while(num_of_remainders_it) {
		pow *= 10;
		--num_of_remainders_it;
	}

	if (pow > 1) {
	    double remainder = (value - number) * pow;

		String_Append(s_result, S("."));

		if (remainder) {
			String s_remainder = Convert_IntToString(remainder);

			String_Append(s_result, s_remainder);
			num_of_remainders -= s_remainder.length;

			String_Destroy(s_remainder);
		}

		while(num_of_remainders) {
			String_Append(s_result, S("0"));
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

	if (String_IsEmpty(*s_data)) {
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

	String_Destroy(*s_data_out);
	String_Append(*s_data_out, s_data);
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
	String_Append(s_result, S(units_size[index]));

	return s_result;
}

instant char *
Convert_ToCString(
	s64 value,
	u32 len = 0
) {
    u64 tmpValue   = value;
	u8  digitCount = 0;
	u8  is_signed  = 0;

	/// make space for '-' sign
	if (value < 0) {
		++digitCount;
		is_signed = 1;
	}

	/// calc number of digits
	do {
		value /= 10;
		++digitCount;
	} while (value);

    /// include null-terminate char
	char *buffer = Memory_Create(char, digitCount + 1);

	/// display negative sign and stay in ascii number
	/// range by removing the sign from the value
	value = tmpValue;
	if (value < 0) {
		*buffer = '-';
		value *= -1;
	}

	if (len AND len > digitCount)
		digitCount = len;

	/// convert digits to char array
	do {
		int digit = value % 10;
		buffer[--digitCount] = digit + '0';
		value /= 10;
	} while (digitCount - is_signed);

	if (len)
		buffer[len] = 0;

	return buffer;
}

instant char *
Convert_ToTempCString(
	s64 value,
	u32 len = 0
) {
    u64 tmpValue   = value;
	u8  digitCount = 0;
	u8  is_signed  = 0;

	/// make space for '-' sign
	if (value < 0) {
		++digitCount;
		is_signed = 1;
	}

	/// calc number of digits
	do {
		value /= 10;
		++digitCount;
	} while (value);

    /// include null-terminate char
	char *buffer = (char *)MemoryArena_Alloc(digitCount + 1);

	/// display negative sign and stay in ascii number
	/// range by removing the sign from the value
	value = tmpValue;
	if (value < 0) {
		*buffer = '-';
		value *= -1;
	}

	if (len AND len > digitCount)
		digitCount = len;

	/// convert digits to char array
	do {
		int digit = value % 10;
		buffer[--digitCount] = digit + '0';
		value /= 10;
	} while (digitCount - is_signed);

	if (len)
		buffer[len] = 0;

	return buffer;
}

instant
s32
Convert_ToInt(
	const String &s_data,
	s32 valueOnFailure = 0
) {
    if (String_IsEmpty(s_data)) {
        return valueOnFailure;
    }

	char *c_data = String_CreateCBufferCopy(s_data);
	s32 result = atoi(c_data);
	Memory_Free(c_data);

	return result;
}

instant
double
Convert_ToDouble(
	const String &s_data,
	double valueOnFailure = 0.0
) {
    if (String_IsEmpty(s_data)) {
        return valueOnFailure;
    }

	char *c_data = String_CreateCBufferCopy(s_data);
	float result = atof(c_data);
	Memory_Free(c_data);

	return result;
}

instant s64
String_ParseNumber(
    const String &s_data,
    s64 *result = nullptr,
    u64 *length = nullptr,
    bool check_sign = false
) {
    s64 nr_index = -1;
    u64 nr_chars = 0;

    String s_dataIt = S(s_data);

    FOR(s_dataIt.length, it) {
        if (IsNumeric(s_dataIt.value[it])) {
            if (!nr_chars) {
                nr_index = it;
            }

            ++nr_chars;
        }
        else {
            if (nr_chars) {
                break;
            }
        }
    }

    if (nr_index >= 0) {
        String_AddOffset(s_dataIt, nr_index);
        String s_number = S(s_dataIt, nr_chars);

        s64 number = Convert_ToInt(s_number);

        if (check_sign) {
            if (nr_index) {
                if (s_number.value[-1] == '-') {
                    number = -number;
                    --nr_index;
                    ++nr_chars;
                }
            }
        }

        if (result) {
            *result = number;
        }
    }

    if (length) {
        *length = nr_chars;
    }

    return nr_index;
}

instant s64
String_ParseNumber(
    const String &s_data,
    double *result = nullptr,
    u64 *length = nullptr,
    bool check_sign = false
) {
    s64 nr_index = -1;
    u64 nr_chars = 0;
    bool found_dot = false;

    String s_dataIt = S(s_data);

    FOR(s_dataIt.length, it) {
        if (IsNumeric(s_dataIt.value[it])) {
            if (!nr_chars) {
                nr_index = it;
            }

            ++nr_chars;
        }
        else if (nr_chars && s_dataIt.value[it] == '.') {
            if (found_dot) {
                break;
            }

            found_dot = true;
            ++nr_chars;
        }
        else {
            if (nr_chars) {
                break;
            }
        }
    }

    if (nr_index >= 0) {
        String_AddOffset(s_dataIt, nr_index);
        String s_number = S(s_dataIt, nr_chars);

        double number = Convert_ToDouble(s_number);

        if (check_sign) {
            if (nr_index) {
                if (s_number.value[-1] == '-') {
                    number = -number;
                    --nr_index;
                    ++nr_chars;
                }
            }
        }

        if (result) {
            *result = number;
        }
    }

    if (length) {
        *length = nr_chars;
    }

    return nr_index;
}
