#pragma once

#include "itemComponent.h"

enum ItemID {
    POTION,
    APPLE,
    SWORD,
    KEY,
    SHIELD
};

extern int itemCount;
extern const int max_items;


extern ItemComponent potion;
extern ItemComponent apple;
extern ItemComponent sword;
extern ItemComponent key;
extern ItemComponent shield;

extern ItemComponent* itemList[];