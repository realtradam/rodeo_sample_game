#include "wall.h"
#include "rodeo.h"

static rodeo_collision_2d_world_t collision_wall_world;

void
init_wall(void)
{
	collision_wall_world = rodeo_collision_2d_world_create();
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

