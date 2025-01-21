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


extern ItemData potion;
extern ItemData apple;
extern ItemData sword;
extern ItemData key;
extern ItemData shield;

extern ItemData* itemList[];