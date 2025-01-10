#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "../../../utils/vector.h"
#include "../../../utils/chained_list.h"

#include "../tiles/tile.h"
#include "../../anim.h"
#include "../../../display.h"
#include "../../../utils/utils.h"

typedef struct entity_s {
    Vector* pos;
    SDL_Rect* hitbox;
    
    Vector* vel;
    int speed;
    
    sprite_list* sprites;
    anim_core* core;

    int maxhealth; int health;
    int damage;

    void* other_variables;
    void (*free_entity_other)(struct entity_s* e);
    
    void (*update)(struct entity_s* e, void* pl, chained_list* entities, chained_list* tiles);
    void (*draw)(struct entity_s* e, SDL_Renderer* ren);
} Entity;

Entity* create_entity(float x, float y, int width, int height, sprite_list* sprites);
void load_entity_texture(Entity* e, SDL_Renderer* ren);
void free_entity(Entity* e);

void set_entity_position(Entity* e, float x, float y);
void move_entity(Entity* e, float dx, float dy);

void update_entity(Entity* e, void* pl, chained_list* entities, chained_list* tiles);

void draw_entity(Entity* e, SDL_Renderer* ren);