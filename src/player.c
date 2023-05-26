#include "rodeo.h"
#include "input.h"
#include "player.h"
#include "rodeo/collision.h"
static rodeo_texture_2d_t player_texture;
static world_id player_collision_id;
static rodeo_collision_2d_world_t player_collision_world;

static float orc_size[] = {13.0f * 2.0f, 19.0f * 2.0f};

void
init_player(void)
{
	player_texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/orc.png"));
	player_collision_world = rodeo_collision_2d_world_create();
	player_collision_id = rodeo_collision_2d_world_item_create(
		&player_collision_world,
		(rodeo_collision_2d_world_item_t){
			.x = 100,
			.y = 100,
			.width = orc_size[0],
			.height = orc_size[1]
		}
	)->id;
}

void
deinit_player(void)
{
	rodeo_texture_2d_destroy(&player_texture);
	rodeo_collision_2d_world_destroy(&player_collision_world);
}

void
draw_player(void)
{
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

}

void
parse_player_input(void)
{
	cvec_collision_2d_world_item_value *player = rodeo_collision_2d_world_item_get_by_id(player_collision_id);
	bool reset_movement = true;
	units_move_up_input(NULL, &reset_movement);
	units_move_down_input(NULL, &reset_movement);
	units_move_left_input(NULL, &reset_movement);
	units_move_right_input(NULL, &reset_movement);
	player->dx = *(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL);
	player->dy = *(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL);
}

void
move_player(void)
{
	cvec_collision_2d_world_item_value *player = rodeo_collision_2d_world_item_get_by_id(player_collision_id);
	player->x += player->dx;
	player->dx = 0;
	player->y += player->dy;
	player->dy = 0;
}

void
player_shoot(rodeo_collision_2d_world_t *bullet_collision_world)
{
	cvec_collision_2d_world_item_value *player = rodeo_collision_2d_world_item_get_by_id(player_collision_id);
	const uint32_t bullet_per_frame = 10;
	for(uint32_t i = 0; i < bullet_per_frame; ++i)
	{
		rodeo_collision_2d_world_item_create(
				bullet_collision_world,
				(rodeo_collision_2d_world_item_t){
				.x = (float)player->x - (orc_size[0] / 2.0f),
				.y = (float)player->y - (orc_size[1] / 2.0f),
				.dx = (float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				.dy = (float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
				.width = 25.0f,
				.height = 25.0f
				}
				);
	}
}


