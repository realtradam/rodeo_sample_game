#pragma once
#include "rodeo.h"

typedef
enum
{
    menu_state_splash,
    menu_state_main,
    menu_state_inactive,
	menu_state_gameover
}
menu_state_t;

void
init_menu(void);

void
deinit_menu(void);

void
draw_menu(void);

void
parse_menu_input(void);

menu_state_t
get_menu_state(void);

void
set_menu_state(menu_state_t);
