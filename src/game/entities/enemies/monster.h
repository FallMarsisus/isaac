#pragma once

#include <SDL2/SDL.h>
#include "../enemy.h"

enemy* create_monster(int x, int y);
void update_monster(enemy* e);