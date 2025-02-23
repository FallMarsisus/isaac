#include "itemsFunctions.h"
#include "../inventory/inventorySystem.h"
#include "../systems.h"
#include "../../../utils/ouputColors.h"

void item_heal(uint32_t entity, ItemData currentItem) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (invent == NULL) return;

	heal(entity, 5);

	remove_item_from_inventory(entity, currentItem);
}

void item_do_nothing(u_int32_t entity, ItemData currentItem) {}

void unselectItem(uint32_t entity, ItemData currentItem) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (invent == NULL) return;

	invent->selected_slot = -1;
	invent->selected_slot_actions = NULL;
}

void dropItem(uint32_t entity, ItemData currentItem) {
	PositionComponent* pos = ECS_GetComponent(entity, POSITION);
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
	if (!invent) return;

	if (pos == NULL || !remove_item_index_invent(entity, invent->selected_slot)) return;


	// printf(CYAN "Dropping item: %d\n" RESET, currentItem.id);
	add_item_entity(pos->x+5, pos->y+5, currentItem, entity, true);
}

void weapon_item_equip(uint32_t entity, ItemData currentItem) {
	// printf("equiping item : %d\n", currentItem.id);
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (!invent) return;

	swap_items_inventory(entity, invent->selected_slot, invent->max_nb_items);
}

void weapon_item_unequip(uint32_t entity, ItemData currentItem) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (!invent) return;

	if (!add_item_to_inventory(entity, currentItem)) return;

	remove_item_index_invent(entity, invent->selected_slot);
}

ActionFunction get_constant_functions(int i, slots slot) {
	// à changer pour mettre un array à la place

	switch (i) {
	case 0:
		return dropItem;
		break;
	
	case 1:
		return unselectItem;
		break;

	default:
		return item_do_nothing;
	}
}

char* get_actions_name(int i, slots slot) {

	switch (i) {
	case 0:
		return "l\xE2\cher";
		break;
	
	case 1:
		return "unselect";
		break;

	default:
		return "should not happen";
	}
}