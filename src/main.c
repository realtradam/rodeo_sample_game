
#include "rodeo.h"
#include <inttypes.h>

rodeo_string_t renderer;
float time_var;

const uint8_t texture_memory[] = {
	0x00, 0xff, 0xff, 0xff,
	0xff, 0x00, 0xff, 0xff,
	0xff, 0xff, 0x00, 0xff,
	0xff, 0xff, 0xff, 0x99,
};

rodeo_texture_2d_t texture;

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
main_loop(void)
{
	if(rodeo_frame_count_get() % 10 == 0)
	{
		time_var = rodeo_frame_perSecond_get();
	}
	mrodeo_frame_do()
	{
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

	   float orc_size[] = {13.0f * 2.0f, 19.0f * 2.0f};
	   rodeo_texture_2d_draw(
			&(rodeo_rectangle_t){
				.x = (float)rodeo_input_mouse_x_get() - (orc_size[0] / 2.0f),
				.y = (float)rodeo_input_mouse_y_get() - (orc_size[1] / 2.0f) ,
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
			rodeo_string_to_constcstr(
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
	mrodeo_window_do(480, 640, "Rodeo Window")
	{
		renderer = rodeo_renderer_name_get();
		rodeo_frame_limit_set(60);

		//texture = rodeo_texture_2d_create_from_RGBA8(
		//	2,
		//	2,
		//	texture_memory
		//);

		texture = rodeo_texture_2d_create_from_path(rodeo_string_create("assets/orc.png"));

		rodeo_mainLoop_run(
			main_loop
		);

		rodeo_texture_2d_destroy(&texture);

	}
	return 0;
}
