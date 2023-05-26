#pragma once

#include "rodeo.h"
#include "rodeo/collision.h"

void
init_bullets(void);

void
deinit_bullets(void);

void
move_bullets(void);

void
draw_bullets(void);

rodeo_collision_2d_world_t *
get_enemy_bullet_world(void);

rodeo_collision_2d_world_t *
get_player_bullet_world(void);
