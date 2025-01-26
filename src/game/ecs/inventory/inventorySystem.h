#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "./inventoryComponent.h"

void initialize_inventory(InventoryComponent* invent, int maxItems, bool isDisplayed);
bool add_item_to_inventory(uint32_t entity, ItemData item);
bool remove_item_from_inventory(uint32_t entity, ItemData item);
void draw_inventory(uint32_t entity, SDL_Renderer* renderer);
