#ifndef MAP_H_
#define MAP_H_

#include "rooms.h"
#include "../utils/dict.h"

typedef struct map_s map;

map* create_map();
void free_map(map* m);

void add_room(map* m, room* r);
void destroy_room(map* m, room* r);

room* get_room(map* m, int posx, int posy);

room* get_up(map* m, room* r);
room* get_down(map* m, room* r);
room* get_left(map* m, room* r);
room* get_right(map* m, room* r);

#endif