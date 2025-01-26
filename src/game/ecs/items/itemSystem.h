#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "itemData.h"
#include "itemComponent.h"

bool transfer_item_into_inventory(uint32_t itemEntity, uint32_t targetEntity);
SDL_Texture* get_texture_from_Id(enum ItemID id);