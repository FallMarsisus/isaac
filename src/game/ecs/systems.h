#pragma once

#include "../../display.h"

#include "render/renderSystems.h"
#include "physics/physicsSystems.h"

#include "components.h"
#include "ecs.h"

#include <SDL2/SDL.h>

void initialize_game(ECS_Manager* ecs);
void free_components(ECS_Manager* ecs);

uint32_t add_player(ECS_Manager* ecs, float x, float y);
uint32_t add_enemy(ECS_Manager* ecs, float x, float y, uint32_t player);
uint32_t add_block(ECS_Manager* ecs, float x, float y);

void handle_input_system(ECS_Manager* ecs, SDL_Event* event);
void update_systems(ECS_Manager* ecs);
void render_systems(ECS_Manager* ecs, SDL_Renderer* renderer);