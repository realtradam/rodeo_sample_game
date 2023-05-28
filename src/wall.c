#include "wall.h"
#include "rodeo.h"

static rodeo_collision_2d_world_t collision_wall_world;
rodeo_texture_2d_t wall_texture;
rodeo_texture_2d_t floor_texture;
rodeo_texture_2d_t goat_texture;
rodeo_texture_2d_t logo_texture;

void
init_wall(void)
{
	wall_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/walls.png"));
	floor_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/floor.png"));
	goat_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/goat_on_a_pole.png"));
	logo_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/tojam2023_tagline_header_clear.png"));
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
moving_wall_resolver(
	rodeo_collision_2d_world_item_t *obj_collision,
	rodeo_collision_2d_world_item_t *wall_collision
)
{
	rodeo_collision_2d_world_item_t *p = obj_collision;
	rodeo_collision_2d_world_item_t *w = wall_collision;
	rodeo_rectangle_t step = (rodeo_rectangle_t){
		.x = p->x + p->dx * rodeo_frame_time_get(),
		.y = p->y + p->dy * rodeo_frame_time_get(),
		.width = p->width,
		.height = p->height
	};
	rodeo_rectangle_t intersection = rodeo_collision_2d_get_collision_rect(p, w);
	if (intersection.width < intersection.height) {
		if (intersection.x == step.x) {
			p->x = w->x + w->width;
			if (p->dx < 0) {
				p->dx = 0;
			}
		} else {
			p->x = w->x - p->width;
			if (p->dx > 0) {
				p->dx = 0;
			}
		}
	}
	else if (intersection.height < intersection.width) {
		if (intersection.y == step.y) {
			p->y = w->y + w->height;
			if (p->dy < 0) {
				p->dy = 0;
			}
		} else {
			p->y = w->y - p->height;
			if (p->dy > 0) {
				p->dy = 0;
			}
		}
	}
	else if (p->width == w->width && p->height == w->height) {
		p->dx = 0;
		p->dy = 0;
	}
}

void
draw_level(void)
{
	rodeo_rectangle_t rect = (rodeo_rectangle_t){0,0,1600,900};
	rodeo_rectangle_t logo_size = (rodeo_rectangle_t){0,0,1024,400};
	rodeo_rectangle_t logo_dest = (rodeo_rectangle_t){1600-(1024*0.25f),10,1024*0.25f,400*0.25f};
	rodeo_rectangle_t goat_size = (rodeo_rectangle_t){0,0,529,1038};
	rodeo_rectangle_t goat_dest = (rodeo_rectangle_t){(1600.0f/2)-(529/2*0.07f),(900.0f/2)-(1038/2*0.07f),529*0.07f,1038*0.07f};
	rodeo_texture_2d_draw(&rect, &rect, &(rodeo_color_RGBAFloat_t){ .array = {0.96f, 0.41f, 0.1f, 1.0f} }, &wall_texture);
	rodeo_texture_2d_draw(&rect, &rect, &(rodeo_color_RGBAFloat_t){ .array = {0.52f, 0.31f, 0.73f, 0.33f} }, &floor_texture);
	rodeo_texture_2d_draw(&goat_dest, &goat_size, &(rodeo_color_RGBAFloat_t){ .array = {1,1,1,1} }, &goat_texture);
	rodeo_texture_2d_draw(&logo_dest, &logo_size, &(rodeo_color_RGBAFloat_t){ .array = {1,1,1,1} }, &logo_texture);

}
