#include "player.h"

uint32_t add_player(float x, float y) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(player, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(player, SPRITE, sizeof(SpriteComponent));
    PlayerMovementComponent* movement = ECS_AddComponent(player, PLAYER, sizeof(PlayerMovementComponent));
    AnimationComponent* animation = ECS_AddComponent(player, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(player, BODY, sizeof(RigidbodyComponent));
    ParentComponent* parent = ECS_AddComponent(player, PARENT, sizeof(ParentComponent));
    InventoryComponent* inv = ECS_AddComponent(player, INVENT, sizeof(InventoryComponent));
    
    // Initialize components
    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;
    inv->max_nb_items = 50;
    inv->item_ids = malloc(sizeof(int) * inv->max_nb_items);
    add_item_to_inventory(player, 5);
    add_item_to_inventory(player, 2);
    add_item_to_inventory(player, 2187);
    add_item_to_inventory(player, 87498);

    init_rigidbody_component(body, 2, 2, 60, 60);
    body->is_dynamic = true;

    init_health_component(player, 11, 100, 0);
    init_sprite_component(sprite, 64, 64, get_sprites()->player_texture);
    init_anim_component(animation, 16, 16);

    init_parent_component(parent);

    for (int j  = 0; j < itemCount - 1; j++) {
        create_item(player, *itemList[j]);
    }

    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    play_anim(animation);

    movement->speed = 5;

    return player;
}
