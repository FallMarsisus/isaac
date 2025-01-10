#include "entity.h"

Entity* create_entity(float x, float y, int width, int height, sprite_list* sprites) {
    Entity* e = malloc(sizeof(Entity));
    e->pos = malloc(sizeof(Vector)); e->pos->x = x; e->pos->y = y;
    e->hitbox = malloc(sizeof(SDL_Rect)); e->hitbox->x = x; e->hitbox->y = y; e->hitbox->w = width; e->hitbox->h = height;

    e->vel = malloc(sizeof(Vector)); e->vel->x = 0; e->vel->y = 0;
    e->speed = 0;
    e->sprites = sprites;
    e->core = NULL;

    e->maxhealth = -1; e->health = e->maxhealth; e->damage = 0;

    e->other_variables = NULL;
    e->free_additionnal = NULL;
    e->update = NULL;
    e->draw = NULL;
    return e;
}
void load_entity_texture(Entity* e, SDL_Renderer* ren) {
    if(ren == NULL || e->sprites == NULL || e->hitbox == NULL) return;
    e->core = create_core(ren, e->sprites->player_texture, e->hitbox->w, e->hitbox->h);
}
void free_entity(Entity* e) {
    free(e->pos);
    free(e->hitbox);
    free(e->vel);
    if(e->core != NULL) free_core(e->core);
    if(e->free_additionnal != NULL) e->free_additionnal(e);
    free(e);
}

void set_entity_position(Entity* e, float x, float y) {
    e->pos->x = x; e->pos->y = y;
    e->hitbox->x = x; e->hitbox->y = y;
}
void move_entity(Entity* e, float dx, float dy) {
    set_entity_position(e, e->pos->x + dx, e->pos->y + dy);
}

void update_entity(Entity* e, void* pl, chained_list* entities, chained_list* tiles) {
    if(e == NULL) return;
    if(e->update != NULL) e->update(e, pl, entities, tiles);

    if(fabs(e->vel->x) > 0.1 || fabs(e->vel->y) > 0.1) {
        normalize(e->vel);
        move_entity(e, e->vel->x * e->speed, e->vel->y * e->speed);
        if(tiles == NULL) return;
        for(cell* c = get_first(tiles); c != NULL; c = get_next(c)) {
            Tile* tile = get_data(c);
            if(tile == NULL) continue;
            if (!checkCollision(e->hitbox, tile->hitbox)) continue; // No collision to handle

            // Handle collision response
            float overlapX = (e->hitbox->x + e->hitbox->w / 2) - (tile->hitbox->x + tile->hitbox->w / 2);
            float overlapY = (e->hitbox->y + e->hitbox->h / 2) - (tile->hitbox->y + tile->hitbox->h / 2);

            if (fabs(overlapX) > fabs(overlapY)) {
                if (overlapX > 0) {
                    set_entity_position(e, tile->hitbox->x + tile->hitbox->w, e->pos->y);
                } else {
                    set_entity_position(e, tile->hitbox->x - e->hitbox->w, e->pos->y);
                }
            } else {
                if (overlapY > 0) {
                    set_entity_position(e, e->pos->x, tile->hitbox->y + tile->hitbox->h);
                } else {
                    set_entity_position(e, e->pos->x, tile->hitbox->y - e->hitbox->h);
                }
            }
        }
    }
}

void draw_entity(Entity* e, SDL_Renderer* ren) {
    if(e->sprites != NULL && e->core == NULL) {
        load_entity_texture(e, ren);
    }

    if(e->core != NULL) {
        draw_core(ren, e->hitbox, e->core);
    }
    else {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderFillRect(ren, e->hitbox);
    }
}