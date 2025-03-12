#include "scripts.h"

void init_player(ScriptComponent* script, int win_width, int true_width) {
    PlayerData* data = malloc(sizeof(PlayerData));
    data->speed = 3.5;
    data->direction = (Vector) {0, 0};
	data->win_width = win_width;
	data->true_width = true_width;

    script->data = data;
    script->update = update_player;
}

static void handle_movement_input(int* dx, int* dy) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    *dx = 0;
    *dy = 0;
    if(state[SDL_SCANCODE_W]) *dy -= 1;
    if(state[SDL_SCANCODE_S]) *dy += 1;
    if(state[SDL_SCANCODE_A]) *dx -= 1;
    if(state[SDL_SCANCODE_D]) *dx += 1;
    if(state[SDL_SCANCODE_ESCAPE]) {
        switch_to_menu(MENU_PAUSE);
    }
}

static void handle_inventory_display(InventoryComponent* inv, uint32_t player, SDL_Rect cam, int win_width, int true_width) {
	if (!inv) return;
    
    static bool is_it_wanting_to_display = false;
	static bool is_displaying_in_console = false;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    if (state[SDL_SCANCODE_E] && !is_it_wanting_to_display) {
        is_it_wanting_to_display = true;
        inv->isDisplayed = !inv->isDisplayed;
    }
    else if (!state[SDL_SCANCODE_E]) {
        is_it_wanting_to_display = false;
    }

	if (!is_displaying_in_console && state[SDL_SCANCODE_B]) {
		is_displaying_in_console = true;
		print_inv_debug(inv);
	} else if (!state[SDL_SCANCODE_B]) {
		is_displaying_in_console = false;
	}

	static bool is_j_pressed = false;
	if (state[SDL_SCANCODE_J] && !is_j_pressed) {
		is_j_pressed = true;

		if (inv->items[inv->max_nb_items].id != -1) {
			throwItemAtMouse(player, inv->max_nb_items, cam, win_width, true_width);
		}
		
	} else if (!state[SDL_SCANCODE_J]) {
		is_j_pressed = false;
	}
}

static void handle_mouse_input(uint32_t player) {
    InventoryComponent* inv = ECS_GetComponent(player, INVENT);
    if (!inv) return;
    
    static bool mouseClicked = false;
    int x, y;
    Uint32 mouseState = SDL_GetMouseState(&x, &y);
    
    if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && !mouseClicked) {
        printf("mouse is in slot n° %d\n", on_clic(player, x, y));
        mouseClicked = true;
    } else if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        mouseClicked = false;
    }
}

static void update_movement_and_animation(PlayerData* movement, PositionComponent* position, 
                                       AnimationComponent* anim, int dx, int dy, float distance) {
    if(distance > 0.01) {
        position->vx = (dx / distance) * movement->speed;
        position->vy = (dy / distance) * movement->speed;
        movement->direction = (Vector) {dx / distance, dy / distance};
        
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

static Vector get_4dir_attack_vector(float dx, float dy) {
    Vector direction = {0, 0};
    const float abs_dx = fabsf(dx);
    const float abs_dy = fabsf(dy);

    // Determine primary axis based on greater distance
    if(abs_dx > abs_dy) {
        direction.x = (dx > 0) ? 1 : -1;
    } else {
        direction.y = (dy > 0) ? 1 : -1;
    }
    return direction;
}

static void handle_combat(uint32_t player, PositionComponent* position, 
                          SwordComponent* sword, uint32_t* entities, 
                          int amount, SDL_Rect cam) {
    static bool sword_used = false;
    static int sword_counter = 0;
    static bool mouse_pressed = false;

    int mouse_x, mouse_y;
    Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    // Convert window coordinates to logical coordinates
    float logical_x, logical_y;
    SDL_RenderWindowToLogical(get_renderer(), mouse_x, mouse_y, &logical_x, &logical_y);
    
    SpriteComponent* sprite = ECS_GetComponent(player, SPRITE);
    if(!sprite) return;

    // Handle mouse click attack
    if((mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) && !mouse_pressed) {
        mouse_pressed = true;
        
        if(!sword_used) {
            float dx = (logical_x + cam.x) - (position->x + (sprite->width / 2.0f));
            float dy = (logical_y + cam.y) - (position->y + (sprite->height / 2.0f));

            Vector attack_dir = get_4dir_attack_vector(dx, dy);

            float attack_x = attack_dir.x * 24.0f;
            float attack_y = attack_dir.y * 24.0f;
            
            use_sword(player, attack_x, attack_y);
            
            sword_used = true;
            sword_counter = 0;
        }
    }
    else if(!(mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        mouse_pressed = false;
    }

    // Sword cooldown
    if(sword_used) {
        sword_counter++;
        if(sword_counter >= 15) {  // ~0.25 seconds at 60 FPS
            sword_used = false;
            sword_counter = 0;
        }
    }
}

void update_player(u_int32_t player, SDL_Rect cam, uint32_t* entities, int amount) {
    ScriptComponent* script = ECS_GetComponent(player, SCRIPT);
    if(!script) return;

    PlayerData* movement = (PlayerData*)script->data;
    if(!movement) return;

    PositionComponent* position = ECS_GetComponent(player, POSITION);
    if(!position) return;

    AnimationComponent* anim = ECS_GetComponent(player, ANIMATION);
    InventoryComponent* inv = ECS_GetComponent(player, INVENT);
    SwordComponent* sword = ECS_GetComponent(player, SWORD_C);

    int dx = 0, dy = 0;
    handle_movement_input(&dx, &dy);
    float distance = sqrt(dx*dx + dy*dy);
    
    update_movement_and_animation(movement, position, anim, dx, dy, distance);

    if(inv) {
        handle_inventory_display(inv, player, cam, movement->win_width, movement->true_width);
        handle_mouse_input(player);
    }

    if(sword) {
        handle_combat(player, position, sword, entities, amount, cam);
    }
}

