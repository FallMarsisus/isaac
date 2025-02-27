#pragma once

#include <SDL2/SDL.h>
#include "../game/game.h"

void init_menu_manager(SDL_Window* win, SDL_Renderer* renderer);
void free_menu_manager();

void update_menu_manager(int win_width, int win_height, float delta);

void draw_menu_manager(SDL_Renderer* renderer, int win_width, int win_height, int true_width, int true_height);

void handle_menu_manager_input(SDL_Event event);
void reset_game();