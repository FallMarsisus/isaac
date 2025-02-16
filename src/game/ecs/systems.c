#include "systems.h"

// Initialize the game with entities and components
void init_room(int rX, int rY, uint32_t player) {
    int start_x = rX * 1920; int start_y = rY * 1080;
    
    // Fixed door positions - centered on each wall
    int north_door = 14; // (1920/64)/2 - 1 = 14 (center of north wall)
    int south_door = 14; // Same as north for symmetry
    int east_door = 8;   // (1080/64)/2 - 1 = 8 (center of east wall)
    int west_door = 8;   // Same as east for symmetry
    
    // Add outer walls with fixed holes in each direction
    for(int x = 0; x < 30; x++) {
        // Skip north door (2 blocks wide hole)
        if(x < north_door || x >= north_door + 2) {
            add_block(start_x + x * 64, start_y, get_sprites()->cobble_texture);
        }
        // Skip south door (2 blocks wide hole)
        if(x < south_door || x >= south_door + 2) {
            add_block(start_x + x * 64, start_y + 1024, get_sprites()->cobble_texture);
        }
    }
    for(int y = 0; y < 17; y++) {
        // Skip west door (2 blocks tall hole)
        if(y < west_door || y >= west_door + 2) {
            add_block(start_x, start_y + y * 64, get_sprites()->cobble_texture);
        }
        // Skip east door (2 blocks tall hole)
        if(y < east_door || y >= east_door + 2) {
            add_block(start_x + 1856, start_y + y * 64, get_sprites()->cobble_texture);
        }
    }
    
    // Add random decorative elements
    int item_x = start_x + ((4 + rand() % 20) * 64); // Between 4-24 blocks in
    int item_y = start_y + ((4 + rand() % 12) * 64); // Between 4-16 blocks in
    add_item_entity(item_x, item_y, apple, -1, false);
    
    int chest_x = start_x + ((3 + rand() % 24) * 64); // Between 3-27 blocks in
    int chest_y = start_y + ((7 + rand() % 4) * 64); // Between 7-11 blocks in
    add_chest(chest_x, chest_y);
    
    // Add multiple slimes in random positions
    int num_slimes = rand() % 3 + 1; // 1-3 slimes
    for(int i = 0; i < num_slimes; i++) {
        int slime_x = start_x + ((3 + rand() % 24) * 64);
        int slime_y = start_y + ((3 + rand() % 11) * 64);
        add_slime(slime_x, slime_y, player, NULL);
    }
    
    // Create a more complex room layout with pillars and inner walls
    // Add corner pillars
    add_block(start_x + 128, start_y + 128, get_sprites()->cobble_texture);
    add_block(start_x + 1728, start_y + 128, get_sprites()->cobble_texture);
    add_block(start_x + 128, start_y + 896, get_sprites()->cobble_texture);
    add_block(start_x + 1728, start_y + 896, get_sprites()->cobble_texture);
    
    // Add random inner structures
    int num_structures = rand() % 4 + 2;
    for(int i = 0; i < num_structures; i++) {
        int struct_x = start_x + ((4 + rand() % 22) * 64); // Keep away from walls
        int struct_y = start_y + ((4 + rand() % 11) * 64);
        
        // Create L-shaped walls
        for(int j = 0; j < 3; j++) {
            add_block(struct_x + j * 64, struct_y, get_sprites()->cobble_texture);
            add_block(struct_x, struct_y + j * 64, get_sprites()->cobble_texture);
        }
    }
    
    // Add some teleporters with random destinations within the room
    if(rand() % 3 == 0) { // 33% chance for teleporters
        int x1 = start_x + ((6 + rand() % 18) * 64);
        int y1 = start_y + ((6 + rand() % 5) * 64);
        int x2 = start_x + ((6 + rand() % 18) * 64);
        int y2 = start_y + ((6 + rand() % 5) * 64);
        add_teleporter(x1, y1, x2, y2);
    }
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
