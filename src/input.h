#pragma once
#include "rodeo/input.h"

typedef
struct
{
	rodeo_input_scene_t *scene;
	rodeo_input_command_t *create;
	rodeo_input_command_t *left;
	rodeo_input_command_t *right;
	rodeo_input_command_t *up;
	rodeo_input_command_t *down;
	rodeo_input_command_t *play_sound;
	rodeo_input_command_t *play_sound2;
	rodeo_input_command_t *play_sound3;
}
scenes_and_commands_t;

void* 
summon_units_input(rodeo_input_any_state_t *input_state, void *data);

void
units_move_generic_input(
	rodeo_input_any_state_t *input_state,
	float *move,
	bool *binary_key,
	float *unbounded_range,
	float *bounded_range,
	bool should_be_positive,
	bool reset
);

void* 
units_move_right_input(
	rodeo_input_any_state_t *input_state,
	void *data
);

void* 
units_move_left_input(
	rodeo_input_any_state_t *input_state,
	void *data
);

void* 
units_move_up_input(
	rodeo_input_any_state_t *input_state,
	void *data
);

void* 
units_move_down_input(
	rodeo_input_any_state_t *input_state,
	void *data
);

void*
play_sound_input(
	rodeo_input_any_state_t *input_state,
	void *data
);

void*
play_sound_input2(
	rodeo_input_any_state_t *input_state,
	void *data
);

void*
play_sound_input3(
	rodeo_input_any_state_t *input_state,
	void *data
);

void
register_inputs(void);

void
unregister_inputs(void);
