#pragma once

#include "../components.h"
#include "../ecs.h"

#include "../inventory/inventorySystem.h"
#include "../health/healthSystem.h"
#include "../items/itemSystem.h"
#include "../damager/damagerSystem.h"
#include "../sword/swordSystem.h"
#include "../physics/physicsSystems.h"

#include "../../objects/enemies/enemies.h"

#include "../../event/eventList.h"
#include "../../event/eventSystem.h"

void init_player_component(PlayerMovementComponent* player);

void update_player(uint32_t id);