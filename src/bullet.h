#pragma once

#include "rodeo.h"

typedef
struct
{
	world_id id;
	rodeo_color_RGBAFloat_t color;
}
bullet_t;

#define i_val bullet_t
#define i_opt c_no_cmp
#include "stc/cvec.h"

void
init_bullets(void);

void
deinit_bullets(void);

#define bullets_init_do() \
	mrodeo_defer_do( \
		init_bullets(), \
		deinit_bullets() \
	)

void
reset_bullets(void);

bullet_t *
spawn_bullet(
	float x,
	float y,
	float dx,
	float dy,
	rodeo_collision_2d_world_t *bullet_world,
	rodeo_color_RGBAFloat_t color
);

void
move_bullets(void);

bullet_t*
get_bullet_by_id(
	world_id id
);

void
draw_bullets(void);

rodeo_collision_2d_world_t *
get_enemy_bullet_world(void);

rodeo_collision_2d_world_t *
get_player_bullet_world(void);

void
bullet_destroy(
	cvec_bullet_t_value* bullet
);

void
detect_bullet_wall_collisions(void);
