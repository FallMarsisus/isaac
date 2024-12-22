#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <SDL2/SDL.h>

typedef struct entity_s entity;

entity* create_entity(SDL_Rect* pos, SDL_Texture* tex);
void free_entity(entity* e);

void print_entity(entity* e);

#endif