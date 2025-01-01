#pragma once

#include <SDL2/SDL.h>

#include "entities.h"

typedef struct enemy_s enemy;

enemy* create_enemy(int x, int y, int w, int h);
void free_enemy(enemy* e);

void update_enemy(enemy* e);
void draw_enemy(SDL_Renderer* ren, enemy* e);