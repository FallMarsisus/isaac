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


void reset_item(InventoryComponent* invent, int i) {
    if (i < 0 || i > invent->max_nb_items + NB_CST_SLOTS) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, RED "reset_item: index out of bounds" RESET);
        return;
    }

    invent->items[i].id = -1;
    invent->items[i].name = "";
    invent->items[i].description = "";
    invent->items[i].value = -1;
	invent->items[i].throwProp = NULL;
}


void initialize_inventory(InventoryComponent* invent, int maxItems, bool isDisplayed) {
    invent->isDisplayed = isDisplayed;
    invent->max_nb_items = maxItems;
    invent->nb_items = 0;
    invent->selected_slot = -1;
    invent->selected_slot_actions = NULL;

    invent->items = malloc(sizeof(ItemData) * (maxItems+NB_CST_SLOTS));

    for (int i = 0; i < maxItems+NB_CST_SLOTS; i++) {
        reset_item(invent, i);
    }
}


void free_inventory(InventoryComponent* invent) {
    free(invent->items);
    if (invent->selected_slot_actions != NULL) {
        free_action(invent->selected_slot_actions);
        invent->selected_slot_actions = NULL; // on sait jamais
    }
}


bool add_item_to_inventory(uint32_t entity, ItemData item) {
    InventoryComponent* inventory = ECS_GetComponent(entity , INVENT);
    
    if (!inventory) {
        fprintf(stderr, RED "Entity has no invent\n" RESET);
        return false;
    }
    
    if (inventory->nb_items >= inventory->max_nb_items) {
        fprintf(stderr, YELLOW "Inventory is full\n" RESET);
        return false;
    }

    for (int i = 0; i < inventory->max_nb_items; i++) {
        if (inventory->items[i].id == -1) {
            inventory->items[i] = item;
            inventory->nb_items++;
            return true;
        }
    }

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, RED "invent failed to add item" RESET);
    return false;
}


bool remove_item_from_inventory(uint32_t entity, ItemData item) { //faudra mettre l'id plus tard plutot que l'item entier
    InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
    if (!inventory || inventory->nb_items <= 0) {
        return false;
    }

    if (inventory->selected_slot != -1 && inventory->items[inventory->selected_slot].id == item.id) {
        free_action(inventory->selected_slot_actions);
        free(inventory->selected_slot_actions);

        inventory->selected_slot = -1;
        inventory->selected_slot_actions = NULL;
    }

    // Remove the element from the array
    for (int i = 0; i < inventory->max_nb_items; i++) {
        if (inventory->items[i].id == item.id) { // Pour l'instant l'id est pas unique donc ça suppr pas forcément le bon
            reset_item(inventory, i);
            return true;
        }
    }

    return false;
}


bool remove_item_index_invent(uint32_t entity, int i) {
    InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
    if (!inventory) {
        fprintf(stderr, RED "Entity has no inventory component\n" RESET);
        return false;
    }

    if (i < 0 || i >= inventory->max_nb_items + NB_CST_SLOTS) {
        fprintf(stderr, RED "swap_items_inventory: index out of bounds\n" RESET);
        return false;
    }

    if (i >= inventory->max_nb_items) {
        printf(YELLOW "beware, removing from constant slots\n" RESET);
    }


    if (inventory->selected_slot == i) {
        free_action(inventory->selected_slot_actions);
        free(inventory->selected_slot_actions);
        inventory->selected_slot = -1;
        inventory->selected_slot_actions = NULL;
    }

    if (inventory->items[i].id != -1)
        inventory->nb_items--;

    reset_item(inventory, i);

    return true;
}


bool swap_items_inventory(uint32_t entity, int i, int j) {
    InventoryComponent* inventory = ECS_GetComponent(entity, INVENT);
    if (!inventory) {
        fprintf(stderr, RED "Entity has no inventory component\n" RESET);
        return false;
    }

    if (i < 0 || i >= inventory->max_nb_items + NB_CST_SLOTS || j < 0 || j >= inventory->max_nb_items + NB_CST_SLOTS) {
        fprintf(stderr, RED "swap_items_inventory: index out of bounds" RESET);
        return false;
    }


    ItemData temp = inventory->items[i];
    inventory->items[i] = inventory->items[j];
    inventory->items[j] = temp;

    return true;
}