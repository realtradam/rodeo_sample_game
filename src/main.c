#include <stdio.h>
#include <stdbool.h>
//#include "SDL2/SDL.h"
//#include "SDL2/SDL_syswm.h"
//#include "bgfx/c99/bgfx.h"

#include "rodeo.h"

Rodeo__data_t _state = {0};
Rodeo__data_t *state = &_state;

int
main()
{
	Rodeo__init_window(state, 480, 640, "Rodeo Window");

	while(!state->quit)
	{
		Rodeo__begin(state);

		Rodeo__draw_debug_text(1, 1, "Debug %s Test", "Text");

		Rodeo__end(state);
	}

	Rodeo__deinit_window(state);
	Rodeo__quit();
	return 0;
}
