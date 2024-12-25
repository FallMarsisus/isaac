#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <assert.h>

typedef struct entity_s entity;

entity* create_entity(int x, int y, int width, int height);
void free_entity(entity* e);

SDL_Rect* get_pos(entity* e);
void print_entity(entity* e);

void set_pos(int x, int y, entity* e);
void set_sprite(entity* e, SDL_Texture* tex);

void draw_entity(SDL_Renderer* ren, entity* e);


#endif