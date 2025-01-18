#include "systems.h"

// Initialize the game with entities and components
void initialize_game(ECS_Manager* ecs) {
    uint32_t player = add_player(ecs);
    add_enemy(ecs, player);
}

uint32_t add_player(ECS_Manager* ecs) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity(ecs);
    PositionComponent* position = ECS_AddComponent(ecs, player, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(ecs, player, SPRITE, sizeof(SpriteComponent));
    PlayerMovementComponent* movement = ECS_AddComponent(ecs, player, PLAYER, sizeof(PlayerMovementComponent));

    // Initialize components
    position->x = 100; position->y = 100;
    position->vx = 0; position->vy = 0;

    sprite->width = 50; sprite->height = 50;
    sprite->texture = get_sprites()->player_texture;
    if (!sprite->texture) {
        fprintf(stderr, "Failed to load player texture.\n");
        exit(1);
    }

    movement->speed = 2; // Pixels per second

    return player;
}

uint32_t add_enemy(ECS_Manager* ecs, uint32_t player) {
    uint32_t enemy = ECS_CreateEntity(ecs);
    PositionComponent* position = ECS_AddComponent(ecs, enemy, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(ecs, enemy, SPRITE, sizeof(SpriteComponent));
    TargetMovementComponent* target = ECS_AddComponent(ecs, enemy, TARGET, sizeof(TargetMovementComponent));

    // Initialize components
    target->entity = player; target->speed = 1;

    position->x = 200; position->y = 200;
    position->vx = 0; position->vy = 0;

    sprite->width = 50; sprite->height = 50;
    sprite->texture = get_sprites()->goblin_texture;
    if (!sprite->texture) {
        fprintf(stderr, "Failed to load player texture.\n");
        exit(1);
    }
    return enemy;
}

// Handle input for player movement
void handle_input_system(ECS_Manager* ecs, SDL_Event* event) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    for (size_t i = 0; i < ecs->count; ++i) {

        PlayerMovementComponent* movement = ECS_GetComponent(ecs, ecs->entity_ids[i], 3);
        PositionComponent* position = ECS_GetComponent(ecs, ecs->entity_ids[i], 0);

        if (movement && position) {
            position->vy = 0; position->vx = 0;
            if(state[SDL_SCANCODE_W]) position->vy -= 1;
            if(state[SDL_SCANCODE_S]) position->vy += 1;
            if(state[SDL_SCANCODE_A]) position->vx -= 1;
            if(state[SDL_SCANCODE_D]) position->vx += 1;

            float distance = sqrt(pow(position->vx, 2) + pow(position->vy, 2));
            if(distance > 0.01) {
                position->vx = (position->vx / distance) * movement->speed;
                position->vy = (position->vy / distance) * movement->speed;
            }
        }
    }
}

// Update all systems
void update_systems(ECS_Manager* ecs) {
    for (size_t i = 0; i < ecs->count; ++i) {
        PositionComponent* position = ECS_GetComponent(ecs, ecs->entity_ids[i], POSITION);
        if (position) {

            TargetMovementComponent* targetComp = ECS_GetComponent(ecs, ecs->entity_ids[i], TARGET);
            if(targetComp) {
                PositionComponent* target_pos = ECS_GetComponent(ecs, targetComp->entity, POSITION);
                if(target_pos) {
                    float dx = target_pos->x - position->x;
                    float dy = target_pos->y - position->y;
                    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
                    position->vx = (dx / distance) * targetComp->speed;
                    position->vy = (dy / distance) * targetComp->speed;
                }
            }
            
            position->x += position->vx;
            position->y += position->vy;
        }
    }
}

// Render all entities
void render_systems(ECS_Manager* ecs, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < ecs->count; ++i) {
        PositionComponent* position = ECS_GetComponent(ecs, ecs->entity_ids[i], 0);
        SpriteComponent* sprite = ECS_GetComponent(ecs, ecs->entity_ids[i], 2);

        if (position && sprite && sprite->texture) {
            SDL_Rect dest = {
                (int)position->x,
                (int)position->y,
                sprite->width,
                sprite->height
            };
            SDL_RenderCopy(renderer, sprite->texture, NULL, &dest);
        }
    }

    SDL_RenderPresent(renderer);
}
