#pragma once

#include "../../display.h"

#include "../../utils/utils.h"

#include "../event/eventSystem.h"
#include "../event/events/listeners.h"

#include "render/renderSystems.h"
#include "physics/physicsSystems.h"
#include "other/otherSystems.h"
#include "pathFinding/pathfindingSystems.h"
#include "inventory/inventorySystem.h"
#include "health/healthSystem.h"
#include "items/itemSystem.h"
#include "damager/damagerSystem.h"

#include "../objects/enemies/enemies.h"
#include "../objects/player/player.h"
#include "../objects/blocks/blocks.h"

#include "components.h"
#include "ecs.h"

#include "items/itemList.h"

#include <SDL2/SDL.h>

/**
 * @brief Initializes the game systems and components
 * @return ID of the initialized game state
 */
uint32_t initialize_game();

/**
 * @brief Frees all components from the ECS_GetManager() manager
 */
void free_components();

/**
 * @brief Processes input events for the game systems
 * @param event SDL event to process
 */
void handle_input_system(SDL_Event* event);

/**
 * @brief Updates all game systems for the specified entities
 * @param entities Array of entity IDs to update
 * @param amount Number of entities in the array
 * @param grid Game world grid
 * @param cam Camera rectangle for viewport calculations
 */
void update_systems(uint32_t* entities, int amount, int** grid, SDL_Rect cam);

/**
 * @brief Renders all visible entities using the game systems
 * @param entities Array of entity IDs to render
 * @param amount Number of entities in the array
 * @param cam Camera rectangle for viewport calculations
 * @param renderer SDL renderer to use for drawing
 */
void render_systems(uint32_t* entities, int amount, SDL_Rect cam, SDL_Renderer* renderer);
