#pragma once

#include <SDL2/SDL.h>

#include "../entity.h"
#include "../anim.h"
#include "../player/player.h"

#include "../../utils/chained_list.h"

entity_container* create_enemy(float x, float y, int w, int h);

void update_enemy(player* p, struct enemy_s* e, chained_list* entities);