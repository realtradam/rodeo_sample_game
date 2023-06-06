#pragma once
#include "rodeo/collision.h"
#include "rodeo.h"

typedef
enum
{
	enemy_ai_nothing,
	enemy_ai_follow,
	//enemy_ai_runaway,
}
enemy_ai_behavior;

typedef
enum
{
	enemy_weapon_fourcross,
	enemy_weapon_basic,
	enemy_weapon_none,
}
enemy_weapon_type;

typedef
struct
{
    float hp;
    rodeo_collision_2d_item_t item;
	enemy_ai_behavior behavior;
	float move_speed;
	struct enemy_weapon
	{
		enemy_weapon_type type;
		float firerate;
		float cooldown;
	} weapon;

} enemy_t;

#define i_val enemy_t
#define i_opt c_no_cmp
#include "stc/cvec.h"

void
enemies_attempt_weapon_fire(void);

void
init_enemies(void);

void
deinit_enemies(void);

#define enemies_init_do() \
	mrodeo_defer_do( \
		init_enemies(), \
		deinit_enemies() \
	)

void
reset_enemies(void);

uint32_t
get_enemy_count(void);

uint32_t
get_ghost_count(void);

enemy_t*
spawn_enemy(
    float x,
    float y
);

enemy_t*
spawn_ghost(float x, float y);

void
draw_enemies(void);

void
move_enemies(void);

enemy_t*
get_enemy_by_id(
    rodeo_collision_2d_item_id_t id
);

rodeo_collision_2d_collection_t
get_enemies_world(void);

rodeo_collision_2d_collection_t
get_ghosts_world(void);

cvec_enemy_t
get_enemies_cvec(void);

void enemy_destroy(
	cvec_enemy_t_value* enemy 
);

void
detect_bullet_enemy_collisions(void);

void
group_follow_target(rodeo_collision_2d_item_t target);

enemy_t*
random_enemy_create(
	rodeo_rectangle_t bounds
);

enemy_t*
attempt_random_enemy_spawn(
	rodeo_rectangle_t bounds
);

void
detect_enemy_wall_collisions(void);
