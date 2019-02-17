#pragma once

struct Texture {
	u32 ID = 0;
	bool flip_h = false;
};

bool
operator == (
	Texture &texture_1,
	Texture &texture_2
) {
	if (texture_1.ID == texture_2.ID){
		return true;
	}

	return false;
}

instant void
Array_Destroy(
	Array<Tuple<Texture, Array<float>>> *array_out
) {
	Assert(array_out);

	FOR_ARRAY(*array_out, it) {
		Tuple<Texture, Array<float>> *t_tuple = &ARRAY_IT(*array_out, it);
		Array<float> *a_second = &t_tuple->second;

		Array_DestroyContainer(a_second);
	}

	Array_DestroyContainer(array_out);
}

instant bool
Texture_IsEmpty(
	Texture *texture
) {
	Assert(texture);

	return (texture->ID == 0);
}

instant void
Texture_GetSizeAndBind(
	Texture *texture,
	s32 *width_out,
	s32 *height_out
) {
    Assert(texture);

    glBindTexture(GL_TEXTURE_2D, texture->ID);
    if (width_out)  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH , width_out);
	if (height_out) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height_out);
}

instant Texture
Texture_Load(
	const u8 *data,
	s32 width,
	s32 height,
	u32 format_input,
	bool linearFilter
) {
	Texture result = {};

    u32 id_texture;

    glGenTextures(1, &id_texture);
    glBindTexture(GL_TEXTURE_2D, id_texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA , width, height, 0,
					format_input, GL_UNSIGNED_BYTE, data);

	s32 param;

	if (linearFilter) param = GL_LINEAR;
	else              param = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);

    result.ID = id_texture;

    return result;
}

instant void
Texture_Destroy(
	Texture *texture_out
) {
	Assert(texture_out);

	glDeleteTextures(1, &texture_out->ID);
	texture_out->ID = 0;
}

instant void
Texture_Reload(
	Texture *texture_io,
	String s_filename
) {
	Assert(texture_io);

	if (File_HasExtension(&s_filename, S(".bmp"))) {
		u32 format_input;

		/// 32-bit bmp only
		Image image = Image_LoadBMP32(s_filename);

		if (image.flip_h)  format_input = GL_ABGR_EXT;
		else               format_input = GL_RGBA;

		Texture_Destroy(texture_io);
		*texture_io = Texture_Load(image.data, image.width, image.height, format_input, false);
		texture_io->flip_h = image.flip_h;

		Image_Destroy(&image);

		return;
	}

	if (File_HasExtension(&s_filename, S(".jpg|.png|.gif"))) {
		char *tc_filename = String_CreateCBufferCopy(s_filename);

		s32 width, height, bits;
		u8 *c_data = stbi_load(tc_filename, &width, &height, &bits, 4);

		Texture_Destroy(texture_io);
		*texture_io = Texture_Load(c_data, width, height, GL_RGBA, true);

		Memory_Free(tc_filename);

		return;
	}
}

instant Texture
Texture_Load(
	String s_filename
) {
	Texture result;

	Texture_Reload(&result, s_filename);

	return result;
}

instant Texture
Texture_Create(
	s32 width,
	s32 height
) {
	return Texture_Load(0, width, height, GL_RGBA, false);
}

instant bool
Texture_IsEqual(
	Texture data1,
	Texture data2
) {
	return (data1.ID == data2.ID);
}
