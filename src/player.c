#include "rodeo.h"
#include "input.h"
#include "player.h"
#include "enemies.h"
#include "rodeo/collision.h"
#include "bullet.h"
#include "sprite.h"
#include "wall.h"

struct player_t
{
	sprite_t sprite;
	rodeo_texture_2d_t texture;
	rodeo_texture_2d_t aim_texture;
	int32_t hp;
	float damage_timer; //ms
	world_id collision_id;
	move_state_t move_state;
}
player;

typedef struct player_t player_t;

// 0-19 jumping
// 61 standing
// 20-60 mid iar

static rodeo_collision_2d_world_t player_collision_world;
static aim_position_t aim_position;

static float orc_size[] = {13.0f * 2.0f, 19.0f * 2.0f};

void
init_player(void)
{
	player.texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/mainblob-128.png"));
	player.aim_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/aim.png"));
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
	aim_position.x = 101;
	aim_position.y = 100;
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
	draw_aim(player_position->x, player_position->y, 0.25f);
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

	// if you arent mid jump, then record attempted movement
	if(player.move_state != mv_state_jumping)
	{
		player_position->dx = *(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL);
		player_position->dy = *(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL);

		if(((player_position->dy != 0) || (player_position->dx != 0)) && player.move_state == mv_state_standing)
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
		player.sprite.iter += 1;
		player.sprite.iter %= player.sprite.config.count;
		if(player.sprite.iter > 19)
		{
			player.move_state = mv_state_mid_air;
		}
	}
	if(player.sprite.iter == 60)
	{
		player.move_state = mv_state_standing;
	}
	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	player_position->x += player_position->dx * ((60.0f - (float)player.sprite.iter) / 60.0f);
	player_position->dx = 0;
	player_position->y += player_position->dy * ((60.0f - (float)player.sprite.iter) / 60.0f);
	player_position->dy = 0;
	update_aim_position();
}

void
player_shoot(rodeo_collision_2d_world_t *bullet_collision_world)
{
	cvec_collision_2d_world_item_value *player_position = rodeo_collision_2d_world_item_get_by_id(player.collision_id);
	const uint32_t bullet_per_frame = 10;
	for(uint32_t i = 0; i < bullet_per_frame; ++i)
	{
		spawn_bullet(
				(float)player_position->x + (orc_size[0] / 2.0f) - 9.0f,
				(float)player_position->y + (orc_size[1] / 2.0f) - 16.0f,
				(float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				(float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				bullet_collision_world,
				(rodeo_color_RGBAFloat_t){ .array = { 1,1,1,1 } }
				);
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

void player_wall_resolver(
	rodeo_collision_2d_world_item_t *player_collision,
	rodeo_collision_2d_world_item_t *wall_collision
)
{
	rodeo_collision_2d_world_item_t *p = player_collision;
	rodeo_collision_2d_world_item_t *w = wall_collision;
	rodeo_rectangle_t step = (rodeo_rectangle_t){
		.x = p->x + p->dx * rodeo_frame_time_get(),
		.y = p->y + p->dy * rodeo_frame_time_get(),
		.width = p->width,
		.height = p->height
	};
	rodeo_rectangle_t intersection = rodeo_collision_2d_get_collision_rect(p, w);
	if (intersection.width < intersection.height) {
		if (intersection.x == step.x) {
			p->x = w->x + w->width;
			if (p->dx < 0) {
				p->dx = 0;
			}
		} else {
			p->x = w->x - p->width;
			if (p->dx > 0) {
				p->dx = 0;
			}
		}
	}
	else if (intersection.height < intersection.width) {
		if (intersection.y == step.y) {
			p->y = w->y + w->height;
			if (p->dy < 0) {
				p->dy = 0;
			}
		} else {
			p->y = w->y - p->height;
			if (p->dy > 0) {
				p->dy = 0;
			}
		}
	}
	else if (p->width == w->width && p->height == w->height) {
		p->dx = 0;
		p->dy = 0;
	}
}

void
detect_player_wall_collisions(void)
{
	rodeo_collision_2d_world_compare_other(&player_collision_world, get_wall_world(), player_wall_resolver);
}

cvec_collision_2d_world_item_value *
get_player_position(void)
{
		return rodeo_collision_2d_world_item_get_by_id(player.collision_id);
}

void
update_aim_position(void)
{

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
