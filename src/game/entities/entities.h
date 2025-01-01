#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <assert.h>

typedef struct entity_s entity;

entity* create_entity(int x, int y, int width, int height);
void free_entity(entity* e);

SDL_Rect* get_pos(entity* e);
int get_max_health(entity* e);
int get_health(entity* e);
int get_damage(entity* e);

void print_entity(entity* e);

void set_pos(entity* e, int x, int y);
void set_sprite(entity* e, SDL_Texture* tex);
void set_max_health(entity* e, int max_health);
void set_health(entity* e, int health);
void set_damage(entity* e, int damage);

void draw_entity(SDL_Renderer* ren, entity* e);