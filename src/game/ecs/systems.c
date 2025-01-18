#include "systems.h"

// Load a texture from a file
SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        fprintf(stderr, "Failed to load BMP file: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Initialize the game with entities and components
void initialize_game(ECS_Manager* ecs) {
    // Create a player entity
    uint32_t player = ECS_CreateEntity(ecs);
    PositionComponent* position = ECS_AddComponent(ecs, player, 0, sizeof(PositionComponent));
    RenderComponent* render = ECS_AddComponent(ecs, player, 1, sizeof(RenderComponent));
    SpriteComponent* sprite = ECS_AddComponent(ecs, player, 2, sizeof(SpriteComponent));
    PlayerMovementComponent* movement = ECS_AddComponent(ecs, player, 3, sizeof(PlayerMovementComponent));

    // Initialize components
    position->x = 100;
    position->y = 100;
    position->vx = 0;
    position->vy = 0;
    render->width = 50;
    render->height = 50;
    sprite->texture = get_sprites()->player_texture;
    if (!sprite->texture) {
        fprintf(stderr, "Failed to load player texture.\n");
        exit(1);
    }
    movement->speed = 200; // Pixels per second
}

// Handle input for player movement
void handle_input_system(ECS_Manager* ecs, SDL_Event* event) {
    for (size_t i = 0; i < ecs->count; ++i) {
        PlayerMovementComponent* movement = ECS_GetComponent(ecs, ecs->entity_ids[i], 3);
        PositionComponent* position = ECS_GetComponent(ecs, ecs->entity_ids[i], 0);
        if (movement && position) {
            if (event->type == SDL_KEYDOWN) {
                switch (event->key.keysym.sym) {
                    case SDLK_UP:    position->vy = -movement->speed; break;
                    case SDLK_DOWN:  position->vy = movement->speed;  break;
                    case SDLK_LEFT:  position->vx = -movement->speed; break;
                    case SDLK_RIGHT: position->vx = movement->speed;  break;
                }
            }
            if (event->type == SDL_KEYUP) {
                switch (event->key.keysym.sym) {
                    case SDLK_UP: case SDLK_DOWN:  position->vy = 0; break;
                    case SDLK_LEFT: case SDLK_RIGHT: position->vx = 0; break;
                }
            }
        }
    }
}

// Update all systems
void update_systems(ECS_Manager* ecs) {
    for (size_t i = 0; i < ecs->count; ++i) {
        PositionComponent* position = ECS_GetComponent(ecs, ecs->entity_ids[i], 0);
        if (position) {
            position->x += position->vx * (1.0 / 60.0); // Assuming 60 FPS
            position->y += position->vy * (1.0 / 60.0);
        }
    }
}

// Render all entities
void render_systems(ECS_Manager* ecs, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < ecs->count; ++i) {
        PositionComponent* position = ECS_GetComponent(ecs, ecs->entity_ids[i], 0);
        RenderComponent* render = ECS_GetComponent(ecs, ecs->entity_ids[i], 1);
        SpriteComponent* sprite = ECS_GetComponent(ecs, ecs->entity_ids[i], 2);

        if (position && render && sprite && sprite->texture) {
            SDL_Rect dest = {
                (int)position->x,
                (int)position->y,
                render->width,
                render->height
            };
            SDL_RenderCopy(renderer, sprite->texture, NULL, &dest);
        }
    }

    SDL_RenderPresent(renderer);
}
