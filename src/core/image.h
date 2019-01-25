#pragma once

struct Image {
	s32   width  = 0;
	s32   height = 0;
	u32   bits   = 0;
	u8   *data   = 0;
	bool  flip_h = false;
};

/// 32-bit BMP only!
instant Image
Image_LoadBMP32(
	const char *c_filename,
	u64 c_length = 0
) {
	Assert(c_filename);

	Image result = {};

	if (!File_Exists(c_filename, c_length)) {
		return result;
	}

	File file = File_Open(c_filename, "rb", c_length);

	String s_data = File_Read(&file);

	File_Close(&file);

    String s_data_it = s_data;

    if (String_StartWith(&s_data_it, "BM")) {
		BITMAPFILEHEADER *bmp_header = (BITMAPFILEHEADER *)s_data_it.value;
		s_data_it.value  += sizeof(BITMAPFILEHEADER);
		s_data_it.length -= sizeof(BITMAPFILEHEADER);
		BITMAPINFOHEADER *bmp_info = (BITMAPINFOHEADER *)s_data_it.value;

		///@Info: make sure you free the memory after usage!
		result.data   = Memory_Create(u8, bmp_info->biSizeImage);
		result.width  = bmp_info->biWidth;
		result.height = bmp_info->biHeight;
		result.bits   = bmp_info->biBitCount / 8;

		result.flip_h = (bmp_info->biHeight > 0);

		s_data_it = s_data;
		s_data_it.value  += bmp_header->bfOffBits;
		s_data_it.length -= bmp_header->bfOffBits;

		Memory_Copy(result.data, s_data_it.value, bmp_info->biSizeImage);
	}

	String_Destroy(&s_data);

	return result;
}

instant void
Image_Destroy(
	Image *image_out
) {
	Assert(image_out);

	Memory_Free(image_out->data);

	*image_out = {};
}