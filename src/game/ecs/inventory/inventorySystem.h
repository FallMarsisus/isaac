#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../invertoryComponent.c"

bool add_item_to_inventory(ECS_Manager* manager, uint32_t entity, int item);
bool remove_item_from_inventory(ECS_Manager* manager, uint32_t entity, int item);