#pragma once

#include "../../display.h"

#include "../../utils/utils.h"

#include "render/renderSystems.h"
#include "physics/physicsSystems.h"
#include "other/otherSystems.h"
#include "inventory/inventorySystem.h"
#include "health/healthSystem.h"

#include "../scripts/teleporters/teleport.h"

#include "components.h"
#include "ecs.h"

#include <SDL2/SDL.h>

uint32_t initialize_game(ECS_Manager* ecs);
void free_components(ECS_Manager* ecs);

uint32_t add_player(ECS_Manager* ecs, float x, float y);
uint32_t add_enemy(ECS_Manager* ecs, float x, float y, uint32_t player);
uint32_t add_block(ECS_Manager* ecs, float x, float y);
uint32_t add_teleporter(ECS_Manager* ecs, float x, float y, float xTarget, float yTarget);

void handle_input_system(ECS_Manager* ecs, SDL_Event* event);
void update_systems(ECS_Manager* ecs, SDL_Rect cam);
void render_systems(ECS_Manager* ecs, SDL_Rect cam, SDL_Renderer* renderer);