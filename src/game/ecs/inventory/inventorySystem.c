#include "inventorySystem.h"

bool add_item_to_inventory(uint32_t entity, int item) {
	InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
	if (inventory && inventory->nb_items < inventory->max_nb_items) {
		inventory->item_ids[inventory->nb_items] = item;
		inventory->nb_items++;	
		return true;
	}
	return false;
}

bool remove_item_from_inventory(uint32_t entity, int item) {
	InventoryComponent* invertory = ECS_GetComponent(entity, INVENT);
	if (!invertory || invertory->nb_items <= 0) {
		return false;
	}

	// Remove the element from the array
	for (int i = 0; i < invertory->nb_items; i++) {
		if (invertory->item_ids[i] == item) {
			for (int j = i; j < invertory->nb_items-1; j++) {
				invertory->item_ids[j] = invertory->item_ids[j+1];
			}
			invertory->nb_items--;
			return true;
		}
	}

	return false;
}

void draw_inventory(uint32_t entity, SDL_Renderer* renderer) {
	InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
	if (!inventory || !inventory->isDisplayed) {
		return;
	}


	printf("drawing inv : %d items\n", inventory->nb_items);
	for (int i = 0; i < inventory->nb_items; i++) {
		SDL_Rect itemRect = { 10, 10 + i * 40, 32, 32 }; // Position and size of each item
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for items
		SDL_RenderFillRect(renderer, &itemRect);
	}
}