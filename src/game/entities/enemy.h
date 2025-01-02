#pragma once

#include <SDL2/SDL.h>

#include "entities.h"
#include "../anim.h"

typedef struct enemy_s enemy;

enemy* create_enemy(int x, int y, int w, int h);
void load_enemy_textures(enemy* e, SDL_Renderer* ren, char* path);
void free_enemy(enemy* e);

void set_update(enemy* e, void (*update)(enemy* e));
void set_draw_additional(enemy* e, void (*draw_additional)(SDL_Renderer* ren, enemy* e));

void update_enemy(enemy* e);
void move_enemy(enemy* e, int dx, int dy);

void draw_enemy(SDL_Renderer* ren, enemy* e);