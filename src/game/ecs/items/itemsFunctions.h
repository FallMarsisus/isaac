#ifndef ITEM_FUNCTIONS_H
#define ITEM_FUNCTIONS_H

#include <stdlib.h>
#include "itemData.h"
#include "../inventory/inventoryComponent.h"
#include "../health/healthSystem.h"

void item_heal(uint32_t entity, ItemData currentItem);
void item_do_nothing(u_int32_t entity, ItemData currentItem);
void unselectItem(uint32_t entity, ItemData currentItem);
void weapon_item_equip(uint32_t entity, ItemData currentItem);

ActionFunction get_constant_functions(int i);
char* get_actions_name(int i);

#endif // ITEM_FUNCTIONS_H