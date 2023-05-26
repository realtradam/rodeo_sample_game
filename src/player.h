#pragma once

#include "rodeo.h"
#include "rodeo/collision.h"
#include "input.h"

void
init_player(void);

void
deinit_player(void);

void
draw_player(void);

void
move_player(void);

void
parse_player_input(void);

void
player_shoot(rodeo_collision_2d_world_t *bullet_collision_world);
