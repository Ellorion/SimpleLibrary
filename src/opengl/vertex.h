#pragma once

template <typename T>
struct Vertex_Buffer {
	u32 id = 0;
	const char *name = 0;
	u32 group_count = 0;
	Array<T> a_buffer;
};

struct Vertex_Settings {
	bool  flip_h = false;
	float scale_x = 1.0f;
	float scale_y = 1.0f;
};

///@Hint: does not take texture ownership
///       so it will not be free'd when
///       destroyed
struct Vertex {
	u32 array_id = 0;
	Texture texture;
	Array<Vertex_Buffer<float>> a_attributes;
	Vertex_Settings settings;
};

bool
operator == (
	Vertex &v1,
	Vertex &v2
) {
	if (v1.texture.ID OR v2.texture.ID) {
		if (v1.texture == v2.texture) {
			return true;
		}

		return false;
	}

	if (v1.array_id == v2.array_id) {
		return true;
	}

	return false;
}

template <typename T>
bool
operator == (
	Vertex_Buffer<T> &b1,
	Vertex_Buffer<T> &b2
) {
	return String_IsEqual(b1.name, b2.name);
}

instant bool
Vertex_IsEmpty(
	Vertex *vertex
) {
	Assert(vertex);

	bool result = (vertex->a_attributes.count == 0);

	/// "vertex_position"
	Vertex_Buffer<float> *t_attribute = &ARRAY_IT(vertex->a_attributes, 0);

	result |= (t_attribute->a_buffer.count == 0);

	return result;
}

instant void
Vertex_GetTextureSize(
	Vertex *vertex,
	s32 *width_out,
	s32 *height_out
) {
	Assert(vertex);
	Assert(vertex->texture.ID);

	GLint id_bound = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &id_bound);

	bool need_bind_restoring = false;

	if (vertex->texture.ID != (u32)id_bound) {
		glBindTexture(GL_TEXTURE_2D, vertex->texture.ID);
		need_bind_restoring = true;
	}

    if (width_out)  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH , width_out);
	if (height_out) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height_out);

	if (need_bind_restoring) {
		glBindTexture(GL_TEXTURE_2D, id_bound);
		LOG_DEBUG("Waring [Performance]: temporary bindings will decrease performance.");
	}
}

inline void
Vertex_Destroy(
	Vertex *vertex_out
) {
	Assert(vertex_out);

	FOR_ARRAY(vertex_out->a_attributes, it) {
		Vertex_Buffer<float> *t_attribute = &ARRAY_IT(vertex_out->a_attributes, it);
		glDeleteBuffers(1, &t_attribute->id);
		Array_DestroyContainer(&t_attribute->a_buffer);
	}

	glDeleteVertexArrays(1, &vertex_out->array_id);
	Array_DestroyContainer(&vertex_out->a_attributes);

	*vertex_out = {};
}

inline void
Vertex_SetTexture(
	ShaderSet *shader_set,
	Vertex *vertex_out,
	Texture *texture
) {
	Assert(shader_set);
	Assert(vertex_out);
	Assert(texture);

	vertex_out->texture = *texture;
	vertex_out->settings.flip_h = texture->flip_h;

	/// use this uniform in a shader, if the texture index has to change
	Shader_BindAndUseIndex0(shader_set, "fragment_texture", &vertex_out->texture);
}

instant void
Vertex_Create(
	Vertex *vertex_out
) {
	Assert(vertex_out);
	Assert(vertex_out->array_id == 0);

	glGenVertexArrays(1, &vertex_out->array_id);
}

instant Vertex
Vertex_Create(
) {
	Vertex vertex = {};

	Vertex_Create(&vertex);

	return vertex;
}

instant Vertex
Vertex_Create(
	ShaderSet *shader_set,
	Texture *texture
) {
	Assert(shader_set);
	Assert(texture);

	Vertex vertex = {};

	if (texture->ID) {
		Vertex_SetTexture(shader_set, &vertex, texture);
	}

	glGenVertexArrays(1, &vertex.array_id);

	return vertex;
}

instant void
Vertex_Load(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(vertex);

	AssertMessage(shader_set->active_id >= 0, "[Vertex] No Shader initialized.");
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);

	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	FOR_ARRAY(vertex->a_attributes, it) {
		Vertex_Buffer<float> *entry = &ARRAY_IT(vertex->a_attributes, it);
		s32 attrib_position = glGetAttribLocation(shader_prog->id, entry->name);

		if (attrib_position < 0) {
			String s_error;
			String_Append(&s_error, "[Vertex] Shader and attributes mismatch.\n    Missing: ");
			String_Append(&s_error, entry->name);

			char *c_error_msg = String_CreateCBufferCopy(s_error.value, s_error.length);

			AssertMessage(false, c_error_msg);

			Memory_Free(c_error_msg);

			continue;
		}

		glEnableVertexAttribArray(attrib_position);
		glBindBuffer(GL_ARRAY_BUFFER, entry->id);
		glVertexAttribPointer(
			attrib_position,
			entry->group_count,
			GL_FLOAT,
			false,
			0,
			0
		);
	}

	if (vertex->texture.ID) {
		Vertex_SetTexture(shader_set, vertex, &vertex->texture);
	}

}

inline void
Vertex_BindAttributes(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(vertex);
	Assert(vertex->a_attributes.count);

	FOR_ARRAY(vertex->a_attributes, it_buffer) {
		Vertex_Buffer<float> *t_buffer = &ARRAY_IT(vertex->a_attributes, it_buffer);

		if (!t_buffer->id)
			glGenBuffers(1, &t_buffer->id);

		glBindBuffer(GL_ARRAY_BUFFER, t_buffer->id);
		glBufferData(GL_ARRAY_BUFFER, t_buffer->a_buffer.count * sizeof(float), t_buffer->a_buffer.memory, GL_DYNAMIC_DRAW);
	}

	Vertex_Load(shader_set, vertex);
}

instant void
Vertex_ClearAttributes(
	Vertex *vertex_out
) {
	Assert(vertex_out);

	FOR_ARRAY(vertex_out->a_attributes, it) {
		auto *t_attribute = &ARRAY_IT(vertex_out->a_attributes, it);
		Array_ClearContainer(&t_attribute->a_buffer);
	}
}

instant void
Vertex_ClearAttributes(
	Array<Vertex> *a_vertex_out
) {
	Assert(a_vertex_out);

	FOR_ARRAY(*a_vertex_out, it_vertex) {
		Vertex *t_vertex = &ARRAY_IT(*a_vertex_out, it_vertex);

		FOR_ARRAY(t_vertex->a_attributes, it_attrib) {
			auto *t_attribute = &ARRAY_IT(t_vertex->a_attributes, it_attrib);
			Array_ClearContainer(&t_attribute->a_buffer);
		}
	}
}

inline void
Vertex_Render(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(vertex);
	Assert(vertex->a_attributes.count);

	///@Hint: vertex positions have to be the first entry in the array
	Vertex_Buffer<float> *a_positions = &ARRAY_IT(vertex->a_attributes, 0);

	Assert(String_IsEqual(a_positions->name, "vertex_position"));

	AssertMessage(	vertex->array_id,
					"[Vertex] Vertex has not been created. Forgot to call Vertex_Create?");

	AssertMessage(	a_positions->id,
					"[Vertex] No Attributes found. Forgot to bind the attributes?");

	Assert(a_positions->group_count);

	Shader_SetValue(shader_set, "flip_h" , vertex->settings.flip_h);
	Shader_SetValue(shader_set, "scale_x", vertex->settings.scale_x);
	Shader_SetValue(shader_set, "scale_y", vertex->settings.scale_y);

	glDrawArrays(GL_POINTS, 0, a_positions->a_buffer.count / a_positions->group_count);
}

instant void
Vertex_Render(
	ShaderSet *shader_set,
	Array<Vertex> *a_vertex
) {
	Assert(shader_set);
	Assert(a_vertex);

	FOR_ARRAY(*a_vertex, it) {
		Vertex *t_vertex = &ARRAY_IT(*a_vertex, it);
		Vertex_BindAttributes(shader_set, t_vertex);

		Vertex_Render(shader_set, t_vertex);
	}
}

instant void
Vertex_FindOrAdd(
	Array<Vertex> *a_vertex_io,
	Texture *texture_find,
	Vertex **entry_out
) {
	Assert(a_vertex_io);
	Assert(texture_find);
	Assert(entry_out);

	Vertex *t_vertex_entry;
	Vertex t_vertex_find;
	t_vertex_find.texture = *texture_find;

	Array_FindOrAdd(a_vertex_io, t_vertex_find, &t_vertex_entry);

	if (!t_vertex_entry->array_id)
		Vertex_Create(t_vertex_entry);

	*entry_out = t_vertex_entry;
}

instant void
Vertex_FindOrAddAttribute(
	Vertex *vertex_io,
	u32 group_count,
	const char *c_attribute_name,
	Vertex_Buffer<float> **a_buffer_out
) {
	Assert(vertex_io);
	Assert(c_attribute_name);
	Assert(a_buffer_out);

	Vertex_Buffer<float> t_attribute_find;
	t_attribute_find.name = c_attribute_name;
	t_attribute_find.group_count = group_count;

	Array_FindOrAdd(&vertex_io->a_attributes, t_attribute_find, a_buffer_out);
}

instant void
Vertex_AddTexturePosition(
	Vertex *vertex_io,
	float x,
	float y
) {
	Assert(vertex_io);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex_io, 2, "vertex_position", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 2);
	Array_Add(&t_attribute->a_buffer, x);
	Array_Add(&t_attribute->a_buffer, y);
}

instant void
Vertex_AddRect32(
	Vertex *vertex_io,
	Rect rect,
	Color32 color
) {
	Assert(vertex_io);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex_io, 4, "vertex_position", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 4);
	Array_Add(&t_attribute->a_buffer, (float)rect.x);
	Array_Add(&t_attribute->a_buffer, (float)rect.y);
	Array_Add(&t_attribute->a_buffer, (float)rect.x + rect.w);
	Array_Add(&t_attribute->a_buffer, (float)rect.y + rect.h);

	Vertex_FindOrAddAttribute(vertex_io, 4, "rect_color", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 4);
	Array_Add(&t_attribute->a_buffer, (float)color.r);
	Array_Add(&t_attribute->a_buffer, (float)color.g);
	Array_Add(&t_attribute->a_buffer, (float)color.b);
	Array_Add(&t_attribute->a_buffer, (float)color.a);
}

instant void
Vertex_AddRectTexture(
	Vertex *vertex_io,
	Rect rect
) {
	Assert(vertex_io);

	Vertex_Buffer<float> *t_attribute;

	Vertex_FindOrAddAttribute(vertex_io, 4, "vertex_position", &t_attribute);
	Array_Reserve(&t_attribute->a_buffer, 4);
	Array_Add(&t_attribute->a_buffer, (float)rect.x);
	Array_Add(&t_attribute->a_buffer, (float)rect.y);
	Array_Add(&t_attribute->a_buffer, (float)rect.x + rect.w);
	Array_Add(&t_attribute->a_buffer, (float)rect.y + rect.h);
}

instant void
Rect_Render(
	ShaderSet *shader_set,
	Vertex *vertex
) {
	Assert(shader_set);
	Assert(shader_set->window);
	Assert(vertex);

	if (vertex->a_attributes.count) {
		Vertex_BindAttributes(shader_set, vertex);
		Vertex_Render(shader_set, vertex);
	}
	else {
		LOG_WARNING("No rectangle data to render set");
	}
}
