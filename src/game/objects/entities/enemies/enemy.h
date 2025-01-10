#pragma once

#include <SDL2/SDL.h>

#include "../entity.h"
#include "../../../anim.h"
#include "../player/player.h"

#include "../../../../utils/chained_list.h"

Entity* create_enemy(float x, float y, sprite_list* sprites);