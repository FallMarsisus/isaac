#pragma once

#include "../../../display.h"

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

#include "../../ecs/render/renderSystems.h"
#include "../../ecs/physics/physicsSystems.h"
#include "../../ecs/other/otherSystems.h"
#include "../../ecs/health/healthSystem.h"
#include "../../ecs/damager/damagerSystem.h"

#include "../../event/eventSystem.h"

#include "enemyStates.h"

/**
 * @brief Creates an enemy entity at specified coordinates
 * @param x X coordinate for enemy spawn
 * @param y Y coordinate for enemy spawn
 * @param player Player entity ID that the enemy will target
 * @return Entity ID of the created enemy
 */
uint32_t add_enemy(float x, float y, uint32_t player);

/**
 * @brief Checks if an entity is colliding with any enemy
 * @param entity Entity ID to check collisions for
 * @return true if collision detected, false otherwise
 */
bool is_colliding_with_enemy(uint32_t entity);

/**
 * @brief Finds the nearest enemy to a given entity
 * @param entity Entity ID to find nearest enemy to
 * @return Entity ID of the nearest enemy
 */
uint32_t get_nearest_enemy(uint32_t entity);