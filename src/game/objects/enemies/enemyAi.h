#pragma once

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"
#include "../../ecs/physics/physicsSystems.h"

#include "../../../utils/vector.h"

void init_enemy_ai_component(EnemyAiComponent* ai, uint32_t target);
void free_enemy_ai(uint32_t id);

void update_enemy_ai(uint32_t id, uint32_t* entities, int amount);