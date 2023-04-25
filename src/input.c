
#include "input.h"
#include "rodeo/input.h"

scenes_and_commands_t inputs = {0};

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

void* 
summon_units_input(rodeo_input_any_state_t *input_state, void *data)
{
	static bool should_summon_units = false;

	if(input_state != NULL)
	{
		should_summon_units = input_state->data.binary_state;
	}
	return &should_summon_units;
}

void
units_move_generic_input(
	rodeo_input_any_state_t *input_state,
	float *move,
	bool *binary_key,
	float *unbounded_range,
	float *bounded_range,
	bool should_be_positive,
	bool submit_inputs
)
{
	if(submit_inputs)
	{
		*move = 0.0f;
		*move += (should_be_positive ? 1 : -1) * (float)*binary_key * 5.0f;
		*move += (*unbounded_range);
		*unbounded_range = 0.0f;
		*move += ((*bounded_range) * 15.0f);
	}
	if(input_state != NULL)
	{
		if(input_state->type == rodeo_input_type_Binary)
		{
			*binary_key = input_state->data.binary_state;
		}
		else if(input_state->type == rodeo_input_type_UnboundedRange)
		{
			if((input_state->data.unbounded_range_state < 0) && !should_be_positive)
			{
				*unbounded_range += input_state->data.unbounded_range_state;
			}
			else if((input_state->data.unbounded_range_state > 0) && should_be_positive)
			{
				*unbounded_range += input_state->data.unbounded_range_state;
			}
		}
		else if(input_state->type == rodeo_input_type_BoundedRange)
		{
			if((input_state->data.bounded_range_state < -0.03) && !should_be_positive)
			{
				*bounded_range = input_state->data.bounded_range_state;
			}
			else if((input_state->data.bounded_range_state > 0.03) && should_be_positive)
			{
				*bounded_range = input_state->data.bounded_range_state;
			}
			else
			{
				*bounded_range = 0;
			}
		}
	}
}

void* 
units_move_right_input(rodeo_input_any_state_t *input_state, void *data)
{
	static float move = 0;
	static bool binary_key = false;
	static float unbounded_range = 0.0f;
	static float bounded_range = 0.0f;

	bool submit_inputs = false;
	if(data == NULL)
	{
		data = &submit_inputs;
	}
	units_move_generic_input(
		input_state,
		&move,
		&binary_key,
		&unbounded_range,
		&bounded_range,
		true,
		*(bool*)data
	);
	return &move;
} 

void* 
units_move_left_input(rodeo_input_any_state_t *input_state, void *data)
{
	static float move = 0;
	static bool binary_key = false;
	static float unbounded_range = 0.0f;
	static float bounded_range = 0.0f;

	bool submit_inputs = false;
	if(data == NULL)
	{
		data = &submit_inputs;
	}
	units_move_generic_input(
		input_state,
		&move,
		&binary_key,
		&unbounded_range,
		&bounded_range,
		false,
		*(bool*)data
	);
	return &move;
} 

void* 
units_move_up_input(rodeo_input_any_state_t *input_state, void *data)
{
	static float move = 0;
	static bool binary_key = false;
	static float unbounded_range = 0.0f;
	static float bounded_range = 0.0f;

	bool submit_inputs = false;
	if(data == NULL)
	{
		data = &submit_inputs;
	}
	units_move_generic_input(
		input_state,
		&move,
		&binary_key,
		&unbounded_range,
		&bounded_range,
		false,
		*(bool*)data
	);
	return &move;
} 

void* 
units_move_down_input(rodeo_input_any_state_t *input_state, void *data)
{
	static float move = 0;
	static bool binary_key = false;
	static float unbounded_range = 0.0f;
	static float bounded_range = 0.0f;

	bool submit_inputs = false;
	if(data == NULL)
	{
		data = &submit_inputs;
	}
	units_move_generic_input(
		input_state,
		&move,
		&binary_key,
		&unbounded_range,
		&bounded_range,
		true,
		*(bool*)data
	);
	return &move;
} 

#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

void
register_inputs(void)
{
	// make scene
	inputs.scene = rodeo_input_scene_create();

	// - make commands -
	inputs.create = rodeo_input_command_create(rodeo_input_type_Binary);

	inputs.up = rodeo_input_command_create(
		rodeo_input_type_Binary | rodeo_input_type_UnboundedRange | rodeo_input_type_BoundedRange
	);
	inputs.down = rodeo_input_command_create(
		rodeo_input_type_Binary | rodeo_input_type_UnboundedRange | rodeo_input_type_BoundedRange
	);
	inputs.left = rodeo_input_command_create(
		rodeo_input_type_Binary | rodeo_input_type_UnboundedRange | rodeo_input_type_BoundedRange
	);
	inputs.right = rodeo_input_command_create(
		rodeo_input_type_Binary | rodeo_input_type_UnboundedRange | rodeo_input_type_BoundedRange
	);
	// --

	// - register inputs -
	rodeo_input_command_register_binary_scancode(inputs.create, rodeo_input_binary_scancode_SPACE);
	rodeo_input_command_register_binary_controllerButton(inputs.create, rodeo_input_binary_controllerButton_A);
	rodeo_input_command_register_binary_mouseButton(inputs.create, rodeo_input_binary_mouseButton_LEFT);

	rodeo_input_command_register_binary_scancode(inputs.up, rodeo_input_binary_scancode_W);
	rodeo_input_command_register_binary_scancode(inputs.down, rodeo_input_binary_scancode_S);
	rodeo_input_command_register_binary_scancode(inputs.left, rodeo_input_binary_scancode_A);
	rodeo_input_command_register_binary_scancode(inputs.right, rodeo_input_binary_scancode_D);
	rodeo_input_command_register_unboundedRange_mouse(inputs.up, rodeo_input_unboundedRange_mouse_Y);
	rodeo_input_command_register_unboundedRange_mouse(inputs.down, rodeo_input_unboundedRange_mouse_Y);
	rodeo_input_command_register_unboundedRange_mouse(inputs.left, rodeo_input_unboundedRange_mouse_X);
	rodeo_input_command_register_unboundedRange_mouse(inputs.right, rodeo_input_unboundedRange_mouse_X);
	rodeo_input_command_register_boundedRange_controllerAxis(inputs.up, rodeo_input_boundedRange_controllerAxisLeft_Y);
	rodeo_input_command_register_boundedRange_controllerAxis(inputs.down, rodeo_input_boundedRange_controllerAxisLeft_Y);
	rodeo_input_command_register_boundedRange_controllerAxis(inputs.left, rodeo_input_boundedRange_controllerAxisLeft_X);
	rodeo_input_command_register_boundedRange_controllerAxis(inputs.right, rodeo_input_boundedRange_controllerAxisLeft_X);
	// --

	// - register callbacks -
	rodeo_input_command_register_callback(inputs.create, *summon_units_input);
	rodeo_input_command_register_callback(inputs.up, *units_move_up_input);
	rodeo_input_command_register_callback(inputs.down, *units_move_down_input);
	rodeo_input_command_register_callback(inputs.left, *units_move_left_input);
	rodeo_input_command_register_callback(inputs.right, *units_move_right_input);
	// --

	// - register commands -
	rodeo_input_scene_register_command(inputs.scene, inputs.create);
	rodeo_input_scene_register_command(inputs.scene, inputs.up);
	rodeo_input_scene_register_command(inputs.scene, inputs.down);
	rodeo_input_scene_register_command(inputs.scene, inputs.left);
	rodeo_input_scene_register_command(inputs.scene, inputs.right);
	// --

	// activate scene
	rodeo_input_scene_activate(inputs.scene);
}

void
unregister_inputs(void)
{
	rodeo_input_scene_unregister_command(inputs.scene, inputs.create);
	rodeo_input_command_destroy(inputs.create);
	rodeo_input_scene_destroy(inputs.scene);
}
