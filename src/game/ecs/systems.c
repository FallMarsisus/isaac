#include "systems.h"

// Initialize the game with entities and components
uint32_t initialize_game() {
    uint32_t player = add_player(100, 200);
    add_enemy(500, 200, player);
    add_blocks(0, 0);
    add_enemy(-200, 300, player);
    add_blocks(-1, 0);

    for(int i = 0; i < 20; i++) {
        add_blocks(random_int(-5, 5), random_int(-5, 5));
        add_enemy(random_int(-5000, 5000), random_int(-5000, 5000), player);
        add_chest(random_int(-5000, 5000), random_int(-5000, 5000));
        add_block(random_int(-5000, 5000), random_int(-5000, 5000), get_sprites()->cobble_texture);
        
        float x1 = random_int(-5000, 5000), y1 = random_int(-5000, 5000), 
              x2 = random_int(-5000, 5000), y2 = random_int(-5000, 5000);
        add_teleporter(x1, y1, x2, y2);
        add_teleporter(x2, y2, x1, y1);
    }

    return player;
}

void free_components() {
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        free_pathfinding_component(ECS_GetComponent(ECS_GetManager()->entity_ids[i], PATHFINDING));
        free_all_other_components(ECS_GetManager()->entity_ids[i]);
        free_all_render_components(ECS_GetManager()->entity_ids[i]);
    }
}

uint32_t add_player(float x, float y) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity(ECS_GetManager());
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

    movement->speed = 10;

    return player;
}

bool is_colliding_with_enemy(uint32_t entity) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    RigidbodyComponent* body = ECS_GetComponent(entity, BODY);
    
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        if (ECS_GetManager()->entity_ids[i] == entity) continue;
        PositionComponent* enemy_pos = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
        RigidbodyComponent* enemy_body = ECS_GetComponent(ECS_GetManager()->entity_ids[i], BODY);
        DamagerComponent* damager = ECS_GetComponent(ECS_GetManager()->entity_ids[i], DAMAGER);
        if( damager == NULL) continue;
        if (isColliding(pos, body, enemy_pos, enemy_body)) return true;
        
    }
    return false ;
}
bool is_colliding_with_chest(uint32_t entity) {
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        if (ECS_GetManager()->entity_ids[i] == entity) continue;
        PositionComponent* chest_pos = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
        SpriteComponent* chest_sprite = ECS_GetComponent(ECS_GetManager()->entity_ids[i], SPRITE);
        
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
                    event->chest_id = ECS_GetManager()->entity_ids[i];
                    event->player_id = entity;
                    event->x = chest_pos->x; event->y = chest_pos->y;
                    trigger_event(EVENT_CHEST_OPENED, event);
                    return true;
                }
            }
        }
    }
    return false;
}

uint32_t get_nearest_enemy(uint32_t entity)
{
    PositionComponent* pos = ECS_GetComponent(entity, POSITION);
    int min_dist = 1000;
    int nearest_enemy = -1;
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        if (ECS_GetManager()->entity_ids[i] == entity) continue;
        
        
        PositionComponent* enemy_pos = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
        if (enemy_pos) {
            int dist = sqrt(pow(pos->x - enemy_pos->x, 2) + pow(pos->y - enemy_pos->y, 2));
            if (dist < min_dist) {
                min_dist = dist;
                nearest_enemy = ECS_GetManager()->entity_ids[i];
            }
        }
    }
    return nearest_enemy;
}

uint32_t add_enemy(float x, float y, uint32_t pl) {
    uint32_t enemy = ECS_CreateEntity(ECS_GetManager());
    PositionComponent* position = ECS_AddComponent(enemy, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(enemy, SPRITE, sizeof(SpriteComponent));
    PathfindingComponent* target = ECS_AddComponent(enemy, PATHFINDING, sizeof(PathfindingComponent));
    AnimationComponent* animation = ECS_AddComponent(enemy, ANIMATION, sizeof(AnimationComponent));
    RigidbodyComponent* body = ECS_AddComponent(enemy, BODY, sizeof(RigidbodyComponent));
    
    create_damager(enemy, (DamagerComponent) {1, 0, false, 0});
    // Initialize components
    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_rigidbody_component(body, 2, 2, 60, 60);
    body->is_dynamic = true;

    init_pathfinding_component(target, pl);

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

uint32_t add_block(float x, float y, SDL_Texture* texture) {
    uint32_t block = ECS_CreateEntity(ECS_GetManager());
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

uint32_t add_chest(float x, float y) {
    uint32_t chest = ECS_CreateEntity(ECS_GetManager());
    PositionComponent* position = ECS_AddComponent(chest, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(chest, SPRITE, sizeof(SpriteComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_sprite_component(sprite, 64, 64, get_sprites()->chest_closed_texture);

    return chest;
}

uint32_t add_blocks(int rX, int rY) {
    for(int x = rX * 1280; x < (rX + 1) * 1280; x += 64) {
        for(int y = rY * 720; y < (rY + 1) * 720; y += 64) {
            if(rand() % 10 == 0) {
                add_block(x, y, get_sprites()->cobble_texture);
            }
        }
    }
    return -1;
}

uint32_t add_teleporter(float x, float y, float xTarget, float yTarget) {
    uint32_t obj = ECS_CreateEntity(ECS_GetManager());
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

// Handle input for player movement
void handle_input_system(SDL_Event* event) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    int dx = 0, dy = 0;
    if(state[SDL_SCANCODE_W]) dy -= 1;
    if(state[SDL_SCANCODE_S]) dy += 1;
    if(state[SDL_SCANCODE_A]) dx -= 1;
    if(state[SDL_SCANCODE_D]) dx += 1;

    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        PlayerMovementComponent* movement = ECS_GetComponent(ECS_GetManager()->entity_ids[i], PLAYER);
        PositionComponent* position = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
        AnimationComponent* anim = ECS_GetComponent(ECS_GetManager()->entity_ids[i], ANIMATION);
        InventoryComponent* inv = ECS_GetComponent(ECS_GetManager()->entity_ids[i], INVENT);

        if (movement && position) {
            static bool is_it_wanting_to_display = false;
            if (state[SDL_SCANCODE_E] && !is_it_wanting_to_display) {
                is_it_wanting_to_display = true;
                inv->isDisplayed = !inv->isDisplayed;
            }
            else if (!state[SDL_SCANCODE_E]) {
                is_it_wanting_to_display = false;
            }
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
void update_systems(uint32_t* entities, int amount, 
                    int** grid, SDL_Rect cam) {
    for(int i = 0; i < amount; i++) {
        u_int32_t id = entities[i];
        
        update_others(id);
        update_pathfinding_system(id, grid, cam);
        
        update_physics(id);
    }
}

// Render all entities
void render_systems(uint32_t* entities, int amount, SDL_Rect cam, SDL_Renderer* renderer) {
    for(int i = 0; i < amount; i++) {
        u_int32_t id = entities[i];
        PathfindingComponent* targetComp = ECS_GetComponent(id, PATHFINDING);

        if(targetComp) {
            PositionComponent* targetPos = ECS_GetComponent(targetComp->target, POSITION);
            if(targetPos) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                for(int j = 0; j < targetComp->path_length - 1; j++) {
                    SDL_RenderDrawRect(renderer, &(SDL_Rect) {
                        64 * targetComp->path[2 * j] + 16, 
                        64 * targetComp->path[2 * j + 1] + 16, 
                        32, 
                        32
                    });
                }
            }
        }
        
        render_component(id, cam, renderer);
    }
}
