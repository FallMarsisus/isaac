#include "systems.h"

// Initialize the game with entities and components
uint32_t initialize_game(ECS_Manager* ecs) {
    uint32_t player = add_player(ecs, 100, 200);

    for(int i = 0; i < 20; i++) {
        add_enemy(ecs, random_int(-5000, 5000), random_int(-5000, 5000), player);
        add_block(ecs, random_int(-5000, 5000), random_int(-5000, 5000));
        
        float x1 = random_int(-5000, 5000), y1 = random_int(-5000, 5000), 
              x2 = random_int(-5000, 5000), y2 = random_int(-5000, 5000);
        add_teleporter(ecs, x1, y1, x2, y2);
        add_teleporter(ecs, x2, y2, x1, y1);
    }

    return player;
}

void free_components(ECS_Manager* ecs) {
    for (size_t i = 0; i < ecs->count; ++i) {
        free_all_render_components(ecs, ecs->entity_ids[i]);
    }
}

uint32_t add_player(ECS_Manager* ecs, float x, float y) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity(ecs);
    PositionComponent* position = ECS_AddComponent(ecs, player, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(ecs, player, SPRITE, sizeof(SpriteComponent));
    PlayerMovementComponent* movement = ECS_AddComponent(ecs, player, PLAYER, sizeof(PlayerMovementComponent));
    AnimationComponent* animation = ECS_AddComponent(ecs, player, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(ecs, player, BODY, sizeof(RigidbodyComponent));

    // Initialize components
    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;

    init_rigidbody_component(body, 64, 64);
    body->is_dynamic = true;

    init_sprite_component(sprite, 64, 64, get_sprites()->player_texture);
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    play_anim(animation);

    movement->speed = 5;

    return player;
}

uint32_t add_enemy(ECS_Manager* ecs, float x, float y, uint32_t pl) {
    uint32_t enemy = ECS_CreateEntity(ecs);
    PositionComponent* position = ECS_AddComponent(ecs, enemy, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(ecs, enemy, SPRITE, sizeof(SpriteComponent));
    TargetMovementComponent* target = ECS_AddComponent(ecs, enemy, TARGET, sizeof(TargetMovementComponent));
    AnimationComponent* animation = ECS_AddComponent(ecs, enemy, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(ecs, enemy, BODY, sizeof(RigidbodyComponent));

    // Initialize components
    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;

    init_rigidbody_component(body, 64, 64);
    body->is_dynamic = true;

    init_target_component(target, pl);

    init_sprite_component(sprite, 64, 64, get_sprites()->goblin_texture);
    init_anim_component(animation, 16, 16);

    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 4);
    add_anim(animation, 0.1, 2);
    add_anim(animation, 0.1, 2);

    set_active_anim(animation, 0);
    play_anim(animation);

    return enemy;
}

uint32_t add_block(ECS_Manager* ecs, float x, float y) {
    uint32_t block = ECS_CreateEntity(ecs);
    PositionComponent* position = ECS_AddComponent(ecs, block, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(ecs, block, SPRITE, sizeof(SpriteComponent));
    RigidbodyComponent* body = ECS_AddComponent(ecs, block, BODY, sizeof(RigidbodyComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;

    init_rigidbody_component(body, 64, 64);

    init_sprite_component(sprite, 64, 64, get_sprites()->cobble_texture);

    return block;
}

uint32_t add_teleporter(ECS_Manager* ecs, float x, float y, float xTarget, float yTarget) {
    uint32_t obj = ECS_CreateEntity(ecs);
    PositionComponent* position = ECS_AddComponent(ecs, obj, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(ecs, obj, SPRITE, sizeof(SpriteComponent));
    TeleporterComponent* teleport = ECS_AddComponent(ecs, obj, TELEPORT, sizeof(TeleporterComponent));
    ScriptComponent* script = ECS_AddComponent(ecs, obj, SCRIPT, sizeof(ScriptComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    init_sprite_component(sprite, 64, 64, get_sprites()->teleporter_texture);
    init_script_component(script, update_teleporter);

    teleport->posX = xTarget; teleport->posY = yTarget;

    return obj;
}

// Handle input for player movement
void handle_input_system(ECS_Manager* ecs, SDL_Event* event) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    int dx = 0, dy = 0;
    if(state[SDL_SCANCODE_W]) dy -= 1;
    if(state[SDL_SCANCODE_S]) dy += 1;
    if(state[SDL_SCANCODE_A]) dx -= 1;
    if(state[SDL_SCANCODE_D]) dx += 1;

    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    for (size_t i = 0; i < ecs->count; ++i) {
        PlayerMovementComponent* movement = ECS_GetComponent(ecs, ecs->entity_ids[i], PLAYER);
        PositionComponent* position = ECS_GetComponent(ecs, ecs->entity_ids[i], POSITION);
        AnimationComponent* anim = ECS_GetComponent(ecs, ecs->entity_ids[i], ANIMATION);

        if (movement && position) {
            if(distance > 0.01) {
                position->vx = (dx / distance) * movement->speed;
                position->vy = (dy / distance) * movement->speed;
                if(anim) {
                    if(dy < 0) set_active_anim(anim, 1);
                    else if(dy > 0) set_active_anim(anim, 0);
                    else if(dx < 0) set_active_anim(anim, 2);
                    else if(dx > 0) set_active_anim(anim, 3);
                    play_anim(anim);
                }
            }
            else {
                position->vx = 0; position->vy = 0;
                if(anim) {
                    stop_anim(anim);
                }
            }
        }
    }
}

// Update all systems
void update_systems(ECS_Manager* ecs, dyn_array* entities, SDL_Rect cam) {
    for(int i = 0; i < get_len(entities); i++) {
        u_int32_t id = *((uint32_t*) get_elt(entities, i));
        
        update_others(id, ecs);
        update_physics(id, ecs);
    }
}

// Render all entities
void render_systems(ECS_Manager* ecs, dyn_array* entities, SDL_Rect cam, SDL_Renderer* renderer) {
    for(int i = 0; i < get_len(entities); i++) {
        u_int32_t id = *((uint32_t*) get_elt(entities, i));
        
        render_component(id, ecs, cam, renderer);
    }
}
