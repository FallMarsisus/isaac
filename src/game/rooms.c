#include "rooms.h"

struct room {
    int x;
    int y;

    struct room* up;
    struct room* down;
    struct room* left;
    struct room* right;

    list_entity entities;
};

typedef struct room room;

room* create_room(int posx, int posy) {
    room* r = malloc(sizeof(room));
    r->x = posx;
    r->y = posy;
    r->entities = create_list();
}

void free_room(room* r) {
    if(r == NULL) return;
    free_entity_list(r->entities);
    free(r);
}