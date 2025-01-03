#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "entities.h"

typedef struct {
    entity* body;
} item;

item* create_item(int x, int y, int w, int h);
void free_item(item* i);

void update_item(item* i);
void draw_item(SDL_Renderer* ren, item* i);