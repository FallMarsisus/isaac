#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "../../anim.h"
#include "../../../utils/vector.h"

typedef struct {
    SDL_Rect* hitbox;

    sprite_list* sprites;
    anim_core* core;
} Tile;

Tile* create_tile(int x, int y, int width, int height, sprite_list* sprites);
void load_tile_texture(Tile* tile, SDL_Renderer* ren);
void free_tile(Tile* tile);

void draw_tile(Tile* tile, SDL_Renderer* ren);