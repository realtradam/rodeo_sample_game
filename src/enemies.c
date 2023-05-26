#include "enemies.h"
#include "bullet.h"

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
	return cvec_enemy_t_push(&enemies, (enemy_t){.hp = 100.0, .id = id});
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

rodeo_collision_2d_world_t
get_enemies_world(void)
{
    return collision_enemies_world;
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
