#include "map.h"

struct map_s {
    dict dict_rooms;
};

typedef struct map_s map;

map* create_map() {
    map* m = malloc(sizeof(map));
    m->dict_rooms = create();
    return m;
}

void free_map(map* m) {
    assert(m != NULL);
    free_dict(m->dict_rooms);
    free(m);
}

void add_room(map* m, room* r) {
    assert(m != NULL);
    int x = getX(r);
    int y = getY(r);
    if(get_room(m, x, y) != NULL) return;
    add(getX(r), getY(r), r, m->dict_rooms);

    room* up = get_room(m, x, y - 1);
    if(up != NULL) setUp(r, up);
    room* down = get_room(m, x, y + 1);
    if(down != NULL) setDown(r, down);
    room* left = get_room(m, x - 1, y);
    if(left != NULL) setLeft(r, left);
    room* right = get_room(m, x + 1, y);
    if(right != NULL) setRight(r, right);
}
void destroy_room(map* m, room* r) {
    assert(m != NULL);
    int x = getX(r);
    int y = getY(r);
    if(get_room(m, x, y) == NULL) return;
    removeValue(x, y, m->dict_rooms);

    room* up = get_room(m, x, y - 1);
    if(up != NULL) clearUp(r, up);
    room* down = get_room(m, x, y + 1);
    if(down != NULL) clearDown(r, down);
    room* left = get_room(m, x - 1, y);
    if(left != NULL) clearLeft(r, left);
    room* right = get_room(m, x + 1, y);
    if(right != NULL) clearRight(r, right);
}

room* get_room(map* m, int posx, int posy) {
    assert(m != NULL);
    return getValue(posx, posy, m->dict_rooms);
}