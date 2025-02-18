#ifndef ITEM_FUNCTIONS_H
#define ITEM_FUNCTIONS_H

#include <stdlib.h>
#include "itemData.h"
#include "../inventory/inventoryComponent.h"
#include "../health/healthSystem.h"

void itemHeal(uint32_t entity, ItemData currentItem);
void itemDoNothing(u_int32_t entity, ItemData currentItem);

ActionFunction get_constant_functions(int i);
char* get_actions_name(int i);

#endif // ITEM_FUNCTIONS_H