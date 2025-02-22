#pragma once

#include "../../../display.h"

#include "../../ecs/components.h"

#include "../../ecs/ecs.h"

#include "../../ecs/render/renderSystems.h"
#include "../../ecs/physics/physicsSystems.h"
#include "../../ecs/other/otherSystems.h"
#include "../../ecs/inventory/inventorySystem.h"
#include "../../ecs/inventory/inventoryUI.h"
#include "../../ecs/health/healthSystem.h"
#include "../../ecs/items/itemSystem.h"
#include "../../ecs/damager/damagerSystem.h"
#include "../../ecs/sword/swordSystem.h"

#include "../../ecs/items/itemList.h"

#include "../../event/eventSystem.h"

#include "../blocks/blocks.h"

#include "scripts.h"

/**
 * @brief Creates a player entity at specified coordinates
 * @param x X coordinate for player spawn
 * @param y Y coordinate for player spawn
 * @return Entity ID of the created player
 */
uint32_t add_player(float x, float y);