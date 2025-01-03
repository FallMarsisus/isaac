#include "player.h"

player* create_player(int x, int y) {
    player* p = malloc(sizeof(player));
    p->body = create_entity(x, y, 32, 32);
    p->body->speed = 2;

    p->keys = malloc(sizeof(bool) * 4);
    for(int i = 0; i < 4; i ++) {
        p->keys[i] = false;
    }
    return p;
}

void load_player_textures(player* p, SDL_Renderer* ren, char* path) {
    p->core = create_core(ren, path, 16, 16);
    printf("Y\n");
    fflush(stdout);
    
    add_anim(p->core, 0, 0.1, 4);
    add_anim(p->core, 1, 0.1, 4);
    add_anim(p->core, 2, 0.1, 2);
    add_anim(p->core, 3, 0.1, 2);
    
    set_active_anim(p->core, 0);
}

void free_player(player* p) {
    free_core(p->core);
    free_entity(p->body);
    free(p->keys);
    free(p);
}

//Maps the keys to the keys array
void get_inputs(player* p) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    p->keys[0] = state[SDL_SCANCODE_UP];
    p->keys[1] = state[SDL_SCANCODE_DOWN];
    p->keys[2] = state[SDL_SCANCODE_LEFT];
    p->keys[3] = state[SDL_SCANCODE_RIGHT];
}

//Moves the player
void move(player* p) {
    p->body->vel->x = 0; p->body->vel->y = 0;

    if(p->keys[0]) p->body->vel->y -= 1;
    if(p->keys[1]) p->body->vel->y += 1;
    if(p->keys[2]) p->body->vel->x -= 1;
    if(p->keys[3]) p->body->vel->x += 1;
}

void update_player_sprite(player* p) {
    bool anim = true;
    if(p->body->vel->y < -0.1) set_active_anim(p->core, 1);
    else if(p->body->vel->y > 0.1) set_active_anim(p->core, 0);
    else if(p->body->vel->x < -0.1) set_active_anim(p->core, 2);
    else if(p->body->vel->x > 0.1) set_active_anim(p->core, 3);
    else {
        stop_anim(p->core);
        anim = false;
    }
    if(anim) play_anim(p->core);
}

//Do the whole shit
void update_player(player* p, int win_width, int win_height) {
    move(p);

    update_entity(p->body);

    update_player_sprite(p);
}

void draw_player(SDL_Renderer* ren, player* p) {
    draw_core(ren, p->body->hitbox, p->core);
}