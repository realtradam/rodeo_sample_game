#pragma once

#include "rodeo.h"

typedef
struct
{
	float iter;
	struct config
	{
		uint32_t width;
		uint32_t height;
		uint32_t count;
		rodeo_gfx_texture_2d_t texture;
	} config;
}
sprite_t;

void
draw_sprite(sprite_t *sprite, float x, float y, float scale, rodeo_color_RGBAFloat_t color);

