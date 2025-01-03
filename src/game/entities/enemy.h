#pragma once

#include <SDL2/SDL.h>

#include "entities.h"
#include "../anim.h"
#include "../player.h"

#include "../../utils/chained_list.h"

typedef struct enemy_s {
    entity* body;

    anim_core* core;

    void (*update)(player* p, struct enemy_s* e, chained_list* entities);
    void (*draw_additional)(SDL_Renderer* ren, struct enemy_s* e);
} enemy;

enemy* create_enemy(float x, float y, int w, int h);
void load_enemy_textures(enemy* e, SDL_Renderer* ren, char* path);
void free_enemy(enemy* e);

void update_enemy(player* p, enemy* e, chained_list* entities);
void move_enemy(enemy* e, float dx, float dy);

void draw_enemy(SDL_Renderer* ren, enemy* e);