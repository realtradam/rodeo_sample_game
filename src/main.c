
#include "rodeo.h"
#include <inttypes.h>
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "enemies.h"
#include "wall.h"
#include "debug.h"
#include "init.h"
#include "menu.h"

const uint16_t window_width = 1600;
const uint16_t window_height = 900;

// variables for storing debug info that can be displayed
cstr renderer_name;
float fps_display;

void
main_loop(void)
{
	if(rodeo_gfx_frame_count_get() % 10 == 0)
	{
		fps_display = rodeo_gfx_frame_perSecond_get();
	}

	mrodeo_gfx_frame_do()
	{
		rodeo_input_poll();
		if (get_player_hp() > 0 && get_menu_state() == menu_state_inactive)
		{
			// retrieve and apply player input
			parse_player_input();

			player_shoot(get_player_bullet_world());
			enemies_attempt_weapon_fire();

			// spawn enemies
			attempt_random_enemy_spawn((rodeo_rectangle_t){ 0, 0, window_width, window_height });
		}
		else 
		{
			parse_menu_input();
			if (get_player_hp() <= 0)
			{
				set_menu_state(menu_state_gameover);
			}
		}

		// run enemy movement ai
		group_follow_target(
			(rodeo_collision_2d_item_t){
				.data_handle = get_player_position()->id.self_handle
			}
		);

		detect_bullet_enemy_collisions();
		detect_bullet_wall_collisions();
		detect_player_enemy_collisions();
		detect_player_wall_collisions();
		detect_enemy_wall_collisions();

		// apply movements
		move_bullets();
		move_enemies();
		move_player();

		draw_level();
		if (get_menu_state() == menu_state_inactive || get_menu_state() == menu_state_gameover)
		{
			draw_bullets();
			if (get_menu_state() != menu_state_gameover)
			{
				draw_player();
			}
			draw_enemies();
			draw_hp_bar();
		}
		draw_menu();

	}
	//draw_debug_text(renderer_name, fps_display);
}

	int
main(void)
{
	inputs_register_do()
	{
		mrodeo_window_do(window_height, window_width, cstr_lit("Bubbles, Behind"))
		{
			mrodeo_gfx_do(1600, 900)
			{
				mrodeo_audio_do(8)
				{
					renderer_name = rodeo_gfx_renderer_name_get();
					//rodeo_gfx_frame_limit_set(60);

					game_systems_init_do()
					{
						// use to test manually spawning enemies
						//spawn_enemy(400.0f,700.0f);
						//spawn_enemy(900.0f,700.0f);
						//spawn_enemy(400.0f,100.0f);
						//spawn_enemy(900.0f,100.0f);
						//for(int i = 0; i < 1500; ++i)
						//{
						//	float rng1 = ((float)rodeo_random_double_get() * 100) + 150;
						//	float rng2 = ((float)rodeo_random_double_get() * 100) + 150;
						//	spawn_ghost(rng1,rng2);
						//}
						rodeo_mainLoop_run(
								main_loop
								);
					}
				}
			}
		}
	}

	return 0;
}
