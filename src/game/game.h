#pragma once

#include "structure/map.h"
#include "objects/entities/player/player.h"

typedef struct game_s Game;

void load_assets();

Game* create_game();

void update_game(Game* game);

void draw_game(SDL_Renderer* ren, Game* game);