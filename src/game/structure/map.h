#pragma once

#include "rooms.h"
#include "objects/entities/player/player.h"
#include "../utils/dict.h"
#include "../display.h"
#include "inventory.h"

typedef struct map_s Map;

Map* create_map(sprite_list* sprites);
void load_textures(Map* m, SDL_Renderer* ren);
void free_map(Map* m);

room* get_room(Map* m, int posx, int posy);

void add_room(Map* m, room* r);
void destroy_room(Map* m, int x, int y);

void change_room(Map* m, int x, int y);

void update_map(Map* m, int win_width, int win_height, float delta);

void draw_map(Map* m, SDL_Renderer* ren);