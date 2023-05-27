
#include "rodeo.h"
#include "sprite.h"

void
draw_sprite(sprite_t *sprite, float x, float y, float scale)
{
	rodeo_texture_2d_draw(
		&(rodeo_rectangle_t){
			.x = x,
			.y = y,
			.width = (float)sprite->config.width * scale,
			.height = (float)sprite->config.height * scale 
		},
		&(rodeo_rectangle_t){ 
			.x = (float)sprite->config.width * (float)sprite->iter,
			.y = 0,
			.width = (float)sprite->config.width,
			.height = (float)sprite->config.height
		},
		&(rodeo_color_RGBAFloat_t){ .array = { 1.0f, 1.0f, 1.0f, 1.0f } },
		sprite->config.texture
	);
}
