#ifndef ITEM_LIST_H
#define ITEM_LIST_H

#include "itemData.h"

enum ItemID {
    POTION,
    APPLE,
    SWORD,
    KEY,
    SHIELD
};

extern const int itemCount;

extern ItemData potion;
extern ItemData apple;
extern ItemData sword;
extern ItemData key;
extern ItemData shield;

extern ItemData* itemList[];

#endif // ITEM_LIST_H