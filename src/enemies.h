#pragma once
#include "rodeo/collision.h"
#include "rodeo.h"

typedef
struct
{
    float hp;
    world_id id;
} enemy_t;

#define i_val enemy_t
#define i_opt c_no_cmp
#include "stc/cvec.h"


void
init_enemies(void);

void
deinit_enemies(void);

enemy_t*
spawn_enemy(
    float x,
    float y
);

void
draw_enemies(void);

