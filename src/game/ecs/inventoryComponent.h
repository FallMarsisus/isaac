#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	bool isDisplayed;
	int max_nb_items;
	int nb_items;
	int* items;
} InventoryComponent;