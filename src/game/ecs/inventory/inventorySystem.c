#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../items/itemSystem.h"
#include "./inventoryComponent.h"

void initialize_inventory(InventoryComponent* invent, int maxItems, bool isDisplayed) {
	invent->isDisplayed = isDisplayed;
	invent->max_nb_items = maxItems;
	invent->nb_items = 0;

	invent->itemsTextures = malloc(sizeof(SDL_Texture*) * maxItems);
	invent->items = malloc(sizeof(ItemData) * maxItems);

	for (int i = 0; i < maxItems; i++) {
		invent->itemsTextures[i] = NULL;
		invent->items[i].id = -1;
		invent->items[i].name = "";
		invent->items[i].description = "";
		invent->items[i].value = -1;
	}
}
void free_inventory(InventoryComponent* invent) {
	free(invent->items);
}

bool add_item_to_inventory(uint32_t entity, ItemData item) {
	InventoryComponent* inventory = ECS_GetComponent(entity , INVENT);
	if (inventory && inventory->nb_items < inventory->max_nb_items) {
		inventory->items[inventory->nb_items] = item;
		inventory->itemsTextures[inventory->nb_items] = get_texture_from_Id(item.id);
		if (inventory->itemsTextures[inventory->nb_items] == NULL) {
			printf("failed to add item texture\n");
			return false;
		}
		printf("not failed\n");
		inventory->nb_items++;	
		return true;
	}
	printf("maxi fail");
	return false;
}

bool remove_item_from_inventory(uint32_t entity, ItemData item) { //faudra mettre l'id plus tard plutot que l'item entier
	InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
	if (!inventory || inventory->nb_items <= 0) {
		return false;
	}

	// Remove the element from the array
	for (int i = 0; i < inventory->nb_items; i++) {
		if (inventory->items[i].id == item.id) { // Pour l'instant l'id est pas unique donc ça suppr pas forcément le bon
			for (int j = i; j < inventory->nb_items-1; j++) {
				inventory->items[j] = inventory->items[j+1];
				inventory->itemsTextures[j] = inventory->itemsTextures[j+1];
			}
			inventory->itemsTextures[inventory->nb_items] = NULL;
			inventory->nb_items--;
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

	int nbRows = 5;
	// Enable alpha blending
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Draw the semi-transparent background
	SDL_SetRenderDrawColor(renderer, 32, 36, 60, 210);
	SDL_Rect backgroundRect = { 5, 5, (inventory->max_nb_items / nbRows) * 42 + 15, nbRows * 40 + 15 };
	SDL_RenderFillRect(renderer, &backgroundRect);

	// draw the and items inside
	for (int i = 0; i < inventory->max_nb_items; i++) {
		int widthPos = i%nbRows;
		int heightPos = i/nbRows;

		// 42 = 10 (margin) + 32 (size of slot)
		SDL_Rect itemRect = { 15 + heightPos * 42, 15 + widthPos * 40, 32, 32 }; // Position and size of each item


		if (i < inventory->nb_items && inventory->itemsTextures[i] != NULL) {
			// Draw the item texture if not NULL
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &itemRect);
			SDL_RenderCopy(renderer, inventory->itemsTextures[i], NULL, &itemRect);
		} else {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
			SDL_RenderFillRect(renderer, &itemRect);
		}
	
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}