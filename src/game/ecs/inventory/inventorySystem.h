#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "./inventoryComponent.h"
#include "../items/itemsFunctions.h"


// valeurs pour l'affichage de l'inventaire
#define NB_ROWS 5
#define SLOT_SIZE 32
#define MARGIN 5
#define SPACING 10
#define NB_CST_SLOTS 2

void initialize_inventory(InventoryComponent* invent, int maxItems, bool isDisplayed);
void free_inventory(InventoryComponent* invent);

bool add_item_to_inventory(uint32_t entity, ItemData item);
bool remove_item_from_inventory(uint32_t entity, ItemData item);
bool remove_item_index_invent(uint32_t entity, int i);
bool swap_items_inventory(uint32_t entity, int i, int j);