#include "systems.h"

// Initialize the game with entities and components
void init_room(int x, int y, uint32_t player) {
    add_item_entity(300 + 1280 * x, 300 + 720 * y, apple, -1, false);
    add_chest(200 + 1280 * x, 500 + 720 * y);
    add_slime(500 + 1280 * x, 200 + 720 * y, player, NULL);
    add_blocks(x, y);
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

// uint32_t add_item_entity(float x, float y, ItemData itemType) {
//     uint32_t itemEntity = ECS_CreateEntity();
//     PositionComponent* position = ECS_AddComponent(itemEntity, POSITION, sizeof(PositionComponent));
//     SpriteComponent* sprite = ECS_AddComponent(itemEntity, SPRITE, sizeof(SpriteComponent));
//     ItemComponent* itemC = ECS_AddComponent(itemEntity, ITEM, sizeof(ItemComponent));
//     RigidbodyComponent* body = ECS_AddComponent(itemEntity, BODY, sizeof(RigidbodyComponent));

//     position->x = x; position->y = y;
//     position->vx = 0; position->vy = 0;
//     position->camFixed = false;

//     init_sprite_component(sprite, 64, 64, get_texture_from_Id(itemType.id));
//     init_rigidbody_component(body, 0, 0, 64, 64);
//     body->is_dynamic = true;

//     itemC->isGettable = true;
//     itemC->item = itemType;

//     return itemEntity;
// }

void update_elt(uint32_t elt, int** grid, uint32_t* entities, int amount, SDL_Rect roomPos, float delta) {
    StateMachineComponent* sm = ECS_GetComponent(elt, STATE_MACHINE);
    ParentComponent* parent = ECS_GetComponent(elt, PARENT);

    if(sm) {
        update_state_machine(sm);
    }
    
    update_others(elt, roomPos);
    update_pathfinding_system(elt, grid, roomPos);
    update_item(elt);
    update_item(elt);
    update_physics(elt, delta);
    
    if(parent) {
        for(int i = 0; i < get_ids_len(parent->children); i++) {
            uint32_t id = get_ids(parent->children)[i];
            update_elt(id, grid, entities, amount, roomPos, delta);
        }
    }
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
