#pragma once
#include "rodeo/collision.h"
#include "rodeo.h"

typedef
struct
{
    float hp;
    world_id id;
} enemy_t;

#define i_val enemy_t
#define i_opt c_no_cmp
#include "stc/cvec.h"


void
init_enemies(void);

void
deinit_enemies(void);

enemy_t*
spawn_enemy(
    float x,
    float y
);

void
draw_enemies(void);

enemy_t*
get_enemy_by_id(
    world_id id
);

rodeo_collision_2d_world_t *
get_enemies_world(void);

cvec_enemy_t
get_enemies_cvec(void);

void enemy_destroy(
	cvec_enemy_t_value* enemy 
);

void
detect_bullet_enemy_collisions(void);
