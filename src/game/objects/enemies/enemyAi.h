#pragma once

#include "../../ecs/components.h"
#include "../../ecs/ecs.h"

#include "../../../utils/vector.h"

void init_enemy_ai(EnemyAiComponent* ai, uint32_t target);
void free_enemy_ai(uint32_t id);

void update_enemy_ai(uint32_t id);