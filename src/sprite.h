#pragma once

#include "rodeo.h"

typedef
struct
{
	uint32_t iter;
	struct config
	{
		uint32_t width;
		uint32_t height;
		uint32_t count;
		rodeo_texture_2d_t *texture;
	} config;
}
sprite_t;

void
draw_sprite(sprite_t *sprite, float x, float y, float scale, rodeo_color_RGBAFloat_t color);

