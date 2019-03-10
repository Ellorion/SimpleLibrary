#pragma once

instant s16
UTF8_GetByteCount(
	const char *ch
) {
	/// amount of bytes used for a UTF-8 encoded character
	u8 utf_char_count = 0;

	     if (*ch == 0xED AND (ch[1] & 0xA0) == 0x0)  return -1; /// U+d800 to U+dfff
	else if (0x00 <= *ch AND *ch < 0x80) 	utf_char_count = 1;	/// 0bbbbbbb
	else if ((*ch & 0xE0) == 0xC0) 			utf_char_count = 2; /// 110bbbbb
	else if ((*ch & 0xF0) == 0xE0) 			utf_char_count = 3; /// 1110bbbb
	else if ((*ch & 0xF8) == 0xF0) 			utf_char_count = 4; /// 11110bbb
	else { return -1; };

	/// integrity check
	FOR((u64)utf_char_count - 1, it) {
		if ((ch[++it] & 0xC0) != 0x80)
			return -1;
	}

	return utf_char_count;
}

instant s32
UTF8_ToCodepoint(
	String *s_data,
	s32 *utf_byte_count = 0
) {
	Assert(s_data);

	String *s_char = s_data;
	s_char->is_reference = true;

    if (utf_byte_count)
		*utf_byte_count = 1;

	s32 it   = 0;
	s32 code = (u8)s_char->value[it];

	/// ascii
    if (code < 0x80)
		return code;

    /// bits after identifier
    const int bit_remain = 6;

	int bit_mask  = (1 << bit_remain) - 1;
    int bit_shift = 0;
    int bit_count = 0;

	s32 codepoint = 0;

	u64 it_rev = 0;

	FOR(3, it_rev) {
		/// check 11000000 identifier
		while((code & 0xC0) == 0xC0) {
			/// remove the leading '1' to check for '11' in the next iteration
			code <<= 1;
			code &= 0xff;

			bit_count += bit_remain;
			bit_mask >>= 1;
			++bit_shift;

			codepoint <<= bit_remain;
			codepoint |= s_char->value[++it - it_rev] & ((1 << bit_remain) - 1);

			if (utf_byte_count)
				*utf_byte_count += 1;
		}

		if (*utf_byte_count > 1)
			break;

		code = (u8)s_char->value[--it];

	}

	if (utf_byte_count)
		*utf_byte_count = -((s64)it_rev) + *utf_byte_count;

    codepoint |= ((code >> bit_shift) & bit_mask) << bit_count;

    return codepoint;
}
