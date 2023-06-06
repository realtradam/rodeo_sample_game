#include "input.h"
#include "init.h"
#include "menu.h"
#include "player.h"
#include "rodeo.h"

rodeo_gfx_texture_2d_t splash_texture;
rodeo_gfx_texture_2d_t main_menu_texture;
rodeo_gfx_texture_2d_t gameover_texture;
menu_state_t menu_state;
float splash_timer;
rodeo_rectangle_t screen_dimensions = (rodeo_rectangle_t){.x = 0, .y = 0, .width = 1600, .height = 900};

void
init_menu(void)
{
	splash_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/splash.png"));
	main_menu_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/main_menu.png"));
	gameover_texture = rodeo_gfx_texture_2d_create_from_path(cstr_lit("assets/restart_menu.png"));
	
	menu_state = menu_state_main;
	splash_timer = 3000.0f;
	rodeo_input_scene_activate(get_command_inputs()->menu_scene);
}

void
deinit_menu(void)
{
	rodeo_gfx_texture_2d_destroy(splash_texture);
	rodeo_gfx_texture_2d_destroy(main_menu_texture);
	rodeo_gfx_texture_2d_destroy(gameover_texture);
}

void
draw_menu(void)
{
	float transparency = 0.65f;
	if (splash_timer > 0 && menu_state == menu_state_splash)
	{
		splash_timer -= rodeo_gfx_frame_time_get();
		rodeo_gfx_texture_2d_draw(
			screen_dimensions, 
			screen_dimensions,
			(rodeo_color_RGBAFloat_t){.array = {1,1,1,1}},
			splash_texture
		);
		if (splash_timer <= 0) {
			menu_state = menu_state_main;
		}
	}
	else if (menu_state == menu_state_main)
	{

		rodeo_gfx_texture_2d_draw(
			screen_dimensions, 
			screen_dimensions,
			(rodeo_color_RGBAFloat_t){ .array = { 0,0,0,transparency }},
			(rodeo_gfx_texture_2d_t){0}
		);
		rodeo_gfx_texture_2d_draw(
			screen_dimensions, 
			screen_dimensions,
			(rodeo_color_RGBAFloat_t){.array = {1,1,1,1}},
			main_menu_texture
		);
	}
	else if (menu_state == menu_state_gameover)
	{
		rodeo_gfx_texture_2d_draw(
			screen_dimensions, 
			screen_dimensions,
			(rodeo_color_RGBAFloat_t){ .array = { 0,0,0,transparency }},
			(rodeo_gfx_texture_2d_t){0}
		);
		rodeo_gfx_texture_2d_draw(
			screen_dimensions, 
			screen_dimensions,
			(rodeo_color_RGBAFloat_t){.array = {1,1,1,1}},
			gameover_texture
		);
	}
}

void
parse_menu_input(void)
{
	int32_t hp = get_player_hp();
	bool input = *(bool*)menu_accept_input(NULL, NULL);
	
	if (input)
	{
		rodeo_input_scene_activate(get_command_inputs()->scene);
		if (menu_state == menu_state_gameover && hp <= 0)
		{
			reset_game_systems();
			menu_state = menu_state_inactive;
		}
		else if (menu_state == menu_state_main)
		{
			//init_game_systems();
			menu_state = menu_state_inactive;
		}
	}
}

menu_state_t
get_menu_state(void)
{
	return menu_state;
}

void
set_menu_state(menu_state_t state)
{
	menu_state = state;
}
