#include "itemSystem.h"
#include "itemList.h"
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
        return get_sprites()->sword_slash; // A CHANGER
        break;

    case KEY:
        // A FAIRE
        break;

    case SHIELD:
        // A FAIRE
        break;

    default:
        return get_sprites()->apple_item_texture;
        break;
    }

    // Pour que les cas pas encore faits ne fassent pas crash le jeu
    return get_sprites()->apple_item_texture;
}

bool update_item(uint32_t entity, uint32_t* other_entitites, int nb_entities) {
    if (!ECS_GetComponent(entity, ITEM)) return false; // If entity is not an item

    InventoryComponent* invent;
    PositionComponent* position;
    PositionComponent* selfPosition = ECS_GetComponent(entity, POSITION);

    if (!selfPosition) return false;

    for (int i = 0; i < nb_entities; i++) {
        invent = ECS_GetComponent(other_entitites[i], INVENT);
        position = ECS_GetComponent(other_entitites[i], POSITION);
        

        if (!invent || !position) {
            continue;
        }


        float distance = sqrt((position->x - selfPosition->x) * (position->x - selfPosition->x) 
            + (position->y - selfPosition->y) * (position->y - selfPosition->y)); 

        if (distance <= 10) {
            return transfer_item_into_inventory(entity, other_entitites[i]);
        }

        // faudra ajouter un event je pense mais là j'ai la flemme

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
