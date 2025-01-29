#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../items/itemSystem.h"
#include "./inventoryComponent.h"

void initialize_inventory(InventoryComponent* invent, int maxItems, bool isDisplayed) {
	invent->isDisplayed = isDisplayed;
	invent->max_nb_items = maxItems;
	invent->nb_items = 0;

	invent->items = malloc(sizeof(ItemData) * maxItems);

	for (int i = 0; i < maxItems; i++) {
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
			}
			inventory->items[inventory->nb_items].id = -1; // Giga important pour enpêcher de draw un objet vide
			inventory->nb_items--;
			return true;
		}
	}

	return false;
}


bool mouse_in_slot(int x, int y, int row, int col) {
	// 15+c*(10+32) <= x < 32 + 15 + c * (10+32)
	// les noms des variables c'est empirique (ce sens marche)
	return 15 + col * 42 <= x && x <= 47 + col * 42 && // check x coord
		   15 + row * 42 <= y && y <= 47 + row * 42; // check y coord
}

bool mouse_in_any_slot(uint32_t entity, int x, int y) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
	if (invent == NULL) {
		return false;
	}

	int nbRows = 5; //Attention magic value à changer partout si elle change
	int nbCol = (invent->max_nb_items + nbRows - 1) / nbRows; // arrondi supérieur (c'est un peu magique)

	x -= 15; //remove margin
	y -= 15; //same


	return x%42 < 32 && x/42 < nbCol && y%42 < 32 && y/42 < nbRows; // magic numbers
}

int get_slot_of_mouse(uint32_t entity, int x, int y) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
	if (invent == NULL) {
		return -1;
	}

	int nbRows = 5; //Attention magic value à changer partout si elle change
	int nbCol = (invent->max_nb_items + nbRows - 1) / nbRows; // arrondi supérieur (c'est un peu magique)

	x -= 15; //remove margin
	y -= 15; //same

	// same as mouse in any slot
	if (!(x%42 < 32 && x/42 < nbCol && y%42 < 32 && y/42 < nbRows)) { 
		return -1;
	}

	return nbRows * (x/42) + y/42; // magic numbers

}

void draw_inventory(uint32_t entity, SDL_Renderer* renderer) {
	InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
	if (!inventory || !inventory->isDisplayed) {
		return;
	}

	int nbRows = 5; // magic value

	// Enable alpha blending
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Draw the semi-transparent background
	SDL_SetRenderDrawColor(renderer, 32, 36, 60, 210);
	SDL_Rect backgroundRect = { 5, 5, (inventory->max_nb_items + nbRows - 1) / nbRows * 42 + 15, nbRows * 42 + 15 }; // retirer le + nb - 1 si ça fait de la merde
	SDL_RenderFillRect(renderer, &backgroundRect);

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// draw the and items inside
	for (int i = 0; i < inventory->max_nb_items; i++) {
		int widthPos = i%nbRows;
		int heightPos = i/nbRows;

		// 42 = 10 (margin) + 32 (size of slot)
		SDL_Rect itemRect = { 15 + heightPos * 42, 15 + widthPos * 42, 32, 32 }; // Position and size of each item


		if (i < inventory->nb_items && inventory->items[i].id != -1) {
			
			// change the color according to pos of mouse
			if (mouse_in_slot(mouseX, mouseY, widthPos, heightPos)) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
			} else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}

			SDL_RenderFillRect(renderer, &itemRect);
			SDL_RenderCopy(renderer, get_texture_from_Id(inventory->items[i].id), NULL, &itemRect);
		} else {

			// change color if mouse in slot
			if (mouse_in_slot(mouseX, mouseY, widthPos, heightPos)) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 255, 128);
			} else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
			}

			SDL_RenderFillRect(renderer, &itemRect);
		}
	
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}