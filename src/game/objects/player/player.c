#include "player.h"

uint32_t add_player(float x, float y) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity();
    InventoryComponent* inv = ECS_AddComponent(player, INVENT, sizeof(InventoryComponent));
    PositionComponent* position = ECS_AddComponent(player, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(player, SPRITE, sizeof(SpriteComponent));
    PlayerMovementComponent* movement = ECS_AddComponent(player, PLAYER, sizeof(PlayerMovementComponent));
    AnimationComponent* animation = ECS_AddComponent(player, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(player, BODY, sizeof(RigidbodyComponent));
    ParentComponent* parent = ECS_AddComponent(player, PARENT, sizeof(ParentComponent));
    SwordComponent* sword = ECS_AddComponent(player, SWORD_C, sizeof(SwordComponent));
    create_sword(player, SWORD, 10, 64, 1);
    position->x = 640; position->y = 360;
    position->vx = 0; position->vy = 0;

    // Initialize components
    initialize_inventory(inv, 20, false);
    for (int j  = 0; j < itemCount - 1; j++) {
        add_item_to_inventory(player, *itemList[j]);
    }

    init_rigidbody_component(body, 2, 2, 60, 60);
    body->is_dynamic = true;

    init_health_component(player, 11, 100, 0);
    init_sprite_component(sprite, 64, 64, get_sprites()->player_texture);
    init_anim_component(animation, 16, 16);

    init_parent_component(parent);


    /*uint32_t sword = add_block(0, 0, get_sprites()->sword_item_texture);
    ChildComponent* child = ECS_AddComponent(sword, CHILD, sizeof(SwordComponent));
    
    init_child_component(child, -64, 0, true, player);
    add_child(parent, child);*/


    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    play_anim(animation);

    movement->speed = 5;

    return player;
}
