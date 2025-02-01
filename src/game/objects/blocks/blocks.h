#pragma once

#include "../../../display.h"

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

#include "../../ecs/render/renderSystems.h"
#include "../../ecs/physics/physicsSystems.h"
#include "../../ecs/other/otherSystems.h"
#include "../../ecs/pathFinding/pathfindingSystems.h"

#include "../../event/eventSystem.h"

#include "../../scripts/teleporters/teleport.h"

/**
 * @brief Creates a block entity at specified coordinates with a texture
 * @param x X coordinate for block placement
 * @param y Y coordinate for block placement
 * @param texture SDL texture for the block
 * @return Entity ID of the created block
 */
uint32_t add_block(float x, float y, SDL_Texture* texture);

/**
 * @brief Creates a background tile entity at specified coordinates
 * @param x X coordinate for block placement
 * @param y Y coordinate for block placement
 * @return Entity ID of the created block
 */
uint32_t add_background_tile(float x, float y);

/**
 * @brief Creates a chest at specified coordinates with a closed_chest texture
 * @param x X coordinate for block placement
 * @param y Y coordinate for block placement
 * @return Entity ID of the created block
 */
uint32_t add_chest(float x, float y);

/**
 * @brief Checks if an entity is colliding with a chest
 *
 * @param entity Entity ID to check for collision
 * @return true if the entity is colliding with a chest
 * @return false if the entity is not colliding with a chest
 */

bool is_colliding_with_item(uint32_t entity);

bool is_colliding_with_chest(uint32_t entity);

/**
 * @brief Creates multiple blocks in a rectangular area
 * @param rX Width of the rectangular area
 * @param rY Height of the rectangular area
 * @return Entity ID of the last created block
 */
uint32_t add_blocks(int rX, int rY);

/**
 * @brief Creates a teleporter entity linking two positions
 * @param x Source X coordinate
 * @param y Source Y coordinate
 * @param xTarget Destination X coordinate
 * @param yTarget Destination Y coordinate
 * @return Entity ID of the created teleporter
 */
uint32_t add_teleporter(float x, float y, float xTarget, float yTarget);