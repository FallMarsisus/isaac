#include "systems.h"

// Initialize the game with entities and components
uint32_t initialize_game() {
    uint32_t player = add_player(100, 200);

    for(int i = 0; i < 20; i++) {
        add_blocks(i % 5 - 2, i / 5 - 2);
        add_enemy(random_int(-5000, 5000), random_int(-5000, 5000), player);
        add_chest(random_int(-5000, 5000), random_int(-5000, 5000));
        
        float x1 = random_int(-5000, 5000), y1 = random_int(-5000, 5000), 
              x2 = random_int(-5000, 5000), y2 = random_int(-5000, 5000);
        add_teleporter(x1, y1, x2, y2);
        add_teleporter(x2, y2, x1, y1);
    }

    return player;
}

void free_components() {
    for (size_t i = 0; i < ECS_GetManager()->count; ++i) {
        StateMachineComponent* sm = ECS_GetComponent(ECS_GetManager()->entity_ids[i], STATE_MACHINE);
        if(sm) {
            update_state_machine(sm);
        }

        free_pathfinding_component(ECS_GetComponent(ECS_GetManager()->entity_ids[i], PATHFINDING));
        free_all_other_components(ECS_GetManager()->entity_ids[i]);
        free_all_render_components(ECS_GetManager()->entity_ids[i]);
    }
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

        StateMachineComponent* sm = ECS_GetComponent(id, STATE_MACHINE);
        if(sm) {
            update_state_machine(sm);
        }
        
        update_others(id);
        update_pathfinding_system(id, grid, cam);
        
        update_physics(id);
    }
}

// Render all entities
void render_systems(uint32_t* entities, int amount, SDL_Rect cam, SDL_Renderer* renderer) {
    //for(int i = 0; i < amount; i++) {
    for(int i = 0; i < ECS_GetManager()->count; i++) {
        u_int32_t id = ECS_GetManager()->entity_ids[i];
        //u_int32_t id = entities[i];
        PositionComponent* position = ECS_GetComponent(id, POSITION);
        SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
        if(!position || !sprite) continue;
        if(!(position->x + sprite->width >= cam.x &&
        position->x <= cam.x + cam.w &&
        position->y + sprite->height >= cam.y &&
        position->y <= cam.y + cam.h)) continue;

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
