#pragma once

#include "rooms.h"
#include "player.h"
#include "../utils/dict.h"

typedef struct {
    player* p;

    dict dict_rooms;

    int map_x; int map_y; //Map Coords
    room* current_room;
} map;

map* create_map();
void load_textures(map* m, SDL_Renderer* ren);
void free_map(map* m);

room* get_room(map* m, int posx, int posy);

void print_map(map* m);

void add_room(map* m, room* r);
void destroy_room(map* m, int x, int y);

void change_room(map* m, int x, int y);

void update_map(map* m, int win_width, int win_height);

void draw_map(map* m, SDL_Renderer* ren);