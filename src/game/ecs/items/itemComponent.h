#pragma once

#include <stdint.h>
#include "itemData.h"
#include <stdbool.h>
#include "../../../utils/timer.h"

typedef struct {
	ItemData item;
	bool isGettable; // c'est un peu random ce qu'il y a ici pour l'instant
	bool isDropperLocked;
	uint32_t dropper;
	Timer* makeDropperUnlocked;
} ItemComponent;
