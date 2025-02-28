#pragma once

#include "../eventSystem.h"
#include "../../stateMachine/stateMachine.h"
#include "../../ecs/ecs.h"
#include "../../ecs/items/itemSystem.h"
#include "../../ecs/damager/damagerComponent.h"
#include "../../ecs/health/healthComponent.h"
#include "../../ecs/physics/forceSystem.h"
#include "../../ecs/physics/physicsSystems.h"

#include <stdint.h>

void on_player_move(Event event);
void on_chest_open(Event event);

void on_state_change(Event event);

void on_collision(Event event);