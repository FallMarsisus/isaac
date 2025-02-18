#include "systems.h"

// Initialize the game with entities and components
void init_room(int rX, int rY, uint32_t player_id) {
    generate_room(rX, rY, player_id);
}

void free_components() {
    for(Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
        free_one_entity(e);
    }
}

void free_one_entity(uint32_t entity) {
    StateMachineComponent* sm = ECS_GetComponent(entity, STATE_MACHINE);
    if(sm) {
        free_state_machine(sm);
        ECS_ClearComponent(entity, STATE_MACHINE);
    }

    InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
    if(invent) {
        free_inventory(invent);
        ECS_ClearComponent(entity, INVENT);
    }

    free_pathfinding_component(ECS_GetComponent(entity, PATHFINDING));
    free_all_other_components(entity);
    free_all_render_components(entity);
    
    ECS_RemoveEntity(entity);
}

void handle_input_system(SDL_Event* event, uint32_t player) {
    static bool is_use_sword = false;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    int dx = 0, dy = 0;
    if(state[SDL_SCANCODE_W]) dy -= 1;
    if(state[SDL_SCANCODE_S]) dy += 1;
    if(state[SDL_SCANCODE_A]) dx -= 1;
    if(state[SDL_SCANCODE_D]) dx += 1;
    if(state[SDL_SCANCODE_ESCAPE]) {
        SDL_Event quit;
        quit.type = SDL_QUIT;
        SDL_PushEvent(&quit);
    }
    
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));

    PlayerMovementComponent* movement = ECS_GetComponent(player, PLAYER);
    PositionComponent* position = ECS_GetComponent(player, POSITION);
    AnimationComponent* anim = ECS_GetComponent(player, ANIMATION);
    InventoryComponent* inv = ECS_GetComponent(player, INVENT);
    SwordComponent* sword = ECS_GetComponent(player, SWORD_C);

    if (movement && position) {
        static bool is_it_wanting_to_display = false;
        if (state[SDL_SCANCODE_E] && !is_it_wanting_to_display) {
            is_it_wanting_to_display = true;
            inv->isDisplayed = !inv->isDisplayed;
        }
        else if (!state[SDL_SCANCODE_E]) {
            is_it_wanting_to_display = false;
        }

        // temp
        static bool mouseClicked = false;
        int x, y;
        Uint32 mouseState = SDL_GetMouseState(&x, &y);
        
        if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && !mouseClicked && inv != NULL) {
            printf("mouse is in slot n° %d\n", onClic(player, x, y));
            mouseClicked = true;
        } else if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
            mouseClicked = false;
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

void update_elt(uint32_t elt, uint32_t* entities, int amount, SDL_Rect roomPos, float delta) {
    StateMachineComponent* sm = ECS_GetComponent(elt, STATE_MACHINE);
    ParentComponent* parent = ECS_GetComponent(elt, PARENT);

    if(sm) {
        update_state_machine(sm);
    }
    
    update_others(elt, roomPos);
    //update_pathfinding_system(elt, roomPos);
    update_item(elt);
    update_item(elt);
    update_physics(elt, delta);
}

// Render all entities
void render_systems(uint32_t* entities, int amount, SDL_Rect cam, SDL_Renderer* renderer) {
    render_background(cam, renderer, get_sprites()->background_texture);
    //for(int i = 0; i < amount; i++) {
    for(Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
        //u_int32_t id = entities[i];
        u_int32_t id = e;
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
                        targetComp->path[2 * j] - cam.x + 16,
                        targetComp->path[2 * j + 1] - cam.y + 16,
                        32, 32
                    });
                }
            }
        }
        
        render_component(id, cam, renderer);
    }
}
