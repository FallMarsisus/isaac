#include "itemsFunctions.h"
#include "../inventory/inventorySystem.h"
#include "./throwScripts.h"
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
	add_item_entity(pos->x+5, pos->y+5, currentItem, entity, true); //+5 to avoid clipping
}

void activateScriptOnTimerEnd(Timer* timer, void* user_data) {
	uint32_t entity = *(uint32_t*)user_data;
	ItemComponent* item = ECS_GetComponent(entity, ITEM);
	if (!item) {
		printf(RED "Error: Item component not found for entity %d\n" RESET, entity);
		return;
	}
	
	ScriptComponent* script = ECS_AddComponent(entity, SCRIPT, sizeof(ScriptComponent));

	script->data = item->item.throwProp->script->data;
	script->update = item->item.throwProp->script->update;

	free(user_data);
}

void throwItemAtMouse(uint32_t player, int itemIndex, SDL_Rect cam, int win_width, int true_width) {
	SpriteComponent* sprite = ECS_GetComponent(player, SPRITE);
    PositionComponent* pos = ECS_GetComponent(player, POSITION);

	if (!pos || !sprite) return;

	int mouseX, mouseY;
	float scaleFactor = (float) true_width / win_width;
	SDL_GetMouseState(&mouseX, &mouseY);

	int adjustedPlayerPosX = (int)((pos->x - cam.x + sprite->width/2)*scaleFactor);
	int adjustedPlayerPosY = (int)((pos->y - cam.y + sprite->height/2)*scaleFactor);

    Vector* throwDirection = malloc(sizeof(Vector));

	throwDirection->x = mouseX - adjustedPlayerPosX;
	throwDirection->y = mouseY - adjustedPlayerPosY;

	throwItem(player, itemIndex, throwDirection);

	free(throwDirection);
}

void throwItem(uint32_t player, int itemIndex, Vector* throwDirection) {
    float force = 10; //norm of speed of resulting item entity

    PositionComponent* pos = ECS_GetComponent(player, POSITION);
    InventoryComponent* invent = ECS_GetComponent(player, INVENT);
	SpriteComponent* sprite = ECS_GetComponent(player, SPRITE);
	
	if (!invent || !pos) return;

    normalize(throwDirection);
    vector_multiply(throwDirection, force);
    
    ItemData itemD = invent->items[itemIndex];
    if(!remove_item_index_invent(player, itemIndex)) {
		printf(RED "failed to remove item from inventory :(" RESET);
		return;
	}

    uint32_t itemEntity = add_item_entity(pos->x+5, pos->y+5, itemD, player, true);

	free_itemData(&itemD);

    ItemComponent* item = ECS_GetComponent(itemEntity, ITEM);
    PositionComponent* itemPos = ECS_GetComponent(itemEntity, POSITION);
    RigidbodyComponent* itemBody = ECS_GetComponent(itemEntity, BODY);

    if (!item || !itemBody || !itemBody) return;

	add_id(itemBody->layer_that_collides_with, 0);

    if (item->item.throwProp != NULL) {
		uint32_t* argument = malloc(sizeof(uint32_t));
		*argument = itemEntity;
        Timer* timer = create_timer(item->item.throwProp->timeBeforeScriptActivation, activateScriptOnTimerEnd, argument);
        start_timer(timer);
    }

    itemPos->vx = throwDirection->x;
    itemPos->vy = throwDirection->y;

    if (get_len(itemBody->forces) > 0) {
        pop(itemBody->forces); //pour suppr le vent de con que j'ai ajouté
    }

    sdfArgs* dragCoef = malloc(sizeof(sdfArgs));
	dragCoef->movingCoef = 150;
	dragCoef->staticCoef = 500;
	dragCoef->lastSignX = 0;
	dragCoef->timesOsciliatingX = 0;
	dragCoef->lastSignY = 0;
	dragCoef->timesOsciliatingY = 0;
	printf("adding force de con\n");
    Force* f = create_force(solid_drag_force, dragCoef);
    add_force(itemEntity, f);
}

void weapon_item_equip(uint32_t entity, ItemData currentItem) {
	// printf("equiping item : %d\n", currentItem.id);
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (!invent) return;

	swap_items_inventory(entity, invent->selected_slot, invent->max_nb_items);

	unselectItem(entity, currentItem);
}

void weapon_item_unequip(uint32_t entity, ItemData currentItem) {
	InventoryComponent* invent = ECS_GetComponent(entity, INVENT);

	if (!invent) return;

	if (!add_item_to_inventory(entity, currentItem)) return;

	remove_item_index_invent(entity, invent->selected_slot);

	unselectItem(entity, currentItem);
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