#include "rooms.h"

typedef struct room_s {
    int coord_x; int coord_y;

    struct room_s* up;
    struct room_s* down;
    struct room_s* left;
    struct room_s* right;

    ID_array* entity_ids;

    int grid_width;
    int grid_height;
    int** grid;
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
    
    r->entity_ids = create_id_array();
    if (r->entity_ids == NULL) {
        free(r);
        return NULL;
    }

    r->grid_width = (int) ceil(1920 / 64) + 1;
    r->grid_height = (int) ceil(1080 / 64) + 1;

    r->grid = malloc(sizeof(int*) * r->grid_height);
    for(int y = 0; y < r->grid_height; y++) {
        r->grid[y] = malloc(sizeof(int) * r->grid_width);
        for(int x = 0; x < r->grid_width; x++) {
            r->grid[y][x] = 0;
        }
    }

    return r;
}

void free_room(Room* r) {
    if (r == NULL) return;
    free_id_array(r->entity_ids);
    for(int i = 0; i < r->grid_height; i++) {
        free(r->grid[i]);
    }
    free(r->grid);

    free(r);
}

void add_entity(Room* r, uint32_t id) {
    assert(r != NULL);
    add_id(r->entity_ids, id);
}

void remove_entity(Room* r, uint32_t id) {
    assert(r != NULL);
    remove_id(r->entity_ids, id);
}

int** get_grid(Room* r) {
    return r->grid;
}
int get_grid_width(Room* r) {
    return r->grid_width;
}
int get_grid_height(Room* r) {
    return r->grid_height;
}
uint32_t* get_entities(Room* r) {
    return get_ids(r->entity_ids);
}
int get_entity_amount(Room* r) {
    return get_ids_len(r->entity_ids);
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

Room* getUp(Room* r) {
    assert(r != NULL);
    return r->up;
}

Room* getDown(Room* r) {
    assert(r != NULL);
    return r->down;
}

Room* getLeft(Room* r) {
    assert(r != NULL);
    return r->left;
}

Room* getRight(Room* r) {
    assert(r != NULL);
    return r->right;
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