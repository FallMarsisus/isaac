#include "entities.h"

entity* create_entity(float x, float y, int width, int height) {
    entity* e = malloc(sizeof(entity));

    e->vel = malloc(sizeof(Vector));
    e->vel->x = 0; e->vel->y = 0;
    e->pos = malloc(sizeof(Vector));
    e->pos->x = x; e->pos->y = y;

    e->hitbox = malloc(sizeof(SDL_Rect));
    e->hitbox->x = x; e->hitbox->y = y; e->hitbox->w = width; e->hitbox->h = height;

    e->speed = 0;

    e->maxhealth = 100; e->health = 100;
    e->has_sprite = false;
    return e;
}

void free_entity(entity* e) {
    free(e);
}

void print_entity(entity* e) {
    printf("Entity {x : %f, y : %f, width : %d, height : %d}\n", e->pos->x, e->pos->y, e->hitbox->w, e->hitbox->h);
}

void set_position(entity* e, float x, float y) {
    e->pos->x = x;
    e->pos->y = y;
    e->hitbox->x = x;
    e->hitbox->y = y;
}

void update_entity_collisions(entity* e1, entity* e2) {
    if (e1->hitbox->x < e2->hitbox->x + e2->hitbox->w &&
        e1->hitbox->x + e1->hitbox->w > e2->hitbox->x &&
        e1->hitbox->y < e2->hitbox->y + e2->hitbox->h &&
        e1->hitbox->y + e1->hitbox->h > e2->hitbox->y) {
            // Handle collision response
            float overlapX = (e1->hitbox->x + e1->hitbox->w / 2) - (e2->hitbox->x + e2->hitbox->w / 2);
            float overlapY = (e1->hitbox->y + e1->hitbox->h / 2) - (e2->hitbox->y + e2->hitbox->h / 2);

            if (fabs(overlapX) > fabs(overlapY)) {
                if (overlapX > 0) {
                    e1->pos->x = e2->hitbox->x + e2->hitbox->w;
                } else {
                    e1->pos->x = e2->hitbox->x - e1->hitbox->w;
                }
            } else {
                if (overlapY > 0) {
                    e1->pos->y = e2->hitbox->y + e2->hitbox->h;
                } else {
                    e1->pos->y = e2->hitbox->y - e1->hitbox->h;
                }
            }

            // Update hitbox position
            e1->hitbox->x = e1->pos->x;
            e1->hitbox->y = e1->pos->y;
    }
}

void update_entity(entity* e) {
    if(e == NULL || e->pos == NULL) return;
    if(e->vel == NULL) return;

    if(fabs(e->vel->x) < 0.1 && fabs(e->vel->y) < 0.1) {
        e->vel->x = 0;
        e->vel->y = 0;
    }
    else {
        normalize(e->vel);
    }
    set_position(e, e->pos->x + e->vel->x * e->speed, e->pos->y + e->vel->y * e->speed);
}

void draw_entity(SDL_Renderer* ren, entity* e) {
    if(e == NULL || e->hitbox == NULL) return;
    if(e->has_sprite) {
        SDL_RenderCopy(ren, e->texture, NULL, e->hitbox);
    }
    else {
        SDL_SetRenderDrawColor(ren , 0, 0, 255, 255 );
        SDL_RenderDrawRect(ren, e->hitbox);
    }
}
