#include "entities.h"

struct entity_s {
    SDL_Rect* pos;
    bool has_sprite;
    SDL_Texture* texture;
};

typedef struct entity_s entity;

entity* create_entity(int x, int y, int width, int height) {
    entity* e = malloc(sizeof(entity));
    e->pos = malloc(sizeof(SDL_Rect));
    e->pos->x = x;
    e->pos->y = y;
    e->pos->w = width;
    e->pos->h = height;
    e->has_sprite = false;
    return e;
}
void free_entity(entity* e) {
    free(e);
}

SDL_Rect* get_pos(entity* e) {
    return e->pos;
}

void print_entity(entity* e) {
    printf("Entity {x : %d, y : %d, width : %d, height : %d}\n", e->pos->x, e->pos->y, e->pos->w, e->pos->h);
}

void set_pos(int x, int y, entity* e) {
    e->pos->x = x;
    e->pos->y = y;
}
void set_sprite(entity* e, SDL_Texture* tex) {
    assert(tex != NULL);
    e->has_sprite = true;
    e->texture = tex;
}

void draw_entity(SDL_Renderer* ren, entity* e) {
    if(e == NULL || e->pos == NULL) return;
    if(e->has_sprite) {
        SDL_RenderCopy(ren, e->texture, NULL, e->pos);
    }
    else {
        SDL_SetRenderDrawColor(ren , 0, 0, 255, 255 );
        SDL_RenderDrawRect(ren, e->pos);
    }
}
