#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "itemSystem.h"
#include "itemList.h"
#include "../health/healthSystem.h"
#include "../../../display.h"
#include "../ecs.h"
#include "../inventory/inventoryComponent.h"
#include "../inventory/inventorySystem.h"
#include "../../../utils/vector.h"
#include "../systems.h"

SDL_Texture* get_texture_from_Id(enum ItemID id) {
    switch (id)
    {
    case POTION:
        // A FAIRE
        break;
    
    case APPLE:
        return get_sprites()->apple_item_texture;
        break;

    case SWORD:
        return get_sprites()->sword_item_texture; // A CHANGER
        break;

    case KEY:
        // A FAIRE
        break;

    case SHIELD:
        // A FAIRE
        break;

    default:
        return get_sprites()->unknown_item_texture;
        break;
    }

    // Pour que les cas pas encore faits ne fassent pas crash le jeu
    return get_sprites()->unknown_item_texture;
}

Action* get_item_actions(enum ItemID id, slots slot) {
    if (id == -1) return NULL;

    Action* action = malloc(sizeof(Action));

    switch (id) {
    case POTION:
        action->nb_actions = 1;

        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->titles[0] = "consommer";

        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);
        action->functions[0] = item_heal; 
        break;
    
    case APPLE:
        action->nb_actions = 1;
        
        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->titles[0] = "consommer";

        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);
        action->functions[0] = item_heal; 
        break;

    case SWORD:
        action->nb_actions = 1;
        
        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);

		if (slot == NORMAL_SLOT) {
			action->titles[0] = "\xE9\quiper";
			action->functions[0] = weapon_item_equip; 
		} else {
			action->titles[0] = "des\xE9\quiper";
			action->functions[0] = weapon_item_unequip;
		}

        break;

    case KEY:
        action->nb_actions = 0;
        
        action->titles = NULL;

        action->functions = NULL;
        break;

    case SHIELD:
        action->nb_actions = 1;
        
        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->titles[0] = "do nothing";

        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);
        action->functions[0] = item_do_nothing;
        break;

    default:
        action->nb_actions = 0;
        
        action->titles = NULL;

        action->functions = NULL;
        break;
    }


    return action;
}

void free_action(Action* act) {
    if (act == NULL) return;

    free(act->titles);
    free(act->functions);
}

void timer_make_item_grabbable_by_dropper(Timer* self, void* arguments) {
    uint32_t* entityAdress = arguments;
    ItemComponent* item = ECS_GetComponent(*entityAdress, ITEM);

    free_timer(self);
    free(arguments);

    if (!item) {
		return;
	}

    item->isDropperLocked = false;
}

uint32_t add_item_entity(float x, float y, ItemData itemType, uint32_t dropper, bool playerLocked) {
    uint32_t itemEntity = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(itemEntity, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(itemEntity, SPRITE, sizeof(SpriteComponent));
    ItemComponent* itemC = ECS_AddComponent(itemEntity, ITEM, sizeof(ItemComponent));
    RigidbodyComponent* body = ECS_AddComponent(itemEntity, BODY, sizeof(RigidbodyComponent));
    
    itemC->makeDropperUnlocked = NULL;
    itemC->isDropperLocked = playerLocked;
    itemC->dropper = dropper;
    if (dropper != -1 && playerLocked) {

        printf("adding timer !!\n");
    
        uint32_t* arguments = malloc(sizeof(uint32_t));
		*arguments = itemEntity;

        Timer* timer = create_timer(2, timer_make_item_grabbable_by_dropper, arguments);

        itemC->makeDropperUnlocked = timer;
        start_timer(timer);
    }

    init_position_component(position, x, y);

    init_sprite_component(sprite, 64, 64, get_texture_from_Id(itemType.id));
    init_rigidbody_component(body, 0, 0, 64, 64);
    body->is_dynamic = true;
	body->mass = 20;

	void* windArgs = malloc(sizeof(int)*3);
	((float*) windArgs)[0] = 3;
	((float*) windArgs)[1] = 2;
	((float*) windArgs)[2] = 10;
	Force* f = create_force(wind_force, windArgs); 
	add_force(itemEntity, f);

    itemC->isGettable = true;
    itemC->item = itemType;

    return itemEntity;
}

bool update_item(uint32_t entity) {
    ItemComponent* item = ECS_GetComponent(entity, ITEM);
    if (!item) return false;

    return true;
}

bool handle_collision_item(uint32_t entity1, uint32_t entity2) {
    uint32_t receiver, item;

    if (entity1 == entity2) {
        return false;
    }

    if (ECS_HasComponent(entity1, ITEM) && ECS_HasComponent(entity2, INVENT)) {
        item = entity1;
        receiver = entity2;

    } else if (ECS_HasComponent(entity2, ITEM) && ECS_HasComponent(entity1, INVENT)) {
        item = entity2;
        receiver = entity1;

    } else {
        return false;
    }


    ItemComponent* itemComp = ECS_GetComponent(item, ITEM);
    if (!itemComp || (itemComp->isDropperLocked && itemComp->dropper == receiver) || !itemComp->isGettable) {
        return false;
    }

    printf("Transfert ID de dropper: %u, receiver: %u of item : %d\n", itemComp->dropper, receiver, itemComp->item.id);

    if (!transfer_item_into_inventory(item, receiver)) return false;

    free_one_entity(item);

    return true;

}

bool transfer_item_into_inventory(uint32_t itemEntity, uint32_t targetEntity) {
    ItemComponent* item = ECS_GetComponent(itemEntity, ITEM);

    if (!item) {
        return false;
    }

    return add_item_to_inventory(targetEntity, item->item);
}
