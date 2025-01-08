#include "enemy.h"

Entity* create_enemy(float x, float y) {
    Entity* e = create_entity(x, y, 32, 32, "assets/player/sprite_sheet.bmp");
    e->free_additionnal = free_additional;
    e->update = update_enemy;
    e->draw_additional = draw_enemy;

    e->speed = 1;
    return e;
}
void free_additional(Entity* e) {
    //
}

void update_enemy(Entity* e, void* cc, chained_list* entities, chained_list* tiles) {
    player* pl = (player*) cc;
    e->vel->x = pl->body->pos->x - e->pos->x;
    e->vel->y = pl->body->pos->y - e->pos->y;

    if(fabs(e->vel->x) < 0.1 && fabs(e->vel->y) < 0.1) {
        e->vel->x = 0;
        e->vel->y = 0;
    }
    else {
        normalize(e->vel);
    }
}
void draw_enemy(SDL_Renderer* ren, Entity* e) {
    //
}