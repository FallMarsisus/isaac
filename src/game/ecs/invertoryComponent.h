#pragma once

#include <stdint.h>

typedef struct {
	int max_nb_items;
	int nb_items;
	int* items;
} InventoryComponent;