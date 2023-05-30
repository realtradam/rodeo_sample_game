#include "rodeo.h"
#include "enemies.h"
#include "debug.h"

void draw_debug_text(cstr renderer_name, float fps_display)
{
		rodeo_debug_text_draw(
			1,
			1,
			" using %s renderer ",
			cstr_str(
				&renderer_name
			)
		);

		rodeo_debug_text_draw(
			2,
			2,
			" frame count: %"PRIu64" ",
			rodeo_frame_count_get()
		);

		rodeo_debug_text_draw(
			2,
			3,
			" fps: %.2f ",
			fps_display
		);

		rodeo_debug_text_draw(
			2,
			4,
			" enemy count: %d ",
			get_enemy_count()
		);

		rodeo_debug_text_draw(
			2,
			5,
			" ghost count: %d ",
			get_ghost_count()
		);

		rodeo_debug_text_draw(
			2,
			6,
			" total count: %d ",
			get_ghost_count() + get_enemy_count()
		);
}
