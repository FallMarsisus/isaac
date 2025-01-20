#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../inventoryComponent.h"

bool add_item_to_inventory(ECS_Manager* manager, uint32_t entity, int item);
bool remove_item_from_inventory(ECS_Manager* manager, uint32_t entity, int item);
void draw_inventory(ECS_Manager* manager, uint32_t entity, SDL_Renderer* renderer);
