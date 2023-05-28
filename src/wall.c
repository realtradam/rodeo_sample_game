#include "wall.h"
#include "rodeo.h"

static rodeo_collision_2d_world_t collision_wall_world;
rodeo_texture_2d_t wall_texture;
rodeo_texture_2d_t floor_texture;

void
init_wall(void)
{
	wall_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/walls.png"));
	floor_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/floor.png"));
	uint16_t window_width = 1600;
	uint16_t window_height = 900;
	collision_wall_world = rodeo_collision_2d_world_create();
	float walls[][4] = {
		{0, -10, window_width, 10},
		{0, window_height, window_width, 10},
		{-10, 0, 10, window_height},
		{window_width, 0, 10, window_height},
		{169,105,72,263},
		{241,105,191,73},
		{1169,105,262,72},
		{1359,177,72,190},
		{1169,723,262,72},
		{1359,533,72,190},
		{169,533,72,262},
		{241,723,191,72},
		{764,200,72,500},
		{550,414,501,72}
	};
	int numwalls = sizeof(walls)/sizeof(walls[0]);
	new_wall(0, -10, window_width, 10);
	new_wall(0, window_height, window_width, 10);
	new_wall(-10, 0, 10, window_height);
	new_wall(window_width, 0, 10, window_height);
	for (int i = 0; i < numwalls; ++i) {
		new_wall(walls[i][0], walls[i][1], walls[i][2], walls[i][3]);
	}
}

void
deinit_wall(void)
{
	rodeo_collision_2d_world_destroy(&collision_wall_world);
}

rodeo_collision_2d_world_t *
get_wall_world(void)
{
	return &collision_wall_world;
}

rodeo_collision_2d_world_item_t *
new_wall(
    float x,
    float y,
    float width,
    float height
)
{
	return rodeo_collision_2d_world_item_create(
			&collision_wall_world,
			(rodeo_collision_2d_world_item_t)
			{
				.x = x,
				.y = y,
				.width = width,
				.height = height
			});
}

bool
coords_inside_wall(
    float x,
    float y
)
{
	c_foreach(i, cvec_collision_2d_world_item, collision_wall_world) {
		if (x >= i.ref->x && x <= i.ref->x + i.ref->width &&
				y >= i.ref->y && y <= i.ref->y + i.ref->height) {
			return true;
		}
	}
	return false;
}

void
draw_level(void)
{
	rodeo_rectangle_t rect = (rodeo_rectangle_t){0,0,1600,900};
	rodeo_texture_2d_draw(&rect, &rect, NULL, &wall_texture);
	rodeo_texture_2d_draw(&rect, &rect, NULL, &floor_texture);

}
