#pragma once

#include "../../display.h"

#include "../../utils/utils.h"
#include "../../utils/dyn_arrays.h"

#include "../event/eventSystem.h"
#include "../event/events/playerListeners.h"

#include "render/renderSystems.h"
#include "physics/physicsSystems.h"
#include "other/otherSystems.h"
#include "pathFinding/pathfindingSystems.h"
#include "inventory/inventorySystem.h"
#include "health/healthSystem.h"
#include "items/itemSystem.h"
#include "damager/damagerSystem.h"
#include "../scripts/teleporters/teleport.h"

#include "components.h"
#include "items/itemList.h"
#include "ecs.h"

#include <SDL2/SDL.h>

/**
 * @brief Initializes the game systems and components
 * @param ecs Pointer to the ECS manager
 * @return ID of the initialized game state
 */
uint32_t initialize_game(ECS_Manager* ecs);

/**
 * @brief Frees all components from the ECS manager
 * @param ecs Pointer to the ECS manager
 */
void free_components(ECS_Manager* ecs);

/**
 * @brief Checks if an entity is colliding with any enemy
 * @param ecs Pointer to the ECS manager
 * @param entity Entity ID to check collisions for
 * @return true if collision detected, false otherwise
 */
bool is_colliding_with_enemy(ECS_Manager* ecs, uint32_t entity);

/**
 * @brief Checks if an entity is colliding with a chest
 *
 * @param ecs Pointer to the ECS Manager instance
 * @param entity Entity ID to check for collision
 * @return true if the entity is colliding with a chest
 * @return false if the entity is not colliding with a chest
 */

bool is_colliding_with_chest(ECS_Manager* ecs, uint32_t entity);

/**
 * @brief Finds the nearest enemy to a given entity
 * @param ecs Pointer to the ECS manager
 * @param entity Entity ID to find nearest enemy to
 * @return Entity ID of the nearest enemy
 */
uint32_t get_nearest_enemy(ECS_Manager* ecs, uint32_t entity);

/**
 * @brief Creates a player entity at specified coordinates
 * @param ecs Pointer to the ECS manager
 * @param x X coordinate for player spawn
 * @param y Y coordinate for player spawn
 * @return Entity ID of the created player
 */
uint32_t add_player(ECS_Manager* ecs, float x, float y);

/**
 * @brief Creates an enemy entity at specified coordinates
 * @param ecs Pointer to the ECS manager
 * @param x X coordinate for enemy spawn
 * @param y Y coordinate for enemy spawn
 * @param player Player entity ID that the enemy will target
 * @return Entity ID of the created enemy
 */
uint32_t add_enemy(ECS_Manager* ecs, float x, float y, uint32_t player);

/**
 * @brief Creates a block entity at specified coordinates with a texture
 * @param ecs Pointer to the ECS manager
 * @param x X coordinate for block placement
 * @param y Y coordinate for block placement
 * @param texture SDL texture for the block
 * @return Entity ID of the created block
 */
uint32_t add_block(ECS_Manager* ecs, float x, float y, SDL_Texture* texture);

/**
 * @brief Creates a chest at specified coordinates with a closed_chest texture
 * @param ecs Pointer to the ECS manager
 * @param x X coordinate for block placement
 * @param y Y coordinate for block placement
 * @return Entity ID of the created block
 */
uint32_t add_chest(ECS_Manager* ecs, float x, float y);
/**
 * @brief Creates multiple blocks in a rectangular area
 * @param ecs Pointer to the ECS manager
 * @param rX Width of the rectangular area
 * @param rY Height of the rectangular area
 * @return Entity ID of the last created block
 */
uint32_t add_blocks(ECS_Manager* ecs, int rX, int rY);

/**
 * @brief Creates a teleporter entity linking two positions
 * @param ecs Pointer to the ECS manager
 * @param x Source X coordinate
 * @param y Source Y coordinate
 * @param xTarget Destination X coordinate
 * @param yTarget Destination Y coordinate
 * @return Entity ID of the created teleporter
 */
uint32_t add_teleporter(ECS_Manager* ecs, float x, float y, float xTarget, float yTarget);

/**
 * @brief Processes input events for the game systems
 * @param ecs Pointer to the ECS manager
 * @param event SDL event to process
 */
void handle_input_system(ECS_Manager* ecs, SDL_Event* event);

/**
 * @brief Updates all game systems for the specified entities
 * @param ecs Pointer to the ECS manager
 * @param entities Array of entity IDs to update
 * @param amount Number of entities in the array
 * @param grid Game world grid
 * @param cam Camera rectangle for viewport calculations
 */
void update_systems(ECS_Manager* ecs, uint32_t* entities, int amount, int** grid, SDL_Rect cam);

/**
 * @brief Renders all visible entities using the game systems
 * @param ecs Pointer to the ECS manager
 * @param entities Array of entity IDs to render
 * @param amount Number of entities in the array
 * @param cam Camera rectangle for viewport calculations
 * @param renderer SDL renderer to use for drawing
 */
void render_systems(ECS_Manager* ecs, uint32_t* entities, int amount, SDL_Rect cam, SDL_Renderer* renderer);
