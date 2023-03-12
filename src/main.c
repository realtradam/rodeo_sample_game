
#include "rodeo.h"
#include <inttypes.h>

rodeo_string_t renderer;
float time_var;

const rodeo_rgba_t red =
{
	1.0f, 0.0f, 0.0f,
	1.0f
};
const rodeo_rgba_t green =
{
	0.0f, 1.0f, 0.0f,
	1.0f
};
const rodeo_rgba_t blue =
{
	0.0f, 0.0f, 1.0f,
	1.0f
};
const rodeo_rgba_t pink =
{
	1.0f, 0.0f, 1.0f,
	1.0f
};

const rodeo_rgba_t red_clear =
{
	1.0f, 0.0f, 0.0f,
	0.5f
};
const rodeo_rgba_t green_clear =
{
	0.0f, 1.0f, 0.0f,
	0.5f
};
const rodeo_rgba_t blue_clear =
{
	0.0f, 0.0f, 1.0f,
	0.5f
};
const rodeo_rgba_t pink_clear =
{
	1.0f, 0.0f, 1.0f,
	0.5f
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

	   rodeo_rectangle_draw(
			(rodeo_rectangle_t){
				rodeo_input_mouse_x_get() - 25,
				rodeo_input_mouse_y_get() - 25,
				50, 50
			},
			pink
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
		rodeo_mainloop_run(
			main_loop
		);

	}
	return 0;
}
