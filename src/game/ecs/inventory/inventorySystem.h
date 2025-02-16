#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "./inventoryComponent.h"
#include "../items/itemsFunctions.h"

void initialize_inventory(InventoryComponent* invent, int maxItems, bool isDisplayed);
void free_inventory(InventoryComponent* invent);

bool add_item_to_inventory(uint32_t entity, ItemData item);
bool remove_item_from_inventory(uint32_t entity, ItemData item);

bool mouse_in_any_slot(uint32_t entity, int x, int y);
int get_slot_of_mouse(uint32_t entity, int x, int y);
int onClic(uint32_t entity, int x, int y);

void draw_inventory(uint32_t entity, SDL_Renderer* renderer, int win_width, int win_height, int true_width , int true_height);
