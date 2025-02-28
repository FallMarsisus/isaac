#pragma once

#include <SDL2/SDL.h>

#include "../game/game.h"
#include "menuComponent.h"

void init_menu_manager(SDL_Window* win, SDL_Renderer* renderer);
void free_menu_manager();

void update_menu_manager(float delta);

void draw_menu_manager(SDL_Renderer* renderer, int true_width, int true_height);

void handle_menu_manager_input(SDL_Event event);
void switch_to_menu(MenuType menuType);

void on_game_over(Event event);