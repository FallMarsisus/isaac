#pragma once

#include "rooms.h"
#include "../../utils/betterArrays/dicts/room_dict.h"

typedef struct map_s Map;

Map* create_map();
void free_map(Map* m);

Room* get_room(Map* m, int coord_x, int coord_y);

void add_room(Map* m, Room* r);
void destroy_room(Map* m, int coord_x, int coord_y);