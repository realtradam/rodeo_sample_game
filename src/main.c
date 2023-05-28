
#include "rodeo.h"
#include <inttypes.h>
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "enemies.h"
#include "wall.h"
#include "rodeo/collision.h"

const uint16_t window_width = 1600;
const uint16_t window_height = 900;


cstr renderer;
float time_var;

rodeo_audio_sound_t *scratch = NULL;
rodeo_audio_music_t *music = NULL;

void
main_loop(void)
{
	if(rodeo_frame_count_get() % 10 == 0)
	{
		time_var = rodeo_frame_perSecond_get();
	}

	mrodeo_frame_do()
	{
		parse_player_input();
		move_player();

		if(*(bool*)play_sound_input(NULL, NULL))
		{
			rodeo_audio_sound_play(scratch);
		}
		if(*(bool*)play_sound_input2(NULL, NULL))
		{
			rodeo_audio_music_play_fadeIn(music, 1000);
		}
		if(*(bool*)play_sound_input3(NULL, NULL))
		{
			rodeo_audio_music_stop_fadeOut(1000);
		}

		player_shoot(get_player_bullet_world());
		enemies_attempt_weapon_fire();
		attempt_random_enemy_spawn((rodeo_rectangle_t){ 0, 0, window_width, window_height });

		move_bullets();
		move_enemies();
		group_follow_target(get_player_position());
		draw_bullets();
		draw_player();
		draw_enemies();
		detect_bullet_enemy_collisions();
		detect_bullet_wall_collisions();
		detect_player_enemy_collisions();
		detect_player_wall_collisions();

		rodeo_debug_text_draw(
			1,
			1,
			" using %s renderer ",
			cstr_str(
				&renderer
			)
		);

		rodeo_debug_text_draw(
			2,
			2,
			" frame count: %"PRIu64" ",
			rodeo_frame_count_get()
		);

		rodeo_debug_text_draw(
			2,
			3,
			" fps: %.2f ",
			time_var
		);
	}
}

int
main(void)
{
	register_inputs();
	
	rodeo_log(
		rodeo_logLevel_info,
		"Testing logging... It seems to work!"
	);
	rodeo_log(
		rodeo_logLevel_warning,
		"Testing warning log level... It seems to work!"
	);
	rodeo_log(
		rodeo_logLevel_error,
		"Testing error log level... It seems to work!"
	);
	mrodeo_window_do(window_height, window_width, cstr_lit("Rodeo Window"))
	{
		renderer = rodeo_renderer_name_get();
		rodeo_frame_limit_set(60);

		scratch = rodeo_audio_sound_create_from_path(cstr_lit("assets/sample.wav"));
		music = rodeo_audio_music_create_from_path(cstr_lit("assets/music.ogg"));

		init_bullets();
		init_player();
		init_enemies();
		init_wall();
		new_wall(0, -10, window_width, 10);
		new_wall(0, window_height, window_width, 10);
		new_wall(-10, 0, 10, window_height);
		new_wall(window_width, 0, 10, window_height);

		rodeo_mainLoop_run(
			main_loop
		);

		deinit_player();
		deinit_enemies();
	}

	unregister_inputs();

	return 0;
}
