
#include "rodeo.h"
#include "sprite.h"

void
draw_sprite(sprite_t *sprite, float x, float y, float scale, rodeo_color_RGBAFloat_t color)
{
	rodeo_texture_2d_draw(
		&(rodeo_rectangle_t){
			.x = x,// - ((float)sprite->config.width * scale / 2),
			.y = y,// - ((float)sprite->config.height * scale / 2),
			.width = (float)sprite->config.width * scale,
			.height = (float)sprite->config.height * scale 
		},
		&(rodeo_rectangle_t){ 
			.x = (float)sprite->config.width * (float)sprite->iter,
			.y = 0,
			.width = (float)sprite->config.width,
			.height = (float)sprite->config.height
		},
		&color,
		sprite->config.texture
	);
}
