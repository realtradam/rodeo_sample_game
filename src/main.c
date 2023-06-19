
#include "rodeo.h"
#include <inttypes.h>
#include "input.h"

cstr renderer;
float time_var;

rodeo_audio_sound_t scratch = {0};
rodeo_audio_music_t music = {0};

typedef
struct
{
	float x;
	float y;
}
summon_t;

summon_t summon_position = { .x = 100, .y = 100 };
summon_t box1_position = { .x = 100, .y = 100 };
summon_t box2_position = { .x = 100, .y = 100 };

float orc_size[] = {13.0f * 2.0f, 19.0f * 2.0f};

rodeo_gfx_texture_2d_t texture;

rodeo_vector2_t units[UINT16_MAX][2] = {0};
uint16_t num_of_units = 0;

const rodeo_color_RGBAFloat_t black_clear =
{ 
	.colors.red = 0.1f, .colors.green = 0.1f, .colors.blue = 0.1f,
	.colors.alpha = 0.75f
};
const rodeo_color_RGBAFloat_t red =
{ 
	.colors.red = 1.0f, .colors.green = 0.0f, .colors.blue = 0.0f,
	.colors.alpha = 1.0f
};
const rodeo_color_RGBAFloat_t green =
{ 
	.colors.red = 0.0f, .colors.green = 1.0f, .colors.blue = 0.0f,
	.colors.alpha = 1.0f
};
const rodeo_color_RGBAFloat_t blue =
{ 
	.colors.red = 0.0f, .colors.green = 0.0f, .colors.blue = 1.0f,
	.colors.alpha = 1.0f
};
const rodeo_color_RGBAFloat_t pink =
{ 
	.colors.red = 1.0f, .colors.green = 0.0f, .colors.blue = 1.0f,
	.colors.alpha = 1.0f
};

const rodeo_color_RGBAFloat_t red_clear =
{ 
	.colors.red = 1.0f, .colors.green = 0.0f, .colors.blue = 0.0f,
	.colors.alpha = 0.5f
};
const rodeo_color_RGBAFloat_t green_clear =
{ 
	.colors.red = 0.0f, .colors.green = 1.0f, .colors.blue = 0.0f,
	.colors.alpha = 0.5f
};
const rodeo_color_RGBAFloat_t blue_clear =
{ 
	.colors.red = 0.0f, .colors.green = 0.0f, .colors.blue = 1.0f,
	.colors.alpha = 0.5f
};
const rodeo_color_RGBAFloat_t pink_clear =
{ 
	.colors.red = 1.0f, .colors.green = 0.0f, .colors.blue = 1.0f,
	.colors.alpha = 0.5f
};

rodeo_collision_2d_collection_t world_orc;
rodeo_collision_2d_collection_t world_other;
rodeo_collision_2d_item_id_t orc_collision_id;
rodeo_collision_2d_item_id_t box_collision_ids[2] = { 0 };

void
summon_units(void)
{
	for(uint8_t i = 0; i < 10; ++i)
	{
		if((num_of_units < UINT16_MAX))//(rodeo_frame_perSecond_get() > 40))
		{
			num_of_units += 1;
			units[num_of_units - 1][0] = (rodeo_vector2_t){ {
				(float)summon_position.x - (orc_size[0] / 2.0f),
				(float)summon_position.y - (orc_size[1] / 2.0f)
			} };
			units[num_of_units - 1][1] = (rodeo_vector2_t){ {
				(float)((int8_t)(irodeo_math_rng_uint64_get_default() % 10) - 5),
					(float)((int8_t)(irodeo_math_rng_uint64_get_default() % 10) - 5)
			} };
		}
	}
}

void collision_resolve(
		rodeo_collision_2d_item_data_t *a,
		rodeo_collision_2d_item_data_t *b
) {
	rodeo_log(
		rodeo_logLevel_info,
		"%d collided with %d",
		*a->id.self_handle, *b->id.self_handle
	);
	for(int32_t i = 0; i < (int32_t)(sizeof box_collision_ids / sizeof *box_collision_ids); ++i)
	{
		if(box_collision_ids[i].self_handle == b->id.self_handle)
		{
			rodeo_collision_2d_item_destroy_by_id(b->id);
			box_collision_ids[i] = (rodeo_collision_2d_item_id_t){0};
			break;
		}
	}
}

void
main_loop(void)
{
	if((uint32_t)rodeo_gfx_frame_time_get() % 10 == 0)
	{
		/* for testing
		rodeo_input_scene_unregister_callback(
			*summon_units_input,
			&scene,
			register_type_e
		);

		rodeo_input_scene_register_callback(
			*summon_units_input,
			&scene,
			register_type_q
		);
		*/

		time_var = rodeo_gfx_frame_perSecond_get();
	}

	rodeo_input_poll();
	mrodeo_gfx_frame_do()
	{
	   rodeo_gfx_rectangle_draw(
			(rodeo_rectangle_t){
				.width = rodeo_gfx_width_get(),
				.height = rodeo_gfx_height_get()
			},
			0,
			blue_clear
		);
		bool reset_movement = true;
		units_move_up_input(NULL, &reset_movement);
		units_move_down_input(NULL, &reset_movement);
		units_move_left_input(NULL, &reset_movement);
		units_move_right_input(NULL, &reset_movement);
		summon_position.x += *(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL);
		summon_position.y += *(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL);

		rodeo_collision_2d_item_t orc_collision_item = rodeo_collision_2d_item_get_by_id(orc_collision_id);

		(*orc_collision_item.data_handle)->rect.x = summon_position.x;
		(*orc_collision_item.data_handle)->rect.y = summon_position.y;
		(*orc_collision_item.data_handle)->dx = *(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL);
		(*orc_collision_item.data_handle)->dy = *(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL);

		rodeo_collision_2d_collection_compare_self(world_orc, collision_resolve);

		if(*(bool*)play_sound_input(NULL, NULL))
		{
			//rodeo_audio_sound_play(scratch);
		}
		if(*(bool*)play_sound_input2(NULL, NULL))
		{
			//rodeo_audio_music_play_fadeIn(music, 1000);
		}
		if(*(bool*)play_sound_input3(NULL, NULL))
		{
			//rodeo_audio_music_stop_fadeOut(1000);
		}

		if(*(bool*)summon_units_input(NULL, NULL)) 
		{
			summon_units();
		}

		rodeo_log(
			rodeo_logLevel_warning,
			"%f, %f",
			(float)*(int64_t*)mouse_x_input(NULL,NULL) - (150.0f / 2.0f),
			(float)*(int64_t*)mouse_y_input(NULL,NULL) - (150.0f / 2.0f)
		);


		rodeo_rectangle_t scissor = {
				.x = (float)*(int64_t*)mouse_x_input(NULL,NULL) - (150.0f / 2.0f),
				.y = (float)*(int64_t*)mouse_y_input(NULL,NULL) - (150.0f / 2.0f),
			.width = 150,
			.height = 150
		};

		mrodeo_gfx_scissor_do(scissor)
		{
			rodeo_gfx_rectangle_draw(
				(rodeo_rectangle_t){ 100, 100, 50, 50 },
				0,
				red
			);
	 	  rodeo_gfx_rectangle_draw(  
	 	  		(rodeo_rectangle_t){ 100, 160, 50, 50 },
				0,
	 	  		green
	 	  );
	 	  rodeo_gfx_rectangle_draw(  
	 	  		(rodeo_rectangle_t){ 160, 100, 50, 50 },
				0,
	 	  		blue
	 	  );
	 	  rodeo_gfx_rectangle_draw(  
	 	  		(rodeo_rectangle_t){ 160, 160, 50, 50 },
				0,
	 	  		pink
	 	  );

	 	  rodeo_gfx_rectangle_draw(  
	 	  		(rodeo_rectangle_t){ 300, 300, 50, 50 },
				0,
	 	  		red_clear
	 	  );
	 	  rodeo_gfx_rectangle_draw(  
	 	  		(rodeo_rectangle_t){ 310, 310, 50, 50 },
				0,
	 	  		green_clear
	 	  );
	 	  rodeo_gfx_rectangle_draw(  
	 	  		(rodeo_rectangle_t){ 320, 320, 50, 50 },
				0,
	 	  		blue_clear
	 	  );
	 	  rodeo_gfx_rectangle_draw(  
	 	  		(rodeo_rectangle_t){ 330, 330, 50, 50 },
				0,
	 	  		pink_clear
	 	  );
		 }
		if(scissor.x < 0)
		{
			scissor.width += scissor.x;
			scissor.x = 0;
		}
		if(scissor.y < 0)
		{
			scissor.height += scissor.y;
			scissor.y = 0;
		}
		rodeo_gfx_rectangle_draw(scissor, 0, pink_clear);


	   for(uint64_t i = 0; i < (sizeof(box_collision_ids) / sizeof(box_collision_ids[0])); ++i)
	   {
		   rodeo_collision_2d_item_t box = rodeo_collision_2d_item_get_by_id(box_collision_ids[i]);
		   if(box.data_handle != NULL)
		   {
		   rodeo_gfx_rectangle_draw(
				(*box.data_handle)->rect,
				0,
				pink
			);
		   }
	   }

	   for(uint64_t i = 0; i < num_of_units; ++i)
	   {
		   units[i][0].position.x += units[i][1].position.x;
		   units[i][0].position.y += units[i][1].position.y;
		   if(units[i][0].position.x > rodeo_gfx_width_get() - orc_size[0] || units[i][0].position.x < 0)
		   {
			   units[i][1].position.x = -units[i][1].position.x;
		   }
		   if(units[i][0].position.y > rodeo_gfx_height_get() - orc_size[1] || units[i][0].position.y < 0)
		   {
			   units[i][1].position.y = -units[i][1].position.y;
		   }

		   rodeo_gfx_texture_2d_draw(
				(rodeo_rectangle_t){
					.x = units[i][0].position.x,
					.y = units[i][0].position.y,
					.width = orc_size[0],
					.height = orc_size[1],
				},
				(rodeo_rectangle_t){
					.x = 5,
					.y = 5,
					.width = 13,
					.height = 19
				},
				0,
				(rodeo_math_vec2_t){0},
				(rodeo_color_RGBAFloat_t){ .array = {1,1,1,1} },
				texture
				);
	   }

	   rodeo_gfx_texture_2d_draw(
			(rodeo_rectangle_t){
				.x = (float)(int32_t)summon_position.x - (orc_size[0] / 2.0f),
				.y = (float)(int32_t)summon_position.y - (orc_size[1] / 2.0f) ,
				.width = orc_size[0],
				.height = orc_size[1],
			},
			(rodeo_rectangle_t){
				.x = 5,
				.y = 5,
				.width = 13,
				.height = 19
			},
			0,
			(rodeo_math_vec2_t){0},
			(rodeo_color_RGBAFloat_t){ .array = {1.0,1.0,1.0,1.0} },
			texture
		);

	   rodeo_gfx_texture_2d_draw(
			(rodeo_rectangle_t){
				.x = (float)*(int64_t*)mouse_x_input(NULL,NULL),
				.y = (float)*(int64_t*)mouse_y_input(NULL,NULL),
				.width = orc_size[0],
				.height = orc_size[1],
			},
			(rodeo_rectangle_t){
				.x = 5,
				.y = 5,
				.width = 13,
				.height = 19
			},
			(float)rodeo_gfx_frame_count_get() / 60.0f,
			(rodeo_math_vec2_t){ .val = { .x = orc_size[0]/2.0f, .y = orc_size[1]/2.0f } },
			(rodeo_color_RGBAFloat_t){ .array = {1.0,1.0,1.0,1.0} },
			texture
		);

	   /*
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
			rodeo_gfx_frame_count_get()
		);

		rodeo_debug_text_draw(
			2,
			3,
			" fps: %.2f ",
			time_var
		);

		rodeo_debug_text_draw(
			2,
			4,
			" orc count: %"PRIu32" ",
			num_of_units
		);
		*/
	   rodeo_rectangle_t letter_first = rodeo_gfx_letterbox_first_get();
	   rodeo_rectangle_t letter_second = rodeo_gfx_letterbox_second_get();

	   rodeo_gfx_rectangle_draw(letter_first, 0, black_clear);
	   rodeo_gfx_rectangle_draw(letter_second, 0, black_clear);
	}
}

int
main(void)
{
	register_inputs_do()
	{
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
		mrodeo_do(640, 480, cstr_lit("Rodeo Window"), 4)
		{
			renderer = rodeo_gfx_renderer_name_get();
			rodeo_gfx_frame_limit_set(60);

			//texture = rodeo_texture_2d_create_from_RGBA8(
			//	2,
			//	2,
			//	texture_memory
			//);
			
			world_orc = rodeo_collision_2d_collection_create(1);
			world_other = rodeo_collision_2d_collection_create(2);

			rodeo_collision_2d_item_data_t orc_collision_params = {
				.rect = {
					.x = summon_position.x,
					.y = summon_position.y,
					.width = orc_size[0],
					.height = orc_size[1]
				}
			};
			orc_collision_id = (*rodeo_collision_2d_item_create(world_orc, orc_collision_params).data_handle)->id;
			
			rodeo_collision_2d_item_data_t test_collision_params = {
				.rect = {
					.x = 320,
					.y = 240,
					.width = orc_size[0],
					.height = orc_size[1]
				}
			};
			box_collision_ids[0] = (*rodeo_collision_2d_item_create(world_orc, test_collision_params).data_handle)->id;
			rodeo_collision_2d_item_data_t test2_collision_params = {
				.rect = {
					.x = 0,
					.y = 240,
					.width = orc_size[0],
					.height = orc_size[1]
				}
			};
			box_collision_ids[1] = (*rodeo_collision_2d_item_create(world_orc, test2_collision_params).data_handle)->id;

			texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/orc.png"));
			//scratch = rodeo_audio_sound_create_from_path(cstr_lit("assets/sample.wav"));
			//music = rodeo_audio_music_create_from_path(cstr_lit("assets/music.ogg"));

			rodeo_mainLoop_run(
				main_loop
			);

			rodeo_gfx_texture_2d_destroy(texture);
			rodeo_collision_2d_collection_destroy(world_orc);
			rodeo_collision_2d_collection_destroy(world_other);
		}
	}
	return 0;
}
