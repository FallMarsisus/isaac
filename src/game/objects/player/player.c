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
    initialize_inventory(inv, 50, false);
    for (int j  = 0; j < itemCount - 1; j++) {
        add_item_to_inventory(player, *itemList[j]);
    }

    init_rigidbody_component(body, 2, 2, 60, 60);
    body->is_dynamic = true;

    init_health_component(player, 11, 100, 0);
    init_sprite_component(sprite, 64, 64, get_sprites()->player_texture);
    init_anim_component(animation, 16, 16);

    init_parent_component(parent);


    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    play_anim(animation);

    movement->speed = 5;

    return player;
}
