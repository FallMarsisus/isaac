#ifndef ITEM_FUNCTIONS_H
#define ITEM_FUNCTIONS_H

#include "itemData.h"
#include "../health/healthSystem.h"

void itemHeal(uint32_t entity, ItemData currentItem);
void itemDoNothing(u_int32_t entity, ItemData currentItem);

#endif // ITEM_FUNCTIONS_H