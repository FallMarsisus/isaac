#include "map.h"

typedef struct map_s {
    dict dict_rooms;
} Map;

Map* create_map() {
    Map* m = malloc(sizeof(Map));
    m->dict_rooms = create_dict();
    return m;
}

void free_room_modif(int x, int y, void* data) {
    free_room((Room*) data);
}
void free_map(Map* m) {
    assert(m != NULL && m->dict_rooms != NULL);
    iter_dict(free_room_modif, m->dict_rooms);
    free_dict(m->dict_rooms, false);
    free(m);
}

Room* get_room(Map* m, int posx, int posy) {
    assert(m != NULL);
    if(!mem(posx, posy, m->dict_rooms)) {
        return NULL;
    }
    return (Room*) getValue(posx, posy, m->dict_rooms);
}

void add_room(Map* m, Room* r) {
    assert(m != NULL && r != NULL && m->dict_rooms != NULL);
    int x = get_x(r);
    int y = get_y(r);
    add(x, y, r, m->dict_rooms);

    Room* up = get_room(m, x, y - 1);
    if(up != NULL) setUp(r, up);
    Room* down = get_room(m, x, y + 1);
    if(down != NULL) setDown(r, down);
    Room* left = get_room(m, x - 1, y);
    if(left != NULL) setLeft(r, left);
    Room* right = get_room(m, x + 1, y);
    if(right != NULL) setRight(r, right);
}
void destroy_room(Map* m, Room* r) {
    assert(m != NULL && r != NULL);
    int x = get_x(r); int y = get_y(r);

    Room* up = get_room(m, x, y - 1);
    if(up != NULL) clearUp(r, up);
    Room* down = get_room(m, x, y + 1);
    if(down != NULL) clearDown(r, down);
    Room* left = get_room(m, x - 1, y);
    if(left != NULL) clearLeft(r, left);
    Room* right = get_room(m, x + 1, y);
    if(right != NULL) clearRight(r, right);

    printf("%d\n", removeValue(x, y, m->dict_rooms));
    free_room(r);
}