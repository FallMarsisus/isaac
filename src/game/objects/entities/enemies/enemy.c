#include "enemy.h"

void load_enemy_textures(Entity* e, SDL_Renderer* ren) {
    e->core = create_core(ren, e->sprites->goblin_texture, 16, 16);
    if (e->core == NULL) {
        fprintf(stderr, "Failed to load enemy texture\n");
        return;
    }

    add_anim(e->core, 0, 0.1, 4);
    add_anim(e->core, 1, 0.1, 4);
    add_anim(e->core, 2, 0.1, 2);
    add_anim(e->core, 3, 0.1, 2);

    play_anim(e->core);
}

void free_enemy(Entity* e) {
    //
}

void update_enemy_sprite(Entity* e) {
    if(e->core == NULL) return;
    bool anim = true;
    if(e->vel->y < -0.5) set_active_anim(e->core, 1);
    else if(e->vel->y > 0.5) set_active_anim(e->core, 0);
    else if(e->vel->x < -0.5) set_active_anim(e->core, 2);
    else if(e->vel->x > 0.5) set_active_anim(e->core, 3);
    else {
        stop_anim(e->core);
        anim = false;
    }
    if(anim) play_anim(e->core);
}

void update_enemy(Entity* e, void* cc, chained_list* entities, chained_list* tiles) {
    player* pl = (player*) cc;
    float distanceX = pl->body->pos->x - e->pos->x;
    float distanceY = pl->body->pos->y - e->pos->y;

    if(sqrt(pow(distanceX, 2) + pow(distanceY, 2)) < 5) {
        e->vel->x = 0;
        e->vel->y = 0;
    } else {
        e->vel->x = distanceX;
        e->vel->y = distanceY;
        normalize(e->vel);
    }

    update_enemy_sprite(e);
}

void draw_enemy(Entity* e, SDL_Renderer* ren) {
    if (e->sprites != NULL && e->core == NULL) {
        load_enemy_textures(e, ren);
    }
    draw_entity(e, ren);
}

Entity* create_enemy(float x, float y, sprite_list* sprites) {
    Entity* e = create_entity(x, y, 32, 32, sprites);
    if (e == NULL) {
        fprintf(stderr, "Failed to create entity\n");
        return NULL;
    }
    e->free_entity_other = free_enemy;
    e->update = update_enemy;
    e->draw = draw_enemy;

    e->speed = 1;
    return e;
}