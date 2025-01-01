#include "entities.h"

struct entity_s {
    SDL_Rect* pos;

    bool has_sprite;
    SDL_Texture* texture;

    int maxhealth; int health;
    int damage;
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
int get_max_health(entity* e) {
    return e->maxhealth;
}
int get_health(entity* e) {
    return e->health;
}
int get_damage(entity* e) {
    return e->damage;
}

void print_entity(entity* e) {
    printf("Entity {x : %d, y : %d, width : %d, height : %d}\n", e->pos->x, e->pos->y, e->pos->w, e->pos->h);
}

void set_pos(entity* e, int x, int y) {
    e->pos->x = x;
    e->pos->y = y;
}
void set_sprite(entity* e, SDL_Texture* tex) {
    assert(tex != NULL);
    e->has_sprite = true;
    e->texture = tex;
}
void set_max_health(entity* e, int max_health) {
    e->maxhealth = max_health;
}
void set_health(entity* e, int health) {
    e->health = health;
}
void set_damage(entity* e, int damage) {
    e->damage = damage;
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
