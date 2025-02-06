#include <stdbool.h>
#include <stdint.h>
#include "../ecs.h"
#include "../items/itemSystem.h"
#include "./inventoryComponent.h"
#include "../../../utils/mouse.h"

// valeurs pour l'affichage de l'inventaire
#define NB_ROWS 5
#define SLOT_SIZE 32
#define MARGIN 5
#define SPACING 10

void initialize_inventory(InventoryComponent* invent, int maxItems, bool isDisplayed) {
	invent->isDisplayed = isDisplayed;
	invent->max_nb_items = maxItems;
	invent->nb_items = 0;
	invent->selected_slot = -1;
	invent->selected_slot_actions = NULL;

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
	freeAction(invent->selected_slot_actions);
	free(invent->selected_slot_actions);
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
	return (MARGIN+SPACING) + col * (SPACING+SLOT_SIZE) <= x && x <= (MARGIN+SPACING) + (col+1) * (SPACING+SLOT_SIZE) && // check x coord
		   (MARGIN+SPACING) + row * (SPACING+SLOT_SIZE) <= y && y <= (MARGIN+SPACING) + (row+1) * (SPACING+SLOT_SIZE); // check y coord
}

bool mouse_in_any_slot(uint32_t entity, int x, int y) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
	if (invent == NULL || !invent->isDisplayed) {
		return false;
	}

	int nbCol = (invent->max_nb_items + NB_ROWS - 1) / NB_ROWS; // arrondi supérieur (c'est un peu magique)

	x -= MARGIN + SPACING; //remove margin
	y -= MARGIN + SPACING; //same


	return x % (SPACING + SLOT_SIZE) < SLOT_SIZE && x / (SPACING + SLOT_SIZE) < nbCol && 
		   y % (SPACING + SLOT_SIZE) < SLOT_SIZE && y / (SPACING + SLOT_SIZE) < NB_ROWS;
}

int get_slot_of_mouse(uint32_t entity, int x, int y) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	// prevent no inventory && inventory not displayed
	if (invent == NULL || !invent->isDisplayed) {
		invent->selected_slot = -1;
		return -1;
	}

	int nbCol = (invent->max_nb_items + NB_ROWS - 1) / NB_ROWS; // arrondi supérieur (c'est un peu magique)

	x -= MARGIN + SPACING; //remove margin
	y -= MARGIN + SPACING; //same

	// check if the mouse is in a slot
	if (!(x % (SPACING + SLOT_SIZE) < SLOT_SIZE && x / (SPACING + SLOT_SIZE) < nbCol && 
		   y % (SPACING + SLOT_SIZE) < SLOT_SIZE && y / (SPACING + SLOT_SIZE) < NB_ROWS)) { 
		return -1;
	}

	invent->selected_slot = NB_ROWS * (x/42) + y/42;
	invent->selected_slot_actions = get_item_actions(invent->items[invent->selected_slot].id);

	return NB_ROWS * (x/42) + y/42; // magic numbers

}

int get_clicked_acion(InventoryComponent* invent, int x, int y) {
	if (!invent || invent->selected_slot == -1 || invent->selected_slot_actions == NULL) {
		return -1;
	}

	int nbRows = NB_ROWS;
	int widthPos = invent->selected_slot % nbRows;
	int heightPos = invent->selected_slot / nbRows;

	int xPos = (MARGIN+SPACING+SLOT_SIZE) + heightPos * (SPACING + SLOT_SIZE);

	for (int i = 0; i < invent->selected_slot_actions->nb_actions; i++) {
		int yPos = (MARGIN+SPACING) + (widthPos+i) * (SPACING + SLOT_SIZE);
		SDL_Rect actions_rect = {
			xPos,
			yPos,
			160,
			SPACING+SLOT_SIZE
		};

		if (mouseInRect(x, y, actions_rect)) {
			return i;
		}
	}

	for (int i = invent->selected_slot_actions->nb_actions; i < invent->selected_slot_actions->nb_actions+2; i++) {
		int yPos = (MARGIN+SPACING) + (widthPos+i) * (SPACING + SLOT_SIZE);
		SDL_Rect actions_rect = {
			xPos,
			yPos,
			160,
			SPACING+SLOT_SIZE
		};

		if (mouseInRect(x, y, actions_rect)) {
			return i;
		}
	}

	return -1;
}

void executeAction(uint32_t entity, InventoryComponent* invent, int selectedAction) {
	// fonction super moche
	(invent->selected_slot_actions->functions[selectedAction])(entity, invent->items[invent->selected_slot]);
}

int onClic(uint32_t entity, int x, int y) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
	if (!invent) return -1;

	if (invent->selected_slot == -1) {
		invent->selected_slot = mouse_in_any_slot(entity, x, y);
		invent->selected_slot_actions = get_item_actions(invent->items[invent->selected_slot].id);
		return invent->selected_slot;
	}

	// pour gérer le cas si y'a déjà un slot sélectionné
	int action = get_clicked_acion(invent, x, y);
	printf("Action selected: %d\n", action);

	if (invent->selected_slot_actions && invent->selected_slot != -1 && action >= invent->selected_slot_actions->nb_actions) {
		get_constant_functions(action - invent->selected_slot_actions->nb_actions)(entity, invent->items[invent->selected_slot]);
		return -1;
	} else if (action >= 0) {
		executeAction(entity, invent, action);
		return -1;
	} else { // ce else me trigger
		invent->selected_slot = mouse_in_any_slot(entity, x, y);
		invent->selected_slot_actions = get_item_actions(invent->items[invent->selected_slot].id);
		return invent->selected_slot;
	}
}



void draw_item_actions(InventoryComponent* inventory, SDL_Renderer* renderer) {

	// si pas besoin d'iter à afficher
	if (!inventory || inventory->selected_slot == -1 || inventory->selected_slot_actions == NULL) return;

	// si pas d'item dans la case
	if (inventory->selected_slot >= inventory->nb_items || inventory->items[inventory->selected_slot].id == -1) return;

	int nbRows = NB_ROWS;
	int widthPos = inventory->selected_slot % nbRows;
	int heightPos = inventory->selected_slot / nbRows;


	int xPos = (MARGIN+SPACING+SLOT_SIZE) + heightPos * (SPACING + SLOT_SIZE); // fixeds
	SDL_Rect actions_rect;


	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	for (int i = 0; i < inventory->selected_slot_actions->nb_actions; i++) {

		int yPos = (MARGIN+SPACING) + (widthPos+i) * (SPACING + SLOT_SIZE);

		actions_rect = (SDL_Rect) {
			xPos,
			yPos,
			160,
			SPACING+SLOT_SIZE
		};

		if (mouseInRect(mouseX, mouseY, actions_rect)) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		} else {
			SDL_SetRenderDrawColor(renderer, 255, 200, 255, 255);
		}

		SDL_RenderFillRect(renderer, &actions_rect);
	}


	// techniquemement ça marche de deux manières donc je préfère faire deux boucles
	for (int i = inventory->selected_slot_actions->nb_actions; i < inventory->selected_slot_actions->nb_actions+2; i++) {

		int yPos = (MARGIN+SPACING) + (widthPos+i) * (SPACING + SLOT_SIZE);
		actions_rect = (SDL_Rect) {
			xPos,
			yPos,
			160,
			SPACING+SLOT_SIZE
		};

		if (mouseInRect(mouseX, mouseY, actions_rect)) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		} else {
			SDL_SetRenderDrawColor(renderer, 255, 200, 255, 255);
		}

		SDL_RenderFillRect(renderer, &actions_rect);
	}


}

void draw_inventory(uint32_t entity, SDL_Renderer* renderer) {
	InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
	if (!inventory || !inventory->isDisplayed) {
		return;
	}

	int nbRows = NB_ROWS;

	// Enable alpha blending
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Draw the semi-transparent background
	SDL_SetRenderDrawColor(renderer, 32, 36, 60, 210); // rgba(50, 54, 96, 0.82) 
	
	// trust me ça marche ça 
	SDL_Rect backgroundRect = {
		MARGIN,
		MARGIN,
		(inventory->max_nb_items + nbRows - 1) / nbRows * (SLOT_SIZE+SPACING) + (MARGIN+SPACING),
		nbRows * (SLOT_SIZE + SPACING) + (MARGIN + SPACING) 
	}; // retirer le + nb - 1 si ça fait de la merde

	SDL_RenderFillRect(renderer, &backgroundRect);

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// draw the and items inside
	for (int i = 0; i < inventory->max_nb_items; i++) {
		int widthPos = i%nbRows;
		int heightPos = i/nbRows;

		// 42 = 10 (margin) + 32 (size of slot)
		SDL_Rect itemRect = {
			(MARGIN+SPACING) + heightPos * (SPACING + SLOT_SIZE),
			(MARGIN+SPACING) + widthPos * (SPACING + SLOT_SIZE),
			SLOT_SIZE,
			SLOT_SIZE
		}; // Position and size of each item



		// change the color according to pos of mouse and if slot is full
		if (i < inventory->nb_items && inventory->items[i].id != -1) {
			
			// change the color according to pos of mouse
			if (mouseInRect(mouseX, mouseY, itemRect)) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
			} else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}

			SDL_RenderFillRect(renderer, &itemRect);
			SDL_RenderCopy(renderer, get_texture_from_Id(inventory->items[i].id), NULL, &itemRect);
		} else {

			// change color if mouse in slot
			if (mouseInRect(mouseX, mouseY, itemRect)) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 255, 128);
			} else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
			}

			SDL_RenderFillRect(renderer, &itemRect);
		}
	
	}

	draw_item_actions(inventory, renderer);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}