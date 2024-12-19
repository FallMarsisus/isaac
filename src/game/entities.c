#include <SDL2/SDL.h>

#include "entities.h"

struct entity_s{
    int x;
    int y;
    int width;
    int height;

    SDL_Texture* texture;
};

typedef struct entity_s entity;

struct cell_entity_s {
    struct cell_entity* next;
    entity* value;
};

typedef struct cell_entity_s cell_entity;

