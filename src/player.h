#pragma once

#include "rodeo.h"
#include "rodeo/collision.h"
#include "input.h"

typedef
enum
{
	mv_state_standing,
	mv_state_jumping,
	mv_state_mid_air
}
move_state_t;

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

void
detect_player_enemy_collisions(void);


cvec_collision_2d_world_item_value *
get_player_position(void);
