#include "itemsFunctions.h"
#include "../inventory/inventorySystem.h"
#include "../systems.h"

void itemHeal(uint32_t entity, ItemData currentItem) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (invent == NULL) return;

	heal(entity, 5);

	remove_item_from_inventory(entity, currentItem);
}

void itemDoNothing(u_int32_t entity, ItemData currentItem) {}

void unselectItem(uint32_t entity, ItemData currentItem) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (invent == NULL) return;

	invent->selected_slot = -1;
	invent->selected_slot_actions = NULL;
}

void dropItem(uint32_t entity, ItemData currentItem) {
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);

	if (pos == NULL || !remove_item_from_inventory(entity, currentItem)) return;

	add_item_entity(pos->x, pos->y, currentItem);
}

ActionFunction get_constant_functions(int i) {
	// à changer pour mettre un array à la place

	switch (i) {
	case 0:
		return dropItem;
		break;
	
	case 1:
		return unselectItem;
		break;

	default:
		return itemDoNothing;
	}
}