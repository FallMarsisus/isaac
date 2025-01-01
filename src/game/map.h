#pragma once

#include "rooms.h"
#include "player.h"
#include "../utils/dict.h"

typedef struct map_s map;

map* create_map();
void load_textures(map* m, SDL_Renderer* ren);
void free_map(map* m);

int get_map_x(map* m);
int get_map_y(map* m);
room* get_current_room(map* m);
room* get_room(map* m, int posx, int posy);

void print_map(map* m);

void add_room(map* m, room* r);
void destroy_room(map* m, int x, int y);

void set_map_x(map* m, int x);
void set_map_y(map* m, int y);
void set_current_room(map* m, room* r);
void change_room(map* m, int x, int y);

void update_map(map* m, int win_width, int win_height);

void draw_map(map* m, SDL_Renderer* ren);