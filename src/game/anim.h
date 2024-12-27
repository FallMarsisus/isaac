#pragma once

#include "../utils/dyn_arrays.h"
#include "entities.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct {
    float interval;
    int amount;
    int line;
} anim;

typedef struct anim_core_s anim_core;

anim_core* create_core(SDL_Renderer* ren, char* path, int sprite_width, int sprite_height);
void free_core(anim_core* core);

//Returns the index of the animation
int add_anim(anim_core* core, int line, float interval, int amount);

void set_active_anim(anim_core* core, int index);
void play_anim(anim_core* core);
void pause_anim(anim_core* core);
void stop_anim(anim_core* core);

void draw_texture(SDL_Renderer* ren, SDL_Rect* pos, anim_core* core);