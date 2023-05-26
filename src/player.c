#include "rodeo.h"
#include "input.h"
#include "player.h"
#include "enemies.h"
#include "rodeo/collision.h"
#include "sprite.h"

struct player_t
{
	sprite_t sprite;
	rodeo_texture_2d_t texture;
	int32_t hp;
	float damage_timer; //ms
	world_id collision_id;
}
player;

// 0-19 jumping
// 61 standing
// 20-60 mid iar

static rodeo_collision_2d_world_t player_collision_world;

static float orc_size[] = {13.0f * 2.0f, 19.0f * 2.0f};

void
init_player(void)
{
	player.texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/mainblob-128.png"));
	player.sprite.config.texture = &player.texture;
	player_collision_world = rodeo_collision_2d_world_create();
	player.collision_id = rodeo_collision_2d_world_item_create(
		&player_collision_world,
		(rodeo_collision_2d_world_item_t){
			.x = 100,
			.y = 100,
			.width = orc_size[0],
			.height = orc_size[1]
		}
	)->id;
	player.sprite = (sprite_t){
	.iter = 0,
	.config = {
		.texture = &player.texture,
		.width = 128,
		.height = 128,
		.count = 61
	}
	};
	player.hp = 100;
}

void
deinit_player(void)
{
	rodeo_texture_2d_destroy(&player.texture);
	rodeo_collision_2d_world_destroy(&player_collision_world);
}

void
draw_player(void)
{
	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	draw_sprite(&player.sprite, player_position->x, player_position->y, 0.25f);
	/*
	cvec_collision_2d_world_item_value *player = rodeo_collision_2d_world_item_get_by_id(player_collision_id);
	   rodeo_texture_2d_draw(
			&(rodeo_rectangle_t){
				.x = (float)(int32_t)player->x - (orc_size[0] / 2.0f),
				.y = (float)(int32_t)player->y - (orc_size[1] / 2.0f) ,
				.width = orc_size[0],
				.height = orc_size[1],
			},
			&(rodeo_rectangle_t){
				.x = 5,
				.y = 5,
				.width = 13,
				.height = 19
			},
			NULL,
			&player_texture
		);
	*/
}

void
parse_player_input(void)
{
	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	bool reset_movement = true;
	units_move_up_input(NULL, &reset_movement);
	units_move_down_input(NULL, &reset_movement);
	units_move_left_input(NULL, &reset_movement);
	units_move_right_input(NULL, &reset_movement);
	player_position->dx = *(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL);
	player_position->dy = *(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL);
}

void
move_player(void)
{
	player.sprite.iter += 1;
	player.sprite.iter %= player.sprite.config.count;
	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	player_position->x += player_position->dx;
	player_position->dx = 0;
	player_position->y += player_position->dy;
	player_position->dy = 0;
}

void
player_shoot(rodeo_collision_2d_world_t *bullet_collision_world)
{
	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	const uint32_t bullet_per_frame = 10;
	for(uint32_t i = 0; i < bullet_per_frame; ++i)
	{
		rodeo_collision_2d_world_item_create(
				bullet_collision_world,
				(rodeo_collision_2d_world_item_t){
				.x = (float)player_position->x - (orc_size[0] / 2.0f),
				.y = (float)player_position->y - (orc_size[1] / 2.0f),
				.dx = (float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				.dy = (float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				.width = 25.0f,
				.height = 25.0f
				}
				);
	}
}

void player_enemy_resolver(
	rodeo_collision_2d_world_item_t *player_collision,
	rodeo_collision_2d_world_item_t *enemy_collision
)
{
	if (player.hp <= 0) {
		rodeo_log(
			rodeo_logLevel_info,
			"player is dead"
		);
	} else if (player.damage_timer > 1000.0) {
		rodeo_log(
			rodeo_logLevel_info,
			"player health is now %d",
			player.hp
		);
		player.hp -= 10;
		player.damage_timer = 0;
	}

}

void
detect_player_enemy_collisions(void)
{
	player.damage_timer += rodeo_frame_time_get();
	rodeo_collision_2d_world_compare_other(&player_collision_world, get_enemies_world(), player_enemy_resolver);
}
