#pragma once

#include "structure/map.h"

#include "ecs/ecs.h"
#include "ecs/components.h"
#include "ecs/systems.h"

typedef struct game_s Game;

Game* create_game();
void free_game(Game* game);

void load_assets(Game* game);

void change_room(Game* game, int x, int y);

void get_keys(Game* game, SDL_Event* event);

void update_game(Game* game, int win_width, int win_height, float delta);

void draw_game(SDL_Renderer* ren, Game* game);