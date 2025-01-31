#include "itemsFunctions.h"

void itemHeal(uint32_t entity, ItemData currentItem) {
	heal(entity, 5); //for the moment heal is not implemented
}

void itemDoNothing(u_int32_t entity, ItemData currentItem) {}

void unselectItem(uint32_t entity, ItemData currentItem) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (invent == NULL) return;

	invent->selected_slot = -1;
	invent->selected_slot_actions = NULL;
}

ActionFunction get_constant_functions(int i) {
	// à changer pour mettre un array à la place

	switch (i) {
	case 0:
		/* code */
		break;
	
	case 1:
		return unselectItem;
		break;

	default:
		return itemDoNothing;
	}
}