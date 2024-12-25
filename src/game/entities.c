#include "entities.h"

struct entity_s {
    SDL_Rect* pos;
    SDL_Texture* texture;
};

typedef struct entity_s entity;

entity* create_entity(int x, int y, int width, int height, SDL_Texture* tex) {
    entity* e = malloc(sizeof(entity));
    e->pos = malloc(sizeof(SDL_Rect));
    e->pos->x = x;
    e->pos->y = y;
    e->pos->w = width;
    e->pos->h = height;
    e->texture = tex;
    return e;
}

void free_entity(entity* e) {
    free(e);
}

void draw_entity(SDL_Renderer* ren, entity* e) {
    if(e == NULL || e->texture == NULL || e->pos == NULL) return;
    SDL_RenderCopy(ren, e->texture, NULL, e->pos);
}

void print_entity(entity* e) {
    printf("Enemy {x : %d, y : %d, width : %d, height : %d}\n", e->pos->x, e->pos->y, e->pos->w, e->pos->h);
}