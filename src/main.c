
#include "rodeo.h"
#include <inttypes.h>
#include "input.h"

cstr renderer;
float time_var;

typedef
struct
{
	float x;
	float y;
}
summon_t;

summon_t summon_position = { .x = 100, .y = 100 };

float orc_size[] = {13.0f * 2.0f, 19.0f * 2.0f};

rodeo_texture_2d_t texture;

rodeo_vector2_t units[UINT16_MAX][2] = {0};
uint16_t num_of_units = 0;

const rodeo_color_RGBAFloat_t red =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 0.0f,
	.alpha = 1.0f
};
const rodeo_color_RGBAFloat_t green =
{ 
	.red = 0.0f, .green = 1.0f, .blue = 0.0f,
	.alpha = 1.0f
};
const rodeo_color_RGBAFloat_t blue =
{ 
	.red = 0.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 1.0f
};
const rodeo_color_RGBAFloat_t pink =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 1.0f
};

const rodeo_color_RGBAFloat_t red_clear =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 0.0f,
	.alpha = 0.5f
};
const rodeo_color_RGBAFloat_t green_clear =
{ 
	.red = 0.0f, .green = 1.0f, .blue = 0.0f,
	.alpha = 0.5f
};
const rodeo_color_RGBAFloat_t blue_clear =
{ 
	.red = 0.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 0.5f
};
const rodeo_color_RGBAFloat_t pink_clear =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 0.5f
};

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
				(float)((int8_t)(rodeo_random_uint64_get() % 10) - 5),
					(float)((int8_t)(rodeo_random_uint64_get() % 10) - 5)
			} };
		}
	}
}

void
main_loop(void)
{
	if(rodeo_frame_count_get() % 10 == 0)
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

		time_var = rodeo_frame_perSecond_get();
	}

	mrodeo_frame_do()
	{
		bool reset_movement = true;
		units_move_up_input(NULL, &reset_movement);
		units_move_down_input(NULL, &reset_movement);
		units_move_left_input(NULL, &reset_movement);
		units_move_right_input(NULL, &reset_movement);
		summon_position.x += *(float*)units_move_right_input(NULL, NULL) + *(float*)units_move_left_input(NULL, NULL);
		summon_position.y += *(float*)units_move_down_input(NULL, NULL) + *(float*)units_move_up_input(NULL, NULL);

		if(*(bool*)summon_units_input(NULL, NULL)) 
		{
			summon_units();
		}

		rodeo_rectangle_draw(
			&(rodeo_rectangle_t){ 100, 100, 50, 50 },
			&red
		);
	   rodeo_rectangle_draw(  
	   		&(rodeo_rectangle_t){ 100, 160, 50, 50 },
	   		&green
	   );
	   rodeo_rectangle_draw(  
	   		&(rodeo_rectangle_t){ 160, 100, 50, 50 },
	   		&blue
	   );
	   rodeo_rectangle_draw(  
	   		&(rodeo_rectangle_t){ 160, 160, 50, 50 },
	   		&pink
	   );

	   rodeo_rectangle_draw(  
	   		&(rodeo_rectangle_t){ 300, 300, 50, 50 },
	   		&red_clear
	   );
	   rodeo_rectangle_draw(  
	   		&(rodeo_rectangle_t){ 310, 310, 50, 50 },
	   		&green_clear
	   );
	   rodeo_rectangle_draw(  
	   		&(rodeo_rectangle_t){ 320, 320, 50, 50 },
	   		&blue_clear
	   );
	   rodeo_rectangle_draw(  
	   		&(rodeo_rectangle_t){ 330, 330, 50, 50 },
	   		&pink_clear
	   );



	   for(uint64_t i = 0; i < num_of_units; ++i)
	   {
		   units[i][0].x += units[i][1].x;
		   units[i][0].y += units[i][1].y;
		   if(units[i][0].x > rodeo_screen_width_get() - orc_size[0] || units[i][0].x < 0)
		   {
			   units[i][1].x = -units[i][1].x;
		   }
		   if(units[i][0].y > rodeo_screen_height_get() - orc_size[1] || units[i][0].y < 0)
		   {
			   units[i][1].y = -units[i][1].y;
		   }

		   rodeo_texture_2d_draw(
				&(rodeo_rectangle_t){
					.x = units[i][0].x,
					.y = units[i][0].y,
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
				&texture
				);
	   }

	   rodeo_texture_2d_draw(
			&(rodeo_rectangle_t){
				.x = (float)(int32_t)summon_position.x - (orc_size[0] / 2.0f),
				.y = (float)(int32_t)summon_position.y - (orc_size[1] / 2.0f) ,
				.width = orc_size[0],
				.height = orc_size[1],
			},
			&(rodeo_rectangle_t){
				.x = 5,
				.y = 5,
				.width = 13,
				.height = 19
			},
			//&(rodeo_color_RGBAFloat_t){ .array = {1.0,1.0,1.0,1.0} },
			NULL,
			&texture
		);

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

		rodeo_debug_text_draw(
			2,
			4,
			" orc count: %"PRIu32" ",
			num_of_units
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
	mrodeo_window_do(480, 640, cstr_lit("Rodeo Window"))
	{
		renderer = rodeo_renderer_name_get();
		rodeo_frame_limit_set(60);

		//texture = rodeo_texture_2d_create_from_RGBA8(
		//	2,
		//	2,
		//	texture_memory
		//);

		texture = rodeo_texture_2d_create_from_path(cstr_lit("assets/orc.png"));

		rodeo_mainLoop_run(
			main_loop
		);

		rodeo_texture_2d_destroy(&texture);

	}

	unregister_inputs();

	return 0;
}
