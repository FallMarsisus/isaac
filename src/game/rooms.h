
#include "entities.h"

typedef struct room room;
typedef struct map map;

void add_room(map* m, int posx, int posy);
void destroy_room(map* m, int posx, int posy);

room* get_room(map* m, int posx, int posy);

room* get_up(map* m, room* r);
room* get_down(map* m, room* r);
room* get_left(map* m, room* r);
room* get_right(map* m, room* r);

map* create_map();
void free_map(map* m);