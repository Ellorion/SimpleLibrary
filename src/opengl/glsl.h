#pragma once

enum SHADER_PROG_TYPE {
	SHADER_PROG_RECT = 0,
	SHADER_PROG_TEXT,
	SHADER_PROG_TEXTURE_FULL,
	SHADER_PROG_TEXTURE_SIZE,
	SHADER_PROG_TRIANGLE_STRIP,
	SHADER_PROG_TRIANGLE_FAN,
	SHADER_PROG_COUNT
};

struct Shader {
	SHADER_PROG_TYPE type;
	const char *code[3];
};

static const Shader shader_rect = {
	SHADER_PROG_RECT,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	uniform float offset_x = 0.0f;
	uniform float offset_y = 0.0f;

	in vec4 vertex_position;
	in vec4 rect_color;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec4 rect_color;
	} out_vertex;

	void main() {
		gl_Position = vec4(vertex_position.x + offset_x,
                           vertex_position.y + offset_y,
                           vertex_position.z + offset_x,
                           vertex_position.w + offset_y);

		out_vertex.proj_matrix  = proj_matrix;
		out_vertex.scale_matrix = scale_matrix;
		out_vertex.rect_color   = rect_color;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip) out;

	/// would show a disconnected section between 2 rects
	/// when using 4 vertices and nvidia gfx card (tested with 1050)
	/// so 6 vertices would overlap that area
	layout(max_vertices = 6) out;

	in Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec4 rect_color;
	} in_vertex[];

	out vec4 rect_color;

	void main() {
		vec4 pt           = gl_in[0].gl_Position;
		mat4 proj_matrix  = in_vertex[0].proj_matrix;
		mat4 scale_matrix = in_vertex[0].scale_matrix;

		mat4 matrix_mod = proj_matrix;

		vec4 v_pos_1 = matrix_mod * vec4(pt.x, pt.y, 0, 1) * scale_matrix;
		vec4 v_pos_2 = matrix_mod * vec4(pt.x, pt.w, 0, 1) * scale_matrix;
		vec4 v_pos_3 = matrix_mod * vec4(pt.z, pt.y, 0, 1) * scale_matrix;
		vec4 v_pos_4 = matrix_mod * vec4(pt.z, pt.w, 0, 1) * scale_matrix;

		// ---------------------------------
		gl_Position = v_pos_1;
		rect_color = in_vertex[0].rect_color;
		EmitVertex();

		gl_Position = v_pos_2;
		rect_color = in_vertex[0].rect_color;
		EmitVertex();

		gl_Position = v_pos_3;
		rect_color = in_vertex[0].rect_color;
		EmitVertex();

		// ---------------------------------
		gl_Position = v_pos_4;
		rect_color = in_vertex[0].rect_color;
		EmitVertex();

		gl_Position = v_pos_1;
		rect_color = in_vertex[0].rect_color;
		EmitVertex();

		// ---------------------------------

		gl_Position = v_pos_2;
		rect_color = in_vertex[0].rect_color;
		EmitVertex();

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	layout(origin_upper_left) in vec4 gl_FragCoord;

	in  vec4 rect_color;
	out vec4 out_frag_color;

	void main() {
		out_frag_color = rect_color;
	}
)"};

static const Shader shader_text = {
	SHADER_PROG_TEXT,
R"(
	#version 330 core

	uniform vec4  viewport = vec4(0, 0, 800, 480);
	uniform float scale_x  = 1.0f;
	uniform float scale_y  = 1.0f;

	uniform float offset_x = 0.0f;
	uniform float offset_y = 0.0f;

	in vec2 vertex_position;
	in vec3 text_color;
	in vec4 render_area;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec3 text_color;
		vec4 render_area;
	} o_Vertex;

	void main() {
		gl_Position = vec4( vertex_position.x + offset_x,
							vertex_position.y + offset_y,
							0,
							1);

		o_Vertex.proj_matrix  = proj_matrix;
		o_Vertex.scale_matrix = scale_matrix;
		o_Vertex.text_color   = text_color;
		o_Vertex.render_area  = render_area;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	uniform sampler2D fragment_texture;
	vec2 size = textureSize(fragment_texture, 0);

	in Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
		vec3 text_color;
		vec4 render_area;
	} i_Vertex[];

	out vec2 tex_coords;
	out vec3 text_color;
	out vec4 render_area;

	void main() {
		vec4 point        = gl_in[0].gl_Position;
		mat4 proj_matrix  = i_Vertex[0].proj_matrix;
		mat4 scale_matrix = i_Vertex[0].scale_matrix;

		vec4 v_pos_1 = proj_matrix * (point + vec4(0     , 0     , 0, 0) * scale_matrix);
		vec4 v_pos_2 = proj_matrix * (point + vec4(size.x, 0     , 0, 0) * scale_matrix);
		vec4 v_pos_3 = proj_matrix * (point + vec4(0     , size.y, 0, 0) * scale_matrix);
		vec4 v_pos_4 = proj_matrix * (point + vec4(size.x, size.y, 0, 0) * scale_matrix);

		/// v1 (top-left)
		gl_Position = v_pos_1;
		tex_coords = vec2(0, 0);
		text_color = i_Vertex[0].text_color;
		render_area = i_Vertex[0].render_area;
		EmitVertex();

		/// v2 (bottom-left)
		gl_Position = v_pos_2;
		tex_coords = vec2(1, 0);
		text_color = i_Vertex[0].text_color;
		render_area = i_Vertex[0].render_area;
		EmitVertex();

		/// v3 (top-right)
		gl_Position = v_pos_3;
		tex_coords = vec2(0, 1);
		text_color = i_Vertex[0].text_color;
		render_area = i_Vertex[0].render_area;
		EmitVertex();

		/// v4 (bottom-rights)
		gl_Position = v_pos_4;
		tex_coords = vec2(1, 1);
		text_color = i_Vertex[0].text_color;
		render_area = i_Vertex[0].render_area;
		EmitVertex();

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	uniform sampler2D fragment_texture;

	layout(origin_upper_left) in vec4 gl_FragCoord;

	in vec2 tex_coords;
	in vec3 text_color;
	in vec4 render_area;

	out vec4 out_frag_color;

	void main() {
		if (gl_FragCoord.x < render_area.x)  discard;
		if (gl_FragCoord.y < render_area.y)  discard;
		if (gl_FragCoord.x > render_area.x + render_area.z)  discard;
		if (gl_FragCoord.y > render_area.y + render_area.w)  discard;

		vec4 color_texture = texture2D(fragment_texture, tex_coords);
		out_frag_color = vec4(text_color.xyz, color_texture.a);
	}
)"};

static const Shader shader_texture_full = {
	SHADER_PROG_TEXTURE_FULL,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);

	in vec2 vertex_position;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
	} o_Vertex;

	void main() {
		gl_Position = vec4(vertex_position, 0, 1);
		o_Vertex.proj_matrix = proj_matrix;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	uniform sampler2D fragment_texture;
	vec2 size = textureSize(fragment_texture, 0);

	uniform bool  flip_h  = false;
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	in Vertex_Data {
		mat4 proj_matrix;
	} i_Vertex[];

	out Vertex_Data {
		vec2 tex_coords;
	} o_Vertex;

	mat4 scale_matrix = mat4(
		scale_x, 0      , 0, 0,
		0      , scale_y, 0, 0,
		0      , 0      , 1, 0,
		0      , 0      , 0, 1
	);

	void main() {
		vec4 point       = gl_in[0].gl_Position;
		mat4 proj_matrix = i_Vertex[0].proj_matrix;

		mat4 matrix_mod = proj_matrix;

		if (!flip_h) {
			/// v1
			gl_Position = matrix_mod * (point + vec4(0     , 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 0);
			EmitVertex();

			/// v3
			gl_Position = matrix_mod * (point + vec4(size.x, 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 0);
			EmitVertex();

			/// v2
			gl_Position = matrix_mod * (point + vec4(0     , size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 1);
			EmitVertex();

			/// v4
			gl_Position = matrix_mod * (point + vec4(size.x, size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 1);
			EmitVertex();
		}

		if (flip_h) {
			/// v1
			gl_Position = matrix_mod * (point + vec4(0     , 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 1);
			EmitVertex();

			/// v3
			gl_Position = matrix_mod * (point + vec4(size.x, 0     , 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 1);
			EmitVertex();

			/// v2
			gl_Position = matrix_mod * (point + vec4(0     , size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(0, 0);
			EmitVertex();

			/// v4
			gl_Position = matrix_mod * (point + vec4(size.x, size.y, 0, 0) * scale_matrix);
			o_Vertex.tex_coords = vec2(1, 0);
			EmitVertex();
		}

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	uniform sampler2D fragment_texture;

	in Vertex_Data {
		vec2 tex_coords;
	} i_Vertex;

	out vec4 out_frag_color;

	void main() {
		out_frag_color = texture2D(fragment_texture, i_Vertex.tex_coords);
	}
)"};

static const Shader shader_texture_size = {
	SHADER_PROG_TEXTURE_SIZE,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	in vec4 vertex_position;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
	} o_Vertex;

	void main() {
		gl_Position           = vertex_position;
		o_Vertex.proj_matrix  = proj_matrix;
		o_Vertex.scale_matrix = scale_matrix;
	}
)",

R"(
	#version 330 core

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	uniform bool flip_h = false;

	in Vertex_Data {
		mat4 proj_matrix;
		mat4 scale_matrix;
	} i_Vertex[];

	out vec2 tex_coords;

	void main() {
		vec4 pt           = gl_in[0].gl_Position;
		mat4 proj_matrix  = i_Vertex[0].proj_matrix;
		mat4 scale_matrix = i_Vertex[0].scale_matrix;

		mat4 matrix_mod = proj_matrix;

		vec4 pos_top_left     = matrix_mod * vec4(pt.x, pt.y, 0, 1) * scale_matrix;
		vec4 pos_top_right    = matrix_mod * vec4(pt.x, pt.w, 0, 1) * scale_matrix;
		vec4 pos_bottom_left  = matrix_mod * vec4(pt.z, pt.y, 0, 1) * scale_matrix;
		vec4 pos_bottom_right = matrix_mod * vec4(pt.z, pt.w, 0, 1) * scale_matrix;

		///@Info:
		/// position: right hand side
		/// u/v     : left  hand side
		///
		/// BOTH in triangle_strip order

		if (!flip_h) {
			gl_Position = pos_top_left;
			tex_coords = vec2(0, 0);
			EmitVertex();

			gl_Position = pos_bottom_left;
			tex_coords = vec2(1, 0);
			EmitVertex();

			gl_Position = pos_top_right;
			tex_coords = vec2(0, 1);
			EmitVertex();

			gl_Position = pos_bottom_right;
			tex_coords = vec2(1, 1);
			EmitVertex();
		}
		else {
			gl_Position = pos_top_left;
			tex_coords = vec2(0, 1);
			EmitVertex();

			gl_Position = pos_bottom_left;
			tex_coords = vec2(1, 1);
			EmitVertex();

			gl_Position = pos_top_right;
			tex_coords = vec2(0, 0);
			EmitVertex();

			gl_Position = pos_bottom_right;
			tex_coords = vec2(1, 0);
			EmitVertex();

		}

		EndPrimitive();
	}
)",

R"(
	#version 330 core

	layout(origin_upper_left) in vec4 gl_FragCoord;

	uniform sampler2D fragment_texture;

	in vec2 tex_coords;

	out vec4 out_frag_color;

	void main() {
		out_frag_color = texture2D(fragment_texture, tex_coords);
	}
)"};

static const Shader shader_triangle_strip = {
	SHADER_PROG_TRIANGLE_STRIP,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	uniform float offset_x = 0.0f;
	uniform float offset_y = 0.0f;

	in vec3 vertex_position;
	in vec4 vertex_color;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		vec4 color;
	} out_vertex;

	void main() {
		gl_Position = proj_matrix *
		              vec4(vertex_position.x + offset_x,
                           vertex_position.y + offset_y,
                           vertex_position.z,
                           1)
					  * scale_matrix;

		out_vertex.color = vertex_color;
	}
)",

0,

R"(
	#version 330 core

	layout(origin_upper_left) in vec4 gl_FragCoord;

	in Vertex_Data {
		vec4 color;
	} in_vertex;

	out vec4 out_frag_color;

	void main() {
		out_frag_color = in_vertex.color;
	}
)"};

static const Shader shader_triangle_fan = {
	SHADER_PROG_TRIANGLE_FAN,
R"(
	#version 330 core

	uniform vec4 viewport = vec4(0, 0, 800, 480);
	uniform float scale_x = 1.0f;
	uniform float scale_y = 1.0f;

	uniform float offset_x = 0.0f;
	uniform float offset_y = 0.0f;

	in vec3 vertex_position;
	in vec4 vertex_color;

	float left   = 0.0f;
	float right  = viewport.z;
	float top    = 0.0f;
	float bottom = viewport.w;

    mat4 proj_matrix = mat4(
		 2.0f / (right - left), 0                    ,  0,  0,
		 0                    , 2.0f / (top - bottom),  0,  0,
		 0                    , 0                    ,  1,  0,
		-(right + left)   / (right - left),
		-(top   + bottom) / (top   - bottom),
		 0,
		 1
	);

	mat4 scale_matrix = mat4(
		scale_x , 0      , 0, 0,
		0       , scale_y, 0, 0,
		0       , 0      , 1, 0,
		0       , 0      , 0, 1
	);

	out Vertex_Data {
		vec4 color;
	} out_vertex;

	void main() {
		gl_Position = proj_matrix *
		              vec4(vertex_position.x + offset_x,
                           vertex_position.y + offset_y,
                           vertex_position.z,
                           1)
					  * scale_matrix;

		out_vertex.color = vertex_color;
	}
)",

0,

R"(
	#version 330 core

	layout(origin_upper_left) in vec4 gl_FragCoord;

	in Vertex_Data {
		vec4 color;
	} in_vertex;

	out vec4 out_frag_color;

	void main() {
		out_frag_color = in_vertex.color;
	}
)"};
