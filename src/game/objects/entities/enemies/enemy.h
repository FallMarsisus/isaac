#pragma once

#include <SDL2/SDL.h>

#include "../entity.h"
#include "../../../anim.h"
#include "../player/player.h"

#include "../../../../utils/chained_list.h"

Entity* create_enemy(float x, float y);
void free_additional(Entity* e);

void update_enemy(Entity* e, void* pl, chained_list* entities, chained_list* tiles);
void draw_enemy(SDL_Renderer* ren, Entity* e);