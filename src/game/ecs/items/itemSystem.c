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

bool update_item(uint32_t entity, uint32_t* other_entities, int nb_entities) {
    ItemComponent* itemComponent = ECS_GetComponent(entity, ITEM);
    if (!itemComponent) {
        // printf("DEBUG: Entity %u is not an item\n", entity);
        return false;
    }
    
    InventoryComponent* invent;
    PositionComponent* position;
    PositionComponent* selfPosition = ECS_GetComponent(entity, POSITION);

    if (!selfPosition) {
        // printf("DEBUG: Item entity %u has no position\n", entity);
        return false;
    }

    // printf("DEBUG: Checking item at position (%.2f, %.2f)\n", selfPosition->x, selfPosition->y);

    for (int i = 0; i < nb_entities; i++) {
        invent = ECS_GetComponent(other_entities[i], INVENT);
        position = ECS_GetComponent(other_entities[i], POSITION);
        
        if (!invent || !position) {
            // printf("DEBUG: Entity %u skipped (no inventory or position)\n", other_entities[i]);
            continue;
        }

        float dx = position->x - selfPosition->x;
        float dy = position->y - selfPosition->y;
        float distance = sqrt(dx * dx + dy * dy);

        // printf("DEBUG: Distance to entity %u: %.2f\n", other_entities[i], distance);

        if (distance <= 50.0f) {
            // printf("DEBUG: Attempting transfer to entity %u\n", other_entities[i]);
            if (transfer_item_into_inventory(entity, other_entities[i])) {
                // printf("DEBUG: Transfer successful\n");
                return true;
            } else {
                // printf("DEBUG: Transfer failed\n");
            }
        }
    }

    return false;
}

bool transfer_item_into_inventory(uint32_t itemEntity, uint32_t targetEntity) {
    ItemComponent* item = ECS_GetComponent(itemEntity, ITEM);

    if (!item) {
        return false;
    }

    return add_item_to_inventory(targetEntity, item->item);
}
