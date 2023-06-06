#include "wall.h"
#include "rodeo.h"

static rodeo_collision_2d_collection_t collision_wall_world;
rodeo_gfx_texture_2d_t wall_texture;
rodeo_gfx_texture_2d_t floor_texture;
rodeo_gfx_texture_2d_t goat_texture;
rodeo_gfx_texture_2d_t logo_texture;

void
init_wall(void)
{
	wall_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/walls.png"));
	floor_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/floor.png"));
	goat_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/goat_on_a_pole.png"));
	logo_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/tojam2023_tagline_header_clear.png"));
	uint16_t window_width = 1600;
	uint16_t window_height = 900;
	collision_wall_world = rodeo_collision_2d_collection_create();
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
	rodeo_collision_2d_collection_destroy(collision_wall_world);
	rodeo_gfx_texture_2d_destroy(wall_texture);
	rodeo_gfx_texture_2d_destroy(floor_texture);
	rodeo_gfx_texture_2d_destroy(goat_texture);
	rodeo_gfx_texture_2d_destroy(logo_texture);
}

rodeo_collision_2d_collection_t
get_wall_world(void)
{
	return collision_wall_world;
}

rodeo_collision_2d_item_t
new_wall(
    float x,
    float y,
    float width,
    float height
)
{
	return rodeo_collision_2d_item_create(
			collision_wall_world,
			(rodeo_collision_2d_item_data_t)
			{
				.rect = {
					.x = x,
					.y = y,
					.width = width,
					.height = height
				}
			});
}

bool
coords_inside_wall(
    float x,
    float y
)
{
	
	c_foreach(i, cvec_collision_2d_item_data, collision_wall_world.data->collection) {
		if (x >= i.ref->rect.x && x <= i.ref->rect.x + i.ref->rect.width &&
				y >= i.ref->rect.y && y <= i.ref->rect.y + i.ref->rect.height) {
			return true;
		}
	}
	return false;
}

void
moving_wall_resolver(
	rodeo_collision_2d_item_data_t *obj_collision,
	rodeo_collision_2d_item_data_t *wall_collision
)
{
	rodeo_collision_2d_item_data_t *p = obj_collision;
	rodeo_collision_2d_item_data_t *w = wall_collision;
	rodeo_rectangle_t step = {
		.x = p->rect.x + p->dx * rodeo_gfx_frame_time_get() / (1000.0f/60.0f),
		.y = p->rect.y + p->dy * rodeo_gfx_frame_time_get() / (1000.0f/60.0f),
		.width = p->rect.width,
		.height = p->rect.height
	};
	rodeo_rectangle_t intersection = rodeo_collision_2d_get_collision_rect(step, w->rect);
	// left collision
	if (intersection.width < intersection.height) {
		// colliding left/right
		// if x equal push right
		if (intersection.x == step.x) {
			p->rect.x = w->rect.x + w->rect.width;
			if (p->dx < 0) {
				p->dx = 0;
			}
		// else push left
		} else {
			p->rect.x = w->rect.x - p->rect.width;
			if (p->dx > 0) {
				p->dx = 0;
			}
		}
	}
	else if (intersection.height < intersection.width) {
		// colliding up/down
		// if y equal push down
		if (intersection.y == step.y) {
			p->rect.y = w->rect.y + w->rect.height;
			if (p->dy < 0) {
				p->dy = 0;
			}
		// else push up
		} else {
			p->rect.y = w->rect.y - p->rect.height;
			if (p->dy > 0) {
				p->dy = 0;
			}
		}
	}
	// tunneled into a hitbox
	// don't allow movement
	else if (p->rect.width == w->rect.width && p->rect.height == w->rect.height) {
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
	rodeo_rectangle_t goat_dest = (rodeo_rectangle_t){(1600.0f/2)-(529.0f/2*0.07f),(900.0f/2)-(1038.0f/2*0.07f),529*0.07f,1038*0.07f};
	rodeo_gfx_texture_2d_draw(rect, rect, (rodeo_color_RGBAFloat_t){ .array = {0.96f, 0.41f, 0.1f, 1.0f} }, wall_texture);
	rodeo_gfx_texture_2d_draw(rect, rect, (rodeo_color_RGBAFloat_t){ .array = {0.52f, 0.31f, 0.73f, 0.33f} }, floor_texture);
	rodeo_gfx_texture_2d_draw(goat_dest, goat_size, (rodeo_color_RGBAFloat_t){ .array = {1,1,1,1} }, goat_texture);
	rodeo_gfx_texture_2d_draw(logo_dest, logo_size, (rodeo_color_RGBAFloat_t){ .array = {1,1,1,1} }, logo_texture);

}
