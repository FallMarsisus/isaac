#include "player.h"

void set_teleport(void* cc) {((player*) cc)->can_teleport = true;}
void stop_dash(void* cc) {
    ((player*) cc)->is_dashing = false;
    ((player*) cc)->body->speed = 2;
    ((player*) cc)->can_dash = false;
    play_timer(((player*) cc)->dash_cooldown, 1);
}
void set_dashable(void* cc) {((player*) cc)->can_dash = true;}

player* create_player(int x, int y, sprite_list* sprites) {
    player* p = malloc(sizeof(player));
    p->body = create_entity(x, y, 32, 32, sprites);
    p->body->sprites = sprites;
    p->body->speed = 2;
    p->inv = create_inventory(10);
    item* new_item = create_item("New Item", 1);
    add_item(p->inv, new_item);

    p->keys = malloc(sizeof(bool) * 4);
    for(int i = 0; i < 4; i ++) {
        p->keys[i] = false;
    }
    p->open_inv = false;

    p->can_teleport = true;
    p->teleport_timer = create_timer(set_teleport, p);

    p->running = false;

    p->is_dashing = false;
    p->dash_timer = create_timer(stop_dash, p);
    p->can_dash = true;
    p->dash_cooldown = create_timer(set_dashable, p);
    return p;
}
void load_player_textures(player* p, SDL_Renderer* ren) {
    p->body->core = create_core(ren, p->body->sprites->player_texture, 16, 16);
    
    add_anim(p->body->core, 0, 0.1, 4);
    add_anim(p->body->core, 1, 0.1, 4);
    add_anim(p->body->core, 2, 0.1, 2);
    add_anim(p->body->core, 3, 0.1, 2);
    
    set_active_anim(p->body->core, 0);
}
void free_player(player* p) {
    free_entity(p->body);
    free_timer(p->teleport_timer);
    free(p->keys);
    free(p);
}

//Maps the keys to the keys array
void get_inputs(player* p) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    p->keys[0] = state[SDL_SCANCODE_W];
    p->keys[1] = state[SDL_SCANCODE_S];
    p->keys[2] = state[SDL_SCANCODE_A];
    p->keys[3] = state[SDL_SCANCODE_D];

    static bool e_was_pressed = false;

    if(state[SDL_SCANCODE_E]) {
        if(!e_was_pressed && p->can_teleport && !p->is_dashing) {
            p->open_inv = !p->open_inv;
            p->body->can_move = !p->body->can_move;
        }
        e_was_pressed = true;
    } else {
        e_was_pressed = false;
    }

    if(state[SDL_SCANCODE_LCTRL] && !p->running && !p->is_dashing) {
        p->body->speed = 3.5;
        p->running = true;
    }
    else if (!state[SDL_SCANCODE_LCTRL] && p->running) {
        p->body->speed = 2;
        p->running = false;
    }

    if(state[SDL_SCANCODE_LSHIFT] && !p->is_dashing && p->can_dash) {
        p->is_dashing = true;
        p->body->speed = 20;
        play_timer(p->dash_timer, 0.1);
    }
}

//Moves the player
void move(player* p) {
    p->body->vel->x = 0; p->body->vel->y = 0;

    if(p->body->can_move) {
        if(p->keys[0]) p->body->vel->y -= 1;
        if(p->keys[1]) p->body->vel->y += 1;
        if(p->keys[2]) p->body->vel->x -= 1;
        if(p->keys[3]) p->body->vel->x += 1;
    }
}
void update_player_sprite(player* p) {
    bool anim = true;
    if(p->body->vel->y < -0.1) set_active_anim(p->body->core, 1);
    else if(p->body->vel->y > 0.1) set_active_anim(p->body->core, 0);
    else if(p->body->vel->x < -0.1) set_active_anim(p->body->core, 2);
    else if(p->body->vel->x > 0.1) set_active_anim(p->body->core, 3);
    else {
        stop_anim(p->body->core);
        anim = false;
    }
    if(anim) play_anim(p->body->core);
}

//Do the whole shit
void update_player(player* p, chained_list* entities, chained_list* tiles) {
    move(p);

    update_entity(p->body, NULL, entities, tiles);

    update_timer(p->teleport_timer);
    update_timer(p->dash_timer);
    update_timer(p->dash_cooldown);

    update_player_sprite(p);
}

void draw_player(SDL_Renderer* ren, player* p) {
    draw_entity(p->body, ren);

}