#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../health/healthComponent.h"
#include "../health/healthSystem.h"
#include "swordComponent.h"

bool create_sword(uint32_t entity, enum ItemID id, int damage, int range, int cooldown);
bool use_sword(uint32_t entity, uint32_t enemy);

void free_sword_component(SwordComponent* sword);