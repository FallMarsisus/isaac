#pragma once

#include <SDL2/SDL.h>
#include "../enemy.h"
#include "../../rooms.h"

#include "../../../utils/vector.h"
#include "../../../utils/chained_list.h"

enemy* create_monster(int x, int y);
void update_monster(player* p, enemy* e, chained_list* entities);