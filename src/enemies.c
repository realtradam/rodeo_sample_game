#include "enemies.h"
#include "bullet.h"
#include "cglm/vec2.h"
#include "player.h"
#include "bullet.h"
#include "wall.h"

static rodeo_collision_2d_collection_t collision_enemies_collection = {0};
static rodeo_collision_2d_collection_t collision_ghosts_collection = {0};
//static rodeo_gfx_texture_2d_t hinotamatchi_texture;
//static rodeo_gfx_texture_2d_t amonghost_texture;
//static rodeo_gfx_texture_2d_t squid_texture;
static rodeo_gfx_texture_2d_t enemy_texture = {0};
static cvec_enemy_t enemies = {0};
static cvec_enemy_t ghosts = {0};
static float spawn_cooldown = 0;
static uint32_t enemy_count = 0;
static uint32_t ghost_count = 0;

void
init_enemies(void)
{
	collision_enemies_collection = rodeo_collision_2d_collection_create(50);
	collision_ghosts_collection = rodeo_collision_2d_collection_create(50);
	//enemies = cvec_enemy_t_init();
	//squid_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/squid.png"));
	//hinotamatchi_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/hinotamatchi.png"));
	//amonghost_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/amonghost.png"));
	enemy_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/enemy_sheet.png"));
}

void
deinit_enemies(void)
{
	rodeo_collision_2d_collection_destroy(collision_enemies_collection);
	rodeo_collision_2d_collection_destroy(collision_ghosts_collection);
	cvec_enemy_t_drop(&enemies);
	cvec_enemy_t_drop(&ghosts);
	//rodeo_gfx_texture_2d_destroy(&hinotamatchi_texture);
	//rodeo_gfx_texture_2d_destroy(&squid_texture);
	//rodeo_gfx_texture_2d_destroy(&amonghost_texture);
	rodeo_gfx_texture_2d_destroy(enemy_texture);
}

void
reset_enemies(void)
{
	c_foreach_rv(i, cvec_enemy_t, enemies)
	{
		enemy_destroy(i.ref);
	}
	c_foreach_rv(i, cvec_enemy_t, ghosts)
	{
		enemy_destroy(i.ref);
	}
}

uint32_t
get_enemy_count(void)
{
	return enemy_count;
}

uint32_t
get_ghost_count(void)
{
	return ghost_count;
}

enemy_t*
spawn_enemy(float x, float y)
{
	uint64_t rng = rodeo_random_uint64_get();
	cvec_enemy_t *obj_world = &enemies;
	rodeo_collision_2d_collection_t collision_world = collision_enemies_collection;

	// debug to force specific enemies to spawn
	//rng = (rng % 2) ? enemy_weapon_fourcross : enemy_weapon_none;
	//rng = enemy_weapon_fourcross;
	//rng = enemy_weapon_none;
	if(rng % 3 == enemy_weapon_basic)
	{
		obj_world = &ghosts;
		collision_world = collision_ghosts_collection;
		ghost_count++;
	}
	else
	{
		enemy_count++;
	}

	rodeo_collision_2d_item_data_t collision = (rodeo_collision_2d_item_data_t)
		{
			.rect = {
				.x = x,
				.y = y,
				.width = 40,
				.height = 40
			}
		};
	rodeo_collision_2d_item_t item = rodeo_collision_2d_item_create(collision_world, collision);

	return cvec_enemy_t_push(
		obj_world,
		(enemy_t){
			.hp = 20.0,
			.move_speed = ((float)(rng % 3) + 1.0f) * 0.3f,
			.behavior = enemy_ai_follow,
			.weapon = {
			.firerate = -((rng % 3) - 2.0f) * 1.6f,
			.type = rng % 3,
			.cooldown = 0,
			},
			.item = item
		});
}

enemy_t*
spawn_ghost(float x, float y)
{
	uint64_t rng = rodeo_random_uint64_get();
	cvec_enemy_t *obj_world = &enemies;
	rodeo_collision_2d_collection_t collision_world = collision_enemies_collection;
	

	rng = enemy_weapon_basic;
	if(rng % 3 == enemy_weapon_basic)
	{
		obj_world = &ghosts;
		collision_world = collision_ghosts_collection;
		ghost_count++;
	}
	else
	{
		enemy_count++;
	}

	rodeo_collision_2d_item_data_t collision = (rodeo_collision_2d_item_data_t)
	{
		.rect = { .x = x, .y = y, .width = 40, .height = 40 }
	};
	rodeo_collision_2d_item_t item = rodeo_collision_2d_item_create(collision_world, collision);

	return cvec_enemy_t_push(
		obj_world,
		(enemy_t){
			.hp = 20.0,
			.move_speed = ((float)(rng % 3) + 1.0f) * 0.3f,
			.behavior = enemy_ai_follow,
			.weapon = {
			.firerate = -((rng % 3) - 2.0f) * 1.6f,
			.type = rng % 3,
			.cooldown = 0,
			},
			.item = item
		});
}

void
draw_enemy(rodeo_collision_2d_item_t enemy)
{
		rodeo_color_RGBAFloat_t color;

		enemy_t *enemy_obj = get_enemy_by_id((*enemy.data_handle)->id);
		if(enemy_obj == NULL) { return; }

		switch(enemy_obj->weapon.type)
		{
			case enemy_weapon_none:
				{
					color = (rodeo_color_RGBAFloat_t){
						.array = { 0.8f, 0.5f, 0.1f, 1.0f  }
					};
				}
				break;
			case enemy_weapon_basic:
				{
					color = (rodeo_color_RGBAFloat_t){
						.array = { 0.25f, 0.95f, 0.25f, 0.5f  }
					};
				}
				break;
			case enemy_weapon_fourcross:
				{
					color = (rodeo_color_RGBAFloat_t){
						.array = { 0.1f, 0.2f, 0.75f, 1.0f  }
					};
				}
				break;
		}

		   rodeo_gfx_texture_2d_draw(
				(*enemy.data_handle)->rect,
				(rodeo_rectangle_t){
					.x = 0 + (40 * (float)enemy_obj->weapon.type),
					.y = 0,
					.width = 40,
					.height = 40
				},
				color,
				enemy_texture
				);

}

void
draw_enemies(void)
{
	c_foreach(i, cvec_enemy_t, enemies) {
		draw_enemy(i.ref->item);
	}
	c_foreach(i, cvec_enemy_t, ghosts) {
		draw_enemy(i.ref->item);
	}
}

void
enemy_overlap_resolver(
	rodeo_collision_2d_item_data_t *a,
	rodeo_collision_2d_item_data_t *b
)
{
	enemy_t *enemy_a = get_enemy_by_id(a->id);
	enemy_t *enemy_b = get_enemy_by_id(b->id);
	vec2 direction;
	glm_vec2_sub(
		(vec2){b->rect.x, b->rect.y},
		(vec2){a->rect.x, a->rect.y},
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
  
   rodeo_collision_2d_collection_compare_self(get_enemies_world(), enemy_overlap_resolver);
   rodeo_collision_2d_collection_compare_self(get_ghosts_world(), enemy_overlap_resolver);
	c_foreach(i, cvec_enemy_t, enemies) {
		rodeo_collision_2d_item_data_t *enemy = (*i.ref->item.data_handle);
		if(enemy == NULL)
		{
			continue;
		}
		enemy->rect.x += enemy->dx * rodeo_gfx_frame_time_get() / (1000.0f/60.0f);
		enemy->dx = 0;
		enemy->rect.y += enemy->dy * rodeo_gfx_frame_time_get() / (1000.0f/60.0f);
		enemy->dy = 0;
	}
	c_foreach(i, cvec_enemy_t, ghosts) {
		rodeo_collision_2d_item_data_t *enemy = (*i.ref->item.data_handle);
		if(enemy == NULL)
		{
			continue;
		}
		enemy->rect.x += enemy->dx * rodeo_gfx_frame_time_get() / (1000.0f/60.0f);
		enemy->dx = 0;
		enemy->rect.y += enemy->dy * rodeo_gfx_frame_time_get() / (1000.0f/60.0f);
		enemy->dy = 0;
	}
}

void
enemies_attempt_weapon_fire(void)
{
	cvec_enemy_t enemy_arry[2] = { enemies, ghosts };
	for(int32_t j = 0; j < 2; ++j)
	{
	c_foreach(i, cvec_enemy_t, enemy_arry[j]) {
		switch(i.ref->weapon.type)
		{
			case enemy_weapon_none:
				{
					// do nothing
				}
				break;
			case enemy_weapon_basic:
				{
					if(i.ref->weapon.cooldown < 0)
					{
						i.ref->weapon.cooldown += i.ref->weapon.firerate;
						//weapon spawn logic
						rodeo_collision_2d_item_data_t *player = get_player_position();
						rodeo_collision_2d_item_data_t *enemy = (*i.ref->item.data_handle);
						vec2 dest;
						glm_vec2_sub(
							(vec2){ player->rect.x, player->rect.y },
							(vec2){ enemy->rect.x, enemy->rect.y },
							dest
						);
						glm_vec2_normalize(dest);

						vec2 rand1 = {
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.250f, 
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.250f, 
						};

						glm_vec2_add(dest, rand1, dest);

						glm_vec2_normalize(dest);

						glm_vec2_scale(dest, 1.0, dest);

						spawn_bullet(
							enemy->rect.x,
							enemy->rect.y,
							dest[0],
							dest[1],
							get_enemy_bullet_world(),
							(rodeo_color_RGBAFloat_t){
								.colors.alpha = 1,
								.colors.red = 0.9f,
								.colors.green = 0.1f,
								.colors.blue = 0.1f
							}
						);
					}
					else
					{
						i.ref->weapon.cooldown -= rodeo_gfx_frame_time_get()/1000.0f;
					}
				}
				break;
			case enemy_weapon_fourcross:
				{
					if(i.ref->weapon.cooldown < 0)
					{
						i.ref->weapon.cooldown += i.ref->weapon.firerate;
						//weapon spawn logic
						rodeo_collision_2d_item_data_t *player = get_player_position();
						rodeo_collision_2d_item_data_t *enemy = (*i.ref->item.data_handle);
						vec2 dest;
						glm_vec2_sub(
							(vec2){ player->rect.x, player->rect.y },
							(vec2){ enemy->rect.x, enemy->rect.y },
							dest
						);
						glm_vec2_normalize(dest);

						vec2 bullet1;
						vec2 bullet2;
						vec2 bullet3;
						vec2 bullet4;

						glm_vec2_rotate(dest, 3.14159f/4.0f, bullet1);
						glm_vec2_rotate(bullet1, 3.14159f/2.0f, bullet2);
						glm_vec2_rotate(bullet2, 3.14159f/2.0f, bullet3);
						glm_vec2_rotate(bullet3, 3.14159f/2.0f, bullet4);

						vec2 rand1 = {
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
						};
						vec2 rand2 = {
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
						};
						vec2 rand3 = {
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
						};
						vec2 rand4 = {
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
							((((float)rodeo_random_double_get()) - 0.5f) * 2.0f) * 0.0f, 
						};

						glm_vec2_add(bullet1, rand1, bullet1);
						glm_vec2_add(bullet2, rand2, bullet2);
						glm_vec2_add(bullet3, rand3, bullet3);
						glm_vec2_add(bullet4, rand4, bullet4);

						glm_vec2_normalize(bullet1);
						glm_vec2_normalize(bullet2);
						glm_vec2_normalize(bullet3);
						glm_vec2_normalize(bullet4);

						//glm_vec2_scale(dest, 1.0, dest);
						
						spawn_bullet(
							enemy->rect.x,
							enemy->rect.y,
							bullet1[0],
							bullet1[1],
							get_enemy_bullet_world(),
							(rodeo_color_RGBAFloat_t){
								.colors.alpha = 1,
								.colors.red = 0.9f,
								.colors.green = 0.1f,
								.colors.blue = 0.1f
							}
						);
						spawn_bullet(
							enemy->rect.x,
							enemy->rect.y,
							bullet2[0],
							bullet2[1],
							get_enemy_bullet_world(),
							(rodeo_color_RGBAFloat_t){
								.colors.alpha = 1,
								.colors.red = 0.9f,
								.colors.green = 0.1f,
								.colors.blue = 0.1f
							}
						);
						spawn_bullet(
							enemy->rect.x,
							enemy->rect.y,
							bullet3[0],
							bullet3[1],
							get_enemy_bullet_world(),
							(rodeo_color_RGBAFloat_t){
								.colors.alpha = 1,
								.colors.red = 0.9f,
								.colors.green = 0.1f,
								.colors.blue = 0.1f
							}
						);
						spawn_bullet(
							enemy->rect.x,
							enemy->rect.y,
							bullet4[0],
							bullet4[1],
							get_enemy_bullet_world(),
							(rodeo_color_RGBAFloat_t){
								.colors.alpha = 1,
								.colors.red = 0.9f,
								.colors.green = 0.1f,
								.colors.blue = 0.1f
							}
						);
					}
					else
					{
						i.ref->weapon.cooldown -= rodeo_gfx_frame_time_get()/1000.0f;
					}
				}
				break;
		}
	}
	}
}

enemy_t*
get_enemy_by_id(
    rodeo_collision_2d_item_id_t id
)
{
	cvec_enemy_t *enemy_vec = &enemies;
	if (id.collection.data == collision_ghosts_collection.data)
	{
		enemy_vec = &ghosts;
	}
	c_foreach(i, cvec_enemy_t, *enemy_vec) {
		if (i.ref->item.data_handle == id.self_handle) {
			return i.ref;
		}
	}
	return NULL;
}

rodeo_collision_2d_collection_t
get_enemies_world(void)
{
    return collision_enemies_collection;
}

rodeo_collision_2d_collection_t
get_ghosts_world(void)
{
    return collision_ghosts_collection;
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
	if (enemy == NULL) { return; }
	cvec_enemy_t *enemy_vec = &enemies;
	if(enemy->weapon.type == enemy_weapon_basic)
	{
		enemy_vec = &ghosts;
		ghost_count--;
	}
	else
	{
		enemy_count--;
	}
	rodeo_collision_2d_item_destroy(enemy->item);
	*enemy = *cvec_enemy_t_back(enemy_vec);
	if(cvec_enemy_t_size(enemy_vec) > 0)
	{
		cvec_enemy_t_pop(enemy_vec);
	}

}

void
damage_enemy_resolver(
	rodeo_collision_2d_item_data_t *enemy_collision,
	rodeo_collision_2d_item_data_t *bullet_collision
)
{
	bullet_t *bullet = get_bullet_by_id(bullet_collision->id);
	if(bullet == NULL) { return; }
	bullet_destroy(bullet);
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
	rodeo_collision_2d_collection_compare_other(collision_ghosts_collection, get_player_bullet_world(), damage_enemy_resolver);
	rodeo_collision_2d_collection_compare_other(collision_enemies_collection, get_player_bullet_world(), damage_enemy_resolver);
}

void
group_follow_target(rodeo_collision_2d_item_t target)
{
	cvec_enemy_t enemy_arry[2] = { enemies, ghosts };
	for(int32_t j = 0; j < 2; ++j)
	{
	c_foreach(i, cvec_enemy_t, enemy_arry[j]) {
		rodeo_collision_2d_item_data_t *enemy = (*i.ref->item.data_handle);
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
			enemy->rect.x,
			enemy->rect.y
		};
		vec2 dest = {
			(*target.data_handle)->rect.x,
			(*target.data_handle)->rect.y
		};
		vec2 direction;
		glm_vec2_sub(dest, source, direction);
		glm_vec2_normalize(direction);
		vec2 result;
		enemy_t *enemy_obj = get_enemy_by_id(enemy->id);
		if(enemy_obj == NULL) { continue; }
		glm_vec2_scale(direction, enemy_obj->move_speed, result);

		enemy->dx = result[0];
		enemy->dy = result[1];
	}
	}
}

enemy_t*
random_enemy_create(
	rodeo_rectangle_t bounds
)
{
	float spawn_coords[2];
	rodeo_collision_2d_item_data_t *p = get_player_position();
	float player_coords[2] = {p->rect.x, p->rect.y};
	float player_radius = p->rect.height * 2 + 100;
	for (int i = 0; i < 10; ++i) {
		spawn_coords[0] = (float)rodeo_random_double_get() * bounds.width + bounds.x;
		spawn_coords[1] = (float)rodeo_random_double_get() * bounds.height + bounds.y;
		float dist = glm_vec2_distance(spawn_coords, player_coords);
		if (!coords_inside_wall(spawn_coords[0], spawn_coords[1]) &&
				dist > player_radius) {
			return spawn_enemy(spawn_coords[0], spawn_coords[1]);
		}
	}
	rodeo_log(rodeo_logLevel_info, "failed to spawn enemy");
	return NULL;
}

enemy_t*
attempt_random_enemy_spawn(
	rodeo_rectangle_t bounds
)
{
	spawn_cooldown -= rodeo_gfx_frame_time_get();
	if (spawn_cooldown <= 0 && get_enemy_count() + get_ghost_count() <= 100) {
		// faster spawning for testing
		//spawn_cooldown += ((float)rodeo_random_double_get() * 15.0f) + 4.50f;
		spawn_cooldown += ((float)rodeo_random_double_get() * 1500.0f) + 450.0f;
		return random_enemy_create(bounds);
	}
	return NULL;
}

void
detect_enemy_wall_collisions(void)
{
	rodeo_collision_2d_collection_compare_other(collision_enemies_collection, get_wall_world(), moving_wall_resolver);
}
