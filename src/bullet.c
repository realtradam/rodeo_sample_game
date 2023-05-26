#include "rodeo.h"
#include "input.h"
#include "player.h"
#include "rodeo/collision.h"

static rodeo_texture_2d_t bullet_texture;
static rodeo_collision_2d_world_t bullet_collision_world;
static rodeo_collision_2d_world_t player_bullet_collision_world;
static rodeo_collision_2d_world_t enemy_bullet_collision_world;

void
init_bullets(void)
{
	bullet_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/bullet.png"));
}

void
deinit_bullets(void)
{
	rodeo_texture_2d_destroy(&bullet_texture);
}

void
move_bullets(void)
{
	c_foreach(i, cvec_collision_2d_world_item, player_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		bullet->x += bullet->dx;
		bullet->y += bullet->dy;
	}
	c_foreach(i, cvec_collision_2d_world_item, enemy_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		bullet->x += bullet->dx;
		bullet->y += bullet->dy;
	}

}

void
draw_bullets(void)
{
	c_foreach(i, cvec_collision_2d_world_item, player_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		   rodeo_texture_2d_draw(
				&(rodeo_rectangle_t){
					.x = bullet->x,
					.y = bullet->y,
					.width = bullet->width,
					.height = bullet->height,
				},
				&(rodeo_rectangle_t){
					.x = 0,
					.y = 0,
					.width = 25,
					.height = 25
				},
				NULL,
				&bullet_texture
				);
	   }
	c_foreach(i, cvec_collision_2d_world_item, enemy_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		   rodeo_texture_2d_draw(
				&(rodeo_rectangle_t){
					.x = bullet->x,
					.y = bullet->y,
					.width = bullet->width,
					.height = bullet->height,
				},
				&(rodeo_rectangle_t){
					.x = 0,
					.y = 0,
					.width = 25,
					.height = 25
				},
				NULL,
				&bullet_texture
				);
	   }
}

rodeo_collision_2d_world_t *
get_enemy_bullet_world(void)
{
	return &enemy_bullet_collision_world;
}

rodeo_collision_2d_world_t *
get_player_bullet_world(void)
{
	return &player_bullet_collision_world;
}
