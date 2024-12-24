#include "map.h"

struct map_s {
    dict dict_rooms;
};

typedef struct map_s map;

map* create_map() {
    map* m = malloc(sizeof(map));
    m->dict_rooms = create_dict();
    return m;
}

void free_map(map* m) {
    assert(m != NULL && m->dict_rooms != NULL);
    free_dict(m->dict_rooms, true);
    //free(m);
}

void print_rm(int x, int y, void* data) {
    print_room((room*) data);
}
void print_map(map* m) {
    iter_dict(print_rm, m->dict_rooms);
}

void add_room(map* m, room* r) {
    assert(m != NULL && r != NULL && m->dict_rooms != NULL);
    int x = getX(r);
    int y = getY(r);
    add(x, y, r, m->dict_rooms);

    room* up = get_room(m, x, y - 1);
    if(up != NULL) setUp(r, up);
    room* down = get_room(m, x, y + 1);
    if(down != NULL) setDown(r, down);
    room* left = get_room(m, x - 1, y);
    if(left != NULL) setLeft(r, left);
    room* right = get_room(m, x + 1, y);
    if(right != NULL) setRight(r, right);
}
void destroy_room(map* m, int x, int y) {
    assert(m != NULL);
    room* r = get_room(m, x, y);
    if(r == NULL) return;

    room* up = get_room(m, x, y - 1);
    if(up != NULL) clearUp(r, up);
    room* down = get_room(m, x, y + 1);
    if(down != NULL) clearDown(r, down);
    room* left = get_room(m, x - 1, y);
    if(left != NULL) clearLeft(r, left);
    room* right = get_room(m, x + 1, y);
    if(right != NULL) clearRight(r, right);

    free_room(r);
    removeValue(x, y, m->dict_rooms);
}

room* get_room(map* m, int posx, int posy) {
    assert(m != NULL);
    if(!mem(posx, posy, m->dict_rooms)) {
        return NULL;
    }
    return (room*) getValue(posx, posy, m->dict_rooms);
}