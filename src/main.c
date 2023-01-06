#include <stdio.h>
#include <stdbool.h>

#include "rodeo.h"

Rodeo__data_t _state = {0};
Rodeo__data_t *state = &_state;

const struct Rodeo__color_rgba_t red =
{
	1.0f, 0.0f, 0.0f,
	1.0f
};
const struct Rodeo__color_rgba_t green =
{
	0.0f, 1.0f, 0.0f,
	1.0f
};
const struct Rodeo__color_rgba_t blue =
{
	0.0f, 0.0f, 1.0f,
	1.0f
};
const struct Rodeo__color_rgba_t pink =
{
	1.0f, 0.0f, 1.0f,
	1.0f
};

int
main()
{
	Rodeo__init_window(state, 480, 640, "Rodeo Window");


	while(!state->quit)
	{
		Rodeo__begin(state);
		Rodeo__draw_rectangle(state, -1, 0, 1, 1, pink);

		Rodeo__draw_rectangle(state, 100, 100, 50, 50, red);
		Rodeo__draw_rectangle(state, 100, 160, 50, 50, green);
		Rodeo__draw_rectangle(state, 160, 100, 50, 50, blue);
		Rodeo__draw_rectangle(state, 160, 160, 50, 50, pink);


		Rodeo__draw_debug_text(
				1,
				1,
				" using %s renderer ",
				Rodeo__get_renderer_name_as_string()
				);

		Rodeo__end(state);
	}

	Rodeo__deinit_window(state);
	Rodeo__quit();
	return 0;
}
