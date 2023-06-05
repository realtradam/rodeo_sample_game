#include "rodeo.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "wall.h"
#include "rodeo/collision.h"

static rodeo_gfx_texture_2d_t bullet_texture;
static rodeo_audio_sound_t pop_sound;
//static rodeo_collision_2d_world_t bullet_collision_world;
static rodeo_collision_2d_world_t player_bullet_collision_world = {0};
static rodeo_collision_2d_world_t enemy_bullet_collision_world = {0};
static cvec_bullet_t bullets = {0};

void
init_bullets(void)
{
	bullet_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/bullet.png"));
	pop_sound = rodeo_audio_sound_create_from_path(cstr_lit("assets/pop.wav"));
}

void
deinit_bullets(void)
{
	rodeo_gfx_texture_2d_destroy(bullet_texture);
	rodeo_audio_sound_destroy(pop_sound);
	rodeo_collision_2d_world_destroy(&player_bullet_collision_world);
	rodeo_collision_2d_world_destroy(&enemy_bullet_collision_world);
	cvec_bullet_t_drop(&bullets);
}

void
reset_bullets(void)
{
	c_foreach_rv(i, cvec_bullet_t, bullets)
	{
		bullet_destroy(i.ref);
	}
}

bullet_t *
spawn_bullet(
	float x,
	float y,
	float dx,
	float dy,
	rodeo_collision_2d_world_t *bullet_world,
	rodeo_color_RGBAFloat_t color
)
{
	bullet_t bullet = {
		.color = color
	};
	bullet.id = rodeo_collision_2d_world_item_create(
		bullet_world,
		(rodeo_collision_2d_world_item_t){
			.rect = {
				.x = x,
				.y = y,
				.width = 25.0f,
				.height = 25.0f
			},
			.dx = dx,
			.dy = dy,
		}
	)->id;
	return cvec_bullet_t_push(
		&bullets,
		bullet
	);
}

void
move_bullets(void)
{
	c_foreach(i, cvec_collision_2d_world_item, player_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		bullet->rect.x += bullet->dx;
		bullet->rect.y += bullet->dy;
	}
	c_foreach(i, cvec_collision_2d_world_item, enemy_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		bullet->rect.x += bullet->dx;
		bullet->rect.y += bullet->dy;
	}

}

bullet_t*
get_bullet_by_id(
	world_id id
)
{
	c_foreach(i, cvec_bullet_t, bullets) {
		if (i.ref->id.id == id.id) {
			return i.ref;
		}
	}
	return NULL;
}

void
draw_bullets(void)
{
	c_foreach(i, cvec_collision_2d_world_item, player_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		bullet_t *bullet_obj = get_bullet_by_id(i.ref->id);
		   rodeo_gfx_texture_2d_draw(
				bullet->rect,
				(rodeo_rectangle_t){
					.x = 0,
					.y = 0,
					.width = 25,
					.height = 25
				},
				bullet_obj->color,
				bullet_texture
				);
	   }
	c_foreach(i, cvec_collision_2d_world_item, enemy_bullet_collision_world) {
		cvec_collision_2d_world_item_value *bullet = i.ref;
		bullet_t *bullet_obj = get_bullet_by_id(i.ref->id);
		   rodeo_gfx_texture_2d_draw(
				bullet->rect,
				(rodeo_rectangle_t){
					.x = 0,
					.y = 0,
					.width = 25,
					.height = 25
				},
				bullet_obj->color,
				bullet_texture
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

void
bullet_destroy(
	cvec_bullet_t_value* bullet
)
{
	bool player_is_moving =
		(*(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL)) != 0 ||
		 (*(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL)) != 0;
	if (bullet->id.world == &player_bullet_collision_world || !player_is_moving || get_player_hp() <= 0)
	{
		rodeo_audio_sound_play(pop_sound);
	}
	rodeo_collision_2d_world_item_destroy_by_id(bullet->id);
	*bullet = *cvec_bullet_t_back(&bullets);
    cvec_bullet_t_pop(&bullets);
}

void bullet_wall_resolver(
	rodeo_collision_2d_world_item_t *bullet_collision,
	rodeo_collision_2d_world_item_t *wall_collision
)
{
	bullet_t *bullet= get_bullet_by_id(bullet_collision->id);
	if (bullet == NULL) { return; }
	bullet_destroy(bullet);
}

void
detect_bullet_wall_collisions(void)
{
	rodeo_collision_2d_world_compare_other(&player_bullet_collision_world, get_wall_world(), bullet_wall_resolver);
	rodeo_collision_2d_world_compare_other(&enemy_bullet_collision_world, get_wall_world(), bullet_wall_resolver);
}

