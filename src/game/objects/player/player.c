#include "player.h"

uint32_t add_player(float x, float y) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity();
    InventoryComponent* inv = ECS_AddComponent(player, INVENT, sizeof(InventoryComponent));
    PositionComponent* position = ECS_AddComponent(player, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(player, SPRITE, sizeof(SpriteComponent));
    AnimationComponent* animation = ECS_AddComponent(player, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(player, BODY, sizeof(RigidbodyComponent));
    ParentComponent* parent = ECS_AddComponent(player, PARENT, sizeof(ParentComponent));
    SwordComponent* sword = ECS_AddComponent(player, SWORD_C, sizeof(SwordComponent));
    ScriptComponent* script = ECS_AddComponent(player, SCRIPT, sizeof(ScriptComponent));

    init_player(script);
    
    create_sword(sword, SWORD, 10, 64, 1);
    init_position_component(position, x, y);

    // Initialize components
    initialize_inventory(inv, 20, false);
    for (int j  = 0; j < itemCount - 1; j++) {
        add_item_to_inventory(player, *itemList[j]);
    }

    init_rigidbody_component(body, 2, 2, 60, 60);
    body->is_dynamic = true;

    init_health_component(player, 12, 100, 0);
    init_sprite_component(sprite, 64, 64, get_sprites()->player_texture);
    sprite->layer = 1;
    
    init_anim_component(animation, 16, 16);

    init_parent_component(parent);

    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 1);
    play_anim(animation);

    return player;
}
