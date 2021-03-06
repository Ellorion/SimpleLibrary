#pragma once

struct ShaderProgram {
	SHADER_PROG_TYPE type;
	u32 id = 0;
	u32 vertex_id   = 0;
	u32 geometry_id = 0;
	u32 fragment_id = 0;
};

struct ShaderSet {
	Array<ShaderProgram> a_shaders;
	s64 active_id  = -1;
	Window *window =  0;
};

instant void
ShaderSet_SetWindow(
	ShaderSet &shader_set,
	Window &window
) {
	shader_set.window = &window;
}

instant void
ShaderProgram_Add(
	ShaderProgram *shader_prog_out,
	u32 type,
	const char *code
) {
	Assert(shader_prog_out);

    u32 id_shader = glCreateShader(type);

	glShaderSource(id_shader, 1, &code, 0);
	glCompileShader(id_shader);

	GLint result = GL_FALSE;
	s32 length_info_log;

	glGetShaderiv(id_shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id_shader, GL_INFO_LOG_LENGTH, &length_info_log);

	if (length_info_log) {
		/// make space for '\0'
		char *c_error_msg = Memory_Create(char, length_info_log + 1);
		glGetShaderInfoLog(id_shader, length_info_log, 0, c_error_msg);
		LOG_DEBUG(c_error_msg);
		Memory_Free(c_error_msg);
	}

	switch (type) {
		case GL_VERTEX_SHADER:   { shader_prog_out->vertex_id   = id_shader; } break;
		case GL_GEOMETRY_SHADER: { shader_prog_out->geometry_id = id_shader; } break;
		case GL_FRAGMENT_SHADER: { shader_prog_out->fragment_id = id_shader; } break;
		default: {
			LOG_DEBUG("Unimplemented shader type");
		}
	}
}

/// returns shader array index
instant u32
ShaderSet_Add(
	ShaderSet *shader_set_io,
	const Shader *shader
) {
	Assert(shader_set_io);
    Assert(shader);

	ShaderProgram *shader_prog;
	u64 array_id = Array_AddEmpty(shader_set_io->a_shaders, &shader_prog);

	AssertMessage(	array_id == shader->type,
					"[ShaderSet] Shader added out of SHADER_PROG_TYPE order.");

	if (shader->code[0])  ShaderProgram_Add(shader_prog, GL_VERTEX_SHADER  , shader->code[0]);
	if (shader->code[1])  ShaderProgram_Add(shader_prog, GL_GEOMETRY_SHADER, shader->code[1]);
	if (shader->code[2])  ShaderProgram_Add(shader_prog, GL_FRAGMENT_SHADER, shader->code[2]);

	shader_prog->id = glCreateProgram();

	if (shader->code[0])  glAttachShader(shader_prog->id, shader_prog->vertex_id);
	if (shader->code[1])  glAttachShader(shader_prog->id, shader_prog->geometry_id);
	if (shader->code[2])  glAttachShader(shader_prog->id, shader_prog->fragment_id);
	glLinkProgram(shader_prog->id);

	GLint result = GL_FALSE;
	s32 length_info_log;
	glGetProgramiv(shader_prog->id, GL_LINK_STATUS, &result);
	glGetProgramiv(shader_prog->id, GL_INFO_LOG_LENGTH, &length_info_log);

	if (length_info_log) {
		/// make space for '\0'
		char *c_error_msg = Memory_Create(char, length_info_log + 1);
		glGetProgramInfoLog(shader_prog->id, length_info_log, 0, c_error_msg);
		LOG_DEBUG(c_error_msg);
		Memory_Free(c_error_msg);
	}

	shader_prog->type = shader->type;

	return array_id;
}

instant ShaderSet
ShaderSet_Create(
	Window &window
) {
	ShaderSet shader_set;
	ShaderSet_SetWindow(shader_set, window);

	ShaderSet_Add(&shader_set, &shader_rect);
	ShaderSet_Add(&shader_set, &shader_text);
	ShaderSet_Add(&shader_set, &shader_texture_full);
	ShaderSet_Add(&shader_set, &shader_texture_size);
	ShaderSet_Add(&shader_set, &shader_triangle_strip);
	ShaderSet_Add(&shader_set, &shader_triangle_fan);

	AssertMessage(	shader_set.a_shaders.count == SHADER_PROG_COUNT,
					"[ShaderSet] Shader missing compared to SHADER_PROG_TYPE count.");

	return shader_set;
}

inline void
ShaderProgram_Destroy(
	ShaderProgram *shader_prog_out
) {
	Assert(shader_prog_out);

    if (shader_prog_out->id) {
		glDeleteProgram(shader_prog_out->id);
		glDeleteShader(shader_prog_out->vertex_id);
		glDeleteShader(shader_prog_out->geometry_id);
		glDeleteShader(shader_prog_out->fragment_id);

		*shader_prog_out = {};
    }
}

inline void
ShaderSet_Destroy(
	ShaderSet *shader_set_out
) {
    Assert(shader_set_out);

    FOR_ARRAY(shader_set_out->a_shaders, it) {
		ShaderProgram *t_shader_prog = &ARRAY_IT(shader_set_out->a_shaders, it);
		ShaderProgram_Destroy(t_shader_prog);
    }

    *shader_set_out = {};
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	const float buffer
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0)
		glProgramUniform1f(shader_prog->id, loc_id, buffer);
}

inline void
Shader_SetValue(
	ShaderSet   *shader_set,
	const char  *name,
	const float *buffer,
	u32 count
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0) {
		switch (count) {
			case 3: {
				glProgramUniform3fv(shader_prog->id, loc_id, 1, buffer);
			} break;

			case 4: {
				glProgramUniform4fv(shader_prog->id, loc_id, 1, buffer);
			} break;

			default: {
				Assert(!"Unknown uniform passing parameter count.");
			} break;
		}
	}
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	int value
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0)
		glProgramUniform1i(shader_prog->id, loc_id, value);
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	int value_a,
	int value_b
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0) {
		glProgramUniform2i(shader_prog->id, loc_id, value_a, value_b);
	}
}

inline void
Shader_SetValue(
	ShaderSet *shader_set,
	const char *name,
	float value_a,
	float value_b
) {
	Assert((u64)shader_set->active_id < shader_set->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set->a_shaders, shader_set->active_id);

	s32 loc_id = glGetUniformLocation(shader_prog->id, name);

	if (loc_id >= 0) {
		glProgramUniform2f(shader_prog->id, loc_id, value_a, value_b);
	}
}

inline void
Shader_BindAndUseIndex0(
	ShaderSet *shader_set,
	const char *name,
	Texture *texture
) {
	Assert(shader_set);
	Assert(name);
	Assert(texture);

	glBindTexture(GL_TEXTURE_2D, texture->ID);
	Shader_SetValue(shader_set, name, 0);
}

instant void
ShaderSet_UpdateViewport(
	ShaderSet *shader_set_io
) {
	if (!shader_set_io->window)
		return;

	RectF viewport;
	Window *t_window = shader_set_io->window;

	viewport.x = t_window->viewport_x;
	viewport.y = t_window->viewport_y;
	viewport.w = (float)t_window->width;
	viewport.h = (float)t_window->height;

	Shader_SetValue(shader_set_io, "viewport", (float *)&viewport, 4);

/// @Testing
//	Shader_SetValue(shader_set_io, "scale_x", shader_set_io->window->scale_x);
//	Shader_SetValue(shader_set_io, "scale_y", shader_set_io->window->scale_y);

	OpenGL_SetBlending(true);
}

instant void
ShaderSet_ResetUniforms(
	ShaderSet *shader_set
) {
	Assert(shader_set);

	Shader_SetValue(shader_set, "offset_x", 0.0f);
	Shader_SetValue(shader_set, "offset_y", 0.0f);
}

inline void
ShaderSet_Use(
	ShaderSet *shader_set_io,
	SHADER_PROG_TYPE type
) {
	Assert(shader_set_io);
	Assert(shader_set_io->window);

	s64 prev_active_id = shader_set_io->active_id;

	switch (type) {
		case SHADER_PROG_RECT:
		case SHADER_PROG_TEXT:
		case SHADER_PROG_TEXTURE_FULL:
		case SHADER_PROG_TEXTURE_SIZE:
		case SHADER_PROG_TRIANGLE_STRIP:
		case SHADER_PROG_TRIANGLE_FAN: {
			shader_set_io->active_id = type;
		} break;

		default: {
			AssertMessage(	false,
							"[ShaderSet] Unhandled Shader-Program type.");
		} break;
	}

/// @Testing
	ShaderSet_ResetUniforms(shader_set_io);

	if (shader_set_io->window AND shader_set_io->window->events.on_size_changed)
		ShaderSet_UpdateViewport(shader_set_io);

	if (prev_active_id == shader_set_io->active_id)
		return;

	Assert((u64)shader_set_io->active_id < shader_set_io->a_shaders.count);
	ShaderProgram *shader_prog = &ARRAY_IT(shader_set_io->a_shaders, shader_set_io->active_id);

	glUseProgram(shader_prog->id);

	ShaderSet_UpdateViewport(shader_set_io);
}
