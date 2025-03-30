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

    init_health_component(health, 12);

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

void apply_knockback(uint32_t player, uint32_t enemy) {
    PositionComponent* player_pos = ECS_GetComponent(player, POSITION);
    PositionComponent* enemy_pos = ECS_GetComponent(enemy, POSITION);
    RigidbodyComponent* player_body = ECS_GetComponent(player, BODY);

    if (!player_pos || !enemy_pos || !player_body) return;

    float knockback_strength = 10.0f;
    float dir_x, dir_y;

    if (fabs(player_pos->vx) > 0.1f || fabs(player_pos->vy) > 0.1f) {
        dir_x = player_pos->vx;
        dir_y = player_pos->vy;
    } else {
        dir_x = player_pos->x - enemy_pos->x;
        dir_y = player_pos->y - enemy_pos->y;

        if (dir_x == 0 && dir_y == 0) {
            dir_x = 1.0f; // Default direction if coordinates are the same
        }
    }

    float length = sqrt(dir_x * dir_x + dir_y * dir_y);

    if (length != 0) {
        dir_x /= length;
        dir_y /= length;
    }

    Force* knockback = create_force(knockback_force, NULL);
    knockback->additionalArgs = malloc(3 * sizeof(float));
    ((float*)knockback->additionalArgs)[0] = dir_x;
    ((float*)knockback->additionalArgs)[1] = dir_y;
    ((float*)knockback->additionalArgs)[2] = ceil(knockback_strength);

    add_force(player, knockback);
}
