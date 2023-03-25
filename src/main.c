
#include "rodeo.h"
#include <inttypes.h>

rodeo_string_t renderer;
float time_var;

const uint32_t texture_memory[] = {
	0xff00ffff,
	0xff00ffff,
	0xff00ffff,
	0xff00ffff,
};

const uint8_t textureData[10 * 10 * 4] = {
    0, 0, 255, 255, 25, 25, 225, 255, 51, 51, 204, 255, 76, 76, 178, 255, 102, 102, 153, 255,
    127, 127, 127, 255, 153, 153, 102, 255, 178, 178, 76, 255, 204, 204, 51, 255, 225, 225, 25, 255,
    255, 255, 0, 255, 255, 230, 0, 255, 255, 204, 0, 255, 255, 178, 0, 255, 255, 153, 0, 255,
    255, 127, 0, 255, 255, 102, 0, 255, 255, 76, 0, 255, 255, 51, 0, 255, 255, 25, 0, 255,
};

rodeo_texture_2d_t texture;

const rodeo_RGBAFloat_t red =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 0.0f,
	.alpha = 1.0f
};
const rodeo_RGBAFloat_t green =
{ 
	.red = 0.0f, .green = 1.0f, .blue = 0.0f,
	.alpha = 1.0f
};
const rodeo_RGBAFloat_t blue =
{ 
	.red = 0.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 1.0f
};
const rodeo_RGBAFloat_t pink =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 1.0f
};

const rodeo_RGBAFloat_t red_clear =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 0.0f,
	.alpha = 0.5f
};
const rodeo_RGBAFloat_t green_clear =
{ 
	.red = 0.0f, .green = 1.0f, .blue = 0.0f,
	.alpha = 0.5f
};
const rodeo_RGBAFloat_t blue_clear =
{ 
	.red = 0.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 0.5f
};
const rodeo_RGBAFloat_t pink_clear =
{ 
	.red = 1.0f, .green = 0.0f, .blue = 1.0f,
	.alpha = 0.5f
};

void
main_loop(void)
{
	if(rodeo_frame_count_get() % 10 == 0)
	{
		time_var = rodeo_frame_persecond_get();
	}
	mrodeo_frame_do()
	{
		rodeo_rectangle_draw(
			(rodeo_rectangle_t){ 100, 100, 50, 50 },
			red
		);
	   rodeo_rectangle_draw(  
	   		(rodeo_rectangle_t){ 100, 160, 50, 50 },
	   		green
	   );
	   rodeo_rectangle_draw(  
	   		(rodeo_rectangle_t){ 160, 100, 50, 50 },
	   		blue
	   );
	   rodeo_rectangle_draw(  
	   		(rodeo_rectangle_t){ 160, 160, 50, 50 },
	   		pink
	   );

	   rodeo_rectangle_draw(  
	   		(rodeo_rectangle_t){ 300, 300, 50, 50 },
	   		red_clear
	   );
	   rodeo_rectangle_draw(  
	   		(rodeo_rectangle_t){ 310, 310, 50, 50 },
	   		green_clear
	   );
	   rodeo_rectangle_draw(  
	   		(rodeo_rectangle_t){ 320, 320, 50, 50 },
	   		blue_clear
	   );
	   rodeo_rectangle_draw(  
	   		(rodeo_rectangle_t){ 330, 330, 50, 50 },
	   		pink_clear
	   );

	   rodeo_texture_2d_draw(
			(rodeo_rectangle_t){
				0, 0,
				1, 1,
			},
			(rodeo_rectangle_t){
				rodeo_input_mouse_x_get() - 25,
				rodeo_input_mouse_y_get() - 25,
				50, 50,
			},
			(rodeo_RGBAFloat_t){ .array = {1.0,1.0,1.0,1.0} },
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
main()
{
	rodeo_log(
		rodeo_loglevel_info,
		"Testing logging... It seems to work!"
	);
	rodeo_log(
		rodeo_loglevel_warning,
		"Testing warning log level... It seems to work!"
	);
	rodeo_log(
		rodeo_loglevel_error,
		"Testing error log level... It seems to work!"
	);
	mrodeo_window_do(480, 640, "Rodeo Window")
	{
		renderer = rodeo_renderer_name_get();
		rodeo_frame_limit_set(60);
		texture = rodeo_texture_2d_create_from_RGBA8(
			10,
			10,
			textureData
		);
		rodeo_mainloop_run(
			main_loop
		);

	}
	return 0;
}
