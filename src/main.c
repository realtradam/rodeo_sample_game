
#include "rodeo.h"

rodeo_data_p state;

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
	mrodeo_do(state)
	{
	   rodeo_rectangle_draw(
	   state,
	   100, 100,
	   50, 50,
	   red
	   );
	   rodeo_rectangle_draw(
	   state,
	   100, 160,
	   50, 50,
	   green
	   );
	   rodeo_rectangle_draw(
	   state,
	   160, 100,
	   50, 50,
	   blue
	   );
	   rodeo_rectangle_draw(
	   state,
	   160, 160,
	   50, 50,
	   pink
	   );

	   rodeo_rectangle_draw(
	   state,
	   300, 300,
	   50, 50,
	   red_clear
	   );
	   rodeo_rectangle_draw(
	   state,
	   310, 310,
	   50, 50,
	   green_clear
	   );
	   rodeo_rectangle_draw(
	   state,
	   320, 320,
	   50, 50,
	   blue_clear
	   );
	   rodeo_rectangle_draw(
	   state,
	   330, 330,
	   50, 50,
	   pink_clear
	   );

	rodeo_debug_text_draw(
			1,
			1,
			" using %s renderer ",
			rodeo_renderer_name_get()
			);
	}
}

int
main()
{
	printf("Init window...\n");
	mrodeo_window_do(state, 480, 640, "Rodeo Window")
	{
		printf("done creating window\n");

		rodeo_mainloop_set(
				state,
				main_loop
				);

	}
	rodeo_deinit();
	return 0;
}
