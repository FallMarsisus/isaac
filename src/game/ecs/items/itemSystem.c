#include <math.h>
#include <stdlib.h>
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

Action* get_item_actions(enum ItemID id) {
    Action* action = malloc(sizeof(Action));
    switch (id) {
    case POTION:
        action->nb_actions = 1;

        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->titles[0] = "consommer";

        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);
        action->functions[0] = itemHeal; 
        break;
    
    case APPLE:
        action->nb_actions = 1;
        
        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->titles[0] = "consommer";

        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);
        action->functions[0] = itemHeal; 
        break;

    case SWORD:
        action->nb_actions = 1;
        
        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->titles[0] = "équiper";

        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);
        action->functions[0] = itemDoNothing; 
        break;

    case KEY:
        action->nb_actions = 0;
        
        action->titles = NULL;

        action->functions = NULL;
        break;

    case SHIELD:
        action->nb_actions = 1;
        
        action->titles = malloc(sizeof(char*) * action->nb_actions);
        action->titles[0] = "équiper";

        action->functions = malloc(sizeof(ActionFunction) * action->nb_actions);
        action->functions[0] = itemDoNothing;
        break;

    default:
        action->nb_actions = 0;
        
        action->titles = NULL;

        action->functions = NULL;
        break;
    }


    return action;
}


void freeAction(Action* act) {
    if (act == NULL) return;

    free(act->titles);
    free(act->functions);
}

bool update_item(uint32_t entity, uint32_t player, uint32_t* other_entities, int nb_entities) {
    /**
     * @deprecated This function is deprecated and may be removed in future versions.
     * Please use the listeners instead.
     */


    ItemComponent* itemComponent = ECS_GetComponent(entity, ITEM);
    if (!itemComponent) {
        // printf("DEBUG: Entity %u is not an item\n", entity);
        return false;
    }

    PositionComponent* selfPosition = ECS_GetComponent(entity, POSITION);

    if (!selfPosition) {
        // printf("DEBUG: Item entity %u has no position\n", entity);
        return false;
    }

    // printf("DEBUG: Checking item at position (%.2f, %.2f)\n", selfPosition->x, selfPosition->y);

    for (int i = 0; i < nb_entities + 1; i++) {
        int id;
        if(i = nb_entities) id = player;
        else id = other_entities[i];

        if (id == entity) {
            // printf("DEBUG: Entity %u skipped (self)\n", id);
            continue;
        }

        InventoryComponent* invent = ECS_GetComponent(id, INVENT);
        PositionComponent* position = ECS_GetComponent(id, POSITION);
        
        if (!invent || !position) {
            // printf("DEBUG: Entity %u skipped (no inventory or position)\n", id);
            continue;
        }

        float dx = position->x - selfPosition->x;
        float dy = position->y - selfPosition->y;
        float distance = sqrt(dx * dx + dy * dy);

        // printf("DEBUG: Distance to entity %u: %.2f\n", id, distance);

        if (distance <= 50.0f) {
            // printf("DEBUG: Attempting transfer to entity %u\n", id);
            if (transfer_item_into_inventory(entity, id)) {
                // printf("DEBUG: Transfer successful\n");
                return true;
            } else {
                // printf("DEBUG: Transfer failed\n");
            }
        }
    }

    return false;
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

    printf("handeling\n");
    if (!transfer_item_into_inventory(item, receiver)) return false;

    free_one_entity(item);
    ECS_RemoveEntity(item);

    return true;

}

bool transfer_item_into_inventory(uint32_t itemEntity, uint32_t targetEntity) {
    ItemComponent* item = ECS_GetComponent(itemEntity, ITEM);

    if (!item) {
        return false;
    }

    return add_item_to_inventory(targetEntity, item->item);
}
