#include "enemies.h"
#include "bullet.h"
#include "cglm/vec2.h"

static rodeo_collision_2d_world_t collision_enemies_world;
static rodeo_texture_2d_t enemy_texture;
static cvec_enemy_t enemies;

void
init_enemies(void)
{
	collision_enemies_world = rodeo_collision_2d_world_create();
	enemies = cvec_enemy_t_init();
	enemy_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/enemy.png"));
}

void
deinit_enemies(void)
{
	rodeo_collision_2d_world_destroy(&collision_enemies_world);
	cvec_enemy_t_drop(&enemies);
	rodeo_texture_2d_destroy(&enemy_texture);
}

enemy_t*
spawn_enemy(float x, float y)
{
	rodeo_collision_2d_world_item_t enemy_collision = (rodeo_collision_2d_world_item_t){.x = x, .y = y, .width = 26, .height = 38};
	world_id id = rodeo_collision_2d_world_item_create(&collision_enemies_world, enemy_collision)->id;
	return cvec_enemy_t_push(
		&enemies,
		(enemy_t){
			.hp = 100.0,
			.move_speed = 1.0f,
			.behavior = enemy_ai_follow,
			.firerate = 1.0f,
			.weapon = enemy_weapon_basic,
			.id = id
		});
}

void
draw_enemies(void)
{
	c_foreach(i, cvec_collision_2d_world_item, collision_enemies_world) {
		cvec_collision_2d_world_item_value *enemy = i.ref;
		
		   rodeo_texture_2d_draw(
				&(rodeo_rectangle_t){
					.x = enemy->x,
					.y = enemy->y,
					.width = enemy->width,
					.height = enemy->height,
				},
				&(rodeo_rectangle_t){
					.x = 0,
					.y = 0,
					.width = 26,
					.height = 38
				},
				NULL,
				&enemy_texture
				);
	}
}

void
enemy_overlap_resolver(
	rodeo_collision_2d_world_item_t *a,
	rodeo_collision_2d_world_item_t *b
)
{
	enemy_t *enemy_a = get_enemy_by_id(a->id);
	enemy_t *enemy_b = get_enemy_by_id(b->id);
	vec2 direction;
	glm_vec2_sub(
		(vec2){b->x, b->y},
		(vec2){a->x, a->y},
		direction
	);
	glm_vec2_normalize(direction);
	vec2 enemy_a_direction;
	vec2 enemy_b_direction;

	glm_vec2_scale(direction, -enemy_a->move_speed, enemy_a_direction);
	glm_vec2_scale(direction, enemy_b->move_speed, enemy_b_direction);

	a->dx += enemy_a_direction[0];
	a->dy += enemy_a_direction[1];
	b->dx += enemy_b_direction[0];
	b->dy += enemy_b_direction[1];
}

void
move_enemies(void)
{	
  
   rodeo_collision_2d_world_compare_self(get_enemies_world(), enemy_overlap_resolver);
	c_foreach(i, cvec_enemy_t, enemies) {
		rodeo_collision_2d_world_item_t *enemy = rodeo_collision_2d_world_item_get_by_id(i.ref->id);
		enemy->x += enemy->dx;
		enemy->dx = 0;
		enemy->y += enemy->dy;
		enemy->dy = 0;
	}
}

enemy_t*
get_enemy_by_id(
    world_id id
)
{
	c_foreach(i, cvec_enemy_t, enemies) {
		if (i.ref->id.id == id.id) {
			return i.ref;
		}
	}
	return NULL;
}

rodeo_collision_2d_world_t *
get_enemies_world(void)
{
    return &collision_enemies_world;
}

cvec_enemy_t
get_enemies_cvec(void)
{
	return enemies;
}

void enemy_destroy(
	cvec_enemy_t_value* enemy 
)
{
	rodeo_collision_2d_world_item_destroy_by_id(enemy->id);
	*enemy = *cvec_enemy_t_back(&enemies);
    cvec_enemy_t_pop(&enemies);

}

void
damage_enemy_resolver(
	rodeo_collision_2d_world_item_t *enemy_collision,
	rodeo_collision_2d_world_item_t *bullet_collision
)
{
	rodeo_collision_2d_world_item_destroy(bullet_collision);
	enemy_t *enemy = get_enemy_by_id(enemy_collision->id);
	if(enemy == NULL) { return; }
	enemy->hp -= 10;
	if (enemy->hp <= 0) {
		enemy_destroy(enemy);
	}
}

void
detect_bullet_enemy_collisions(void)
{
	rodeo_collision_2d_world_compare_other(&collision_enemies_world, get_player_bullet_world(), damage_enemy_resolver);
}

void
group_follow_target(rodeo_collision_2d_world_item_t *target)
{
	c_foreach(i, cvec_enemy_t, enemies) {
		rodeo_collision_2d_world_item_t *enemy = rodeo_collision_2d_world_item_get_by_id(i.ref->id);
		/*
		float direction[2] = {
			target->x - enemy->x,
			target->y - enemy->y
		};
		float mag = sqrtf(direction[0] * direction[0] + direction[1] * direction[1]);
		direction[0] /= mag;
		direction[1] /= mag;

		enemy->dx = direction[0];
		enemy->dy = direction[1];
		*/
		vec2 source = {
			enemy->x,
			enemy->y
		};
		vec2 dest = {
			target->x,
			target->y
		};
		vec2 direction;
		glm_vec2_sub(dest, source, direction);
		glm_vec2_normalize(direction);
		vec2 result;
		glm_vec2_scale(direction, get_enemy_by_id(enemy->id)->move_speed, result);

		rodeo_log(
			rodeo_logLevel_info,
			"%f, %f\n",
			result[0],
			result[1]
		);

		enemy->dx = result[0];
		enemy->dy = result[1];
	}
}
