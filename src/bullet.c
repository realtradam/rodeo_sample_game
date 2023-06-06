#include "rodeo.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "wall.h"
#include "rodeo/collision.h"

static rodeo_gfx_texture_2d_t bullet_texture;
static rodeo_audio_sound_t pop_sound;
//static rodeo_collision_2d_collection_t bullet_collision_world;
static rodeo_collision_2d_collection_t player_bullet_collision_world = {0};
static rodeo_collision_2d_collection_t enemy_bullet_collision_world = {0};
static cvec_bullet_t bullets = {0};

void
init_bullets(void)
{
	player_bullet_collision_world = rodeo_collision_2d_collection_create();
	enemy_bullet_collision_world = rodeo_collision_2d_collection_create();
	bullet_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/bullet.png"));
	pop_sound = rodeo_audio_sound_create_from_path(cstr_lit("assets/pop.wav"));
}

void
deinit_bullets(void)
{
	rodeo_gfx_texture_2d_destroy(bullet_texture);
	rodeo_audio_sound_destroy(pop_sound);
	rodeo_collision_2d_collection_destroy(player_bullet_collision_world);
	rodeo_collision_2d_collection_destroy(enemy_bullet_collision_world);
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
	rodeo_collision_2d_collection_t bullet_world,
	rodeo_color_RGBAFloat_t color
)
{
	bullet_t bullet = {
		.color = color
	};
	bullet.item = rodeo_collision_2d_item_create(
		bullet_world,
		(rodeo_collision_2d_item_data_t){
			.rect = {
				.x = x,
				.y = y,
				.width = 25.0f,
				.height = 25.0f
			},
			.dx = dx,
			.dy = dy,
		}
	);
	return cvec_bullet_t_push(
		&bullets,
		bullet
	);
}

void
move_bullets(void)
{
	c_foreach(i, cvec_bullet_t, bullets) {
		rodeo_collision_2d_item_data_t *bullet = *(i.ref->item.data_handle);
		bullet->rect.x += bullet->dx;
		bullet->rect.y += bullet->dy;
	}
}

bullet_t*
get_bullet_by_id(
	rodeo_collision_2d_item_id_t id
)
{
	c_foreach(i, cvec_bullet_t, bullets) {
		if (i.ref->item.data_handle == id.self_handle) {
			return i.ref;
		}
	}
	return NULL;
}

void
draw_bullets(void)
{
	c_foreach(i, cvec_bullet_t, bullets)
	{
		rodeo_collision_2d_item_data_t *bullet = *(i.ref->item.data_handle);
		   rodeo_gfx_texture_2d_draw(
				bullet->rect,
				(rodeo_rectangle_t){
					.x = 0,
					.y = 0,
					.width = 25,
					.height = 25
				},
				i.ref->color,
				bullet_texture
				);
	   }
}


rodeo_collision_2d_collection_t
get_enemy_bullet_world(void)
{
	return enemy_bullet_collision_world;
}

rodeo_collision_2d_collection_t
get_player_bullet_world(void)
{
	return player_bullet_collision_world;
}

void
bullet_destroy(
	cvec_bullet_t_value* bullet
)
{
	bool player_is_moving =
		(*(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL)) != 0 ||
		 (*(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL)) != 0;
	if ((*bullet->item.data_handle)->id.collection.data == player_bullet_collision_world.data || !player_is_moving || get_player_hp() <= 0)
	{
		rodeo_audio_sound_play(pop_sound);
	}
	rodeo_collision_2d_item_destroy(bullet->item);
	*bullet = *cvec_bullet_t_back(&bullets);
    cvec_bullet_t_pop(&bullets);
}

void bullet_wall_resolver(
	rodeo_collision_2d_item_data_t *bullet_collision,
	rodeo_collision_2d_item_data_t *wall_collision
)
{
	bullet_t *bullet = get_bullet_by_id(bullet_collision->id);
	if (bullet == NULL) { return; }
	bullet_destroy(bullet);
}

void
detect_bullet_wall_collisions(void)
{
	rodeo_collision_2d_collection_compare_other(player_bullet_collision_world, get_wall_world(), bullet_wall_resolver);
	rodeo_collision_2d_collection_compare_other(enemy_bullet_collision_world, get_wall_world(), bullet_wall_resolver);
}

