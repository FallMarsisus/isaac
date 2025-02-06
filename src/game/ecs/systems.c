#include "systems.h"

// Initialize the game with entities and components
uint32_t initialize_game() {
    uint32_t player = add_player(100, 200);
    add_item_entity(300, 300, apple);
    add_chest(200, 500);
    add_enemy(500, 200, player);

    
    /*
    for(int i = 0; i < 30; i++) {
        add_blocks(i % 5 - 2, i / 5 - 2);
        add_enemy(random_int(-5000, 5000), random_int(-5000, 5000), player);
        add_chest(random_int(-5000, 5000), random_int(-5000, 5000));
        
        float x1 = random_int(-5000, 5000), y1 = random_int(-5000, 5000), 
              x2 = random_int(-5000, 5000), y2 = random_int(-5000, 5000);
        add_teleporter(x1, y1, x2, y2);
        add_teleporter(x2, y2, x1, y1);
    }
    */


    return player;
}

void free_components() {
    for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
        Node* current = ECS_GetManager()->st->dict->array[i];
        while (current) {
            StateMachineComponent* sm = ECS_GetComponent(current->key, STATE_MACHINE);
            if(sm) {
                free_state_machine(sm);
            }

            InventoryComponent* invent = ECS_GetComponent(current->key, INVENT);
            if(invent) {
                free_inventory(invent);
            }

            free_pathfinding_component(ECS_GetComponent(current->key, PATHFINDING));
            free_all_other_components(current->key);
            free_all_render_components(current->key);
            current = current->next;
        }
    }
}

void free_one_entity(uint32_t entity) {
    StateMachineComponent* sm = ECS_GetComponent(entity, STATE_MACHINE);
    if(sm) {
        free_state_machine(sm);
    }

    InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
    if(invent) {
        free_inventory(invent);
    }

    free_pathfinding_component(ECS_GetComponent(entity, PATHFINDING));
    free_all_other_components(entity);
    free_all_render_components(entity);

    ECS_RemoveEntity(entity);
}
void handle_input_system(SDL_Event* event) {

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
    for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
        Node* current = ECS_GetManager()->st->dict->array[i];
        while (current) {
            PlayerMovementComponent* movement = ECS_GetComponent(current->key, PLAYER);
            PositionComponent* position = ECS_GetComponent(current->key, POSITION);
            AnimationComponent* anim = ECS_GetComponent(current->key, ANIMATION);
            InventoryComponent* inv = ECS_GetComponent(current->key, INVENT);
            SwordComponent* sword = ECS_GetComponent(current->key, SWORD_C);

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
                    printf("mouse is in slot n° %d\n", onClic(current->key, x, y));
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

            

            current = current->next;
        }
    }
}


uint32_t add_item_entity(float x, float y, ItemData itemType) {
    uint32_t itemEntity = ECS_CreateEntity();
    PositionComponent* position = ECS_AddComponent(itemEntity, POSITION, sizeof(PositionComponent));
    SpriteComponent* sprite = ECS_AddComponent(itemEntity, SPRITE, sizeof(SpriteComponent));
    ItemComponent* itemC = ECS_AddComponent(itemEntity, ITEM, sizeof(ItemComponent));

    position->x = x; position->y = y;
    position->vx = 0; position->vy = 0;
    position->camFixed = false;

    init_sprite_component(sprite, 64, 64, get_texture_from_Id(itemType.id));

    itemC->isGettable = true;
    itemC->item = itemType;

    return itemEntity;
}


void update_elt(uint32_t elt, int** grid, SDL_Rect cam, float delta) {
    StateMachineComponent* sm = ECS_GetComponent(elt, STATE_MACHINE);
    ParentComponent* parent = ECS_GetComponent(elt, PARENT);

    if(sm) {
        update_state_machine(sm);
    }
    
    update_others(elt, cam);
    update_pathfinding_system(elt, grid, cam);
    
    update_physics(elt, delta);

    

    if(parent) {
        for(int i = 0; i < get_ids_len(parent->children); i++) {
            uint32_t id = get_ids(parent->children)[i];
            update_elt(id, grid, cam, delta);
        }
    }
}

// Render all entities
void render_systems(uint32_t* entities, int amount, SDL_Rect cam, SDL_Renderer* renderer) {

    render_background(cam, renderer, get_sprites()->background_texture);
    //for(int i = 0; i < amount; i++) {
    for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
        Node* current = ECS_GetManager()->st->dict->array[i];
        while (current) {    
            u_int32_t id = current->key;
            //u_int32_t id = entities[i];
            PositionComponent* position = ECS_GetComponent(id, POSITION);
            SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
            if(!position || !sprite) {
                current = current->next;
                continue;
            }
            if(!(position->x + sprite->width >= cam.x &&
            position->x <= cam.x + cam.w &&
            position->y + sprite->height >= cam.y &&
            position->y <= cam.y + cam.h)) {
                current = current->next;
                continue;
            }

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
            current = current->next;
        }
    }
}
