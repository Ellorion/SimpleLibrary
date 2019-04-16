#include "src/SLib.h"
#include "test/core.h"

struct Grid {
	u16   x_count = 0;
	u16   y_count = 0;
	u64   width_px  = 0;
	u64   height_px = 0;

	u16   spacing = 1;
	float scale   = 1.0f;

	u16   grid_width  = 10;
	u16   grid_height = 10;

	Array<Rect>    a_rects;
	Array<Color32> a_colors;

	Array<Vertex>  a_vertex;
};

instant void
Grid_Update(
	Grid *grid,
	Color32 color
) {
	Assert(grid);

	/// create tiles
	Array_ClearContainer(&grid->a_rects);
	Array_ClearContainer(&grid->a_colors);

	Rect t_rect = {0, 0, grid->grid_width, grid->grid_height};

	FOR(grid->y_count, it_y) {
		t_rect.x = 0;
		t_rect.y = it_y * t_rect.h + (it_y * grid->spacing);

		FOR(grid->x_count, it_x) {
			t_rect.x = it_x * t_rect.w + (it_x * grid->spacing);

			Array_Add(&grid->a_rects , t_rect);
			Array_Add(&grid->a_colors, color);
		}
	}

	grid->width_px  = t_rect.x + t_rect.w;
	grid->height_px = t_rect.y + t_rect.h;

	/// create vertex array
	Array_Clear(&grid->a_vertex);

	Vertex *element;

	FOR_ARRAY(grid->a_rects, it) {
		Array_AddEmpty(&grid->a_vertex, &element);
		Vertex_Create(element);
		Vertex_AddRect32(element, ARRAY_IT(grid->a_rects, it), ARRAY_IT(grid->a_colors, it));
	}
}

instant Grid
Grid_Load(
	u16 x_count,
	u16 y_count,
	Color32 color
) {
	Grid grid;
	grid.x_count = x_count;
	grid.y_count = y_count;

	Grid_Update(&grid, color);

	return grid;
}

instant void
Grid_Destroy(
	Grid *grid
) {
	Assert(grid);

    Array_Destroy(&grid->a_vertex);
    Array_DestroyContainer(&grid->a_rects);
    Array_DestroyContainer(&grid->a_colors);
}

instant void
Grid_Render(
	ShaderSet *shader_set,
	Grid      *grid
) {
	Assert(shader_set);
	Assert(grid);

	float center_x = 0.0f;
	float center_y = 0.0f;

	if (shader_set->window) {
		center_x = (shader_set->window->width  - grid->width_px)  / 2;
		center_y = (shader_set->window->height - grid->height_px) / 2;
	}

	ShaderSet_Use(shader_set, SHADER_PROG_RECT);
	Shader_SetValue(shader_set, "scale_x", floor(grid->scale));
	Shader_SetValue(shader_set, "scale_y", floor(grid->scale));
	Shader_SetValue(shader_set, "x_offset", center_x);
	Shader_SetValue(shader_set, "y_offset", center_y);
    Vertex_Render(shader_set, &grid->a_vertex);
}

int main() {
	Keyboard keyboard;

	Window window = Window_Create("Grid-Example", 800, 480, true, true, &keyboard, 0);

	ShaderSet shader_set = ShaderSet_Create(&window);
	Grid      grid       = Grid_Load(10, 10, {0, 1, 1, 1});

	grid.grid_height = (window.height - 20) / grid.y_count;
	grid.grid_width  = (window.width  - 20) / grid.x_count;

	Grid_Update(&grid, {0, 0, 1, 1});

	MemorySegment_Add(&window.a_segments_reset, window.events);

	while(Window_IsRunning(&window)) {
		Window_ReadMessage(&window);

		if (window.events.on_size_changed)
			grid.scale = window.scale_y;

		if (keyboard.up[VK_ESCAPE])
			Window_Close(&window);

		OpenGL_ClearScreen();

		Grid_Render(&shader_set, &grid);
	}

	Grid_Destroy(&grid);
	ShaderSet_Destroy(&shader_set);
	Window_Destroy(&window);

	return 0;
}
