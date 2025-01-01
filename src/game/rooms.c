#include "rooms.h"

typedef enum {
    ENEMY,
    ITEM,
    ENTITY_SIMPLE
} entity_type;

typedef struct {
    entity_type type;
    void* data;
} container;

struct room {
    int x;
    int y;

    struct room* up;
    struct room* down;
    struct room* left;
    struct room* right;

    chained_list* entities;
};

typedef struct room room;

room* create_room(int posx, int posy) {
    room* r = malloc(sizeof(room));
    r->x = posx;
    r->y = posy;
    r->entities = create_list();
    r->up = NULL;
    r->down = NULL;
    r->left = NULL;
    r->right = NULL;

    add_entity(r, create_entity(rand() % 640, rand() % 360, 50, 50));
    return r;
}

void free_room(room* r) {
    if(r == NULL) return;
    free_list(r->entities);
}

int getX(room* r) {
    return r->x;
}
int getY(room* r) {
    return r->y;
}

void print_entity_modif(void* data) {
    container* c = data;
    print_entity((entity*) c->data);
}
void print_room(room* r) {
    printf("----------Printing room x : %d, y : %d----------\n", r->x, r->y);
    if(r->up) printf("Has up\n");
    if(r->down) printf("Has down\n");
    if(r->left) printf("Has left\n");
    if(r->right) printf("Has right\n");
    iter(r->entities, print_entity_modif);
}

void add_entity(room* r, entity* e) {
    container* c = malloc(sizeof(container));
    c->data = e;
    c->type = ENTITY_SIMPLE;
    append_elt(r->entities, c);
}

void update_room(room* r) {
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((container*) get_data(c) != NULL) {
            if(((container*) get_data(c))->type == ENTITY_SIMPLE)
                update_entity(((container*) get_data(c))->data);
            else if(((container*) get_data(c))->type == ENEMY)
                update_enemy(((container*) get_data(c))->data);
            else if(((container*) get_data(c))->type == ITEM)
                update_item(((container*) get_data(c))->data);
        }
    }
}

void draw_room(SDL_Renderer* ren, room* r) {
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((container*) get_data(c) != NULL) draw_entity(ren, ((container*) get_data(c))->data);
    }
}

void setUp(room* r, room* up) {
    assert(r != NULL && up != NULL);
    r->up = up;
    up->down = r;
}
void setDown(room* r, room* down) {
    assert(r != NULL && down != NULL);
    r->down = down;
    down->up = r;
}
void setLeft(room* r, room* left) {
    assert(r != NULL && left != NULL);
    r->left = left;
    left->right = r;
}
void setRight(room* r, room* right) {
    assert(r != NULL && right != NULL);
    r->right = right;
    right->left = r;
}

void clearUp(room* r, room* up) {
    assert(r != NULL && up != NULL);
    r->up = NULL;
    up->down = NULL;
}
void clearDown(room* r, room* down) {
    assert(r != NULL && down != NULL);
    r->down = NULL;
    down->up = NULL;
}
void clearLeft(room* r, room* left) {
    assert(r != NULL && left != NULL);
    r->left = NULL;
    left->right = NULL;
}
void clearRight(room* r, room* right) {
    assert(r != NULL && right != NULL);
    r->right = NULL;
    right->left = NULL;
}