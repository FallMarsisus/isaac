#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../itemComponent.h"

bool create_item(ECS_Manager* manager, uint32_t entity, ItemComponent itemC);
bool destroy_item(ECS_Manager* manager, uint32_t entity, ItemComponent item);

ItemComponent get_item_by_id(ECS_Manager* manager, uint32_t entity, int id);