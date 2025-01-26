#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "itemData.h"

bool create_item(uint32_t entity, ItemData itemC);
bool destroy_item(uint32_t entity, ItemData item);

ItemData get_item_by_id(uint32_t entity, int id);