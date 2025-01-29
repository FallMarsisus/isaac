#include "blocks.h"

uint32_t add_block(float x, float y, SDL_Texture* texture) {
    uint32_t block = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(block, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(block, SPRITE, sizeof(SpriteComponent));
    RigidbodyComponent* body = ECS_AddComponent(block, BODY, sizeof(RigidbodyComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_rigidbody_component(body, 2, 2, 60, 60);

    init_sprite_component(sprite, 64, 64, texture);

    return block;
}

uint32_t add_chest(float x, float y) {
    uint32_t chest = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(chest, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(chest, SPRITE, sizeof(SpriteComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_sprite_component(sprite, 64, 64, get_sprites()->chest_closed_texture);

    return chest;
}


/*
bool is_colliding_with_item(uint32_t entity) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        if (ECS_GetManager()->entity_ids[i] == entity) continue;
        PositionComponent* item_pos = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
        SpriteComponent* item_sprite = ECS_GetComponent(ECS_GetManager()->entity_ids[i], SPRITE);
        
        if (item_sprite) {
            if (pos && item_pos) {
                float dx = pos->x - item_pos->x;
                float dy = pos->y - item_pos->y;
                float distance = sqrt(dx*dx + dy*dy);
                
                if (distance < 32) { // Assuming 64 is collision radius
                    
                    free_sprite_component(item_sprite);
                    return true;
                }
            }
        }
    }
    return false;
}
*/

bool is_colliding_with_chest(uint32_t entity) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        if (ECS_GetManager()->entity_ids[i] == entity) continue;
        PositionComponent* chest_pos = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
        SpriteComponent* chest_sprite = ECS_GetComponent(ECS_GetManager()->entity_ids[i], SPRITE);
        
        // Check if entity is a chest by checking its texture
        if (chest_sprite && chest_sprite->texture == get_sprites()->chest_closed_texture) {
            if (pos && chest_pos) {
                // Simple distance check for collision
                float dx = pos->x - chest_pos->x;
                float dy = pos->y - chest_pos->y;
                float distance = sqrt(dx*dx + dy*dy);
                
                if (distance < 64) { // Assuming 64 is collision radius
                    // Change chest texture to opened
                    chest_sprite->texture = get_sprites()->chest_opened_texture;
                    ChestOpenedEvent* event = malloc(sizeof(ChestOpenedEvent));
                    event->chest_id = ECS_GetManager()->entity_ids[i];
                    event->player_id = entity;
                    event->x = chest_pos->x; event->y = chest_pos->y;
                    trigger_event(EVENT_CHEST_OPENED, event);
                    return true;
                }
            }
        }
    }
    return false;
}

uint32_t add_blocks(int rX, int rY) {
    int start_x = rX * 1280; int start_y = rY * 720;
    int door_x = rand() % GRID_WIDTH; int door_y = rand() % GRID_HEIGHT;
    for(int x = start_x; x < start_x + 1280; x += 64) {
        if(start_x + door_x * 64 <= x && x <= (door_x + 1) * 64 + start_x) continue;
        add_block(x, start_y, get_sprites()->cobble_texture);
    }
    for(int y = start_y; y < start_y  + 720; y += 64) {
        if(start_y + door_y * 64 <= y && y <= (door_y + 1) * 64 + start_y) continue;
        add_block(start_x, y, get_sprites()->cobble_texture);
    }
    return -1;
}

uint32_t add_teleporter(float x, float y, float xTarget, float yTarget) {
    uint32_t obj = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(obj, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(obj, SPRITE, sizeof(SpriteComponent));
    TeleporterComponent* teleport = ECS_AddComponent(obj, TELEPORT, sizeof(TeleporterComponent));
    ScriptComponent* script = ECS_AddComponent(obj, SCRIPT, sizeof(ScriptComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;
    init_sprite_component(sprite, 64, 64, get_sprites()->teleporter_texture);
    init_script_component(script, update_teleporter);

    teleport->posX = xTarget; teleport->posY = yTarget;

    return obj;
}
