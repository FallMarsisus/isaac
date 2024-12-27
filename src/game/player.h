#pragma once

#include "rooms.h"
#include "map.h"
#include "../utils/vector.h"
#include "anim.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct player_s player;

player* create_player(int x, int y);
void load_player_textures(player* p, SDL_Renderer* ren, char* path);

void free_player(player* p);

void get_inputs(player* p);

//Returns the room currently in
room* update(player* p, int width, int height, map* m, room* r);

void draw_player(SDL_Renderer* ren, player* p);