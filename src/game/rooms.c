#include "rooms.h"

room* create_room(int posx, int posy) {
    room* r = malloc(sizeof(room));
    r->x = posx;
    r->y = posy;
    r->entities = create_list();
    r->up = NULL;
    r->down = NULL;
    r->left = NULL;
    r->right = NULL;

    add_enemy(r, create_monster(rand() % 640, rand() % 360));
    return r;
}

void free_room(room* r) {
    if(r == NULL) return;
    free_list(r->entities);
}

void print_entity_modif(void* data) {
    entity_container* c = data;
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
    entity_container* c = malloc(sizeof(entity_container));
    c->data = e;
    c->type = ENTITY_SIMPLE;
    append_elt(r->entities, c);
}
void add_enemy(room* r, enemy* e) {
    entity_container* c = malloc(sizeof(entity_container));
    c->data = e;
    c->type = ENEMY;
    append_elt(r->entities, c);
}
void add_item(room* r, item* i) {
    entity_container* c = malloc(sizeof(entity_container));
    c->data = i;
    c->type = ITEM;
    append_elt(r->entities, c);
}

void update_room(player* p, room* r) {
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((entity_container*) get_data(c) != NULL) {
            if(((entity_container*) get_data(c))->type == ENTITY_SIMPLE)
                update_entity(((entity_container*) get_data(c))->data);
            else if(((entity_container*) get_data(c))->type == ENEMY)
                update_enemy(p, ((entity_container*) get_data(c))->data, r->entities);
            else if(((entity_container*) get_data(c))->type == ITEM)
                update_item(((entity_container*) get_data(c))->data);
        }
    }
}

void draw_room(SDL_Renderer* ren, room* r) {
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((entity_container*) get_data(c) != NULL) {
            if(((entity_container*) get_data(c))->type == ENTITY_SIMPLE)
                draw_entity(ren, ((entity_container*) get_data(c))->data);
            else if(((entity_container*) get_data(c))->type == ENEMY)
                draw_enemy(ren, ((entity_container*) get_data(c))->data);
            else if(((entity_container*) get_data(c))->type == ITEM)
                draw_item(ren, ((entity_container*) get_data(c))->data);
        }
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