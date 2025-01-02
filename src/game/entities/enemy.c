#include "enemy.h"

typedef struct enemy_s {
    entity* body;

    anim_core* core;

    void (*update)(struct enemy_s* e);
    void (*draw_additional)(SDL_Renderer* ren, struct enemy_s* e);
} enemy;

enemy* create_enemy(int x, int y, int w, int h) {
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

void set_update(enemy* e, void (*update)(enemy* e)) {
    e->update = update;
}
void set_draw_additional(enemy* e, void (*draw_additional)(SDL_Renderer* ren, enemy* e)) {
    e->draw_additional = draw_additional;
}

void update_enemy(enemy* e) {
    update_entity(e->body);
    if(e->update != NULL) e->update(e);
}
void move_enemy(enemy* e, int dx, int dy) {
    set_pos(e->body, get_pos(e->body)->x + dx, get_pos(e->body)->y + dy);
}

void draw_enemy(SDL_Renderer* ren, enemy* e) {
    if(e->draw_additional != NULL) e->draw_additional(ren, e);

    if(e->core != NULL) draw_core(ren, get_pos(e->body), e->core);
    else draw_entity(ren, e->body);
}