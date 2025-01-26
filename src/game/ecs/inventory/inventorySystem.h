#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "inventoryComponent.h"

bool add_item_to_inventory(uint32_t entity, int item);
bool remove_item_from_inventory(uint32_t entity, int item);
void draw_inventory(uint32_t entity, SDL_Renderer* renderer);
