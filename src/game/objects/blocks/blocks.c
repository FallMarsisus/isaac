#include "blocks.h"

uint32_t add_tile(float x, float y, int tile_x, int tile_y, SDL_Texture* tileset_texture, bool has_collision) {
    uint32_t tile = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(tile, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(tile, SPRITE, sizeof(SpriteComponent));
    TileComponent* tile_comp = ECS_AddComponent(tile, TILE, sizeof(TileComponent));

    init_position_component(position, x, y);
    init_sprite_component(sprite, 64, 64, tileset_texture);
    init_tile_component(tile_comp, tile_x, tile_y, 16, 16);

    if(has_collision) {
        RigidbodyComponent* body = ECS_AddComponent(tile, BODY, sizeof(RigidbodyComponent));
        init_rigidbody_component(body, 2, 2, 60, 60);
    }

    return tile;
}

uint32_t add_background_tile(float x, float y) {
    uint32_t block = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(block, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(block, SPRITE, sizeof(SpriteComponent));

    init_position_component(position, x, y);

    init_sprite_component(sprite, 64, 64, get_sprites()->background_texture);

    return block;
}

uint32_t add_chest(float x, float y) {
    uint32_t chest = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(chest, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(chest, SPRITE, sizeof(SpriteComponent));

    init_position_component(position, x, y);

    init_sprite_component(sprite, 64, 64, get_sprites()->chest_closed_texture);

    return chest;
}

uint32_t add_teleporter(float x, float y, float xTarget, float yTarget) {
    uint32_t obj = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(obj, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(obj, SPRITE, sizeof(SpriteComponent));
    TeleporterComponent* teleport = ECS_AddComponent(obj, TELEPORT, sizeof(TeleporterComponent));
    ScriptComponent* script = ECS_AddComponent(obj, SCRIPT, sizeof(ScriptComponent));

    init_position_component(position, x, y);
    
    init_sprite_component(sprite, 64, 64, get_sprites()->teleporter_texture);
    init_script_component(script, update_teleporter);

    teleport->posX = xTarget; teleport->posY = yTarget;

    return obj;
}

uint32_t add_door(float x, float y) {
    uint32_t door = add_tile(x, y, 0, 0, get_sprites()->tileset_texture, true);
    return door;
}

uint32_t add_trap(float x, float y) {
    uint32_t trap = add_tile(x, y, 4, 1, get_sprites()->tileset_texture, true);
    ScriptComponent* script = ECS_AddComponent(trap, SCRIPT, sizeof(ScriptComponent));
    init_script_component(script, update_trap);

    AnimationComponent* anim = ECS_AddComponent(trap, ANIMATION, sizeof(AnimationComponent));
    init_anim_component(anim, 16, 16);

    add_anim_tile(anim, 4, 1, 1, 2);

    set_active_anim(anim, 0);
    play_anim(anim);

    return trap;
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
