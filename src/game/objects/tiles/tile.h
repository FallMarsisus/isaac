#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../anim.h"
#include "../../../utils/vector.h"

typedef struct tile_s{
    SDL_Rect* hitbox;

    sprite_list* sprites;
    anim_core* core;

    bool collide;

    void* other_variables;
    void (*free_tile_other)(struct tile_s* e);
    
    void (*update)(struct tile_s* e, void* pl);
    void (*draw)(struct tile_s* e, SDL_Renderer* ren);
} Tile;

Tile* create_tile(int x, int y, int width, int height, sprite_list* sprites);
void load_tile_texture(Tile* tile, SDL_Renderer* ren);
void free_tile(Tile* tile);

void update_tile(Tile* tile, void* pl);

void draw_tile(Tile* tile, SDL_Renderer* ren);