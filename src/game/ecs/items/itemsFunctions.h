#ifndef ITEM_FUNCTIONS_H
#define ITEM_FUNCTIONS_H

#include <stdlib.h>
#include "itemData.h"
#include "../inventory/inventoryComponent.h"
#include "../health/healthSystem.h"

typedef enum slots {
	NORMAL_SLOT,
	WEAPON,
	ARMOR
} slots;

void item_heal(uint32_t entity, ItemData currentItem);
void item_do_nothing(u_int32_t entity, ItemData currentItem);
void unselectItem(uint32_t entity, ItemData currentItem);
void weapon_item_equip(uint32_t entity, ItemData currentItem);
void weapon_item_unequip(uint32_t entity, ItemData currentItem);
void throwItemAtMouse(uint32_t player, int itemIndex, SDL_Rect cam, int win_width, int true_width);
void throwItem(uint32_t player, int itemIndex, Vector* throwDirection);

ActionFunction get_constant_functions(int i, slots slot);
char* get_actions_name(int i, slots slot);

#endif // ITEM_FUNCTIONS_H