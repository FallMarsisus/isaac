#include "rooms.h"

typedef struct room_s {
    int coord_x; int coord_y;

    struct room_s* up;
    struct room_s* down;
    struct room_s* left;
    struct room_s* right;
} Room;

Room* create_room(int posx, int posy) {
    Room* r = malloc(sizeof(Room));
    if (r == NULL) {
        fprintf(stderr, "Failed to allocate memory for Room\n");
        return NULL;
    }
    r->coord_x = posx; r->coord_y = posy;

    r->up = NULL;
    r->down = NULL;
    r->left = NULL;
    r->right = NULL;

    return r;
}

void free_room(Room* r) {
    if (r == NULL) return;
    free(r);
}

int get_x(Room* room) {
    return room->coord_x;
}
int get_y(Room* room) {
    return room->coord_y;
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