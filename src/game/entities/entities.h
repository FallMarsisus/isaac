#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <assert.h>

#include "../../utils/vector.h"

typedef struct {
    Vector* pos;
    Vector* vel;
    int speed;

    bool has_sprite;
    SDL_Texture* texture;
    SDL_Rect* hitbox;

    int maxhealth; int health;
    int damage;
} entity;

entity* create_entity(float x, float y, int width, int height);
void free_entity(entity* e);

void print_entity(entity* e);

void set_position(entity* e, float x, float y);

void update_entity_collisions(entity* e1, entity* e2);

void update_entity(entity* e);

void draw_entity(SDL_Renderer* ren, entity* e);