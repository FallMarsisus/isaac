#include "rooms.h"

room* create_room(int posx, int posy, sprite_list* sprites) {
    room* r = malloc(sizeof(room));
    if (r == NULL) {
        fprintf(stderr, "Failed to allocate memory for room\n");
        return NULL;
    }
    r->x = posx;
    r->y = posy;

    int offsetX = 50;//random_int(50, 100);
    int offsetY = offsetX;
    r->bg_rect = malloc(sizeof(SDL_Rect));
    *(r->bg_rect) = (SDL_Rect) {offsetX, offsetY, 640 - 2 * offsetX, 360 - 2 * offsetY};

    r->entities = create_list();
    r->tiles = create_list();
    r->up = NULL;
    r->down = NULL;
    r->left = NULL;
    r->right = NULL;

    Entity* enemy = create_enemy(
        r->bg_rect->x + rand() % r->bg_rect->w, 
        r->bg_rect->y + rand() % r->bg_rect->h, 
        sprites
    );
    if (enemy != NULL) {
        add_entity_to_room(r, enemy);
    } else {
        fprintf(stderr, "Failed to create enemy\n");
    }

    Tile* tile = create_tile(
        r->bg_rect->x + rand() % r->bg_rect->w, 
        r->bg_rect->y + rand() % r->bg_rect->h, 
        32,
        32,
        sprites
    );
    if (tile != NULL) {
        add_tile_to_room(r, tile);
    } else {
        fprintf(stderr, "Failed to create tile\n");
    }

    return r;
}

void add_entity_to_room(room* r, Entity* e) {
    if (r == NULL || e == NULL) return;
    append_elt(r->entities, e);
}
void add_tile_to_room(room* r, Tile* tile) {
    if (r == NULL || tile == NULL) return;
    append_elt(r->tiles, tile);
}

void free_room(room* r) {
    if (r == NULL) return;
    free_list(r->entities);
    free_list(r->tiles);
    free(r);
}
void update_room(player* p, room* r, float delta) {
    for(cell* c = get_first(r->tiles); c != NULL; c = get_next(c)) {
        if((Tile*) get_data(c) != NULL) {
            Tile* tile = get_data(c);
            update_tile(tile, p);
        }
    }
    for(cell* c = get_first(r->entities); c != NULL; c = get_next(c)) {
        if((Entity*) get_data(c) != NULL) {
            Entity* e = get_data(c);
            update_entity(e, p, r->entities, r->tiles, delta);
        }
    }
}

void draw_room(SDL_Renderer* ren, room* r) {
    draw_rect(r->bg_rect, ren, 20, 20, 20, 255);

    for(cell* c = get_first(r->tiles); c != NULL; c = get_next(c)) {
        if((Tile*) get_data(c) != NULL) {
            Tile* tile = get_data(c);
            if(tile->draw != NULL) tile->draw(tile, ren);
            else draw_tile(tile, ren);
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