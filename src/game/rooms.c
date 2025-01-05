#include "rooms.h"

room* create_room(int posx, int posy) {
    room* r = malloc(sizeof(room));
    r->x = posx;
    r->y = posy;
    r->entities = create_list();
    r->tiles = create_list();
    r->up = NULL;
    r->down = NULL;
    r->left = NULL;
    r->right = NULL;

    add_entity_to_room(r, create_entity(rand() % 640, rand() % 360, 32, 32, "assets/player/sprite_sheet.bmp"), ENTITY_SIMPLE);
    return r;
}
void add_entity_to_room(room* r, void* e, entity_type type) {
    entity_container* e_c = malloc(sizeof(entity_container));
    e_c->data = e;
    e_c->type = type;

    append_elt(r->entities, e_c);
}
void free_room(room* r) {
    if(r == NULL) return;
    free_list(r->entities);
    free_list(r->tiles);
    free(r);
}

void update_room(player* p, room* r) {
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((entity_container*) get_data(c) != NULL) {
            entity_container* e_c = get_data(c);
            switch (e_c->type) {
                case ENEMY: break;
                case NPC: break;
                case ENTITY_SIMPLE:
                    Entity* e = e_c->data;
                    update_entity(e, p, r->entities, r->tiles);
                    break;
                default: break;
            }
        }
    }
}

void draw_room(SDL_Renderer* ren, room* r) {
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((entity_container*) get_data(c) != NULL) {
            entity_container* e_c = get_data(c);
            switch (e_c->type) {
                case ENEMY: break;
                case NPC: break;
                case ENTITY_SIMPLE:
                    Entity* e = e_c->data;
                    draw_entity(e, ren);
                    break;
                default: break;
            }
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