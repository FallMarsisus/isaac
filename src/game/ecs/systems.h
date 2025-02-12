#pragma once

#include "../../display.h"

#include "../../utils/utils.h"
#include "../../utils/mouse.h"

#include "../event/eventSystem.h"
#include "../event/events/listeners.h"

#include "../stateMachine/stateMachine.h"

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
* @brief Frees one entity 
* @param entity entity to be freed
*/
void free_one_entity(uint32_t entity);

/**
 * @brief Processes input events for the game systems
 * @param event SDL event to process
 */
void handle_input_system(SDL_Event* event);

/**
 * @brief Updates all systems for a single entity
 * @param elt Entity ID to update
 * @param grid Grid of the current room
 * @param entities Array of entity IDs in the current room
 * @param amount Number of entities in the array
 * @param roomPos The room Position
 * @param delta Time since last frame
 */
void update_elt(uint32_t elt, int** grid, uint32_t* entities, int amount, SDL_Rect roomPos, float delta);
/**
 * @brief Renders all visible entities using the game systems
 * @param entities Array of entity IDs to render
 * @param amount Number of entities in the array
 * @param cam Camera rectangle for viewport calculations
 * @param renderer SDL renderer to use for drawing
 */
void render_systems(uint32_t* entities, int amount, SDL_Rect cam, SDL_Renderer* renderer);
