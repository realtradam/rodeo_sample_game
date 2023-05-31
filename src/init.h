#include "rodeo.h"

void
init_game_systems(void);

void
deinit_game_systems(void);

#define game_systems_init_do() \
	mrodeo_defer_do( \
		init_game_systems(), \
		deinit_game_systems() \
	)

void
reset_game_systems(void);
