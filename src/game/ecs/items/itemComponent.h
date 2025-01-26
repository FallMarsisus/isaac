#pragma once

#include <stdint.h>
#include "itemData.h"
#include <stdbool.h>

typedef struct {
	ItemData item;
	bool isGettable; // c'est un peu random ce qu'il y a ici pour l'instant
} ItemComponent;
