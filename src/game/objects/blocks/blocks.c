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

uint32_t add_effect(float x, float y, SDL_Texture* texture) {
    uint32_t effect = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(effect, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(effect, SPRITE, sizeof(SpriteComponent));
    EffectComponent* effectComp = ECS_AddComponent(effect, EFFECT, sizeof(EffectComponent));

    init_effect_component(effectComp, effect, 10, 0.5);

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_sprite_component(sprite, 64, 64, texture);

    return effect;
}

uint32_t add_background_tile(float x, float y) {
    uint32_t block = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(block, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(block, SPRITE, sizeof(SpriteComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = true;

    init_sprite_component(sprite, 64, 64, get_sprites()->background_texture);

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

bool is_colliding_with_chest(uint32_t entity) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);

    for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
        if(e == entity) continue;
        PositionComponent* chest_pos = ECS_GetComponent(e, POSITION);
        SpriteComponent* chest_sprite = ECS_GetComponent(e, SPRITE);
        
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
                    event->chest_id = e;
                    event->player_id = entity;
                    event->x = chest_pos->x; event->y = chest_pos->y;
                    trigger_event(EVENT_CHEST_OPENED, event, true);
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
    
    // Add outer walls
    for(int x = start_x; x < start_x + 1280; x += 64) {
        if(start_x + door_x * 64 <= x && x <= (door_x + 1) * 64 + start_x) continue;
        add_block(x, start_y, get_sprites()->cobble_texture);
    }
    for(int y = start_y; y < start_y + 720; y += 64) {
        if(start_y + door_y * 64 <= y && y <= (door_y + 1) * 64 + start_y) continue;
        add_block(start_x, y, get_sprites()->cobble_texture);
    }
    // Add random inner walls
    int num_inner_walls = rand() % 5 + 3; // Random number of inner walls between 3 and 8
    for (int i = 0; i < num_inner_walls; i++) {
        int wall_x, wall_y;
        bool horizontal = rand() % 2;
        int wall_length = rand() % 2 + 3; // Random wall length between 3 and 5 blocks

        if (horizontal) {
            wall_x = start_x + (rand() % (1280 / 64 - wall_length - 2) + 1) * 64;
            wall_y = start_y + (rand() % (720 / 64 - 2) + 1) * 64;
        } else {
            wall_x = start_x + (rand() % (1280 / 64 - 2) + 1) * 64;
            wall_y = start_y + (rand() % (720 / 64 - wall_length - 2) + 1) * 64;
        }

        for (int j = 0; j < wall_length; j++) {
            if (horizontal) {
                if (wall_x + j * 64 < start_x + 1280 && !(start_x + door_x * 64 <= wall_x + j * 64 && wall_x + j * 64 <= (door_x + 1) * 64 + start_x)) {
                    add_block(wall_x + j * 64, wall_y, get_sprites()->cobble_texture);
                }
            } else {
                if (wall_y + j * 64 < start_y + 720 && !(start_y + door_y * 64 <= wall_y + j * 64 && wall_y + j * 64 <= (door_y + 1) * 64 + start_y)) {
                    add_block(wall_x, wall_y + j * 64, get_sprites()->cobble_texture);
                }
            }
        }
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
