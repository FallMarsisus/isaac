#pragma once

#include <stdint.h>
#include "../items/itemData.h"

typedef struct {
    enum ItemID id; // to link the identifier with an item
    int damage; 
    int range;
    int cooldown;
    SDL_Renderer* renderer;
} SwordComponent;