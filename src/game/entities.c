#include "entities.h"

struct entity_s {
    SDL_Rect* pos;
    SDL_Texture* texture;
};

typedef struct entity_s entity;

entity* create_entity(SDL_Rect* pos, SDL_Texture* tex) {
    entity* e = malloc(sizeof(entity));
    e->pos = pos;
    e->texture = tex;
    return e;
}

void free_entity(entity* e) {
    free(e);
}

void print_entity(entity* e) {
    printf("Enemy {x : %d, y : %d, width : %d, height : %d}\n", e->pos->x, e->pos->y, e->pos->w, e->pos->h);
}