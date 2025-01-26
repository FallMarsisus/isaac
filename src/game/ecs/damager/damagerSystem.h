#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../damagerComponent.h"
#include "../healthComponent.h"
#include "../health/healthSystem.h"
#include "../components.h"

bool create_damager(uint32_t entity, DamagerComponent damagerC);
bool destroy_damager(uint32_t entity, DamagerComponent damager);

bool apply_damage(uint32_t entity, uint32_t target);