#pragma once

#include <SDL2/SDL.h>

#include "entities.h"
#include "../anim.h"
#include "../player.h"

typedef struct enemy_s enemy;

enemy* create_enemy(float x, float y, int w, int h);
void load_enemy_textures(enemy* e, SDL_Renderer* ren, char* path);
void free_enemy(enemy* e);

SDL_Rect* get_enemy_pos(enemy* e);

void set_update(enemy* e, void (*update)(player* p, enemy* e));
void set_draw_additional(enemy* e, void (*draw_additional)(SDL_Renderer* ren, enemy* e));

void update_enemy(player* p, enemy* e);
void move_enemy(enemy* e, float dx, float dy);

void draw_enemy(SDL_Renderer* ren, enemy* e);