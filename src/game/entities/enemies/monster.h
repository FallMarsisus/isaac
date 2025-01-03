#pragma once

#include <SDL2/SDL.h>
#include "../enemy.h"

#include "../../../utils/vector.h"

enemy* create_monster(int x, int y);
void update_monster(player* p, enemy* e);