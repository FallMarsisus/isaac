#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "../../../utils/vector.h"
#include "../../../utils/chained_list.h"

#include "../../anim.h"

typedef enum {
    ENEMY,
    NPC,
    ENTITY_SIMPLE
} entity_type ;

typedef struct {
    void* data;
    entity_type type;
} entity_container;

typedef struct entity_s {
    Vector* pos;
    SDL_Rect* hitbox;
    
    Vector* vel;
    int speed;

    char* texture_path;
    anim_core* core;

    int maxhealth; int health;
    int damage;

    void* other_variables;
    void (*free_additionnal)(struct entity_s* e);

    void (*update)(struct entity_s* e, void* pl, chained_list* entities, chained_list* tiles);
    void (*draw_additional)(SDL_Renderer* ren, struct entity_s* e);
} Entity;

Entity* create_entity(float x, float y, int width, int height, char* tex);
void load_entity_texture(Entity* e, SDL_Renderer* ren);
void free_entity(Entity* e);

void set_entity_position(Entity* e, float x, float y);
void move_entity(Entity* e, float dx, float dy);

void update_entity(Entity* e, void* pl, chained_list* entities, chained_list* tiles);

void draw_entity(Entity* e, SDL_Renderer* ren);