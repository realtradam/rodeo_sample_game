#include "init.h"
#include "bullet.h"
#include "player.h"
#include "enemies.h"
#include "wall.h"

void
init_game_systems(void)
{
	init_bullets();
	init_player();
	init_enemies();
	init_wall();
}

void
deinit_game_systems(void)
{
	deinit_bullets();
	deinit_player();
	deinit_enemies();
	deinit_wall();
}
