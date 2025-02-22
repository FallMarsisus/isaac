#include "./inventorySystem.h"
#include "./inventoryUI.h"
#include "../items/itemSystem.h"
#include "../../../utils/mouse.h"
#include "../../../utils/displayText.h"
#include "../../../utils/ouputColors.h"
#include "../../../utils/mouse.h"
#include "../../../utils/displayText.h"
#include "../../../utils/ouputColors.h"
#include "../ecs.h"
#include "../items/itemSystem.h"
#include "./inventoryComponent.h"
#include "./inventorySystem.h"
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

bool mouse_in_slot(int x, int y, int row, int col) {
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

    x -= MARGIN + SPACING; // remove margin
    y -= MARGIN + SPACING; // same


	return x % (SPACING + SLOT_SIZE) < SLOT_SIZE && x / (SPACING + SLOT_SIZE) < nbCol && 
		   y % (SPACING + SLOT_SIZE) < SLOT_SIZE && y / (SPACING + SLOT_SIZE) < NB_ROWS;
}


bool mouse_in_constant_slots(int nth, int mouseX, int mouseY) {
	SDL_Rect weaponRect = {
		(MARGIN + SPACING) + nth * (SPACING + SLOT_SIZE),
		(MARGIN + 2*SPACING) + NB_ROWS * (SPACING + SLOT_SIZE),
		SLOT_SIZE,
		SLOT_SIZE
	};

	return mouse_in_rect(mouseX, mouseY, weaponRect);
}

int get_slot_of_mouse(uint32_t entity, int mouseX, int mouseY) {
    InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

    // prevent no inventory && inventory not displayed
    if (invent == NULL || !invent->isDisplayed) {
        invent->selected_slot = -1;
        return -1;
    }

    int nbCol = (invent->max_nb_items + NB_ROWS - 1) / NB_ROWS; // arrondi supérieur (c'est un peu magique)

	int adjustedMouseX = mouseX - MARGIN - SPACING; // remove margin
	int adjustedMouseY = mouseY - MARGIN - SPACING; // same

    // check if the mouse is in a slot
	if ((adjustedMouseX % (SPACING + SLOT_SIZE) < SLOT_SIZE && adjustedMouseX / (SPACING + SLOT_SIZE) < nbCol && 
		   adjustedMouseY % (SPACING + SLOT_SIZE) < SLOT_SIZE && adjustedMouseY / (SPACING + SLOT_SIZE) < NB_ROWS)) { 
		int slot =  NB_ROWS * (adjustedMouseX / (SLOT_SIZE + SPACING)) + adjustedMouseY / (SLOT_SIZE + SPACING);
		invent->selected_slot = slot;
        return slot;
    }

	
	//handle the armor & weapon slots
	if (mouse_in_constant_slots(0, mouseX, mouseY)) { //weapon
		invent->selected_slot = invent->max_nb_items;
		return invent->selected_slot;
	} else if (mouse_in_constant_slots(1, mouseX, mouseY)) { //armor
		invent->selected_slot = invent->max_nb_items+1;
		return invent->selected_slot;
	}

	return -1;

}


int get_clicked_acion(InventoryComponent* invent, int mouseX, int mouseY) {
    if (!invent || invent->selected_slot < 0 || invent->selected_slot >= invent->max_nb_items + NB_CST_SLOTS || invent->selected_slot_actions == NULL) {
        fprintf(stderr, RED "invalud argument on get_clicked_action\n" RESET);
		return -1;
    }
	printf(GREEN "get_clicked_action called with mouseX: %d, mouseY: %d\n" RESET, mouseX, mouseY);
    int nbRows = NB_ROWS;
	int xPos, defaultY;

	if (invent->selected_slot >= invent->max_nb_items) {
		xPos = (MARGIN + SPACING + SLOT_SIZE) + (invent->selected_slot - invent->max_nb_items) * (SPACING + SLOT_SIZE);
        defaultY = (MARGIN + 2*SPACING) + NB_ROWS * (SPACING + SLOT_SIZE);
    } else {
		xPos = (MARGIN + SPACING + SLOT_SIZE) + (invent->selected_slot / nbRows) * (SPACING + SLOT_SIZE);
		defaultY = (MARGIN + SPACING) + (invent->selected_slot % nbRows) * (SPACING + SLOT_SIZE);
	}


    for (int i = 0; i < invent->selected_slot_actions->nb_actions; i++) {
        int yPos = defaultY + i * (SPACING + SLOT_SIZE);
        SDL_Rect actions_rect = {
            xPos,
            yPos,
            160,
            SPACING + SLOT_SIZE
        };

        if (mouse_in_rect(mouseX, mouseY, actions_rect)) {
            return i;
        }
    }

    for (int i = invent->selected_slot_actions->nb_actions; i < invent->selected_slot_actions->nb_actions + 2; i++) {
        int yPos = defaultY + i * (SPACING + SLOT_SIZE);
        SDL_Rect actions_rect = {
            xPos,
            yPos,
            160,
            SPACING + SLOT_SIZE
        };

        if (mouse_in_rect(mouseX, mouseY, actions_rect)) {
            return i;
        }
    }

    return -1;
}


void executeAction(uint32_t entity, InventoryComponent* invent, int selectedAction) {
    // fonction super moche
    (invent->selected_slot_actions->functions[selectedAction])(entity, invent->items[invent->selected_slot]);
}

int on_clic(uint32_t entity, int mouseX, int mouseY) {

    InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
    if (!invent)
        return -1;

    if (invent->selected_slot == -1) {
        invent->selected_slot = get_slot_of_mouse(entity, mouseX, mouseY);

        free_action(invent->selected_slot_actions);
        free(invent->selected_slot_actions);

        invent->selected_slot_actions = get_item_actions(invent->items[invent->selected_slot].id);
        return invent->selected_slot;
    }


    // pour gérer le cas si y'a déjà un slot sélectionné
    int action;
    if (invent->selected_slot_actions != NULL) {
        action = get_clicked_acion(invent, mouseX, mouseY);
    } else {
        action = -1;
    }

    if (invent->selected_slot_actions && invent->selected_slot != -1 && action >= invent->selected_slot_actions->nb_actions) {
        get_constant_functions(action - invent->selected_slot_actions->nb_actions)(entity, invent->items[invent->selected_slot]);
        return -1;
    } else if (action >= 0) {
        executeAction(entity, invent, action);
        return -1;
    }
    invent->selected_slot = get_slot_of_mouse(entity, mouseX, mouseY);

    free_action(invent->selected_slot_actions);
    free(invent->selected_slot_actions);

    invent->selected_slot_actions = get_item_actions(invent->items[invent->selected_slot].id);

    return invent->selected_slot;
}



void draw_item_actions(InventoryComponent* inventory, SDL_Renderer* renderer, int trueWidth, int renderWidth) {

    double scaleFactor = (double) renderWidth / trueWidth;


    // si pas besoin d'iter à afficher
    if (!inventory || inventory->selected_slot == -1 || inventory->selected_slot_actions == NULL) return;

    // si pas d'item dans la case
    if (inventory->selected_slot >= inventory->nb_items || inventory->items[inventory->selected_slot].id == -1) return;

    int nbRows = NB_ROWS;
    int widthPos = inventory->selected_slot % nbRows;
    int heightPos = inventory->selected_slot / nbRows;

    int xPos = ((MARGIN + SPACING + SLOT_SIZE) + heightPos * (SPACING + SLOT_SIZE)) * scaleFactor; // fixeds
    SDL_Rect actions_rect;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

	SDL_Color textColor = {255, 255, 255, 255};
	int fontSize = SLOT_SIZE * scaleFactor / 2;
	int yOffset = 2;
	int xOffset = 10;

    for (int i = 0; i < inventory->selected_slot_actions->nb_actions; i++) {

        int yPos = ((MARGIN + SPACING) + (widthPos + i) * (SPACING + SLOT_SIZE)) * scaleFactor;

        actions_rect = (SDL_Rect) {
            xPos,
            yPos,
            160 * scaleFactor,
            (SPACING + SLOT_SIZE) * scaleFactor
        };

        if (mouse_in_rect_fix_drift(mouseX, mouseY, &actions_rect, trueWidth, renderWidth)) {
            SDL_SetRenderDrawColor(renderer, 64, 72, 120, 240);
        } else {
            SDL_SetRenderDrawColor(renderer, 32, 36, 60, 240);
        }

        SDL_RenderFillRect(renderer, &actions_rect);

		display_text(inventory->selected_slot_actions->titles[i], renderer, get_fonts()->calibri, &textColor, xPos + xOffset, yPos + yOffset, fontSize);
    }

    // techniquemement ça marche de deux manières donc je préfère faire deux boucles
    for (int i = inventory->selected_slot_actions->nb_actions; i < inventory->selected_slot_actions->nb_actions + 2; i++) {

        int yPos = ((MARGIN + SPACING) + (widthPos + i) * (SPACING + SLOT_SIZE)) * scaleFactor;
        actions_rect = (SDL_Rect) {
            xPos,
            yPos,
            160 * scaleFactor,
            (SPACING + SLOT_SIZE) * scaleFactor
        };

        if (mouse_in_rect_fix_drift(mouseX, mouseY, &actions_rect, trueWidth, renderWidth)) {
            SDL_SetRenderDrawColor(renderer, 64, 72, 120, 240);
        } else {
            SDL_SetRenderDrawColor(renderer, 32, 36, 60, 240);
        }

        SDL_RenderFillRect(renderer, &actions_rect);
		display_text(get_actions_name(i - inventory->selected_slot_actions->nb_actions), renderer, get_fonts()->calibri, &textColor, xPos + xOffset, yPos + yOffset, fontSize);

    }
}

void displayConstantSlot(SDL_Renderer* renderer, InventoryComponent* invent, int win_width, int win_height, int true_width, int true_height, int nbRows, 
	int mouseX, int mouseY, int nth, SDL_Texture* texture) {

    // display hand slot
    SDL_Rect slotRect = {
        ((MARGIN + SPACING) + nth * (SPACING + SLOT_SIZE)) * win_width / (double)true_width,
        ((MARGIN + 2*SPACING) + nbRows * (SPACING + SLOT_SIZE)) * win_height / (double)true_height,
        SLOT_SIZE * win_width / (double)true_width,
        SLOT_SIZE * win_height / (double)true_height
    };

	bool mouseOverSlot = mouse_in_rect_fix_drift(mouseX, mouseY, &slotRect, true_width, win_width);
	int i = nth + invent->max_nb_items;
    if (mouseOverSlot && invent->items[i].id != -1) {
		SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
    } else if (mouseOverSlot) {
		SDL_SetRenderDrawColor(renderer, 200, 200, 255, 128);
    } else if (invent->items[i].id != -1) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	} else {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
	}

    SDL_RenderFillRect(renderer, &slotRect);

	if (invent->items[i].id != -1) {
		SDL_RenderCopy(renderer, get_texture_from_Id(invent->items[i].id), NULL, &slotRect);
	} else {
		SDL_RenderCopy(renderer, texture, NULL, &slotRect);
	}

}

void draw_inventory(uint32_t entity, SDL_Renderer* renderer, int win_width, int win_height, int true_width , int true_height) {
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
        MARGIN * win_width/1920,
        MARGIN * win_height/1024,
        ((inventory->max_nb_items + nbRows - 1) / nbRows * (SLOT_SIZE + SPACING) + (MARGIN + SPACING)) * win_width / true_width,
        ((nbRows+1) * (SLOT_SIZE + SPACING) + (MARGIN + 2*SPACING)) * win_height/true_height
    }; // retirer le + nb - 1 si ça fait de la merde

    SDL_RenderFillRect(renderer, &backgroundRect);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // draw the and items inside
    for (int i = 0; i < inventory->max_nb_items; i++) {
        int widthPos = i % nbRows;
        int heightPos = i / nbRows;

        // 42 = 10 (margin) + 32 (size of slot)
        SDL_Rect itemRect = {
            ((MARGIN + SPACING) + heightPos * (SPACING + SLOT_SIZE)) * win_width / (double)true_width,
            ((MARGIN + SPACING) + widthPos * (SPACING + SLOT_SIZE)) * win_height / (double)true_height,
            SLOT_SIZE * win_width / (double)true_width,
            SLOT_SIZE * win_height / (double)true_height
        }; // Position and size of each item

        // change the color according to pos of mouse and if slot is full
        if (i < inventory->nb_items && inventory->items[i].id != -1) {

            // change the color according to pos of mouse
            if (mouse_in_rect_fix_drift(mouseX, mouseY, &itemRect, true_width, win_width)) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_RenderFillRect(renderer, &itemRect);
            SDL_RenderCopy(renderer, get_texture_from_Id(inventory->items[i].id), NULL, &itemRect);
        } else {

            // change color if mouse in slot
            if (mouse_in_rect_fix_drift(mouseX, mouseY, &itemRect, true_width, win_width)) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 255, 128);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            }

            SDL_RenderFillRect(renderer, &itemRect);
        }
    }


	// weapon slot
	displayConstantSlot(renderer, inventory, win_width, win_height, true_width, true_height, nbRows, mouseX, mouseY, 0, get_sprites()->sword_slot_texture);
	
	// armor slot s
	displayConstantSlot(renderer, inventory, win_width, win_height, true_width, true_height, nbRows, mouseX, mouseY, 1, get_sprites()->armor_slot_texture);


    draw_item_actions(inventory, renderer, true_width, win_width);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}


void print_inv_debug(InventoryComponent* invent) {
	if (!invent) {
		printf("No inventory component found for entity \n");
		return;
	}

	printf("\n");
	printf("Max items: %d\n", invent->max_nb_items);
	printf("Number of items: %d\n", invent->nb_items);
	printf("Selected slot: %d\n", invent->selected_slot);
	printf("Is displayed: %s\n", invent->isDisplayed ? "true" : "false");

	for (int i = 0; i < invent->max_nb_items; i++) {
		printf("Slot %d: Item ID %d\n", i, invent->items[i].id);
	}

	for (int i = invent->max_nb_items; i < invent->max_nb_items + NB_CST_SLOTS; i++) {
		printf("Constant Slot %d: Item ID %d\n", i, invent->items[i].id);
	}
}