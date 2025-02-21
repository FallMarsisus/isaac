#pragma once

#include "../../../display.h"

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

#include "../../ecs/render/renderSystems.h"
#include "../../ecs/physics/physicsSystems.h"
#include "../../ecs/other/otherSystems.h"
#include "../../ecs/pathFinding/pathfindingSystems.h"

#include "../../event/eventSystem.h"

#include "scripts.h"

uint32_t add_tile(float x, float y, int tile_x, int tile_y, SDL_Texture* tileset_texture, bool has_collision);

uint32_t add_background_tile(float x, float y);

uint32_t add_chest(float x, float y);
uint32_t add_door(float x, float y);
uint32_t add_trap(float x, float y);

/**
 * @brief Checks if an entity is colliding with a chest
 *
 * @param entity Entity ID to check for collision
 * @return true if the entity is colliding with a chest
 * @return false if the entity is not colliding with a chest
 */
bool is_colliding_with_chest(uint32_t entity);

/**
 * @brief Creates a teleporter entity linking two positions
 * @param x Source X coordinate
 * @param y Source Y coordinate
 * @param xTarget Destination X coordinate
 * @param yTarget Destination Y coordinate
 * @return Entity ID of the created teleporter
 */
uint32_t add_teleporter(float x, float y, float xTarget, float yTarget);