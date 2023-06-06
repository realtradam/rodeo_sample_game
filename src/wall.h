#include "rodeo/collision.h"

void
init_wall(void);

void
deinit_wall(void);

#define wall_init_do() \
	mrodeo_defer_do( \
		init_wall(), \
		deinit_wall() \
	)

rodeo_collision_2d_collection_t
get_wall_world(void);

rodeo_collision_2d_item_t
new_wall(
    float x,
    float y,
    float width,
    float height
);

bool
coords_inside_wall(
    float x,
    float y
);

void
moving_wall_resolver(
	rodeo_collision_2d_item_data_t *obj_collision,
	rodeo_collision_2d_item_data_t *wall_collision
);

void
draw_level(void);
