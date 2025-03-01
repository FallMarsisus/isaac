#include "player.h"

uint32_t add_player(float x, float y, int render_width, int win_width) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity();
    InventoryComponent* inv = ECS_AddComponent(player, INVENT, sizeof(InventoryComponent));
    PositionComponent* position = ECS_AddComponent(player, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(player, SPRITE, sizeof(SpriteComponent));
    AnimationComponent* animation = ECS_AddComponent(player, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(player, BODY, sizeof(RigidbodyComponent));
    ParentComponent* parent = ECS_AddComponent(player, PARENT, sizeof(ParentComponent));
    SwordComponent* swordC = ECS_AddComponent(player, SWORD_C, sizeof(SwordComponent));
    ScriptComponent* script = ECS_AddComponent(player, SCRIPT, sizeof(ScriptComponent));
    HealthComponent* health = ECS_AddComponent(player, HEALTH, sizeof(HealthComponent));

    init_player(script, win_width, render_width);
    
    create_sword(swordC, SWORD, 10, 128, 1);
    init_position_component(position, x, y);

    // Initialize components
    initialize_inventory(inv, 20, false);
    for (int j  = 0; j < itemCount - 1; j++) {
        add_item_to_inventory(player, *itemList[j]);
    }
	add_item_to_inventory(player, sword);
	swap_items_inventory(player, itemCount-1, inv->max_nb_items);

    ID_array* col_layers_mask = create_id_array();
    add_id(col_layers_mask, 0);
    add_id(col_layers_mask, 1);
    add_id(col_layers_mask, 3);

    init_rigidbody_component(body, 2, 2, 60, 60, 2, col_layers_mask);
    body->is_dynamic = true;

    init_health_component(health, 12, 100, 0);
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
