#include "rodeo.h"
#include "input.h"
#include "player.h"
#include "enemies.h"
#include "rodeo/collision.h"
#include "bullet.h"
#include "sprite.h"
#include "wall.h"
#include "cglm/vec2.h"
#include "wall.h"

#include <math.h>

struct player_t
{
	sprite_t sprite;
	rodeo_texture_2d_t texture;
	rodeo_texture_2d_t shadow_texture;
	rodeo_texture_2d_t aim_texture;
	rodeo_texture_2d_t heart_texture;
	int32_t hp;
	float damage_timer; //ms
	float damage_cooldown_rate;
	world_id collision_id;
	move_state_t move_state;
	struct player_weapon
	{
		float firerate;
		float cooldown;
		float spread;
	} weapon;
}
player = { 0 };

typedef struct player_t player_t;
static rodeo_audio_sound_t *bubbles_sound;

// 0-19 jumping
// 61 standing
// 20-60 mid iar

static rodeo_collision_2d_world_t player_collision_world;
static aim_position_t aim_position;

static float orc_size[] = {13.0f * 2.0f, 19.0f * 2.0f};

void
init_player(void)
{
	bubbles_sound = rodeo_audio_sound_create_from_path(cstr_lit("assets/blowing_bubbles.wav"));
	player.texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/mainblob-128.png"));
	player.shadow_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/blobshadow.png"));
	player.aim_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/aim.png"));
	player.heart_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/heart.png"));
	player.sprite.config.texture = &player.texture;
	player_collision_world = rodeo_collision_2d_world_create();
	player.collision_id = rodeo_collision_2d_world_item_create(
		&player_collision_world,
		(rodeo_collision_2d_world_item_t){
			.x = 1600/2,
			.y = 900/2,
			.width = orc_size[0],
			.height = orc_size[1]
		}
	)->id;
	player.sprite = (sprite_t){
	.iter = 60,
	.config = {
		.texture = &player.texture,
		.width = 128,
		.height = 128,
		.count = 61
	}
	};
	player.hp = 100;
	player.weapon.cooldown = 0;
	player.weapon.firerate = 0.1f;
	player.weapon.spread = 0.425f;
	aim_position.x = 101;
	aim_position.y = 100;
	player.damage_cooldown_rate = 1000.0f;
}

void
deinit_player(void)
{
	rodeo_texture_2d_destroy(&player.texture);
	rodeo_collision_2d_world_destroy(&player_collision_world);
	rodeo_audio_sound_destroy(bubbles_sound);
}

void
draw_player(void)
{
	float transparency = 1.0f;
	if(!(player.damage_timer >= player.damage_cooldown_rate || (rodeo_frame_count_get() % 8 < 4)))
	{
		transparency = 0.33f;
	}
	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	const float scale = 0.25f;
	draw_aim(aim_position.x, aim_position.y, scale);
	rodeo_texture_2d_draw(
		&(rodeo_rectangle_t){
			.x = player_position->x,// - ((float)sprite->config.width * scale / 2),
			.y = player_position->y + (96 * scale),// - ((float)sprite->config.height * scale / 2),
			.width = 128 * scale,
			.height = (13.0f / 30.0f * 128.0f) * scale,
		},
		&(rodeo_rectangle_t){ 
			.width = 30,
			.height = 13
		},
		&(rodeo_color_RGBAFloat_t){ .array = { 1.0f, 1.0f, 1.0f, 1.0f } },
		&player.shadow_texture
	);
	draw_sprite(&player.sprite, player_position->x, player_position->y, scale, (rodeo_color_RGBAFloat_t){ .array = {1,1,1,transparency} });
}

void
draw_hp_bar(void)
{
	for(int i = 0; i < (player.hp / 10); ++i)
	{
		/*
		rodeo_log(
				rodeo_logLevel_info,
				"%f\n",
				10.0f + (40.0f * (float)i)
				);*/
		rodeo_texture_2d_draw(
				&(rodeo_rectangle_t){ .x = 10.0f + (40.0f * (float)i), .y = 10, .width = 35, .height = 35 },
				&(rodeo_rectangle_t){ .x = 0, .y = 0, .width = 35, .height = 35 },
				NULL,
				&player.heart_texture);
	}
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

	// if you arent mid jump, then record attempted movement
	if(player.move_state != mv_state_jumping)
	{
		vec2 inputs = {
			(*(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL)),
			 (*(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL))
		};
		if(glm_vec2_norm2(inputs) > 1)
		{
			glm_vec2_normalize(inputs);
		}
		player_position->dx = inputs[0] * (rodeo_frame_time_get() / (1000.0f/60.0f) * ((60.0f - (float)player.sprite.iter) / 60.0f)) * 7;
		player_position->dy = inputs[1] * (rodeo_frame_time_get() / (1000.0f/60.0f) * ((60.0f - (float)player.sprite.iter) / 60.0f)) * 7;

		if(((inputs[0] != 0) || (inputs[1] != 0)) && player.move_state == mv_state_standing)
		{
			player.move_state = mv_state_jumping;
			player_position->dy = 0;
			player_position->dx = 0;
		}
	}
}



void
move_player(void)
{
	if(player.move_state != mv_state_standing)
	{
		player.sprite.iter += 1 * rodeo_frame_time_get() / (1000.0f/60.0f);
		player.sprite.iter = fmodf(player.sprite.iter, (float)player.sprite.config.count);
		if(player.sprite.iter > 19)
		{
			player.move_state = mv_state_mid_air;
		}
	}
	if((uint32_t)player.sprite.iter == 60)
	{
		player.move_state = mv_state_standing;
	}
	if((uint32_t)player.sprite.iter == 1)
	{
		rodeo_audio_sound_play(bubbles_sound);
	}

	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	player_position->x += player_position->dx;
	player_position->dx = 0;
	player_position->y += player_position->dy;
	player_position->dy = 0;
	update_aim_position();
}

void
player_shoot(rodeo_collision_2d_world_t *bullet_collision_world)
{

	if(player.weapon.cooldown > 0)
	{
		player.weapon.cooldown -= rodeo_frame_time_get() / 1000;
	}
	if(player.move_state == mv_state_mid_air)
	{
	while(player.weapon.cooldown <= 0)
	{
		player.weapon.cooldown += player.weapon.firerate;
		cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);

		vec2 direction_vec;

		glm_vec2_sub(
				(vec2){aim_position.x, aim_position.y},
				(vec2){player_position->x, player_position->y},
				direction_vec
				);
		glm_vec2_normalize(direction_vec);

		//for(uint32_t i = 0; i < bullet_per_frame; ++i)
		//{
		vec2 result_vec;
		glm_vec2_copy(direction_vec, result_vec);

		vec2 rand = {
			((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * player.weapon.spread, 
			((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * player.weapon.spread, 
		};

		result_vec[0] += rand[0];
		result_vec[1] += rand[1];
		glm_vec2_normalize(result_vec);
		glm_vec2_scale(result_vec, 5.0f, result_vec);

		spawn_bullet(
				(float)player_position->x + (orc_size[0] / 2.0f) - 9.0f,
				(float)player_position->y + (orc_size[1] / 2.0f) - 16.0f,
				(float)(result_vec[0]),
				(float)(result_vec[1]),
				//(float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				//(float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				bullet_collision_world,
				(rodeo_color_RGBAFloat_t){ .array = { 1,1,1,1 } }
				);
	}
	}
	}

void player_enemy_resolver(
	rodeo_collision_2d_world_item_t *player_collision,
	rodeo_collision_2d_world_item_t *enemy_collision
)
{
	if (player.hp <= 0) {
		/*rodeo_log(
			rodeo_logLevel_info,
			"player is dead"
		);*/
	} else if (player.damage_timer >= player.damage_cooldown_rate) {
		rodeo_log(
			rodeo_logLevel_info,
			"player health is now %d",
			player.hp
		);
		player.hp -= 10;
		player.damage_timer = 0;
		enemy_t *enemy = get_enemy_by_id(enemy_collision->id);
		enemy_destroy(enemy);
	}

}

void player_bullet_resolver(
	rodeo_collision_2d_world_item_t *player_collision,
	rodeo_collision_2d_world_item_t *bullet_collision
)
{
	if (player.hp <= 0) {
		/*rodeo_log(
			rodeo_logLevel_info,
			"player is dead"
		);*/
	} else if (player.damage_timer >= player.damage_cooldown_rate) {
		rodeo_log(
			rodeo_logLevel_info,
			"player health is now %d",
			player.hp
		);
		player.hp -= 10;
		player.damage_timer = 0;
	}
	bullet_t *bullet = get_bullet_by_id(bullet_collision->id);
	bullet_destroy(bullet);

}

void
detect_player_enemy_collisions(void)
{
	player.damage_timer += rodeo_frame_time_get();
	rodeo_collision_2d_world_compare_other(&player_collision_world, get_enemies_world(), player_enemy_resolver);
	rodeo_collision_2d_world_compare_other(&player_collision_world, get_enemy_bullet_world(), player_bullet_resolver);
}

void
detect_player_wall_collisions(void)
{
	rodeo_collision_2d_world_compare_other(&player_collision_world, get_wall_world(), moving_wall_resolver);
}

cvec_collision_2d_world_item_value *
get_player_position(void)
{
		return rodeo_collision_2d_world_item_get_by_id(player.collision_id);
}

void
update_aim_position(void)
{
	cvec_collision_2d_world_item_value *player_position = get_player_position();
	vec2 player_position_vec = { player_position->x, player_position->y };
	vec2 aim_position_vec = { aim_position.x, aim_position.y };
	float distance = glm_vec2_distance2(player_position_vec, aim_position_vec);
	// move towards player
	if(distance > (25.0f * 25.0f))
	{
		vec2 direction_vec;
		glm_vec2_sub(player_position_vec, aim_position_vec, direction_vec);
		glm_vec2_normalize(direction_vec);
		glm_vec2_scale(direction_vec, -25.0f, direction_vec);
		glm_vec2_add(player_position_vec, direction_vec, direction_vec);
		aim_position.x = direction_vec[0];
		aim_position.y = direction_vec[1];
	}
}

void
draw_aim(float player_x, float player_y, float scale)
{
	rodeo_texture_2d_draw(
			&(rodeo_rectangle_t){
				//.x = player_x - (((128.0f * scale) * 1.3f) - (128.0f * scale) ),
				.x = player_x - (((128.0f * scale) * 0.6f) / 2),
				.y = player_y + ((((44.0f) * scale) * 0.6f) / 2) + (80.0f * scale),
				.width = (128 * scale) * 1.6f,
				.height = (44 * scale) * 1.6f,
			},
			&(rodeo_rectangle_t){
				.width = 128,
				.height = 44,
			},
			&(rodeo_color_RGBAFloat_t){ .array = {0.9f,0.9f,0.9f,1.0} },
			&player.aim_texture
		);
}

