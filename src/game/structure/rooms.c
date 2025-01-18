#include "rooms.h"

typedef struct room_s {
    int coord_x; int coord_y;

    struct room_s* up;
    struct room_s* down;
    struct room_s* left;
    struct room_s* right;

    chained_list* entities;
} Room;

Room* create_room(int posx, int posy) {
    Room* r = malloc(sizeof(Room));
    if (r == NULL) {
        fprintf(stderr, "Failed to allocate memory for Room\n");
        return NULL;
    }
    r->coord_x = posx; r->coord_y = posy;
    
    r->entities = create_list();

    r->up = NULL;
    r->down = NULL;
    r->left = NULL;
    r->right = NULL;

    return r;
}

/*
void add_entity_to_room(Room* r, Entity* e) {
    if (r == NULL || e == NULL) return;
    append_elt(r->entities, e);
}
*/

void free_room(Room* r) {
    if (r == NULL) return;

    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if(get_data(c) != NULL) {

        }
    }
    free_list(r->entities, false);

    free(r);
}

int get_x(Room* room) {
    return room->coord_x;
}
int get_y(Room* room) {
    return room->coord_y;
}
chained_list* get_entities(Room* room) {
    return room->entities;
}

void setUp(Room* r, Room* up) {
    assert(r != NULL && up != NULL);
    r->up = up;
    up->down = r;
}
void setDown(Room* r, Room* down) {
    assert(r != NULL && down != NULL);
    r->down = down;
    down->up = r;
}
void setLeft(Room* r, Room* left) {
    assert(r != NULL && left != NULL);
    r->left = left;
    left->right = r;
}
void setRight(Room* r, Room* right) {
    assert(r != NULL && right != NULL);
    r->right = right;
    right->left = r;
}

void clearUp(Room* r, Room* up) {
    assert(r != NULL && up != NULL);
    r->up = NULL;
    up->down = NULL;
}
void clearDown(Room* r, Room* down) {
    assert(r != NULL && down != NULL);
    r->down = NULL;
    down->up = NULL;
}
void clearLeft(Room* r, Room* left) {
    assert(r != NULL && left != NULL);
    r->left = NULL;
    left->right = NULL;
}
void clearRight(Room* r, Room* right) {
    assert(r != NULL && right != NULL);
    r->right = NULL;
    right->left = NULL;
}