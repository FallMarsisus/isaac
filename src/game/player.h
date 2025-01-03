#pragma once

#include "../utils/vector.h"
#include "anim.h"
#include "entities/entities.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    bool* keys; //Keys pressed
    
    entity* body;

    anim_core* core;
} player;

player* create_player(int x, int y);
void load_player_textures(player* p, SDL_Renderer* ren, char* path);
void free_player(player* p);

void get_inputs(player* p);

void update_player(player* p, int width, int height);

void draw_player(SDL_Renderer* ren, player* p);