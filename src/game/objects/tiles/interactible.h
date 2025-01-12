#pragma once

#include <stdbool.h>

#include "tile.h"
#include "../entities/player/player.h"
#include "../../../utils/utils.h"
#include "../../../utils/dyn_arrays.h"

typedef struct interactible_core_s {
    SDL_Scancode key;
    bool hovered;
    
    anim_core* core;
    SDL_Rect* icon_draw_box;

    void* other_var;
    void (*free_other_var)(struct interactible_core_s* e);
    
    void (*on_interact)(Tile* tile, void* pl);
} interactible_core;

#define get_interact_core(tile) ((interactible_core*) tile->other_variables)

Tile* create_interactible_tile(int posx, int posy, int width, int height, sprite_list* sprites);
void load_interactive_textures(Tile* tile, SDL_Renderer* ren);
void free_interactive_tile(Tile* tile);

void update_interactive_tile(Tile* tile, void* pl);

void draw_interactive_tile(Tile* tile, SDL_Renderer* ren);