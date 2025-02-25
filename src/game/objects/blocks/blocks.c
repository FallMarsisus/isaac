#include "blocks.h"

uint32_t add_tile(float x, float y, int tile_x, int tile_y, SDL_Texture* tileset_texture, bool has_collision, int layer) {
    uint32_t tile = ECS_CreateEntity();

	
	if (tile == 137) {
		printf("tile ID is 137\n");
	}

    PositionComponent* position = ECS_AddComponent(tile, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(tile, SPRITE, sizeof(SpriteComponent));
    TileComponent* tile_comp = ECS_AddComponent(tile, TILE, sizeof(TileComponent));

    init_position_component(position, x, y);
    init_sprite_component(sprite, 64, 64, tileset_texture);
    sprite->layer = layer;
    init_tile_component(tile_comp, tile_x, tile_y, 16, 16);

    RigidbodyComponent* body = ECS_AddComponent(tile, BODY, sizeof(RigidbodyComponent));
    init_rigidbody_component(body, 2, 2, 60, 60);
    body->colliding = has_collision;

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
    ScriptComponent* script = ECS_AddComponent(obj, SCRIPT, sizeof(ScriptComponent));

    init_position_component(position, x, y);
    init_sprite_component(sprite, 64, 64, get_sprites()->teleporter_texture);
    init_teleporter(script, xTarget, yTarget);

    return obj;
}
uint32_t add_door(float x, float y) {
    uint32_t door = add_tile(x, y, 0, 0, get_sprites()->tileset_texture, true, 0);

	
    return door;
}
uint32_t add_trap(float x, float y) {
    uint32_t trap = add_tile(x, y, 4, 1, get_sprites()->tileset_texture, false, 0);
    ScriptComponent* script = ECS_AddComponent(trap, SCRIPT, sizeof(ScriptComponent));
    init_trap(script);
    
    return trap;
}

bool is_colliding_with_chest(uint32_t entity, uint32_t* entities, int amount) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);

    for (int i = 0; i < amount; i++) {
        if(entities[i] == entity) continue;
        PositionComponent* chest_pos = ECS_GetComponent(entities[i], POSITION);
        SpriteComponent* chest_sprite = ECS_GetComponent(entities[i], SPRITE);
        
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
                    event->chest_id = entities[i];
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
