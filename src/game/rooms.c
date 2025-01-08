#include "rooms.h"

room* create_room(int posx, int posy) {
    room* r = malloc(sizeof(room));
    r->x = posx;
    r->y = posy;
    r->entities = create_list();
    r->tiles = create_list();
    r->items = create_list();
    r->up = NULL;
    r->down = NULL;
    r->left = NULL;
    r->right = NULL;

    add_entity_to_room(r, create_enemy(rand() % 640, rand() % 360));
    add_item_to_room(r, create_item(rand() % 640, rand() % 360, 16, 16, "assets/player/sword.bmp"));
    return r;
}
void add_entity_to_room(room* r, Entity* e) {
    append_elt(r->entities, e);
}
void add_item_to_room(room* r, Item* item) {
    if(r == NULL || item == NULL) return;
    append_elt(r->items, item);
}
void free_room(room* r) {
    if(r == NULL) return;
    free_list(r->items);
    free_list(r->entities);
    free_list(r->tiles);
    free(r);
}

void update_room(player* p, room* r) {
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((Entity*) get_data(c) != NULL) {
            Entity* e = get_data(c);
            update_entity(e, p, r->entities, r->tiles);
        }
    }
}

void draw_room(SDL_Renderer* ren, room* r) {
    for(cell* c = get_first(r->items); c != NULL; c = get_next(c)) {
        if((Item*) get_data(c) != NULL) {
            Item* item = get_data(c);
            draw_item(item, ren);
        }
    }
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((Entity*) get_data(c) != NULL) {
            Entity* e = get_data(c);
            if(e->draw != NULL) e->draw(e, ren);
            else draw_entity(e, ren);
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