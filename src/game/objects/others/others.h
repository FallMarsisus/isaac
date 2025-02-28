#pragma once

#include "../../../display.h"

#include "../player/scripts.h"

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

#include "../../ecs/render/renderSystems.h"
#include "../../ecs/physics/physicsSystems.h"
#include "../../ecs/other/otherSystems.h"
#include "../../ecs/damager/damagerSystem.h"
#include "../../ecs/damager/damagerComponent.h"
#include "../../ecs/sword/swordComponent.h"

#include "../../event/eventSystem.h"

#include "../../../utils/pi.h"

/**
 * @brief Creates a block entity at specified coordinates with a texture
 * @param x X coordinate for block placement
 * @param y Y coordinate for block placement
 * @param texture SDL texture for the block
 * @return Entity ID of the created block
 */
uint32_t add_effect(float x, float y, float time, int width, int height, SDL_Texture* texture);
uint32_t add_projectile(float x, float y, float vx, float vy, float time, SDL_Texture* texture);
uint32_t add_flame(float x, float y, float vx, float vy);
uint32_t add_sword(uint32_t player, SwordComponent* sword, float offsetX, float offsetY);