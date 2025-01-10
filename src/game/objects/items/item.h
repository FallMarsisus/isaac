#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "../../../utils/vector.h"
#include "../../anim.h"

typedef struct {
    Vector* pos;
    SDL_Rect* hitbox;

    anim_core* core;
    sprite_list* sprites;
} Item;

Item* create_item(float x, float y, float w, float h, sprite_list* sprites);
void load_item_textures(Item* item, SDL_Renderer* ren);
void free_item(Item* item);

void draw_item(Item* item, SDL_Renderer* ren);