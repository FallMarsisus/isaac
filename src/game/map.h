#pragma once

#include "rooms.h"
#include "objects/entities/player/player.h"
#include "../utils/dict.h"
#include "../display.h"
#include "inventory.h"

typedef struct {
    player* p;

    dict dict_rooms;

    int map_x; int map_y; //Map Coords
    room* current_room;

    sprite_list* sprites;
} map;

map* create_map(sprite_list* sprites);
void load_textures(map* m, SDL_Renderer* ren);
void free_map(map* m);

room* get_room(map* m, int posx, int posy);

void add_room(map* m, room* r);
void destroy_room(map* m, int x, int y);

void change_room(map* m, int x, int y);

void update_map(map* m, int win_width, int win_height);

void draw_map(map* m, SDL_Renderer* ren);