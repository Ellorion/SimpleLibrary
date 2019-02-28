#pragma once

instant char *
Base64_Encode(
	const char *text
) {
	const char encodingTable[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int length = String_GetLength(text);
	char *encoded = Memory_Create(char, (length + 2) / 3 * 4 + 1);
	int enc_index = -1;

	for (int index = 0; index < length; index += 3) {
		u32 buffer =  (text[index + 0] << 16)
					| (text[index + 1] <<  8)
					| (text[index + 2]);

		/// 6 bits -> 0-63 * position in buffer & trunkated by 63 (0x3F)
		encoded[++enc_index] = encodingTable[(buffer >> (6 * 3) & 0x3F)];
		encoded[++enc_index] = encodingTable[(buffer >> (6 * 2) & 0x3F)];

		if (text[index + 1] == '\0') {
			encoded[++enc_index] = '=';
			encoded[++enc_index] = '=';
			break;
		}

		encoded[++enc_index] = encodingTable[(buffer >> (6 * 1) & 0x3F)];

		if (text[index + 2] == '\0') {
			encoded[++enc_index] = '=';
			break;
		}

		encoded[++enc_index] = encodingTable[(buffer >> (6 * 0) & 0x3F)];
	}

	return encoded;
}
