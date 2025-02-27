#include "scripts.h"

void init_player(ScriptComponent* script) {
    PlayerData* data = malloc(sizeof(PlayerData));
    data->speed =6;
    data->direction = (Vector) {0, 0};

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
        SDL_Event quit;
        quit.type = SDL_QUIT;
        SDL_PushEvent(&quit);
    }
}

static void handle_inventory_display(InventoryComponent* inv) {
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
}

static void handle_mouse_input(uint32_t player) {
    static bool mouseClicked = false;
    int x, y;
    Uint32 mouseState = SDL_GetMouseState(&x, &y);
    InventoryComponent* inv = ECS_GetComponent(player, INVENT);
    
    if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && !mouseClicked && inv != NULL) {
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

static void handle_combat(uint32_t player, PositionComponent* position, SwordComponent* sword, uint32_t* entities, int amount) {
    static bool attacked = false;
    static bool sword_used = false;
    static int sword_counter = 0;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    uint32_t nearest_enemy = get_nearest_enemy(player, entities, amount);

    if (sword_used) {
        sword_counter++;
        if (sword_counter >= 20) {
            sword_used = false;
            sword_counter = 0;
        }
    }

    if (state[SDL_SCANCODE_LSHIFT] && !sword_used) {
        use_sword(player, nearest_enemy);
        sword_used = true;
        sword_counter = 0;

        if (nearest_enemy != -1 && is_colliding_with_enemy(player, entities, amount) && !attacked) {
            attacked = true;
            if (apply_damage(nearest_enemy, player) == false) {
                printf("ERROR : Player not found\n");
            } else {
                printf("Player is taking damage from entity %d\n", nearest_enemy);
            }
        }
    }
    
    if (!is_colliding_with_enemy(player, entities, amount)) {
        attacked = false;
    }
}

void update_player(u_int32_t player, SDL_Rect cam, uint32_t* entities, int amount) {
    ScriptComponent* script = ECS_GetComponent(player, SCRIPT);
    if(!script) return;

    int dx = 0, dy = 0;
    handle_movement_input(&dx, &dy);
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));

    PlayerData* movement = (PlayerData*)script->data;
    PositionComponent* position = ECS_GetComponent(player, POSITION);
    AnimationComponent* anim = ECS_GetComponent(player, ANIMATION);
    InventoryComponent* inv = ECS_GetComponent(player, INVENT);
    SwordComponent* sword = ECS_GetComponent(player, SWORD_C);

    if (movement && position) {
        handle_inventory_display(inv);
        handle_mouse_input(player);
        update_movement_and_animation(movement, position, anim, dx, dy, distance);
    }

    if (position && sword) {
        handle_combat(player, position, sword, entities, amount);
    }
}