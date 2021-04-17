#pragma once

instant String
Base64_Encode(
	String s_data
) {
	Assert(!String_IsEmpty(s_data));

	const char encodingTable[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	u64 length = s_data.length;

	String s_encoded = String_CreateBuffer((length + 2) / 3 * 4 + 1);

	s64 enc_index = -1;

	for (u64 index = 0; index < length; index += 3) {
		u32 buffer_0 =                       (s_data.value[index + 0] << 16);
		u32 buffer_1 = (index + 1 < length ? (s_data.value[index + 1] <<  8) : '\0');
		u32 buffer_2 = (index + 2 < length ? (s_data.value[index + 2] <<  0) : '\0');

		u32 buffer = buffer_0 | buffer_1 | buffer_2;

		/// 6 bits -> 0-63 * position in buffer & trunkated by 63 (0x3F)
		s_encoded.value[++enc_index] = encodingTable[(buffer >> (6 * 3) & 0x3F)];
		s_encoded.value[++enc_index] = encodingTable[(buffer >> (6 * 2) & 0x3F)];

		if (buffer_1 == '\0') {
			s_encoded.value[++enc_index] = '=';
			s_encoded.value[++enc_index] = '=';
			break;
		}

		s_encoded.value[++enc_index] = encodingTable[(buffer >> (6 * 1) & 0x3F)];

		if (buffer_2 == '\0') {
			s_encoded.value[++enc_index] = '=';
			break;
		}

		s_encoded.value[++enc_index] = encodingTable[(buffer >> (6 * 0) & 0x3F)];
	}

	return s_encoded;
}
