#pragma once

#include <stdint.h>
#include "itemComponent.h"
#include <stdbool.h>

typedef struct {
	bool isDisplayed;
	int max_nb_items;
	int nb_items;
	int* item_ids;
} InventoryComponent;

const int max_items = 50;