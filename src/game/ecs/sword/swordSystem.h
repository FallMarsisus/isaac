#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../health/healthComponent.h"
#include "../health/healthSystem.h"
#include "swordComponent.h"
#include "../../objects/others/others.h"

bool create_sword(SwordComponent* sword, enum ItemID id, int damage, int range, int cooldown);
uint32_t use_sword(uint32_t entity, uint32_t enemy);

void free_sword_component(SwordComponent* sword);