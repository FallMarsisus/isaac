#include "map.h"

struct map {
    chained_list** dict_rooms;
    int amount;
    int maxlen;
};

typedef struct map map;

int h(map* m, int posx, int posy) {
    return ipow(2, posx) * ipow(3, posy) % m->maxlen;
}

map* create_map();
void free_map(map* m);

void add_room(map* m, room* r);
void destroy_room(map* m, room* r);

room* get_room(map* m, int posx, int posy);

room* get_up(map* m, room* r);
room* get_down(map* m, room* r);
room* get_left(map* m, room* r);
room* get_right(map* m, room* r);