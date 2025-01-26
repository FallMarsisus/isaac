#include "itemSystem.h"

bool create_item(uint32_t entity, ItemData itemC) {
    ItemData *item = ECS_AddComponent(entity, ITEM, sizeof(ItemData));
    item->id = itemC.id; ;
    item->name = itemC.name;
    item->description = itemC.description;
    item->value = itemC.value;
    return true;
}

bool destroy_item(uint32_t entity, ItemData itemC)
{
    // TODO: Implement this function
    return false;    
}

ItemData get_item_by_id(uint32_t entity, int id)
{
    ItemData *item = ECS_GetComponent(entity, ITEM);
    for (int i = 0; i < 50; i++)
    {
        if (item->id == id)
        {
            return *item;
        }
    }
    return (ItemData){-1, "NULL", "NULL", 0};
}
