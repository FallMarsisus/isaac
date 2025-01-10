#include "player.h"

player* create_player(int x, int y, sprite_list* sprites) {
    player* p = malloc(sizeof(player));
    p->body = create_entity(x, y, 32, 32, sprites);
    p->body->sprites = sprites;
    p->body->speed = 2;

    p->keys = malloc(sizeof(bool) * 4);
    for(int i = 0; i < 4; i ++) {
        p->keys[i] = false;
    }
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

    update_player_sprite(p);
}

void draw_player(SDL_Renderer* ren, player* p) {
    draw_entity(p->body, ren);
}