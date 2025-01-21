#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../itemData.h"

bool create_item(ECS_Manager* manager, uint32_t entity, ItemData itemC);
bool destroy_item(ECS_Manager* manager, uint32_t entity, ItemData item);

ItemData get_item_by_id(ECS_Manager* manager, uint32_t entity, int id);