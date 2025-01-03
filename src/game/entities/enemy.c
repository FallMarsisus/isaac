#include "enemy.h"

enemy* create_enemy(float x, float y, int w, int h) {
    enemy* e = malloc(sizeof(enemy));
    e->body = create_entity(x, y, w, h);

    e->update = NULL;
    e->draw_additional = NULL;
    e->core = NULL;
    return e;
}
void load_enemy_textures(enemy* e, SDL_Renderer* ren, char* path) {
    e->core = create_core(ren, path, 16, 16);
    add_anim(e->core, 0, 0.1, 4);
    set_active_anim(e->core, 0);
}
void free_enemy(enemy* e) {
    free_entity(e->body);
    free(e);
}

void update_enemy(player* p, enemy* e, chained_list* entities) {
    update_entity(e->body);
    if(e->update != NULL) e->update(p, e, entities);
    update_entity_collisions(p->body, e->body);
}
void move_enemy(enemy* e, float dx, float dy) {
    set_position(e->body, e->body->pos->x + dx, e->body->pos->y + dy);
}

void draw_enemy(SDL_Renderer* ren, enemy* e) {
    if(e->draw_additional != NULL) e->draw_additional(ren, e);

    if(e->core != NULL) draw_core(ren, e->body->hitbox, e->core);
    else draw_entity(ren, e->body);
}