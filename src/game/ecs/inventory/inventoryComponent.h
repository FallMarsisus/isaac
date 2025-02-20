#pragma once

#include <stdint.h>
#include "../items/itemData.h"
#include <stdbool.h>

// fct de type void en sortie et entity en argument
typedef void (*ActionFunction)(uint32_t entity, ItemData currentItem);

// faudra voir si je mets ça dans l'invent ou dans chaque item
typedef struct {
	int nb_actions;
	char** titles;
	ActionFunction* functions;
} Action;

typedef struct {
	bool isDisplayed;
	int max_nb_items;
	int nb_items;
	int selected_slot;
	Action* selected_slot_actions;
	ItemData* items;
	ItemData handSlot;
	ItemData ArmorSlot;	
} InventoryComponent;

