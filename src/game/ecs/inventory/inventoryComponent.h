#pragma once

#include <stdint.h>
#include "../items/itemData.h"
#include <stdbool.h>

typedef struct {
	bool isDisplayed;
	int max_nb_items;
	int nb_items;
	ItemData* items;
} InventoryComponent;
