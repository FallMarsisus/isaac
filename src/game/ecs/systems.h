#pragma once

#include "../../display.h"

#include "ecs.h"
#include <SDL2/SDL.h>

void initialize_game(ECS_Manager* ecs);

uint32_t add_player(ECS_Manager* ecs);
uint32_t add_enemy(ECS_Manager* ecs, uint32_t player);

void handle_input_system(ECS_Manager* ecs, SDL_Event* event);
void update_systems(ECS_Manager* ecs);
void render_systems(ECS_Manager* ecs, SDL_Renderer* renderer);