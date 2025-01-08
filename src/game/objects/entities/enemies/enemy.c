#include "enemy.h"

Entity* create_enemy(float x, float y) {
    Entity* e = create_entity(x, y, 32, 32, "assets/alien/sprite_sheet.bmp");
    if (e == NULL) {
        fprintf(stderr, "Failed to create entity\n");
        return NULL;
    }
    e->free_additionnal = free_additional;
    e->update = update_enemy;
    e->draw = draw_enemy;

    e->speed = 1;
    return e;
}

void load_enemy_textures(Entity* e, SDL_Renderer* ren) {
    e->core = create_core(ren, e->texture_path, 16, 16);
    if (e->core == NULL) {
        fprintf(stderr, "Failed to load enemy texture\n");
        return;
    }

    add_anim(e->core, 0, 0.1, 4);
    add_anim(e->core, 1, 0.1, 4);
    add_anim(e->core, 2, 0.1, 2);
    add_anim(e->core, 3, 0.1, 2);

    set_active_anim(e->core, 0);
    play_anim(e->core);
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
    } else {
        normalize(e->vel);
    }
}

void draw_enemy(Entity* e, SDL_Renderer* ren) {
    if (e->texture_path != NULL && e->core == NULL) {
        load_enemy_textures(e, ren);
    }
    draw_entity(e, ren);
}